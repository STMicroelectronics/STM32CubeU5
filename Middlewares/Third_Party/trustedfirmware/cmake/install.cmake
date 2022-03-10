#-------------------------------------------------------------------------------
# Copyright (c) 2020-2021, Arm Limited. All rights reserved.
# Copyright (c) 2020, Cypress Semiconductor Corporation. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

# Generate correct filename
string(TOUPPER ${TFM_PLATFORM} TFM_PLATFORM_UPPERCASE)
string(REGEX REPLACE "-" "_" TFM_PLATFORM_UPPERCASE_UNDERSCORE ${TFM_PLATFORM_UPPERCASE})
string(REGEX REPLACE "^(\\.\\.([\\/\\\\]))+" "EXTERNAL\\2" TFM_PLATFORM_UPPERCASE_UNDERSCORE_NO_PARENT_DIR ${TFM_PLATFORM_UPPERCASE_UNDERSCORE})

install(DIRECTORY ${CMAKE_BINARY_DIR}/bin/
        DESTINATION ${TFM_INSTALL_PATH}/outputs/${TFM_PLATFORM_UPPERCASE_UNDERSCORE_NO_PARENT_DIR}
)

set(INTERFACE_INC_DIR ${CMAKE_SOURCE_DIR}/interface/include)
set(INTERFACE_SRC_DIR ${CMAKE_SOURCE_DIR}/interface/src)

set(INSTALL_INTERFACE_INC_DIR    ${TFM_INSTALL_PATH}/interface/include)
set(INSTALL_INTERFACE_SRC_DIR    ${TFM_INSTALL_PATH}/interface/src)
set(INSTALL_INTERFACE_LIB_DIR    ${TFM_INSTALL_PATH}/interface/lib)

# export veneer lib
if (NOT TFM_MULTI_CORE_TOPOLOGY)
    install(FILES       ${CMAKE_BINARY_DIR}/secure_fw/s_veneers.o
            DESTINATION ${INSTALL_INTERFACE_LIB_DIR})
endif()

####################### export headers #########################################

install(FILES       ${INTERFACE_INC_DIR}/psa/client.h
                    ${INTERFACE_INC_DIR}/psa/error.h
        DESTINATION ${INSTALL_INTERFACE_INC_DIR}/psa)

install(FILES       ${INTERFACE_INC_DIR}/os_wrapper/common.h
                    ${INTERFACE_INC_DIR}/os_wrapper/msg_queue.h
                    ${INTERFACE_INC_DIR}/os_wrapper/mutex.h
                    ${INTERFACE_INC_DIR}/os_wrapper/semaphore.h
                    ${INTERFACE_INC_DIR}/os_wrapper/thread.h
                    ${INTERFACE_INC_DIR}/os_wrapper/tick.h
        DESTINATION ${INSTALL_INTERFACE_INC_DIR}/os_wrapper)

install(FILES       ${CMAKE_BINARY_DIR}/generated/interface/include/psa_manifest/sid.h
        DESTINATION ${INSTALL_INTERFACE_INC_DIR}/psa_manifest)

install(FILES       ${INTERFACE_INC_DIR}/tfm_api.h
                    ${INTERFACE_INC_DIR}/tfm_ns_interface.h
                    ${INTERFACE_INC_DIR}/tfm_ns_svc.h
        DESTINATION ${INSTALL_INTERFACE_INC_DIR})

install(FILES       ${INTERFACE_INC_DIR}/ext/tz_context.h
        DESTINATION ${INSTALL_INTERFACE_INC_DIR}/ext)

if (TFM_MULTI_CORE_TOPOLOGY)
    install(FILES       ${INTERFACE_INC_DIR}/multi_core/tfm_multi_core_api.h
                        ${INTERFACE_INC_DIR}/multi_core/tfm_ns_mailbox.h
                        ${INTERFACE_INC_DIR}/multi_core/tfm_mailbox.h
                        ${INTERFACE_INC_DIR}/multi_core/tfm_mailbox_config.h
            DESTINATION ${INSTALL_INTERFACE_INC_DIR})
else()
    install(FILES       ${CMAKE_BINARY_DIR}/generated/interface/include/tfm_veneers.h
            DESTINATION ${INSTALL_INTERFACE_INC_DIR}/tfm/veneers)
endif()

if (TFM_NS_CLIENT_IDENTIFICATION)
    install(FILES       ${INTERFACE_INC_DIR}/tfm_nspm_api.h
                        ${INTERFACE_INC_DIR}/tfm_nspm_svc_handler.h
            DESTINATION ${INSTALL_INTERFACE_INC_DIR})
