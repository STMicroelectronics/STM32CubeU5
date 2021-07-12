/*
 * Copyright (c) 2018-2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
#include <inttypes.h>
#include "tfm_arch.h"
#include "tfm_thread.h"
#include "tfm_utils.h"
#include "tfm_memory_utils.h"
#include "tfm_svc.h"
#include "spm_api.h"
#include "tfm_core_utils.h"

/* Force ZERO in case ZI(bss) clear is missing */
static struct tfm_thrd_ctx *p_thrd_head = NULL;
static struct tfm_thrd_ctx *p_runn_head = NULL;
static struct tfm_thrd_ctx *p_curr_thrd = NULL;

/* Define Macro to fetch global to support future expansion (PERCPU e.g.) */
#define LIST_HEAD   p_thrd_head
#define RUNN_HEAD   p_runn_head
#define CURR_THRD   p_curr_thrd

static struct tfm_thrd_ctx *find_next_running_thread(struct tfm_thrd_ctx *pth)
{
    while (pth && pth->status != THRD_STAT_RUNNING) {
        pth = pth->next;
    }

    return pth;
}

/* To get next running thread for scheduler */
struct tfm_thrd_ctx *tfm_thrd_next_thread(void)
{
    /*
     * First RUNNING thread has highest priority since threads are sorted with
     * priority.
     */
    return find_next_running_thread(RUNN_HEAD);
}

/* To get current thread for caller */
struct tfm_thrd_ctx *tfm_thrd_curr_thread()
{
    return CURR_THRD;
}

/* Insert a new thread into list by descending priority (Highest at head) */
static void insert_by_prior(struct tfm_thrd_ctx **head,
                            struct tfm_thrd_ctx *node)
{
    if (*head == NULL || (node->prior <= (*head)->prior)) {
        node->next = *head;
        *head = node;
    } else {
        struct tfm_thrd_ctx *iter = *head;

        while (iter->next && (node->prior > iter->next->prior)) {
            iter = iter->next;
        }
        node->next = iter->next;
        iter->next = node;
    }
}

/*
 * Set first running thread as head to reduce enumerate
 * depth while searching for a first running thread.
 */
static void update_running_head(struct tfm_thrd_ctx **runn,
                                struct tfm_thrd_ctx *node)
{
    if ((node->status == THRD_STAT_RUNNING) &&
        (*runn == NULL || (node->prior < (*runn)->prior))) {
        *runn = node;
    } else {
        *runn = LIST_HEAD;
    }
}

/* Set context members only. No validation here */
void tfm_thrd_init(struct tfm_thrd_ctx *pth,
                   tfm_thrd_func_t pfn, void *param,
                   uintptr_t sp_btm, uintptr_t sp_top)
{
    pth->prior = THRD_PRIOR_MEDIUM;
    pth->status = THRD_STAT_CREATING;
    pth->pfn = pfn;
    pth->param = param;
    pth->sp_btm = sp_btm;
    pth->sp_top = sp_top;
}

__attribute__((section("SFN")))
static void exit_zone(void)
{
    tfm_thrd_exit();
}

static void tfm_thrd_initialize_context(struct tfm_state_context *ctx,
                                        void *param, uintptr_t pfn,
                                        uintptr_t sp_btm, uintptr_t sp_top)
{
    /*
     * For security consideration, set unused registers into ZERO;
     * and only necessary registers are set here.
     */
    struct tfm_state_context_t *p_ctxa =
                            (struct tfm_state_context_t *)sp_btm;

    /*
     * Shift back SP to leave space for holding base context
     * since thread is kicked off through exception return.
     */
    p_ctxa--;

    /* Basic context is considerate at thread start.*/
    tfm_core_util_memset(p_ctxa, 0, sizeof(*p_ctxa));
    p_ctxa->r0 = (uint32_t)param;
    p_ctxa->ra = (uint32_t)pfn;
    p_ctxa->lr = (uint32_t)exit_zone;
    p_ctxa->xpsr = XPSR_T32;

    tfm_core_util_memset(ctx, 0, sizeof(*ctx));

    tfm_arch_initialize_ctx_ext(&ctx->ctxb, (uint32_t)p_ctxa, (uint32_t)sp_top);
}

uint32_t tfm_thrd_start(struct tfm_thrd_ctx *pth)
{
    /* Validate parameters before really start */
    if ((pth->status != THRD_STAT_CREATING) ||
        (pth->pfn == NULL)                  ||
        (pth->sp_btm == 0)              ||
        (pth->sp_top == 0)) {
        return THRD_ERR_INVALID_PARAM;
    }

    /* Thread management runs in handler mode; set context for thread mode. */
    tfm_thrd_initialize_context(&pth->state_ctx,
                                pth->param, (uintptr_t)pth->pfn,
                                pth->sp_btm, pth->sp_top);

    /* Insert a new thread with priority */
    insert_by_prior(&LIST_HEAD, pth);

    /* Mark it as RUNNING after insertion */
    tfm_thrd_set_status(pth, THRD_STAT_RUNNING);

    return THRD_SUCCESS;
}

void tfm_thrd_set_status(struct tfm_thrd_ctx *pth, uint32_t new_status)
{
    TFM_ASSERT(pth != NULL && new_status < THRD_STAT_INVALID);

    pth->status = new_status;
    update_running_head(&RUNN_HEAD, pth);
}

/* Scheduling won't happen immediately but after the exception returns */
void tfm_thrd_activate_schedule(void)
{
    tfm_arch_trigger_pendsv();
}

void tfm_thrd_start_scheduler(struct tfm_thrd_ctx *pth)
{
    /*
     * There is no selected thread before scheduler start, assign
     * a caller provided thread as current thread. This function
     * should get called only ONCE; further calling triggers assert.
     */
    TFM_ASSERT(CURR_THRD == NULL);
    TFM_ASSERT(pth != NULL);

    CURR_THRD = pth;
    tfm_thrd_activate_schedule();
}

/* Remove current thread out of the schedulable list */
void tfm_svcall_thrd_exit(void)
{
    CURR_THRD->status = THRD_STAT_DETACH;
    tfm_arch_trigger_pendsv();
}

__attribute__((section("SFN")))
void tfm_thrd_exit(void)
{
    SVC(TFM_SVC_EXIT_THRD);
    while (1) {
        ;
    }
}

void tfm_thrd_context_switch(struct tfm_state_context_ext *ctxb,
                             struct tfm_thrd_ctx *prev,
                             struct tfm_thrd_ctx *next)
{
    TFM_ASSERT(prev != NULL);
    TFM_ASSERT(next != NULL);

    /*
     * First, update latest context into the current thread context.
     * Then, update background context with next thread's context.
     */
    tfm_core_util_memcpy(&prev->state_ctx.ctxb, ctxb, sizeof(*ctxb));
    tfm_core_util_memcpy(ctxb, &next->state_ctx.ctxb,
                         sizeof(next->state_ctx.ctxb));

    /* Update current thread indicator */
    CURR_THRD = next;
}
