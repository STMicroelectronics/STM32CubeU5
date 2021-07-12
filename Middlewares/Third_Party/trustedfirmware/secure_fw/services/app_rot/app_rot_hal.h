/*
 * Copyright (c) 2018, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
#ifndef APP_ROT_HAL_H
#define APP_ROT_HAL_H

#include "tfm_client.h"
#include "tfm_secure_api.h"
#include "tfm_app_rot_api.h"


enum tfm_app_rot_err_t app_rot_hal_ioctl(tfm_app_rot_ioctl_req_t request,
                                               psa_invec *in_vec,
                                               psa_outvec *out_vec);

#endif /* APP_ROT_HAL */