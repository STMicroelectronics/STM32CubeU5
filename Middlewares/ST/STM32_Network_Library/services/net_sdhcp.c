/**
  ******************************************************************************
  * @file    net_sdhcp.c
  * @author  MCD Application Team
  * @brief   Basic DHCP server used when the WiFi device is Access Point
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
/**
  * This source implements a very basic DHCP server in order to allow
  * mobile phone connection when the WiFi is in Access Point.
  * The DHCP allocates 3 addresses for a limited period of time.
  */

#include "net_connect.h"
#include "net_internals.h"
#include "net_mem.h"
#include "net_errors.h"

#include "lwip/tcpip.h"
#include <stdbool.h>
#include <inttypes.h>

#define TASK_SDHCP_NAME                 "sdhcp"
#define TASK_SDHCP_PRIORITY             (osPriorityNormal)
#define TASK_SDHCP_STACK                 2048

#define MIN_IP                           15U
#define MAX_IP                           17U
#define MAX_ADDRESS_INFO                 (MAX_IP - MIN_IP)
#define SDHCP_TIMEOUT                    300U

#ifndef MIN
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif /* MIN */

#ifndef MAX
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif /* MAX */

#define DWIP0(dw) (uint8_t)(((dw)>> 0U) & 0xffU)
#define DWIP1(dw) (uint8_t)(((dw)>> 8U) & 0xffU)
#define DWIP2(dw) (uint8_t)(((dw)>>16U) & 0xffU)
#define DWIP3(dw) (uint8_t)(((dw)>>24U) & 0xffU)


#define MAX_UDP_MESSAGE_SIZE  (1000)
#define DHCP_SERVER_PORT      ((uint16_t)67)
#define DHCP_CLIENT_PORT      ((uint16_t)68)
#define BROADCAST_FLAG        (0x80U)
#define MAX_HOSTNAME_LENGTH   (256)

#define SDHCP_RES_TRUE        (1U)
#define SDHCP_RES_FALSE       (0U)

#define SDHCP_ASSERT(test)    NET_ASSERT((test) != 0U, "SDHCP")

typedef struct t_dhcp_addr
{
  uint32_t addrV4;
  uint8_t *pClientID;
  uint32_t szClientID;
} dhcp_addr_t;

typedef struct t_sdhcp_handle
{
  int32_t                 hSocket;
  uint32_t                lastOfferAddr;
  net_sockaddr_in_t       nethost;
  net_sockaddr_in_t       netmsk;
  net_sockaddr_in_t       netmin;
  net_sockaddr_in_t       netmax;
  char_t                 *pHostName;
  dhcp_addr_t             tAddressInfo[MAX_ADDRESS_INFO];
  uint32_t                szAddressInfo;
} sdhcp_handle_t;


#pragma pack(1)
typedef struct t_net_dhcp_msg
{
  uint8_t   op;
  uint8_t   htype;
  uint8_t   hlen;
  uint8_t   hops;
  uint32_t  xid;
  uint16_t  secs;
  uint16_t  flags;
  uint32_t  ciaddr;
  uint32_t  yiaddr;
  uint32_t  siaddr;
  uint32_t  giaddr;
  uint8_t   chaddr[16];
  char_t    sname[64];
  char_t    file[128];
  uint8_t   options[];
} net_dhcp_msg_t;

typedef struct t_net_dhcp_option
{
  uint8_t tMagic[4];
  uint8_t tMessageType[3];
  uint8_t tLeaseTime[6];
  uint8_t tSubnetMask[6];
  uint8_t tServerID[6];
  uint8_t tRouter[6];
  uint8_t bEND;
} net_dhcp_option_t;

typedef struct t_sdhcp_process_request
{
  uint8_t *pbClientIdentifier;
  uint32_t szClientID;
} sdhcp_process_request_t;

#pragma pack()


typedef enum t_op_values
{
  BOOTREQUEST = 1,
  BOOTREPLY   = 2
} op_values_t;

/* RFC 2132 section 9.6  */

typedef enum t_option_values
{
  PAD                = 0,
  SUBNETMASK         = 1,
  HOSTNAME           = 12,
  REQUESTEDIPADDRESS = 50,
  IPADDRESSLEASETIME = 51,
  DHCPMESSAGETYPE    = 53,
  SERVERIDENTIFIER   = 54,
  CLIENTIDENTIFIER   = 61,
  ROUTER             = 3,
  END                = 255
} option_values_t;

