:: Absolute path this script
set "projectdir=%~dp0"
set "cube_fw_path=%~dp0..\..\..\..\..\.."
call "%projectdir%\..\..\env.bat"
set "preprocess_bl2_file=%projectdir%\image_macros_preprocessed_bl2.c"
set "hardening=%projectdir%\hardening.bat"
set "regression=%projectdir%\regression.bat"
set "update=%projectdir%\SBSFU_UPDATE.bat"
set "postbuild_appli=%projectdir%\..\..\SBSFU_Appli\EWARM\postbuild.bat"
set "postbuild_loader=%projectdir%\..\..\SBSFU_Loader\EWARM\postbuild.bat"
set "data_img_gen=%projectdir%\dataimg.bat"

::line for windows executable
set "applicfg=%cube_fw_path%\Utilities\PC_Software\ROT_AppliConfig\dist\AppliCfg.exe"
set "python="
if exist %applicfg% (
echo run config Appli with windows executable
goto postbuild
)

::line for python
echo run config Appli with python script
set "applicfg=%cube_fw_path%\Utilities\PC_Software\ROT_AppliConfig\AppliCfg.py"
set "python=python "

:postbuild
set "command=%python%%applicfg% flash --layout %preprocess_bl2_file% -b secbootadd0 -m  RE_BL2_BOOT_ADDRESS  -d 0x80 %regression% > %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

set "command=%python%%applicfg% flash --layout %preprocess_bl2_file% -b flashsectnbr -m  RE_FLASH_PAGE_NBR %regression% > %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

set "command=%python%%applicfg% flash --layout %preprocess_bl2_file% -b nsbootadd -m  RE_BL2_BOOT_ADDRESS  -d 0x80 %hardening% >> %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

set "command=%python%%applicfg% flash --layout %preprocess_bl2_file%  -b boot -m  RE_BL2_PERSO_ADDRESS %update% >> %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

set "command=%python%%applicfg% flash --layout %preprocess_bl2_file%  -b slot0 -m  RE_IMAGE_FLASH_ADDRESS_SECURE %update% >> %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

set "command=%python%%applicfg% flash --layout %preprocess_bl2_file% -b slot1 -m  RE_IMAGE_FLASH_ADDRESS_NON_SECURE %update% >> %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

set "command=%python%%applicfg% flash --layout %preprocess_bl2_file%  -b slot2 -m  RE_IMAGE_FLASH_SECURE_UPDATE %update% >> %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

set "command=%python%%applicfg% flash --layout %preprocess_bl2_file% -b slot3 -m  RE_IMAGE_FLASH_NON_SECURE_UPDATE %update% >> %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

set "command=%python%%applicfg% flash --layout %preprocess_bl2_file% -b slot4 -m  RE_IMAGE_FLASH_ADDRESS_DATA_SECURE %update% >> %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

set "command=%python%%applicfg% flash --layout %preprocess_bl2_file% -b slot5 -m  RE_IMAGE_FLASH_ADDRESS_DATA_NON_SECURE %update% >> %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

set "command=%python%%applicfg% flash --layout %preprocess_bl2_file% -b slot6 -m  RE_IMAGE_FLASH_DATA_SECURE_UPDATE %update% >> %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

set "command=%python%%applicfg% flash --layout %preprocess_bl2_file% -b slot7 -m  RE_IMAGE_FLASH_DATA_NON_SECURE_UPDATE %update% >> %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

set "command=%python%%applicfg% flash --layout %preprocess_bl2_file% -b wrp_start -m  RE_BL2_WRP_START -d 0x2000 --decimal %hardening% >> %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

set "command=%python%%applicfg% flash --layout %preprocess_bl2_file% -b wrp_end -m  RE_BL2_WRP_END -d 0x2000 --decimal %hardening% >> %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

set "command=%python%%applicfg% flash --layout %preprocess_bl2_file% -b sec1_end -m  RE_BL2_SEC1_END -d 0x2000 --decimal %hardening% >> %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

set "command=%python%%applicfg% flash --layout %preprocess_bl2_file% -b sec2_start -m  RE_BL2_SEC2_START -d 0x2000 --decimal %hardening% >> %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

set "command=%python%%applicfg% flash --layout %preprocess_bl2_file% -b sec2_end -m  RE_BL2_SEC2_END -d 0x2000 --decimal %hardening% >> %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

set "command=%python%%applicfg% flash --layout %preprocess_bl2_file% -b hdp_end -m  RE_BL2_HDP_END -d 0x2000 --decimal %hardening% >> %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

set "command=%python%%applicfg% flash --layout %preprocess_bl2_file% -b image_s_size -m  RE_IMAGE_FLASH_SECURE_IMAGE_SIZE %postbuild_appli% >> %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

set "command=%python%%applicfg% flash --layout %preprocess_bl2_file% -b image_s_size -m  RE_IMAGE_FLASH_SECURE_DATA_IMAGE_SIZE %data_img_gen% >> %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

