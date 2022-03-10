#!/bin/bash -
#  ******************************************************************************
#  * @attention
#  *
#  * Copyright (c) 2021 STMicroelectronics.
#  * All rights reserved.
#  *
#  * This software is licensed under terms that can be found in the LICENSE file
#  * in the root directory of this software component.
#  * If no LICENSE file comes with this software, it is provided AS-IS.
#  *
#  ******************************************************************************

echo "regression script started"
PATH="/C/Program Files/STMicroelectronics/STM32Cube/STM32CubeProgrammer/bin/":$PATH
stm32programmercli="STM32_Programmer_CLI"
# remove write protection
secbootadd0=0x180030
connect="-c port=SWD mode=UR --hardRst"
connect_no_reset="-c port=SWD mode=HotPlug"
rdp_0="-ob RDP=0xAA TZEN=1"
remove_bank1_protect="-ob SECWM1_PSTRT=127 SECWM1_PEND=0 WRP1A_PSTRT=127 WRP1A_PEND=0 WRP1B_PSTRT=127 WRP1B_PEND=0"
remove_bank2_protect="-ob SECWM2_PSTRT=127 SECWM2_PEND=0 WRP2A_PSTRT=127 WRP2A_PEND=0 WRP2B_PSTRT=127 WRP2B_PEND=0"
erase_all="-e all"
remove_hdp_protection="-ob HDP1_PEND=0 HDP1EN=0 HDP2_PEND=0 HDP2EN=0"
default_ob1="-ob SRAM2_RST=0 SECBOOTADD0="$secbootadd0" DBANK=1 SWAP_BANK=0 SECWM1_PSTRT=0 SECWM1_PEND=127"
default_ob2="-ob SECWM2_PSTRT=0 SECWM2_PEND=127"
echo "Regression to RDP 0 and  tzen=1"
$stm32programmercli $connect_no_reset $rdp_0
echo "Remove bank1 protection and erase all"
$stm32programmercli $connect $remove_bank1_protect $erase_all
echo "Remove bank2 protection and erase all"
$stm32programmercli $connect $remove_bank2_protect $erase_all
echo "Remove hdp protection"
$stm32programmercli $connect_no_reset $remove_hdp_protection
echo "Set default OB 1 (dual bank, swap bank, sram2 reset, secure entry point, bank 1 full secure)"
$stm32programmercli $connect_no_reset $default_ob1
echo "Set default OB 2 (bank 2 full secure)"
$stm32programmercli $connect_no_reset $default_ob2
echo "regression script Done"
