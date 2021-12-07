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
#include "net_connect.h"
#include "net_internals.h"
#include "net_errors.h"
#include "net_mem.h"
#include "net_perf.h"

#include "mx_wifi.h"
#include "core/mx_wifi_hci.h"
#include <inttypes.h>

#if (MX_WIFI_NETWORK_BYPASS_MODE == 1)
#include "net_ip_lwip.h"
#include "lwip/etharp.h"
#include "lwip/tcpip.h"
#endif /* MX_WIFI_NETWORK_BYPASS_MODE */

/* mxchip wifi api header file and configuration*/
#include "mx_wifi.h"

/* max wifi frame length */
#define MX_WIFI_PAYLOAD_MTU   (MX_WIFI_MTU_SIZE + MX_WIFI_BYPASS_HEADER_SIZE)

/* debug log function */
#ifndef MX_WIFI_BYPASS_DEBUG
#define MX_WIFI_BYPASS_DEBUG  (0)
#endif /* MX_WIFI_BYPASS_DEBUG */


#ifdef MX_WIFI_API_DEBUG
#define DEBUG_LOG(M, ...)  printf((M), ##__VA_ARGS__)
#else
#define DEBUG_LOG(M, ...)
#endif /* MX_WIFI_API_DEBUG */


/* IPv4 address transfer */
#define NET_ARTON(A)     ((uint32_t)(((uint32_t)A[3] << 24U) |\
                                     ((uint32_t)A[2] << 16U) |\
                                     ((uint32_t)A[1] <<  8U) |\
                                     ((uint32_t)A[0] <<  0U)))

/* Declaration of generic class functions               */

/* function declaration */
void HAL_Delay(uint32_t Delay);

/* mxchip wifi adaptor functions for ST connect library */
int32_t mx_wifi_driver(net_if_handle_t *pnetif);

static int32_t mx_wifi_if_init(net_if_handle_t *pnetif);
static int32_t mx_wifi_if_deinit(net_if_handle_t *pnetif);

static int32_t mx_wifi_if_start(net_if_handle_t *pnetif);
static int32_t mx_wifi_if_stop(net_if_handle_t *pnetif);
static int32_t mx_wifi_if_yield(net_if_handle_t *pnetif, uint32_t timeout);
static void mx_wifi_status_changed(uint8_t cate, uint8_t status, void *arg);

static int32_t mx_wifi_if_connect(net_if_handle_t *pnetif);
static int32_t mx_wifi_if_disconnect(net_if_handle_t *pnetif);

/* bypass mode functions */
#if (MX_WIFI_NETWORK_BYPASS_MODE == 1)
static err_t low_level_output(struct netif *netif, struct pbuf *p);
static err_t low_level_init(struct netif *netif);
static FIFO_DECLARE(fifo);
void   _MX_WIFI_TranmitThread(void const *context);

#else
static int32_t mx_wifi_socket(int32_t domain, int32_t type, int32_t protocol);
static int32_t mx_wifi_bind(int32_t sock, const net_sockaddr_t *addr, uint32_t addrlen);
static int32_t mx_wifi_listen(int32_t sock, int32_t backlog);
static int32_t mx_wifi_accept(int32_t sock, net_sockaddr_t *addr, uint32_t *addrlen);
static int32_t mx_wifi_connect(int32_t sock, const net_sockaddr_t *addr, uint32_t addrlen);
static int32_t mx_wifi_send(int32_t sock, uint8_t *buf, int32_t len, int32_t flags);
static int32_t mx_wifi_recv(int32_t sock, uint8_t *buf, int32_t len, int32_t flags);
static int32_t mx_wifi_sendto(int32_t sock, uint8_t *buf, int32_t len, int32_t flags, net_sockaddr_t *to,
                              uint32_t tolen);
static int32_t mx_wifi_recvfrom(int32_t sock, uint8_t *buf, int32_t len, int32_t flags, net_sockaddr_t *from,
                                uint32_t *fromlen);
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

/* Declaration and definition of class-specific functions */
static int32_t mx_wifi_scan(net_if_handle_t *pnetif, net_wifi_scan_mode_t mode, char *ssid);
static int32_t mx_wifi_get_scan_result(net_if_handle_t *pnetif, net_wifi_scan_results_t *scan_bss_array,
                                       uint8_t scan_bss_array_size);
static int32_t mx_wifi_if_start_station(net_if_handle_t *pnetif);
static int32_t mx_wifi_if_start_softap(net_if_handle_t *pnetif);
static int32_t hw_start(net_if_handle_t *pnetif);

extern int32_t mxwifi_probe(void **ll_drv_context);
extern MX_WIFIObject_t *wifi_obj_get(void);
extern uint32_t HAL_GetTick(void);


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
#if (MX_WIFI_NETWORK_BYPASS_MODE == 1)
/**
  * @brief                   low level init function for wifi driver
  * @param  netif            low level net interface
  * @return err_t            return ERR_OK if success, otherwise error code
  */
static err_t low_level_init(struct netif *netif)
{
  err_t ret = -1;
  MX_WIFIObject_t *pMxWifiObj = wifi_obj_get();
  static THREAD_DECLARE(MX_WIFI_TransmitThreadId);

  FIFO_INIT(fifo, MX_WIFI_MAX_TX_BUFFER_COUNT);

  if (THREAD_OK == THREAD_INIT(MX_WIFI_TransmitThreadId, _MX_WIFI_TranmitThread, NULL,
                               MX_WIFI_TRANSMIT_THREAD_STACK_SIZE,
                               MX_WIFI_TRANSMIT_THREAD_PRIORITY))
  {
    /* set interface name for mxchip wifi */
    netif->name[0] = 'm';
    netif->name[1] = 'x';

    /* We directly use etharp_output() here to save a function call.
      * You can instead declare your own function an call etharp_output()
      * from it if you have to do some checks before sending (e.g. if link
      * is available...)
      */
    netif->output = etharp_output;
#if LWIP_IPV6
    /* netif->output_ip6 = ethip6_output; */
#endif /* LWIP_IPV6 */
    netif->linkoutput = low_level_output;

    /* Set MAC hardware address length ( 6)*/
    netif->hwaddr_len = (u8_t) ETHARP_HWADDR_LEN;

    /* Setup the physical address of this IP instance. */
    (void)memcpy(netif->hwaddr, pMxWifiObj->SysInfo.MAC, 6);
#if MX_WIFI_BYPASS_DEBUG
    NET_PRINT(" MAC address %x.%x.%x.%x.%x.%x\n", netif->hwaddr[0], netif->hwaddr[1], netif->hwaddr[2],
              netif->hwaddr[3], netif->hwaddr[4], netif->hwaddr[5]);
#endif /* MX_WIFI_BYPASS_DEBUG */

    /* Set Maximum Transfer Unit */
    netif->mtu = (u16_t) MX_WIFI_PAYLOAD_MTU;

    /* Set device capabilities. Don't set NETIF_FLAG_ETHARP if this device is not an Ethernet one */
    netif->flags = (u8_t)(NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP | NETIF_FLAG_ETHERNET);

    /* Do whatever else is needed to initialize interface. */
#if LWIP_IGMP
    netif->flags |= NETIF_FLAG_IGMP;
    netif_set_igmp_mac_filter(netif, lwip_igmp_mac_filter);
#endif /* LWIP_IGMP */

    /* Register a handler for any address changes  and when interface goes up or down*/
    netif_set_status_callback(netif, net_ip_status_cb);
    netif_set_link_callback(netif, net_ip_status_cb);

    ret = (err_t)ERR_OK;
  }
  return ret;
}

