/**
  ******************************************************************************
  * @file    net_os.c
  * @author  MCD Application Team
  * @brief   OS needed functions implementation
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include <stdarg.h>
#include <stdint.h>
#include <inttypes.h>

#include "net_conf.h"
#include "net_connect.h"
#include "net_internals.h"
#include "net_perf.h"
#include "net_mem.h"


#ifdef NET_USE_RTOS
#include "task.h"

#if defined(NET_PERF_TASK)
static const char *GetTaskName(TaskHandle_t xHandle);

#define TAG_TASK_ID                                  7U
#define MERGE_TASKID_AND_TIME(TASK_NUM, TIME_VALUE)  (((TASK_NUM) & 7U) + ((TIME_VALUE)<<3U))
#define EXTRACT_TASK_ID(VALUE)                       ((VALUE) & 7U)
#define EXTRACT_DTIME(VALUE)                         ((VALUE)>>3U)
#endif /* NET_PERF_TASK */


#define OSSEMAPHOREDECLARE(A)              osSemaphoreId A
#define OSSEMAPHORE(NAME)                  osSemaphore(NAME)

#if (osCMSIS < 0x20000U)
#define OSSEMAPHOREINIT(A, COUNT, OPTION)  A = osSemaphoreCreate((OPTION), (COUNT))
#else
#define OSSEMAPHOREINIT(A, COUNT, OPTION)  A = osSemaphoreNew((COUNT), (COUNT), NULL)
#endif /* (osCMSIS < 0x20000U) */

#if (osCMSIS >= 0x20000U)
#define OSSEMAPHOREWAIT(A, TIMEOUT)        osSemaphoreAcquire((A), (TIMEOUT))
#else
#define OSSEMAPHOREWAIT(A, TIMEOUT)        osSemaphoreWait((A), (TIMEOUT))
#endif /* osCMSIS */

#define OSSEMAPHORESIGNAL(A)               osSemaphoreRelease((A))

#define OSSEMAPHOREDEINIT(A)               osSemaphoreDelete((A))


extern void *pxCurrentTCB;

#if (osCMSIS < 0x20000U)
static osSemaphoreDef_t NetMutex_def[NET_LOCK_NUMBER] = {0};
#endif /* (osCMSIS < 0x20000U) */

static OSSEMAPHOREDECLARE(NetMutex[NET_LOCK_NUMBER]);


void net_init_locks(void)
{
#ifdef NET_MBEDTLS_HOST_SUPPORT
  net_tls_init();
#endif /* NET_MBEDTLS_HOST_SUPPORT */

  for (int32_t i = 0; i < NET_LOCK_NUMBER; i++)
  {
    OSSEMAPHOREINIT(NetMutex[i], 1, &NetMutex_def[i]);
    NET_ASSERT(NetMutex[i] != NULL, "Failed on mutex creation\n");
  }
}


void net_destroy_locks(void)
{
#ifdef NET_MBEDTLS_HOST_SUPPORT
  net_tls_destroy();
#endif /* NET_MBEDTLS_HOST_SUPPORT */

  for (int32_t i = 0; i < NET_LOCK_NUMBER; i++)
  {
    (void)OSSEMAPHOREDEINIT(NetMutex[i]);
  }
}


void net_lock(int32_t sock, uint32_t timeout_in)
{
  int32_t ret;
  uint32_t timeout = timeout_in;
  if (timeout == NET_OS_WAIT_FOREVER)
  {
    /*MISRA issue so hand coded  osWaitForever*/
    timeout = 0xffffffffU;
  }
  ret = (int32_t) OSSEMAPHOREWAIT(NetMutex[sock], timeout);
  NET_ASSERT(ret == 0, "Failed locking mutex\n");
}


void net_unlock(int32_t sock)
{
  const int32_t ret = (int32_t)OSSEMAPHORESIGNAL(NetMutex[sock]);
  NET_ASSERT(ret == 0, "Failed unlocking mutex\n");
}


void net_lock_nochk(int32_t sock, uint32_t timeout_in)
{
  uint32_t timeout = timeout_in;
  if (timeout == NET_OS_WAIT_FOREVER)
  {
    /* MISRA issue so hand coded  t osWaitForever*/
    timeout = 0xffffffffU;
  }
  (void) OSSEMAPHOREWAIT(NetMutex[sock], timeout);
}

