/**
  ******************************************************************************
  * @file    net_mx_wifi.c
  * @author  MCD Application Team
  * @brief   MXCHIP Wi-Fi specific BSD-like socket wrapper
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
#include "net_conf.h"
#include "net_connect.h"
#include "net_internals.h"
#include "net_errors.h"
#include "net_mem.h"
#include "net_perf.h"

/* Mxchip WiFi API header file and configuration. */
#include "mx_wifi.h"
#include "mx_wifi_conf.h"
#include "core/mx_wifi_hci.h"
#include "io_pattern/mx_wifi_io.h"

#include <inttypes.h>

#if (defined(MX_WIFI_NETWORK_BYPASS_MODE) && (MX_WIFI_NETWORK_BYPASS_MODE == 1))
#include "net_ip_lwip.h"

#if LWIP_IPV6
#include "lwip/ethip6.h"
#endif /* LWIP_IPV6 */

#include "lwip/etharp.h"
#include "lwip/tcpip.h"
#endif /* (MX_WIFI_NETWORK_BYPASS_MODE == 1) */



#if (defined(MX_WIFI_NETWORK_BYPASS_MODE) && (MX_WIFI_NETWORK_BYPASS_MODE == 1))
/* Max WiFI frame length. */
#define MX_WIFI_PAYLOAD_MTU   (MX_WIFI_MTU_SIZE + MX_WIFI_BYPASS_HEADER_SIZE)
#endif /* (MX_WIFI_NETWORK_BYPASS_MODE == 1) */

/* Debug log function */
#ifndef MX_WIFI_BYPASS_DEBUG
#define MX_WIFI_BYPASS_DEBUG  (0)
#endif /* MX_WIFI_BYPASS_DEBUG */


#ifdef MX_WIFI_API_DEBUG
#define DEBUG_LOG(M, ...)  printf((M), ##__VA_ARGS__) /*;*/
#else
#define DEBUG_LOG(M, ...)  /*;*/
#endif /* MX_WIFI_API_DEBUG */


/* IPv4 address transfer */
#define NET_ARTON(A)     ((uint32_t)(((uint32_t)(A)[3] << 24U) |\
                                     ((uint32_t)(A)[2] << 16U) |\
                                     ((uint32_t)(A)[1] <<  8U) |\
                                     ((uint32_t)(A)[0] <<  0U)))

/* Declaration of generic class functions               */

/* mxchip WiFi adapter functions for the STM32 Network library. */
int32_t mx_wifi_driver(net_if_handle_t *pnetif);

static int32_t mx_wifi_if_init(net_if_handle_t *pnetif);
static int32_t mx_wifi_if_deinit(net_if_handle_t *pnetif);

static int32_t mx_wifi_if_start(net_if_handle_t *pnetif);
static int32_t mx_wifi_if_stop(net_if_handle_t *pnetif);
static int32_t mx_wifi_if_yield(net_if_handle_t *pnetif, uint32_t timeout_ms);
static void mx_wifi_status_changed(uint8_t cate, uint8_t status, void *arg);

static int32_t mx_wifi_if_connect(net_if_handle_t *pnetif);
static int32_t mx_wifi_if_disconnect(net_if_handle_t *pnetif);


/* Bypass mode functions. */
#if (defined(MX_WIFI_NETWORK_BYPASS_MODE) && (MX_WIFI_NETWORK_BYPASS_MODE == 1))
static struct netif *get_netif(uint8_t low_level_netif_idx);
static void PushToDriver(uint32_t timeout);
static err_t low_level_output(struct netif *netif, struct pbuf *p);
static err_t low_level_init(struct netif *netif);
static FIFO_DECLARE(Fifo);
static void NET_MX_WIFI_TranmitThread(THREAD_CONTEXT_TYPE context);

#else
static int32_t mx_wifi_socket(int32_t domain, int32_t type, int32_t protocol);
static int32_t mx_wifi_bind(int32_t sock, const net_sockaddr_t *addr, uint32_t addrlen);
static int32_t mx_wifi_listen(int32_t sock, int32_t backlog);
static int32_t mx_wifi_accept(int32_t sock, net_sockaddr_t *addr, uint32_t *addrlen);
static int32_t mx_wifi_connect(int32_t sock, const net_sockaddr_t *addr, uint32_t addrlen);
static int32_t mx_wifi_send(int32_t sock, const uint8_t *buf, int32_t len, int32_t flags);
static int32_t mx_wifi_recv(int32_t sock, uint8_t *buf, int32_t len, int32_t flags);
static int32_t mx_wifi_sendto(int32_t sock, const uint8_t *buf, int32_t len, int32_t flags, net_sockaddr_t *to,
                              uint32_t to_len);
static int32_t mx_wifi_recvfrom(int32_t sock, uint8_t *buf, int32_t len, int32_t flags, net_sockaddr_t *from,
                                uint32_t *from_len);
static int32_t mx_wifi_setsockopt(int32_t sock, int32_t level, int32_t optname, const void *optvalue, uint32_t optlen);
static int32_t mx_wifi_getsockopt(int32_t sock, int32_t level, int32_t optname, void *optvalue, uint32_t *optlen);
static int32_t mx_wifi_getsockname(int32_t sock, net_sockaddr_t *name, uint32_t *namelen);
static int32_t mx_wifi_getpeername(int32_t sock, net_sockaddr_t *name, uint32_t *namelen);
static int32_t mx_wifi_close(int32_t sock, bool isaclone);
static int32_t mx_wifi_shutdown(int32_t sock, int32_t mode);
static int32_t mx_wifi_gethostbyname(net_if_handle_t *pnetif, net_sockaddr_t *addr, char_t *name);

static int32_t mx_wifi_ping(net_if_handle_t *pnetif, net_sockaddr_t *addr, int32_t count, int32_t delay,
                            int32_t response[]);
#endif /* MX_WIFI_NETWORK_BYPASS_MODE */

/* Declaration and definition of class-specific functions. */
static int32_t mx_wifi_scan(net_if_handle_t *pnetif, net_wifi_scan_mode_t mode, char *ssid);
static int32_t mx_wifi_get_scan_result(net_if_handle_t *pnetif, net_wifi_scan_results_t *scan_bss_array,
                                       uint8_t scan_bss_count);
static int32_t mx_wifi_if_start_station(net_if_handle_t *pnetif);
static int32_t mx_wifi_if_start_softap(net_if_handle_t *pnetif);
static int32_t hw_start(net_if_handle_t *pnetif);



/* Internal structure to manage the WiFi socket. */
typedef struct mxwifi_tls_data_s
{
  char *tls_ca_certs;          /**< Socket option. */
  char *tls_ca_crl;            /**< Socket option. */
  char *tls_dev_cert;          /**< Socket option. */
  char *tls_dev_key;           /**< Socket option. */
  uint8_t *tls_dev_pwd;        /**< Socket option. */
  bool tls_srv_verification;   /**< Socket option. */
  char *tls_srv_name;          /**< Socket option. */
} mxwifi_tls_data_t;

/* bypass mode functions */
#if (defined(MX_WIFI_NETWORK_BYPASS_MODE) && (MX_WIFI_NETWORK_BYPASS_MODE == 1))
static net_if_handle_t *Station_pNetif = NULL;
static net_if_handle_t *SoftAP_pNetif = NULL;

static struct netif *get_netif(uint8_t low_level_netif_idx)
{
  struct netif *netif;

  if (low_level_netif_idx == (uint8_t)SOFTAP_IDX)
  {
    netif = SoftAP_pNetif->netif;
  }
  else if (low_level_netif_idx == (uint8_t)STATION_IDX)
  {
    netif = Station_pNetif->netif;
  }
  else
  {
    netif = NULL;
  }

  return netif;
}


/**
  * @brief                   Low level initialization function for the WiFi driver
  * @param  netif            low level net interface
  * @return err_t            return ERR_OK if success, otherwise error code
  */
