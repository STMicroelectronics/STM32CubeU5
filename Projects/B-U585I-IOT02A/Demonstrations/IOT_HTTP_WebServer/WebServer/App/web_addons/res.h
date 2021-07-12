/**
  **********************************************************************************************************************
  * @file    res.h
  * @author  MCD Application Team
  * @brief   This file implements the web page resources
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

/* Define to prevent recursive inclusion -----------------------------------------------------------------------------*/
#ifndef RES_H
#define RES_H

/* Exported variables ------------------------------------------------------------------------------------------------*/
/* Web page main css resources buffer */
extern const char     css_main_buff[];
extern const uint32_t css_main_buff_size;

/* Web page shunk css resources buffer */
extern const char     css_shunk_buff[];
extern const uint32_t css_shunk_buff_size;

/* Web page favicon resources buffer */
extern const char     favicon_buff[];
extern const uint32_t favicon_buff_size;

/* Web page font resources buffer */
extern const char     font_buff[];
extern const uint32_t font_buff_size;

/* Web page html resources buffer */
extern const char     html_buff[];
extern const uint32_t html_buff_size;

/* Web page html image buffer */
extern const char     image_buff[];
extern const uint32_t image_buff_size;

/* Web page main java script resources buffer */
extern const char     js_main_buff[];
extern const uint32_t js_main_buff_size;

/* Web page shunk java script resources buffer */
extern const char     js_shunk_buff[];
extern const uint32_t js_shunk_buff_size;

/* Web page json resources buffer */
extern const char     json_buff[];
extern const uint32_t json_buff_size;

#endif /* RES_H */
