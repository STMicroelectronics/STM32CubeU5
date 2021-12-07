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
/*
  This source implements a very basic DHCP server in order to allow
  mobile phone connection when the WiFi is in access point.
  The DHCP allocates 3 addresses for a limited period of time.
  */

#include "net_connect.h"
#include "net_internals.h"
#include "net_mem.h"
#include "net_errors.h"

#include "lwip/tcpip.h"


#define TASK_SDHCP_NAME                 "sdhcp"
#define TASK_SDHCP_PRIORITY             (osPriorityNormal)
#define TASK_SDHCP_STACK                 2048

#define MIN_IP                           15U
#define MAX_IP                           17U
#define MAX_ADRES_INFO                   (MAX_IP - MIN_IP)
#define SDHCP_TIMEOUT                   300U

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
#define DHCP_SERVER_PORT      (67)
#define DHCP_CLIENT_PORT      (68)
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
  int32_t             hSocket;
  uint32_t            lastOfferAddr;
  struct net_sockaddr_in  nethost;
  struct net_sockaddr_in  netmsk;
  struct net_sockaddr_in  netmin;
  struct net_sockaddr_in  netmax;
  char_t             *pHostName;
  dhcp_addr_t         tAddressInfo[MAX_ADRES_INFO];
  uint32_t            szAddressInfo;
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
  BOOTREPLY = 2
} op_values_t;

/* RFC 2132 section 9.6  */

typedef enum t_option_values
{
  PAD = 0,
  SUBNETMASK = 1,
  HOSTNAME = 12,
  REQUESTEDIPADDRESS = 50,
  IPADDRESSLEASETIME = 51,
  DHCPMESSAGETYPE = 53,
  SERVERIDENTIFIER = 54,
  CLIENTIDENTIFIER = 61,
  ROUTER = 3,
  END = 255
} option_values_t;

typedef enum t_dhcp_msg_types
{
  DISCOVER = 1U,
  OFFER = 2U,
  REQUEST = 3U,
  DECLINE = 4U,
  ACK = 5U,
  NAK = 6U,
  RELEASE = 7U,
  INFORM = 8U
} dhcp_msg_types_t;

typedef uint8_t(*sdhcp_find_cb)(dhcp_addr_t *raiui, void *pvFilterData);


/* DHCP magic cookie values*/
static int8_t bRunTask = 0;

#if (osCMSIS < 0x20000U)
static osThreadId hSdhcpTask;
#else
static osThreadId_t hSdhcpTask;
#endif /* osCMSIS */

static uint32_t sdhcp_swap_ip(uint32_t dw);
static int32_t sdhcp_find_index(sdhcp_handle_t *pHandle, sdhcp_find_cb pFilter, void *pData);
static uint8_t sdhcp_addInfo(sdhcp_handle_t *pHandle, dhcp_addr_t *pInfo);
static uint8_t service_sdhcp_init(sdhcp_handle_t *pHandle);
static uint8_t sdhcp_find_option(uint8_t bOption, uint8_t *pOptions, int32_t szOptions, uint8_t **pData,
                                 uint32_t *pszData);
static uint8_t sdhcp_get_message_type(uint8_t *pOptions,  int32_t szOptions, dhcp_msg_types_t *pMessageType);
static uint8_t sdhcp_add_addr_filter(dhcp_addr_t  *pInfo, void *const pFilter);
static uint8_t sdhcp_check_address_in_use(dhcp_addr_t *pInfo, void *pFilter);
static void sdhcp_process_request(sdhcp_handle_t *pHandle, void *pData, uint32_t szData);
static void service_sdhcp_task(void const *param);

/* Reverse the IP order*/
static uint32_t sdhcp_swap_ip(uint32_t dw_in)
{
  uint32_t dw = dw_in;
  uint32_t swapped = 0;
  for (int32_t  aa = 0; aa < 4; aa++)
  {
    swapped <<= 8;
    swapped |= (dw & 0xFFU);
    dw >>= 8;
  }
  return swapped;
}

