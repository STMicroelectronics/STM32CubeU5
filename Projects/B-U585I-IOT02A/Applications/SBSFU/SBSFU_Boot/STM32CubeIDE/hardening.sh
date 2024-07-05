#!/bin/bash -
sec1_end=
sec2_start=
sec2_end=
wrp_start=
wrp_end=
hdp_end=
wrp_bank2_start=
wrp_bank2_end=
nsbootadd=
echo "hardening script started"
source ../../env.sh
bank2_secure="-ob SECWM2_PSTRT="$sec2_start" SECWM2_PEND="$sec2_end
connect="-c port=SWD mode=UR"
connect_no_reset="-c port=SWD mode=HotPlug"
wrp_loader="WRP2A_PSTRT="$wrp_bank2_start" WRP2A_PEND="$wrp_bank2_end
wrp_sbsfu="WRP1A_PSTRT="$wrp_start" WRP1A_PEND="$wrp_end
nsboot_add_set="NSBOOTADD0="$nsbootadd" NSBOOTADD1="$nsbootadd
write_protect_secure="-ob "$wrp_sbsfu" "$wrp_loader" SECWM1_PEND="$sec1_end" HDP1_PEND="$hdp_end" HDP1EN=1 "$nsboot_add_set" BOOT_LOCK=1"
"$stm32programmercli" $connect $bank2_secure
ret=$?
if [ $ret != 0 ]; then
  if [ "$1" != "AUTO" ]; then read -p "hardening script failed, press a key" -n1 -s; fi
  exit 1
fi
"$stm32programmercli" $connect $write_protect_secure
ret=$?
if [ $ret != 0 ]; then
  if [ "$1" != "AUTO" ]; then read -p "hardening script failed, press a key" -n1 -s; fi
  exit 1
fi
if [ "$1" != "AUTO" ]; then read -p "hardening script Done, press a key" -n1 -s; fi
exit 0