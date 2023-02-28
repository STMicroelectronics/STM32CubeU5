/**
  **********************************************************************************************************************
  * @file    webserver_wifi.c
  * @author  MCD Application Team
  * @brief   This file implements the web server WiFi services.
  **********************************************************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  **********************************************************************************************************************
  */

/* Includes ----------------------------------------------------------------------------------------------------------*/
#include "webserver_wifi.h"
#include "net_connect.h"
#include "net_interface.h"
#include "mx_wifi.h"
#include "io_pattern/mx_wifi_io.h"

/* Private typedef ---------------------------------------------------------------------------------------------------*/
/* Private define ----------------------------------------------------------------------------------------------------*/
/* Private macro -----------------------------------------------------------------------------------------------------*/
/* Private variables -------------------------------------------------------------------------------------------------*/
/* The STM32 network interface in use. */
net_if_handle_t *Netif = NULL;

/* MxChip WiFi SPI handle declaration */
SPI_HandleTypeDef Wifi_SPIHandle;

/* Console buffer declaration */
extern char console_buffer[CONSOLE_BUFFER_SIZE];
/* The network interface in use. */
extern net_if_handle_t *Netif;

/* Access point credentials declaration */
static char SSID[MX_WIFI_MAX_SSID_NAME_SIZE];
static char PassWord[MX_WIFI_MAX_PSWD_NAME_SIZE];
static ap_t net_wifi_registred_hotspot[] =
{
  {NULL, NULL},
  {NULL, NULL}
};

/* Private function prototypes ---------------------------------------------------------------------------------------*/
static WebServer_StatusTypeDef Wifi_SPI_Config(void);
static void Wifi_IO_Init(void);
int32_t mx_wifi_driver(net_if_handle_t  *pnetif);
static void hnet_notify(void *context, uint32_t event_class, uint32_t event_id, void  *event_data);
void mxchip_WIFI_ISR(uint16_t isr_source);
static WebServer_StatusTypeDef wifi_get_credentials(void);

/* Functions prototypes ----------------------------------------------------------------------------------------------*/

/**
  * @brief  Initialize WiFi interface
  * @param  None
  * @retval Web Server status
  */
WebServer_StatusTypeDef webserver_wifi_init(void)
{
  /* WiFi IO configuration */
  Wifi_IO_Init();

  /* WiFi SPI initialization and configuration */
  if (Wifi_SPI_Config() != WEBSERVER_OK)
  {
    return WIFI_ERROR;
  }

  return WEBSERVER_OK;
}

/**
  * @brief  Get WiFi credentials
  * @param  None
  * @retval Web Server status
  */
static WebServer_StatusTypeDef wifi_get_credentials(void)
{
  /* Get user SSID */
  if (webserver_console_get_ssid(net_wifi_registred_hotspot, SSID) != WEBSERVER_OK)
  {
    return CONSOLE_ERROR;
  }

  /* Get user PWD */
  if (webserver_console_get_password(net_wifi_registred_hotspot, PassWord) != WEBSERVER_OK)
  {
    return CONSOLE_ERROR;
  }

  return WEBSERVER_OK;
}

/**
  * @brief  Connect WiFi interface
  * @param  None
  * @retval Web Server status
  */
WebServer_StatusTypeDef webserver_wifi_connect(void)
{
  static net_wifi_credentials_t  WifiCredentials = {
  .ssid = "SSID-Not-Found",
  .psk = "none",
  .security_mode = 0
  };

  /* start network interface */
  Netif = NetInterfaceOn(mx_wifi_driver, hnet_notify);

  /* Check if a valid WIFI interface is initialized */
  if (NET_INTERFACE_IS_WIFI(Netif))
  {
    /* Scan available WIFIs */
    scan_cmd(0, NULL);

    /* Get user credentials */
    wifi_get_credentials();

    /* Scan available WIFIs */
    NetWifiGetDefaultStation(&WifiCredentials, net_wifi_registred_hotspot);

    /* Connect to selected WIFI */
    NetInterfaceConnect(Netif, true, &WifiCredentials, NET_WIFI_MODE_STA);
  }

  return WEBSERVER_OK;
}

