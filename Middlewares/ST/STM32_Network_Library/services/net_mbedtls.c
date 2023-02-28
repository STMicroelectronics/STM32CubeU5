/**
  ******************************************************************************
  * @file    net_mbedtls.c
  * @author  MCD Application Team
  * @brief   Network abstraction at transport layer level. mbedTLS implementation.
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
#include <stdint.h>
#include <inttypes.h>

#define NET_IMPLEMENTATION_USES_PLATFORM_DECLS
#include "net_conf.h"

#include "net_internals.h"
#include "net_perf.h"


#ifdef NET_MBEDTLS_HOST_SUPPORT

#if defined(HAL_RNG_MODULE_ENABLED)
extern RNG_HandleTypeDef hrng;

#else
/* Forward a declaration.*/
extern struct __RNG_HandleTypeDef hrng;
#endif /* HAL_RNG_MODULE_ENABLED */


/* Private defines -----------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void mbedtls_free_resource(net_socket_t *sock);
static int32_t mbedtls_net_recv(void *ctx, uchar_t *buf, size_t len, uint32_t timeout);
static int32_t mbedtls_net_send(void *ctx, const uchar_t *buf, size_t len);

#ifdef NET_USE_RTOS
extern void *pxCurrentTCB;

#ifdef MBEDTLS_THREADING_ALT

typedef int (*mutex_t)(mbedtls_threading_mutex_t *);

static void mutex_init(mbedtls_threading_mutex_t *mutex);
static void mutex_free(mbedtls_threading_mutex_t *mutex);
static int32_t mutex_lock(mbedtls_threading_mutex_t *mutex);
static int32_t mutex_unlock(mbedtls_threading_mutex_t *mutex);

static void mutex_init(mbedtls_threading_mutex_t *mutex)
{
#if (osCMSIS >= 0x20000U)
  mutex->id = osSemaphoreNew(1, 1, NULL);
#else
  mutex->id = osSemaphoreCreate(&mutex->def, 1);
#endif /* osCMSIS */
}


static void mutex_free(mbedtls_threading_mutex_t *mutex)
{
  (void)osSemaphoreDelete(mutex->id);
}

#define WAITFOREVER     0xFFFFFFFFU     /* redefined for  MISRA checks */

static int32_t mutex_lock(mbedtls_threading_mutex_t *mutex)
{
  BaseType_t ret;

#if (osCMSIS >= 0x20000U)
  ret = (BaseType_t)osSemaphoreAcquire(mutex->id, WAITFOREVER);

#else
  ret = (BaseType_t)osSemaphoreWait(mutex->id, WAITFOREVER);

#endif /* osCMSIS */

  return (ret >= 0) ? 0 : -1;
}


static int32_t mutex_unlock(mbedtls_threading_mutex_t *mutex)
{
  BaseType_t ret;
  ret = (BaseType_t)osSemaphoreRelease(mutex->id);
  return (ret >= 0) ? 0 : -1;
}

#endif /* MBEDTLS_THREADING_ALT */
#endif /* NET_USE_RTOS */


void net_tls_init(void)
{
#ifdef MBEDTLS_THREADING_ALT
  mbedtls_threading_set_alt(mutex_init, mutex_free, (mutex_t) mutex_lock, (mutex_t)mutex_unlock);
#endif /* MBEDTLS_THREADING_ALT */
}


void net_tls_destroy(void)
{
#ifdef MBEDTLS_THREADING_ALT
  mbedtls_threading_free_alt();
#endif /* MBEDTLS_THREADING_ALT */
}


/* Functions Definition ------------------------------------------------------*/
bool net_mbedtls_check_tlsdata(net_socket_t *sock)
{
  bool ret = true;

  if (NULL == sock->tlsData)
  {
    void *const p = NET_MALLOC(sizeof(net_tls_data_t));
    if (p == NULL)
    {
      NET_DBG_ERROR("Error during setting option.\n");
      ret = false;
    }
    else
    {
      sock->tlsData = p;
      (void) memset(sock->tlsData, 0, sizeof(*sock->tlsData));
      sock->tlsData->tls_srv_verification = true;
    }
  }
  return ret;
}


#if defined(MBEDTLS_DEBUG_C)
static void DebugPrint(void *ctx,
                       int32_t level,
                       const char_t *file,
                       int32_t line,
                       const char_t *str)

