/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    app_netxduo.h
  * @author  MCD Application Team
  * @brief   NetXDuo applicative header file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __APP_NETXDUO_H__
#define __APP_NETXDUO_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "nx_api.h"

/* Private includes ----------------------------------------------------------*/

/* USER CODE BEGIN Includes */
#include <inttypes.h>
#include "main.h"
#include "nx_ip.h"
#include "nxd_dns.h"
#include "mx_wifi.h"
#include "nx_driver_emw3080.h"
#include "msg.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
#define PAYLOAD_SIZE                         NX_DRIVER_PACKET_SIZE
#define NX_PACKET_POOL_SIZE                  ((PAYLOAD_SIZE + sizeof(NX_PACKET)) * 20)

#define ARP_MEMORY_SIZE                      DEFAULT_MEMORY_SIZE

#define MAIN_THREAD_STACK_SIZE               (3 * DEFAULT_MEMORY_SIZE)
#define MAIN_THREAD_PRIORITY                 (NETX_IP_THREAD_PRIORITY + 3)

#define MAIN2_THREAD_STACK_SIZE              (2 * DEFAULT_MEMORY_SIZE)
#define MAIN2_THREAD_PRIORITY                (NETX_IP_THREAD_PRIORITY + 3)


#define BSD_COMPAT_LAYER_THREAD_STACK_SIZE   (2 * DEFAULT_MEMORY_SIZE)
#define BSD_COMPAT_LAYER_THREAD_PRIORITY     (NETX_IP_THREAD_PRIORITY + 2)

#define NETWORK_BASICS_THREAD_STACK_SIZE     (6 * DEFAULT_MEMORY_SIZE)
#define NETWORK_BASICS_THREAD_PRIORITY       (NETX_IP_THREAD_PRIORITY + 2)


#define NULL_ADDRESS                         ((void*)0)
/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
#define PRINT_IP_ADDRESS(addr)                                    \
  do {                                                            \
    MSG_INFO("%" PRIu32 ".%" PRIu32 ".%" PRIu32 ".%" PRIu32 "\n", \
            ((uint32_t)(addr) >> 24) & 0xff,                      \
            ((uint32_t)(addr) >> 16) & 0xff,                      \
            ((uint32_t)(addr) >> 8) & 0xff,                       \
            ((uint32_t)(addr) & 0xff));                           \
  } while(0)
/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
UINT MX_NetXDuo_Init(VOID *memory_ptr);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* USER CODE BEGIN 1 */
extern TX_BYTE_POOL *AppBytePool;
extern NX_IP IpInstance;


int32_t http_download_cmd(int32_t argc, char *argv[]);

int32_t ping_cmd(int32_t argc, char *argv[]);

int32_t scan_cmd(int32_t argc, char *argv[]);

int32_t test_echo_server(int32_t argc, char *argv[]);

/* USER CODE END 1 */

#ifdef __cplusplus
}
#endif
#endif /* __APP_NETXDUO_H__ */
