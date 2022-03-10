#!/bin/bash -
#  ******************************************************************************
#  * @attention
#  *
#  * Copyright (c) 20219 STMicroelectronics.
#  * All rights reserved.
#  *
#  * This software is licensed under terms that can be found in the LICENSE file
#  * in the root directory of this software component.
#  * If no LICENSE file comes with this software, it is provided AS-IS.
#  *
#  ******************************************************************************

echo "TFM UPDATE started"
# Absolute path to this script
SCRIPT=$(readlink -f $0)
# Absolute path this script
SCRIPTPATH=`dirname $SCRIPT`
BINPATH="$SCRIPTPATH/bin"
PATH="/C/Program Files/STMicroelectronics/STM32Cube/STM32CubeProgrammer/bin/":$PATH
stm32programmercli="STM32_Programmer_CLI"
external_loader="C:\PROGRA~1\STMicroelectronics\STM32Cube\STM32CubeProgrammer\bin\ExternalLoader\MX25LM51245G_STM32L562E-DK.stldr"
connect_no_reset="-c port=SWD mode=UR -el $external_loader"
connect="-c port=SWD mode=UR --hardRst -el $external_loader"

echo "Write TFM_Appli Secure"
# part ot be updated according to flash_layout.h
slot0=
slot1=
slot2=
slot3=
its=
sst=
scratch=
nvcounter=
boot=
unused=

$stm32programmercli $connect -d $BINPATH/tfm_s_signed.bin $slot0 -v
echo "TFM_Appli Secure Written"
echo "Write TFM_Appli NonSecure"
$stm32programmercli $connect -d $BINPATH/tfm_ns_signed.bin $slot1 -v
echo "TFM_Appli NonSecure Written"
echo "Write TFM_SBSFU_Boot"
$stm32programmercli $connect -d $BINPATH/bl2.bin $boot -v
echo "TFM_SBSFU_Boot Written"
echo "TFM_UPDATE Done"