endif()

if (TFM_PARTITION_PROTECTED_STORAGE OR FORWARD_PROT_MSG)
    install(FILES       ${INTERFACE_INC_DIR}/psa/protected_storage.h
            DESTINATION ${INSTALL_INTERFACE_INC_DIR}/psa)
endif()

if (TFM_PARTITION_INTERNAL_TRUSTED_STORAGE OR FORWARD_PROT_MSG)
    install(FILES       ${INTERFACE_INC_DIR}/psa/internal_trusted_storage.h
                        ${INTERFACE_INC_DIR}/psa/storage_common.h
            DESTINATION ${INSTALL_INTERFACE_INC_DIR}/psa)
endif()

if (TFM_PARTITION_CRYPTO OR FORWARD_PROT_MSG)
    install(FILES       ${INTERFACE_INC_DIR}/psa/crypto_extra.h
                        ${INTERFACE_INC_DIR}/psa/crypto_compat.h
                        ${INTERFACE_INC_DIR}/psa/crypto.h
                        ${INTERFACE_INC_DIR}/psa/crypto_client_struct.h
                        ${INTERFACE_INC_DIR}/psa/crypto_sizes.h
                        ${INTERFACE_INC_DIR}/psa/crypto_struct.h
                        ${INTERFACE_INC_DIR}/psa/crypto_types.h
                        ${INTERFACE_INC_DIR}/psa/crypto_values.h
            DESTINATION ${INSTALL_INTERFACE_INC_DIR}/psa)
    install(FILES       ${INTERFACE_INC_DIR}/tfm_crypto_defs.h
            DESTINATION ${INSTALL_INTERFACE_INC_DIR})
endif()

if (TFM_PARTITION_INITIAL_ATTESTATION OR FORWARD_PROT_MSG)
    install(FILES       ${INTERFACE_INC_DIR}/psa/initial_attestation.h
            DESTINATION ${INSTALL_INTERFACE_INC_DIR}/psa)
endif()

if(TFM_PARTITION_AUDIT_LOG)
    install(FILES       ${INTERFACE_INC_DIR}/psa_audit_api.h
                        ${INTERFACE_INC_DIR}/psa_audit_defs.h
            DESTINATION ${INSTALL_INTERFACE_INC_DIR})
endif()

if(TFM_PARTITION_PLATFORM OR FORWARD_PROT_MSG)
    install(FILES       ${INTERFACE_INC_DIR}/tfm_platform_api.h
            DESTINATION ${INSTALL_INTERFACE_INC_DIR})
endif()

if(TFM_PARTITION_FIRMWARE_UPDATE)
    install(FILES       ${INTERFACE_INC_DIR}/psa/update.h
            DESTINATION ${INSTALL_INTERFACE_INC_DIR}/psa)
    install(FILES       ${INTERFACE_INC_DIR}/tfm_fwu_defs.h
            DESTINATION ${INSTALL_INTERFACE_INC_DIR})
endif()

####################### export sources #########################################

if (TFM_MULTI_CORE_TOPOLOGY)
    install(FILES       ${INTERFACE_SRC_DIR}/multi_core/tfm_ns_mailbox.c
                        ${INTERFACE_SRC_DIR}/multi_core/tfm_multi_core_ns_api.c
                        ${INTERFACE_SRC_DIR}/multi_core/tfm_multi_core_psa_ns_api.c
                        ${INTERFACE_SRC_DIR}/multi_core/tfm_ns_mailbox_rtos_api.c
                        ${INTERFACE_SRC_DIR}/multi_core/tfm_ns_mailbox_thread.c
                        ${INTERFACE_SRC_DIR}/multi_core/tfm_ns_mailbox_test.c
            DESTINATION ${INSTALL_INTERFACE_SRC_DIR})
else()
    install(FILES       ${INTERFACE_SRC_DIR}/tfm_ns_interface.c
            DESTINATION ${INSTALL_INTERFACE_SRC_DIR})

    if(TFM_PSA_API)
        install(FILES       ${INTERFACE_SRC_DIR}/tfm_psa_ns_api.c
                DESTINATION ${INSTALL_INTERFACE_SRC_DIR})
    endif()
endif()

if (TFM_NS_CLIENT_IDENTIFICATION)
    install(FILES       ${INTERFACE_SRC_DIR}/tfm_nspm_api.c
                        ${INTERFACE_SRC_DIR}/tfm_nspm_svc_handler.c
            DESTINATION ${INSTALL_INTERFACE_SRC_DIR})
endif()