{
  /* Unused parameters. */
  (void)ctx;
  (void)level;

#ifdef NET_USE_RTOS
  NET_PRINT_WO_CR("%p => %s:%04" PRId32 ": %s\n", pxCurrentTCB, file, line, str);

#else
  NET_PRINT_WO_CR("%s:%04" PRId32 ": %s\n", file, line, str);
#endif /* NET_USE_RTOS */
}
#endif /* MBEDTLS_DEBUG_C */


void net_mbedtls_set_read_timeout(net_socket_t *sock)
{
  net_tls_data_t *const tls_data = sock->tlsData;

  if (tls_data != NULL)
  {
    mbedtls_ssl_conf_read_timeout(&tls_data->conf, (uint32_t)sock->read_timeout);
  }
}


static void *net_wrapper_calloc(size_t num, size_t size)
{
  return NET_CALLOC(num, size);
}

typedef void (*mbedtls_debug_func_t)(void *, int, const char *, int, const char *);
typedef int (*mbedtls_rng_func_t)(void *, unsigned char *, size_t);


static void net_wrapper_free(void *p)
{
  NET_FREE(p);
}


int32_t net_mbedtls_start(net_socket_t *sock)
{
  int32_t ret = NET_OK;
  net_tls_data_t *const tls_data = sock->tlsData;
  uint32_t start_tick;

  (void)mbedtls_platform_set_calloc_free(net_wrapper_calloc, net_wrapper_free);
  mbedtls_ssl_init(&tls_data->ssl);
  mbedtls_ssl_config_init(&tls_data->conf);

#if defined(MBEDTLS_DEBUG_C)
  mbedtls_ssl_conf_dbg(&tls_data->conf, (mbedtls_debug_func_t) DebugPrint, NULL);
  mbedtls_debug_set_threshold(NET_MBEDTLS_DEBUG_LEVEL);

#else
  mbedtls_ssl_conf_dbg(&tls_data->conf, (mbedtls_debug_func_t) NULL, NULL);
#endif /* MBEDTLS_DEBUG_C */

  mbedtls_x509_crt_init(&tls_data->cacert);

  if (tls_data->tls_dev_cert != NULL)
  {
    mbedtls_x509_crt_init(&tls_data->clicert);
  }

  if (tls_data->tls_dev_key != NULL)
  {
    mbedtls_pk_init(&tls_data->pkey);
  }

  /* Root CA */
  if (tls_data->tls_ca_certs != NULL)
  {
    ret = mbedtls_x509_crt_parse(&tls_data->cacert, (uchar_t const *) tls_data->tls_ca_certs,
                                 strlen((char_t const *) tls_data->tls_ca_certs) + 1U);

    if (ret != 0)
    {
      NET_DBG_ERROR("Failed!\n mbedtls_x509_crt_parse returned 0x%" PRIx32 " while parsing root certificate\n",
                    (uint32_t)ret);
      mbedtls_free_resource(sock);
      ret = NET_ERROR_MBEDTLS_CRT_PARSE;
    }
  }
  else
  {
    /*no root so cannot check server identification ?
     tlsData->tls_srv_verification = */
  }


  /* Client certificate and key. */
  if ((ret == NET_OK) && (tls_data->tls_dev_cert != NULL) && (tls_data->tls_dev_key != NULL))
  {
    ret = mbedtls_x509_crt_parse(&tls_data->clicert, (uchar_t const *) tls_data->tls_dev_cert,
                                 strlen((char_t const *)tls_data->tls_dev_cert) + 1U);
    if (ret != 0)
    {
      NET_DBG_ERROR("Failed!\n mbedtls_x509_crt_parse returned -0x%" PRIx32 " while parsing device certificate\n",
                    (uint32_t)(-ret));
      mbedtls_free_resource(sock);
      ret = NET_ERROR_MBEDTLS_CRT_PARSE;
    }
    else
    {
      ret = mbedtls_pk_parse_key(&tls_data->pkey, (uchar_t const *)tls_data->tls_dev_key,
                                 strlen((char_t const *)tls_data->tls_dev_key) + 1U,
                                 (uchar_t const *)tls_data->tls_dev_pwd, tls_data->tls_dev_pwd_len);
      if (ret != 0)
      {
        NET_DBG_ERROR("Failed!\n mbedtls_pk_parse_key returned -0x%" PRIx32 " while parsing private key\n\n", (uint32_t)(-ret));
        mbedtls_free_resource(sock);
        ret = NET_ERROR_MBEDTLS_KEY_PARSE;
      }
    }
  }

  /* TLS Connection */
  if (ret == NET_OK)
  {
    ret = mbedtls_ssl_config_defaults(&tls_data->conf, MBEDTLS_SSL_IS_CLIENT, MBEDTLS_SSL_TRANSPORT_STREAM,
                                      MBEDTLS_SSL_PRESET_DEFAULT);
    if (ret != 0)
    {
      NET_DBG_ERROR("Failed!\n mbedtls_ssl_config_defaults returned -0x%" PRIu32 "\n\n", (uint32_t)(-ret));
      mbedtls_free_resource(sock);
      ret = NET_ERROR_MBEDTLS_CONFIG;
    }
  }

  if (ret == NET_OK)
  {
    /* Allow the user to select a TLS profile? */
    if (tls_data->tls_cert_prof != NULL)
    {
      mbedtls_ssl_conf_cert_profile(&tls_data->conf, tls_data->tls_cert_prof);
    }
    /* Only for debug  mbedtls_ssl_conf_verify  _iot_tls_verify_cert  NULL */
    if (tls_data->tls_srv_verification == true)
    {
      mbedtls_ssl_conf_authmode(&tls_data->conf, MBEDTLS_SSL_VERIFY_REQUIRED);
    }
    else
    {
      mbedtls_ssl_conf_authmode(&tls_data->conf, MBEDTLS_SSL_VERIFY_OPTIONAL);
    }

    mbedtls_ssl_conf_rng(&tls_data->conf, (mbedtls_rng_func_t) mbedtls_rng_raw, &hrng);
    mbedtls_ssl_conf_ca_chain(&tls_data->conf, &tls_data->cacert, NULL);

    if ((tls_data->tls_dev_cert != NULL) && (tls_data->tls_dev_key != NULL))
    {
      ret = mbedtls_ssl_conf_own_cert(&tls_data->conf, &tls_data->clicert, &tls_data->pkey);
      if (ret != 0)
      {
        NET_DBG_ERROR("Failed!\n mbedtls_ssl_conf_own_cert returned -0x%" PRIx32 "\n\n", (uint32_t)(-ret));
        mbedtls_free_resource(sock);
        ret = NET_ERROR_MBEDTLS_CONFIG;
      }
    }
  }
  if (ret == NET_OK)
  {
    ret = mbedtls_ssl_setup(&tls_data->ssl, &tls_data->conf);
    if (ret != 0)
    {
      NET_DBG_ERROR("Failed!\n mbedtls_ssl_setup returned -0x%" PRIx32 "\n\n", (uint32_t)(-ret));
      mbedtls_free_resource(sock);
      ret = NET_ERROR_MBEDTLS_SSL_SETUP;
    }
  }

  if ((ret == NET_OK) && (tls_data->tls_srv_name != NULL))
  {
    ret = mbedtls_ssl_set_hostname(&tls_data->ssl, (char_t const *)tls_data->tls_srv_name);
    if (ret  != 0)
    {
      NET_DBG_ERROR("Failed!\n mbedtls_ssl_set_hostname returned %" PRId32 "\n\n", ret);
      mbedtls_free_resource(sock);
      ret = NET_ERROR_MBEDTLS_SET_HOSTNAME;
    }
  }

  if (ret == NET_OK)
  {
    mbedtls_ssl_set_bio(&tls_data->ssl, sock, (mbedtls_ssl_send_t *) mbedtls_net_send, NULL,
                        (mbedtls_ssl_recv_timeout_t *) mbedtls_net_recv);
    mbedtls_ssl_conf_read_timeout(&tls_data->conf, (uint32_t)sock->read_timeout);

    NET_DBG_INFO("\n\nSSL state connect: %d", sock->tls_data->ssl.state);
    NET_DBG_INFO(" . Performing the SSL/TLS handshake...");

    ret = mbedtls_ssl_handshake(&tls_data->ssl);
    start_tick = NET_TICK();
    while (ret != 0)
    {
      uint32_t elapsed_tick = NET_TICK() - start_tick;

      if (elapsed_tick > NET_MBEDTLS_CONNECT_TIMEOUT)
      {
        mbedtls_free_resource(sock);
        ret = NET_ERROR_MBEDTLS_CONNECT;
        break;
      }

      if ((ret != MBEDTLS_ERR_SSL_WANT_READ) && (ret != MBEDTLS_ERR_SSL_WANT_WRITE))
      {
        tls_data->flags = mbedtls_ssl_get_verify_result(&tls_data->ssl);
        if (tls_data->flags != 0U)
        {
          char_t verify_buf[512];
          (void) mbedtls_x509_crt_verify_info(verify_buf, sizeof(verify_buf), "  ! ", tls_data->flags);
          if (tls_data->tls_srv_verification == true)
          {
            NET_DBG_ERROR("Server verification:\n%s\n", verify_buf);
          }
          else
          {
            NET_DBG_INFO("Server verification:\n%s\n", verify_buf);
          }
        }
        NET_DBG_ERROR("Failed!\n mbedtls_ssl_handshake returned -0x%" PRIx32 "\n", (uint32_t)(-ret));

        mbedtls_free_resource(sock);
        ret = (ret == MBEDTLS_ERR_X509_CERT_VERIFY_FAILED) ? NET_ERROR_MBEDTLS_REMOTE_AUTH : NET_ERROR_MBEDTLS_CONNECT;
        break;
      }
      ret = mbedtls_ssl_handshake(&tls_data->ssl);
    }

    if (ret == NET_OK)
    {
      int32_t exp;
      NET_DBG_INFO(" OK\n [Protocol is %s]\n [Ciphersuite is %s]\n",
                   mbedtls_ssl_get_version(&sock->tls_data->ssl),
                   mbedtls_ssl_get_ciphersuite(&sock->tls_data->ssl));

      exp = mbedtls_ssl_get_record_expansion(&tls_data->ssl);
      if (exp >= 0)
      {
        NET_DBG_INFO("    [Record expansion is %d]\n", exp);
      }
      else
      {
        NET_DBG_INFO("    [Record expansion is unknown (compression)]\n");
      }

      NET_DBG_INFO("  . Verifying peer X.509 certificate...");


#ifdef NET_DBG_INFO
#define NET_CERTIFICATE_DISPLAY_LEN     2048U
      if (mbedtls_ssl_get_peer_cert(&sock->tlsData->ssl) != NULL)
      {
        char_t *buf = NET_MALLOC(sizeof(char_t) * NET_CERTIFICATE_DISPLAY_LEN);

        if (buf != NULL)
        {
          NET_DBG_INFO("  . Peer certificate information ...\n");
          (void) mbedtls_x509_crt_info(buf, NET_CERTIFICATE_DISPLAY_LEN - 1U, "      ",
                                       mbedtls_ssl_get_peer_cert(&sock->tlsData->ssl));
          NET_DBG_INFO("%s\n", buf);
          NET_FREE(buf);
        }
        else
        {
          NET_DBG_INFO(" . Cannot allocate memory to display certificate information ...\n");
        }
      }
#endif /* NET_DBG_INFO */
    }
  }
  return ret;

}