/**
  * @brief  Handles net notifications
  * @param  None
  * @retval None
  */
static void hnet_notify(void *context, uint32_t event_class, uint32_t event_id, void  *event_data)
{
  net_if_handle_t *netif = context;
  ((void)event_data);

  if (NET_EVENT_STATE_CHANGE == event_class)
  {
    net_state_t new_state = (net_state_t) event_id;
    switch (new_state)
    {
      /* Initialized state */
    case NET_STATE_INITIALIZED:
      {
        printf("- Network Interface initialized:\r\n");
        break;
      }

      /* Started state */
    case NET_STATE_STARTING:
      {
        printf("- Network Interface starting:\r\n");
        break;
      }

      /* Ready state */
    case NET_STATE_READY:
      {
        printf("- Network Interface ready:\r\n");
        printf("   - Device Name : %s.\r\n", netif->DeviceName);
        printf("   - Device ID   : %s.\r\n", netif->DeviceID);
        printf("   - Device Version : %s.\r\n", netif->DeviceVer);
        printf("   - MAC address: %x.%x.%x.%x.%x.%x\r\n",
               netif->macaddr.mac[0], netif->macaddr.mac[1], netif->macaddr.mac[2],
               netif->macaddr.mac[3], netif->macaddr.mac[4], netif->macaddr.mac[5]);
        break;
      }

      /* Connecting state */
    case NET_STATE_CONNECTING:
      {
        printf("- Network Interface connecting:\r\n");
        break;
      }

      /* Connected state */
    case NET_STATE_CONNECTED:
      {
        printf("- Network Interface connected:\r\n");
        printf("   - IP address :  %s. \r\n", NET_NTOA(&netif->ipaddr));
        break;
      }

      /* Disconnecting state */
    case NET_STATE_DISCONNECTING:
      printf("- Network Interface disconnecting\r\n");
      break;

      /* Stopping state */
    case NET_STATE_STOPPING:
      {
        printf("- Network Interface stopping\r\n");
        break;
      }

      /* De-Initialized state */
    case NET_STATE_DEINITIALIZED:
      {
        printf("- Network Interface de-initialized\r\n");
        break;
      }

      /* Lost state */
    case NET_STATE_CONNECTION_LOST:
      {
        printf("- Network Interface connection lost\r\n");
        break;
      }

    default:
      {
        break;
      }
    }
  }
}

/**
  * @brief SPI2 Initialization Function
  * @param None
  * @retval None
  */