void net_unlock_nochk(int32_t sock)
{
  (void) OSSEMAPHORESIGNAL(NetMutex[sock]);
}


#if !defined(NET_ALLOC_DEBUG)
#define MAX_ALLOC_VALUE 0x4000U

void *net_calloc(size_t num, size_t size)
{
  void *p = NULL;

  if ((num < MAX_ALLOC_VALUE) && (size < MAX_ALLOC_VALUE))
  {
    p = pvPortMalloc(num * size);
  }
  if (p != NULL)
  {
    (void) memset(p, 0, num * size);
  }
  return p;
}


void *net_realloc(void *ptr, size_t size)
{
  void *ret;
  if (ptr == NULL)
  {
    if (size != (size_t) 0)
    {
      ret = pvPortMalloc(size);
      if (ret == NULL)
      {
        /* to avoid erroneous MISRA detection */
        ret = NULL;
      }
    }
    else
    {
      ret = NULL;
    }
  }
  else
  {
    if (size != (size_t) 0)
    {
      void *new_ptr;
      new_ptr = pvPortMalloc(size);
      if (new_ptr != NULL)
      {
        (void) memcpy(new_ptr, ptr, size);
        vPortFree(ptr);
        ret = new_ptr;
      }
      else
      {
        ret = NULL;
      }
    }
    else
    {
      vPortFree(ptr);
      ret = NULL;
    }
  }
  return ret;
}
#endif /* !NET_ALLOC_DEBUG */


/* Below functions are not supposed to be used, all malloc /free should be mapped to NET_MALLOC/NET_FREE macros */
/* if not the case, it means that some mapping are missing                                                      */
/* Using directly the malloc/free prevent to track original location of memory leakages                         */

#if !defined(STM32_THREAD_SAFE_STRATEGY)
void *realloc(void *p, size_t size)
{
  return NET_REALLOC(p, size);
}


void *calloc(size_t num, size_t size)
{
  return NET_CALLOC(num, size);
}


void *malloc(size_t size)
{
  return NET_MALLOC(size);
}


void free(void *p)
{
  NET_FREE(p);
}
#endif /* STM32_THREAD_SAFE_STRATEGY */

#endif /* NET_USE_RTOS */


#ifdef NET_ALLOC_DEBUG

#ifdef NET_ALLOC_DEBUG_TREE

#define NET_DISPLAY_RESULT_COLUMN (NET_DISPLAY_WIDTH-((2U*NET_DISPLAY_RESULT_WIDTH)+1U))

typedef struct net_alloc_node_s
{
  const char_t            *name;
  uint32_t                 namelen;
  struct net_alloc_node_s *down;
  struct net_alloc_node_s *up;
  struct net_alloc_node_s *neighbour;
  uint32_t                 current_alloc;
  uint32_t                 max_total_alloc;
#ifdef NET_ALLOC_HISTOGRAM
  uint16_t                 histogram[NET_MALLOC_MAX_HISTOGRAM];
#endif /* NET_ALLOC_HISTOGRAM */
} net_alloc_node_t;


static net_alloc_node_t Nodes[NET_ALLOC_MAX_NODE + 1U];
static uint32_t         NodesCount;

static net_alloc_node_t *RootNode;

#endif /* NET_ALLOC_DEBUG_TREE */

typedef struct net_debug_alloc
{
  const char_t     *filename;
  uint16_t          line;
  uint16_t          iter;
  uint32_t          size;
  void             *p;
#ifdef NET_ALLOC_DEBUG_TREE
  net_alloc_node_t *node;
#endif /* NET_ALLOC_DEBUG_TREE */
} net_debug_alloc_t;

static uint32_t AllocCurrent  = 0;
static uint32_t AllocTotalMax = 0;

static uint32_t iteralloc       = 0;
static uint32_t iterfree        = 0;
static uint32_t max_alive_alloc = 0;
static net_debug_alloc_t AllocInfos[NET_LEAKAGE_ARRAY];


void *net_calloc_debug(size_t num, size_t size, const char_t *filename, uint32_t line)
{
  void *p = net_malloc_debug(num * size, filename, line);

  if (p != NULL)
  {
    (void) memset(p, 0, num * size);
  }
  return p;
}


