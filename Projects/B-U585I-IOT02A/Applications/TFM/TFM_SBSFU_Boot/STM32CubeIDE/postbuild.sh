#!/bin/bash - 
# Absolute path to this script
SCRIPT=$(readlink -f $0)
# Absolute path this script
projectdir=`dirname $SCRIPT`
preprocess_bl2_file=$projectdir/image_macros_preprocessed_bl2.c
hardening=$projectdir/hardening.sh
regression=$projectdir/regression.sh
update=$projectdir/TFM_UPDATE.sh
postbuild_appli=$projectdir/../../TFM_Appli/STM32CubeIDE/postbuild.sh
postbuild_loader=$projectdir/../../TFM_Loader/STM32CubeIDE/postbuild.sh
page_size=0x2000
current_directory=`pwd`
echo $current_directory
cd $projectdir"/../../../../../../Middlewares/Third_Party/mcuboot"
mcuboot=`pwd`
cd $current_directory

imgtool=$mcuboot"/scripts/dist/imgtool/imgtool.exe"
uname | grep -i -e windows -e mingw
if [ $? == 0 ] && [ -e "$imgtool" ]; then
#line for window executable
echo Postbuild with windows executable
cmd=""
else
#line for python
echo Postbuild with python script
imgtool=$mcuboot"/scripts/imgtool.py"
#determine/check python version command
cmd="python "
fi

