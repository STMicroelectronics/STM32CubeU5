/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usbpd_dpm_core.c
  * @author  MCD Application Team
  * @brief   USBPD dpm core file
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
/* USER CODE END Header */

#define __USBPD_DPM_CORE_C

/* Includes ------------------------------------------------------------------*/
#include "usbpd_core.h"
#include "usbpd_trace.h"
#include "usbpd_dpm_core.h"
#include "usbpd_dpm_conf.h"
#include "usbpd_dpm_user.h"

#if defined(_LOW_POWER)
#include "usbpd_lowpower.h"
#endif /* _LOW_POWER */

/* OS management */
#include "usbpd_os_port_mx.h"

/* Private definition -------------------------------------------------------*/
/* function import prototypes -----------------------------------------------*/
/* Generic STM32 prototypes */
extern uint32_t HAL_GetTick(void);

/* Private function prototypes -----------------------------------------------*/
DEF_TASK_FUNCTION(USBPD_TaskUser);
DEF_TASK_FUNCTION(USBPD_CAD_Task);
DEF_TASK_FUNCTION(USBPD_PE_CableTask);

#if defined(USE_STM32_UTILITY_OS)
void TimerCADfunction(void *);
#endif /* USE_STM32_UTILITY_OS */

#if defined(USE_STM32_UTILITY_OS)
void USBPD_PE_Task_P0(void);
void USBPD_PE_Task_P1(void);
void TimerPE0function(void *pArg);
void TimerPE1function(void *pArg);
#endif /* USE_STM32_UTILITY_OS */

/* Private typedef -----------------------------------------------------------*/

#define OS_PE_PRIORITY                    1
#define OS_PE_STACK_SIZE                  1024

#define OS_CAD_PRIORITY                   1
#define OS_CAD_STACK_SIZE                 1024

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define CHECK_PE_FUNCTION_CALL(_function_)  do{                                     \
                                                _retr = _function_;                  \
                                               if(USBPD_OK != _retr) {goto error;}   \
                                              } while(0);

#define CHECK_CAD_FUNCTION_CALL(_function_) if(USBPD_CAD_OK != _function_)      \
  {                                   \
    _retr = USBPD_ERROR;              \
    goto error;                       \
  }

#if defined(_DEBUG_TRACE)
#define DPM_CORE_DEBUG_TRACE(_PORTNUM_, __MESSAGE__)  \
  USBPD_TRACE_Add(USBPD_TRACE_DEBUG, _PORTNUM_, 0u, (uint8_t *)(__MESSAGE__), sizeof(__MESSAGE__) - 1u);
#else
#define DPM_CORE_DEBUG_TRACE(_PORTNUM_, __MESSAGE__)
#endif /* _DEBUG_TRACE */

/* Private variables ---------------------------------------------------------*/
static OS_QUEUE_ID CADQueueId;
static OS_TASK_ID CADThread;

USBPD_ParamsTypeDef   DPM_Params[USBPD_PORT_COUNT];
/* Private function prototypes -----------------------------------------------*/

void USBPD_DPM_CADCallback(uint8_t PortNum, USBPD_CAD_EVENT State, CCxPin_TypeDef Cc);

static void USBPD_DPM_CADTaskWakeUp(void);

/**
  * @brief  Initialize the core stack (port power role, PWR_IF, CAD and PE Init procedures)
  * @retval USBPD status
  */
USBPD_StatusTypeDef USBPD_DPM_InitCore(void)
{
  /* variable to get dynamique memory allocated by usbpd stack */
  uint32_t stack_dynamemsize;
  USBPD_StatusTypeDef _retr = USBPD_OK;

  /* Check the lib selected */
  if (USBPD_TRUE != USBPD_PE_CheckLIB(LIB_ID))
  {
    _retr = USBPD_ERROR;
    goto error;
  }

  /* to get how much memory are dynamically allocated by the stack
     the memory return is corresponding to 2 ports so if the application
     managed only one port divide the value return by 2                   */
  stack_dynamemsize = USBPD_PE_GetMemoryConsumption();

  /* done to avoid warning */
  (void)stack_dynamemsize;

  DPM_Params[USBPD_PORT_0].PE_PowerRole     = DPM_Settings[USBPD_PORT_0].PE_DefaultRole;

  {
    static const USBPD_CAD_Callbacks CAD_cbs = { USBPD_DPM_CADCallback, USBPD_DPM_CADTaskWakeUp };
    /* Init CAD */
    CHECK_CAD_FUNCTION_CALL(USBPD_CAD_Init(USBPD_PORT_0, &CAD_cbs, (USBPD_SettingsTypeDef *)&DPM_Settings[USBPD_PORT_0],
                                           &DPM_Params[USBPD_PORT_0]));

    /* Enable CAD on Port 0 */
    USBPD_CAD_PortEnable(USBPD_PORT_0, USBPD_CAD_ENABLE);
  }

error :
  return _retr;
}