void *net_realloc_debug(void *p, size_t size, const char_t *filename, uint32_t line)
{
  void *ret;
  if (p == NULL)
  {
    if (size != (size_t) 0)
    {
      ret = net_malloc_debug(size, filename, line);
      if (ret == NULL)
      {
        /* to avoid erroneous MISRA detection */
        ret = NULL;
      }
    }
    else
    {
      ret = NULL;
    }
  }
  else
  {
    if (size != (size_t) 0)
    {
      void *new_ptr;
      new_ptr = net_malloc_debug(size, filename, line);
      if (new_ptr != NULL)
      {
        (void) memcpy(new_ptr, p, size);
        net_free_debug(p);
        ret = new_ptr;
      }
      else
      {
        ret = NULL;
      }
    }
    else
    {
      net_free_debug(p);
      ret = NULL;
    }
  }
  return ret;
}


#ifdef NET_ALLOC_DEBUG_TREE

static uint32_t getdirlen(const char_t *filename_in)
{
  uint32_t len = 0;
  const char_t *filename = filename_in;

  if (*filename != '\0')
  {
    if (*filename == '\\')
    {
      len = 1;
      filename++;
    }

    if ((filename[0] == 'C') && (filename[1] == ':') && (filename[2] == '\\'))
    {
      len = 3;
      filename++;
      filename++;
      filename++;
    }

    while ((*filename != '\0') && (*filename != '\\'))
    {
      len++;
      filename++;
    }
  }
  return len;
}


static net_alloc_node_t *get_new_node(const char_t *filename, uint32_t dir_name_len, uint32_t n)
{
  net_alloc_node_t *p = &Nodes[NodesCount];
  uint32_t next_dir_name_len = getdirlen(&filename[dir_name_len]);

  NodesCount++;
  p->current_alloc = n;
  p->max_total_alloc = n;
  p->name = filename;
  p->namelen = dir_name_len;

  if (NodesCount == NET_ALLOC_MAX_NODE)
  {
    while (true)
    {
      (void) printf("ERROR: Please increase NET_ALLOC_MAX_NODE, current value %" PRIu32 " is too low\n",
                    (uint32_t)NET_ALLOC_MAX_NODE);
    }
  }

  if (next_dir_name_len != 0U)
  {
    p->down = get_new_node(&filename[dir_name_len], next_dir_name_len, n);
    p->down->up = p;
  }

  return p;
}


#ifdef NET_ALLOC_HISTOGRAM
static void histogram(net_alloc_node_t *root, uint32_t  n_in)
{
  uint32_t nn = n_in;
  int32_t ln2 = 0;
  while (nn != 0U)
  {
    nn >>= 1;
    ln2++;
  }
  ln2 -= 5;
  if (ln2 < 0)
  {
    ln2 = 0;
  }
  if (ln2 > (NET_MALLOC_MAX_HISTOGRAM - 1))
  {
    ln2 = NET_MALLOC_MAX_HISTOGRAM - 1;
  }
  root->histogram[ln2]++;
}
#endif /* NET_MALLOC_MAX_HISTOGRAM */


static net_alloc_node_t *net_walk_malloc_node(net_alloc_node_t *root, const char_t *filename, uint32_t n)
{
  net_alloc_node_t *p = root;
  uint32_t dir_name_len = getdirlen(filename);

  /* Search among the neighbor (same level tree). */
  while (NULL != p)
  {
    if (strncmp(p->name, filename, dir_name_len) == 0)
    {
      /* Find a matching neighbor. */
      p->current_alloc += n;
      if (p->current_alloc > p->max_total_alloc)
      {
        p->max_total_alloc = p->current_alloc;
      }
      /*printf("ALLOC %s %d %d inc %d\n",filename,p->max_total_alloc,p->current_alloc,n);*/
      /* Did not reach a leaf so continue down in the tree */
      if (filename[dir_name_len] != '\0')
      {
        p = net_walk_malloc_node(p->down, &filename[dir_name_len], n);
      }
#ifdef NET_ALLOC_HISTOGRAM
      else
      {
        histogram(p, n);
      }
#endif /* NET_ALLOC_HISTOGRAM */
      break;
    }
    p = p->neighbour;
  }

  if (NULL == p)
  {
    /* Create a new neighbor and all the associated tree. */
    p = get_new_node(filename, dir_name_len, n);
    /* Add to the neighbor list. */
    if (NULL != root)
    {
      p->up = root->up;
      p->neighbour = root->neighbour;
      root->neighbour = p;
    }
    /* But we should return the leaf. */
    while (NULL != p->down)
    {
      p = p->down;
    }
#ifdef NET_ALLOC_HISTOGRAM
    histogram(p, n);
#endif /* NET_ALLOC_HISTOGRAM */
  }

  return p;
}


