echo "TFM_UPDATE started"
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
echo "Write TFM_Appli Secure"
%stm32programmercli% %connect% -d %~dp0..\..\TFM_Appli\binary\tfm_s_init.bin %slot0% -v
IF %errorlevel% NEQ 0 goto :error
echo "TFM_Appli Secure Written"
echo "Write TFM_Appli NonSecure"
%stm32programmercli% %connect% -d %~dp0..\..\TFM_Appli\binary\tfm_ns_init.bin %slot1% -v
IF %errorlevel% NEQ 0 goto :error
echo "TFM_Appli NonSecure Written"
)
if  "%image_number%" == "1" (
echo "Write TFM_Appli"
%stm32programmercli% %connect% -d %~dp0..\..\TFM_Appli\binary\tfm_init.bin %slot0% -v
IF %errorlevel% NEQ 0 goto :error
echo "TFM_Appli Written"
)
:: write loader if config loader is active
if  "%cfg_loader%" == "1" (
echo "Write TFM_Loader"
%stm32programmercli% %connect% -d %~dp0..\..\TFM_Loader\binary\loader.bin %loader% -v
IF %errorlevel% NEQ 0 goto :error
echo "TFM_Loader  Written"
)
echo "Write TFM_SBSFU_Boot"
%stm32programmercli% %connect% -d %~dp0.\B-U585I-IOT02A\Exe\project.bin %boot% -v
IF %errorlevel% NEQ 0 goto :error
echo "TFM_SBSFU_Boot Written"
echo "TFM_UPDATE script done, press a key"
IF [%1] NEQ [AUTO] pause
exit 0

:error
echo "TFM_UPDATE script failed, press key"
IF [%1] NEQ [AUTO] pause
exit 1