static err_t low_level_init(struct netif *netif)
{
  err_t ret = -1;
#ifndef MX_WIFI_BARE_OS_H
  static THREAD_DECLARE(NET_MX_WIFI_TransmitThreadId);
#endif /* MX_WIFI_BARE_OS_H */

  net_if_handle_t *const pnetif = (net_if_handle_t *)(netif->state);

  FIFO_INIT(Fifo, MX_WIFI_MAX_TX_BUFFER_COUNT);

  if (THREAD_OK == THREAD_INIT(NET_MX_WIFI_TransmitThreadId, NET_MX_WIFI_TranmitThread, NULL,
                               MX_WIFI_TRANSMIT_THREAD_STACK_SIZE,
                               MX_WIFI_TRANSMIT_THREAD_PRIORITY))
  {
    /* Set the interface name for the mxchip WiFi. */

    /**
      * We directly use etharp_output() here to save a function call.
      * You can instead declare your own function an call etharp_output()
      * from it if you have to do some checks before sending (e.g. if link
      * is available...)
      */
    netif->output = etharp_output;

#if LWIP_IPV6
    netif->output_ip6 = ethip6_output;
    netif->ip6_autoconfig_enabled = 1;
#endif /* LWIP_IPV6 */

    netif->linkoutput = low_level_output;

    /* Set MAC hardware address length (6). */
    netif->hwaddr_len = (u8_t) ETHARP_HWADDR_LEN;

    /* Setup the physical address of this IP instance. */
    if (pnetif->pdrv->extension.wifi->mode == NET_WIFI_MODE_AP)
    {
      netif->name[0] = 'M';
      netif->name[1] = 'A';
      (void)memcpy(netif->hwaddr, wifi_obj_get()->SysInfo.apMAC, 6);
    }
    else
    {
      netif->name[0] = 'M';
      netif->name[1] = 'S';
      (void)memcpy(netif->hwaddr, wifi_obj_get()->SysInfo.MAC, 6);
    }

#if MX_WIFI_BYPASS_DEBUG
    NET_PRINT(" MAC address %x.%x.%x.%x.%x.%x\n",
              netif->hwaddr[0], netif->hwaddr[1], netif->hwaddr[2],
              netif->hwaddr[3], netif->hwaddr[4], netif->hwaddr[5]);
#endif /* MX_WIFI_BYPASS_DEBUG */

    /* Set the Maximum Transfer Unit. */
    netif->mtu = (u16_t) MX_WIFI_PAYLOAD_MTU;

    /* Set device capabilities. Don't set NETIF_FLAG_ETHARP if this device is not an Ethernet one */
    netif->flags = (u8_t)(NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP | NETIF_FLAG_ETHERNET);

    /* Do whatever else is needed to initialize interface. */
#if LWIP_IGMP
    netif->flags |= NETIF_FLAG_IGMP;
    netif_set_igmp_mac_filter(netif, lwip_igmp_mac_filter);
#endif /* LWIP_IGMP */

    /* Register a handler for any address changes and when the interface goes up or down. */
    netif_set_status_callback(netif, net_ip_status_cb);
    netif_set_link_callback(netif, net_ip_status_cb);

    ret = (err_t)ERR_OK;
  }
  return ret;
}


/**
  * This function should do the actual transmission of the packet. The packet is
  * contained in the pbuf that is passed to the function. This pbuf
  * might be chained.
  *
  * @param netif the lwip network interface structure for this ethernetif
  * @param p the MAC packet to send (e.g. IP packet including MAC addresses and type)
  * @return ERR_OK if the packet could be sent
  *         an err_t value if the packet couldn't be sent
  *
  * @note Returning ERR_MEM here if a DMA queue of your MAC is full can lead to
  *       strange results. You might consider waiting for space in the DMA queue
  *       to become available since the stack doesn't retry to send a packet
  *       dropped because of memory failure (except for the TCP timers).
  */
static err_t low_level_output(struct netif *netif, struct pbuf *p)
{
  err_t err = (err_t) ERR_ARG;
  struct pbuf *pbuf_send;

  net_if_handle_t *const pnetif = (net_if_handle_t *)(netif->state);

  if (NULL != p)
  {
    /* Take a reference to this packet. */
    pbuf_ref(p);

#if MX_WIFI_BYPASS_DEBUG
    NET_PRINT("Transmit buffer %p next=%p  tot_len=%" PRIu32 " len=%" PRIu32 "\n",
              p, p->next, (uint32_t)p->tot_len, (uint32_t)p->len);
#endif /* MX_WIFI_BYPASS_DEBUG */

    if (pnetif->pdrv->extension.wifi->mode == NET_WIFI_MODE_AP)
    {
      p->if_idx = (uint8_t)SOFTAP_IDX;
    }
    else
    {
      p->if_idx = (uint8_t)STATION_IDX;
    }
    /* No chained buffers. */
    if ((p->next != NULL) || ((p->tot_len != p->len)))
    {
      /* chained buffer to output */
      pbuf_send = pbuf_clone(PBUF_RAW, PBUF_RAM, p);
      (void)pbuf_free(p);
      if (NULL == pbuf_send)
      {
        NET_PRINT("no memory for chained buffer!!!");
        err = (err_t)ERR_MEM;
      }
      else
      {
        void *handle = pbuf_send;
        NET_PERF_TASK_TAG(3);

        if (FIFO_OK == FIFO_PUSH(Fifo, handle, WAIT_FOREVER, PushToDriver))
        {
          err = (err_t)ERR_OK;
        }
      }
    }
    else
    {
      void *handle = p;
      NET_PERF_TASK_TAG(3);
      if (FIFO_OK == FIFO_PUSH(Fifo, handle, WAIT_FOREVER, PushToDriver))
      {
        err = (err_t)ERR_OK;
      }
    }
  }
  else
  {
    /* Cannot transmit wifi not ready */
    err = (err_t) ERR_INPROGRESS; /* Note that signaling ERR_CLSD or ERR_CONN causes loss of connectivity on a roam */
  }

  return err;
}


#if MX_WIFI_TX_BUFFER_NO_COPY
#if PBUF_LINK_ENCAPSULATION_HLEN < MX_WIFI_MIN_TX_HEADER_SIZE
#error "When MX_WIFI_TX_BUFFER_NO_COPY is set, \
lwipopts.h must reserved buffer headroom thanks to PBUF_LINK_ENCAPSULATION_HLEN"
#endif /* PBUF_LINK_ENCAPSULATION_HLEN < MX_WIFI_MIN_TX_HEADER_SIZE  */

#endif  /* MX_WIFI_TX_BUFFER_NO_COPY */


/**
  * @brief         Push WiFi packet to the WiFi driver to send
  * @param timeout timeout in milliseconds
  */
static void PushToDriver(uint32_t timeout)
{
  void *const handle = FIFO_POP(Fifo, timeout, NULL);
  if (handle != NULL)
  {
    struct pbuf *p = (struct pbuf *) handle;

    LINK_STATS_INC(link.xmit);
    NET_PERF_TASK_TAG(4);

    (void)MX_WIFI_Network_bypass_netlink_output(wifi_obj_get(),
                                                p->payload, (int32_t)p->len,
                                                (int32_t)p->if_idx);
    (void)pbuf_free(p);
  }
}


/**
  * @brief         Thread to push WiFi packet to the WiFi driver
  * @param context Thread context argument
  */
static void NET_MX_WIFI_TranmitThread(THREAD_CONTEXT_TYPE context)
{
  (void)context;
  while (true)
  {
    PushToDriver(WAIT_FOREVER);
  }
}


/**
  * @brief                   Low level data callback for the WiFi driver
  * @param  buffer           low level data buffer
  * @param  user_args        user arguments
  */
