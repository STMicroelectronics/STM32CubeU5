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
#include "net_connect.h"
#include "net_internals.h"
#include "net_perf.h"
#include "net_mem.h"

#include "stdarg.h"
#include <inttypes.h>

#ifdef NET_USE_RTOS
#include "task.h"


#if defined(NET_PERF_TASK)
static const char_t *GetTaskName(TaskHandle_t xHandle);

#define TAG_TASK_ID                                  7U
#define MERGE_TASKID_AND_TIME(TASK_NUM, TIME_VALUE)  (((TASK_NUM) & 7U) + ((TIME_VALUE)<<3U))
#define EXTRACT_TASK_ID(VALUE)                       ((VALUE) & 7U)
#define EXTRACT_DTIME(VALUE)                         ((VALUE)>>3U)
#endif /* NET_PERF_TASK */

#if (osCMSIS >= 0x20000U)
#define OSSEMAPHOREWAIT         osSemaphoreAcquire
#else
#define OSSEMAPHOREWAIT         osSemaphoreWait
#endif /* osCMSIS */

extern void *pxCurrentTCB;


static osSemaphoreId net_mutex[NET_LOCK_NUMBER];

void net_init_locks(void)
{
#if (osCMSIS < 0x20000U)
  static osSemaphoreDef_t mutex_def[NET_LOCK_NUMBER] = {0};
#endif /* osCMSIS */
#ifdef NET_MBEDTLS_HOST_SUPPORT
  net_tls_init();
#endif /* NET_MBEDTLS_HOST_SUPPORT */
  for (int32_t i = 0; i < NET_LOCK_NUMBER; i++)
  {
#if (osCMSIS < 0x20000U)
    net_mutex[i] = osSemaphoreCreate(&mutex_def[i], 1);
#else
    net_mutex[i] = osSemaphoreNew(1, 1, NULL);

#endif /* osCMSIS */
    NET_ASSERT(net_mutex[i] > 0, "Failed on mutex creation");
  }
}


void net_destroy_locks(void)
{
#ifdef NET_MBEDTLS_HOST_SUPPORT
  net_tls_destroy();
#endif /* NET_MBEDTLS_HOST_SUPPORT */
  for (int32_t i = 0; i < NET_LOCK_NUMBER; i++)
  {
    (void)osSemaphoreDelete(net_mutex[i]);
  }
}


void net_lock(int32_t sock, uint32_t timeout_in)
{
  int32_t   ret;
  uint32_t timeout = timeout_in;
  if (timeout ==  NET_OS_WAIT_FOREVER)
  {
    /*MISRA issue so hand coded  osWaitForever*/
    timeout = 0xffffffffU;
  }
  ret = (int32_t) OSSEMAPHOREWAIT(net_mutex[sock], timeout);
  NET_ASSERT(ret == 0, "Failed locking mutex");
}


void net_unlock(int32_t sock)
{
  int32_t   ret;
  ret = (int32_t) osSemaphoreRelease(net_mutex[sock]);
  NET_ASSERT(ret == 0, "Failed unlocking mutex");
}


void net_lock_nochk(int32_t sock, uint32_t timeout_in)
{
  uint32_t timeout = timeout_in;
  if (timeout == NET_OS_WAIT_FOREVER)
  {
    /* MISRA issue so hand coded  t osWaitForever*/
    timeout = 0xffffffffU;
  }
  (void) OSSEMAPHOREWAIT(net_mutex[sock], timeout);
}

void net_unlock_nochk(int32_t sock)
{
  (void) osSemaphoreRelease(net_mutex[sock]);
}


#if !defined(NET_ALLOC_DEBUG)
#define MAX_ALLOC_VALUE 0x4000U

