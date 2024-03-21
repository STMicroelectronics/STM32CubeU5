:: arg1 is the build directory
:: arg2 is the version i.e "1.0.0"
:: arg3 is the binary type (1 nonsecure, 2 secure)
:: Following args are optional
:: arg4 : dependency with the other image
:: "1.0.0" : version dependency with other image
:: "nodep": no dependency
:: arg5 is version and dependency in output filename
:: "version" : filename contains the version and dependency
:: arg6 is to force python execution
:: "python" : execute with python script
set "projectdir=%~dp0"
set "version=%2"
set "signing=%~3"
set "userAppBinary=%projectdir%\..\Binary"
IF "%4"=="" (
goto :veropt;
)
set "dep=%~4"
if "%dep%"=="nodep" (
goto :veropt
)
set "depname=_%dep%"

IF "%signing%" == "nonsecure" (
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

set "sbsfu_s_init=%userAppBinary%\sbsfu_s_app_init%ver%.bin"
set "sbsfu_ns_init=%userAppBinary%\sbsfu_ns_app_init%ver%.bin"
set "sbsfu_s_sign=%userAppBinary%\sbsfu_s_app_sign%ver%.bin"
set "sbsfu_ns_sign=%userAppBinary%\sbsfu_ns_app_sign%ver%.bin"
set "sbsfu_s_enc_sign=%userAppBinary%\sbsfu_s_app_enc_sign%ver%.bin"
set "sbsfu_ns_enc_sign=%userAppBinary%\sbsfu_ns_app_enc_sign%ver%.bin"
:: variable for app_image_number = 1
set "sbsfu_init=%userAppBinary%\sbsfu_app_init%ver%.bin"
set "sbsfu_sign=%userAppBinary%\sbsfu_app_sign%ver%.bin"
set "sbsfu_enc_sign=%userAppBinary%\sbsfu_app_enc_sign%ver%.bin"
set "sbsfu=%userAppBinary%\sbsfu_app.bin"

::field updated with SBSFU_Boot postbuild

set image_ns_size=
set image_s_size=
set primary_only=
set app_image_number=
set crypto_scheme=
set external_flash_enable=
set ns_code_start=
set encrypted=
set over_write=
:: end of updated field
pushd %projectdir%\..\..\SBSFU_Boot\Src
set sbsfu_key_dir=%cd%
popd
pushd %projectdir%\..\..\..\..\..\..\Middlewares\Third_Party\mcuboot
set mcuboot_dir=%cd%
popd
::Make sure we have a Binary sub-folder in UserApp folder
if not exist "%userAppBinary%" (
mkdir "%userAppBinary%"
)

IF "%signing%" == "nonsecure" (
goto :start;
)

if  "%app_image_number%" == "1" (
:: when image number is 1, image is assemble and sign during non secure postbuild
exit 0
)
:start
IF [%~6] NEQ [python] (
goto exe:
)
goto py:
:exe
::line for window executable
set "imgtool=%mcuboot_dir%\scripts\dist\imgtool\imgtool.exe"
set "python="
if exist %imgtool% (
echo Postbuild with windows executable
goto postbuild
)
:py
::line for python
echo Postbuild with python script
set "imgtool=%mcuboot_dir%\scripts\imgtool\main.py"
set "python=python "
:postbuild


::sign mode
if "%crypto_scheme%" == "0" (
set "mode=rsa2048"
)
if "%crypto_scheme%" == "1" (
set "mode=rsa3072"
)
if "%crypto_scheme%" == "2" (
set "mode=ec256"
)
::keys selection
if  "%mode%" == "rsa2048" (
set "key_s=%sbsfu_key_dir%\root-rsa-2048.pem"
set "key_ns=%sbsfu_key_dir%\root-rsa-2048_1.pem"
set "key_enc_pub=%sbsfu_key_dir%\enc-rsa2048-pub.pem"
)
if  "%mode%" == "rsa3072" (
set "key_s=%sbsfu_key_dir%\root-rsa-3072.pem"
set "key_ns=%sbsfu_key_dir%\root-rsa-3072_1.pem"
set "key_enc_pub=%sbsfu_key_dir%\enc-rsa2048-pub.pem"
)
if  "%mode%" == "ec256" (
set "key_s=%sbsfu_key_dir%\root-ec-p256.pem"
set "key_ns=%sbsfu_key_dir%\root-ec-p256_1.pem"
set "key_enc_pub=%sbsfu_key_dir%\enc-ec256-pub.pem"
)
set "sbsfu_s=%projectdir%\Secure\B-U585I-IOT02A_S\Exe\Project.bin"
set "sbsfu_ns=%projectdir%\NonSecure\B-U585I-IOT02A_NS\Exe\Project.bin"
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
echo %signing% %mode% %option% app_image_number=%app_image_number% > %projectdir%\output.txt 2>&1

if "%app_image_number%" == "2" (
goto continue
)
echo "assemble image" >> %projectdir%\output.txt 2>&1
set "command_ass=%python%%imgtool% ass -f %sbsfu_s% -o %image_s_size%  -i 0x0 %sbsfu_ns% %sbsfu% >> %projectdir%\output.txt 2>&1"
%command_ass%
IF %ERRORLEVEL% NEQ 0 goto :error_ass
:: sign assemble binary as secure image, slot size is image_ns_size
set "image_s_size=%image_ns_size%"
set "signing="
set "sbsfu_s=%sbsfu%"
set "sbsfu_s_init=%sbsfu_init%"
set "sbsfu_s_sign=%sbsfu_sign%"
set "sbsfu_s_enc_sign=%sbsfu_enc_sign%"
:continue
::alignment value (minimum write size value in bytes)
set "val=16"

::nb sectors in image areas
set /A image_ns_sectors=%image_ns_size% / 0x2000
set /A image_s_sectors=%image_s_size% / 0x2000

echo "%signing% init image signing" >> %projectdir%\output.txt 2>&1
IF "%signing%" == "nonsecure" (
set "command_init=%python%%imgtool% sign -k %key_ns% %encrypt% -e little -S %image_ns_size% -M %image_ns_sectors% -H 0x400 --pad-header %option% -v %version% --confirm --pad -s auto --align %val% %sbsfu_ns% %sbsfu_ns_init% >> %projectdir%\output.txt 2>&1"
goto :docommand_init
)
set "command_init=%python%%imgtool% sign -k %key_s% %encrypt% -e little -S %image_s_size% -M %image_s_sectors% -H 0x400 --pad-header %option% -v %version% --confirm --pad -s auto --align %val% %sbsfu_s% %sbsfu_s_init% >> %projectdir%\output.txt 2>&1"
:docommand_init
%command_init%
IF %ERRORLEVEL% NEQ 0 goto :error_init

echo "%signing% clear image signing" >> %projectdir%\output.txt 2>&1
IF "%signing%" == "nonsecure" (
set "command_clear=%python%%imgtool% sign -k %key_ns% -e little -S %image_ns_size% -M %image_ns_sectors% -H 0x400 --pad-header %option% -v %version% -s auto --align %val% %sbsfu_ns% %sbsfu_ns_sign% >> %projectdir%\output.txt 2>&1"
goto :docommand_clear
)
set "command_clear=%python%%imgtool% sign -k %key_s% -e little -S %image_s_size% -M %image_s_sectors% -H 0x400 --pad-header %option% -v %version% -s auto --align %val% %sbsfu_s% %sbsfu_s_sign% >> %projectdir%\output.txt 2>&1"
:docommand_clear
%command_clear%
IF %ERRORLEVEL% NEQ 0 goto :error_clear

if "%encrypted%" == "0" (
exit 0
)
echo "%signing% enc image encrypting and signing" >> %projectdir%\output.txt 2>&1
IF "%signing%" == "nonsecure" (
set "command_enc=%python%%imgtool% sign -k %key_ns% -E %key_enc_pub% -e little -S %image_ns_size% -M %image_ns_sectors% -H 0x400 --pad-header %option% -v %version%  -s auto --align %val% %sbsfu_ns% %sbsfu_ns_enc_sign% >> %projectdir%\output.txt 2>&1"
goto :docommand_enc
)
set "command_enc=%python%%imgtool% sign -k %key_s% -E %key_enc_pub% -e little -S %image_s_size% -M %image_s_sectors% -H 0x400 --pad-header %option% -v %version%  -s auto --align %val% %sbsfu_s% %sbsfu_s_enc_sign% >> %projectdir%\output.txt 2>&1"
:docommand_enc
%command_enc%
IF %ERRORLEVEL% NEQ 0 goto :error_enc

exit 0
:error_init
echo "%command_init% : failed" >> %projectdir%\\output.txt
exit 1
:error_clear
echo "%command_clear% : failed" >> %projectdir%\\output.txt
exit 1
:error_enc
echo "%command_enc% : failed" >> %projectdir%\\output.txt
exit 1
:error_ass
echo "%command_ass% : failed" >> %projectdir%\\output.txt
exit 1