static void net_mx_wifi_netlink_input_callback(mx_buf_t *buffer, void *user_args)
{
  uint8_t low_level_netif_idx = (uint8_t) * ((uint32_t *) user_args);

  if ((buffer != NULL) && (buffer->len > 0U) && (buffer->len <= (uint32_t) MX_WIFI_PAYLOAD_MTU))
  {
    struct netif *netif = get_netif(low_level_netif_idx);

    /* Points to packet payload, which starts with an Ethernet header. */
    struct eth_hdr *ethernet_header = (struct eth_hdr *) buffer->payload;

    const uint16_t ethertype = lwip_htons(ethernet_header->type);

    switch (ethertype)
    {
      case ETHTYPE_IPV6:
      case ETHTYPE_IP:
      case ETHTYPE_ARP:

#if PPPOE_SUPPORT
      /* PPPoE packet? */
      case ETHTYPE_PPPOEDISC:
      case ETHTYPE_PPPOE:
#endif /* PPPOE_SUPPORT */

#if MX_WIFI_BYPASS_DEBUG
        NET_PRINT("process input packet 0x%02x, len=%d\n", ethertype, buffer->tot_len);
#endif /* MX_WIFI_BYPASS_DEBUG */

        /* Send to packet to tcpip_thread to process */
        if (NULL == netif)
        {
          (void)pbuf_free(buffer);
        }
        else
        {
          if (tcpip_input(buffer, netif) != (err_t)ERR_OK)
          {
            (void)pbuf_free(buffer);
          }
        }
        break;

      default:
        (void)pbuf_free(buffer);
        break;
    }
  }
  else
  {
    if (NULL != buffer)
    {
      (void)pbuf_free(buffer);
    }
  }
}
#endif /* (MX_WIFI_NETWORK_BYPASS_MODE == 1) */


/**
  * @brief                   WiFi driver initialization
  * @param  pnetif           net interface pointer
  * @return int32_t          0 is success, otherwise failed
  */
int32_t mx_wifi_driver(net_if_handle_t *pnetif)
{
  /* Initialize the LwIP library here if not already done by another network interface. */
#if (defined(MX_WIFI_NETWORK_BYPASS_MODE) && (MX_WIFI_NETWORK_BYPASS_MODE == 1))
  net_ip_init();
#endif /* (MX_WIFI_NETWORK_BYPASS_MODE == 1) */

  return mx_wifi_if_init(pnetif);
}


/**
  * @brief                   Mxchip WiFi driver interface initialization
  * @param  pnetif           net interface pointer
  * @return int32_t          0 is success, otherwise failed
  */
static int32_t mx_wifi_if_init(net_if_handle_t *pnetif)
{
  int32_t ret;
  void *ptmp;

  /* Create netif and set Mxchip WiFi functions. */
  ptmp = NET_MALLOC(sizeof(net_if_drv_t));

  if (ptmp != NULL)
  {
    net_if_drv_t *const p = (net_if_drv_t *)ptmp;

    p->if_class = NET_INTERFACE_CLASS_WIFI;

    p->if_init = mx_wifi_if_init;
    p->if_deinit = mx_wifi_if_deinit;

    p->if_start = mx_wifi_if_start;
    p->if_stop = mx_wifi_if_stop;
    p->if_yield = mx_wifi_if_yield;

    p->if_connect = mx_wifi_if_connect;
    p->if_disconnect = mx_wifi_if_disconnect;

    /* socket functions */
#if (MX_WIFI_NETWORK_BYPASS_MODE == 0)
    p->psocket = mx_wifi_socket;
    p->pbind = mx_wifi_bind;
    p->plisten = mx_wifi_listen;
    p->paccept = mx_wifi_accept;
    p->pconnect = mx_wifi_connect;
    p->psend = mx_wifi_send;
    p->precv = mx_wifi_recv;
    p->psendto = mx_wifi_sendto;
    p->precvfrom = mx_wifi_recvfrom;
    p->psetsockopt = mx_wifi_setsockopt;
    p->pgetsockopt = mx_wifi_getsockopt;
    p->pgetsockname = mx_wifi_getsockname;
    p->pgetpeername = mx_wifi_getpeername;
    p->pclose = mx_wifi_close;
    p->pshutdown = mx_wifi_shutdown;
    p->pgethostbyname = mx_wifi_gethostbyname;
    p->pping = mx_wifi_ping;

#else
    p->pping = icmp_ping;
#endif /* MX_WIFI_NETWORK_BYPASS_MODE */


    /* WiFi function. */
    ptmp = NET_MALLOC(sizeof(net_if_wifi_class_extension_t));
    if (NULL == ptmp)
    {
      NET_DBG_ERROR("Can't allocate memory for mx_wifi_driver class\n");
      NET_FREE(p);
      ret = NET_ERROR_NO_MEMORY;
    }
    else
    {
      p->extension.wifi = (net_if_wifi_class_extension_t *)ptmp;
      /* DHCP mode. */
      pnetif->dhcp_mode = true;
      pnetif->pdrv = p;
      /* Scan function. */
      p->extension.wifi->scan = mx_wifi_scan;
      p->extension.wifi->get_scan_results = mx_wifi_get_scan_result;
      p->extension.wifi->mode = NET_WIFI_MODE_STA;

      ret = hw_start(pnetif);
    }
  }
  else
  {
    NET_DBG_ERROR("Can't allocate memory for mx_wifi_driver class\n");
    ret = NET_ERROR_NO_MEMORY;
  }

  return ret;
}


/**
  * @brief                   Mxchip WiFi driver interface de-initialization
  * @param  pnetif           net interface pointer
  * @return int32_t          0 is success, otherwise failed
  */
static int32_t mx_wifi_if_deinit(net_if_handle_t *pnetif)
{
  int32_t ret = NET_OK;

#if (defined(MX_WIFI_NETWORK_BYPASS_MODE) && (MX_WIFI_NETWORK_BYPASS_MODE == 1))
  (void)MX_WIFI_Network_bypass_mode_set(wifi_obj_get(), 0, NULL, NULL);
#endif /* (MX_WIFI_NETWORK_BYPASS_MODE == 1) */

  if (MX_WIFI_STATUS_OK != MX_WIFI_DeInit(wifi_obj_get()))
  {
    ret = NET_ERROR_GENERIC;
  }

  NET_FREE(pnetif->pdrv->extension.wifi);
  pnetif->pdrv->extension.wifi = NULL;
  NET_FREE(pnetif->pdrv);
  pnetif->pdrv = NULL;

  return ret;
}


/**
  * @brief                   Mxchip WiFi driver start
  * @param  pnetif           net interface pointer
  * @return int32_t          0 is success, otherwise failed
  */
static int32_t mx_wifi_if_start(net_if_handle_t *pnetif)
{
  int32_t ret;
  if (pnetif->pdrv->extension.wifi->mode == NET_WIFI_MODE_STA)
  {
    ret = mx_wifi_if_start_station(pnetif);

#if (defined(MX_WIFI_NETWORK_BYPASS_MODE) && (MX_WIFI_NETWORK_BYPASS_MODE == 1))
    if (NET_OK == ret)
    {
      Station_pNetif = pnetif;
    }
#endif /* (MX_WIFI_NETWORK_BYPASS_MODE == 1) */
  }
  else
  {
    ret = mx_wifi_if_start_softap(pnetif);

#if (defined(MX_WIFI_NETWORK_BYPASS_MODE) && (MX_WIFI_NETWORK_BYPASS_MODE == 1))
    if (NET_OK == ret)
    {
      SoftAP_pNetif = pnetif;
    }
#endif /* (MX_WIFI_NETWORK_BYPASS_MODE == 1) */
  }

  return ret;
}


/**
  * @brief                   Mxchip WiFi hardware initialization
  * @param  pnetif           net interface pointer
  * @return int32_t          0 is success, otherwise failed
  */
