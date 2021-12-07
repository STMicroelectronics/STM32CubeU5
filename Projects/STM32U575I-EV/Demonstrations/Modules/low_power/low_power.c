/**
  ******************************************************************************
  * @file    sad.c
  * @author  MCD Application Team
  * @brief   Sound Acquisiton Detection (SAD) implementation
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

#include <string.h>
#include <stdio.h>

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "main.h"
#include "stm32u5xx_hal.h"
#include "stm32u5xx.h"
#include "stm32u575i_eval.h"
#include "stm32u575i_eval_ts.h"
#include "stm32u575i_eval_lcd.h"
#include "k_config.h"
#include "k_module.h"
#include "k_menu.h"

/* Private typedef ----------------------------------------------------------*/
/* Private constants ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
KMODULE_RETURN _LowPowerDemoExec(void);
void LowPowerDemo(void);
void Lowpower_UserAction(uint8_t sel);
/* Private Variable ----------------------------------------------------------*/
const tMenuItem LowPowerMenuMenuItems[] =
{
  {NULL, 14, 30, TYPE_EXEC, MODULE_NONE, LowPowerDemo, NULL, NULL, NULL, NULL  },
};

const tMenu LowPowerMenu = {
  NULL, LowPowerMenuMenuItems, countof(LowPowerMenuMenuItems), TYPE_EXEC, 1, 1 };

/* used to exit application */
static __IO uint8_t user_event=0;
static __IO uint8_t user_action=0;

/* Private typedef -----------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
const K_ModuleItem_Typedef ModuleLowPower =
{
  MODULE_LOWPOWER,
  NULL,
  _LowPowerDemoExec,
  NULL,
  NULL
};

/**
* @brief  Run the 8 uart application
* @param  None.
* @note   run and display information about the uart transaction.
* @retval KMODULE_RETURN status.
*/
KMODULE_RETURN _LowPowerDemoExec(void)
{
  /* Prepare the main MMI */
  kMenu_Execute(LowPowerMenu);
  
  /* Execute the app 8uart */
  /* App initialization    */
  return KMODULE_OK;
}
/* Private typedef ----------------------------------------------------------*/
/* Private constants ----------------------------------------------------------*/
/* Private Variables ---------------------------------------------------------*/
/* Used to exit application */
static __IO uint8_t UserEvent=0;
static __IO uint8_t UserEntry=0;
static uint8_t LowPowerIndex = 0;
static uint8_t count=0;
/* RTC handler declaration */
extern RTC_HandleTypeDef RtcHandle;

static uint8_t LowpowerStrTab[7][10]={" SLEEP "," STOP0 "," STOP1 "," STOP2 "," STOP3 ","STANDBY","SHUTDWN"};