static void net_walk_free_node(net_alloc_node_t *node_in, uint32_t n)
{
  net_alloc_node_t *node = node_in;
  while (NULL != node)
  {
    node->current_alloc -= n;
    /*printf("FREE %s %d %d inc %d\n",node->name,node->max_total_alloc,node->current_alloc,n);*/
    node = node->up;
  }
}

static net_alloc_node_t *net_malloc_tree_start(net_alloc_node_t *root)
{
  net_alloc_node_t *p = root;

  while ((NULL != p) && (NULL != p->down) && (NULL == p->down->neighbour))
  {
    p = p->down;
  }
  return p;
}


static void net_malloc_tree_print(net_alloc_node_t *root, uint32_t level)
{
  net_alloc_node_t *p = root;
  if (level == 0U)
  {
    (void) printf("\n### Dynamic Allocation tree report, "\
                  "uses %" PRIu32 " nodes out of %" PRIu32 " (%" PRIu32 " bytes)\n",
                  NodesCount, (uint32_t)NET_ALLOC_MAX_NODE, NodesCount * sizeof(net_alloc_node_t));

    for (uint32_t i = 0U; i < NET_DISPLAY_RESULT_COLUMN; i++)
    {
      (void) printf("-");
    }
    (void) printf("\n%*s %*s\n", NET_DISPLAY_RESULT_WIDTH, "Maximum", NET_DISPLAY_RESULT_WIDTH, "Leakage");
  }

  while (NULL != p)
  {
    {
      char_t sn[NET_DISPLAY_DIRNAME_LEN] = {0};
      uint32_t ww = level;
      int field_width = NET_DISPLAY_RESULT_COLUMN - ((level + 1U) * NET_DISPLAY_TAB) - NET_DISPLAY_DIRNAME_LEN;
      (void)strncpy(sn, p->name, sizeof(sn) - 1);
      (void)printf("%*c", NET_DISPLAY_TAB, ' ');
      while (ww != 0U)
      {
        ww--;
        (void) printf(".");
#if (NET_DISPLAY_TAB > 1)
        (void)printf("%*c", NET_DISPLAY_TAB - 1U, ' ');
#endif /* NET_DISPLAY_TAB */
      }
      (void) printf("%-*s", NET_DISPLAY_DIRNAME_LEN, sn);
      if ((NULL != p->down)
          && (p->down->max_total_alloc == p->max_total_alloc)
          && (p->down->current_alloc == p->current_alloc))
      {
        (void) printf("\n");
      }
      else
      {
        (void) printf("%*c%*" PRIu32 " %*" PRIu32 "\n",
                      field_width, ' ',
                      (int)NET_DISPLAY_RESULT_WIDTH, p->max_total_alloc,
                      (int)NET_DISPLAY_RESULT_WIDTH, p->current_alloc);
#ifdef NET_ALLOC_HISTOGRAM
        for (int32_t i = 0; i < NET_MALLOC_MAX_HISTOGRAM; i++)
        {
          if (p->histogram[i] != 0U)
          {
            (void) printf("%*c    histogram [ %3" PRIu32 "..%3" PRIu32 " ] = %" PRIu32 "\n",
                          field_width, ' ',
                          (i > 0) ? (uint32_t)(1U << ((uint32_t)i + 4U)) : (uint32_t)0,
                          (uint32_t)(1U << ((uint32_t)i + 5U)),
                          (uint32_t)p->histogram[i]);
          }
        }
#endif /* NET_ALLOC_HISTOGRAM */
      }
    }
    net_malloc_tree_print(p->down, level + 1U);
    p = p->neighbour;
  }
}

#endif /* NET_ALLOC_DEBUG_TREE */


