set "projectdir=%~dp0"
set "preprocess_bl2_file=%projectdir%\image_macros_preprocessed_bl2.c"
set "hardening=%projectdir%\hardening.bat"
set "regression=%projectdir%\regression.bat"
set "update=%projectdir%\SBSFU_UPDATE.bat"
set "postbuild_appli=%projectdir%\..\..\SBSFU_Appli\MDK-ARM\postbuild.bat"
set "postbuild_loader=%projectdir%\..\..\SBSFU_Loader\MDK-ARM\postbuild.bat"
set "data_img_gen=%projectdir%\dataimg.bat"
pushd %projectdir%\..\..\..\..\..\..\Middlewares\Third_Party\mcuboot
set mcuboot=%cd%
popd

:start
goto exe:
goto py:
:exe
::line for window executable
set "imgtool=%mcuboot%\scripts\dist\imgtool\imgtool.exe"
set "python="
if exist %imgtool% (
echo Postbuild with windows executable
goto postbuild
)
:py
::line for python
echo Postbuild with python script
set "imgtool=%mcuboot%\scripts\imgtool\main.py"
set "python=python "
:postbuild
set "command=%python%%imgtool% flash --layout %preprocess_bl2_file% -b secbootadd0 -m  RE_BL2_BOOT_ADDRESS  -d 0x80  -s 1 %regression% > %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

set "command=%python%%imgtool% flash --layout %preprocess_bl2_file% -b flashsectnbr -m  RE_FLASH_PAGE_NBR -s 1 %regression% > %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

set "command=%python%%imgtool% flash --layout %preprocess_bl2_file% -b nsbootadd -m  RE_BL2_BOOT_ADDRESS  -d 0x80  -s 1 %hardening% >> %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

set "command=%python%%imgtool% flash --layout %preprocess_bl2_file%  -b boot -m  RE_BL2_PERSO_ADDRESS -s 1 %update% >> %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

set "command=%python%%imgtool% flash --layout %preprocess_bl2_file%  -b slot0 -m  RE_IMAGE_FLASH_ADDRESS_SECURE -s 1 %update% >> %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

set "command=%python%%imgtool% flash --layout %preprocess_bl2_file% -b slot1 -m  RE_IMAGE_FLASH_ADDRESS_NON_SECURE -s 1 %update% >> %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

set "command=%python%%imgtool% flash --layout %preprocess_bl2_file%  -b slot2 -m  RE_IMAGE_FLASH_SECURE_UPDATE -s 1 %update% >> %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

set "command=%python%%imgtool% flash --layout %preprocess_bl2_file% -b slot3 -m  RE_IMAGE_FLASH_NON_SECURE_UPDATE -s 1 %update% >> %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

set "command=%python%%imgtool% flash --layout %preprocess_bl2_file% -b slot4 -m  RE_IMAGE_FLASH_ADDRESS_DATA_SECURE -s 1 %update% >> %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

set "command=%python%%imgtool% flash --layout %preprocess_bl2_file% -b slot5 -m  RE_IMAGE_FLASH_ADDRESS_DATA_NON_SECURE -s 1 %update% >> %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

set "command=%python%%imgtool% flash --layout %preprocess_bl2_file% -b slot6 -m  RE_IMAGE_FLASH_DATA_SECURE_UPDATE -s 1 %update% >> %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

set "command=%python%%imgtool% flash --layout %preprocess_bl2_file% -b slot7 -m  RE_IMAGE_FLASH_DATA_NON_SECURE_UPDATE -s 1 %update% >> %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

set "command=%python%%imgtool% flash --layout %preprocess_bl2_file% -b wrp_start -m  RE_BL2_WRP_START -d 0x2000 -s 1 -p 0 %hardening% >> %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

set "command=%python%%imgtool% flash --layout %preprocess_bl2_file% -b wrp_end -m  RE_BL2_WRP_END -d 0x2000 -s 1 -p 0 %hardening% >> %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

set "command=%python%%imgtool% flash --layout %preprocess_bl2_file% -b sec1_end -m  RE_BL2_SEC1_END -d 0x2000 -s 1 -p 0 %hardening% >> %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

set "command=%python%%imgtool% flash --layout %preprocess_bl2_file% -b sec2_start -m  RE_BL2_SEC2_START -d 0x2000 -s 1 -p 0 %hardening% >> %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

set "command=%python%%imgtool% flash --layout %preprocess_bl2_file% -b sec2_end -m  RE_BL2_SEC2_END -d 0x2000 -s 1 -p 0 %hardening% >> %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

set "command=%python%%imgtool% flash --layout %preprocess_bl2_file% -b hdp_end -m  RE_BL2_HDP_END -d 0x2000 -s 1 -p 0 %hardening% >> %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