typedef enum t_dhcp_msg_types
{
  DHCP_TYPE_DISCOVER    = 1U,
  DHCP_TYPE_FIRST_VALUE = DHCP_TYPE_DISCOVER,
  DHCP_TYPE_OFFER       = 2U,
  DNCP_TYPE_REQUEST     = 3U,
  DHCP_TYPE_DECLINE     = 4U,
  DHCP_TYPE_ACK         = 5U,
  DHCP_TYPE_NAK         = 6U,
  DHCP_TYPE_RELEASE     = 7U,
  DHCP_TYPE_INFORM      = 8U,
  DHCP_TYPE_LAST_VALUE  = DHCP_TYPE_INFORM
} dhcp_msg_types_t;

typedef uint8_t(*sdhcp_find_cb)(dhcp_addr_t *raiui, void *pvFilterData);

static net_if_handle_t *pNetifObject = NULL;

/* DHCP magic cookie values*/
static volatile int8_t RunTask = 0;

#if (osCMSIS < 0x20000U)
static osThreadId SdhcpTask;
#else
static osThreadId_t SdhcpTask;
#endif /* osCMSIS */

static uint32_t sdhcp_swap_ip(uint32_t Addr);
static int32_t sdhcp_find_index(sdhcp_handle_t *pHandle, sdhcp_find_cb pFilter, void *pData);
static uint8_t sdhcp_addInfo(sdhcp_handle_t *pHandle, dhcp_addr_t *pInfo);
static uint8_t service_sdhcp_init(sdhcp_handle_t *pSdhcp);
static uint8_t sdhcp_find_option(uint8_t Option, uint8_t *pOptions, int32_t OptionsLen,
                                 uint8_t **pData, uint32_t *pDataLen);
static uint8_t sdhcp_get_message_type(uint8_t *pOptions, int32_t OptionsLen, dhcp_msg_types_t *pMessageType);
static uint8_t sdhcp_add_addr_filter(dhcp_addr_t *pInfo, void *const pFilter);
static uint8_t sdhcp_check_address_in_use(dhcp_addr_t *pInfo, void *pFilter);
static void sdhcp_process_request(sdhcp_handle_t *pSdhcp, void *pData, uint32_t DataLen);
static void service_sdhcp_task(void const *pParam);

/* Reverse the IP address byte order. */
static uint32_t sdhcp_swap_ip(uint32_t Addr)
{
  uint32_t dw = Addr;
  uint32_t swapped = 0;
  for (int32_t aa = 0; aa < 4; aa++)
  {
    swapped <<= 8;
    swapped |= (dw & 0xFFU);
    dw >>= 8;
  }
  return swapped;
}

/* Find a client in the list. */
static int32_t sdhcp_find_index(sdhcp_handle_t *pHandle, sdhcp_find_cb pFilter, void *pData)
{
  int32_t ret = -1;

  SDHCP_ASSERT((0U != pFilter) && (NULL != pData));

  for (int32_t i = 0; i < (int32_t) pHandle->szAddressInfo; i++)
  {
    if (pFilter(&pHandle->tAddressInfo[i], pData) != 0U)
    {
      ret = i;
      break;
    }
  }
  return ret;
}


/* Add a client to the list. */
static uint8_t sdhcp_addInfo(sdhcp_handle_t *pHandle, dhcp_addr_t *pInfo)
{
  uint8_t ret = SDHCP_RES_FALSE;

  SDHCP_ASSERT(NULL != pHandle);
  SDHCP_ASSERT(NULL != pInfo);

  if (pHandle->szAddressInfo <= MAX_ADDRESS_INFO)
  {
    pHandle->tAddressInfo[pHandle->szAddressInfo] = *pInfo;
    pHandle->szAddressInfo++;
    ret = SDHCP_RES_TRUE;
  }

  return ret;
}