void EnterLowPowerMode(uint8_t lowpower_index);
/* Private typedef -----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
void LowPowerDemo(void)
{
  uint32_t joyState = JOY_NONE;
  uint32_t Button   = 0;
  /* IMPLEMENT APPLICATION HERE */
  
  UTIL_LCD_SetFont(&Font24);
  UTIL_LCD_Clear(UTIL_LCD_COLOR_ST_GRAY_DARK);
  
  UTIL_LCD_FillRect(0, 0, 320, 24,UTIL_LCD_COLOR_ST_BLUE_DARK);
  
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_ST_BLUE_DARK);
  
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
  
  UTIL_LCD_DisplayStringAt(0, 0, (uint8_t *)"Low Power Module ", CENTER_MODE);
  
  /* Control Panel */
  
  UTIL_LCD_FillRect(2, 30, 316, 90,UTIL_LCD_COLOR_WHITE);  
  
  UTIL_LCD_SetFont(&Font20);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_ST_GRAY);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_ST_PURPLE);
  UTIL_LCD_DisplayStringAt(4, 36, (uint8_t *)"Low Power Mode", CENTER_MODE);
  
  
  UTIL_LCD_SetFont(&Font16);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_ST_BLUE_DARK);
  UTIL_LCD_DisplayStringAt(4, 82, (uint8_t *)" SLEEP ", CENTER_MODE);
  
  /* Button*/
  UTIL_LCD_DrawRect(110, 76, 100, 26,UTIL_LCD_COLOR_RED); 
  
  UTIL_LCD_DrawRect(60, 80, 42, 20,UTIL_LCD_COLOR_RED);
  UTIL_LCD_DrawRect(59, 79, 44, 22,UTIL_LCD_COLOR_RED);
  UTIL_LCD_FillRect(61, 81, 40, 18,UTIL_LCD_COLOR_ST_YELLOW); 
  UTIL_LCD_FillRect(62, 82, 38, 16,UTIL_LCD_COLOR_BLACK);  
  
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLACK);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_ST_YELLOW);
  UTIL_LCD_DisplayStringAt(72, 82, (uint8_t *)"<<", LEFT_MODE);    
  
  UTIL_LCD_DrawRect(220, 80, 42, 20,UTIL_LCD_COLOR_BLACK);
  UTIL_LCD_FillRect(221, 81, 40, 18,UTIL_LCD_COLOR_ST_YELLOW); 
  UTIL_LCD_FillRect(222, 82, 38, 16,UTIL_LCD_COLOR_BLACK);  
  
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLACK);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_ST_YELLOW);
  UTIL_LCD_DisplayStringAt(232, 82, (uint8_t *)">>", LEFT_MODE); 
  
  UTIL_LCD_SetFont(&Font12);  
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_MAGENTA);
  
  UTIL_LCD_DisplayStringAt(4, 104, (uint8_t *)"(Use User Button for wakeup)", CENTER_MODE); 
  
  /* Guide Panel */
  UTIL_LCD_FillRect(2, 126, 316, 90,UTIL_LCD_COLOR_WHITE);    
  
  UTIL_LCD_SetFont(&Font16);  
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_ST_PURPLE);
  UTIL_LCD_DisplayStringAt(4, 130, (uint8_t *)"You can measure ", CENTER_MODE);
  UTIL_LCD_DisplayStringAt(4, 150, (uint8_t *)"the MCU Power Consumption", CENTER_MODE);
  UTIL_LCD_DisplayStringAt(4, 170, (uint8_t *)"using IDD (JP19) ", CENTER_MODE);
  
  UTIL_LCD_DrawRect(60, 186, 190, 26,UTIL_LCD_COLOR_BLACK);
  UTIL_LCD_FillRect(61, 187, 188, 24,UTIL_LCD_COLOR_ST_YELLOW); 
  UTIL_LCD_FillRect(62, 188, 186, 22,UTIL_LCD_COLOR_BLACK);  
  
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLACK);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_ST_YELLOW);
  UTIL_LCD_DisplayStringAt(63, 192, (uint8_t *)"Enter Low Power", LEFT_MODE); 
  
  /* Log Panel */
  UTIL_LCD_FillRect(2, 220, 316, 18,UTIL_LCD_COLOR_WHITE);    
  
  UTIL_LCD_SetFont(&Font16);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_GREEN);
  
  UTIL_LCD_DisplayStringAt(40, 220, (uint8_t *) "System in Run mode ", LEFT_MODE);
  
  /* Return button */  
  UTIL_LCD_DrawRect(280, 220, 40, 18,UTIL_LCD_COLOR_BLACK);
  UTIL_LCD_FillRect(281, 221, 38, 16,UTIL_LCD_COLOR_ST_YELLOW); 
  UTIL_LCD_FillRect(282, 222, 36, 14,UTIL_LCD_COLOR_BLACK);  
  
  UTIL_LCD_SetFont(&Font12);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLACK);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_ST_YELLOW);
  UTIL_LCD_DisplayStringAt(286, 224, (uint8_t *)"BCK", LEFT_MODE);  
  
  /* Use Joystick interrupt for Wakeup*/
  BSP_JOY_Init(JOY1, JOY_MODE_EXTI, JOY_ALL);
  
  do 
  {
    joyState = JOY_NONE;
    while (joyState == JOY_NONE)
    {  
      joyState = BSP_JOY_GetState(JOY1);
      UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
      UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_MAGENTA);
      
      if (count%4 == 0)
      {
        UTIL_LCD_DisplayStringAt(20, 222, (uint8_t *) "/", LEFT_MODE);      
        UTIL_LCD_DisplayStringAt(260, 222, (uint8_t *) "/", LEFT_MODE);      
        
      }
      if (count%4 == 1)
      {
        UTIL_LCD_DisplayStringAt(20, 222, (uint8_t *) "-", LEFT_MODE);      
        UTIL_LCD_DisplayStringAt(260, 222, (uint8_t *) "-", LEFT_MODE);      
      }
      if (count%4 == 2)
      {
        UTIL_LCD_DisplayStringAt(20, 222, (uint8_t *) "\\", LEFT_MODE);      
        UTIL_LCD_DisplayStringAt(260, 222, (uint8_t *) "\\", LEFT_MODE);      
        
      }
      if (count%4 == 3)
      {
        UTIL_LCD_DisplayStringAt(20, 222, (uint8_t *) "|", LEFT_MODE);      
        UTIL_LCD_DisplayStringAt(260, 222, (uint8_t *) "|", LEFT_MODE);      
      }
      
      
      count++;
      HAL_Delay(200);
      
    }
    
    /* anti bounding assert */ 
    while ( BSP_JOY_GetState(JOY1) != JOY_NONE);
    HAL_Delay(100);
    
    if (joyState != JOY_SEL)
    {    
      Button++;
      if (Button == 4)
      {
        Button = 0;
      }
      UTIL_LCD_DrawRect(60, 80, 42, 20,UTIL_LCD_COLOR_BLACK);
      UTIL_LCD_DrawRect(59, 79, 44, 22,UTIL_LCD_COLOR_WHITE);
      UTIL_LCD_DrawRect(220, 80, 42, 20,UTIL_LCD_COLOR_BLACK);
      UTIL_LCD_DrawRect(219, 79, 44, 22,UTIL_LCD_COLOR_WHITE);
      UTIL_LCD_DrawRect(60, 186, 190, 26,UTIL_LCD_COLOR_BLACK);
      UTIL_LCD_DrawRect(59, 185, 192, 28,UTIL_LCD_COLOR_WHITE);
      UTIL_LCD_DrawRect(280, 220, 40, 18,UTIL_LCD_COLOR_BLACK);
      UTIL_LCD_DrawRect(279, 219, 41, 20,UTIL_LCD_COLOR_WHITE);
      
      switch(Button)
      {
      case 0 :
        UTIL_LCD_DrawRect(60, 80, 42, 20,UTIL_LCD_COLOR_RED);
        UTIL_LCD_DrawRect(59, 79, 44, 22,UTIL_LCD_COLOR_RED);
        break;
      case 1 :
        UTIL_LCD_DrawRect(220, 80, 42, 20,UTIL_LCD_COLOR_RED);
        UTIL_LCD_DrawRect(219, 79, 44, 22,UTIL_LCD_COLOR_RED);
        break;
      case 2 :
        UTIL_LCD_DrawRect(60, 186, 190, 26,UTIL_LCD_COLOR_RED);
        UTIL_LCD_DrawRect(59, 185, 192, 28,UTIL_LCD_COLOR_RED);
        break;
      case 3 :
        UTIL_LCD_DrawRect(280, 220, 40, 18,UTIL_LCD_COLOR_RED);
        UTIL_LCD_DrawRect(279, 219, 41, 20,UTIL_LCD_COLOR_RED);
        break;        
      default:
        break;
      }
    }
    
    
    
    if (joyState == JOY_SEL) 
    {
      
      
      if (Button == 0)
      {
        
        if (LowPowerIndex > 0)
        {
          LowPowerIndex--;
          
        }
        UTIL_LCD_SetFont(&Font16);
        UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
        UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_ST_BLUE_DARK);
        UTIL_LCD_DisplayStringAt(4, 82, LowpowerStrTab[LowPowerIndex], CENTER_MODE); 
        HAL_Delay(100);
        
      }
      if (Button == 1)
      {
        
        if (LowPowerIndex < 6)
        {
          LowPowerIndex++;
        }
        
        UTIL_LCD_SetFont(&Font16);
        UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
        UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_ST_BLUE_DARK);
        UTIL_LCD_DisplayStringAt(4, 82, LowpowerStrTab[LowPowerIndex], CENTER_MODE); 
        HAL_Delay(100);
      }
      
      if (Button == 2)
      {
        
        
        UTIL_LCD_SetFont(&Font16);
        UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
        UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_RED);
        UTIL_LCD_DisplayStringAt(40, 220, (uint8_t *) "System in Low Power ", LEFT_MODE);
        
        EnterLowPowerMode(LowPowerIndex); 
        
        
        UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
        UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_GREEN);
        
        UTIL_LCD_DisplayStringAt(40, 220, (uint8_t *) "System in Run mode ", LEFT_MODE); 
        
      }
      if (Button == 3)
      {
        break;  
      }
    }
    
    
  } while (1); 
}

