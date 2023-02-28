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
#include "iap_client.h"
#include "nxd_http_client.h"
#include "mx_wifi.h"
#include "nx_ip.h"
#include "msg.h"
#include "nxd_dhcp_client.h"
#include <stdbool.h>
#include <inttypes.h>
#include "io_pattern/mx_wifi_io.h"
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
/* USER CODE BEGIN PV */
static TX_THREAD AppMainThread;
static CHAR AppMainThreadName[] = "App Main thread";

static TX_THREAD AppMain2Thread;
static CHAR AppMain2ThreadName[] = "App Main2 thread";

static __IO bool AppMain2ThreadRunning = true;

static TX_THREAD AppIapThread;
static CHAR AppIapThreadName[] = "App IAP thread";

TX_BYTE_POOL *AppBytePool;

static NX_PACKET_POOL AppPacketPool;

static NX_IP IpInstance;
static CHAR IpInstanceName[] = "NetX IP Instance 0";

static NX_DHCP DhcpClient;
static TX_SEMAPHORE DhcpSemaphore;

static TX_EVENT_FLAGS_GROUP IapFlags;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
static VOID App_Main_Thread_Entry(ULONG thread_input);
static VOID App_Main2_Thread_Entry(ULONG thread_input);

static VOID App_IAP_Thread_Entry(ULONG thread_input);
static VOID ip_address_change_notify_callback(NX_IP *ip_instance, VOID *ptr);

void NetXDuo_DeInit(void);
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
  AppBytePool = byte_pool;
  /* USER CODE END App_NetXDuo_MEM_POOL */
  /* USER CODE BEGIN 0 */

  /* USER CODE END 0 */

  /* USER CODE BEGIN MX_NetXDuo_Init */
  MSG_INFO("\nNx_IAP_Client application started.\n");
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
    ret = nx_ip_create(&IpInstance, IpInstanceName, 0, 0,
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

  /* Allocate the memory for main2 thread. */
  {
    const ULONG stack_size = MAIN2_THREAD_STACK_SIZE;
    VOID *stack_ptr;

    if (tx_byte_allocate(byte_pool, &stack_ptr, stack_size, TX_NO_WAIT) != TX_SUCCESS)
    {
      return TX_POOL_ERROR;
    }

    /* Create the main2 thread. */
    ret = tx_thread_create(&AppMain2Thread, AppMain2ThreadName, App_Main2_Thread_Entry,
                           (ULONG)byte_pool, stack_ptr, stack_size,
                           MAIN2_THREAD_PRIORITY, MAIN2_THREAD_PRIORITY, TX_NO_TIME_SLICE, TX_AUTO_START);

    if (ret != TX_SUCCESS)
    {
      return NX_NOT_ENABLED;
    }
    MSG_DEBUG("tx_thread_create() done\n");
  }

  /* Allocate the memory for the IAP client thread. */
  {
    const ULONG stack_size = IAP_THREAD_STACK_SIZE;
    VOID *stack_ptr;

    if (tx_byte_allocate(byte_pool, &stack_ptr, stack_size, TX_NO_WAIT) != TX_SUCCESS)
    {
      return TX_POOL_ERROR;
    }

    /* Create the IAP client thread. */
    ret = tx_thread_create(&AppIapThread, AppIapThreadName, App_IAP_Thread_Entry,
                           (ULONG)byte_pool, stack_ptr, stack_size,
                           IAP_THREAD_PRIORITY, IAP_THREAD_PRIORITY, TX_NO_TIME_SLICE, TX_DONT_START);

    if (ret != TX_SUCCESS)
    {
      return NX_NOT_ENABLED;
    }
    MSG_DEBUG("tx_thread_create() done\n");
  }

  /* Create the event flags. */
  ret = tx_event_flags_create(&IapFlags, "IAP event flags");

  /* Check for errors */
  if (ret != NX_SUCCESS)
  {
    return NX_NOT_ENABLED;
  }

  /* Set DHCP notification callback */
  tx_semaphore_create(&DhcpSemaphore, "DHCP Semaphore", 0);
  /* USER CODE END MX_NetXDuo_Init */

  return ret;
}

/* USER CODE BEGIN 1 */
/**
  * @brief ip address change callback.
  * @param ip_instance: NX_IP instance
  * @param ptr: user data
  * @retval none
  */
static VOID ip_address_change_notify_callback(NX_IP *ip_instance, VOID *ptr)
{
  NX_PARAMETER_NOT_USED(ip_instance);
  NX_PARAMETER_NOT_USED(ptr);

  MSG_DEBUG(">\"%s\"\n", ip_instance->nx_ip_name);

  /* Release the semaphore as soon as an IP address is available. */
  tx_semaphore_put(&DhcpSemaphore);
}


/**
  * @brief  Main thread entry.
  * @param thread_input: ULONG user argument used by the thread entry
  * @retval none
  */