int32_t net_mbedtls_sock_recv(net_socket_t *sock, uint8_t *buf, size_t len)
{
  net_tls_data_t *const tls_data = sock->tlsData;
  int32_t ret;

  ret = mbedtls_ssl_read(&tls_data->ssl, buf, len);

  if (ret <= 0)
  {
    switch (ret)
    {
      case 0:
        ret = NET_ERROR_DISCONNECTED;
        break;

      case MBEDTLS_ERR_SSL_TIMEOUT: /* In case a blocking read function was passed through mbedtls_ssl_set_bio() */
        ret = NET_TIMEOUT;
        break;

      case MBEDTLS_ERR_SSL_WANT_READ:
        ret = NET_TIMEOUT;
        break;

      default:
        NET_DBG_ERROR("Failed!\n mbedtls_ssl_read returned -0x%" PRIx32 "\n\n", (uint32_t)(-ret));
        ret = NET_ERROR_MBEDTLS;
        break;
    }
  }
  return ret;
}


int32_t net_mbedtls_sock_send(net_socket_t *sock, const uint8_t *buf, size_t len)
{
  int32_t ret;
  net_tls_data_t *const tls_data = sock->tlsData;

#if PERF
  stat.mbedtls_send_cycle -= net_get_cycle();
#endif /* PERF */

  ret = mbedtls_ssl_write(&tls_data->ssl, buf, len);
  if (ret == 0)
  {
    ret = NET_ERROR_DISCONNECTED;
  }
  else if (ret < 0)
  {
    NET_DBG_ERROR("Failed!\n mbedtls_ssl_write returned -0x%" PRIx32 "\n\n", (uint32_t)(-ret));
    ret = NET_ERROR_MBEDTLS;
  }
  else
  {
    /* ret > 0 nothing to do, MISRA checks */
  }

#if PERF
  stat.mbedtls_send_cycle += net_get_cycle();
#endif /* PERF */

  return (ret);
}