set "command=%python%%imgtool% flash --layout %preprocess_bl2_file% -b image_s_size -m  RE_IMAGE_FLASH_SECURE_IMAGE_SIZE -s 1 %postbuild_appli% >> %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

set "command=%python%%imgtool% flash --layout %preprocess_bl2_file% -b image_s_size -m  RE_IMAGE_FLASH_SECURE_DATA_IMAGE_SIZE -s 1 %data_img_gen% >> %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

set "command=%python%%imgtool% flash --layout %preprocess_bl2_file% -b image_ns_size -m  RE_IMAGE_FLASH_NON_SECURE_IMAGE_SIZE -s 1 %postbuild_appli% >> %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

set "command=%python%%imgtool% flash --layout %preprocess_bl2_file% -b image_ns_size -m  RE_IMAGE_FLASH_NON_SECURE_DATA_IMAGE_SIZE -s 1 %data_img_gen% >> %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

set "command=%python%%imgtool% flash --layout %preprocess_bl2_file% -b external_flash_enable -m  RE_EXTERNAL_FLASH_ENABLE -s 1 -p 0 %postbuild_appli% >> %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

set "command=%python%%imgtool% flash --layout %preprocess_bl2_file% -b ns_code_start -m  RE_CODE_START_NON_SECURE -s 1 %postbuild_appli% >> %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

set "command=%python%%imgtool% flash --layout %preprocess_bl2_file% -b loader -m  RE_IMAGE_FLASH_LOADER_ADDRESS -s 1 %update% >> %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

set "command=%python%%imgtool% flash --layout %preprocess_bl2_file% -b wrp_bank2_start -m  RE_LOADER_WRP_START -d 0x2000 -s 1 -p 0 %hardening% >> %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

set "command=%python%%imgtool% flash --layout %preprocess_bl2_file% -b wrp_bank2_end -m  RE_LOADER_WRP_END -d 0x2000 -s 1 -p 0 %hardening% >> %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

set "command=%python%%imgtool% flash --layout %preprocess_bl2_file% -b primary_only -m  RE_PRIMARY_ONLY -s 1 -p 0 %postbuild_appli% >> %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

set "command=%python%%imgtool% flash --layout %preprocess_bl2_file% -b primary_only -m  RE_PRIMARY_ONLY -s 1 -p 0 %data_img_gen% >> %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

set "command=%python%%imgtool% flash --layout %preprocess_bl2_file% -b app_image_number -m  RE_APP_IMAGE_NUMBER -s 1 -p 0 %postbuild_appli% >> %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

set "command=%python%%imgtool% flash --layout %preprocess_bl2_file% -b cfg_loader -m  RE_EXT_LOADER -s 1 -p 0 %update% >> %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

set "command=%python%%imgtool% flash --layout %preprocess_bl2_file% -b app_image_number -m  RE_APP_IMAGE_NUMBER -s 1 -p 0 %update% >> %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

set "command=%python%%imgtool% flash --layout %preprocess_bl2_file% -b s_data_image_number -m  RE_S_DATA_IMAGE_NUMBER -s 1 -p 0 %update% >> %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

set "command=%python%%imgtool% flash --layout %preprocess_bl2_file% -b ns_data_image_number -m  RE_NS_DATA_IMAGE_NUMBER -s 1 -p 0 %update% >> %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

set "command=%python%%imgtool% flash --layout %preprocess_bl2_file% -b crypto_scheme -m  RE_CRYPTO_SCHEME -s 1 -p 0 %postbuild_appli% >> %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

set "command=%python%%imgtool% flash --layout %preprocess_bl2_file% -b crypto_scheme -m  RE_CRYPTO_SCHEME -s 1 -p 0 %data_img_gen% >> %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

set "command=%python%%imgtool% flash --layout %preprocess_bl2_file% -b over_write -m  RE_OVER_WRITE -s 1 -p 0 %postbuild_appli% >> %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

set "command=%python%%imgtool% flash --layout %preprocess_bl2_file% -b over_write -m  RE_OVER_WRITE -s 1 -p 0 %data_img_gen% >> %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

set "command=%python%%imgtool% flash --layout %preprocess_bl2_file% -b encrypted -m  RE_ENCRYPTION -s 1 -p 0 %postbuild_appli% >> %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

set "command=%python%%imgtool% flash --layout %preprocess_bl2_file% -b encrypted -m  RE_ENCRYPTION -s 1 -p 0 %data_img_gen% >> %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

set "command=%python%%imgtool% flash --layout %preprocess_bl2_file% -b loader_ns_size -m  RE_LOADER_NS_CODE_SIZE -s 1 %postbuild_loader% >> %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

set "command=%python%%imgtool% flash --layout %preprocess_bl2_file% -b loader_s_size -m  RE_LOADER_IMAGE_S_CODE_SIZE -s 1 %postbuild_loader% >> %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error
exit 0

:error
echo "%command% : failed" >> %projectdir%\\output.txt
exit 1