void *net_malloc_debug(size_t size, const char_t *filename, uint32_t line)
{
  void *pp;
  uint32_t i;

#ifdef NET_USE_RTOS
  vTaskSuspendAll();
#endif /* NET_USE_RTOS */

  if (iteralloc == 0U)
  {
    (void) memset(AllocInfos, 0, sizeof(AllocInfos));
    AllocCurrent = 0;
    AllocTotalMax = 0;
#ifdef NET_ALLOC_DEBUG_TREE
    NodesCount = 0;
    RootNode = NULL;
    (void) memset(Nodes, 0, sizeof(Nodes));
#endif /* NET_ALLOC_DEBUG_TREE */
  }

  iteralloc++;
  AllocCurrent += size;
  if (AllocCurrent > AllocTotalMax)
  {
    AllocTotalMax = AllocCurrent;
  }
  /*printf("-ALLOC- Max %d Current %d inc %d\n",max_total_alloc,current_alloc,nn);*/

#ifdef NET_USE_RTOS
  pp = pvPortMalloc(size + NET_ALLOC_OVERWRITE_CHECK_OFFSET);
#else
  pp = malloc(size + NET_ALLOC_OVERWRITE_CHECK_OFFSET);
#endif /* NET_USE_RTOS */

  if (pp == NULL)
  {
    printf("Allocation failure\n");
    while (1);
  }
#if NET_ALLOC_OVERWRITE_CHECK_OFFSET == 4
  *((uint8_t *)((uint8_t *) pp + size + 0)) = 0xDE;
  *((uint8_t *)((uint8_t *) pp + size + 1)) = 0xAD;
  *((uint8_t *)((uint8_t *) pp + size + 2)) = 0xBE;
  *((uint8_t *)((uint8_t *) pp + size + 3)) = 0xEF;
#endif  /* NET_ALLOC_OVERWRITE_CHECK_OFFSET */
  if ((iteralloc - iterfree) > max_alive_alloc)
  {
    max_alive_alloc = iteralloc - iterfree;
  }

  for (i = 0; i < NET_LEAKAGE_ARRAY; i++)
  {
    if (AllocInfos[i].p == 0U)
    {
      AllocInfos[i].p = pp;
      AllocInfos[i].size = size;
      AllocInfos[i].line = (uint16_t) line;
      AllocInfos[i].filename = filename;
      AllocInfos[i].iter = (uint16_t) iteralloc;
#ifdef NET_ALLOC_DEBUG_TREE
      AllocInfos[i].node = net_walk_malloc_node(RootNode, filename, size);
      if (NULL == RootNode)
      {
        RootNode = AllocInfos[i].node;
        /* We got the leaf, so move to top. */
        while (NULL != RootNode->up)
        {
          RootNode = RootNode->up;
        }
      }
#endif /*  NET_ALLOC_DEBUG_TREE */
      break;
    }
  }

  NET_ASSERT((iteralloc != NET_ALLOC_BREAK), "Reach Allocation break\n");

  NET_ASSERT((i != NET_LEAKAGE_ARRAY),
             "Too many allocations, Please increase NET_LEAKAGE_ARRAY (%" PRIu32 ") in net_conf.h",
             (uint32_t)NET_LEAKAGE_ARRAY);

#if NET_ALLOC_VERBOSE
  (void) printf("%lu bytes allocated: Allocating size %lu at %s:%lu\n",
                total_alloc, AllocInfos[i].size,
                AllocInfos[i].filename, AllocInfos[i].line);
#endif /* NET_ALLOC_VERBOSE */

#ifdef NET_USE_RTOS
  (void) xTaskResumeAll();
#endif /* NET_USE_RTOS */

  return pp;
}


