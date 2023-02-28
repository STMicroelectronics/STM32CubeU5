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
#include "app_netxduo.h"
#include "app_azure_rtos.h"
#include "httpserver_sentevents_socket.h"
#include "nx_ip.h"
#include "msg.h"
#include <stdbool.h>
#include <inttypes.h>
#include "io_pattern/mx_wifi_io.h"

#include "nxd_dhcp_server.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* Define the interface index.  */
#define NX_DHCP_INTERFACE_INDEX     0

#define NX_DHCP_SERVER_IP_ADDRESS   IP_ADDRESS(192, 168,   1, 1)
#define NX_DHCP_SERVER_IP_MASK      IP_ADDRESS(255, 255, 254, 0)

#define START_IP_ADDRESS_LIST_0     IP_ADDRESS(192, 168,   1, 10)
#define END_IP_ADDRESS_LIST_0       IP_ADDRESS(192, 168,   1, 19)
#define IP_ADRESS_LIST_0_COUNT      (END_IP_ADDRESS_LIST_0 - START_IP_ADDRESS_LIST_0 + 1)

#define NX_DHCP_SUBNET_MASK_0       IP_ADDRESS(255, 255, 254,  0)

#define NX_DHCP_DEFAULT_GATEWAY_0   IP_ADDRESS(192, 168,   1,  1)
#define NX_DHCP_DNS_SERVER_0        IP_ADDRESS(192, 168,   1,  1)
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
static TX_THREAD AppMainThread;
static CHAR AppMainThreadName[] = "App Main thread";

static NX_PACKET_POOL AppPacketPool;

NX_IP IpInstance;
static CHAR IpInstanceName[] = "NetX IP Instance 0";

static NX_DHCP_SERVER DhcpServer;
static CHAR DhcpServerName[] = "DHCP Server";
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
static UINT DHCP_Server_start(TX_BYTE_POOL *byte_pool);

static VOID App_Main_Thread_Entry(ULONG thread_input);

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
  /* USER CODE BEGIN 0 */

  /* USER CODE END 0 */

  /* USER CODE BEGIN MX_NetXDuo_Init */

  /* Change default WiFi mode to Access Point for this application. */
  WifiMode = MC_SOFTAP;

  MSG_INFO("Nx_HTTP_SERVER application started.\n");
  MSG_INFO("# build: %s-%s, %s, %s %s\n",
           "SPI",
           "RTOS",
#if (defined(MX_WIFI_NETWORK_BYPASS_MODE) && (MX_WIFI_NETWORK_BYPASS_MODE == 1))
           "Network on host",
#else
           "Network on module",
#endif /* MX_WIFI_NETWORK_BYPASS_MODE */
           __TIME__, __DATE__);

  MSG_INFO("\nTX_TIMER_TICKS_PER_SECOND: %" PRIu32 "\n", (uint32_t)TX_TIMER_TICKS_PER_SECOND);
  MSG_INFO("NX_IP_PERIODIC_RATE      : %" PRIu32 "\n\n", (uint32_t)NX_IP_PERIODIC_RATE);

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

    MSG_DEBUG("[%06" PRIu32 "] Calling nx_ip_create()\n", HAL_GetTick());

    /* Create the main NX_IP instance. */
    ULONG ip_address = NX_DHCP_SERVER_IP_ADDRESS;
    ULONG network_mask = NX_DHCP_SERVER_IP_MASK;

    ret = nx_ip_create(&IpInstance, IpInstanceName,
                       ip_address,
                       network_mask,
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

  /* Allocate the memory for the main thread. */
  {
    const ULONG stack_size = MAIN_THREAD_STACK_SIZE;
    VOID *stack_ptr;

    if (tx_byte_allocate(byte_pool, &stack_ptr, stack_size, TX_NO_WAIT) != TX_SUCCESS)
    {
      return TX_POOL_ERROR;
    }

    /* Create the main thread */
    ret = tx_thread_create(&AppMainThread, AppMainThreadName, App_Main_Thread_Entry,
                           (ULONG)byte_pool, stack_ptr, stack_size,
                           MAIN_THREAD_PRIORITY, MAIN_THREAD_PRIORITY, TX_NO_TIME_SLICE, TX_AUTO_START);

    if (ret != TX_SUCCESS)
    {
      return NX_NOT_ENABLED;
    }
    MSG_DEBUG("tx_thread_create() done\n");
  }
  /* USER CODE END MX_NetXDuo_Init */

  return ret;
}