/* This function should do the actual transmission of the packet. The packet is
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

void PushToDriver(uint32_t timeout);
static err_t low_level_output(struct netif *netif, struct pbuf *p)
{
  err_t err = (err_t) ERR_ARG;
  struct pbuf *pbuf_send;
  MX_WIFIObject_t *pMxWifiObj = wifi_obj_get();

  (void)netif;

  if ((pMxWifiObj != NULL) && (NULL != p))
  {
    /* Take a reference to this packet */
    pbuf_ref(p);
    /* (void) printf("Transmit buffer %p next=%p  tot_len=%d len=%d\n", p, p->next, p->tot_len, p->len); */

    /* No chained buffers*/
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

        if (FIFO_OK == FIFO_PUSH(fifo, handle, WAIT_FOREVER, PushToDriver))
        {
          err = (err_t)ERR_OK;
        }
      }
    }
    else
    {
      void *handle = p;
      NET_PERF_TASK_TAG(3);
      if (FIFO_OK == FIFO_PUSH(fifo, handle, WAIT_FOREVER, PushToDriver))
      {
        err = (err_t)ERR_OK;
      }
    }
  }
  else
  {
    /* cannot transmit wifi not ready */

    /* Stop lint warning about packet not being freed - it is not being referenced */ /*@-mustfree@*/
    err = (err_t) ERR_INPROGRESS; /* Note that signaling ERR_CLSD or ERR_CONN causes loss of connectivity on a roam */
    /*@+mustfree@*/
  }

  return err;
}


#if MX_WIFI_TX_BUFFER_NO_COPY
#if PBUF_LINK_ENCAPSULATION_HLEN < MX_WIFI_MIN_TX_HEADER_SIZE
#error "When MX_WIFI_TX_BUFFER_NO_COPY is set, lwipopts.h must reserved buffer headroom thanks to PBUF_LINK_ENCAPSULATION_HLEN"
#endif /* PBUF_LINK_ENCAPSULATION_HLEN < MX_WIFI_MIN_TX_HEADER_SIZE  */

#endif  /* MX_WIFI_TX_BUFFER_NO_COPY */

void PushToDriver(uint32_t timeout)
{
  void *handle;
  MX_WIFIObject_t *pMxWifiObj = wifi_obj_get();

  net_if_handle_t *pnetif = (net_if_handle_t *)(pMxWifiObj->Runtime.netlink_user_args);
  if (NULL != pnetif)
  {
    handle = FIFO_POP(fifo, timeout, NULL);
    if (handle != NULL)
    {
      struct pbuf *p = (struct pbuf *) handle;
      LINK_STATS_INC(link.xmit);
      NET_PERF_TASK_TAG(4);

      (void)MX_WIFI_Network_bypass_netlink_output(pMxWifiObj,
                                                  p->payload, (int32_t)(p->len),
                                                  (int32_t)(pnetif->pdrv->extension.wifi->mode));
      (void)pbuf_free(p);
    }
  }
}

void _MX_WIFI_TranmitThread(void const *context)
{
  while (true)
  {
    PushToDriver(WAIT_FOREVER);
  }
}


/**
  * @brief                   low level data callback for the wifi driver
  * @param  pbuf             low level data pbuf pointer
  * @param  size             low level data pbuf size
  * @param  data             low level data buffer
  * @param  len              low level data length
  * @param  user_args        user arguments
  */