/**
* @brief  Get User action
* @param  sel User selection (JOY_SEL,...)
* @note   This example is the only way to get user information.
* @retval None
*/
void Lowpower_UserAction(uint8_t sel)
{
  UserEvent = sel;
  UserEntry = 1;
}



/**
* @brief  Enter low power mode
* @param  None
* @retval None
*/
void EnterLowPowerMode(uint8_t lowpower_index)
{
  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();
  
  
  
  if ((lowpower_index == 4) || (lowpower_index == 5) || (lowpower_index == 6))
  {

    HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN2_HIGH_1);
    
    /* Clear wake up flag */
    while (__HAL_PWR_GET_FLAG(PWR_WAKEUP_FLAG1) || __HAL_PWR_GET_FLAG(PWR_WAKEUP_FLAG2) || \
      __HAL_PWR_GET_FLAG(PWR_WAKEUP_FLAG3) || __HAL_PWR_GET_FLAG(PWR_WAKEUP_FLAG4) || \
        __HAL_PWR_GET_FLAG(PWR_WAKEUP_FLAG5) || __HAL_PWR_GET_FLAG(PWR_WAKEUP_FLAG6) || \
          __HAL_PWR_GET_FLAG(PWR_WAKEUP_FLAG7) || __HAL_PWR_GET_FLAG(PWR_WAKEUP_FLAG8))
    {
      __HAL_PWR_CLEAR_FLAG(PWR_WAKEUP_ALL_FLAG);
    }
    
  }
  if ((lowpower_index  == 0) ||(lowpower_index  == 1) || (lowpower_index == 2) || (lowpower_index == 3))
  {
    
    BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI);

    
  }
  
    HAL_NVIC_DisableIRQ(EXTI5_IRQn);
    HAL_NVIC_DisableIRQ(RTC_IRQn);
  switch (lowpower_index) 
  {
  case 0:
    /* Suspend Tick increment */
    HAL_SuspendTick();
    /* Enter the CPU to SLEEP mode */
    HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI) ;
    /* Resume Tick increment */
    HAL_ResumeTick();
    break;
  case 1:
    /* Enter the SYSTEM to STOP 0 mode */
    HAL_PWR_EnterSTOPMode(PWR_MAINREGULATOR_ON, PWR_STOPENTRY_WFI);
    HAL_NVIC_SystemReset();
    break;    
  case 2:
    /* Enter the SYSTEM to STOP 1 mode */
    HAL_PWREx_EnterSTOP1Mode(PWR_STOPENTRY_WFI);
    HAL_NVIC_SystemReset();
    break; 
  case 3:
    /* Enter the SYSTEM to STOP 2 mode */
    HAL_PWREx_EnterSTOP2Mode(PWR_STOPENTRY_WFI);
    HAL_NVIC_SystemReset();
    break; 
  case 4:
    /* STOP3 wake up global interrupt configuration */
    HAL_NVIC_SetPriority(PWR_S3WU_IRQn, 7, 7);
    HAL_NVIC_EnableIRQ(PWR_S3WU_IRQn);
    /* Enter the SYSTEM to STOP 3 mode */
    HAL_PWREx_EnterSTOP3Mode(PWR_STOPENTRY_WFI);
    HAL_NVIC_SystemReset();
    break; 
  case 5:
    /* Enter the SYSTEM to STANDBY mode */
    HAL_PWR_EnterSTANDBYMode();
    break; 
  case 6:
    /* Enter the SYSTEM to SHUTDOWN mode */
    HAL_PWREx_EnterSHUTDOWNMode();
    break; 
  default:
    
    break; 
  }

  
}