int32_t net_mbedtls_stop(net_socket_t *sock)
{
  int32_t ret;
  /* Closure notification is required by TLS if the session was not already closed by the remote host. */
  net_tls_data_t *const tls_data = sock->tlsData;
  do
  {
    ret = mbedtls_ssl_close_notify(&tls_data->ssl);
  } while ((ret == MBEDTLS_ERR_SSL_WANT_WRITE) || (ret == MBEDTLS_ERR_SSL_WANT_READ));

  /* All other negative return values indicate connection needs to be reset.
   * No further action required since this is disconnect call */

  mbedtls_free_resource(sock);
  return NET_OK;
}


static void mbedtls_free_resource(net_socket_t *sock)
{
  net_tls_data_t *const tls_data = sock->tlsData;

  mbedtls_x509_crt_free(&tls_data->clicert);
  mbedtls_pk_free(&tls_data->pkey);
  mbedtls_x509_crt_free(&tls_data->cacert);
  mbedtls_ssl_free(&tls_data->ssl);
  mbedtls_ssl_config_free(&tls_data->conf);
  NET_FREE(tls_data);
  sock->tlsData = 0;
  return;
}



/* received interface implementation.*/
static int32_t mbedtls_net_recv(void *ctx, uchar_t *buf, size_t len, uint32_t timeout)
{
  int32_t ret = NET_OK;
  int32_t flags = 0;
  net_socket_t *const p_socket = (net_socket_t *) ctx;

  if (p_socket->read_timeout != (int32_t)timeout)
  {
    ret = p_socket->pnetif->pdrv->psetsockopt(p_socket->ulsocket,
                                              NET_SOL_SOCKET, NET_SO_RCVTIMEO, &timeout, sizeof(uint32_t));
    if (ret == NET_OK)
    {
      p_socket->read_timeout = (int32_t) timeout;
    }
  }

  if (ret == NET_OK)
  {
    if (p_socket->read_timeout == 0)
    {
      flags = (int8_t) NET_MSG_DONTWAIT;
    }
    UNLOCK_SOCK(p_socket->idx);
    ret = p_socket->pnetif->pdrv->precv(p_socket->ulsocket, buf, len, flags);
    LOCK_SOCK(p_socket->idx);

    if (ret <= 0)
    {
      switch (ret)
      {
        case 0:
          ret = MBEDTLS_ERR_SSL_WANT_READ;
          break;

        case NET_TIMEOUT:
          /* According to mbedtls headers, MBEDTLS_ERR_SSL_TIMEOUT should be returned. */
          /* But it saturates the error log with false errors. By contrast,            */
          /* MBEDTLS_ERR_SSL_WANT_READ does not raise any error.                       */
          ret =  MBEDTLS_ERR_SSL_WANT_READ;
          break;

        default:
          NET_DBG_ERROR("mbedtls_net_recv(): error %" PRId32 " in recv() - requestedLen=%" PRIu32 "\n", ret, (uint32_t)len);
          ret = MBEDTLS_ERR_SSL_INTERNAL_ERROR;
          break;
      }
    }
  }

  return ret;
}


