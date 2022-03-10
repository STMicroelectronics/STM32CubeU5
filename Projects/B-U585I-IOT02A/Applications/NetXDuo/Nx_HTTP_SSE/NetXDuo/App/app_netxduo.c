/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    app_netxduo.c
  * @author  MCD Application Team
  * @brief   NetXDuo applicative file
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

/* Includes ------------------------------------------------------------------*/
#include "app_netxduo.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "app_azure_rtos.h"
#include "main.h"
#include "httpserver_sentevents_socket.h"
#include "nxd_dns.h"
#include "nxd_http_client.h"
#include "mx_wifi.h"
#include "nx_ip.h"
#include "msg.h"
#include <stdbool.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
static TX_THREAD AppMainThread;
static TX_THREAD AppMain2Thread;
static __IO bool AppMain2ThreadRunning = true;

static NX_PACKET_POOL AppPacketPool;

NX_IP IpInstance;

static NX_DHCP DhcpClient;
static TX_SEMAPHORE DhcpSemaphore;

NX_DNS DnsClient;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
#if 0
static UINT DHCP_Server_start(void);
static VOID DHCP_Server_Thread_Entry(ULONG thread_input);
#endif

static VOID App_Main_Thread_Entry(ULONG thread_input);
static VOID App_Main2_Thread_Entry(ULONG thread_input);

static VOID ip_address_change_notify_callback(NX_IP *ip_instance, VOID *ptr);
/* USER CODE END PFP */
/**
  * @brief  Application NetXDuo Initialization.
  * @param memory_ptr: memory pointer
  * @retval int
  */
UINT MX_NetXDuo_Init(VOID *memory_ptr)
{
  UINT ret = NX_SUCCESS;
  TX_BYTE_POOL *byte_pool = (TX_BYTE_POOL*)memory_ptr;

   /* USER CODE BEGIN App_NetXDuo_MEM_POOL */
  (void)byte_pool;
  /* USER CODE END App_NetXDuo_MEM_POOL */

  /* USER CODE BEGIN MX_NetXDuo_Init */

  MSG_INFO("Nx_HTTP_SSE application started..\n");
  MSG_INFO("# build: %s-%s, %s, %s %s\n",
           "SPI",
           "RTOS",
           "Network on module",
           __TIME__, __DATE__);

  /* Initialize the NetX system. */
  nx_system_initialize();

  /* Allocate the memory for packet_pool.  */
  {
    const ULONG pool_size = NX_PACKET_POOL_SIZE;
    VOID *pool_start;

    if (tx_byte_allocate(byte_pool, &pool_start, pool_size, TX_NO_WAIT) != TX_SUCCESS)
    {
      return TX_POOL_ERROR;
    }

    /* Create the Packet pool to be used for packet allocation */
    ret = nx_packet_pool_create(&AppPacketPool, "Main Packet Pool", PAYLOAD_SIZE, pool_start, pool_size);

    if (ret != NX_SUCCESS)
    {
      return NX_NOT_ENABLED;
    }
    MSG_DEBUG("nx_packet_pool_create() done\n");
  }

  /* Allocate the memory for Ip_Instance */
  {
    const ULONG stack_size = NETX_IP_THREAD_STACK_SIZE;
    VOID *stack_ptr;
    if (tx_byte_allocate(byte_pool, &stack_ptr, stack_size, TX_NO_WAIT) != TX_SUCCESS)
    {
      return TX_POOL_ERROR;
    }

    /* Create the main NX_IP instance. */
    ret = nx_ip_create(&IpInstance, "NetX IP Instance 0", 0, 0,
                       &AppPacketPool, nx_driver_emw3080_entry,
                       stack_ptr, stack_size, NETX_IP_THREAD_PRIORITY);

    if (ret != NX_SUCCESS)
    {
      return NX_NOT_ENABLED;
    }
    MSG_DEBUG("nx_ip_create() done\n");
  }

  {
    ret = nx_ip_address_change_notify(&IpInstance, ip_address_change_notify_callback, NULL);
    if (ret != NX_SUCCESS)
    {
      Error_Handler();
    }
    MSG_DEBUG("nx_ip_address_change_notify() done\n");
  }

  /* Create the DHCP client. */
  ret = nx_dhcp_create(&DhcpClient, &IpInstance, "DHCP Client");
  if (ret != NX_SUCCESS)
  {
    return NX_NOT_ENABLED;
  }
  MSG_DEBUG("nx_dhcp_create() done\n");

  /* Allocate the memory for ARP */
  {
    const ULONG arp_cache_size = ARP_MEMORY_SIZE;
    VOID *arp_cache_memory;

    if (tx_byte_allocate(byte_pool, &arp_cache_memory, arp_cache_size, TX_NO_WAIT) != TX_SUCCESS)
    {
      return TX_POOL_ERROR;
    }

    /* Enable the ARP protocol and provide the ARP cache size for the IP instance */
    ret = nx_arp_enable(&IpInstance, arp_cache_memory, arp_cache_size);

    if (ret != NX_SUCCESS)
    {
      return NX_NOT_ENABLED;
    }
    MSG_DEBUG("nx_arp_enable() done\n");
  }

  /* Enable the ICMP */
  {
    ret = nx_icmp_enable(&IpInstance);

    if (ret != NX_SUCCESS)
    {
      return NX_NOT_ENABLED;
    }
    MSG_DEBUG("nx_icmp_enable() done\n");
  }

  /* Enable the UDP protocol required for DNS/DHCP communication. */
  {
    ret = nx_udp_enable(&IpInstance);

    if (ret != NX_SUCCESS)
    {
      return NX_NOT_ENABLED;
    }
    MSG_DEBUG("nx_udp_enable() done\n");
  }

  /* Enable the TCP protocol. */
  {
    ret = nx_tcp_enable(&IpInstance);

    if (ret != NX_SUCCESS)
    {
      return NX_NOT_ENABLED;
    }
    MSG_DEBUG("nx_tcp_enable() done\n");
  }

  /* Allocate the memory for main thread */
  {
    const ULONG stack_size = MAIN_THREAD_STACK_SIZE;
    VOID *stack_ptr;

    if (tx_byte_allocate(byte_pool, &stack_ptr, stack_size, TX_NO_WAIT) != TX_SUCCESS)
    {
      return TX_POOL_ERROR;
    }

    /* Create the main thread */
    ret = tx_thread_create(&AppMainThread, "App Main thread", App_Main_Thread_Entry,
                           (ULONG)byte_pool, stack_ptr, stack_size,
                           MAIN_THREAD_PRIORITY, MAIN_THREAD_PRIORITY, TX_NO_TIME_SLICE, TX_AUTO_START);

    if (ret != TX_SUCCESS)
    {
      return NX_NOT_ENABLED;
    }
    MSG_DEBUG("tx_thread_create() done\n");
  }

  /* Allocate the memory for main2 thread. */
  {
    const ULONG stack_size = MAIN2_THREAD_STACK_SIZE;
    VOID *stack_ptr;

    if (tx_byte_allocate(byte_pool, &stack_ptr, stack_size, TX_NO_WAIT) != TX_SUCCESS)
    {
      return TX_POOL_ERROR;
    }

    /* Create the main2 thread. */
    ret = tx_thread_create(&AppMain2Thread, "App Main2 thread", App_Main2_Thread_Entry,
                           (ULONG)byte_pool, stack_ptr, stack_size,
                           MAIN2_THREAD_PRIORITY, MAIN2_THREAD_PRIORITY, TX_NO_TIME_SLICE, TX_AUTO_START);

    if (ret != TX_SUCCESS)
    {
      return NX_NOT_ENABLED;
    }
    MSG_DEBUG("tx_thread_create() done\n");
  }
  /* set DHCP notification callback  */
  tx_semaphore_create(&DhcpSemaphore, "DHCP Semaphore", 0);
  /* USER CODE END MX_NetXDuo_Init */

  return ret;
}

