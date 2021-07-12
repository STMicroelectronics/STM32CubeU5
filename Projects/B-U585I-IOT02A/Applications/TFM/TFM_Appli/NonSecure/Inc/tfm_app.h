/**
  ******************************************************************************
  * @file    tfm_app.h
  * @author  MCD Application Team
  * @brief   This file contains definitions for tkms application examples.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef TFM_APP_H
#define TFM_APP_H

#ifdef __cplusplus
extern "C" {
#endif


#define INVOKE_SCHEDULE_NEEDS()                                                \
      do {                                                                     \
        ;                                                                      \
      } while(0);



/* Exported prototypes ------------------------------------------------------ */
void tfm_app_menu(void);

#ifdef __cplusplus
}
#endif

#endif /* TFM_APP_H */