void *net_calloc(size_t n, size_t m)
{
  void *p = NULL;

  if ((n < MAX_ALLOC_VALUE) && (m < MAX_ALLOC_VALUE))
  {
    p =   pvPortMalloc(n * m);
  }
  if (p != NULL)
  {
    (void) memset(p, 0, n * m);
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

/* below function are not supposed to be used , all malloc /free should be mapped to NET_MALLOC/NET_FREE macros  */
/* if not the case , it means that some mapping are missing                                                      */
/* Using directly the malloc/free prevent to track original location of memory leakages                           */


void *realloc(void *p, size_t n)
{
  return NET_REALLOC(p, n);
}


void *calloc(size_t n, size_t m)
{
  return NET_CALLOC(n, m);
}


void *malloc(size_t n)
{
  return NET_MALLOC(n);
}

void free(void *p)
{
  NET_FREE(p);
}

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


static  net_alloc_node_t  node[NET_ALLOC_MAX_NODE + 1U];
static  net_alloc_node_t *root_node;
static  uint32_t          max_node;

#endif /* NET_ALLOC_DEBUG_TREE */

typedef struct net_debug_alloc
{
  const char_t      *filename;
  uint16_t           line;
  uint16_t           iter;
  uint32_t           size;
  void              *p;
#ifdef NET_ALLOC_DEBUG_TREE
  net_alloc_node_t  *node;
#endif /* NET_ALLOC_DEBUG_TREE */
} net_debug_alloc_t;

static uint32_t current_alloc   = 0;
static uint32_t max_total_alloc = 0;

static uint32_t iteralloc       = 0;
static uint32_t iterfree        = 0;
static uint32_t max_alive_alloc = 0;
static net_debug_alloc_t allocated_info[NET_LEAKAGE_ARRAY];


void *net_calloc_debug(size_t nn, size_t msize, const char_t *filename, uint32_t line)
{
  void *p = net_malloc_debug(nn * msize, filename, line);

  if (p != NULL)
  {
    (void) memset(p, 0, nn * msize);
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
      new_ptr =  net_malloc_debug(size, filename, line);
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
  const char_t  *filename = filename_in;

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
  net_alloc_node_t *p = &node[max_node];
  uint32_t next_dir_name_len = getdirlen(&filename[dir_name_len]);

  max_node++;
  p->current_alloc = n;
  p->max_total_alloc = n;
  p->name  = filename;
  p->namelen = dir_name_len;
  if (max_node == NET_ALLOC_MAX_NODE)
  {
    while (true)
    {
      (void) printf("ERROR: Please increase NET_ALLOC_MAX_NODE, current value %lu is too low\n", NET_ALLOC_MAX_NODE);
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

  /* search among the neighbour (same level tree) */
  while (NULL != p)
  {
    if (strncmp(p->name, filename, dir_name_len) == 0)
    {
      /* find a matching neighbour */
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
    /* create a new neighbour and all the associated tree */
    p = get_new_node(filename, dir_name_len, n);
    /* add to neighbour list */
    if (NULL != root)
    {
      p->up = root->up;
      p->neighbour = root->neighbour;
      root->neighbour = p;
    }
    /* but we should return the leaf */
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

  while (NULL == p->down->neighbour)
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
    (void) printf("\n### Dynamic Allocation tree report, uses %lu nodes out of %lu (%lu bytes)\n", max_node,
                  NET_ALLOC_MAX_NODE, max_node * sizeof(net_alloc_node_t));
    for (uint32_t i = 0U; i < NET_DISPLAY_RESULT_COLUMN; i++)
    {
      (void) printf("-");
    }
    (void) printf("%*s %*s", NET_DISPLAY_RESULT_WIDTH, "Maximum", NET_DISPLAY_RESULT_WIDTH, "Leakage");
    (void) printf("\n");
  }

  while (NULL != p)
  {
    {
      char_t sn[100];
      uint32_t ww = level;
      uint32_t nn = NET_DISPLAY_RESULT_COLUMN - ((level + 1U) * NET_DISPLAY_TAB) - NET_DISPLAY_DIRNAME_LEN;
      (void) strncpy(sn, &p->name[1], p->namelen - 1U);
      sn[p->namelen - 1U] = '\0';
      (void) printf("%*c", NET_DISPLAY_TAB, ' ');
      while (ww != 0U)
      {
        ww--;
        (void) printf(".");
#if  (NET_DISPLAY_TAB>1)
        (void)printf("%*c", NET_DISPLAY_TAB - 1U, ' ');
#endif /* NET_DISPLAY_TAB */
      }
      (void) printf("%-*s", NET_DISPLAY_DIRNAME_LEN, sn);
      if ((NULL != p->down) && (p->down->max_total_alloc == p->max_total_alloc)
          && (p->down->current_alloc == p->current_alloc))
      {
        (void) printf("\n");
      }
      else
      {
        (void) printf("%*c%*d %*d\n", nn, ' ', NET_DISPLAY_RESULT_WIDTH, p->max_total_alloc, NET_DISPLAY_RESULT_WIDTH,
                      p->current_alloc);
#ifdef NET_ALLOC_HISTOGRAM
        for (int32_t i = 0; i < NET_MALLOC_MAX_HISTOGRAM; i++)
        {
          if (p->histogram[i] != 0U)
          {
            (void) printf("%*c    histogram [ %d..%d ] = %lu\n", nn, ' ', \
                          (i > 0) ? 1U << ((uint32_t)i + 4U) : 0, 1U << ((uint32_t)i + 5U), p->histogram[i]);
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


void *net_malloc_debug(size_t nn, const char_t *filename, uint32_t line)
{
  void *pp;
  uint32_t i;

#ifdef NET_USE_RTOS
  vTaskSuspendAll();
#endif /* NET_USE_RTOS */

  if (iteralloc == 0U)
  {
    (void) memset(allocated_info, 0, sizeof(allocated_info));
    current_alloc = 0;
    max_total_alloc = 0;
#ifdef NET_ALLOC_DEBUG_TREE
    max_node = 0;
    root_node = NULL;
    (void) memset(node, 0, sizeof(node));
#endif /* NET_ALLOC_DEBUG_TREE */
  }

  iteralloc++;
  current_alloc += nn;
  if (current_alloc > max_total_alloc)
  {
    max_total_alloc = current_alloc;
  }
  /*printf("-ALLOC- Max %d Current %d inc %d\n",max_total_alloc,current_alloc,nn);*/

#ifdef NET_USE_RTOS
  pp = pvPortMalloc(nn + NET_ALLOC_OVERWRITE_CHECK_OFFSET);
#else
  pp = malloc(nn + NET_ALLOC_OVERWRITE_CHECK_OFFSET);
#endif /* NET_USE_RTOS */
  if (pp == NULL)
  {
    printf("Allocation failure\n");
    while (1);
  }
#if NET_ALLOC_OVERWRITE_CHECK_OFFSET == 4
  *((uint8_t *)((uint8_t *) pp + nn + 0)) = 0xDE;
  *((uint8_t *)((uint8_t *) pp + nn + 1)) = 0xAD;
  *((uint8_t *)((uint8_t *) pp + nn + 2)) = 0xBE;
  *((uint8_t *)((uint8_t *) pp + nn + 3)) = 0xEF;
#endif  /* NET_ALLOC_OVERWRITE_CHECK_OFFSET */
  if ((iteralloc - iterfree) > max_alive_alloc)
  {
    max_alive_alloc = iteralloc - iterfree;
  }

  for (i = 0; i < NET_LEAKAGE_ARRAY ; i++)
  {
    if (allocated_info[i].p == 0U)
    {
      allocated_info[i].p = pp ;
      allocated_info[i].size = nn ;
      allocated_info[i].line = (uint16_t) line ;
      allocated_info[i].filename = filename;
      allocated_info[i].iter = (uint16_t) iteralloc;
#ifdef NET_ALLOC_DEBUG_TREE
      allocated_info[i].node = net_walk_malloc_node(root_node, filename, nn);
      if (NULL == root_node)
      {
        root_node = allocated_info[i].node;
        /* we got the leaf , so move to top */
        while (NULL != root_node->up)
        {
          root_node = root_node->up;
        }
      }
#endif /*  NET_ALLOC_DEBUG_TREE */
      break;
    }
  }

  NET_ASSERT((iteralloc != NET_ALLOC_BREAK), "Reach Allocation break\n");

  NET_ASSERT((i != NET_LEAKAGE_ARRAY), "Too much allocations,Please increase NET_LEAKAGE_ARRAY (%lu)in net_conf.h",
             NET_LEAKAGE_ARRAY);
#if NET_ALLOC_VERBOSE
  (void) printf("%lu bytes allocated: Allocating size %lu at %s:%lu\n", total_alloc, allocated_info[i].size,
                allocated_info[i].filename, allocated_info[i].line);
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
    (void) printf("Free function : Freeing a NULL pointer, seriously ?\n");
    while (1);
  }
#endif /* NET_STOP_ON_FREEING_NULL_POINTER */

  if (NULL != p)
  {
    iterfree++;
    for (i = 0; i < NET_LEAKAGE_ARRAY; i++)
    {
      if (allocated_info[i].p == p)
      {
        allocated_info[i].p = NULL;
        current_alloc -= allocated_info[i].size;
#if  NET_ALLOC_OVERWRITE_CHECK_OFFSET == 4
        uint8_t  *dead = (uint8_t *) p + allocated_info[i].size;
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
          (void) printf("\tAllocation number #%lu  is overwritten ! %p  size %lu at %s:%lu\n",
                        allocated_info[i].iter, allocated_info[i].p,
                        allocated_info[i].size, allocated_info[i].filename, allocated_info[i].line);

          while (1);
        }
#endif /* NET_ALLOC_OVERWRITE_CHECK_OFFSET */


        /*printf("-FREE- Max %d Current %d inc %d\n",max_total_alloc,current_alloc,allocated_info[i].size);*/
#ifdef NET_ALLOC_DEBUG_TREE
        net_walk_free_node(allocated_info[i].node, allocated_info[i].size);
#endif /* NET_ALLOC_DEBUG_TREE */
        allocated_info[i].size = 0;
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
  uint32_t i;
  uint32_t count = 0;
  uint32_t leak = 0;

#ifdef NET_USE_RTOS
  vTaskSuspendAll();
#endif /* NET_USE_RTOS */

  (void) printf("\n### Net Malloc report: max alloc: %lu bytes, current leakage: %lu bytes,"
                "number of allocation: % lu, number of free: % lu, alloc table usage % lu / % lu( % lu) bytes\n\n",
                max_total_alloc, current_alloc, iteralloc, iterfree, max_alive_alloc,
                NET_LEAKAGE_ARRAY, sizeof(allocated_info));

  for (i = 0; i < NET_LEAKAGE_ARRAY; i++)
  {
    if (allocated_info[i].p != 0)
    {
      count++;
      leak += allocated_info[i].size;
      (void) printf("\tAllocation number #%lu Not free %p  size %lu at %s:%lu\n",
                    allocated_info[i].iter, allocated_info[i].p,
                    allocated_info[i].size, allocated_info[i].filename, allocated_info[i].line);
    }
  }
  (void) printf("\n%lu allocation not freed  %lu bytes out of %lu allocated byte\n\n", count, leak, max_total_alloc);

#ifdef NET_ALLOC_DEBUG_TREE
  net_malloc_tree_print(net_malloc_tree_start(root_node), 0);
#endif /* NET_ALLOC_DEBUG_TREE */
  (void) printf("\n### Net Malloc end report\n");

#ifdef NET_USE_RTOS
  (void) xTaskResumeAll();
#endif /* NET_USE_RTOS */

}

#endif  /* NET_DEBUG_ALLOC */

static struct net_perf
{
  uint32_t      total;
#if defined(NET_USE_RTOS) && defined(NET_PERF_TASK)
  uint32_t      elapsed_cycle[NET_PERF_MAXTHREAD];
  TaskHandle_t  handle[NET_PERF_MAXTHREAD];
  uint32_t      task_history[NET_TASK_HISTORY_SIZE + 1U];
  uint32_t      prio[NET_TASK_HISTORY_SIZE + 1U];
  uint32_t      task_history_count;
  uint32_t      task_current_cycle;
#endif /* NET_USE_RTOS */
} perf;


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
  if (NET_TASK_HISTORY_SIZE != perf.task_history_count)
  {
    perf.task_history[perf.task_history_count] = MERGE_TASKID_AND_TIME(TAG_TASK_ID, msg_id);
    perf.task_history_count++;
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
    if (NULL == perf.handle[i])
    {
      /* new thread */
      perf.handle[i] = xHandle;
    }
    if (xHandle == perf.handle[i])
    {
      perf.elapsed_cycle[i] -= net_get_cycle();
      break;
    }
  }
  NET_ASSERT(NET_PERF_MAXTHREAD != i, "Net perf ,please increase NET_PERF_MAX_THREAD");
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
    if (NULL == perf.handle[i])
    {
      /* new thread */
      perf.handle[i] = xHandle;
    }
    if (xHandle == perf.handle[i])
    {
      perf.elapsed_cycle[i] += net_get_cycle();
      break;
    }
  }

  if (NET_TASK_HISTORY_SIZE != perf.task_history_count)
  {
    perf.task_history[perf.task_history_count] = MERGE_TASKID_AND_TIME(i, net_get_cycle() - perf.task_current_cycle);
    perf.prio[perf.task_history_count] = GetPrio(xHandle);
    perf.task_history_count++;
    perf.task_current_cycle = net_get_cycle();
  }

  NET_ASSERT(NET_PERF_MAXTHREAD != i, "Net perf ,please increase NET_PERF_MAX_THREAD");
}

static const char_t *GetTaskName(TaskHandle_t xHandle)
{
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
  return (const char_t *)xTaskDetails.pcTaskName;
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


void net_perf_start(void)
{
  NET_DWT_CONTROL |= NET_DWT_CYCCNTENA_BIT ;
  NET_DWT_CYCCNT = 0U;
  (void) memset(&perf, 0, sizeof(perf));
#if defined(NET_PERF_TASK)

#if NET_TASK_HISTORY_SIZE > 0
  perf.task_current_cycle = net_get_cycle();
#endif /* NET_TASK_HISTORY_SIZE */

  net_perf_task_in();
#endif /* NET_PERF_TASK */
}

void net_perf_report(void)
{
#if defined(NET_PERF_TASK)
  net_perf_task_out();
#endif /* NET_PERF_TASK */
  perf.total += net_get_cycle();
  (void) printf("\n### Net Performance report    CPU Freq %3"PRIu32" Mhz\n\n", SystemCoreClock / 1000000U);
  (void) printf("\tTotal   %12"PRIu32" cycles  %8"PRIu32" ms\n", perf.total, perf.total / (SystemCoreClock / 1000U));
#if defined(NET_PERF_TASK)
  uint32_t    count = 0;
  (void) printf("\n");
  while (perf.handle[count] != 0)
  {
    (void) printf("\tthread #%lu  %25s : %12lu cycles  %7lu ms\n", count, GetTaskName(perf.handle[count]),
                  perf.elapsed_cycle[count], perf.elapsed_cycle[count] / (SystemCoreClock / 1000U));
    count++;
  }
#endif /* NET_PERF_TASK */


#define TO_MICRO_SECOND    1000000.0
#if NET_TASK_HISTORY_SIZE > 0
  float rf = TO_MICRO_SECOND / SystemCoreClock;
  uint32_t tt = 0;
  printf("last record index %d out of %d\n", perf.task_history_count, NET_TASK_HISTORY_SIZE);
  for (uint32_t  i = 0; i < perf.task_history_count - 1; i++)
  {
    uint32_t task_id = EXTRACT_TASK_ID(perf.task_history[i]);
    uint32_t dtime = EXTRACT_DTIME(perf.task_history[i]);
    uint32_t based_prio = perf.prio[i] & 0xFFFFU;
    uint32_t inherited_prio =  perf.prio[i] >> 16U;

    if (TAG_TASK_ID != task_id)
    {
      printf("%08lu    %20s:  %lu us base prio %lu inherited %lu\n",
             (uint32_t)(tt * rf), GetTaskName(perf.handle[task_id]),
             (uint32_t)(dtime * rf), based_prio, inherited_prio);
      tt += dtime;
    }
    else
    {
      printf("            Event %d\n", dtime);
    }
  }
#endif /* NET_TASK_HISTORY_SIZE */

  (void) printf("\n### Net Performance end report\n\n");
}