void net_free_debug(void *p)
{
  uint32_t i;

#ifdef NET_USE_RTOS
  vTaskSuspendAll();
#endif /* NET_USE_RTOS */

#ifdef NET_FREE_STOP_ON_NULL_POINTER
  if (NULL == p)
  {
    (void) printf("Free function : Freeing a NULL pointer\n");
    while (1);
  }
#endif /* NET_STOP_ON_FREEING_NULL_POINTER */

  if (NULL != p)
  {
    iterfree++;
    for (i = 0; i < NET_LEAKAGE_ARRAY; i++)
    {
      if (AllocInfos[i].p == p)
      {
        AllocInfos[i].p = NULL;
        AllocCurrent -= AllocInfos[i].size;
#if (NET_ALLOC_OVERWRITE_CHECK_OFFSET == 4)
        uint8_t  *dead = (uint8_t *) p + AllocInfos[i].size;
        bool dead_failed = false;

        if (dead[0] != 0xDE)
        {
          dead_failed = true;
        }
        if (dead[1] != 0xAD)
        {
          dead_failed = true;
        }
        if (dead[2] != 0xBE)
        {
          dead_failed = true;
        }
        if (dead[3] != 0xEF)
        {
          dead_failed = true;
        }
        if (dead_failed)
        {
          (void) printf("\tAllocation number #%" PRIu32 " is overwritten ! %p size %" PRIu32 " at %s:%" PRIu32 "\n",
                        (uint32_t)AllocInfos[i].iter, AllocInfos[i].p, AllocInfos[i].size,
                        AllocInfos[i].filename, (uint32_t)AllocInfos[i].line);

          while (1);
        }
#endif /* NET_ALLOC_OVERWRITE_CHECK_OFFSET */


        /*printf("-FREE- Max %d Current %d inc %d\n",max_total_alloc,current_alloc,allocated_info[i].size);*/
#ifdef NET_ALLOC_DEBUG_TREE
        net_walk_free_node(AllocInfos[i].node, AllocInfos[i].size);
#endif /* NET_ALLOC_DEBUG_TREE */
        AllocInfos[i].size = 0;
        break;
      }
    }

    if (i == NET_LEAKAGE_ARRAY)
    {
      (void) printf("Free function : did not find this segment %p\n", p);
      while (true) {};
    }
  }

#ifdef NET_USE_RTOS
  vPortFree(p);
  (void) xTaskResumeAll();
#else
  free(p);
#endif /* NET_USE_RTOS */
}


void net_alloc_report(void)
{
  uint32_t count = 0;
  uint32_t leak = 0;

#ifdef NET_USE_RTOS
  vTaskSuspendAll();
#endif /* NET_USE_RTOS */

  (void) printf("\n### Net Malloc report: max alloc: %" PRIu32 " bytes, current leakage: %" PRIu32 " bytes, "
                "number of allocation: %" PRIu32 ", number of free: %" PRIu32 ", "
                "alloc table usage %" PRIu32 " / %" PRIu32 "( %" PRIu32 ") bytes\n\n",
                AllocTotalMax, AllocCurrent, iteralloc, iterfree, max_alive_alloc,
                (uint32_t)NET_LEAKAGE_ARRAY, (uint32_t)sizeof(AllocInfos));

  for (uint32_t i = 0; i < NET_LEAKAGE_ARRAY; i++)
  {
    if (AllocInfos[i].p != 0)
    {
      count++;
      leak += AllocInfos[i].size;
      (void) printf("\tAllocation number #%" PRIu32 " Not free %p size %4" PRIu32 " at %s: %" PRIu32 "\n",
                    (uint32_t)AllocInfos[i].iter, AllocInfos[i].p, AllocInfos[i].size,
                    AllocInfos[i].filename, (uint32_t)AllocInfos[i].line);
    }
  }
  (void) printf("\n%" PRIu32 " allocation not freed %" PRIu32 " bytes out of %" PRIu32 " allocated byte\n\n",
                count, leak, AllocTotalMax);

#ifdef NET_ALLOC_DEBUG_TREE
  net_malloc_tree_print(net_malloc_tree_start(RootNode), 0);
#endif /* NET_ALLOC_DEBUG_TREE */
  (void) printf("\n### Net Malloc end report\n");

#ifdef NET_USE_RTOS
  (void) xTaskResumeAll();
#endif /* NET_USE_RTOS */

}

#endif  /* NET_DEBUG_ALLOC */

/* Forward declaration of the system clock, previously set by the platform. */
extern uint32_t SystemCoreClock;


static struct net_perf
{
  uint32_t      total;
#if defined(NET_USE_RTOS) && defined(NET_PERF_TASK)
  uint32_t      elapsed_cycle[NET_PERF_MAXTHREAD];
  TaskHandle_t  handle[NET_PERF_MAXTHREAD];
  char          name[NET_PERF_MAXTHREAD][24];
  uint32_t      task_history[NET_TASK_HISTORY_SIZE + 1U];
  uint32_t      prio[NET_TASK_HISTORY_SIZE + 1U];
  uint32_t      task_history_count;
  uint32_t      task_current_cycle;
#endif /* NET_USE_RTOS && NET_PERF_TASK */
} Perfs;