command=$cmd" "$imgtool" flash --layout "$preprocess_bl2_file" -b secbootadd0 -m  RE_BL2_BOOT_ADDRESS  -d 0x80  -s 0 "$regression
$command >> $projectdir"/output.txt"
ret=$?
if [ $ret != 0 ]; then
echo "postbuild.sh failed"
exit 1
fi
command=$cmd" "$imgtool" flash --layout "$preprocess_bl2_file" -b nsbootadd -m  RE_BL2_BOOT_ADDRESS  -d 0x80  -s 0 "$hardening
$command  >> $projectdir"/output.txt"
ret=$?
if [ $ret != 0 ]; then
echo "postbuild.sh failed"
exit 1
fi
command=$cmd" "$imgtool" flash --layout "$preprocess_bl2_file" -b boot -m  RE_BL2_PERSO_ADDRESS -s 0 "$update
$command  >> $projectdir"/output.txt"
ret=$?
if [ $ret != 0 ]; then
echo "postbuild.sh failed"
exit 1
fi
command=$cmd" "$imgtool" flash --layout "$preprocess_bl2_file" -b slot0 -m  RE_IMAGE_FLASH_ADDRESS_SECURE -s 0 "$update
$command  >> $projectdir"/output.txt"
ret=$?
if [ $ret != 0 ]; then
echo "postbuild.sh failed"
exit 1
fi
command=$cmd" "$imgtool" flash --layout "$preprocess_bl2_file" -b slot1 -m  RE_IMAGE_FLASH_ADDRESS_NON_SECURE -s 0 "$update
$command  >> $projectdir"/output.txt"
ret=$?
if [ $ret != 0 ]; then
echo "postbuild.sh failed"
exit 1
fi
command=$cmd" "$imgtool" flash --layout "$preprocess_bl2_file" -b slot2 -m  RE_IMAGE_FLASH_SECURE_UPDATE -s 0 "$update
$command  >> $projectdir"/output.txt"
ret=$?
if [ $ret != 0 ]; then
echo "postbuild.sh failed"
exit 1
fi
command=$cmd" "$imgtool" flash --layout "$preprocess_bl2_file" -b slot3 -m  RE_IMAGE_FLASH_NON_SECURE_UPDATE -s 0 "$update
$command  >> $projectdir"/output.txt"
ret=$?
if [ $ret != 0 ]; then
echo "postbuild.sh failed"
exit 1
fi
command=$cmd" "$imgtool" flash --layout "$preprocess_bl2_file" -b wrp_start -m  RE_BL2_WRP_START -d $page_size -s 0 -p 0 "$hardening
$command  >> $projectdir"/output.txt"
ret=$?
if [ $ret != 0 ]; then
echo "postbuild.sh failed"
exit 1
fi
command=$cmd" "$imgtool" flash --layout "$preprocess_bl2_file" -b wrp_end -m  RE_BL2_WRP_END -d $page_size -s 0 -p 0 "$hardening
$command  >> $projectdir"/output.txt"
ret=$?
if [ $ret != 0 ]; then
echo "postbuild.sh failed"
exit 1
fi
command=$cmd" "$imgtool" flash --layout "$preprocess_bl2_file" -b sec1_end -m  RE_BL2_SEC1_END -d $page_size -s 0 -p 0 "$hardening
$command  >> $projectdir"/output.txt"
ret=$?
if [ $ret != 0 ]; then
echo "postbuild.sh failed"
exit 1
fi
command=$cmd" "$imgtool" flash --layout "$preprocess_bl2_file" -b sec2_start -m  RE_BL2_SEC2_START -d $page_size -s 0 -p 0 "$hardening
$command  >> $projectdir"/output.txt"
ret=$?
if [ $ret != 0 ]; then
echo "postbuild.sh failed"
exit 1
fi
command=$cmd" "$imgtool" flash --layout "$preprocess_bl2_file" -b sec2_end -m  RE_BL2_SEC2_END -d $page_size -s 0 -p 0 "$hardening
$command  >> $projectdir"/output.txt"
ret=$?
if [ $ret != 0 ]; then
echo "postbuild.sh failed"
exit 1
fi
command=$cmd" "$imgtool" flash --layout "$preprocess_bl2_file" -b hdp_end -m  RE_BL2_HDP_END -d $page_size -s 0 -p 0 "$hardening
$command  >> $projectdir"/output.txt"
ret=$?
if [ $ret != 0 ]; then
echo "postbuild.sh failed"
exit 1
fi
command=$cmd" "$imgtool" flash --layout "$preprocess_bl2_file" -b image_s_size -m  RE_IMAGE_FLASH_SECURE_IMAGE_SIZE -s 0 "$postbuild_appli
$command  >> $projectdir"/output.txt"
ret=$?
if [ $ret != 0 ]; then
echo "postbuild.sh failed"
exit 1
fi
command=$cmd" "$imgtool" flash --layout "$preprocess_bl2_file" -b image_ns_size -m  RE_IMAGE_FLASH_NON_SECURE_IMAGE_SIZE -s 0 "$postbuild_appli
$command  >> $projectdir"/output.txt"
ret=$?
if [ $ret != 0 ]; then
echo "postbuild.sh failed"
exit 1
fi
command=$cmd" "$imgtool" flash --layout "$preprocess_bl2_file" -b external_flash_enable -m  RE_EXTERNAL_FLASH_ENABLE -s 0 -p 0 "$postbuild_appli
$command  >> $projectdir"/output.txt"
ret=$?
if [ $ret != 0 ]; then
echo "postbuild.sh failed"
exit 1
fi
command=$cmd" "$imgtool" flash --layout "$preprocess_bl2_file" -b ns_code_start -m  RE_CODE_START_NON_SECURE -s 0 "$postbuild_appli
$command  >> $projectdir"/output.txt"
ret=$?
if [ $ret != 0 ]; then
echo "postbuild.sh failed"
exit 1
fi
command=$cmd" "$imgtool" flash --layout "$preprocess_bl2_file" -b loader -m  RE_IMAGE_FLASH_LOADER_ADDRESS -s 0 "$update
$command  >> $projectdir"/output.txt"
ret=$?
if [ $ret != 0 ]; then
echo "postbuild.sh failed"
exit 1
fi
command=$cmd" "$imgtool" flash --layout "$preprocess_bl2_file" -b wrp_bank2_start -m  RE_LOADER_WRP_START -d $page_size -s 0 -p 0 "$hardening
$command  >> $projectdir"/output.txt"
ret=$?
if [ $ret != 0 ]; then
echo "postbuild.sh failed"
exit 1
fi
command=$cmd" "$imgtool" flash --layout "$preprocess_bl2_file" -b wrp_bank2_end -m  RE_LOADER_WRP_END -d $page_size -s 0 -p 0 "$hardening
$command  >> $projectdir"/output.txt"
ret=$?
if [ $ret != 0 ]; then
echo "postbuild.sh failed"
exit 1
fi
command=$cmd" "$imgtool" flash --layout "$preprocess_bl2_file" -b primary_only -m  RE_PRIMARY_ONLY -s 0 -p 0 "$postbuild_appli
$command  >> $projectdir"/output.txt"
ret=$?
if [ $ret != 0 ]; then
echo "postbuild.sh failed"
exit 1
fi
command=$cmd" "$imgtool" flash --layout "$preprocess_bl2_file" -b image_number -m  RE_IMAGE_NUMBER -s 0 -p 0 "$postbuild_appli
$command  >> $projectdir"/output.txt"
ret=$?
if [ $ret != 0 ]; then
echo "postbuild.sh failed"
exit 1
fi
command=$cmd" "$imgtool" flash --layout "$preprocess_bl2_file" -b cfg_loader -m  RE_EXT_LOADER -s 0 -p 0 "$update
$command  >> $projectdir"/output.txt"
ret=$?
if [ $ret != 0 ]; then
echo "postbuild.sh failed"
exit 1
fi
command=$cmd" "$imgtool" flash --layout "$preprocess_bl2_file" -b image_number -m  RE_IMAGE_NUMBER -s 0 -p 0 "$update
$command  >> $projectdir"/output.txt"
ret=$?
if [ $ret != 0 ]; then
echo "postbuild.sh failed"
exit 1
fi
command=$cmd" "$imgtool" flash --layout "$preprocess_bl2_file" -b crypto_scheme -m  RE_CRYPTO_SCHEME -s 0 -p 0 "$postbuild_appli
$command  >> $projectdir"/output.txt"
ret=$?
if [ $ret != 0 ]; then
echo "postbuild.sh failed"
exit 1
fi
command=$cmd" "$imgtool" flash --layout "$preprocess_bl2_file" -b over_write -m  RE_OVER_WRITE -s 0 -p 0 "$postbuild_appli
$command  >> $projectdir"/output.txt"
ret=$?
if [ $ret != 0 ]; then
echo "postbuild.sh failed"
exit 1
fi
command=$cmd" "$imgtool" flash --layout "$preprocess_bl2_file" -b encrypted -m  RE_ENCRYPTION -s 0 -p 0 "$postbuild_appli
$command  >> $projectdir"/output.txt"
ret=$?
if [ $ret != 0 ]; then
echo "postbuild.sh failed"
exit 1
fi
command=$cmd" "$imgtool" flash --layout "$preprocess_bl2_file" -b loader_ns_size -m  RE_LOADER_NS_CODE_SIZE -s 0 -p 0 "$postbuild_loader
$command  >> $projectdir"/output.txt"
ret=$?
if [ $ret != 0 ]; then
echo "postbuild.sh failed"
exit 1
fi
command=$cmd" "$imgtool" flash --layout "$preprocess_bl2_file" -b loader_s_size -m  RE_LOADER_IMAGE_S_CODE_SIZE -s 0 -p 0 "$postbuild_loader
$command  >> $projectdir"/output.txt"
ret=$?
if [ $ret != 0 ]; then
echo "postbuild.sh failed"
exit 1
fi
echo $hardeningbat" updated"
echo $hardening" updated"
echo $postbuild_appli" updated"
exit 0
