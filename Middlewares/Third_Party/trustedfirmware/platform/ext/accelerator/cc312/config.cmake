#-------------------------------------------------------------------------------
# Copyright (c) 2020, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

############ Override defaults for CC312 Accelerator ##############################

# The CC312 needs atleast 0x4000 of heap size to work properly
if(NOT (CRYPTO_ENGINE_BUF_SIZE GREATER 0x4000))
    set(CRYPTO_ENGINE_BUF_SIZE              0x4000      CACHE STRING    "Heap size for the crypto backend")
endif()