static void net_mx_wifi_netlink_input_callback(mx_buf_t *buffer, void *user_args)
{
  struct eth_hdr *ethernet_header;
  struct netif   *netif = ((net_if_handle_t *)user_args)->netif;
  uint16_t        ethertype;

  if ((buffer != NULL) && (buffer->len > 0U) && (buffer->len <= (uint32_t) MX_WIFI_PAYLOAD_MTU))
  {
    /* points to packet payload, which starts with an Ethernet header */
    ethernet_header = (struct eth_hdr *) buffer->payload;

    ethertype = lwip_htons(ethernet_header->type);
    switch (ethertype)
    {
      case ETHTYPE_IP:
      /* case ETHTYPE_IPV6: */
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
#endif /* MX_WIFI_NETWORK_BYPASS_MODE */

/**
  * @brief                   wifi driver init
  * @param  pnetif           net interface pointer
  * @return int32_t          0 is success, otherwise failed
  */
int32_t mx_wifi_driver(net_if_handle_t *pnetif)
{
  /* init lwip library here if not already done by another network interface */
#if (MX_WIFI_NETWORK_BYPASS_MODE == 1)
  net_ip_init();
#endif /* MX_WIFI_NETWORK_BYPASS_MODE */

  return mx_wifi_if_init(pnetif);
}

/**
  * @brief                   mxchip wifi driver interface init
  * @param  pnetif           net interface pointer
  * @return int32_t          0 is success, otherwise failed
  */
static int32_t mx_wifi_if_init(net_if_handle_t *pnetif)
{
  int32_t ret;
  void *ptmp;

  /* create netif and set mxchip wifi functions */
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


    /* wifi function */
    ptmp = NET_MALLOC(sizeof(net_if_wifi_class_extension_t));
    if (NULL == ptmp)
    {
      NET_DBG_ERROR("can't allocate memory for mx_wifi_driver class\n");
      NET_FREE(p);
      ret = NET_ERROR_NO_MEMORY;
    }
    else
    {
      p->extension.wifi = (net_if_wifi_class_extension_t *)ptmp;
      /* dhcp mode */
      pnetif->dhcp_mode = true;
      pnetif->pdrv = p;
      /* scan function */
      p->extension.wifi->scan = mx_wifi_scan;
      p->extension.wifi->get_scan_results = mx_wifi_get_scan_result;
      p->extension.wifi->mode = NET_WIFI_MODE_STA;

      ret = hw_start(pnetif);
    }
  }
  else
  {
    NET_DBG_ERROR("can't allocate memory for mx_wifi_driver class\n");
    ret = NET_ERROR_NO_MEMORY;
  }

  return ret;
}

/**
  * @brief                   mxchip wifi driver interface deinit
  * @param  pnetif           net interface pointer
  * @return int32_t          0 is success, otherwise failed
  */
static int32_t mx_wifi_if_deinit(net_if_handle_t *pnetif)
{
  int32_t ret = NET_OK;
  MX_WIFIObject_t  *pMxWifiObj = wifi_obj_get();

#if (MX_WIFI_NETWORK_BYPASS_MODE == 1)
  (void)MX_WIFI_Network_bypass_mode_set(pMxWifiObj, 0, NULL, NULL);
#endif /* MX_WIFI_NETWORK_BYPASS_MODE */

  if (MX_WIFI_STATUS_OK != MX_WIFI_DeInit(pMxWifiObj))
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
  * @brief                   mxchip wifi driver start
  * @param  pnetif           net interface pointer
  * @return int32_t          0 is success, otherwise failed
  */
static int32_t mx_wifi_if_start(net_if_handle_t *pnetif)
{
  int32_t ret;
  if (pnetif->pdrv->extension.wifi->mode == NET_WIFI_MODE_STA)
  {
    ret =  mx_wifi_if_start_station(pnetif);
  }
  else
  {
    ret =  mx_wifi_if_start_softap(pnetif);
  }
  return ret;
}


static int32_t hw_start(net_if_handle_t *pnetif)
{
  int32_t ret;

  MX_WIFIObject_t *pMxWifiObj;

  if (mxwifi_probe(&pnetif->pdrv->context) == NET_OK)
  {
    DEBUG_LOG("MX_WIFI IO [OK]\r\n");
    pMxWifiObj = wifi_obj_get();

    if (pMxWifiObj->Runtime.interfaces == 0U)
    {
      /* wifi module hardware reboot */
      DEBUG_LOG("MX_WIFI REBOOT(HW) ...\r\n");
      ret = MX_WIFI_HardResetModule(pMxWifiObj);
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
      /* wait for mxchip wifi reboot */

      /* Init the WiFi module */
      if (MX_WIFI_STATUS_OK != MX_WIFI_Init(pMxWifiObj))
      {
        ret = NET_ERROR_INTERFACE_FAILURE;
      }
      else
      {
        DEBUG_LOG("MX_WIFI_Init [OK]\r\n");
        /* Retrieve the WiFi module information */
        (void)strncpy(pnetif->DeviceName, (char_t *)pMxWifiObj->SysInfo.Product_Name,
                      MIN(strlen((char_t *)pMxWifiObj->SysInfo.Product_Name) + 1, (uint32_t) NET_DEVICE_NAME_LEN));
        (void)strncpy(pnetif->DeviceID, (char_t *)pMxWifiObj->SysInfo.Product_ID,
                      MIN(strlen((char_t *)pMxWifiObj->SysInfo.Product_ID) + 1, (uint32_t) NET_DEVICE_ID_LEN));
        (void)strncpy(pnetif->DeviceVer, (char_t *)pMxWifiObj->SysInfo.FW_Rev,
                      MIN(strlen((char_t *)pMxWifiObj->SysInfo.FW_Rev) + 1, (uint32_t) NET_DEVICE_VER_LEN));

        (void)MX_WIFI_GetMACAddress(pMxWifiObj, pnetif->macaddr.mac);

        /* set bypass mode */
#if (MX_WIFI_NETWORK_BYPASS_MODE == 1)
        if (MX_WIFI_STATUS_OK != MX_WIFI_Network_bypass_mode_set(wifi_obj_get(), 1,
                                                                 net_mx_wifi_netlink_input_callback, pnetif))
        {
          NET_DBG_ERROR("*** set mx_wifi module bypass mode failed !\n");
          ret = NET_ERROR_MODULE_INITIALIZATION;
        }
        else
#endif /* MX_WIFI_NETWORK_BYPASS_MODE */
        {
          ret = NET_OK;
        }
      }
    }
  }
  else
  {
    ret = NET_ERROR_DEVICE_ERROR;
  }
  return ret;
}

/**
  * @brief                   mxchip wifi driver interface stop
  * @param  pnetif           net interface pointer
  * @return int32_t          0 if success, otherwise failed
  */
static int32_t mx_wifi_if_stop(net_if_handle_t *pnetif)
{
  int32_t ret;
  MX_WIFIObject_t *pMxWifiObj = wifi_obj_get();
  if (pnetif->pdrv->extension.wifi->mode == NET_WIFI_MODE_STA)
  {
    if (((int32_t)NET_WIFI_SM_WPS_OPEN == pnetif->pdrv->extension.wifi->credentials->security_mode) || \
        ((int32_t)NET_WIFI_SM_WPS_SECURE == pnetif->pdrv->extension.wifi->credentials->security_mode))
    {
      (void)MX_WIFI_WPS_Stop(pMxWifiObj);
    }
    (void)MX_WIFI_Disconnect(pMxWifiObj);
  }
  else
  {
    (void)MX_WIFI_StopAP(pMxWifiObj);
  }

  (void) net_state_manage_event(pnetif, NET_EVENT_INTERFACE_INITIALIZED);
  ret = NET_OK;

  return ret;
}

/**
  * @brief                   mxchip wifi interface data yield
  * @param  pnetif           net interface
  * @param  timeout          timeout in ms
  * @return int32_t          0 if success, otherwise failed
  */
static int32_t mx_wifi_if_yield(net_if_handle_t *pnetif, uint32_t timeout)
{
  int32_t ret;
  MX_WIFIObject_t *pMxWifiObj = wifi_obj_get();

  (void)pnetif;
  ret = MX_WIFI_IO_YIELD(pMxWifiObj, timeout);

  return ret;
}

/**
  * @brief                   mxchip wifi status change callback
  * @param  cate             status cate
  * @param  status           status
  * @param  arg              user arguments
  */
static void mx_wifi_status_changed(uint8_t cate, uint8_t status, void *arg)
{
  net_if_handle_t *pnetif;
  net_state_t net_state;
  MX_WIFIObject_t *pMxWifiObj = wifi_obj_get();

  (void)memcpy((void *) &pnetif, (void *) &arg, sizeof(pnetif));


  (void) net_if_getState(pnetif, &net_state);

  if ((uint8_t)MC_STATION == cate)
  {
    switch (status)
    {
      case MWIFI_EVENT_STA_DOWN:
        DEBUG_LOG("MWIFI_EVENT_STA_DOWN\n");

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
        break;

      case MWIFI_EVENT_STA_UP:
        DEBUG_LOG("MWIFI_EVENT_STA_UP\n");
#if (MX_WIFI_NETWORK_BYPASS_MODE == 1)
        if (NET_STATE_STARTING == net_state)
        {
          (void)net_state_manage_event(pnetif, NET_EVENT_INTERFACE_READY);
        }
#else
        (void)net_state_manage_event(pnetif, NET_EVENT_INTERFACE_READY);
#endif /* MX_WIFI_NETWORK_BYPASS_MODE */
        break;

      case MWIFI_EVENT_STA_GOT_IP:
        DEBUG_LOG("MWIFI_EVENT_STA_GOT_IP\n");

#ifdef NET_USE_LWIP_DEFINITIONS
        pnetif->ipaddr.type = (u8_t)IPADDR_TYPE_V4;
        pnetif->ipaddr.u_addr.ip4.addr = NET_ARTON(pMxWifiObj->NetSettings.IP_Addr);
        pnetif->netmask.type = (u8_t)IPADDR_TYPE_V4;
        pnetif->netmask.u_addr.ip4.addr = NET_ARTON(pMxWifiObj->NetSettings.IP_Mask);
        pnetif->gateway.type = (u8_t)IPADDR_TYPE_V4;
        pnetif->gateway.u_addr.ip4.addr = NET_ARTON(pMxWifiObj->NetSettings.Gateway_Addr);
#if NET_USE_IPV6
        pnetif->ipaddr6.type = (u8_t)IPADDR_TYPE_V6;
        (void) memcpy(pnetif->ipaddr6.u_addr.ip6.addr, pMxWifiObj->NetSettings.IP6_Addr[0], 16);
        pnetif->netmask6.type = (u8_t)IPADDR_TYPE_V6;
        (void) memcpy(pnetif->netmask6.u_addr.ip6.addr, pMxWifiObj->NetSettings.IP6_Mask, 16);
        pnetif->gateway6.type = (u8_t)IPADDR_TYPE_V6;
        (void) memcpy(pnetif->gateway6.u_addr.ip6.addr, pMxWifiObj->NetSettings.Gateway6_Addr, 16);
#endif /* NET_USE_IPV6 */
#else /* IPv4 only */
        pnetif->ipaddr.addr = NET_ARTON(pMxWifiObj->NetSettings.IP_Addr);
        pnetif->netmask.addr = NET_ARTON(pMxWifiObj->NetSettings.IP_Mask);
        pnetif->gateway.addr = NET_ARTON(pMxWifiObj->NetSettings.Gateway_Addr);
#endif /* NET_USE_LWIP_DEFINITIONS */
#if (MX_WIFI_NETWORK_BYPASS_MODE == 0)
        /* in current driver implementation , its not possible to control the state of the driver
        between the Ready state (Wifi AP/STA running) and the connected state (IPADDR available).
        This correspond to a single state in Wifi Driver (no command to control the IP stack).
        Current Workaround is to consider that network library Ready state as the one from
        the driver. So wait for IPaddr event from driver to send the Ready event
            to grant the transition to the Ready state. Connect and Disconnect state are implemented
        as transparent state, no action on Wifi driver */
        (void)net_state_manage_event(pnetif, NET_EVENT_IPADDR);
#else
        (void) net_state_manage_event(pnetif, NET_EVENT_IPADDR);
#endif  /* MX_WIFI_NETWORK_BYPASS_MODE */
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
        DEBUG_LOG("MWIFI_EVENT_AP_DOWN\n");
        if (NET_STATE_CONNECTED == net_state)
        {
          (void)net_state_manage_event(pnetif, NET_EVENT_LINK_DOWN);
        }
        else
        {
          (void)net_state_manage_event(pnetif, NET_EVENT_INTERFACE_READY);
        }
        break;

      case MWIFI_EVENT_AP_UP:
        DEBUG_LOG("MWIFI_EVENT_AP_UP\n");
#ifdef NET_USE_LWIP_DEFINITIONS
        pnetif->ipaddr = pnetif->static_ipaddr;
        pnetif->gateway = pnetif->static_gateway;
        pnetif->netmask = pnetif->static_netmask;
#else
        pnetif->ipaddr.addr = pnetif->static_ipaddr.addr;
        pnetif->gateway.addr = pnetif->static_gateway.addr;
        pnetif->netmask.addr = pnetif->static_netmask.addr;
#endif /* NET_USE_LWIP_DEFINITIONS */
        (void) net_state_manage_event(pnetif, NET_EVENT_INTERFACE_READY);
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
  * @brief                   mxchip wifi connect to AP
  * @param  pnetif           net interface
  * @return int32_t          0 if success, otherwise failed
  */
static int32_t mx_wifi_if_start_station(net_if_handle_t *pnetif)
{
  int32_t ret;
  MX_WIFI_SecurityType_t secure_type;
  MX_WIFIObject_t *pMxWifiObj = wifi_obj_get();
  const net_wifi_credentials_t *credentials =  pnetif->pdrv->extension.wifi->credentials;

  if (false == pnetif->dhcp_mode)
  {
    pMxWifiObj->NetSettings.DHCP_IsEnabled = 0;
    /* ipv6 */
#if NET_USE_IPV6
    (void)memcpy(pMxWifiObj->NetSettings.IP6_Addr[0], &(pnetif->static_ipaddr.u_addr.ip6), 16);
    (void)memcpy(pMxWifiObj->NetSettings.IP6_Mask, &(pnetif->static_netmask.u_addr.ip6), 16);
    (void)memcpy(pMxWifiObj->NetSettings.Gateway6_Addr, &(pnetif->static_gateway.u_addr.ip6), 16);
#else
    /* ipv4 */
    (void)memcpy(pMxWifiObj->NetSettings.IP_Addr, &(pnetif->static_ipaddr), 4);
    (void)memcpy(pMxWifiObj->NetSettings.IP_Mask, &(pnetif->static_netmask), 4);
    (void)memcpy(pMxWifiObj->NetSettings.Gateway_Addr, &(pnetif->static_gateway), 4);
#endif /* NET_USE_IPV6 */
  }
  else
  {
    pMxWifiObj->NetSettings.DHCP_IsEnabled = 1;
  }

  (void)MX_WIFI_RegisterStatusCallback(pMxWifiObj, mx_wifi_status_changed, pnetif);

  if (((int32_t)NET_WIFI_SM_WPS_OPEN == credentials->security_mode) || \
      ((int32_t)NET_WIFI_SM_WPS_SECURE == credentials->security_mode))
  {
    ret = MX_WIFI_WPS_Connect(pMxWifiObj);
  }
  else
  {
    /* NOTE: secure type is auto for mxchip wifi */
    secure_type = MX_WIFI_SEC_AUTO;
    ret = MX_WIFI_Connect(pMxWifiObj, credentials->ssid, credentials->psk, secure_type);
  }
  return ret;
}

/* ip address helper */
#define BYTE(A,n) ((A)>>(8u*(n))) & 0xffu
#define BYTE3(A)  BYTE((A),3u)
#define BYTE2(A)  BYTE((A),2u)
#define BYTE1(A)  BYTE((A),1u)
#define BYTE0(A)  BYTE((A),0u)
#define ADDR(a) BYTE0(a),BYTE1(a),BYTE2(a),BYTE3(a)

/**
  * @brief                   mxchip wifi start AP mode
  * @param  pnetif           net interface
  * @return int32_t          0 if success, otherwise failed
  */
static int32_t mx_wifi_if_start_softap(net_if_handle_t *pnetif)
{
#if NET_USE_IPV6
  /* this is independent of what is supported by IP stack IPv6 or Ipv4 */
  return NET_ERROR_UNSUPPORTED;
#else
  int32_t ret = NET_ERROR_GENERIC;

  MX_WIFI_APSettings_t ap_cfg;
  MX_WIFIObject_t *pMxWifiObj = wifi_obj_get();
  const net_wifi_credentials_t *credentials =  pnetif->pdrv->extension.wifi->credentials;

  (void) memset(&ap_cfg, 0, sizeof(ap_cfg));

  if (credentials != NULL)
  {
    (void) strcpy(ap_cfg.SSID, credentials->ssid);
    (void) strcpy(ap_cfg.pswd, credentials->psk);
  }

  ap_cfg.channel = pnetif->pdrv->extension.wifi->access_channel;

#ifdef NET_USE_LWIP_DEFINITIONS
  (void) sprintf(ap_cfg.ip.localip, "%ld.%ld.%ld.%ld", ADDR(pnetif->static_ipaddr.u_addr.ip4.addr));
  (void) sprintf(ap_cfg.ip.gateway, "%ld.%ld.%ld.%ld", ADDR(pnetif->static_gateway.u_addr.ip4.addr));
  (void) sprintf(ap_cfg.ip.netmask, "%ld.%ld.%ld.%ld", ADDR(pnetif->static_netmask.u_addr.ip4.addr));
  (void) sprintf(ap_cfg.ip.dnserver, "%ld.%ld.%ld.%ld", ADDR(pnetif->static_dnserver.u_addr.ip4.addr));
#else
  (void) sprintf(ap_cfg.ip.localip, "%"PRIu32".%"PRIu32".%"PRIu32".%"PRIu32"", ADDR(pnetif->static_ipaddr.addr));
  (void) sprintf(ap_cfg.ip.gateway, "%"PRIu32".%"PRIu32".%"PRIu32".%"PRIu32"", ADDR(pnetif->static_gateway.addr));
  (void) sprintf(ap_cfg.ip.netmask, "%"PRIu32".%"PRIu32".%"PRIu32".%"PRIu32"", ADDR(pnetif->static_netmask.addr));
  (void) sprintf(ap_cfg.ip.dnserver, "%"PRIu32".%"PRIu32".%"PRIu32".%"PRIu32"", ADDR(pnetif->static_dnserver.addr));
#endif /* NET_USE_LWIP_DEFINITIONS */

  (void)MX_WIFI_RegisterStatusCallback(pMxWifiObj, mx_wifi_status_changed, pnetif);

  if (MX_WIFI_STATUS_OK == MX_WIFI_StartAP(pMxWifiObj, &ap_cfg))
  {
    ret = NET_OK;
  }

  return ret;
#endif /* NET_USE_IPV6 */
}

/**
  * @brief                   mxchip wifi disconnect from AP
  * @param  pnetif           net interface
  * @return int32_t          0 if success, otherwise failed
  */
static int32_t mx_wifi_if_disconnect(net_if_handle_t *pnetif)
{
  int32_t ret;

#if (MX_WIFI_NETWORK_BYPASS_MODE == 1)
  MX_WIFIObject_t *pMxWifiObj = wifi_obj_get();
  ret = net_ip_disconnect(pnetif);
  if (ret == NET_OK)
  {
    ret =  net_ip_remove_if(pnetif, NULL);
    if (ret == NET_OK)
    {
      (void)MX_WIFI_Network_bypass_mode_set(pMxWifiObj, 0, NULL, NULL);
      (void)net_state_manage_event(pnetif, NET_EVENT_INTERFACE_READY);
    }
  }
#else
  ret = NET_OK;
  (void) net_state_manage_event(pnetif, NET_EVENT_INTERFACE_READY);
#endif /* MX_WIFI_NETWORK_BYPASS_MODE */
  return ret;
}

/**
  * @brief                   mxchip wifi start IP services
  * @param  pnetif           net interface
  * @return int32_t          0 if success, otherwise failed
  */
static int32_t mx_wifi_if_connect(net_if_handle_t *pnetif)
{
  int32_t ret;

#if (MX_WIFI_NETWORK_BYPASS_MODE == 1)

  /* netif add for bypass mode */

  ret = net_ip_add_if(pnetif, low_level_init, NET_ETHERNET_FLAG_DEFAULT_IF);
  if (ret == NET_OK)
  {
    netif_set_link_up(pnetif->netif);

    ret =  net_ip_connect(pnetif);
    if (ret != NET_OK)
    {
      NET_DBG_ERROR("*** net_ip_connect failed!\n");
      ret = NET_ERROR_NO_CONNECTION;
    }
  }
  else
  {
    NET_DBG_ERROR("can't add interface (netif)\n");
  }
#else
  mwifi_if_t wifi_if;
  ret = NET_OK;
  if (pnetif->pdrv->extension.wifi->mode == NET_WIFI_MODE_AP)
  {
    wifi_if = MC_SOFTAP;
  }
  else
  {
    wifi_if = MC_STATION;
  }
#if NET_USE_IPV6
  uint8_t ip[16];
  if (MX_WIFI_STATUS_OK == MX_WIFI_GetIP6Address(wifi_obj_get(), &(ip[0]), 0, wifi_if))
  {
    pnetif->ipaddr6.type = (u8_t)IPADDR_TYPE_V6;
    (void) memcpy(pnetif->ipaddr6.u_addr.ip6.addr, wifi_obj_get()->NetSettings.IP6_Addr[0], 16);
    pnetif->netmask6.type = (u8_t)IPADDR_TYPE_V6;
    (void) memcpy(pnetif->netmask6.u_addr.ip6.addr, wifi_obj_get()->NetSettings.IP6_Mask, 16);
    pnetif->gateway6.type = (u8_t)IPADDR_TYPE_V6;
    (void) memcpy(pnetif->gateway6.u_addr.ip6.addr, wifi_obj_get()->NetSettings.Gateway6_Addr, 16);

    (void) net_state_manage_event(pnetif, NET_EVENT_IPADDR);
  }
  else
  {
    NET_DBG_ERROR("can't get ipv6 address!\n");
    ret = NET_ERROR_NO_ADDRESS;
  }
#else
  uint8_t ip[4];
  int32_t retry = 3;
  while (retry > 0)
  {
    retry--;
    if (MX_WIFI_STATUS_OK == MX_WIFI_GetIPAddress(wifi_obj_get(), &(ip[0]), wifi_if))
    {
      pnetif->ipaddr.addr = NET_ARTON(wifi_obj_get()->NetSettings.IP_Addr);
      pnetif->netmask.addr = NET_ARTON(wifi_obj_get()->NetSettings.IP_Mask);
      pnetif->gateway.addr = NET_ARTON(wifi_obj_get()->NetSettings.Gateway_Addr);
      (void) net_state_manage_event(pnetif, NET_EVENT_IPADDR);
      ret = NET_OK;
      break;
    }
    else
    {
      ret = NET_ERROR_NO_ADDRESS;
#ifdef NET_USE_RTOS
      osDelay(1000);
#else
      HAL_Delay(1000);
#endif /* NET_USE_RTOS */
    }
  }
#endif  /* NET_USE_IPV6 */
#endif /* MX_WIFI_NETWORK_BYPASS_MODE */

  return ret;
}

/**
  * @brief                   mxchip wifi scan function
  * @param  pnetif           net interface
  * @param  mode             scan mode
  * @param  ssid             ssid to scan in active mode
  * @return int32_t          0 if success, otherwise failed
  */
static int32_t mx_wifi_scan(net_if_handle_t *pnetif, net_wifi_scan_mode_t mode, char *ssid)
{
  int32_t ret;
  MX_WIFIObject_t *pMxWifiObj = wifi_obj_get();
  uint32_t len = 0u;

  (void) pnetif;

  if (ssid != NULL)
  {
    len = strlen(ssid);
  }
  ret = MX_WIFI_Scan(pMxWifiObj, (uint8_t)mode, ssid, (int32_t) len);
  return ret;
}

/**
  * @brief                       mxchip wifi get scan result
  * @param  pnetif               net interface
  * @param  scan_bss_array       bss array buffer
  * @param  scan_bss_array_size  bss array size
  * @return int32_t              number of the bss got
  */
static int32_t mx_wifi_get_scan_result(net_if_handle_t *pnetif, net_wifi_scan_results_t *scan_bss_array,
                                       uint8_t scan_bss_array_size)
{
  int32_t ret;
  MX_WIFIObject_t *pMxWifiObj = wifi_obj_get();
  net_wifi_scan_results_t *scan_bss = scan_bss_array;

  (void)pnetif;
  /* mxchip wifi security mode */
  static  uint32_t mxsec[] =
  {
    NET_WIFI_SM_OPEN,
    NET_WIFI_SM_WEP_PSK,        /**< Wired Equivalent Privacy. WEP security. */
    NET_WIFI_SM_WPA_TKIP_PSK,   /**< WPA /w TKIP */
    NET_WIFI_SM_WPA_AES_PSK,    /**< WPA /w AES */
    NET_WIFI_SM_WPA2_TKIP_PSK,  /**< WPA2 /w TKIP */
    NET_WIFI_SM_WPA2_AES_PSK,   /**< WPA2 /w AES */
    NET_WIFI_SM_WPA2_MIXED_PSK  /**< WPA2 /w AES or TKIP */
  };

  if ((NULL == scan_bss_array) || (0u == scan_bss_array_size))
  {
    ret = NET_ERROR_PARAMETER;
  }
  else
  {
    /* create buff for mc_wifi results */
    void *ptmp = NET_MALLOC(sizeof(mwifi_ap_info_t) * scan_bss_array_size);

    if (NULL == ptmp)
    {
      ret = NET_ERROR_NO_MEMORY;
    }
    else
    {
      mwifi_ap_info_t *ap_info;
      mwifi_ap_info_t *const ap_list_head = (mwifi_ap_info_t *)ptmp;
      ap_info = ap_list_head;
      (void) memset(ap_list_head, 0, sizeof(mwifi_ap_info_t) * scan_bss_array_size);

      /* get real mc_wifi scan results data */
      const uint8_t number = (uint8_t) MX_WIFI_Get_scan_result(pMxWifiObj, (uint8_t *) ap_info, scan_bss_array_size);

      /* transfer to ST bss info */
      for (uint32_t i = 0U; i < number; i++)
      {
        (void) memset(scan_bss, 0, sizeof(net_wifi_scan_bss_t));
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
      ret = (int32_t) number;
      NET_FREE((void *) ap_list_head);
    }
  }
  return ret;
}

#if (MX_WIFI_NETWORK_BYPASS_MODE == 0)
/**
  * @brief                   mxchip wifi socket create
  * @param  domain           socket domain
  * @param  type             socket type
  * @param  protocol         socket protocol
  * @return int32_t          socket fd, if failed return error code(<0)
  */
static int32_t mx_wifi_socket(int32_t domain, int32_t type, int32_t protocol)
{
  int32_t ret;
  MX_WIFIObject_t  *pMxWifiObj = wifi_obj_get();

  ret = MX_WIFI_Socket_create(pMxWifiObj, domain, type, protocol);
  return ret;
}

/**
  * @brief                   mxchip wifi set socket option
  * @param  sock             socket fd
  * @param  level            socket level
  * @param  optname          socket option name
  * @param  optvalue         socket option value to set
  * @param  optlen           socket option value length
  * @return int32_t          0 if success, if failed, return error code(< 0)
  */
static int32_t mx_wifi_setsockopt(int32_t sock, int32_t level, int32_t optname, const void *optvalue, uint32_t optlen)
{
  int32_t ret;
  MX_WIFIObject_t  *pMxWifiObj = wifi_obj_get();

  ret = MX_WIFI_Socket_setsockopt(pMxWifiObj, sock, level, optname, optvalue, (int32_t)optlen);
  return ret;
}

/**
  * @brief                   mxchip wifi get socket option
  * @param  sock             socket fd
  * @param  level            socket level
  * @param  optname          socket option name
  * @param  optvalue         socket option value
  * @param  optlen           socket option value length
  * @return int32_t          0 if success, if failed return error code(<0)
  */
static int32_t mx_wifi_getsockopt(int32_t sock, int32_t level, int32_t optname, void *optvalue, uint32_t *optlen)
{
  int32_t ret;
  MX_WIFIObject_t *pMxWifiObj = wifi_obj_get();

  ret = MX_WIFI_Socket_getsockopt(pMxWifiObj, sock, level, optname, optvalue, optlen);
  return ret;
}

/**
  * @brief                   mxchip wifi bind socket
  * @param  sock             socket fd
  * @param  addr             address to bind
  * @param  addrlen          address length
  * @return int32_t          0 if success, if failed, return error code(<0)
  */
static int32_t mx_wifi_bind(int32_t sock, const net_sockaddr_t *addr, uint32_t addrlen)
{
  int32_t ret = MX_WIFI_STATUS_ERROR;
  MX_WIFIObject_t *pMxWifiObj = wifi_obj_get();

  if ((NULL != addr) && (addrlen == sizeof(struct sockaddr)))
  {
#if NET_USE_IPV6
    ret = MX_WIFI_Socket_bind(pMxWifiObj, sock, (const struct sockaddr *)addr, (int32_t)addrlen);
#else
    struct sockaddr mx_addr;
    (void)memcpy(&mx_addr, addr, sizeof(mx_addr));
    ret = MX_WIFI_Socket_bind(pMxWifiObj, sock, &mx_addr, (int32_t)addrlen);
#endif /* NET_USE_IPV6 */
  }

  return ret;
}

/**
  * @brief                   mxchip wifi socket listen
  * @param  sock             socket fd
  * @param  backlog          max socket to accept
  * @return int32_t          0 if success, if failed return error code(<0)
  */
static int32_t mx_wifi_listen(int32_t sock, int32_t backlog)
{
  int32_t ret;
  MX_WIFIObject_t *pMxWifiObj = wifi_obj_get();

  ret = MX_WIFI_Socket_listen(pMxWifiObj, sock, backlog);
  return ret;
}

/**
  * @brief                   mxchip wifi socket accept
  * @param  sock             server socket fd
  * @param  addr             address buffer to accept new socket
  * @param  addrlen          address buffer length
  * @return int32_t          accepted socket fd, if failed return error code(<0)
  */
static int32_t mx_wifi_accept(int32_t sock, net_sockaddr_t *addr, uint32_t *addrlen)
{
  int32_t ret;
  MX_WIFIObject_t *pMxWifiObj = wifi_obj_get();

#if NET_USE_IPV6
  ret = MX_WIFI_Socket_accept(pMxWifiObj, sock, (struct sockaddr *)addr, addrlen);
#else
  struct sockaddr mx_addr;
  (void)memcpy(&mx_addr, addr, sizeof(mx_addr));
  ret = MX_WIFI_Socket_accept(pMxWifiObj, sock, &mx_addr, addrlen);
#endif /* NET_USE_IPV6 */

  return ret;
}

/**
  * @brief                   mxchip wifi socket connect
  * @param  sock             socket fd
  * @param  addr             address to connect
  * @param  addrlen          address length
  * @return int32_t          0 if success, if failed return error code(<0)
  */
static int32_t mx_wifi_connect(int32_t sock, const net_sockaddr_t *addr, uint32_t addrlen)
{
  int32_t ret;
  MX_WIFIObject_t *pMxWifiObj = wifi_obj_get();

#if NET_USE_IPV6
  ret = MX_WIFI_Socket_connect(pMxWifiObj, sock, (const struct sockaddr *)addr, (int32_t)addrlen);
#else
  struct sockaddr mx_addr;
  (void)memcpy(&mx_addr, addr, sizeof(mx_addr));
  ret = MX_WIFI_Socket_connect(pMxWifiObj, sock, &mx_addr, (int32_t)addrlen);
#endif /* NET_USE_IPV6 */

  return ret;
}

/**
  * @brief                   mxchip wifi socket shutdown
  * @param  sock             socket fd
  * @param  mode             shutdown mode(0,1,2)
  * @return int32_t          0 if success, if failed return error code(<0)
  */
static int32_t mx_wifi_shutdown(int32_t sock, int32_t mode)
{
  int32_t ret;
  MX_WIFIObject_t *pMxWifiObj = wifi_obj_get();

  ret = MX_WIFI_Socket_shutdown(pMxWifiObj, sock, mode);
  return ret;
}

/**
  * @brief                   mxchip wifi socket close
  * @param  sock             socket fd
  * @param  isaclone         not supported, just compliant for ST connect lib
  * @return int32_t          0 if success, if failed return error code(<0)
  */
static int32_t mx_wifi_close(int32_t sock, bool isaclone)
{
  int32_t ret;
  MX_WIFIObject_t *pMxWifiObj = wifi_obj_get();

  (void)isaclone;
  ret = MX_WIFI_Socket_close(pMxWifiObj, sock);
  return ret;
}

/**
  * @brief                   mxchip wifi socket send
  * @param  sock             socket fd
  * @param  buf              data buffer to send
  * @param  len              data length
  * @param  flags            socket flag
  * @return int32_t          bytes sent, if failed return error code(<0)
  */
static int32_t mx_wifi_send(int32_t sock, uint8_t *buf, int32_t len, int32_t flags)
{
  int32_t ret;
  MX_WIFIObject_t *pMxWifiObj = wifi_obj_get();

  ret = MX_WIFI_Socket_send(pMxWifiObj, sock, buf, len, flags);
  return ret;
}

/**
  * @brief                   mxchip wifi socket recv
  * @param  sock             socket fd
  * @param  buf              buffer to recv data
  * @param  len              buffer size
  * @param  flags            socket flag
  * @return int32_t          bytes received, if failed return error code(<0)
  */
static int32_t mx_wifi_recv(int32_t sock, uint8_t *buf, int32_t len, int32_t flags)
{
  int32_t ret;
  MX_WIFIObject_t *pMxWifiObj = wifi_obj_get();

  ret = MX_WIFI_Socket_recv(pMxWifiObj, sock, buf, len, flags);
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
  * @brief                   mxchip wifi socket sendto
  * @param  sock             socket fd
  * @param  buf              data buffer to send
  * @param  len              data length
  * @param  flags            socket flag
  * @param  to               address to send to
  * @param  tolen            size of address to send to
  * @return int32_t          bytes sent. If failed return error code(<0)
  */
static int32_t mx_wifi_sendto(int32_t sock, uint8_t *buf, int32_t len, int32_t flags, net_sockaddr_t *to,
                              uint32_t tolen)
{
  int32_t ret = MX_WIFI_STATUS_ERROR;
  MX_WIFIObject_t *pMxWifiObj = wifi_obj_get();

  if ((NULL != to) && (tolen == sizeof(struct sockaddr)))
  {
#if NET_USE_IPV6
    ret = MX_WIFI_Socket_sendto(pMxWifiObj, sock, buf, len, flags, (struct sockaddr *)to, (int32_t)tolen);
#else
    struct sockaddr mx_addr;
    (void)memcpy(&mx_addr, to, sizeof(mx_addr));
    ret = MX_WIFI_Socket_sendto(pMxWifiObj, sock, buf, len, flags, &mx_addr, (int32_t)tolen);
#endif /* NET_USE_IPV6 */
  }
  return ret;
}

/**
  * @brief                   mxchip wifi socket recvfrom
  * @param  sock             socket fd
  * @param  buf              buffer to recv data
  * @param  len              buffer size
  * @param  flags            socket flag
  * @param  from             address of the data source
  * @param  fromlen          size of address
  * @return int32_t          bytes received, if failed, return error code(<0)
  */
static int32_t mx_wifi_recvfrom(int32_t sock, uint8_t *buf, int32_t len, int32_t flags, net_sockaddr_t *from,
                                uint32_t *fromlen)
{
  int32_t ret = MX_WIFI_STATUS_ERROR;
  MX_WIFIObject_t *pMxWifiObj = wifi_obj_get();

  if ((NULL != from) && (*fromlen == sizeof(struct sockaddr)))
  {
#if NET_USE_IPV6
    ret = MX_WIFI_Socket_recvfrom(pMxWifiObj, sock, buf, len, flags, (struct sockaddr *)from, fromlen);
#else
    struct sockaddr mx_addr;
    (void)memcpy(&mx_addr, from, sizeof(mx_addr));
    ret = MX_WIFI_Socket_recvfrom(pMxWifiObj, sock, buf, len, flags, &mx_addr, fromlen);
#endif /* NET_USE_IPV6 */
  }
  return ret;
}

/**
  * @brief                   mxchip wifi gethostbyname
  * @param  pnetif           net interface
  * @param  addr             address of the host
  * @param  name             host name
  * @return int32_t          0 if success, otherwise failed
  */
static int32_t mx_wifi_gethostbyname(net_if_handle_t *pnetif, net_sockaddr_t *addr, char_t *name)
{
  int32_t ret;
  MX_WIFIObject_t *pMxWifiObj = wifi_obj_get();

#if NET_USE_IPV6
  struct addrinfo hints;
  struct mc_addrinfo res;
  if (AF_INET6 == addr->sa_family)
  {
    (void)memset(&hints, 0, sizeof(struct addrinfo));
    (void)memset(&res, 0, sizeof(struct mc_addrinfo));
    hints.ai_flags = 0;
    hints.ai_family = AF_INET6;
    hints.ai_socktype = IPPROTO_TCP;
    hints.ai_protocol = SOCK_STREAM;
    ret = MX_WIFI_Socket_getaddrinfo(pMxWifiObj, name, NULL, &hints, &res);
    if (MX_WIFI_STATUS_OK == ret)
    {
      (void)memcpy(addr, &(res.ai_addr), res.ai_addrlen);
    }
  }
  else
#endif /* NET_USE_IPV6 */
  {
    /* IPv4 */
    struct sockaddr mx_addr;
    (void)pnetif;
    ret = MX_WIFI_Socket_gethostbyname(pMxWifiObj, &mx_addr, (char_t *)name);
    if (MX_WIFI_STATUS_OK == ret)
    {
      (void)memcpy(addr, &mx_addr, sizeof(mx_addr));
    }
  }
  return ret;
}





/**
  * @brief                   mxchip wifi ping function
  * @param  pnetif           net interface
  * @param  addr             address to ping
  * @param  count            request count
  * @param  delay            request delay in ms
  * @param  response         response array, size is count
  * @return int32_t          0 if success, otherwise failed
  */
static int32_t mx_wifi_ping(net_if_handle_t *pnetif, net_sockaddr_t *addr, int32_t count, int32_t delay,
                            int32_t response[])
{
  int32_t ret;
  MX_WIFIObject_t *pMxWifiObj = wifi_obj_get();
  net_ip_addr_t ip_addr;

  (void)pnetif;
#ifdef NET_USE_LWIP_DEFINITIONS
  (void)memcpy(&ip_addr, addr, sizeof(net_ip_addr_t));
#else /* IPv4 only */
  net_sockaddr_in_t addr_in;
  (void)memcpy(&addr_in, addr, sizeof(addr_in));
  ip_addr.addr = addr_in.sin_addr.s_addr;
#endif /* NET_USE_LWIP_DEFINITIONS */

  ret = MX_WIFI_Socket_ping(pMxWifiObj, (char_t *)NET_NTOA(&ip_addr), count, delay, response);
  return ret;
}

/**
  * @brief                   mxchip wifi getsockname
  * @param  sock             socket fd
  * @param  name             socket name
  * @param  namelen          socket name length
  * @return int32_t          0 if success, otherwise failed
  */
static int32_t mx_wifi_getsockname(int32_t sock, net_sockaddr_t *name, uint32_t *namelen)
{
  int32_t ret;
  MX_WIFIObject_t *pMxWifiObj = wifi_obj_get();

  /* net_sockaddr_t is alias of struct sockaddr definition from LwIp or exact redefinition. */
  struct sockaddr *mx_addr = (struct sockaddr *)(/*(void *)*/name);
  ret = MX_WIFI_Socket_getsockname(pMxWifiObj, sock, mx_addr, namelen);

  return ret;
}

/**
  * @brief                   mxchip wifi getpeername
  * @param  sock             socket fd
  * @param  name             socket name
  * @param  namelen          socket name length
  * @return int32_t          0 if success, otherwise failed
  */
static int32_t mx_wifi_getpeername(int32_t sock, net_sockaddr_t *name, uint32_t *namelen)
{
  int32_t ret;
  MX_WIFIObject_t *pMxWifiObj = wifi_obj_get();

  /* net_sockaddr_t is alias of struct sockaddr definition from LwIp or exact redefinition. */
  struct sockaddr *mx_addr = (struct sockaddr *)(/*(void *)*/name);

  ret = MX_WIFI_Socket_getpeername(pMxWifiObj, sock, mx_addr, namelen);

  return ret;
}

#endif /* MX_WIFI_NETWORK_BYPASS_MODE */