if (TFM_PARTITION_PROTECTED_STORAGE OR FORWARD_PROT_MSG)
    if (TFM_PSA_API)
        install(FILES       ${INTERFACE_SRC_DIR}/tfm_ps_ipc_api.c
                DESTINATION ${INSTALL_INTERFACE_SRC_DIR})
    else()
        install(FILES       ${INTERFACE_SRC_DIR}/tfm_ps_func_api.c
                DESTINATION ${INSTALL_INTERFACE_SRC_DIR})
    endif()
endif()

if (TFM_PARTITION_INTERNAL_TRUSTED_STORAGE OR FORWARD_PROT_MSG)
    if (TFM_PSA_API)
        install(FILES       ${INTERFACE_SRC_DIR}/tfm_its_ipc_api.c
                DESTINATION ${INSTALL_INTERFACE_SRC_DIR})
    else()
        install(FILES       ${INTERFACE_SRC_DIR}/tfm_its_func_api.c
                DESTINATION ${INSTALL_INTERFACE_SRC_DIR})
    endif()
endif()

if (TFM_PARTITION_CRYPTO OR FORWARD_PROT_MSG)
    if (TFM_PSA_API)
        install(FILES       ${INTERFACE_SRC_DIR}/tfm_crypto_ipc_api.c
                DESTINATION ${INSTALL_INTERFACE_SRC_DIR})
    else()
        install(FILES       ${INTERFACE_SRC_DIR}/tfm_crypto_func_api.c
                DESTINATION ${INSTALL_INTERFACE_SRC_DIR})
    endif()
endif()

if (TFM_PARTITION_INITIAL_ATTESTATION OR FORWARD_PROT_MSG)
    if (TFM_PSA_API)
        install(FILES       ${INTERFACE_SRC_DIR}/tfm_initial_attestation_ipc_api.c
                DESTINATION ${INSTALL_INTERFACE_SRC_DIR})
    else()
        install(FILES       ${INTERFACE_SRC_DIR}/tfm_initial_attestation_func_api.c
                DESTINATION ${INSTALL_INTERFACE_SRC_DIR})
    endif()
endif()

if(TFM_PARTITION_AUDIT_LOG)
    install(FILES       ${INTERFACE_SRC_DIR}/tfm_audit_func_api.c
            DESTINATION ${INSTALL_INTERFACE_SRC_DIR})
endif()

if(TFM_PARTITION_PLATFORM OR FORWARD_PROT_MSG)
    if(TFM_PSA_API)
        install(FILES       ${INTERFACE_SRC_DIR}/tfm_platform_ipc_api.c
                DESTINATION ${INSTALL_INTERFACE_SRC_DIR})
    else()
        install(FILES       ${INTERFACE_SRC_DIR}/tfm_platform_func_api.c
                DESTINATION ${INSTALL_INTERFACE_SRC_DIR})
    endif()
endif()


##################### Export image signing information #########################

set(INSTALL_IMAGE_SIGNING_DIR ${TFM_INSTALL_PATH}/image_signing)

if(BL2)
    install(DIRECTORY bl2/ext/mcuboot/scripts
            DESTINATION ${INSTALL_IMAGE_SIGNING_DIR})

    if (MCUBOOT_ENC_IMAGES)
        install(FILES ${MCUBOOT_KEY_ENC}
                DESTINATION ${INSTALL_IMAGE_SIGNING_DIR}/keys)
    endif()

    install(FILES $<TARGET_OBJECTS:signing_layout_s>
            DESTINATION ${INSTALL_IMAGE_SIGNING_DIR}/layout_files)

    if(MCUBOOT_IMAGE_NUMBER GREATER 1)
        install(FILES $<TARGET_OBJECTS:signing_layout_ns>
                DESTINATION ${INSTALL_IMAGE_SIGNING_DIR}/layout_files)

        install(FILES ${MCUBOOT_KEY_NS}
                DESTINATION ${INSTALL_IMAGE_SIGNING_DIR}/keys)
    else()
        install(FILES ${MCUBOOT_KEY_S}
                DESTINATION ${INSTALL_IMAGE_SIGNING_DIR}/keys)
    endif()
endif()

if(TFM_PARTITION_FIRMWARE_UPDATE)
    if(TFM_PSA_API)
        install(FILES       ${INTERFACE_SRC_DIR}/tfm_firmware_update_ipc_api.c
                DESTINATION ${INSTALL_INTERFACE_SRC_DIR})
    else()
        install(FILES       ${INTERFACE_SRC_DIR}/tfm_firmware_update_func_api.c
                DESTINATION ${INSTALL_INTERFACE_SRC_DIR})
    endif()
endif()