static int32_t hw_start(net_if_handle_t *pnetif)
{
  int32_t ret = NET_ERROR_DEVICE_ERROR;

  if (mxwifi_probe(&pnetif->pdrv->context) == 0)
  {
    DEBUG_LOG("%s\n", "MX_WIFI IO [OK]");

    if (wifi_obj_get()->Runtime.interfaces == 0U)
    {
      /* WiFi module hardware reboot. */
      DEBUG_LOG("%s\n", "MX_WIFI REBOOT(HW) ...");
      ret = MX_WIFI_HardResetModule(wifi_obj_get());
    }
    else
    {
      ret = MX_WIFI_STATUS_OK;
    }

    if (MX_WIFI_STATUS_OK != ret)
    {
      ret = NET_ERROR_DEVICE_ERROR;
    }
    else
    {
      /* Wait for Mxchip WiFi reboot. */

      /* Initialize the WiFi module. */
      if (MX_WIFI_STATUS_OK != MX_WIFI_Init(wifi_obj_get()))
      {
        ret = NET_ERROR_INTERFACE_FAILURE;
      }
      else
      {
        DEBUG_LOG("%s\n", "MX_WIFI_Init [OK]");
        /* Retrieve the WiFi module information. */
        (void)strncpy(pnetif->DeviceName, (const char *)wifi_obj_get()->SysInfo.Product_Name, sizeof(pnetif->DeviceName));
        (void)strncpy(pnetif->DeviceID, (const char *)wifi_obj_get()->SysInfo.Product_ID, sizeof(pnetif->DeviceID));
        (void)strncpy(pnetif->DeviceVer, (const char *)wifi_obj_get()->SysInfo.FW_Rev, sizeof(pnetif->DeviceVer));

        (void)MX_WIFI_GetMACAddress(wifi_obj_get(), pnetif->macaddr.mac);

        /* Set bypass mode. */
#if (defined(MX_WIFI_NETWORK_BYPASS_MODE) && (MX_WIFI_NETWORK_BYPASS_MODE == 1))
        if (MX_WIFI_STATUS_OK != MX_WIFI_Network_bypass_mode_set(wifi_obj_get(), 1,
                                                                 net_mx_wifi_netlink_input_callback, pnetif))
        {
          NET_DBG_ERROR("*** set mx_wifi module bypass mode failed!\n");
          ret = NET_ERROR_MODULE_INITIALIZATION;
        }
        else
#endif /* (MX_WIFI_NETWORK_BYPASS_MODE == 1) */
        {
          ret = NET_OK;
        }
      }
    }
  }

  return ret;
}


/**
  * @brief                   Mxchip WiFi driver interface stop
  * @param  pnetif           net interface pointer
  * @return int32_t          0 if success, otherwise failed
  */
static int32_t mx_wifi_if_stop(net_if_handle_t *pnetif)
{
  if (pnetif->pdrv->extension.wifi->mode == NET_WIFI_MODE_STA)
  {
    if (((int32_t)NET_WIFI_SM_WPS_OPEN == pnetif->pdrv->extension.wifi->credentials->security_mode) || \
        ((int32_t)NET_WIFI_SM_WPS_SECURE == pnetif->pdrv->extension.wifi->credentials->security_mode))
    {
      (void)MX_WIFI_WPS_Stop(wifi_obj_get());
    }
    (void)MX_WIFI_Disconnect(wifi_obj_get());

#if (defined(MX_WIFI_NETWORK_BYPASS_MODE) && (MX_WIFI_NETWORK_BYPASS_MODE == 1))
    Station_pNetif = NULL;
#endif /* (MX_WIFI_NETWORK_BYPASS_MODE == 1) */
  }
  else
  {
    (void)MX_WIFI_StopAP(wifi_obj_get());

#if (defined(MX_WIFI_NETWORK_BYPASS_MODE) && (MX_WIFI_NETWORK_BYPASS_MODE == 1))
    SoftAP_pNetif = NULL;
#endif /* (MX_WIFI_NETWORK_BYPASS_MODE == 1) */
  }

  (void) net_state_manage_event(pnetif, NET_EVENT_INTERFACE_INITIALIZED);

  return NET_OK;
}


/**
  * @brief                   Mxchip WiFi interface data yield
  * @param  pnetif           net interface
  * @param  timeout_ms       timeout in milliseconds
  * @return int32_t          0 if success, otherwise failed
  */
static int32_t mx_wifi_if_yield(net_if_handle_t *pnetif, uint32_t timeout_ms)
{
  const int32_t ret = MX_WIFI_IO_YIELD(wifi_obj_get(), timeout_ms);

  (void)pnetif;

  return ret;
}


/**
  * @brief                   Mxchip WiFi status change callback
  * @param  cate             status cate
  * @param  status           status
  * @param  arg              user arguments
  */
static void mx_wifi_status_changed(uint8_t cate, uint8_t status, void *arg)
{
  net_if_handle_t *const pnetif = (net_if_handle_t *)arg;
  net_state_t net_state = NET_STATE_DEINITIALIZED;

  net_if_getState(pnetif, &net_state);

  if ((uint8_t)MC_STATION == cate)
  {
    switch (status)
    {
      case MWIFI_EVENT_STA_DOWN:
      {
        DEBUG_LOG("%s\n", "MWIFI_EVENT_STA_DOWN");

        if (NET_STATE_STOPPING == net_state)
        {
          (void)net_state_manage_event(pnetif, NET_EVENT_INTERFACE_INITIALIZED);
        }
        else if (NET_STATE_CONNECTED == net_state)
        {
          (void)net_state_manage_event(pnetif, NET_EVENT_LINK_DOWN);
        }
        else
        {
        }
      }
      break;

      case MWIFI_EVENT_STA_UP:
      {
        DEBUG_LOG("%s\n", "MWIFI_EVENT_STA_UP");

#if (defined(MX_WIFI_NETWORK_BYPASS_MODE) && (MX_WIFI_NETWORK_BYPASS_MODE == 1))
        if (NET_STATE_STARTING == net_state)
        {
          (void)net_state_manage_event(pnetif, NET_EVENT_INTERFACE_READY);
        }
#else
        (void)net_state_manage_event(pnetif, NET_EVENT_INTERFACE_READY);
#endif /* MX_WIFI_NETWORK_BYPASS_MODE */
      }
      break;

      case MWIFI_EVENT_STA_GOT_IP:
      {
        DEBUG_LOG("%s\n", "MWIFI_EVENT_STA_GOT_IP");

#ifdef NET_USE_LWIP_DEFINITIONS
        pnetif->ipaddr.type = (u8_t)IPADDR_TYPE_V4;
        pnetif->ipaddr.u_addr.ip4.addr = NET_ARTON(wifi_obj_get()->NetSettings.IP_Addr);
        pnetif->netmask.type = (u8_t)IPADDR_TYPE_V4;
        pnetif->netmask.u_addr.ip4.addr = NET_ARTON(wifi_obj_get()->NetSettings.IP_Mask);
        pnetif->gateway.type = (u8_t)IPADDR_TYPE_V4;
        pnetif->gateway.u_addr.ip4.addr = NET_ARTON(wifi_obj_get()->NetSettings.Gateway_Addr);

#if defined(NET_USE_IPV6) && (NET_USE_IPV6 == 1)
        pnetif->ipaddr6.type = (u8_t)IPADDR_TYPE_V6;
        (void) memcpy(pnetif->ipaddr6.u_addr.ip6.addr, wifi_obj_get()->NetSettings.IP6_Addr[0], 16);
        pnetif->netmask6.type = (u8_t)IPADDR_TYPE_V6;
        (void) memcpy(pnetif->netmask6.u_addr.ip6.addr, wifi_obj_get()->NetSettings.IP6_Mask, 16);
        pnetif->gateway6.type = (u8_t)IPADDR_TYPE_V6;
        (void) memcpy(pnetif->gateway6.u_addr.ip6.addr, wifi_obj_get()->NetSettings.Gateway6_Addr, 16);
#endif /* NET_USE_IPV6 */

#else /* IPv4 only */
        pnetif->ipaddr.addr = NET_ARTON(wifi_obj_get()->NetSettings.IP_Addr);
        pnetif->netmask.addr = NET_ARTON(wifi_obj_get()->NetSettings.IP_Mask);
        pnetif->gateway.addr = NET_ARTON(wifi_obj_get()->NetSettings.Gateway_Addr);
#endif /* NET_USE_LWIP_DEFINITIONS */

#if (MX_WIFI_NETWORK_BYPASS_MODE == 0)
        /**
          * In current driver implementation, its not possible to control the state of the driver
          * between the Ready state (WiFi AP/STA running) and the connected state (IPADDR available).
          * This corresponds to a single state in WiFi Driver (no command to control the IP stack).
          * Current Workaround is to consider that network library Ready state as the one from
          * the driver. So wait for IPaddr event from driver to send the Ready event
          * to grant the transition to the Ready state. Connect and Disconnect state are implemented
          * as transparent state, no action on WiFi driver.
          */
        (void)net_state_manage_event(pnetif, NET_EVENT_IPADDR);
#else
        (void) net_state_manage_event(pnetif, NET_EVENT_IPADDR);
#endif  /* MX_WIFI_NETWORK_BYPASS_MODE */
      }
      break;

      default:
        break;
    }
  }
  else if ((uint8_t)MC_SOFTAP == cate)
  {
    switch (status)
    {
      case MWIFI_EVENT_AP_DOWN:
      {
        DEBUG_LOG("%s\n", "MWIFI_EVENT_AP_DOWN");
        if (NET_STATE_CONNECTED == net_state)
        {
          (void)net_state_manage_event(pnetif, NET_EVENT_LINK_DOWN);
        }
        else
        {
          (void)net_state_manage_event(pnetif, NET_EVENT_INTERFACE_READY);
        }
      }
      break;

      case MWIFI_EVENT_AP_UP:
      {
        DEBUG_LOG("%s\n", "MWIFI_EVENT_AP_UP");
        pnetif->ipaddr = pnetif->static_ipaddr;
        pnetif->gateway = pnetif->static_gateway;
        pnetif->netmask = pnetif->static_netmask;
        (void) net_state_manage_event(pnetif, NET_EVENT_INTERFACE_READY);
      }
      break;

      default:
        break;
    }
  }
  else
  {
    /* nothing */
  }
}