/* USER CODE BEGIN 1 */
#if 0
static UINT DHCP_Server_start(void)
{
  UINT status;
  UINT addresses_added;
  UCHAR *pointer;

  /* Allocate the memory for HTTP Server thread   */
  if (tx_byte_allocate(byte_pool, (VOID **) &pointer, DHCP_SERVER_THREAD_MEMORY, TX_NO_WAIT) != TX_SUCCESS)
  {
    return TX_POOL_ERROR;
  }

  /* Create the DHCP Server.  */
  status = nx_dhcp_server_create(&DHCPserver, &IpInstance, pointer, DHCP_SERVER_THREAD_MEMORY,
                                 "DHCP Server", &DhcpServerPool);

  /* Check for errors creating the DHCP Server. */
  if (status)
  {
    printf("Error creating DHCP server\r\n");
    Error_Handler();
  }

  /* Load the assignable DHCP IP addresses for the first interface.  */
  status = nx_dhcp_create_server_ip_address_list(&DHCPserver, NX_DHCP_INTERFACE_INDEX, START_IP_ADDRESS_LIST_0,
                                                 END_IP_ADDRESS_LIST_0, &addresses_added);

  /* Check for errors creating the list. */
  if (status)
  {
    printf("Error creating DHCP IP address list\r\n");
    Error_Handler();
  }

  /* Verify all the addresses were added to the list. */
  if (addresses_added != 10)
  {
    printf("Error while adding IP address \r\n");
    Error_Handler();
  }

  /* Set the interface network parameters.  */
  status = nx_dhcp_set_interface_network_parameters(&DhcpServer, NX_DHCP_INTERFACE_INDEX, NX_DHCP_SUBNET_MASK_0,
                                                    NX_DHCP_DEFAULT_GATEWAY_0, NX_DHCP_DNS_SERVER_0);

  /* Check for errors setting network parameters. */
  if (status)
  {
    printf("errors setting network parameters \r\n");
    Error_Handler();
  }

  /* Start DHCP Server task.  */
  status = nx_dhcp_server_start(&DhcpServer);

  /* Check for errors starting up the DHCP server.  */
  if (status)
  {
    printf("errors starting up the DHCP server \r\n");
    Error_Handler();
  }

  printf("DHCP Server started successfully !!\r\n");

  /* the network is correctly initialized, start the TCP server thread */
  tx_thread_resume(&AppHTTPServerThread);

  /* this thread is not needed any more, we relinquish it */
  tx_thread_relinquish();

  return NX_SUCCESS;
}

