#add command to generated the file
set "projectdir=%~dp0"
pushd %projectdir%\..\..\..\..\..\..\Middlewares\Third_Party\
set Third_Party=%cd%
popd
set "python=C:\Data\tools\Python\Python39-5\python "
set "TFM_TEST_PATH=%Third_Party%\trustedfirmware\test"
set "TFM_PATH=%Third_Party%\trustedfirmware"
set "TOOLS=%Third_Party%\trustedfirmware\tools"
::recopy config for generation 
set "CONFIG=%projectdir%\..\Secure\Src"
set "PARTITION=%Third_Party%\trustedfirmware\secure_fw\partitions"
::config from application
copy %CONFIG%\tfm_manifest_list.yaml %projectdir% /Y
copy %CONFIG%\tfm_generated_file_list.yaml %projectdir% /Y

::config platform customizeable
copy %CONFIG%\tfm_app_rot.yaml %projectdir% /Y
copy %CONFIG%\tfm_platform.yaml %projectdir% /Y

::config from middleware standard services
copy %PARTITION%\crypto\tfm_crypto.yaml %projectdir% /Y
copy %PARTITION%\protected_storage\tfm_protected_storage.yaml %projectdir% /Y
copy %PARTITION%\protected_storage\tfm_protected_storage.yaml %projectdir% /Y
copy %PARTITION%\internal_trusted_storage\tfm_internal_trusted_storage.yaml %projectdir% /Y
copy %PARTITION%\initial_attestation\tfm_initial_attestation.yaml %projectdir% /Y
copy %PARTITION%\firmware_update\tfm_firmware_update.yaml %projectdir% /Y
copy %PARTITION%\tfm_ffm11_partition\tfm_ffm11_partition.yaml %projectdir% /Y
::copy template file from middleware
copy %PARTITION%\manifestfilename.template %projectdir% /Y
copy %PARTITION%\partition_intermedia.template %projectdir% /Y
copy %PARTITION%\tfm_service_list.inc.template  %projectdir% /Y
copy %TFM_PATH%\interface\include\tfm_veneers.h.template %projectdir% /Y
copy %TFM_PATH%\secure_fw\spm\cmsis_psa\tfm_secure_irq_handlers_ipc.inc.template %projectdir% /Y
copy %TFM_PATH%\secure_fw\spm\cmsis_psa\tfm_spm_db_ipc.inc.template %projectdir% /Y
copy %TFM_PATH%\interface\include\psa_manifest\sid.h.template %projectdir% /Y
copy %TFM_PATH%\interface\include\psa_manifest\pid.h.template %projectdir% /Y
::copy config for test
copy %TFM_TEST_PATH%\test_services\tfm_core_test\tfm_ss_core_test.yaml %projectdir% /Y
copy %TFM_TEST_PATH%\test_services\tfm_core_test_2\tfm_ss_core_test_2.yaml %projectdir% /Y
copy %TFM_TEST_PATH%\test_services\tfm_secure_client_service\tfm_secure_client_service.yaml %projectdir% /Y
copy %TFM_TEST_PATH%\test_services\tfm_ipc_service\tfm_ipc_service_test.yaml %projectdir% /Y
copy %TFM_TEST_PATH%\test_services\tfm_secure_client_service\tfm_secure_client_service.yaml %projectdir% /Y
copy %TFM_TEST_PATH%\test_services\tfm_ipc_client\tfm_ipc_client_test.yaml %projectdir% /Y
copy %TFM_TEST_PATH%\test_services\tfm_ps_test_service\tfm_ps_test_service.yaml %projectdir% /Y
copy %TFM_TEST_PATH%\test_services\tfm_ipc_service\tfm_ipc_service_test.yaml %projectdir% /Y
copy %TFM_TEST_PATH%\test_services\tfm_ipc_client\tfm_ipc_client_test.yaml %projectdir% /Y
copy %TFM_TEST_PATH%\test_services\tfm_irq_test_service_1\tfm_irq_test_service_1.yaml %projectdir% /Y
copy %TFM_TEST_PATH%\test_services\tfm_ps_test_service\tfm_ps_test_service.yaml %projectdir% /Y
copy %TFM_TEST_PATH%\test_services\tfm_secure_client_2\tfm_secure_client_2.yaml %projectdir% /Y
copy %TFM_TEST_PATH%\test_services\tfm_core_test\tfm_core_test %projectdir% /Y
::config platform customizeable
copy %CONFIG%\tfm_app_rot.yaml %projectdir% /Y
copy %CONFIG%\tfm_platform.yaml %projectdir% /Y
dir %TOOLS% > %1\output.txt 2>&1
#%python% %TOOLS%\dist\pyscript\tfm_parse_manifest_list.py -o %projectdir%\g -m %projectdir%\tfm_manifest_list.yaml -f %projectdir%\tfm_generated_file_list.yaml >> %1\output.txt 2>&1
%TOOLS%\dist\tfm_parse_manifest_list\tfm_parse_manifest_list.exe -o %projectdir%\g -m %projectdir%\tfm_manifest_list.yaml -f %projectdir%\tfm_generated_file_list.yaml >> %1\output.txt 2>&1
iccarm.exe --cpu=Cortex-M33 -D%2 -DBL2 -DTFM_PSA_API -DTFM_PARTITION_PLATFORM  -I%1\..\..\Linker  %1\tfm_common_s.icf  --silent --preprocess=ns %1\tfm_common_s.icf.i > %1\output.txt 2>&1
