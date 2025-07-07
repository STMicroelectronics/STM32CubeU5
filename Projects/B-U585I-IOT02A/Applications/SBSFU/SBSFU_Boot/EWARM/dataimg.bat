:: args are optional
:: arg1 is the build directory
:: arg2 is the version i.e "2.1.0" (default is "1.0.0")
:: arg3 is the binary type (nonsecure, secure) (default is both)
:: arg4 : dependency with the other image
:: "1.0.0" : version dependency with appli image
:: "nodep": no dependency
:: arg5 is version and dependency in output filename
:: "version" : filename contains the version and dependency
:: arg6 is to force python execution
:: "python" : execute with python script
set "projectdir=%~dp0"
call "%projectdir%\..\..\env.bat"
IF "%2"=="" (
set "version=1.0.0"
goto :set_version_end
)
set "version=%2"
:set_version_end
set "signing=%~3"
set "userAppBinary=%projectdir%\..\..\SBSFU_Appli\Binary"
IF "%4"=="" (
goto :veropt;
)
set "dep=%~4"
if "%dep%"=="nodep" (
goto :veropt
)
set "depname=_%dep%"

IF "%signing%" == "secure" (
set "option=-d ^(0,%dep%^)"
goto :veropt
)
set "option=-d ^(1,%dep%^)"


:veropt
IF [%5] NEQ [version] (
goto :namebuild;
)

set "ver=_%version%%depname%"

:namebuild

set "sbsfu_s_init=%userAppBinary%\sbsfu_s_data_init%ver%.bin"
set "sbsfu_ns_init=%userAppBinary%\sbsfu_ns_data_init%ver%.bin"
set "sbsfu_s_sign=%userAppBinary%\sbsfu_s_data_sign%ver%.bin"
set "sbsfu_ns_sign=%userAppBinary%\sbsfu_ns_data_sign%ver%.bin"
set "sbsfu_s_enc_sign=%userAppBinary%\sbsfu_s_data_enc_sign%ver%.bin"
set "sbsfu_ns_enc_sign=%userAppBinary%\sbsfu_ns_data_enc_sign%ver%.bin"

::field updated by SBSFU_Boot postbuild

set image_ns_size=
set image_s_size=
set primary_only=
set crypto_scheme=
set encrypted=
set over_write=
set flash_area_scratch_size=
:: end of updated field
pushd %projectdir%\..\..\SBSFU_Boot\Src
set sbsfu_key_dir=%cd%
popd

::Make sure we have a Binary sub-folder in UserApp folder
if not exist "%userAppBinary%" (
mkdir "%userAppBinary%"
)

:start
if exist %imgtool% (
echo dataimg with windows executable
goto dataimg
) else (
  echo ""
  echo "!!! WARNING : imgtool has not been found on your installation."
  echo ""
  echo "  Install CubeProgrammer on your machine in default path : C:\Program Files\STMicroelectronics\STM32Cube\STM32CubeProgrammer"
  echo "  or "
  echo "  Update your %projectdir%\..\..\env.bat with the proper path."
  echo ""
  exit 0
)

:dataimg


::sign mode
if "%crypto_scheme%" == "2" (
set "mode=ec256"
)
::keys selection
if  "%mode%" == "ec256" (
set "key_s=%sbsfu_key_dir%\root-ec-p256.pem"
set "key_ns=%sbsfu_key_dir%\root-ec-p256_1.pem"
set "key_enc_pub=%sbsfu_key_dir%\enc-ec256-pub.pem"
)
set "sbsfu_s=%projectdir%\..\..\SBSFU_Boot\Src\s_data.bin"
set "sbsfu_ns=%projectdir%\..\..\SBSFU_Boot\Src\ns_data.bin"
:: encrypt option is for init image in swap mode
if "%encrypted%" == "1" (
set "encrypt=-E %key_enc_pub% -c"
)
if "%primary_only%" == "1" (
set "option=%option% --primary-only --overwrite-only"
set "encrypt="
goto signing
)
if "%over_write%" == "1" (
set "option=%option% --overwrite-only"
set "encrypt="
)
:signing
echo %signing% %mode% %option% > %projectdir%\output.txt 2>&1

::alignment value (minimum write size value in bytes)
set "val=16"