/* Find a client in the list */
static int32_t sdhcp_find_index(sdhcp_handle_t *pHandle, sdhcp_find_cb pFilter, void *pData)
{
  int32_t ret = -1;
  SDHCP_ASSERT((0U != pFilter) && (NULL != pData));
  for (int32_t  i = 0; i < (int32_t) pHandle->szAddressInfo; i++)
  {
    if (pFilter(&pHandle->tAddressInfo[i], pData) != 0U)
    {
      ret =  i;
      break;
    }
  }
  return ret;
}

/* Add a client to the list */
static uint8_t sdhcp_addInfo(sdhcp_handle_t *pHandle, dhcp_addr_t *pInfo)
{
  uint8_t ret;

  SDHCP_ASSERT(NULL != pInfo);
  if (pHandle->szAddressInfo > MAX_ADRES_INFO)
  {
    ret = SDHCP_RES_FALSE;
  }
  else
  {
    pHandle->tAddressInfo[pHandle->szAddressInfo] = *pInfo;
    pHandle->szAddressInfo++;
    ret = SDHCP_RES_TRUE;
  }
  return ret;
}

/* Init the sdhcp instance */
static uint8_t service_sdhcp_init(sdhcp_handle_t *pHandle)
{
  SDHCP_ASSERT(pHandle != NULL);

  uint8_t bSuccess = SDHCP_RES_FALSE;

  /* Open socket and set broadcast option on it    */

  pHandle->hSocket = net_socket(NET_AF_INET, NET_SOCK_DGRAM, NET_IPPROTO_IP);
  if (pHandle->hSocket >= 0)
  {
    struct net_sockaddr_in  sAddr;
    sAddr.sin_family = NET_AF_INET;
    sAddr.sin_addr.s_addr = pHandle->nethost.sin_addr.s_addr;  /* Already in network byte order*/
    sAddr.sin_port = NET_HTONS((int16_t)DHCP_SERVER_PORT);
    if (-1 != net_bind(pHandle->hSocket, (net_sockaddr_t *) &sAddr, sizeof(sAddr)))
    {
      int32_t bOption = (int32_t) SDHCP_RES_TRUE;
      if (0 == net_setsockopt(pHandle->hSocket, NET_SOL_SOCKET, NET_SO_BROADCAST,
                              (char_t *)(&bOption), sizeof(bOption)))
      {
        bSuccess = SDHCP_RES_TRUE;
      }
      else
      {
        NET_DBG_ERROR("Unable to set socket options.");
      }
      bOption = (int32_t) SDHCP_TIMEOUT; /*300ms timeout */
      net_setsockopt(pHandle->hSocket, NET_SOL_SOCKET, NET_SO_RCVTIMEO, (char *)(&bOption), sizeof(bOption));
    }
    else
    {
      NET_DBG_ERROR("Unable to bind to server socket (port %d).", DHCP_SERVER_PORT);
    }
  }
  else
  {
    NET_DBG_ERROR("Unable to open server socket (port %d).", DHCP_SERVER_PORT);
  }
  return bSuccess;
}

/* Find an option DHCP cmd */
static uint8_t sdhcp_find_option(uint8_t bOption, uint8_t *pOptions, int32_t szOptions, uint8_t **pData,
                                 uint32_t *pszData)
{
  SDHCP_ASSERT(((0 == szOptions) || (0U != pOptions)) && (0U != pData) && (0U != pszData) &&
               (PAD != bOption) && (END != bOption));
  uint8_t bSuccess = SDHCP_RES_FALSE;
  /* RFC 2132 */
  uint8_t bHitEND = SDHCP_RES_FALSE;
  uint8_t *pCur = pOptions;
  while (((pCur - pOptions) < szOptions) && !bHitEND && !bSuccess)
  {
    uint8_t bCur = *pCur;
    if (PAD == bCur)
    {
      pCur++;
    }
    else if (END == bCur)
    {
      bHitEND = SDHCP_RES_TRUE;
    }
    else
    {
      pCur++;
      if ((pCur - pOptions) < szOptions)
      {
        uint8_t bOptionLen = *pCur;
        pCur++;
        if (bOption == bCur)
        {
          *pData = pCur;
          *pszData = bOptionLen;
          bSuccess = SDHCP_RES_TRUE;
        }
        pCur += bOptionLen;
      }
      else
      {
        NET_DBG_PRINT("Invalid option data (not enough room for required length byte");
      }
    }
  }
  return bSuccess;
}

