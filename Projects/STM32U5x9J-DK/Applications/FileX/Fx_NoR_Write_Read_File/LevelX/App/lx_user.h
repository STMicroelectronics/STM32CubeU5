/**************************************************************************/
/*                                                                        */
/*       Copyright (c) Microsoft Corporation. All rights reserved.        */
/*                                                                        */
/*       This software is licensed under the Microsoft Software License   */
/*       Terms for Microsoft Azure RTOS. Full text of the license can be  */
/*       found in the LICENSE file at https://aka.ms/AzureRTOS_EULA       */
/*       and in the root directory of this software.                      */
/*                                                                        */
/**************************************************************************/

/**************************************************************************/
/**************************************************************************/
/**                                                                       */
/** LevelX Component                                                      */
/**                                                                       */
/**   User Specific                                                       */
/**                                                                       */
/**************************************************************************/
/**************************************************************************/

/**************************************************************************/
/*                                                                        */
/*  PORT SPECIFIC C INFORMATION                            RELEASE        */
/*                                                                        */
/*    lx_user.h                                           PORTABLE C      */
/*                                                           6.1.7        */
/*                                                                        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    William E. Lamie, Microsoft Corporation                             */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This file contains user defines for configuring LevelX in specific  */
/*    ways. This file will have an effect only if the application and     */
/*    LevelX library are built with LX_INCLUDE_USER_DEFINE_FILE defined.  */
/*    Note that all the defines in this file may also be made on the      */
/*    command line when building LevelX library and application objects.  */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  11-09-2020     William E. Lamie         Initial Version 6.1.2         */
/*  06-02-2021     Bhupendra Naphade        Modified comment(s), and      */
/*                                            added standalone support,   */
/*                                            resulting in version 6.1.7  */
/*                                                                        */
/**************************************************************************/

#ifndef LX_USER_H
#define LX_USER_H

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/* #define LX_DIRECT_READ */

/* #define LX_FREE_SECTOR_DATA_VERIFY */

/* #define LX_NOR_DISABLE_EXTENDED_CACHE */

/* #define LX_NOR_EXTENDED_CACHE_SIZE         8 */

/* #define LX_NOR_SECTOR_MAPPING_CACHE_SIZE         16 */

/* Configure the LevelX in Standalone mode. */

/* #define LX_STANDALONE_ENABLE */

/* #define LX_THREAD_SAFE_ENABLE */

/* USER CODE BEGIN 2 */

/* USER CODE END 2 */

#endif
