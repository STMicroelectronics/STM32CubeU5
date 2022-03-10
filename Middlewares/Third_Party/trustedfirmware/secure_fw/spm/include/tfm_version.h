/*
 * Copyright (c) 2020-2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __TFM_VERSION_H__
#define __TFM_VERSION_H__

/*
 * Defines for TFM version.
 */
#define VERSION_MAJOR    1
#define VERSION_MINOR    3
#define VERSION_HOTFIX   0
#define VERSION_STRING   ""

#define VERSTR(x)      #x
#define VERCON(major, minor, hotfix) VERSTR(major)"."VERSTR(minor)"."VERSTR(hotfix)

#define VERSION_FULLSTR VERCON(VERSION_MAJOR, VERSION_MINOR, VERSION_HOTFIX)""VERSION_STRING



#endif /* __TFM_VERSION_H__ */