/* Return the option type */
static uint8_t sdhcp_get_message_type(uint8_t *pOptions, int32_t szOptions, dhcp_msg_types_t *pMessageType)
{
  SDHCP_ASSERT(((0 == szOptions) || (0U != pOptions)) && (0U != pMessageType));
  uint8_t bSuccess = SDHCP_RES_FALSE;
  uint8_t *pDHCPMessage;
  uint32_t szDHCPMessage;
  if (sdhcp_find_option(DHCPMESSAGETYPE, pOptions, szOptions, &pDHCPMessage, &szDHCPMessage) &&
      (1 == szDHCPMessage) && (1 <= *pDHCPMessage) && (*pDHCPMessage <= 8))
  {
    *pMessageType = (dhcp_msg_types_t)(*pDHCPMessage);
    bSuccess = SDHCP_RES_TRUE;
  }
  return bSuccess;
}

/* Address filter */
static uint8_t sdhcp_add_addr_filter(dhcp_addr_t  *pInfo, void *const pFilter)
{
  uint32_t  *pAddrValue = (uint32_t *)pFilter;
  return (*pAddrValue == pInfo->addrV4);
}

/* Check the address*/
static uint8_t sdhcp_check_address_in_use(dhcp_addr_t *pInfo, void *pFilter)
{
  sdhcp_process_request_t  *pcid = (sdhcp_process_request_t *)pFilter;
  SDHCP_ASSERT(0U != pcid);
  return ((0U != pInfo->szClientID) && (pcid->szClientID == pInfo->szClientID) &&
          (0 == memcmp(pcid->pbClientIdentifier, pInfo->pClientID, pcid->szClientID)));
}