::nb sectors in image areas
IF "%over_write%" == "1" (
set "image_ns_sectors="
set "image_s_sectors="
set "flag="
) ELSE (
set /A "image_ns_sectors=(((%image_ns_size%+1) / %flash_area_scratch_size%)+1)"
set /A "image_s_sectors=(((%image_s_size%+1) / %flash_area_scratch_size%)+1)"
set "flag=-M"
)

IF "%signing%" == "secure" (
goto :docommand_nonsecure_end
)
echo "%signing% init ns image signing" >> %projectdir%\output.txt 2>&1
set "command_init_ns=%imgtool% sign -k %key_ns% %encrypt% -e little -S %image_ns_size% %flag% %image_ns_sectors% -H 0x20 --pad-header %option% -v %version% --confirm --pad -s auto --align %val% %sbsfu_ns% %sbsfu_ns_init% >> %projectdir%\output.txt 2>&1"
%command_init_ns%
IF %ERRORLEVEL% NEQ 0 goto :error_init_ns

echo "%signing% clear ns image signing" >> %projectdir%\output.txt 2>&1
set "command_clear_ns=%imgtool% sign -k %key_ns% -e little -S %image_ns_size% %flag% %image_ns_sectors% -H 0x20 --pad-header %option% -v %version% -s auto --align %val% %sbsfu_ns% %sbsfu_ns_sign% >> %projectdir%\output.txt 2>&1"
%command_clear_ns%
IF %ERRORLEVEL% NEQ 0 goto :error_clear_ns

if "%encrypted%" == "0" (
goto :docommand_nonsecure_end
)
echo "%signing% enc ns image encrypting and signing" >> %projectdir%\output.txt 2>&1
set "command_enc_ns=%imgtool% sign -k %key_ns% -E %key_enc_pub% -e little -S %image_ns_size% %flag% %image_ns_sectors% -H 0x20 --pad-header %option% -v %version%  -s auto --align %val% %sbsfu_ns% %sbsfu_ns_enc_sign% >> %projectdir%\output.txt 2>&1"
%command_enc_ns%
IF %ERRORLEVEL% NEQ 0 goto :error_enc_ns
:docommand_nonsecure_end

IF "%signing%" == "nonsecure" (
goto :docommand_secure_end
)
echo "%signing% init s image signing" >> %projectdir%\output.txt 2>&1
set "command_init_s=%imgtool% sign -k %key_s% %encrypt% -e little -S %image_s_size% %flag% %image_s_sectors% -H 0x20 --pad-header %option% -v %version% --confirm --pad -s auto --align %val% %sbsfu_s% %sbsfu_s_init% >> %projectdir%\output.txt 2>&1"
%command_init_s%
IF %ERRORLEVEL% NEQ 0 goto :error_init_s

echo "%signing% clear s image signing" >> %projectdir%\output.txt 2>&1
set "command_clear_s=%imgtool% sign -k %key_s% -e little -S %image_s_size% %flag% %image_s_sectors% -H 0x20 --pad-header %option% -v %version% -s auto --align %val% %sbsfu_s% %sbsfu_s_sign% >> %projectdir%\output.txt 2>&1"
%command_clear_s%
IF %ERRORLEVEL% NEQ 0 goto :error_clear_s

if "%encrypted%" == "0" (
goto :docommand_secure_end
)
echo "%signing% enc s image encrypting and signing" >> %projectdir%\output.txt 2>&1
set "command_enc_s=%imgtool% sign -k %key_s% -E %key_enc_pub% -e little -S %image_s_size% %flag% %image_s_sectors% -H 0x20 --pad-header %option% -v %version%  -s auto --align %val% %sbsfu_s% %sbsfu_s_enc_sign% >> %projectdir%\output.txt 2>&1"
%command_enc_s%
IF %ERRORLEVEL% NEQ 0 goto :error_enc_s
:docommand_secure_end

exit 0
:error_init_ns
echo "%command_init_ns% : failed" >> %projectdir%\\output.txt
exit 1
:error_clear_ns
echo "%command_clear_ns% : failed" >> %projectdir%\\output.txt
exit 1
:error_enc_ns
echo "%command_enc_ns% : failed" >> %projectdir%\\output.txt
exit 1
:error_init_s
echo "%command_init_s% : failed" >> %projectdir%\\output.txt
exit 1
:error_clear_s
echo "%command_clear_s% : failed" >> %projectdir%\\output.txt
exit 1
:error_enc_s
echo "%command_enc_s% : failed" >> %projectdir%\\output.txt
exit 1