static int32_t mbedtls_net_send(void *ctx, const uchar_t *buf, size_t len)
{
  net_socket_t *const p_socket = (net_socket_t *) ctx;
  int32_t ret;
  int32_t flags = 0;

  if (p_socket->write_timeout == 0)
  {
    flags = (int8_t) NET_MSG_DONTWAIT;
  }

  UNLOCK_SOCK(p_socket->idx);
  ret = p_socket->pnetif->pdrv->psend(p_socket->ulsocket, (uchar_t *) buf, len, flags);
  LOCK_SOCK(p_socket->idx);

  if (ret >= 0)
  {
    if (ret == 0)
    {
      ret = MBEDTLS_ERR_SSL_WANT_WRITE;
    }
  }
  else
  {
    NET_DBG_ERROR("mbedtls_net_send(): error %" PRIu32 " in send() - requestedLen=%" PRId32 "\n", ret, (uint32_t)len);

    /** The underlying layers do not allow to distinguish between
      *          MBEDTLS_ERR_SSL_INTERNAL_ERROR,
      *          MBEDTLS_ERR_SSL_PEER_CLOSE_NOTIFY,
      *          MBEDTLS_ERR_SSL_CONN_EOF.
      * Most often, the error is due to the closure of the connection by the remote host.
      */

    ret =  MBEDTLS_ERR_SSL_PEER_CLOSE_NOTIFY;
  }
  return ret;
}

#endif /* NET_MBEDTLS_HOST_SUPPORT */