/* Initialize the SDHCP instance. */
static uint8_t service_sdhcp_init(sdhcp_handle_t *pSdhcp)
{
  uint8_t status_done = SDHCP_RES_FALSE;

  SDHCP_ASSERT(pSdhcp != NULL);

  /* Open socket and set broadcast option on it. */
  pSdhcp->hSocket = net_socket(NET_AF_INET, NET_SOCK_DGRAM, NET_IPPROTO_IP);

  if (pSdhcp->hSocket >= 0)
  {
    net_sockaddr_in_t s_addr_in = {0};
    s_addr_in.sin_len = sizeof(s_addr_in);
    s_addr_in.sin_family = NET_AF_INET;
    s_addr_in.sin_addr.s_addr = pSdhcp->nethost.sin_addr.s_addr;  /* Already in network byte order. */
    s_addr_in.sin_port = NET_HTONS(DHCP_SERVER_PORT);

    net_setsockopt(pSdhcp->hSocket, NET_SOL_SOCKET, NET_SO_BINDTODEVICE, pNetifObject, sizeof(pNetifObject));

    if (-1 != net_bind(pSdhcp->hSocket, (net_sockaddr_t *) &s_addr_in, sizeof(s_addr_in)))
    {
      {
        int32_t broadcast = (int32_t) SDHCP_RES_TRUE;
        if (0 == net_setsockopt(pSdhcp->hSocket, NET_SOL_SOCKET, NET_SO_BROADCAST,
                                (const void *)&broadcast, sizeof(broadcast)))
        {
          status_done = SDHCP_RES_TRUE;
        }
        else
        {
          NET_DBG_ERROR("Unable to set socket options.\n");
        }
      }
      {
        int32_t timeout = (int32_t) SDHCP_TIMEOUT; /* 300 ms timeout. */
        net_setsockopt(pSdhcp->hSocket, NET_SOL_SOCKET, NET_SO_RCVTIMEO, (const void *)&timeout, sizeof(timeout));
      }
    }
    else
    {
      NET_DBG_ERROR("Unable to bind to server socket (port %" PRIu32 ").\n", (uint32_t)DHCP_SERVER_PORT);
    }

    if (status_done != SDHCP_RES_TRUE)
    {
      net_closesocket(pSdhcp->hSocket);
    }
  }
  else
  {
    NET_DBG_ERROR("Unable to open server socket (port %" PRIu32 ").\n", (uint32_t)DHCP_SERVER_PORT);
  }

  return status_done;
}


/* Find an option DHCP command. */
static uint8_t sdhcp_find_option(uint8_t Option, uint8_t *pOptions, int32_t OptionsLen,
                                 uint8_t **pData, uint32_t *pDataLen)
{
  uint8_t status_done = SDHCP_RES_FALSE;
  /* RFC 2132 */
  bool hit_end = false;
  uint8_t *pCur = pOptions;

  SDHCP_ASSERT(((0 == OptionsLen) || (0U != pOptions)) && \
               (0U != pData) && (0U != pDataLen) && (PAD != Option) && (END != Option));


  while (((pCur - pOptions) < OptionsLen) && !hit_end && !status_done)
  {
    uint8_t bCur = *pCur;
    if (PAD == bCur)
    {
      pCur++;
    }
    else if (END == bCur)
    {
      hit_end = true;
    }
    else
    {
      pCur++;
      if ((pCur - pOptions) < OptionsLen)
      {
        uint8_t bOptionLen = *pCur;
        pCur++;
        if (Option == bCur)
        {
          *pData = pCur;
          *pDataLen = bOptionLen;
          status_done = SDHCP_RES_TRUE;
        }
        pCur += bOptionLen;
      }
      else
      {
        NET_DBG_PRINT("Invalid option data (not enough room for required length byte).\n");
      }
    }
  }

  return status_done;
}


/* Return the option type. */
static uint8_t sdhcp_get_message_type(uint8_t *pOptions, int32_t OptionsLen, dhcp_msg_types_t *pMessageType)
{
  uint8_t status_done = SDHCP_RES_FALSE;
  uint8_t *p_dhcp_message = NULL;
  uint32_t dhcp_message_len = 0;

  SDHCP_ASSERT(((0 == OptionsLen) || (0U != pOptions)) && (0U != pMessageType));

  if (sdhcp_find_option(DHCPMESSAGETYPE, pOptions, OptionsLen, &p_dhcp_message, &dhcp_message_len) &&
      (1 == dhcp_message_len) && (DHCP_TYPE_FIRST_VALUE <= *p_dhcp_message) && (*p_dhcp_message <= DHCP_TYPE_LAST_VALUE))
  {
    *pMessageType = (dhcp_msg_types_t)(*p_dhcp_message);
    status_done = SDHCP_RES_TRUE;
  }

  return status_done;
}