static WebServer_StatusTypeDef Wifi_SPI_Config(void)
{
  /* Set SPI instance */
  Wifi_SPIHandle.Instance                        = SPI2;

  /* Set parameter to be configured */
  Wifi_SPIHandle.Init.Mode                       = SPI_MODE_MASTER;
  Wifi_SPIHandle.Init.Direction                  = SPI_DIRECTION_2LINES;
  Wifi_SPIHandle.Init.DataSize                   = SPI_DATASIZE_8BIT;
  Wifi_SPIHandle.Init.CLKPolarity                = SPI_POLARITY_LOW;
  Wifi_SPIHandle.Init.CLKPhase                   = SPI_PHASE_1EDGE;
  Wifi_SPIHandle.Init.NSS                        = SPI_NSS_SOFT;
  Wifi_SPIHandle.Init.BaudRatePrescaler          = SPI_BAUDRATEPRESCALER_8;
  Wifi_SPIHandle.Init.FirstBit                   = SPI_FIRSTBIT_MSB;
  Wifi_SPIHandle.Init.TIMode                     = SPI_TIMODE_DISABLE;
  Wifi_SPIHandle.Init.CRCCalculation             = SPI_CRCCALCULATION_DISABLE;
  Wifi_SPIHandle.Init.CRCPolynomial              = 0x7;
  Wifi_SPIHandle.Init.NSSPMode                   = SPI_NSS_PULSE_DISABLE;
  Wifi_SPIHandle.Init.NSSPolarity                = SPI_NSS_POLARITY_LOW;
  Wifi_SPIHandle.Init.FifoThreshold              = SPI_FIFO_THRESHOLD_01DATA;
  Wifi_SPIHandle.Init.MasterSSIdleness           = SPI_MASTER_SS_IDLENESS_00CYCLE;
  Wifi_SPIHandle.Init.MasterInterDataIdleness    = SPI_MASTER_INTERDATA_IDLENESS_00CYCLE;
  Wifi_SPIHandle.Init.MasterReceiverAutoSusp     = SPI_MASTER_RX_AUTOSUSP_DISABLE;
  Wifi_SPIHandle.Init.MasterKeepIOState          = SPI_MASTER_KEEP_IO_STATE_DISABLE;
  Wifi_SPIHandle.Init.IOSwap                     = SPI_IO_SWAP_DISABLE;
  Wifi_SPIHandle.Init.ReadyMasterManagement      = SPI_RDY_MASTER_MANAGEMENT_INTERNALLY;
  Wifi_SPIHandle.Init.ReadyPolarity              = SPI_RDY_POLARITY_HIGH;

  /* SPI initialization */
  if (HAL_SPI_Init(&Wifi_SPIHandle) != HAL_OK)
  {
    return PERIPH_ERROR;
  }

  return WEBSERVER_OK;
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void Wifi_IO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(MXCHIP_RESET_GPIO_Port, MXCHIP_RESET_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(MXCHIP_NSS_GPIO_Port, MXCHIP_NSS_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin : MXCHIP_FLOW_Pin */
  GPIO_InitStruct.Pin = MXCHIP_FLOW_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(MXCHIP_FLOW_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : MXCHIP_RESET_Pin */
  GPIO_InitStruct.Pin = MXCHIP_RESET_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(MXCHIP_RESET_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PD14 */
  GPIO_InitStruct.Pin = GPIO_PIN_14;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : MXCHIP_NSS_Pin */
  GPIO_InitStruct.Pin = MXCHIP_NSS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(MXCHIP_NSS_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : MXCHIP_NOTIFY_Pin */
  GPIO_InitStruct.Pin = MXCHIP_NOTIFY_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(MXCHIP_NOTIFY_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI14_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(EXTI14_IRQn);

  HAL_NVIC_SetPriority(EXTI15_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(EXTI15_IRQn);
}

/**
  * @brief GPIO EXTI callback function
  * @param None
  * @retval None
  */
void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin)
{
  switch (GPIO_Pin)
  {
    /* MXCHIP flow pin notification */
    case MXCHIP_FLOW_Pin:
    {
      mxchip_WIFI_ISR(MXCHIP_FLOW_Pin);
      break;
    }

    /* MXCHIP notify pin notification */
    case (MXCHIP_NOTIFY_Pin):
    {
      mxchip_WIFI_ISR(MXCHIP_NOTIFY_Pin);
      break;
    }

  default:
    {
      break;
    }
  }
}

/**
  * @brief Rx Transfer completed callback.
  * @param hspi : pointer to a SPI_HandleTypeDef structure that contains the configuration information for SPI
  *                    module.
  * @retval None
  */
void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi)
{
  if (hspi == &Wifi_SPIHandle)
  {
    HAL_SPI_TransferCallback(hspi);
  }
}

/**
  * @brief Tx Transfer completed callback.
  * @param hspi : pointer to a SPI_HandleTypeDef structure that contains the configuration information for SPI
  *                    module.
  * @retval None
  */
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
  if (hspi == &Wifi_SPIHandle)
  {
    HAL_SPI_TransferCallback(hspi);
  }
}

/**
  * @brief Tx and Rx Transfer completed callback.
  * @param hspi : pointer to a SPI_HandleTypeDef structure that contains the configuration information for SPI
  *                    module.
  * @retval None
  */
void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
  if (hspi == &Wifi_SPIHandle)
  {
    HAL_SPI_TransferCallback(hspi);
  }
}