/**
  * @brief  Initialize the OS parts (task, queue,... )
  * @retval USBPD status
  */
uint32_t USBPD_DPM_InitOS(void *MemoryPtr)
{
  OS_INIT();
  {
    OS_CREATE_QUEUE(CADQueueId, "QCAD", USBPD_PORT_COUNT, OS_ELEMENT_SIZE);
    OS_DEFINE_TASK(CAD, USBPD_CAD_Task, OS_CAD_PRIORITY, OS_CAD_STACK_SIZE, NULL);
    OS_CREATE_TASK(CADThread, CAD, USBPD_CAD_Task,  OS_CAD_PRIORITY, OS_CAD_STACK_SIZE, (int)NULL);
  }

error:
  return _retr;
}

/**
  * @brief  Initialize the OS parts (port power role, PWR_IF, CAD and PE Init procedures)
  * @retval None
  */
/* Nothing to do kernel run is managed on CubeMX side */
/* this is obsolete in threadX context void USBPD_DPM_Run(void) */

/**
  * @brief  WakeUp CAD task
  * @retval None
  */
static void USBPD_DPM_CADTaskWakeUp(void)
{
  OS_PUT_MESSAGE_QUEUE(CADQueueId, 0xFFFF, 0);
}

/**
  * @brief  Main task for CAD layer
  * @param  argument Not used
  * @retval None
  */
DEF_TASK_FUNCTION(USBPD_CAD_Task)
{
  uint32_t _timing;
#ifdef _LOW_POWER
  UTIL_LPM_SetOffMode(LPM_CAD, UTIL_LPM_DISABLE);
#endif /* _LOW_POWER */
  for (;;)
  {
    _timing = USBPD_CAD_Process();
    OS_GETMESSAGE_QUEUE(CADQueueId, _timing);
  }
}

/**
  * @brief  CallBack reporting events on a specified port from CAD layer.
  * @param  PortNum   The handle of the port
  * @param  State     CAD state
  * @param  Cc        The Communication Channel for the USBPD communication
  * @retval None
  */
void USBPD_DPM_CADCallback(uint8_t PortNum, USBPD_CAD_EVENT State, CCxPin_TypeDef Cc)
{
#ifdef _TRACE
  USBPD_TRACE_Add(USBPD_TRACE_CADEVENT, PortNum, (uint8_t)State, NULL, 0);
#endif /* _TRACE */

  switch (State)
  {
#if defined(USBPDCORE_VPD)
    case USPPD_CAD_EVENT_VPD :
#endif /* USBPDCORE_VPD */
    case USBPD_CAD_EVENT_ATTEMC :
    case USBPD_CAD_EVENT_ATTACHED :
    {
      USBPD_DPM_UserCableDetection(PortNum, State);
      USBPD_DPM_Notification(PortNum, USBPD_NOTIFY_USBSTACK_START);
      break;
    }
    case USBPD_CAD_EVENT_DETACHED :
    case USBPD_CAD_EVENT_EMC :
    {
      USBPD_DPM_UserCableDetection(PortNum, State);
      USBPD_DPM_Notification(PortNum, USBPD_NOTIFY_USBSTACK_STOP);
      break;
    }
    default :
      /* nothing to do */
      break;
  }
}

__WEAK void USBPD_DPM_ErrorHandler(void)
{
  /* This function is called to block application execution
     in case of an unexpected behavior
     another solution could be to reset application */
  while (1u == 1u) {};
}