/**
  * @brief                   Mxchip WiFi connect to AP
  * @param  pnetif           net interface
  * @return int32_t          0 if success, otherwise failed
  */
static int32_t mx_wifi_if_start_station(net_if_handle_t *pnetif)
{
  int32_t ret;
  const net_wifi_credentials_t *const credentials = pnetif->pdrv->extension.wifi->credentials;

  if (false == pnetif->dhcp_mode)
  {
    wifi_obj_get()->NetSettings.DHCP_IsEnabled = 0;

    /* IPv6 */
#if defined(NET_USE_IPV6) && (NET_USE_IPV6 == 1)
    (void)memcpy(wifi_obj_get()->NetSettings.IP6_Addr[0], &pnetif->static_ipaddr.u_addr.ip6, 16);
    (void)memcpy(wifi_obj_get()->NetSettings.IP6_Mask, &pnetif->static_netmask.u_addr.ip6, 16);
    (void)memcpy(wifi_obj_get()->NetSettings.Gateway6_Addr, &pnetif->static_gateway.u_addr.ip6, 16);

#else
    /* IPv4 */
    (void)memcpy(wifi_obj_get()->NetSettings.IP_Addr, &pnetif->static_ipaddr, 4);
    (void)memcpy(wifi_obj_get()->NetSettings.IP_Mask, &pnetif->static_netmask, 4);
    (void)memcpy(wifi_obj_get()->NetSettings.Gateway_Addr, &pnetif->static_gateway, 4);
#endif /* NET_USE_IPV6 */
  }
  else
  {
    wifi_obj_get()->NetSettings.DHCP_IsEnabled = 1;
  }

  (void)MX_WIFI_RegisterStatusCallback_if(wifi_obj_get(), mx_wifi_status_changed, pnetif, MC_STATION);

  if (((int32_t)NET_WIFI_SM_WPS_OPEN == credentials->security_mode) || \
      ((int32_t)NET_WIFI_SM_WPS_SECURE == credentials->security_mode))
  {
    ret = MX_WIFI_WPS_Connect(wifi_obj_get());
  }
  else
  {
    /* NOTE: secure type is auto for Mxchip WiFi */
    const MX_WIFI_SecurityType_t secure_type = MX_WIFI_SEC_AUTO;
    ret = MX_WIFI_Connect(wifi_obj_get(), credentials->ssid, credentials->psk, secure_type);
  }

  return ret;
}


/* IP address helpers. */
#define BYTE(A, N) ((A) >> (8u*(N))) & 0xFFu
#define BYTE3(A)   BYTE((A), 3u)
#define BYTE2(A)   BYTE((A), 2u)
#define BYTE1(A)   BYTE((A), 1u)
#define BYTE0(A)   BYTE((A), 0u)
#define ADDR(A)    BYTE0((A)), BYTE1((A)), BYTE2((A)), BYTE3((A))


/**
  * @brief                   Mxchip WiFi start AP mode
  * @param  pnetif           net interface
  * @return int32_t          0 if success, otherwise failed
  */
static int32_t mx_wifi_if_start_softap(net_if_handle_t *pnetif)
{
#if defined(NET_USE_IPV6) && (NET_USE_IPV6 == 1)
  (void)pnetif;
  /* This is independent of what is supported by IP stack IPv6 or Ipv4 */
  return NET_ERROR_UNSUPPORTED;

#else
  int32_t ret = NET_ERROR_GENERIC;

  MX_WIFI_APSettings_t ap_cfg = {0};
  const net_wifi_credentials_t *const credentials = pnetif->pdrv->extension.wifi->credentials;

  if (credentials != NULL)
  {
    (void)strncpy(ap_cfg.SSID, credentials->ssid, sizeof(ap_cfg.SSID));
    (void)strncpy(ap_cfg.pswd, credentials->psk, sizeof(ap_cfg.pswd));
  }

  ap_cfg.channel = pnetif->pdrv->extension.wifi->access_channel;

#ifdef NET_USE_LWIP_DEFINITIONS
  (void)snprintf(ap_cfg.ip.localip, sizeof(ap_cfg.ip.localip), "%"PRIu32".%"PRIu32".%"PRIu32".%"PRIu32"",
                 ADDR(pnetif->static_ipaddr.u_addr.ip4.addr));
  (void)snprintf(ap_cfg.ip.gateway, sizeof(ap_cfg.ip.gateway), "%"PRIu32".%"PRIu32".%"PRIu32".%"PRIu32"",
                 ADDR(pnetif->static_gateway.u_addr.ip4.addr));
  (void)snprintf(ap_cfg.ip.netmask, sizeof(ap_cfg.ip.netmask), "%"PRIu32".%"PRIu32".%"PRIu32".%"PRIu32"",
                 ADDR(pnetif->static_netmask.u_addr.ip4.addr));
  (void)snprintf(ap_cfg.ip.dnserver, sizeof(ap_cfg.ip.dnserver), "%"PRIu32".%"PRIu32".%"PRIu32".%"PRIu32"",
                 ADDR(pnetif->static_dnserver.u_addr.ip4.addr));

#else
  (void)snprintf(ap_cfg.ip.localip, sizeof(ap_cfg.ip.localip), "%"PRIu32".%"PRIu32".%"PRIu32".%"PRIu32"",
                 ADDR(pnetif->static_ipaddr.addr));
  (void)snprintf(ap_cfg.ip.gateway, sizeof(ap_cfg.ip.gateway), "%"PRIu32".%"PRIu32".%"PRIu32".%"PRIu32"",
                 ADDR(pnetif->static_gateway.addr));
  (void)snprintf(ap_cfg.ip.netmask, sizeof(ap_cfg.ip.netmask), "%"PRIu32".%"PRIu32".%"PRIu32".%"PRIu32"",
                 ADDR(pnetif->static_netmask.addr));
  (void)snprintf(ap_cfg.ip.dnserver, sizeof(ap_cfg.ip.dnserver), "%"PRIu32".%"PRIu32".%"PRIu32".%"PRIu32"",
                 ADDR(pnetif->static_dnserver.addr));
#endif /* NET_USE_LWIP_DEFINITIONS */

  (void)MX_WIFI_RegisterStatusCallback_if(wifi_obj_get(), mx_wifi_status_changed, pnetif, MC_SOFTAP);

  if (MX_WIFI_STATUS_OK == MX_WIFI_StartAP(wifi_obj_get(), &ap_cfg))
  {
    /* Soft AP MAC must get after Soft AP up, call this one to update SysInfo.apMAC. */
    (void)MX_WIFI_GetsoftapMACAddress(wifi_obj_get(), pnetif->macaddr.mac);

    ret = NET_OK;
  }

  return ret;
#endif /* NET_USE_IPV6 */
}


