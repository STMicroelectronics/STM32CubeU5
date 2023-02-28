#!/bin/bash -
echo "TFM_UPDATE started"
# Absolute path to this script
SCRIPT=$(readlink -f $0)
# Absolute path this script
SCRIPTPATH=`dirname $SCRIPT`
PATH="/C/Program Files/STMicroelectronics/STM32Cube/STM32CubeProgrammer/bin/":$PATH
stm32programmercli="STM32_Programmer_CLI"
connect_no_reset="-c port=SWD mode=UR"
connect="-c port=SWD mode=UR --hardRst"
slot0=
slot1=
slot2=
slot3=
slot4=
slot5=
slot6=
slot7=
boot=
loader=
cfg_loader=
app_image_number=
s_data_image_number=
ns_data_image_number=
if [ $app_image_number == 2 ]; then
echo "Write TFM_Appli Secure"
$stm32programmercli $connect -d $SCRIPTPATH/../../TFM_Appli/Binary/tfm_s_app_init.bin $slot0 -v
ret=$?
if [ $ret != 0 ]; then
  if [ "$1" != "AUTO" ]; then read -p "TFM_UPDATE script failed, press a key" -n1 -s; fi
  exit 1
fi
echo "TFM_Appli Secure Written"
echo "Write TFM_Appli NonSecure"
$stm32programmercli $connect -d $SCRIPTPATH/../../TFM_Appli/Binary/tfm_ns_app_init.bin $slot1 -v
ret=$?
if [ $ret != 0 ]; then
  if [ "$1" != "AUTO" ]; then read -p "TFM_UPDATE script failed, press a key" -n1 -s; fi
  exit 1
fi
echo "TFM_Appli NonSecure Written"
fi
if [ $app_image_number == 1 ]; then
echo "Write TFM_Appli"
$stm32programmercli $connect -d $SCRIPTPATH/../../TFM_Appli/Binary/tfm_app_init.bin $slot0 -v
ret=$?
if [ $ret != 0 ]; then
  if [ "$1" != "AUTO" ]; then read -p "TFM_UPDATE script failed, press a key" -n1 -s; fi
  exit 1
fi
echo "TFM_Appli Written"
fi
if [ $s_data_image_number == 1 ]; then
echo "Write TFM_Appli Secure Data"
if [ ! -e $SCRIPTPATH/../../TFM_Appli/Binary/tfm_s_data_init.bin ]; then
    if [ "$1" != "AUTO" ]; then read -p "Error : sbsfu_s_data_init.bin does not exist! Run dataimg.bat script to generate it" -n1 -s; fi
    exit 1
fi
$stm32programmercli $connect -d $SCRIPTPATH/../../TFM_Appli/Binary/tfm_s_data_init.bin $slot4 -v
ret=$?
if [ $ret != 0 ]; then
  if [ "$1" != "AUTO" ]; then read -p "TFM_UPDATE script failed, press a key" -n1 -s; fi
  exit 1
fi
echo "TFM_Appli Secure Data Written"
fi
if [ $ns_data_image_number == 1 ]; then
echo "Write TFM_Appli NonSecure Data"
if [ ! -e $SCRIPTPATH/../../TFM_Appli/Binary/tfm_ns_data_init.bin ]; then
    if [ "$1" != "AUTO" ]; then read -p "Error : sbsfu_ns_data_init.bin does not exist! Run dataimg.bat script to generate it" -n1 -s; fi
    exit 1
fi
$stm32programmercli $connect -d $SCRIPTPATH/../../TFM_Appli/Binary/tfm_ns_data_init.bin $slot5 -v
ret=$?
if [ $ret != 0 ]; then
  if [ "$1" != "AUTO" ]; then read -p "TFM_UPDATE script failed, press a key" -n1 -s; fi
  exit 1
fi
echo "TFM NonSecure Data Written"
fi

# write loader if config loader is active
if [ $cfg_loader == 1 ]; then
echo "Write TFM_Loader"
$stm32programmercli $connect -d $SCRIPTPATH/../../TFM_Loader/Binary/loader.bin $loader -v
ret=$?
if [ $ret != 0 ]; then
  if [ "$1" != "AUTO" ]; then read -p "TFM_UPDATE script failed, press a key" -n1 -s; fi
  exit 1
fi
echo "TFM_Loader Written"
fi
echo "Write TFM_SBSFU_Boot"
$stm32programmercli $connect -d $SCRIPTPATH/Release/TFM_SBSFU_Boot.bin $boot -v
ret=$?
if [ $ret != 0 ]; then
  if [ "$1" != "AUTO" ]; then read -p "TFM_UPDATE script failed, press a key" -n1 -s; fi
  exit 1
fi
echo "TFM_SBSFU_Boot Written"
if [ "$1" != "AUTO" ]; then read -p "TFM_UPDATE script Done, press a key" -n1 -s; fi
exit 0