/* Address filter */
static uint8_t sdhcp_add_addr_filter(dhcp_addr_t *pInfo, void *const pFilter)
{
  uint32_t *pAddrValue = (uint32_t *)pFilter;

  return (*pAddrValue == pInfo->addrV4);
}


/* Check the address. */
static uint8_t sdhcp_check_address_in_use(dhcp_addr_t *pInfo, void *pFilter)
{
  sdhcp_process_request_t *pcid = (sdhcp_process_request_t *)pFilter;

  SDHCP_ASSERT(0U != pcid);

  return ((0U != pInfo->szClientID) && (pcid->szClientID == pInfo->szClientID) &&
          (0 == memcmp(pcid->pbClientIdentifier, pInfo->pClientID, pcid->szClientID)));
}


/* Process the DHCP request. */
static void sdhcp_process_request(sdhcp_handle_t *pSdhcp, void *pData, uint32_t DataLen)
{
  static const uint8_t tCookie[] = { 99, 130, 83, 99 };
  net_dhcp_msg_t *const pRequest = (net_dhcp_msg_t *)pData;

  SDHCP_ASSERT(pSdhcp);

  if ((((sizeof(*pRequest) + sizeof(tCookie)) <= DataLen)
       &&  /* Take into account mandatory DHCP magic cookie values in options array (RFC 2131 section 3)*/
       ((uint8_t)BOOTREQUEST == pRequest->op) &&
       (0 == memcmp(tCookie, pRequest->options, sizeof(tCookie))))
     )
  {
    uint8_t *pOptions = pRequest->options + sizeof(tCookie);
    int32_t szOptions = DataLen - (int)sizeof(*pRequest) - (int)sizeof(tCookie);
    dhcp_msg_types_t dhcp_msg_type;

    if (sdhcp_get_message_type(pOptions, szOptions, &dhcp_msg_type))
    {
      /* Determine client host name. */
      char_t tHostName[MAX_HOSTNAME_LENGTH] = {0};
      uint8_t *pHostNameData = NULL;
      uint32_t szHostNameData = 0;

      if (sdhcp_find_option(HOSTNAME, pOptions, szOptions, &pHostNameData, &szHostNameData))
      {
        strncpy(tHostName, (char_t *)pHostNameData, szHostNameData);
        tHostName[szHostNameData] = 0;

      }
      if ((pSdhcp->pHostName != 0U) && (strncmp(tHostName, pSdhcp->pHostName, strlen(pSdhcp->pHostName)) != 0U))
      {
        /* Determine client identifier in proper RFC 2131 order (client identifier option then chaddr)*/
        uint8_t *pServieReq;
        uint32_t szServieReq;
        if (!sdhcp_find_option(CLIENTIDENTIFIER, pOptions, szOptions, &pServieReq, &szServieReq))
        {
          pServieReq = (uint8_t *)pRequest->chaddr;
          szServieReq = sizeof(pRequest->chaddr);
        }
        /* Determine if we've seen this client before*/

        uint8_t bSeenBefore = SDHCP_RES_FALSE;
        uint32_t OfferAddr = (uint32_t)NET_INADDR_BROADCAST;  /* Invalid IP address for later comparison*/
        sdhcp_process_request_t cid = { pServieReq, (uint32_t)szServieReq };
        int32_t iIndex = sdhcp_find_index(pSdhcp, sdhcp_check_address_in_use, &cid);
        if (-1 != iIndex)
        {
          dhcp_addr_t *pInfo = &pSdhcp->tAddressInfo[iIndex];
          OfferAddr = sdhcp_swap_ip(pInfo->addrV4);
          bSeenBefore = SDHCP_RES_TRUE;
        }
        uint8_t tMsgBuffer[sizeof(net_dhcp_msg_t) + sizeof(net_dhcp_option_t)] = {0};
        net_dhcp_msg_t *pReply = (net_dhcp_msg_t *)&tMsgBuffer;

        pReply->op = BOOTREPLY;
        pReply->htype = pRequest->htype;
        pReply->hlen = pRequest->hlen;
        pReply->xid = pRequest->xid;
        pReply->flags = pRequest->flags;
        pReply->giaddr = pRequest->giaddr;
        memcpy(pReply->chaddr, pRequest->chaddr, sizeof(pReply->chaddr));
        strncpy((char *)(pReply->sname), pSdhcp->pHostName, sizeof(pReply->sname));
        net_dhcp_option_t  *pServerOptions = (net_dhcp_option_t *)(pReply->options);
        memcpy(pServerOptions->tMagic, tCookie, sizeof(pServerOptions->tMagic));
        pServerOptions->tMessageType[0] = DHCPMESSAGETYPE;
        pServerOptions->tMessageType[1] = 1;
        pServerOptions->tLeaseTime[0] = IPADDRESSLEASETIME;
        pServerOptions->tLeaseTime[1] = 4;
        SDHCP_ASSERT(sizeof(uint32_t) == 4);

        *((uint32_t *)(&(pServerOptions->tLeaseTime[2]))) = NET_HTONL(1 * 60 * 60);
        pServerOptions->tSubnetMask[0] = SUBNETMASK;
        pServerOptions->tSubnetMask[1] = 4;
        SDHCP_ASSERT(sizeof(uint32_t) == 4);
        *((uint32_t *)(&(pServerOptions->tSubnetMask[2]))) = pSdhcp->netmsk.sin_addr.s_addr;
        pServerOptions->tServerID[0] = SERVERIDENTIFIER;
        pServerOptions->tServerID[1] = 4;
        SDHCP_ASSERT(sizeof(uint32_t) == 4);
        *((uint32_t *)(&(pServerOptions->tServerID[2]))) = pSdhcp->nethost.sin_addr.s_addr;
        pServerOptions->tRouter[0] = ROUTER;
        pServerOptions->tRouter[1] = 4;
        SDHCP_ASSERT(sizeof(uint32_t) == 4);
        *((uint32_t *)(&(pServerOptions->tRouter[2]))) = pSdhcp->nethost.sin_addr.s_addr;
        pServerOptions->bEND = END;

        uint8_t bSendDHCPMessage = SDHCP_RES_FALSE;

        switch (dhcp_msg_type)
        {
          case DHCP_TYPE_DISCOVER:
          {
            /* UNSUPPORTED: Requested IP Address option*/
            /* Initialize to max to wrap and offer min first*/
            pSdhcp->lastOfferAddr = (uint32_t)sdhcp_swap_ip((uint32_t)pSdhcp->netmax.sin_addr.s_addr);
            uint32_t minAddr = sdhcp_swap_ip(pSdhcp->netmin.sin_addr.s_addr);
            uint32_t maxAddr = sdhcp_swap_ip(pSdhcp->netmax.sin_addr.s_addr);
            uint32_t dwOfferAddrValue;
            uint8_t bOfferAddrValueValid = SDHCP_RES_FALSE;
            if (bSeenBefore)
            {
              dwOfferAddrValue = sdhcp_swap_ip(OfferAddr);
              bOfferAddrValueValid = SDHCP_RES_TRUE;
            }
            else
            {
              dwOfferAddrValue = pSdhcp->lastOfferAddr + 1;
            }
            /* Search for an available address if necessary. */
            uint32_t dwInitialOfferAddrValue = dwOfferAddrValue;
            uint8_t offeredInitial = SDHCP_RES_FALSE;
            /* Detect address exhaustion*/
            while (!bOfferAddrValueValid && !(offeredInitial && (dwInitialOfferAddrValue == dwOfferAddrValue)))
            {
              if (maxAddr < dwOfferAddrValue)
              {
                SDHCP_ASSERT(maxAddr + 1 == dwOfferAddrValue);
                dwOfferAddrValue = minAddr;
              }
              bOfferAddrValueValid = (-1 == sdhcp_find_index(pSdhcp, sdhcp_add_addr_filter, &dwOfferAddrValue));
              offeredInitial = SDHCP_RES_TRUE;
              if (!bOfferAddrValueValid)
              {
                dwOfferAddrValue++;
              }
            }
            if (bOfferAddrValueValid)
            {
              pSdhcp->lastOfferAddr = dwOfferAddrValue;
              uint32_t dwOfferAddr = sdhcp_swap_ip(dwOfferAddrValue);
              SDHCP_ASSERT((0U != szServieReq) && (0U != pServieReq));
              dhcp_addr_t info;
              info.addrV4 = dwOfferAddrValue;
              info.pClientID = (uint8_t *)NET_MALLOC(szServieReq);
              if (0U != info.pClientID)
              {
                memcpy(info.pClientID, pServieReq, szServieReq);
                info.szClientID = szServieReq;
                if (bSeenBefore || sdhcp_addInfo(pSdhcp, &info))
                {
                  pReply->yiaddr = dwOfferAddr;
                  pServerOptions->tMessageType[2] = (uint8_t) DHCP_TYPE_OFFER;
                  bSendDHCPMessage = SDHCP_RES_TRUE;
                  NET_DBG_PRINT("SDNS:Offering client \"%s\" IP address %d.%d.%d.%d",
                                tHostName, DWIP0(dwOfferAddr), DWIP1(dwOfferAddr),
                                DWIP2(dwOfferAddr), DWIP3(dwOfferAddr));
                }
                else
                {
                  NET_FREE(info.pClientID);
                  NET_DBG_ERROR("Insufficient memory to add client address.");
                }
                if (bSeenBefore)
                {
                  NET_FREE(info.pClientID);
                }
              }
              else
              {
                NET_DBG_ERROR("Insufficient memory to add client address.");
              }
            }
            else
            {
              NET_DBG_ERROR("No more IP addresses available for client \"%s\"", tHostName);
            }
          }
          break;

          case DNCP_TYPE_REQUEST:
          {
            /* Determine requested IP address. */
            uint32_t reqIP = NET_INADDR_BROADCAST;  /* Invalid IP address for later comparison*/
            uint8_t *pReqAddressData = NULL;
            uint32_t szReqAddressData = 0;
            if (sdhcp_find_option(REQUESTEDIPADDRESS, pOptions, szOptions, &pReqAddressData, &szReqAddressData)
                && (sizeof(reqIP) == szReqAddressData))
            {
              reqIP = *((uint32_t *)pReqAddressData);
            }
            /* Determine server identifier. */
            uint8_t *pRequServerID = 0;
            uint32_t szRequServerID = 0;
            if (sdhcp_find_option(SERVERIDENTIFIER, pOptions, szOptions, &pRequServerID, &szRequServerID) &&
                (sizeof(pSdhcp->nethost.sin_addr.s_addr) == szRequServerID) &&
                (pSdhcp->nethost.sin_addr.s_addr == *((uint32_t *)pRequServerID)))
            {
              /* Response to OFFER*/
              SDHCP_ASSERT(0 == pRequest->ciaddr);
              if (bSeenBefore)
              {
                /* Already have an IP address for this client - ACK it. */
                pServerOptions->tMessageType[2] = (uint8_t) DHCP_TYPE_ACK;
              }
              else
              {
                /* Haven't seen this client before - NAK it. */
                pServerOptions->tMessageType[2] = (uint8_t) DHCP_TYPE_NAK;
              }
            }
            else
            {
              if (((NET_INADDR_BROADCAST != reqIP)) ||
                  ((NET_INADDR_BROADCAST == reqIP) && (0U != pRequest->ciaddr)))
              {
                if (bSeenBefore && ((OfferAddr == reqIP) || (OfferAddr == pRequest->ciaddr)))
                {
                  /* Already have an IP address for this client - ACK it. */
                  pServerOptions->tMessageType[2] = (uint8_t) DHCP_TYPE_ACK;
                }
                else
                {
                  /* Haven't seen this client before or requested IP address is invalid. */
                  pServerOptions->tMessageType[2] = (uint8_t) DHCP_TYPE_NAK;
                }
              }
              else
              {
                NET_WARNING("Invalid DHCP message (invalid data)");
              }
            }
            switch (pServerOptions->tMessageType[2])
            {
              case DHCP_TYPE_ACK:
                SDHCP_ASSERT(NET_INADDR_BROADCAST != OfferAddr);
                pReply->ciaddr = OfferAddr;
                pReply->yiaddr = OfferAddr;
                bSendDHCPMessage = SDHCP_RES_TRUE;
                NET_DBG_PRINT("SDNS:Acknowledging client \"%s\" has IP address %d.%d.%d.%d",
                              tHostName, DWIP0(OfferAddr),
                              DWIP1(OfferAddr), DWIP2(OfferAddr), DWIP3(OfferAddr));
                break;
              case DHCP_TYPE_NAK:
                SDHCP_ASSERT(0 == PAD);
                (void) memset(pServerOptions->tLeaseTime, 0, sizeof(pServerOptions->tLeaseTime));
                (void) memset(pServerOptions->tSubnetMask, 0, sizeof(pServerOptions->tSubnetMask));
                bSendDHCPMessage = SDHCP_RES_TRUE;
                NET_DBG_PRINT("SDNS:Denying client \"%s\" unoffered IP address.", tHostName);
                break;
              default:
                /* Nothing to do*/
                break;
            }
          }
          break;
          case DHCP_TYPE_DECLINE:
          case DHCP_TYPE_RELEASE:
            break;
          case DHCP_TYPE_INFORM:
            break;
          case DHCP_TYPE_OFFER:
          case DHCP_TYPE_ACK:
          case DHCP_TYPE_NAK:
            NET_WARNING("Unexpected DHCP message type.");
            break;
          default:
            NET_DBG_ERROR("Invalid DHCPMessageType");
            break;
        }
        if (bSendDHCPMessage != 0U)
        {
          /* Must have set an option if we're going to be sending this message. */
          SDHCP_ASSERT(0U != pServerOptions->tMessageType[2]);
          /* Determine how to send the reply*/
          uint32_t ulAddr = NET_INADDR_LOOPBACK;  /* Invalid value */
          if (0U == pRequest->giaddr)
          {
            switch (pServerOptions->tMessageType[2])
            {
              case DHCP_TYPE_OFFER:
              case DHCP_TYPE_ACK:
              {
                if (0U == pRequest->ciaddr)
                {
                  if (0U != (BROADCAST_FLAG & pRequest->flags))
                  {
                    ulAddr = NET_INADDR_BROADCAST;
                  }
                  else
                  {
                    ulAddr = pRequest->yiaddr;  /* Already in network order */
                    if (0U == ulAddr)
                    {
                      /* UNSUPPORTED: Unicast to hardware address
                      Instead, broadcast the response and rely on other DHCP clients to ignore it*/
                      ulAddr = NET_INADDR_BROADCAST;
                    }
                  }
                }
                else
                {
                  ulAddr = pRequest->ciaddr;  /* Already in network order*/
                }
              }
              break;
              case DHCP_TYPE_NAK:
              {
                ulAddr = NET_INADDR_BROADCAST;
              }
              break;
              default:
                NET_DBG_ERROR("Invalid DHCPMessageType");
                break;
            }
          }
          else
          {
            ulAddr = pRequest->giaddr;  /* Already in network order*/
            pReply->flags |= BROADCAST_FLAG;  /* Indicate to the relay agent that it must broadcast*/
          }
          SDHCP_ASSERT((NET_INADDR_LOOPBACK != ulAddr) && (0U != ulAddr));
          net_sockaddr_in_t sAddr = {0};
          sAddr.sin_len = (uint8_t) sizeof(sAddr);
          sAddr.sin_family = NET_AF_INET;
          sAddr.sin_addr.s_addr = ulAddr;
          sAddr.sin_port = NET_HTONS(DHCP_CLIENT_PORT);
          int32_t ret = net_sendto(pSdhcp->hSocket, (uint8_t *)pReply, sizeof(tMsgBuffer),
                                   0, (net_sockaddr_t *) &sAddr, sizeof(sAddr));
          SDHCP_ASSERT(ret != -1);
        }
      }
    }
    else
    {
      NET_WARNING("Invalid DHCP message (invalid or missing DHCP message type).\n");
    }
  }
  else
  {
    NET_WARNING("Invalid DHCP message (failed initial checks).\n");
  }
}