/**
  * @brief                   Mxchip WiFi disconnect from AP
  * @param  pnetif           net interface
  * @return int32_t          0 if success, otherwise failed
  */
static int32_t mx_wifi_if_disconnect(net_if_handle_t *pnetif)
{
  int32_t ret;

#if (defined(MX_WIFI_NETWORK_BYPASS_MODE) && (MX_WIFI_NETWORK_BYPASS_MODE == 1))
  ret = net_ip_disconnect(pnetif);
  if (ret == NET_OK)
  {
    ret = net_ip_remove_if(pnetif, NULL);
    if (ret == NET_OK)
    {
      (void)net_state_manage_event(pnetif, NET_EVENT_INTERFACE_READY);
    }
  }

#else
  ret = NET_OK;
  (void) net_state_manage_event(pnetif, NET_EVENT_INTERFACE_READY);
#endif /* (MX_WIFI_NETWORK_BYPASS_MODE == 1) */

  return ret;
}


/**
  * @brief                   Mxchip WiFi start IP services
  * @param  pnetif           net interface
  * @return int32_t          0 if success, otherwise failed
  */
static int32_t mx_wifi_if_connect(net_if_handle_t *pnetif)
{
  int32_t ret;

#if (defined(MX_WIFI_NETWORK_BYPASS_MODE) && (MX_WIFI_NETWORK_BYPASS_MODE == 1))
  /* netif add for bypass mode */

  ret = net_ip_add_if(pnetif, low_level_init, NET_ETHERNET_FLAG_DEFAULT_IF);
  if (ret == NET_OK)
  {
    netif_set_link_up(pnetif->netif);

    ret = net_ip_connect(pnetif);
    if (ret != NET_OK)
    {
      NET_DBG_ERROR("*** net_ip_connect failed!\n");
      ret = NET_ERROR_NO_CONNECTION;
    }
  }
  else
  {
    NET_DBG_ERROR("Can't add interface (netif)\n");
  }

#else
  mwifi_if_t wifi_mode;
  bool ip_got_flag = false;

  ret = NET_OK;

  if (pnetif->pdrv->extension.wifi->mode == NET_WIFI_MODE_AP)
  {
    wifi_mode = MC_SOFTAP;
  }
  else
  {
    wifi_mode = MC_STATION;
  }

#if (defined(NET_USE_IPV6) && (NET_USE_IPV6 == 1))
  {
    uint8_t ip[16] = {0};
    if (MX_WIFI_STATUS_OK == MX_WIFI_GetIP6Address(wifi_obj_get(), &ip[0], 0, wifi_mode))
    {
      pnetif->ipaddr6.type = (u8_t)IPADDR_TYPE_V6;
      (void) memcpy(pnetif->ipaddr6.u_addr.ip6.addr, wifi_obj_get()->NetSettings.IP6_Addr[0], 16);
      pnetif->netmask6.type = (u8_t)IPADDR_TYPE_V6;
      (void) memcpy(pnetif->netmask6.u_addr.ip6.addr, wifi_obj_get()->NetSettings.IP6_Mask, 16);
      pnetif->gateway6.type = (u8_t)IPADDR_TYPE_V6;
      (void) memcpy(pnetif->gateway6.u_addr.ip6.addr, wifi_obj_get()->NetSettings.Gateway6_Addr, 16);

      ip_got_flag = true;
    }
    else
    {
      NET_DBG_ERROR("Can't get IPv6 address!\n");
      ret = NET_ERROR_NO_ADDRESS;
    }
  }

#else
  {
    uint8_t ip[4] = {0};
    int32_t retry = 3;
    while (retry > 0)
    {
      retry--;
      if (MX_WIFI_STATUS_OK == MX_WIFI_GetIPAddress(wifi_obj_get(), &ip[0], wifi_mode))
      {

#ifdef NET_USE_LWIP_DEFINITIONS
        pnetif->ipaddr.type = (u8_t)IPADDR_TYPE_V4;
        pnetif->ipaddr.u_addr.ip4.addr = NET_ARTON(wifi_obj_get()->NetSettings.IP_Addr);
        pnetif->netmask.u_addr.ip4.addr = NET_ARTON(wifi_obj_get()->NetSettings.IP_Mask);
        pnetif->gateway.u_addr.ip4.addr = NET_ARTON(wifi_obj_get()->NetSettings.Gateway_Addr);
#else
        pnetif->ipaddr.addr = NET_ARTON(wifi_obj_get()->NetSettings.IP_Addr);
        pnetif->netmask.addr = NET_ARTON(wifi_obj_get()->NetSettings.IP_Mask);
        pnetif->gateway.addr = NET_ARTON(wifi_obj_get()->NetSettings.Gateway_Addr);
#endif /* NET_USE_LWIP_DEFINITIONS */

        ip_got_flag = true;
        ret = NET_OK;
        break;
      }
      else
      {
        ret = NET_ERROR_NO_ADDRESS;
        DELAY_MS(1000);
      }
    }
  }
#endif  /* NET_USE_IPV6 */


  if (ip_got_flag)
  {
    (void)net_state_manage_event(pnetif, NET_EVENT_IPADDR);
  }

#endif /* (MX_WIFI_NETWORK_BYPASS_MODE == 1) */

  return ret;
}


/**
  * @brief                   Mxchip WiFi scan function
  * @param  pnetif           net interface
  * @param  mode             scan mode
  * @param  ssid             SSID to scan in active mode
  * @return int32_t          0 if success, otherwise failed
  */
static int32_t mx_wifi_scan(net_if_handle_t *pnetif, net_wifi_scan_mode_t mode, char *ssid)
{
  int32_t ret;
  uint32_t len = 0u;

  (void) pnetif;

  if (ssid != NULL)
  {
    len = strlen(ssid);
  }
  ret = MX_WIFI_Scan(wifi_obj_get(), (uint8_t)mode, ssid, (int32_t) len);
  return ret;
}


/**
  * @brief                       Mxchip WiFi get scan result
  * @param  pnetif               net interface
  * @param  scan_bss_array       BSS array buffer
  * @param  scan_bss_count       BSS count
  * @return int32_t              The number of BSS obtained
  */