static VOID App_Main_Thread_Entry(ULONG thread_input)
{
  UINT ret = NX_SUCCESS;

  MSG_DEBUG(">\n");

 ret = nx_ip_address_change_notify(&IpInstance, ip_address_change_notify_callback, NULL);
  if (ret != NX_SUCCESS)
  {
    Error_Handler();
  }
  MSG_DEBUG("nx_ip_address_change_notify() done\n");

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
    MSG_ERROR("ERROR: Cannot connect the WiFi interface!\n");
    Error_Handler();
  }

  /* Kill our terminator. */
  AppMain2ThreadRunning = false;


  {
    const ULONG status_check_mask = \
    NX_IP_INITIALIZE_DONE | NX_IP_ADDRESS_RESOLVED | NX_IP_LINK_ENABLED | \
    NX_IP_ARP_ENABLED | NX_IP_UDP_ENABLED | NX_IP_TCP_ENABLED | \
    /* NX_IP_IGMP_ENABLED | */ NX_IP_RARP_COMPLETE | NX_IP_INTERFACE_LINK_ENABLED;
    ULONG interface_status = 0;

    ret = nx_ip_interface_status_check(&IpInstance, 0U, status_check_mask, &interface_status, NX_WAIT_FOREVER);
    MSG_INFO("Interface status: %04" PRIX32 "\n", (uint32_t)interface_status);
  }

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
    ULONG gateway_address = 0;

    ret = nx_ip_address_get(&IpInstance, &ip_address, &net_mask);
    if (ret != TX_SUCCESS)
    {
      Error_Handler();
    }

    ret = nx_ip_gateway_address_get(&IpInstance, &gateway_address);
    if (ret != TX_SUCCESS)
    {
      Error_Handler();
    }

    MSG_INFO("\n- Network Interface connected: ");
    PRINT_IP_ADDRESS(ip_address);
    MSG_INFO("\n");

    MSG_INFO("Mask: ");
    PRINT_IP_ADDRESS(net_mask);

    MSG_INFO("Gateway: ");
    PRINT_IP_ADDRESS(gateway_address);
  }

  /* The network is correctly initialized, start the IAP server thread. */
  tx_thread_resume(&AppIapThread);

  /* this thread is not needed any more, we relinquish it */
  tx_thread_relinquish();

  return;
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
      MSG_DEBUG("Could not get an IP address!\n");
      MSG_INFO("ERROR: Cannot connect WiFi interface!\n");
      Error_Handler();
    }
  }
  MSG_DEBUG("<\n");
}


/**
  * @brief IAP thread entry.
  * @param thread_input: ULONG user argument used by the thread entry
  * @retval none
  */
/* Define the client thread.  */
static void App_IAP_Thread_Entry(ULONG thread_input)
{
  TX_BYTE_POOL *const byte_pool = (TX_BYTE_POOL *) thread_input;

  MSG_DEBUG("[%06" PRIu32 "]>\n", HAL_GetTick());

  {
    MSG_INFO(" - Device Name    : %s.\n", wifi_obj_get()->SysInfo.Product_Name);
    MSG_INFO(" - Device ID      : %s.\n", wifi_obj_get()->SysInfo.Product_ID);
    MSG_INFO(" - Device Version : %s.\n", wifi_obj_get()->SysInfo.FW_Rev);
    MSG_INFO(" - MAC address    : %02X.%02X.%02X.%02X.%02X.%02X\n\n",
             wifi_obj_get()->SysInfo.MAC[0], wifi_obj_get()->SysInfo.MAC[1],
             wifi_obj_get()->SysInfo.MAC[2], wifi_obj_get()->SysInfo.MAC[3],
             wifi_obj_get()->SysInfo.MAC[4], wifi_obj_get()->SysInfo.MAC[5]);
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

  StartWiFiIAP(byte_pool, &IpInstance);

  MSG_DEBUG("<\n");
}


void NetXDuo_DeInit(void)
{
  MSG_DEBUG(">\n");
  {
    const UINT status = nx_dhcp_delete(&DhcpClient);
    MSG_INFO("nx_dhcp_delete() done with 0x%" PRIx32 "\n", (uint32_t)status);
  }

  {
    const UINT status = nx_ip_delete(&IpInstance);
    MSG_INFO("nx_ip_delete() done with 0x%" PRIx32 "\n", (uint32_t)status);
  }

  {
    CHAR *name = (CHAR *)TX_NULL;
    ULONG available_bytes = 0;
    ULONG fragments = 0;
    TX_THREAD *first_suspended;
    ULONG suspended_count;
    TX_BYTE_POOL *next_pool;

    tx_byte_pool_info_get(AppBytePool, &name,
                          &available_bytes, &fragments,
                          &first_suspended, &suspended_count, &next_pool);
    MSG_INFO("Pool \"%s\" (%" PRIu32 ")\n", (name != TX_NULL) ? name : "", (uint32_t)AppBytePool->tx_byte_pool_size);
    MSG_INFO(" - available bytes: %" PRIu32 "\n", (uint32_t)available_bytes);
    MSG_INFO(" - fragments      : %" PRIu32 "\n", (uint32_t)fragments);
    MSG_INFO(" - suspended count: %" PRIu32 "\n", (uint32_t)suspended_count);
  }

  {
    ULONG total_packets = 0;
    ULONG free_packets = 0;
    ULONG empty_pool_requests = 0;
    ULONG empty_pool_suspensions = 0;
    ULONG invalid_packet_releases = 0;

    nx_packet_pool_info_get(&AppPacketPool,
                            &total_packets, &free_packets,
                            &empty_pool_requests, &empty_pool_suspensions,
                            &invalid_packet_releases);
    MSG_INFO("Packet Pool \"%s\" (%" PRIu32 ")\n",
             AppPacketPool.nx_packet_pool_name,
             (uint32_t)AppPacketPool.nx_packet_pool_payload_size);
    MSG_INFO(" - total packets          : %" PRIu32 "\n", (uint32_t)total_packets);
    MSG_INFO(" - free packets           : %" PRIu32 "\n", (uint32_t)free_packets);
    MSG_INFO(" - invalid packet releases: %" PRIu32 "\n", (uint32_t)invalid_packet_releases);
  }

  MSG_DEBUG("<\n");
}
/* USER CODE END 1 */