/* USER CODE BEGIN 1 */
static UINT DHCP_Server_start(TX_BYTE_POOL *byte_pool)
{
  UINT status;
  UINT addresses_added = {0};
  const ULONG stack_size = NETX_DHCP_SERVER_THREAD_STACK_SIZE;
  VOID *stack_ptr;


  /* Allocate the memory for DHCP Server thread. */
  if (tx_byte_allocate(byte_pool, &stack_ptr, stack_size, TX_NO_WAIT) != TX_SUCCESS)
  {
    return TX_POOL_ERROR;
  }

  /* Create the DHCP Server. */
  status = nx_dhcp_server_create(&DhcpServer, &IpInstance, stack_ptr, stack_size,
                                 DhcpServerName, &AppPacketPool);

  /* Check for errors creating the DHCP Server. */
  if (status)
  {
    MSG_ERROR("Creating DHCP server failed!\n");
    Error_Handler();
  }

  /* Load the assignable DHCP IP addresses for the first interface.  */
  status = nx_dhcp_create_server_ip_address_list(&DhcpServer, NX_DHCP_INTERFACE_INDEX,
                                                 START_IP_ADDRESS_LIST_0,
                                                 END_IP_ADDRESS_LIST_0, &addresses_added);

  /* Check for errors creating the list. */
  if (status)
  {
    MSG_ERROR("Creating DHCP IP address list failed!\n");
    Error_Handler();
  }

  /* Verify all the addresses were added to the list. */
  if (addresses_added != IP_ADRESS_LIST_0_COUNT)
  {
    MSG_ERROR("Adding IP address failed!\n");
    Error_Handler();
  }

  /* Set the interface network parameters.  */
  status = nx_dhcp_set_interface_network_parameters(&DhcpServer, NX_DHCP_INTERFACE_INDEX,
                                                    NX_DHCP_SUBNET_MASK_0,
                                                    NX_DHCP_DEFAULT_GATEWAY_0,
                                                    NX_DHCP_DNS_SERVER_0);

  /* Check for errors setting network parameters. */
  if (status)
  {
    MSG_ERROR("Setting network parameters failed!\n");
    Error_Handler();
  }

  /* Start DHCP Server task.  */
  status = nx_dhcp_server_start(&DhcpServer);

  /* Check for errors starting up the DHCP server.  */
  if (status)
  {
    MSG_ERROR("Starting up the DHCP server failed!\n");
    Error_Handler();
  }

  MSG_DEBUG("DHCP Server started successfully !!\n");

  return NX_SUCCESS;
}


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

  /* Start the DHCP server. */
  ret = DHCP_Server_start(byte_pool);
  if (ret != NX_SUCCESS)
  {
    Error_Handler();
  }
  MSG_DEBUG("DHCP_Server_start() done\n");

#ifdef ENABLE_IOT_INFO
  {
    const UINT IpIndex = 0;
    CHAR *interface_name = NULL;
    ULONG ip_address = 0;
    ULONG net_mask = 0;
    ULONG mtu_size = 0;
    ULONG physical_addres_msw = 0;
    ULONG physical_address_lsw = 0;

    _nx_ip_interface_info_get(&IpInstance, IpIndex, &interface_name,
                              &ip_address, &net_mask,
                              &mtu_size,
                              &physical_addres_msw, &physical_address_lsw);

    MSG_INFO("\nIP: \"%s\", MTU: %" PRIu32 "\n", interface_name, (uint32_t)mtu_size);
    MSG_DEBUG("0x%" PRIX32 "%" PRIX32 "\n", physical_addres_msw, physical_address_lsw);
  }
#endif /* ENABLE_IOT_INFO */

  /* Read back IP address and gateway address. */
  {
    ULONG ip_address = 0;
    ULONG net_mask = 0;

    ret = nx_ip_address_get(&IpInstance, &ip_address, &net_mask);
    if (ret != TX_SUCCESS)
    {
      Error_Handler();
    }

    MSG_INFO("\n- Network Interface connected: ");
    PRINT_IP_ADDRESS(ip_address);
    MSG_INFO("\n");

    MSG_INFO("Mask: ");
    PRINT_IP_ADDRESS(net_mask);
    MSG_INFO("\n");
  }

  {
    MSG_INFO(" - Device Name    : %s.\n", wifi_obj_get()->SysInfo.Product_Name);
    MSG_INFO(" - Device ID      : %s.\n", wifi_obj_get()->SysInfo.Product_ID);
    MSG_INFO(" - Device Version : %s.\n", wifi_obj_get()->SysInfo.FW_Rev);
    MSG_INFO(" - MAC address    : %02X.%02X.%02X.%02X.%02X.%02X\n",
             wifi_obj_get()->SysInfo.MAC[0], wifi_obj_get()->SysInfo.MAC[1],
             wifi_obj_get()->SysInfo.MAC[2], wifi_obj_get()->SysInfo.MAC[3],
             wifi_obj_get()->SysInfo.MAC[4], wifi_obj_get()->SysInfo.MAC[5]);
    MSG_INFO(" - MAC address AP : %02X.%02X.%02X.%02X.%02X.%02X\n",
             wifi_obj_get()->SysInfo.apMAC[0], wifi_obj_get()->SysInfo.apMAC[1],
             wifi_obj_get()->SysInfo.apMAC[2], wifi_obj_get()->SysInfo.apMAC[3],
             wifi_obj_get()->SysInfo.apMAC[4], wifi_obj_get()->SysInfo.apMAC[5]);
  }

  {
    uint8_t ip_addr[4] = {0};
    MX_WIFI_GetIPAddress(wifi_obj_get(), ip_addr, MC_STATION);
    MSG_INFO("IP (STA)   : %02X.%02X.%02X.%02X\n", ip_addr[0], ip_addr[1], ip_addr[2], ip_addr[3]);
  }

  {
    uint8_t ip_addr[4] = {0};
    MX_WIFI_GetIPAddress(wifi_obj_get(), ip_addr, MC_SOFTAP);
    MSG_INFO("IP (SOFTAP): %02X.%02X.%02X.%02X\n", ip_addr[0], ip_addr[1], ip_addr[2], ip_addr[3]);
  }

  {
    uint8_t fw_rev[] = {'0', '1', '2', '.', '.', '.'}; /* Set a size less than the actual version string length. */
    MX_WIFI_GetVersion(wifi_obj_get(), fw_rev, sizeof(fw_rev));
    MSG_INFO("Version : %s.\n", fw_rev);
  }
  MSG_INFO("\n\nAP mode set, please join \"%s\"\n\n", WIFI_SSID);


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
/* USER CODE END 1 */