static int32_t mx_wifi_get_scan_result(net_if_handle_t *pnetif, net_wifi_scan_results_t *scan_bss_array,
                                       uint8_t scan_bss_count)
{
  int32_t ret = NET_ERROR_PARAMETER;
  /* Mxchip WiFi security mode */
  static const uint32_t mxsec[] =
  {
    NET_WIFI_SM_OPEN,
    NET_WIFI_SM_WEP_PSK,        /**< Wired Equivalent Privacy. WEP security. */
    NET_WIFI_SM_WPA_TKIP_PSK,   /**< WPA /w TKIP */
    NET_WIFI_SM_WPA_AES_PSK,    /**< WPA /w AES */
    NET_WIFI_SM_WPA2_TKIP_PSK,  /**< WPA2 /w TKIP */
    NET_WIFI_SM_WPA2_AES_PSK,   /**< WPA2 /w AES */
    NET_WIFI_SM_WPA2_MIXED_PSK  /**< WPA2 /w AES or TKIP */
  };

  (void)pnetif;

  if ((NULL != scan_bss_array) && (0u != scan_bss_count))
  {
    /* Create buffer for results. */
    mwifi_ap_info_t *ap_infos_tmp = (mwifi_ap_info_t *)NET_CALLOC(scan_bss_count, sizeof(*ap_infos_tmp));

    if (NULL == ap_infos_tmp)
    {
      ret = NET_ERROR_NO_MEMORY;
    }
    else
    {
      /* Get real mx_wifi scan results data. */
      const int8_t number = MX_WIFI_Get_scan_result(wifi_obj_get(), (uint8_t *) ap_infos_tmp, scan_bss_count);

      /* transfer to ST bss info */
      {
        mwifi_ap_info_t *ap_info = ap_infos_tmp;
        net_wifi_scan_bss_t *scan_bss = scan_bss_array;

        for (int8_t i = 0U; i < number; i++)
        {
          (void) memset(scan_bss, 0, sizeof(*scan_bss));
          (void) memcpy(scan_bss->ssid.value, ap_info->ssid, NET_WIFI_MAX_SSID_SIZE);
          scan_bss->ssid.length = (uint8_t) strlen(ap_info->ssid);
          scan_bss->security = mxsec[ap_info->security];
          (void) memcpy(&scan_bss->bssid, ap_info->bssid, NET_WIFI_MAC_ADDRESS_SIZE);
          scan_bss->rssi = (int8_t)ap_info->rssi;
          scan_bss->channel = (uint8_t) ap_info->channel;
          (void) memcpy(scan_bss->country, ".CN", 4);  /* NOT SUPPORT for MX_WIFI */

          scan_bss++;
          ap_info++;
        }
      }
      ret = (int32_t) number;
      NET_FREE(ap_infos_tmp);
    }
  }
  return ret;
}


#if (defined(MX_WIFI_NETWORK_BYPASS_MODE) && (MX_WIFI_NETWORK_BYPASS_MODE == 0))
/**
  * @brief                   Mxchip WiFi socket create
  * @param  domain           socket domain
  * @param  type             socket type
  * @param  protocol         socket protocol
  * @return int32_t          socket fd, if failed return error code(<0)
  */
static int32_t mx_wifi_socket(int32_t domain, int32_t type, int32_t protocol)
{
  const int32_t ret = MX_WIFI_Socket_create(wifi_obj_get(), domain, type, protocol);
  return ret;
}


/**
  * @brief                   Mxchip WiFi set socket option
  * @param  sock             socket value
  * @param  level            socket level
  * @param  optname          socket option name
  * @param  optvalue         socket option value to set
  * @param  optlen           socket option value length
  * @return int32_t          0 if success, if failed, return error code(< 0)
  */
static int32_t mx_wifi_setsockopt(int32_t sock, int32_t level, int32_t optname, const void *optvalue, uint32_t optlen)
{
  int32_t ret = MX_WIFI_STATUS_OK;

  /* Handle setsockopt forwarded by ST NetworkLibrary API. */
  /**
    * Note: bind device option is not used for the mxchip WiFi module,
    *       the module select the net interface by the socket address internally.
    */
  if (NET_SO_BINDTODEVICE != optname)
  {
    ret = MX_WIFI_Socket_setsockopt(wifi_obj_get(), sock, level, optname, optvalue, (int32_t)optlen);
  }

  return ret;
}


/**
  * @brief                   Mxchip wiFi get socket option
  * @param  sock             socket value
  * @param  level            socket level
  * @param  optname          socket option name
  * @param  optvalue         socket option value
  * @param  optlen           socket option value length
  * @return int32_t          0 if success, if failed return error code(<0)
  */
static int32_t mx_wifi_getsockopt(int32_t sock, int32_t level, int32_t optname, void *optvalue, uint32_t *optlen)
{
  const int32_t ret = MX_WIFI_Socket_getsockopt(wifi_obj_get(), sock, level, optname, optvalue, optlen);

  return ret;
}


/**
  * @brief                   Mxchip WiFi bind socket
  * @param  sock             socket value
  * @param  addr             address to bind
  * @param  addrlen          address length
  * @return int32_t          0 if success, if failed, return error code(<0)
  */
static int32_t mx_wifi_bind(int32_t sock, const net_sockaddr_t *addr, uint32_t addrlen)
{
  int32_t ret = MX_WIFI_STATUS_ERROR;

  if (NULL != addr)
  {
    if ((addrlen == sizeof(struct mx_sockaddr_in6)) || (addrlen == sizeof(struct mx_sockaddr_in /*mx_sockaddr*/)))
    {
      ret = MX_WIFI_Socket_bind(wifi_obj_get(), sock, (const struct mx_sockaddr *)addr, (int32_t)addrlen);
    }
  }

  return ret;
}


/**
  * @brief                   Mxchip WiFi socket listen
  * @param  sock             socket value
  * @param  backlog          max socket to accept
  * @return int32_t          0 if success, if failed return error code(<0)
  */
static int32_t mx_wifi_listen(int32_t sock, int32_t backlog)
{
  const int32_t ret = MX_WIFI_Socket_listen(wifi_obj_get(), sock, backlog);

  return ret;
}


/**
  * @brief                   Mxchip WiFi socket accept
  * @param  sock             server socket value
  * @param  addr             address buffer to accept new socket
  * @param  addrlen          address buffer length
  * @return int32_t          accepted socket fd, if failed return error code(<0)
  */
static int32_t mx_wifi_accept(int32_t sock, net_sockaddr_t *addr, uint32_t *addrlen)
{
  const int32_t ret = MX_WIFI_Socket_accept(wifi_obj_get(), sock, (struct mx_sockaddr *)addr, addrlen);

  return ret;
}


/**
  * @brief                   mxchip WiFi socket connect
  * @param  sock             socket value
  * @param  addr             address to connect
  * @param  addrlen          address length
  * @return int32_t          0 if success, if failed return error code (<0)
  */
static int32_t mx_wifi_connect(int32_t sock, const net_sockaddr_t *addr, uint32_t addrlen)
{
  const int32_t ret = MX_WIFI_Socket_connect(wifi_obj_get(), sock, (const struct mx_sockaddr *)addr, (int32_t)addrlen);

  return ret;
}


/**
  * @brief                   mxchip WiFi socket shutdown
  * @param  sock             socket value
  * @param  mode             shutdown mode (0: stop receiving, 1: stop sending, 2: stop send and receive)
  * @return int32_t          0 if success, if failed return error code (<0)
  */
static int32_t mx_wifi_shutdown(int32_t sock, int32_t mode)
{
  const int32_t ret = MX_WIFI_Socket_shutdown(wifi_obj_get(), sock, mode);

  return ret;
}


/**
  * @brief                   mxchip WiFi socket close
  * @param  sock             socket value
  * @param  isaclone         not supported, just compliant for ST connect lib
  * @return int32_t          0 if success, if failed return error code (<0)
  */
static int32_t mx_wifi_close(int32_t sock, bool isaclone)
{
  const int32_t ret = MX_WIFI_Socket_close(wifi_obj_get(), sock);

  (void)isaclone;

  return ret;
}


