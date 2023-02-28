/*
 * Copyright (c) 2020-2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __TFM_HAL_ISOLATION_H__
#define __TFM_HAL_ISOLATION_H__

#include <stddef.h>
#include <stdint.h>
#include "fih.h"
#include "tfm_hal_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Memory access attributes */
#define TFM_HAL_ACCESS_EXECUTABLE       (1UL << 0)
#define TFM_HAL_ACCESS_READABLE         (1UL << 1)
#define TFM_HAL_ACCESS_WRITABLE         (1UL << 2)
#define TFM_HAL_ACCESS_UNPRIVILEGED     (1UL << 3)
#define TFM_HAL_ACCESS_DEVICE           (1UL << 4)
#define TFM_HAL_ACCESS_NS               (1UL << 5)

#ifdef TFM_FIH_PROFILE_ON
/**
 * \brief  Sets up the static isolation boundaries which are constant throughout
 *         the runtime of the system, including the SPE/NSPE and partition
 *         boundaries.
 *
 * \return TFM_HAL_SUCCESS - the isolation boundaries have been set up.
 *         TFM_HAL_ERROR_GENERIC - failed to set up the isolation boundaries.
 */
fih_int tfm_hal_set_up_static_boundaries(void);

#if TFM_LVL == 3
/**
 * \brief  Updates the partition isolation boundary for isolation level 3.
 *         The boundary protects the private data of the running partition.
 *         The boundary is updated with SPM switching partition in level 3.
 *
 * \param[in] start     start address of the partition boundary.
 * \param[in] end       end address of the partition boundary.
 *
 * \return TFM_HAL_SUCCESS - the isolation boundary has been set up.
 *         TFM_HAL_ERROR_GENERIC - failed to set up the isolation boundary.
 *
 * \note   When FIH_ENABLE_DOUBLE_VARS is enabled, the return code will be
 *         wrapped and protected in \ref fih_int structure.
 */
fih_int tfm_hal_mpu_update_partition_boundary(uintptr_t start,
                                              uintptr_t end);
#endif
#else /* TFM_FIH_PROFILE_ON */
/**
 * \brief  Sets up the static isolation boundaries which are constant throughout
 *         the runtime of the system, including the SPE/NSPE and partition
 *         boundaries.
 *
 * \return TFM_HAL_SUCCESS - the isolation boundaries have been set up.
 *         TFM_HAL_ERROR_GENERIC - failed to set up the isolation boundaries.
 *
 * \note   When FIH_ENABLE_DOUBLE_VARS is enabled, the return code will be
 *         wrapped and protected in \ref fih_int structure.
 */
enum tfm_hal_status_t tfm_hal_set_up_static_boundaries(void);

#if TFM_LVL == 3
/**
 * \brief  Updates the partition isolation boundary for isolation level 3.
 *         The boundary protects the private data of the running partition.
 *         The boundary is updated with SPM switching partition in level 3.
 *
 * \param[in] start     start address of the partition boundary.
 * \param[in] end       end address of the partition boundary.
 *
 * \return TFM_HAL_SUCCESS - the isolation boundary has been set up.
 *         TFM_HAL_ERROR_GENERIC - failed to set up the isolation boundary.
 *
 * \note   When FIH_ENABLE_DOUBLE_VARS is enabled, the return code will be
 *         wrapped and protected in \ref fih_int structure.
 */
enum tfm_hal_status_t tfm_hal_mpu_update_partition_boundary(uintptr_t start,
                                                            uintptr_t end);
#endif
#endif /* TFM_FIH_PROFILE_ON */

/**
 * \brief  This API checks if the memory region defined by base and size
 *         matches the given attributes - attr.
 *         The attributes can include NSPE access, privileged mode, and
 *         read-write permissions.
 *
 * \param[in]   base    The base address of the region.
 * \param[in]   size    The size of the region.
 * \param[in]   attr    The memory access attributes.
 *
 * \return TFM_HAL_SUCCESS - The memory region has the access permissions.
 *         TFM_HAL_ERROR_MEM_FAULT - The memory region has not the access
 *                                   permissions.
 *         TFM_HAL_ERROR_INVALID_INPUT - Invalid inputs.
 *         TFM_HAL_ERROR_GENERIC - An error occurred.
 */
#ifdef TFM_FIH_PROFILE_ON
fih_int tfm_hal_memory_has_access(uintptr_t base,
                                  size_t size,
                                  uint32_t attr);
#else
enum tfm_hal_status_t tfm_hal_memory_has_access(uintptr_t base,
                                                size_t size,
                                                uint32_t attr);
#endif

#ifdef __cplusplus
}
#endif

#endif /* __TFM_HAL_ISOLATION_H__ */