/* DHCP task. */
static void service_sdhcp_task(void const *pParam)
{
  struct netif *const netif = (struct netif *) pParam;
  sdhcp_handle_t *p_sdhcp = NET_MALLOC(sizeof(*p_sdhcp));
  net_sockaddr_in_t s_addr_in = {0};
  uint32_t s_addr_in_len = sizeof(s_addr_in);

  SDHCP_ASSERT(p_sdhcp);

  (void) memset(p_sdhcp, 0, sizeof(*p_sdhcp));

  pNetifObject = (net_if_handle_t *)(netif->state);
  SDHCP_ASSERT(pNetifObject);

  p_sdhcp->pHostName = "ST-HotSpot";

#if (LWIP_IPV4 && LWIP_IPV6)
  p_sdhcp->netmin.sin_addr.s_addr = netif->ip_addr.u_addr.ip4.addr;
  ((uint8_t *)&p_sdhcp->netmin.sin_addr.s_addr)[3] = MIN_IP;

  p_sdhcp->netmax.sin_addr.s_addr = netif->ip_addr.u_addr.ip4.addr;
  ((uint8_t *)&p_sdhcp->netmax.sin_addr.s_addr)[3] = MAX_IP;

  p_sdhcp->nethost.sin_addr.s_addr = netif->ip_addr.u_addr.ip4.addr;
  p_sdhcp->netmsk.sin_addr.s_addr = netif->netmask.u_addr.ip4.addr;

#else
  p_sdhcp->netmin.sin_addr.s_addr = netif->ip_addr.addr;
  ((uint8_t *)&p_sdhcp->netmin.sin_addr.s_addr)[3] = MIN_IP;

  p_sdhcp->netmax.sin_addr.s_addr = netif->ip_addr.addr;
  ((uint8_t *)&p_sdhcp->netmax.sin_addr.s_addr)[3] = MAX_IP;

  p_sdhcp->nethost.sin_addr.s_addr = netif->ip_addr.addr;
  p_sdhcp->netmsk.sin_addr.s_addr = netif->netmask.addr;
#endif /* LWIP_IPV4 && LWIP_IPV6 */

  if ((p_sdhcp->nethost.sin_addr.s_addr == 0U) && (p_sdhcp->netmsk.sin_addr.s_addr == 0U))
  {
    NET_DBG_ERROR("Unable to run DHCP server, missing parameters.\n");
  }
  else
  {
    if (service_sdhcp_init(p_sdhcp) == SDHCP_RES_FALSE)
    {
      NET_DBG_ERROR("Unable to initialize the DHCP server.\n");
    }
    else
    {
      uint8_t *p_udp_buffer = (uint8_t *)NET_MALLOC(MAX_UDP_MESSAGE_SIZE);
      if (NULL != p_udp_buffer)
      {
        while (RunTask != 0U)
        {
          int32_t szRet = net_recvfrom(p_sdhcp->hSocket, (uint8_t *)p_udp_buffer, MAX_UDP_MESSAGE_SIZE,
                                       0, (net_sockaddr_t *)&s_addr_in, &s_addr_in_len);
          if (szRet > 0)
          {
            sdhcp_process_request(p_sdhcp, p_udp_buffer, (uint32_t) szRet);
          }
        }
        NET_DBG_PRINT("SDHCP: service exit.\n");
        NET_FREE(p_udp_buffer);
      }
    }

    if (p_sdhcp->hSocket >= 0)
    {
      net_closesocket(p_sdhcp->hSocket);
    }
    NET_FREE(p_sdhcp);
  }

  (void) osThreadTerminate(SdhcpTask);
}


