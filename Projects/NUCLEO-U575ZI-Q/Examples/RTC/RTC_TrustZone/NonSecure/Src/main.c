/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    RTC/TrustZoneEnabled/NonSecure/Src/main.c
  * @author  MCD Application Team
  * @brief   Main program body (non-secure)
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stm32u5xx_nucleo.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

RTC_HandleTypeDef hrtc;

/* USER CODE BEGIN PV */
volatile uint32_t SecureErrorCounter = 0;
volatile uint32_t AlarmB;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
static void MX_RTC_Init(void);
/* USER CODE BEGIN PFP */

static void RTC_TestBackUpRegisters(void);
static void RTC_TryDesactivateAlarmA(void);
static void SecureError_Check(uint32_t expectedCount);
static void SecureError_Clear(void);
void SecureFault_Callback(void);
void SecureError_Callback(void);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

 /* Clear secure error counter */
  SecureError_Clear();
  AlarmB = 0;

  /* Init LED1 and LED3 */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED3);

  /* USER CODE END Init */

  /* USER CODE BEGIN SysInit */

  /* Register SecureFault callback defined in non-secure and to be called by secure handler */
  SECURE_RegisterCallback(SECURE_FAULT_CB_ID, (void *)SecureFault_Callback);

  /* Register SecureError callback defined in non-secure and to be called by secure handler */
  SECURE_RegisterCallback(GTZC_ERROR_CB_ID, (void *)SecureError_Callback);

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_RTC_Init();
  /* USER CODE BEGIN 2 */

   SecureError_Check(0);

  /* Test BackUp registers Read and Write */
  RTC_TestBackUpRegisters();

  /* Alarm A is secure, try to deactivate it! */
  RTC_TryDesactivateAlarmA();

  /* Wait for Alarm A, Alarm B and WUT */
  while ((AlarmB == 0) || (SECURE_GetRtcSecureStatus() == 0))
  {
    HAL_Delay(250);
  }

   /* End of example LED1 on */
  BSP_LED_On(LED1);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  RTC_AlarmTypeDef sAlarm = {0};

  /* USER CODE BEGIN RTC_Init 1 */
  HAL_NVIC_SetPriority(RTC_IRQn, 0x4, 0); /* Not yet managed by cubeMX */
  HAL_NVIC_EnableIRQ(RTC_IRQn);  /* Not yet managed by cubeMX */
  hrtc.Instance = RTC;  /* Not yet managed by cubeMX */

  /* USER CODE END RTC_Init 1 */

  /** Enable the Alarm B
  */
  sAlarm.AlarmTime.Hours = 0x2;
  sAlarm.AlarmTime.Minutes = 0x20;
  sAlarm.AlarmTime.Seconds = 0x6;
  sAlarm.AlarmTime.SubSeconds = 0x0;
  sAlarm.AlarmMask = RTC_ALARMMASK_NONE;
  sAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
  sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
  sAlarm.AlarmDateWeekDay = 0x1;
  sAlarm.Alarm = RTC_ALARM_B;
  if (HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

/* USER CODE BEGIN 4 */
/**
  * @brief  Alarm B callback
  * @param  hrtc : RTC handle
  * @retval None
  */
void HAL_RTCEx_AlarmBEventCallback(RTC_HandleTypeDef *hrtc)
{
  AlarmB = 1;
}

/**
  * @brief  alarm A is secure, try to deactivate it !
  * @param  None
  * @retval None
  */
static void RTC_TryDesactivateAlarmA(void)
{
  HAL_RTC_DeactivateAlarm(&hrtc, RTC_ALARM_A);
  /* check the error for secure and non-secure registers */
  SecureError_Check(1);
}

/**
  * @brief  Check Backup Registers : 3 different zones,  3 different accesses.
  *         Secure errors generated by GTZC because of illegal access are checked.
  * @param  None
  * @retval None
  */
static void  RTC_TestBackUpRegisters(void)
{
  RTC_SecureStateTypeDef secureState;
  uint32_t i;

  /* Get limits of zones */
  HAL_RTCEx_SecureModeGet(&hrtc, &secureState);

  for (i = 0; i < secureState.backupRegisterStartZone2; i++)
  {
    /* Test Zone 1 : NS Read no (read as zero)  Write no */
    /* Write backup register */
    HAL_RTCEx_BKUPWrite(&hrtc, i, 0xFFFFFFFFu);

    /* Wait for secure fault interrupt and check it */
    HAL_Delay(1);
    SecureError_Check(1);

    /* Read backup register */
    if (HAL_RTCEx_BKUPRead(&hrtc, i) != 0)
    {
      Error_Handler();
    }

    /* Wait for secure fault interrupt */
    HAL_Delay(1);
    SecureError_Check(1);
  }

  for (i = secureState.backupRegisterStartZone2; i < secureState.backupRegisterStartZone3; i++)
  {
    /* Test Zone 2 : NS Read yes Write no */
    /* Write backup register */
    HAL_RTCEx_BKUPWrite(&hrtc, i, 0xFFFFFFFFu);

    /* Wait for secure fault interrupt and check it */
    HAL_Delay(1);
    SecureError_Check(1);

    /* Read backup register */
    if (HAL_RTCEx_BKUPRead(&hrtc, i) != i) /* Write by Secure SW */
    {
      Error_Handler();
    }

    /* Wait for secure fault interrupt */
    HAL_Delay(1);
    SecureError_Check(0);
  }

  for (i = secureState.backupRegisterStartZone3; i < RTC_BKP_NB; i++)
  {
    /* Test Zone 3 : NS Read yes Write yes */
     /* Verify backup register are well configured*/
    if (HAL_RTCEx_BKUPRead(&hrtc, i) != i) /* Write by Secure SW */
    {
      Error_Handler();
    }

    /* Write backup register */
    HAL_RTCEx_BKUPWrite(&hrtc, i, 0xFFFFFFFFu);
    /* Read backup register */
    if (HAL_RTCEx_BKUPRead(&hrtc, i) != 0xFFFFFFFFu)
    {
      Error_Handler();
    }

    /* Wait for secure fault interrupt and check it */
    HAL_Delay(1);
    SecureError_Check(0);
  }
}

/**
  * @brief  Callback called by secure code following a secure fault interrupt
  * @note   This callback is called by secure code thanks to the registration
  *         done by the non-secure application with non-secure callable API
  *         SECURE_RegisterCallback(SECURE_FAULT_CB_ID, (void *)SecureFault_Callback);
  * @retval None
  */
void SecureFault_Callback(void)
{
  /* Go to infinite loop when Secure fault generated by IDAU/SAU check */
  /* because of illegal access */
  while (1)
  {
  }
}

/**
  * @brief  Callback called by secure code following a GTZC TZIC secure interrupt (GTZC_IRQn)
  * @note   This callback is called by secure code thanks to the registration
  *         done by the non-secure application with non-secure callable API
  *         SECURE_RegisterCallback(GTZC_ERROR_CB_ID, (void *)SecureError_Callback);
  * @retval None
  */
void SecureError_Callback(void)
{
  /* Secure error generated by GTZC because of illegal access */
  SecureErrorCounter++;
}

/**
  * @brief  Check and clear secure error counter
  * @param  expectedCount expected number of secure error .
  * @note
  * @retval None
  */
static void SecureError_Check(uint32_t expectedCount)
{
  if (SecureErrorCounter != expectedCount)
  {
    Error_Handler();
  }
  /* Clear SecureErrorCounter */
  SecureError_Clear();
}

/**
  * @brief  Clear secure error counter
  * @note
  * @retval None
  */
static void SecureError_Clear(void)
{
  SecureErrorCounter = 0;
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  /* LED3 on */
  BSP_LED_On(LED3);

  while(1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