set "command=%python%%applicfg% flash --layout %preprocess_bl2_file% -b image_ns_size -m  RE_IMAGE_FLASH_NON_SECURE_IMAGE_SIZE %postbuild_appli% >> %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

set "command=%python%%applicfg% flash --layout %preprocess_bl2_file% -b image_ns_size -m  RE_IMAGE_FLASH_NON_SECURE_DATA_IMAGE_SIZE %data_img_gen% >> %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

set "command=%python%%applicfg% flash --layout %preprocess_bl2_file% -b external_flash_enable -m  RE_EXTERNAL_FLASH_ENABLE --decimal %postbuild_appli% >> %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

set "command=%python%%applicfg% flash --layout %preprocess_bl2_file% -b ns_code_start -m  RE_CODE_START_NON_SECURE %postbuild_appli% >> %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

set "command=%python%%applicfg% flash --layout %preprocess_bl2_file% -b loader -m  RE_IMAGE_FLASH_LOADER_ADDRESS %update% >> %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

set "command=%python%%applicfg% flash --layout %preprocess_bl2_file% -b wrp_bank2_start -m  RE_LOADER_WRP_START -d 0x2000 --decimal %hardening% >> %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

set "command=%python%%applicfg% flash --layout %preprocess_bl2_file% -b wrp_bank2_end -m  RE_LOADER_WRP_END -d 0x2000 --decimal %hardening% >> %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

set "command=%python%%applicfg% flash --layout %preprocess_bl2_file% -b primary_only -m  RE_PRIMARY_ONLY --decimal %postbuild_appli% >> %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

set "command=%python%%applicfg% flash --layout %preprocess_bl2_file% -b primary_only -m  RE_PRIMARY_ONLY --decimal %data_img_gen% >> %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

set "command=%python%%applicfg% flash --layout %preprocess_bl2_file% -b app_image_number -m  RE_APP_IMAGE_NUMBER --decimal %postbuild_appli% >> %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

set "command=%python%%applicfg% flash --layout %preprocess_bl2_file% -b cfg_loader -m  RE_EXT_LOADER --decimal %update% >> %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

set "command=%python%%applicfg% flash --layout %preprocess_bl2_file% -b app_image_number -m  RE_APP_IMAGE_NUMBER --decimal %update% >> %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

set "command=%python%%applicfg% flash --layout %preprocess_bl2_file% -b s_data_image_number -m  RE_S_DATA_IMAGE_NUMBER --decimal %update% >> %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

set "command=%python%%applicfg% flash --layout %preprocess_bl2_file% -b ns_data_image_number -m  RE_NS_DATA_IMAGE_NUMBER --decimal %update% >> %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

set "command=%python%%applicfg% flash --layout %preprocess_bl2_file% -b crypto_scheme -m  RE_CRYPTO_SCHEME --decimal %postbuild_appli% >> %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

set "command=%python%%applicfg% flash --layout %preprocess_bl2_file% -b crypto_scheme -m  RE_CRYPTO_SCHEME --decimal %data_img_gen% >> %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

set "command=%python%%applicfg% flash --layout %preprocess_bl2_file% -b over_write -m  RE_OVER_WRITE --decimal %postbuild_appli% >> %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

set "command=%python%%applicfg% flash --layout %preprocess_bl2_file% -b over_write -m  RE_OVER_WRITE --decimal %data_img_gen% >> %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

set "command=%python%%applicfg% flash --layout %preprocess_bl2_file% -b encrypted -m  RE_ENCRYPTION --decimal %postbuild_appli% >> %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

set "command=%python%%applicfg% flash --layout %preprocess_bl2_file% -b encrypted -m  RE_ENCRYPTION --decimal %data_img_gen% >> %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

set "command=%python%%applicfg% flash --layout %preprocess_bl2_file% -b loader_ns_size -m  RE_LOADER_NS_CODE_SIZE --decimal %postbuild_loader% >> %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

set "command=%python%%applicfg% flash --layout %preprocess_bl2_file% -b loader_s_size -m  RE_LOADER_IMAGE_S_CODE_SIZE --decimal %postbuild_loader% >> %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

set "command=%python%%applicfg% flash --layout %preprocess_bl2_file% -b flash_area_scratch_size -m  RE_FLASH_AREA_SCRATCH_SIZE %postbuild_appli% >> %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

set "command=%python%%applicfg% flash --layout %preprocess_bl2_file% -b flash_area_scratch_size -m  RE_FLASH_AREA_SCRATCH_SIZE %data_img_gen% >> %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

echo %regression%" updated" >> %projectdir%\output.txt 2>&1
echo %hardening%" updated" >> %projectdir%\output.txt 2>&1
echo %update%" updated" >> %projectdir%\output.txt 2>&1
echo %postbuild_appli%" updated" >> %projectdir%\output.txt 2>&1
echo %data_img_gen%" updated" >> %projectdir%\output.txt 2>&1
echo %postbuild_loader%" updated" >> %projectdir%\output.txt 2>&1
exit 0

:error
echo "%command% : failed" >> %projectdir%\\output.txt
exit 1