/* Create the wake-up service. */
int32_t service_sdhcp_create(struct netif *pNetif)
{
  int32_t ret = NET_OK;

#if (osCMSIS < 0x20000U)
  const osThreadDef_t os_thread_def =
  {
    TASK_SDHCP_NAME,
    service_sdhcp_task,
    TASK_SDHCP_PRIORITY,
    0,
    TASK_SDHCP_STACK
  };
  SdhcpTask = osThreadCreate(&os_thread_def, pNetif);

#else
  const osThreadAttr_t attributes =
  {
    .name = TASK_SDHCP_NAME,
    .stack_size = TASK_SDHCP_STACK,
    .priority = TASK_SDHCP_PRIORITY,
  };
  SdhcpTask = osThreadNew((osThreadFunc_t) service_sdhcp_task, pNetif, &attributes);
#endif /* osCMSIS */

  RunTask = 1;

  NET_DBG_PRINT("SDHCP: Start Service.\n");

  if (SdhcpTask  == 0)
  {
    NET_DBG_ERROR("Create task %s\n", TASK_SDHCP_NAME);
    ret = NET_ERROR_UNSUPPORTED;
  }

  return ret;
}


/* Delete the service. */
int32_t service_sdhcp_delete(void)
{
  RunTask = 0;

  return NET_OK;
}