#if defined(__CORTEX_M)
#define NET_DWT_CONTROL              DWT->CTRL
#define NET_DWT_CYCCNT               DWT->CYCCNT
#define NET_DEMCR                    CoreDebug->DEMCR

#else
/* Redeclare in case not provided through the net_conf.h. */

#ifndef __IO
#define __IO volatile
#endif /* __IO */

/* Enable DWT cycle counter. */
#define NET_DWT_CONTROL             (*((__IO uint32_t*)0xE0001000U))
/* DWT Cycle Counter register. */
#define NET_DWT_CYCCNT              (*((__IO uint32_t*)0xE0001004U))
/* DEMCR: Debug Exception and Monitor Control Register. */
#define NET_DEMCR                   (*((__IO uint32_t*)0xE000EDFCU))
#endif /* __CORTEX_M */

#define NET_DWT_CYCCNTENA_BIT       (1UL<<0U)
#define NET_TRCENA_BIT              (1UL<<24U)


static uint32_t net_get_cycle(void)
{
  return NET_DWT_CYCCNT;
}


#if 0
static uint32_t net_get_us(void)
{
  return net_get_cycle() / (SystemCoreClock / 1000000U);
}
#endif /* 0 */


void net_stop_cycle(void)
{
  NET_DWT_CONTROL &= ~NET_DWT_CYCCNTENA_BIT;
}


void net_start_cycle(void)
{
  NET_DWT_CONTROL |= NET_DWT_CYCCNTENA_BIT;
}


#if defined(NET_PERF_TASK)

/**
  * @brief  Executed for each context switch in
  * @param  None
  * @retval None
  */

#if !defined(NET_USE_RTOS)
#error "NET_USE_RTOS must be defined to use NET_PERF_TASK"
#endif /* NET_USE_RTOS */


void net_perf_task_tag(uint32_t msg_id)
{
  if (NET_TASK_HISTORY_SIZE != Perfs.task_history_count)
  {
    Perfs.task_history[Perfs.task_history_count] = MERGE_TASKID_AND_TIME(TAG_TASK_ID, msg_id);
    Perfs.task_history_count++;
  }
}


static uint32_t GetPrio(TaskHandle_t xHandle)
{
  uint32_t vv;
  TaskStatus_t xTaskDetails;

  vTaskGetInfo(
    /* The handle of the task being queried. */
    xHandle,
    /* The TaskStatus_t structure to complete with information
    on xTask. */
    &xTaskDetails,
    /* Include the stack high water mark value in the
    TaskStatus_t structure. */
    pdTRUE,
    /* Include the task state in the TaskStatus_t structure. */
    eInvalid);

  vv = xTaskDetails.uxBasePriority;
  vv += (xTaskDetails.uxCurrentPriority << 16U);
  return vv;
}


void net_perf_task_in(void)
{
  uint32_t i;
  TaskHandle_t xHandle = xTaskGetCurrentTaskHandle();

  for (i = 0; i < NET_PERF_MAXTHREAD; i++)
  {
    if (NULL == Perfs.handle[i])
    {
      /* New thread */
      Perfs.handle[i] = xHandle;
      snprintf(Perfs.name[i], 24, "%23s", GetTaskName(xHandle));
    }
    if (xHandle == Perfs.handle[i])
    {
      Perfs.elapsed_cycle[i] -= net_get_cycle();
      break;
    }
  }
  NET_ASSERT(NET_PERF_MAXTHREAD != i, "Net perf, please increase NET_PERF_MAX_THREAD");
}


/**
  * @brief  Executed for each context switch out
  * @param  None
  * @retval None
  */
void net_perf_task_out(void)
{
  uint32_t i;
  TaskHandle_t xHandle = xTaskGetCurrentTaskHandle();

  for (i = 0; i < NET_PERF_MAXTHREAD; i++)
  {
    if (NULL == Perfs.handle[i])
    {
      /* New thread */
      Perfs.handle[i] = xHandle;
      snprintf(Perfs.name[i], 26, "%25s", GetTaskName(xHandle));
    }
    if (xHandle == Perfs.handle[i])
    {
      Perfs.elapsed_cycle[i] += net_get_cycle();
      break;
    }
  }

  if (NET_TASK_HISTORY_SIZE != Perfs.task_history_count)
  {
    Perfs.task_history[Perfs.task_history_count] = MERGE_TASKID_AND_TIME(i, net_get_cycle() - Perfs.task_current_cycle);
    Perfs.prio[Perfs.task_history_count] = GetPrio(xHandle);
    Perfs.task_history_count++;
    Perfs.task_current_cycle = net_get_cycle();
  }

  NET_ASSERT(NET_PERF_MAXTHREAD != i, "Net perf, please increase NET_PERF_MAX_THREAD");
}