/**
  * @brief  DHCP Server thread entry.
  * @param thread_input: none
  * @retval none
  */
/* Define the DHCP Server thread.  */

static void DHCP_Server_Thread_Entry(ULONG info)
{
  if (DHCP_Server_start() != 0)
  {
    Error_Handler();
  }
}
#endif

/**
  * @brief  ip address change callback.
  * @param ip_instance: NX_IP instance
  * @param ptr: user data
  * @retval none
  */
static VOID ip_address_change_notify_callback(NX_IP *ip_instance, VOID *ptr)
{
  NX_PARAMETER_NOT_USED(ip_instance);
  NX_PARAMETER_NOT_USED(ptr);

  MSG_DEBUG(">\"%s\"\n", ip_instance->nx_ip_name);

  /* release the semaphore as soon as an IP address is available */
  tx_semaphore_put(&DhcpSemaphore);
}

/**
  * @brief  Main thread entry.
  * @param thread_input: ULONG user argument used by the thread entry
  * @retval none
  */
static VOID App_Main_Thread_Entry(ULONG thread_input)
{
  UINT ret;
  TX_BYTE_POOL *const byte_pool = (TX_BYTE_POOL *) thread_input;

  MSG_DEBUG(">\n");

  /* Start the DHCP client. */
  ret = nx_dhcp_start(&DhcpClient);
  if (ret != NX_SUCCESS)
  {
    Error_Handler();
  }
  MSG_DEBUG("nx_dhcp_start() done\n");

  /* Wait until an IP address is ready. */
  if (tx_semaphore_get(&DhcpSemaphore, TX_WAIT_FOREVER) != TX_SUCCESS)
  {
    MSG_ERROR("ERROR: Cannot connect WiFi interface !\n");
    Error_Handler();
  }

  /* Kill our terminator. */
  AppMain2ThreadRunning = false;

  {
    ULONG IpAddress;
    ULONG NetMask;
    ret = nx_ip_address_get(&IpInstance, &IpAddress, &NetMask);

    if (ret != TX_SUCCESS)
    {
      Error_Handler();
    }

    MSG_INFO("\n- Network Interface connected: ");
    PRINT_IP_ADDRESS(IpAddress);
    MSG_INFO("\n");
  }

  {
    MX_WIFIObject_t *pMxWifiObj = wifi_obj_get();

    MSG_INFO(" - Device Name    : %s. \n", pMxWifiObj->SysInfo.Product_Name);
    MSG_INFO(" - Device ID      : %s. \n", pMxWifiObj->SysInfo.Product_ID);
    MSG_INFO(" - Device Version : %s. \n", pMxWifiObj->SysInfo.FW_Rev);
    MSG_INFO(" - MAC address    : %02X.%02X.%02X.%02X.%02X.%02X\n",
             pMxWifiObj->SysInfo.MAC[0], pMxWifiObj->SysInfo.MAC[1], pMxWifiObj->SysInfo.MAC[2],
             pMxWifiObj->SysInfo.MAC[3], pMxWifiObj->SysInfo.MAC[4], pMxWifiObj->SysInfo.MAC[5]);
  }

  /* The network is correctly initialized, start the TCP server thread */
  ret = http_server_socket_init(byte_pool);

  if (ret != TX_SUCCESS)
  {
    Error_Handler();
  }

  while (1)
  {
    /* Everything else is handled from TCP callbacks dispatched from
     * IP instance thread
     */
    tx_thread_suspend(tx_thread_identify());
  }
}

/**
  * @brief  Main2 thread entry.
  * @param thread_input: ULONG user argument used by the thread entry
  * @retval none
  */
static VOID App_Main2_Thread_Entry(ULONG thread_input)
{
  const uint32_t ticks_for_1s = TX_TIMER_TICKS_PER_SECOND;
  uint32_t time_out = 0;
  NX_PARAMETER_NOT_USED(thread_input);

  MSG_DEBUG(">\n");

  AppMain2ThreadRunning = true;

  while (AppMain2ThreadRunning)
  {
    tx_thread_sleep(ticks_for_1s);
    time_out++;

    if (time_out == 20)
    {
      MSG_DEBUG("Could not get an IP address !\n");
      MSG_INFO("ERROR: Cannot connect WiFi interface !\n");
      Error_Handler();
    }
  }
  MSG_DEBUG("<\n");
}
/* USER CODE END 1 */