/* Process the DHCP request */
static void sdhcp_process_request(sdhcp_handle_t *pHandle, void *pData, uint32_t szData)
{
  SDHCP_ASSERT(pHandle);
  static const uint8_t tCookie[] = { 99, 130, 83, 99 };
  net_dhcp_msg_t  *pRequest = (net_dhcp_msg_t *)pData;
  if ((((sizeof(*pRequest) + sizeof(tCookie)) <= szData)
       &&  /* Take into account mandatory DHCP magic cookie values in options array (RFC 2131 section 3)*/
       ((uint8_t)BOOTREQUEST == pRequest->op) &&
       (0 == memcmp(tCookie, pRequest->options, sizeof(tCookie))))
     )
  {
    uint8_t *pOptions = pRequest->options + sizeof(tCookie);
    int32_t szOptions = szData - (int)sizeof(*pRequest) - (int)sizeof(tCookie);
    dhcp_msg_types_t dhcpmtMessageType;
    if (sdhcp_get_message_type(pOptions, szOptions, &dhcpmtMessageType))
    {
      /* Determine client host name*/
      char_t tHostName[MAX_HOSTNAME_LENGTH];
      tHostName[0] = '\0';
      uint8_t  *pHostNameData;
      uint32_t  szHostNameData;
      if (sdhcp_find_option(HOSTNAME, pOptions, szOptions, &pHostNameData, &szHostNameData))
      {
        strncpy(tHostName, (char_t *)pHostNameData, szHostNameData);
        tHostName[szHostNameData] = 0;

      }
      if ((pHandle->pHostName != 0U) && (strncmp(tHostName, pHandle->pHostName, strlen(pHandle->pHostName)) != 0U))
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
        int32_t iIndex = sdhcp_find_index(pHandle, sdhcp_check_address_in_use, &cid);
        if (-1 != iIndex)
        {
          dhcp_addr_t *pInfo = &pHandle->tAddressInfo[iIndex];
          OfferAddr = sdhcp_swap_ip(pInfo->addrV4);
          bSeenBefore = SDHCP_RES_TRUE;
        }
        uint8_t tMsgBuffer[sizeof(net_dhcp_msg_t) + sizeof(net_dhcp_option_t)];
        (void) memset(tMsgBuffer, 0, sizeof(tMsgBuffer));
        net_dhcp_msg_t  *pReply = (net_dhcp_msg_t *)&tMsgBuffer;
        pReply->op = BOOTREPLY;
        pReply->htype = pRequest->htype;
        pReply->hlen = pRequest->hlen;
        pReply->xid = pRequest->xid;
        pReply->flags = pRequest->flags;
        pReply->giaddr = pRequest->giaddr;
        memcpy(pReply->chaddr, pRequest->chaddr, sizeof(pReply->chaddr));
        strncpy((char *)(pReply->sname), pHandle->pHostName, sizeof(pReply->sname));
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
        *((uint32_t *)(&(pServerOptions->tSubnetMask[2]))) = pHandle->netmsk.sin_addr.s_addr;
        pServerOptions->tServerID[0] = SERVERIDENTIFIER;
        pServerOptions->tServerID[1] = 4;
        SDHCP_ASSERT(sizeof(uint32_t) == 4);
        *((uint32_t *)(&(pServerOptions->tServerID[2]))) = pHandle->nethost.sin_addr.s_addr;
        pServerOptions->tRouter[0] = ROUTER;
        pServerOptions->tRouter[1] = 4;
        SDHCP_ASSERT(sizeof(uint32_t) == 4);
        *((uint32_t *)(&(pServerOptions->tRouter[2]))) = pHandle->nethost.sin_addr.s_addr;
        pServerOptions->bEND = END;
        uint8_t bSendDHCPMessage = SDHCP_RES_FALSE;
        switch (dhcpmtMessageType)
        {
          case DISCOVER:
          {
            /* UNSUPPORTED: Requested IP Address option*/
            /* Initialize to max to wrap and offer min first*/
            pHandle->lastOfferAddr = (uint32_t)sdhcp_swap_ip((uint32_t)pHandle->netmax.sin_addr.s_addr);
            uint32_t minAddr = sdhcp_swap_ip(pHandle->netmin.sin_addr.s_addr);
            uint32_t maxAddr = sdhcp_swap_ip(pHandle->netmax.sin_addr.s_addr);
            uint32_t dwOfferAddrValue;
            uint8_t bOfferAddrValueValid = SDHCP_RES_FALSE;
            if (bSeenBefore)
            {
              dwOfferAddrValue = sdhcp_swap_ip(OfferAddr);
              bOfferAddrValueValid = SDHCP_RES_TRUE;
            }
            else
            {
              dwOfferAddrValue = pHandle->lastOfferAddr + 1;
            }
            /* Search for an available address if necessary*/
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
              bOfferAddrValueValid = (-1 == sdhcp_find_index(pHandle, sdhcp_add_addr_filter, &dwOfferAddrValue));
              offeredInitial = SDHCP_RES_TRUE;
              if (!bOfferAddrValueValid)
              {
                dwOfferAddrValue++;
              }
            }
            if (bOfferAddrValueValid)
            {
              pHandle->lastOfferAddr = dwOfferAddrValue;
              uint32_t dwOfferAddr = sdhcp_swap_ip(dwOfferAddrValue);
              SDHCP_ASSERT((0U != szServieReq) && (0U != pServieReq));
              dhcp_addr_t info;
              info.addrV4 = dwOfferAddrValue;
              info.pClientID = (uint8_t *)NET_MALLOC(szServieReq);
              if (0U != info.pClientID)
              {
                memcpy(info.pClientID, pServieReq, szServieReq);
                info.szClientID = szServieReq;
                if (bSeenBefore || sdhcp_addInfo(pHandle, &info))
                {
                  pReply->yiaddr = dwOfferAddr;
                  pServerOptions->tMessageType[2] = (uint8_t) OFFER;
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
          case REQUEST:
          {
            /* Determine requested IP address*/
            uint32_t reqIP = NET_INADDR_BROADCAST;  /* Invalid IP address for later comparison*/
            uint8_t *pReqAddressData = 0;
            uint32_t szReqAddressData = 0;
            if (sdhcp_find_option(REQUESTEDIPADDRESS, pOptions, szOptions, &pReqAddressData, &szReqAddressData)
                && (sizeof(reqIP) == szReqAddressData))
            {
              reqIP = *((uint32_t *)pReqAddressData);
            }
            /* Determine server identifier*/
            uint8_t *pRequServerID = 0;
            uint32_t szRequServerID = 0;
            if (sdhcp_find_option(SERVERIDENTIFIER, pOptions, szOptions, &pRequServerID, &szRequServerID) &&
                (sizeof(pHandle->nethost.sin_addr.s_addr) == szRequServerID) &&
                (pHandle->nethost.sin_addr.s_addr == *((uint32_t *)pRequServerID)))
            {
              /* Response to OFFER*/
              SDHCP_ASSERT(0 == pRequest->ciaddr);
              if (bSeenBefore)
              {
                /* Already have an IP address for this client - ACK it*/
                pServerOptions->tMessageType[2] = (uint8_t) ACK;
              }
              else
              {
                /* Haven't seen this client before - NAK it*/
                pServerOptions->tMessageType[2] = (uint8_t) NAK;
              }
            }
            else
            {
              if (((NET_INADDR_BROADCAST != reqIP)) ||
                  ((NET_INADDR_BROADCAST == reqIP) && (0U != pRequest->ciaddr)))
              {
                if (bSeenBefore && ((OfferAddr == reqIP) || (OfferAddr == pRequest->ciaddr)))
                {
                  /* Already have an IP address for this client - ACK it*/
                  pServerOptions->tMessageType[2] = (uint8_t) ACK;
                }
                else
                {
                  /* Haven't seen this client before or requested IP address is invalid*/
                  pServerOptions->tMessageType[2] = (uint8_t) NAK;
                }
              }
              else
              {
                NET_WARNING("Invalid DHCP message (invalid data)");
              }
            }
            switch (pServerOptions->tMessageType[2])
            {
              case ACK:
                SDHCP_ASSERT(NET_INADDR_BROADCAST != OfferAddr);
                pReply->ciaddr = OfferAddr;
                pReply->yiaddr = OfferAddr;
                bSendDHCPMessage = SDHCP_RES_TRUE;
                NET_DBG_PRINT("SDNS:Acknowledging client \"%s\" has IP address %d.%d.%d.%d",
                              tHostName, DWIP0(OfferAddr),
                              DWIP1(OfferAddr), DWIP2(OfferAddr), DWIP3(OfferAddr));
                break;
              case NAK:
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
          case DECLINE:
          case RELEASE:
            break;
          case INFORM:
            break;
          case OFFER:
          case ACK:
          case NAK:
            NET_WARNING("Unexpected DHCP message type.");
            break;
          default:
            NET_DBG_ERROR("Invalid DHCPMessageType");
            break;
        }
        if (bSendDHCPMessage != 0U)
        {
          /* Must have set an option if we're going to be sending this message*/
          SDHCP_ASSERT(0U != pServerOptions->tMessageType[2]);
          /* Determine how to send the reply*/
          uint32_t ulAddr = NET_INADDR_LOOPBACK;  /* Invalid value */
          if (0U == pRequest->giaddr)
          {
            switch (pServerOptions->tMessageType[2])
            {
              case OFFER:
              case ACK:
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
              case NAK:
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
          struct net_sockaddr_in sAddr;
          (void) memset(&sAddr, 0, sizeof(sAddr));
          sAddr.sin_len = (uint8_t) sizeof(sAddr);
          sAddr.sin_family = NET_AF_INET;
          sAddr.sin_addr.s_addr = ulAddr;
          sAddr.sin_port = NET_HTONS((int16_t)DHCP_CLIENT_PORT);
          int32_t ret = net_sendto(pHandle->hSocket, (uint8_t *)pReply, sizeof(tMsgBuffer),
                                   0, (net_sockaddr_t *) &sAddr, sizeof(sAddr));
          SDHCP_ASSERT(ret != -1);
        }
      }
    }
    else
    {
      NET_WARNING("Invalid DHCP message (invalid or missing DHCP message type).");
    }
  }
  else
  {
    NET_WARNING("Invalid DHCP message (failed initial checks).");
  }
}

/* DHCP task */
static void service_sdhcp_task(void const *param)
{
  struct netif *netif = (struct netif *) param;
  sdhcp_handle_t *pHandle = NET_MALLOC(sizeof(sdhcp_handle_t));
  struct net_sockaddr_in sAddr;
  uint32_t szAddr = sizeof(sAddr);
  int32_t szRet;
  uint8_t *pBuffer;

  SDHCP_ASSERT(pHandle);
  (void) memset(pHandle, 0, sizeof(sdhcp_handle_t));


  pHandle->pHostName = "ST-HotSpot";
#if LWIP_IPV4 && LWIP_IPV6
  pHandle->netmin.sin_addr.s_addr = netif->ip_addr.u_addr.ip4.addr;
  ((uint8_t *)&pHandle->netmin.sin_addr.s_addr)[3] = MIN_IP;

  pHandle->netmax.sin_addr.s_addr = netif->ip_addr.u_addr.ip4.addr;
  ((uint8_t *)&pHandle->netmax.sin_addr.s_addr)[3] = MAX_IP;

  pHandle->nethost.sin_addr.s_addr = netif->ip_addr.u_addr.ip4.addr;
  pHandle->netmsk.sin_addr.s_addr = netif->netmask.u_addr.ip4.addr;
#else
  pHandle->netmin.sin_addr.s_addr = netif->ip_addr.addr;
  ((uint8_t *)&pHandle->netmin.sin_addr.s_addr)[3] = MIN_IP;

  pHandle->netmax.sin_addr.s_addr = netif->ip_addr.addr;
  ((uint8_t *)&pHandle->netmax.sin_addr.s_addr)[3] = MAX_IP;

  pHandle->nethost.sin_addr.s_addr = netif->ip_addr.addr;
  pHandle->netmsk.sin_addr.s_addr = netif->netmask.addr;
#endif /* LWIP_IPV4 && LWIP_IPV6 */
  if ((pHandle->nethost.sin_addr.s_addr == 0U) && (pHandle->netmsk.sin_addr.s_addr == 0U))
  {
    NET_DBG_ERROR("Unable to run DHCP server, missing parameters.");
  }
  else
  {
    if (service_sdhcp_init(pHandle) == SDHCP_RES_FALSE)
    {
      NET_DBG_ERROR("Unable to init DHCP server");
    }
    else
    {
      pBuffer = (uint8_t *)NET_MALLOC(MAX_UDP_MESSAGE_SIZE);
      if (NULL != pBuffer)
      {
        while (bRunTask != 0U)
        {
          szRet = net_recvfrom(pHandle->hSocket, (uint8_t *)pBuffer, MAX_UDP_MESSAGE_SIZE,
                               0, (net_sockaddr_t *)(&sAddr), &szAddr);
          if (szRet > 0)
          {
            sdhcp_process_request(pHandle, pBuffer, (uint32_t) szRet);
          }
        }
        NET_FREE(pBuffer);
      }
    }
  }
  NET_FREE(pHandle);
  (void) osThreadTerminate(hSdhcpTask);
}
/* Create the wakeup service */
int32_t service_sdhcp_create(struct netif *netif)
{
  int32_t       ret = NET_OK;
#if (osCMSIS < 0x20000U)
  const osThreadDef_t os_thread_def = { TASK_SDHCP_NAME, service_sdhcp_task, TASK_SDHCP_PRIORITY, 0, TASK_SDHCP_STACK};
  hSdhcpTask =  osThreadCreate(&os_thread_def, netif);
#else
  const osThreadAttr_t attributes =
  {
    .name = TASK_SDHCP_NAME,
    .stack_size = TASK_SDHCP_STACK,
    .priority = TASK_SDHCP_PRIORITY,
  };
  hSdhcpTask =  osThreadNew((osThreadFunc_t) service_sdhcp_task, netif, &attributes);
#endif /* osCMSIS */

  bRunTask = 1;

  NET_DBG_PRINT("SDHCP: Start Service");

  if (hSdhcpTask  == 0)
  {
    NET_DBG_ERROR("Create task %s", TASK_SDHCP_NAME);
    ret =  NET_ERROR_UNSUPPORTED;
  }

  return ret;
}


/* Delete the service */
int32_t service_sdhcp_delete(void)
{
  bRunTask = 0;

  return NET_OK;
}
