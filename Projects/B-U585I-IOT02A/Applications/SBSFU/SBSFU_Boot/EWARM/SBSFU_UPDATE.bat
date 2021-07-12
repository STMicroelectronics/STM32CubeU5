echo "SBSFU_UPDATE started"
set stm32programmercli="C:\Program Files\STMicroelectronics\STM32Cube\STM32CubeProgrammer\bin\STM32_Programmer_CLI.exe"
set connect_no_reset=-c port=SWD mode=UR
set connect=-c port=SWD mode=UR --hardRst
set slot0=
set slot1=
set slot2=
set slot3=
set boot=
set loader=
set cfg_loader=
set image_number=
if  "%image_number%" == "2" (
echo "Write SBSFU_Appli Secure"
%stm32programmercli% %connect% -d %~dp0..\..\SBSFU_Appli\binary\sbsfu_s_init.bin %slot0% -v
IF %errorlevel% NEQ 0 goto :error
echo "SBSFU_Appli Secure Written"
echo "Write SBSFU_Appli NonSecure"
%stm32programmercli% %connect% -d %~dp0..\..\SBSFU_Appli\binary\sbsfu_ns_init.bin %slot1% -v
IF %errorlevel% NEQ 0 goto :error
echo "SBSFU_Appli NonSecure Written"
)
if  "%image_number%" == "1" (
echo "Write SBSFU_Appli"
%stm32programmercli% %connect% -d %~dp0..\..\SBSFU_Appli\binary\sbsfu_init.bin %slot0% -v
IF %errorlevel% NEQ 0 goto :error
echo "SBSFU_Appli Written"
)
:: write loader if config loader is active
if  "%cfg_loader%" == "1" (
echo "Write SBSFU_Loader"
%stm32programmercli% %connect% -d %~dp0..\..\SBSFU_Loader\binary\loader.bin %loader% -v
IF %errorlevel% NEQ 0 goto :error
echo "SBSFU_Loader  Written"
)
echo "Write SBSFU_Boot"
%stm32programmercli% %connect% -d %~dp0.\B-U585I-IOT02A\Exe\project.bin %boot% -v
IF %errorlevel% NEQ 0 goto :error
echo "SBSFU_Boot Written"
echo "SBSFU_UPDATE script done, press a key"
IF [%1] NEQ [AUTO] pause
exit 0

:error
echo "SBSFU_UPDATE script failed, press key"
IF [%1] NEQ [AUTO] pause
exit 1