/**
  * @brief                   mxchip WiFi socket send
  * @param  sock             socket value
  * @param  buf              data buffer to send
  * @param  len              data length
  * @param  flags            socket flag
  * @return int32_t          bytes sent, if failed return error code(<0)
  */
static int32_t mx_wifi_send(int32_t sock, const uint8_t *buf, int32_t len, int32_t flags)
{
  const int32_t ret = MX_WIFI_Socket_send(wifi_obj_get(), sock, buf, len, flags);

  return ret;
}


/**
  * @brief                   mxchip WiFi socket recv
  * @param  sock             socket value
  * @param  buf              buffer to recv data
  * @param  len              buffer size
  * @param  flags            socket flag
  * @return int32_t          bytes received, if failed return error code(<0)
  */
static int32_t mx_wifi_recv(int32_t sock, uint8_t *buf, int32_t len, int32_t flags)
{
  int32_t ret = MX_WIFI_Socket_recv(wifi_obj_get(), sock, buf, len, flags);

  if (MX_WIFI_STATUS_OK != ret)
  {
    if (MX_WIFI_STATUS_TIMEOUT == ret)
    {
      ret = NET_TIMEOUT;
    }
  }

  return ret;
}


/**
  * @brief                   mxchip WiFi socket sendto
  * @param  sock             socket value
  * @param  buf              data buffer to send
  * @param  len              data length
  * @param  flags            socket flag
  * @param  to               address to send to
  * @param  to_len           size of address to send to
  * @return int32_t          bytes sent. If failed return error code(<0)
  */
static int32_t mx_wifi_sendto(int32_t sock, const uint8_t *buf, int32_t len, int32_t flags,
                              net_sockaddr_t *to, uint32_t to_len)
{
  int32_t ret = MX_WIFI_STATUS_ERROR;

  if (NULL != to)
  {
    if ((to_len == sizeof(struct mx_sockaddr_in6)) || (to_len == sizeof(struct mx_sockaddr_in /*mx_sockaddr*/)))
    {
      ret = MX_WIFI_Socket_sendto(wifi_obj_get(), sock, buf, len, flags, (struct mx_sockaddr *)to, (int32_t)to_len);
    }
  }

  return ret;
}


/**
  * @brief                   Mxchip WiFi socket recvfrom
  * @param  sock             socket value
  * @param  buf              buffer to receive data
  * @param  len              buffer size
  * @param  flags            socket flag
  * @param  from             address of the data source
  * @param  from_len         size of address
  * @return int32_t          bytes received, if failed, return error code(<0)
  */
static int32_t mx_wifi_recvfrom(int32_t sock, uint8_t *buf, int32_t len, int32_t flags,
                                net_sockaddr_t *from, uint32_t *from_len)
{
  int32_t ret = MX_WIFI_STATUS_ERROR;

  if ((NULL != from) && (NULL != from_len))
  {
    if ((*from_len == sizeof(struct mx_sockaddr_in6)) || (*from_len == sizeof(struct mx_sockaddr_in /*mx_sockaddr*/)))
    {
      ret = MX_WIFI_Socket_recvfrom(wifi_obj_get(), sock, buf, len, flags, (struct mx_sockaddr *)from, from_len);
    }
  }

  return ret;
}


/**
  * @brief                   mxchip WiFi gethostbyname
  * @param  pnetif           net interface
  * @param  addr             address of the host
  * @param  name             host name
  * @return int32_t          0 if success, otherwise failed
  */
static int32_t mx_wifi_gethostbyname(net_if_handle_t *pnetif, net_sockaddr_t *addr, char_t *name)
{
  int32_t ret;
  (void)pnetif;

#if (defined(NET_USE_IPV6) && (NET_USE_IPV6 == 1))
  if (NET_AF_INET6 == addr->sa_family)
  {
    struct mx_addrinfo hints = {0};
    struct mx_addrinfo res = {0};

    hints.ai_flags = 0;
    hints.ai_family = MX_AF_INET6;
    hints.ai_socktype = MX_IPPROTO_TCP;
    hints.ai_protocol = MX_SOCK_STREAM;

    /* Keep port possibly set as it is. */
    res.ai_addr.s2_data1[0] = addr->sa_data[0];
    res.ai_addr.s2_data1[1] = addr->sa_data[1];

    ret = MX_WIFI_Socket_getaddrinfo(wifi_obj_get(), name, NULL, &hints, &res);
    if (MX_WIFI_STATUS_OK == ret)
    {
      (void)memcpy(addr, &res.ai_addr, res.ai_addrlen);
    }
  }
  else
#endif /* NET_USE_IPV6 */
  {
    /* IPv4 */
    ret = MX_WIFI_Socket_gethostbyname(wifi_obj_get(), (struct mx_sockaddr *)addr, (mx_char_t *)name);
  }

  return ret;
}


/**
  * @brief                   mxchip WiFi ping function
  * @param  pnetif           net interface
  * @param  addr             address to ping
  * @param  count            request count
  * @param  delay            request delay in milliseconds
  * @param  response         response array, size is count
  * @return int32_t          0 if success, otherwise failed
  */
static int32_t mx_wifi_ping(net_if_handle_t *pnetif, net_sockaddr_t *addr, int32_t count, int32_t delay,
                            int32_t response[])
{
  int32_t ret = -1;
  net_ip_addr_t ip_addr = {0};

  (void)pnetif;

#if (defined(NET_USE_IPV6) && (NET_USE_IPV6 == 1))
  if (addr->sa_family == NET_AF_INET6)
  {
    net_sockaddr_in6_t * const p_s_addr_in6 = (net_sockaddr_in6_t *)addr;
    inet6_addr_to_ip6addr(&ip_addr.u_addr.ip6, &p_s_addr_in6->sin6_addr);
    ip_addr.type = (u8_t)IPADDR_TYPE_V6;
  }
  else
#endif /* NET_USE_IPV6 */
  {
    const net_sockaddr_in_t * const p_s_addr_in = (net_sockaddr_in_t *)addr;

#ifdef NET_USE_LWIP_DEFINITIONS
    ip_addr.u_addr.ip4.addr = p_s_addr_in->sin_addr.s_addr;
#else
    ip_addr.addr = p_s_addr_in->sin_addr.s_addr;
#endif /* NET_USE_LWIP_DEFINITIONS */
  }

  if (addr->sa_family == NET_AF_INET6)
  {
    ret = MX_WIFI_Socket_ping6(wifi_obj_get(), (char_t *)NET_NTOA(&ip_addr), count, delay, response);
  }
  else
  {
    ret = MX_WIFI_Socket_ping(wifi_obj_get(), (char_t *)NET_NTOA(&ip_addr), count, delay, response);
  }

  return ret;
}

/**
  * @brief                   Mxchip WiFi getsockname
  * @param  sock             socket value
  * @param  name             socket name
  * @param  namelen          socket name length
  * @return int32_t          0 if success, otherwise failed
  */
static int32_t mx_wifi_getsockname(int32_t sock, net_sockaddr_t *name, uint32_t *namelen)
{
  const int32_t ret = MX_WIFI_Socket_getsockname(wifi_obj_get(), sock, (struct mx_sockaddr *) name, namelen);

  return ret;
}


/**
  * @brief                   Mxchip WiFi get peer name
  * @param  sock             socket value
  * @param  name             socket name
  * @param  namelen          socket name length
  * @return int32_t          0 if success, otherwise failed
  */
static int32_t mx_wifi_getpeername(int32_t sock, net_sockaddr_t *name, uint32_t *namelen)
{
  const int32_t ret = MX_WIFI_Socket_getpeername(wifi_obj_get(), sock, (struct mx_sockaddr *) name, namelen);

  return ret;
}

#endif /* MX_WIFI_NETWORK_BYPASS_MODE */