static const char *GetTaskName(TaskHandle_t xHandle)
{
  TaskStatus_t xTaskDetails;
  vTaskGetInfo(
    /* The handle of the task being queried. */
    xHandle,
    /* The TaskStatus_t structure to complete with information on xTask. */
    &xTaskDetails,
    /* Include the stack high water mark value in the TaskStatus_t structure. */
    pdTRUE,
    /* Include the task state in the TaskStatus_t structure. */
    eInvalid);
  return (const char *)xTaskDetails.pcTaskName;
}

#else

void net_perf_task_in(void);
void net_perf_task_out(void);

void net_perf_task_in(void)
{
}


void net_perf_task_out(void)
{
}
#endif /* NET_PERF_TASK */

#if (NET_TASK_HISTORY_SIZE > 0)
static const float TO_MICRO_SECOND = 1000000.0;
#endif  /* NET_TASK_HISTORY_SIZE > 0 */


void net_perf_start(void)
{
  NET_DWT_CONTROL |= NET_DWT_CYCCNTENA_BIT;
  NET_DWT_CYCCNT = 0U;
  (void)memset(&Perfs, 0, sizeof(Perfs));

#if defined(NET_PERF_TASK)

#if (NET_TASK_HISTORY_SIZE > 0)
  Perfs.task_current_cycle = net_get_cycle();
#endif /* NET_TASK_HISTORY_SIZE */

  net_perf_task_in();
#endif /* NET_PERF_TASK */
}


void net_perf_report(void)
{
#if defined(NET_PERF_TASK)
  net_perf_task_out();
#endif /* NET_PERF_TASK */

  Perfs.total += net_get_cycle();
  (void) printf("\n### Net Performance report CPU Freq %3"PRIu32" Mhz\n\n", SystemCoreClock / 1000000U);
  (void) printf("\tTotal   %12" PRIu32 " cycles  %8" PRIu32 " ms\n", Perfs.total,
                Perfs.total / (SystemCoreClock / 1000U));

#if defined(NET_PERF_TASK)
  {
    uint32_t count = 0;

    (void) printf("\n");
    while ((count < NET_PERF_MAXTHREAD) && Perfs.handle[count] != 0)
    {
      (void) printf("\tthread #%" PRIu32 " %25s : %12" PRIu32 " cycles  %7" PRIu32 " ms\n",
                    count, Perfs.name[count],
                    Perfs.elapsed_cycle[count], Perfs.elapsed_cycle[count] / (SystemCoreClock / 1000U));
      count++;
    }
  }
#endif /* NET_PERF_TASK */


#if (NET_TASK_HISTORY_SIZE > 0)

  float rf = TO_MICRO_SECOND / SystemCoreClock;
  uint32_t tt = 0;

  printf("last record index %" PRIu32 " out of %" PRIu32 "\n",
         Perfs.task_history_count, (uint32_t)NET_TASK_HISTORY_SIZE);

  for (uint32_t i = 0; i < Perfs.task_history_count - 1; i++)
  {
    const uint32_t task_id = EXTRACT_TASK_ID(Perfs.task_history[i]);
    const uint32_t dtime = EXTRACT_DTIME(Perfs.task_history[i]);
    const uint32_t based_prio = Perfs.prio[i] & 0xFFFFU;
    const uint32_t inherited_prio = Perfs.prio[i] >> 16U;

    if (TAG_TASK_ID != task_id)
    {
      printf("%08" PRIu32 "    %20s:  %" PRIu32 " us base prio %" PRIu32 " inherited %" PRIu32 "\n",
             (uint32_t)(tt * rf), Perfs.name[task_id],
             (uint32_t)(dtime * rf), based_prio, inherited_prio);
      tt += dtime;
    }
    else
    {
      printf("            Event %" PRIu32 "\n", dtime);
    }
  }
#endif /* NET_TASK_HISTORY_SIZE */

  (void) printf("\n### Net Performance end report\n\n");
}
