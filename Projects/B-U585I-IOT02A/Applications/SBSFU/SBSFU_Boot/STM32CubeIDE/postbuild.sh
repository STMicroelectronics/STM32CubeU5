#!/bin/bash -
# Absolute path to this script
SCRIPT=$(readlink -f $0)
SCRIPT_PATH=`dirname $SCRIPT`
# Absolute path this script
projectdir=${SCRIPT_PATH}
cube_fw_path=${SCRIPT_PATH}/../../../../../../
source $projectdir/../../env.sh
preprocess_bl2_file=$projectdir/image_macros_preprocessed_bl2.c
hardening=$projectdir/hardening.sh
regression=$projectdir/regression.sh
update=$projectdir/SBSFU_UPDATE.sh
postbuild_appli=$projectdir/../../SBSFU_Appli/STM32CubeIDE/postbuild.sh
postbuild_loader=$projectdir/../../SBSFU_Loader/STM32CubeIDE/postbuild.sh
data_img_gen=$projectdir/dataimg.sh

applicfg="$cube_fw_path/Utilities/PC_Software/ROT_AppliConfig/dist/AppliCfg.exe"
uname | grep -i -e windows -e mingw
if [ $? == 0 ] && [ -e "$applicfg" ]; then
  #line for window executable
  echo "AppliCfg with windows executable"
  python=""
else
  # line for python
  echo "AppliCfg with python script"
  applicfg="$cube_fw_path/Utilities/PC_Software/ROT_AppliConfig/AppliCfg.py"
  #determine/check python version command
  python="python "
fi

page_size=0x2000

command="$python$applicfg flash --layout "$preprocess_bl2_file" -b secbootadd0 -m  RE_BL2_BOOT_ADDRESS  -d 0x80 "$regression
$command >> $projectdir"/output.txt"
ret=$?
if [ $ret != 0 ]; then
echo "postbuild.sh failed"
exit 1
fi
command="$python$applicfg flash --layout "$preprocess_bl2_file" -b flashsectnbr -m  RE_FLASH_PAGE_NBR "$regression
$command >> $projectdir"/output.txt"
ret=$?
if [ $ret != 0 ]; then
echo "postbuild.sh failed"
exit 1
fi
command="$python$applicfg flash --layout "$preprocess_bl2_file" -b nsbootadd -m  RE_BL2_BOOT_ADDRESS  -d 0x80 "$hardening
$command  >> $projectdir"/output.txt"
ret=$?
if [ $ret != 0 ]; then
echo "postbuild.sh failed"
exit 1
fi
command="$python$applicfg flash --layout "$preprocess_bl2_file" -b boot -m  RE_BL2_PERSO_ADDRESS "$update
$command  >> $projectdir"/output.txt"
ret=$?
if [ $ret != 0 ]; then
echo "postbuild.sh failed"
exit 1
fi
command="$python$applicfg flash --layout "$preprocess_bl2_file" -b slot0 -m  RE_IMAGE_FLASH_ADDRESS_SECURE "$update
$command  >> $projectdir"/output.txt"
ret=$?
if [ $ret != 0 ]; then
echo "postbuild.sh failed"
exit 1
fi
command="$python$applicfg flash --layout "$preprocess_bl2_file" -b slot1 -m  RE_IMAGE_FLASH_ADDRESS_NON_SECURE "$update
$command  >> $projectdir"/output.txt"
ret=$?
if [ $ret != 0 ]; then
echo "postbuild.sh failed"
exit 1
fi
command="$python$applicfg flash --layout "$preprocess_bl2_file" -b slot2 -m  RE_IMAGE_FLASH_SECURE_UPDATE "$update
$command  >> $projectdir"/output.txt"
ret=$?
if [ $ret != 0 ]; then
echo "postbuild.sh failed"
exit 1
fi
command="$python$applicfg flash --layout "$preprocess_bl2_file" -b slot3 -m  RE_IMAGE_FLASH_NON_SECURE_UPDATE "$update
$command  >> $projectdir"/output.txt"
ret=$?
if [ $ret != 0 ]; then
echo "postbuild.sh failed"
exit 1
fi
command="$python$applicfg flash --layout "$preprocess_bl2_file" -b slot4 -m  RE_IMAGE_FLASH_ADDRESS_DATA_SECURE "$update
$command  >> $projectdir"/output.txt"
ret=$?
if [ $ret != 0 ]; then
echo "postbuild.sh failed"
exit 1
fi
command="$python$applicfg flash --layout "$preprocess_bl2_file" -b slot5 -m  RE_IMAGE_FLASH_ADDRESS_DATA_NON_SECURE "$update
$command  >> $projectdir"/output.txt"
ret=$?
if [ $ret != 0 ]; then
echo "postbuild.sh failed"
exit 1
fi
command="$python$applicfg flash --layout "$preprocess_bl2_file" -b slot6 -m  RE_IMAGE_FLASH_DATA_SECURE_UPDATE "$update
$command  >> $projectdir"/output.txt"
ret=$?
if [ $ret != 0 ]; then
echo "postbuild.sh failed"
exit 1
fi
command="$python$applicfg flash --layout "$preprocess_bl2_file" -b slot7 -m  RE_IMAGE_FLASH_DATA_NON_SECURE_UPDATE "$update
$command  >> $projectdir"/output.txt"
ret=$?
if [ $ret != 0 ]; then
echo "postbuild.sh failed"
exit 1
fi
command="$python$applicfg flash --layout "$preprocess_bl2_file" -b wrp_start -m  RE_BL2_WRP_START -d $page_size --decimal "$hardening
$command  >> $projectdir"/output.txt"
ret=$?
if [ $ret != 0 ]; then
echo "postbuild.sh failed"
exit 1
fi
command="$python$applicfg flash --layout "$preprocess_bl2_file" -b wrp_end -m  RE_BL2_WRP_END -d $page_size --decimal "$hardening
$command  >> $projectdir"/output.txt"
ret=$?
if [ $ret != 0 ]; then
echo "postbuild.sh failed"
exit 1
fi
command="$python$applicfg flash --layout "$preprocess_bl2_file" -b sec1_end -m  RE_BL2_SEC1_END -d $page_size --decimal "$hardening
$command  >> $projectdir"/output.txt"
ret=$?
if [ $ret != 0 ]; then
echo "postbuild.sh failed"
exit 1
fi
command="$python$applicfg flash --layout "$preprocess_bl2_file" -b sec2_start -m  RE_BL2_SEC2_START -d $page_size --decimal "$hardening
$command  >> $projectdir"/output.txt"
ret=$?
if [ $ret != 0 ]; then
echo "postbuild.sh failed"
exit 1
fi
command="$python$applicfg flash --layout "$preprocess_bl2_file" -b sec2_end -m  RE_BL2_SEC2_END -d $page_size --decimal "$hardening
$command  >> $projectdir"/output.txt"
ret=$?
if [ $ret != 0 ]; then
echo "postbuild.sh failed"
exit 1
fi
command="$python$applicfg flash --layout "$preprocess_bl2_file" -b hdp_end -m  RE_BL2_HDP_END -d $page_size --decimal "$hardening
$command  >> $projectdir"/output.txt"
ret=$?
if [ $ret != 0 ]; then
echo "postbuild.sh failed"
exit 1
fi
command="$python$applicfg flash --layout "$preprocess_bl2_file" -b image_s_size -m  RE_IMAGE_FLASH_SECURE_IMAGE_SIZE "$postbuild_appli
$command  >> $projectdir"/output.txt"
ret=$?
if [ $ret != 0 ]; then
echo "postbuild.sh failed"
exit 1
fi
command="$python$applicfg flash --layout "$preprocess_bl2_file" -b image_s_size -m  RE_IMAGE_FLASH_SECURE_DATA_IMAGE_SIZE "$data_img_gen
$command  >> $projectdir"/output.txt"
ret=$?
if [ $ret != 0 ]; then
echo "postbuild.sh failed"
exit 1
fi
command="$python$applicfg flash --layout "$preprocess_bl2_file" -b image_ns_size -m  RE_IMAGE_FLASH_NON_SECURE_IMAGE_SIZE "$postbuild_appli
$command  >> $projectdir"/output.txt"
ret=$?
if [ $ret != 0 ]; then
echo "postbuild.sh failed"
exit 1
fi
command="$python$applicfg flash --layout "$preprocess_bl2_file" -b image_ns_size -m  RE_IMAGE_FLASH_NON_SECURE_DATA_IMAGE_SIZE "$data_img_gen
$command  >> $projectdir"/output.txt"
ret=$?
if [ $ret != 0 ]; then
echo "postbuild.sh failed"
exit 1
fi
command="$python$applicfg flash --layout "$preprocess_bl2_file" -b external_flash_enable -m  RE_EXTERNAL_FLASH_ENABLE --decimal "$postbuild_appli
$command  >> $projectdir"/output.txt"
ret=$?
if [ $ret != 0 ]; then
echo "postbuild.sh failed"
exit 1
fi
command="$python$applicfg flash --layout "$preprocess_bl2_file" -b ns_code_start -m  RE_CODE_START_NON_SECURE "$postbuild_appli
$command  >> $projectdir"/output.txt"
ret=$?
if [ $ret != 0 ]; then
echo "postbuild.sh failed"
exit 1
fi
command="$python$applicfg flash --layout "$preprocess_bl2_file" -b loader -m  RE_IMAGE_FLASH_LOADER_ADDRESS "$update
$command  >> $projectdir"/output.txt"
ret=$?
if [ $ret != 0 ]; then
echo "postbuild.sh failed"
exit 1
fi
command="$python$applicfg flash --layout "$preprocess_bl2_file" -b wrp_bank2_start -m  RE_LOADER_WRP_START -d $page_size --decimal "$hardening
$command  >> $projectdir"/output.txt"
ret=$?
if [ $ret != 0 ]; then
echo "postbuild.sh failed"
exit 1
fi
command="$python$applicfg flash --layout "$preprocess_bl2_file" -b wrp_bank2_end -m  RE_LOADER_WRP_END -d $page_size --decimal "$hardening
$command  >> $projectdir"/output.txt"
ret=$?
if [ $ret != 0 ]; then
echo "postbuild.sh failed"
exit 1
fi
command="$python$applicfg flash --layout "$preprocess_bl2_file" -b primary_only -m  RE_PRIMARY_ONLY --decimal "$postbuild_appli
$command  >> $projectdir"/output.txt"
ret=$?
if [ $ret != 0 ]; then
echo "postbuild.sh failed"
exit 1
fi
command="$python$applicfg flash --layout "$preprocess_bl2_file" -b primary_only -m  RE_PRIMARY_ONLY --decimal "$data_img_gen
$command  >> $projectdir"/output.txt"
ret=$?
if [ $ret != 0 ]; then
echo "postbuild.sh failed"
exit 1
fi
command="$python$applicfg flash --layout "$preprocess_bl2_file" -b app_image_number -m  RE_APP_IMAGE_NUMBER --decimal "$postbuild_appli
$command  >> $projectdir"/output.txt"
ret=$?
if [ $ret != 0 ]; then
echo "postbuild.sh failed"
exit 1
fi
command="$python$applicfg flash --layout "$preprocess_bl2_file" -b cfg_loader -m  RE_EXT_LOADER --decimal "$update
$command  >> $projectdir"/output.txt"
ret=$?
if [ $ret != 0 ]; then
echo "postbuild.sh failed"
exit 1
fi
command="$python$applicfg flash --layout "$preprocess_bl2_file" -b app_image_number -m  RE_APP_IMAGE_NUMBER --decimal "$update
$command  >> $projectdir"/output.txt"
ret=$?
if [ $ret != 0 ]; then
echo "postbuild.sh failed"
exit 1
fi
command="$python$applicfg flash --layout "$preprocess_bl2_file" -b s_data_image_number -m  RE_S_DATA_IMAGE_NUMBER --decimal "$update
$command  >> $projectdir"/output.txt"
ret=$?
if [ $ret != 0 ]; then
echo "postbuild.sh failed"
exit 1
fi
command="$python$applicfg flash --layout "$preprocess_bl2_file" -b ns_data_image_number -m  RE_NS_DATA_IMAGE_NUMBER --decimal "$update
$command  >> $projectdir"/output.txt"
ret=$?
if [ $ret != 0 ]; then
echo "postbuild.sh failed"
exit 1
fi
command="$python$applicfg flash --layout "$preprocess_bl2_file" -b crypto_scheme -m  RE_CRYPTO_SCHEME --decimal "$postbuild_appli
$command  >> $projectdir"/output.txt"
ret=$?
if [ $ret != 0 ]; then
echo "postbuild.sh failed"
exit 1
fi
command="$python$applicfg flash --layout "$preprocess_bl2_file" -b crypto_scheme -m  RE_CRYPTO_SCHEME --decimal "$data_img_gen
$command  >> $projectdir"/output.txt"
ret=$?
if [ $ret != 0 ]; then
echo "postbuild.sh failed"
exit 1
fi
command="$python$applicfg flash --layout "$preprocess_bl2_file" -b over_write -m  RE_OVER_WRITE --decimal "$postbuild_appli
$command  >> $projectdir"/output.txt"
ret=$?
if [ $ret != 0 ]; then
echo "postbuild.sh failed"
exit 1
fi
command="$python$applicfg flash --layout "$preprocess_bl2_file" -b over_write -m  RE_OVER_WRITE --decimal "$data_img_gen
$command  >> $projectdir"/output.txt"
ret=$?
if [ $ret != 0 ]; then
echo "postbuild.sh failed"
exit 1
fi
command="$python$applicfg flash --layout "$preprocess_bl2_file" -b encrypted -m  RE_ENCRYPTION --decimal "$postbuild_appli
$command  >> $projectdir"/output.txt"
ret=$?
if [ $ret != 0 ]; then
echo "postbuild.sh failed"
exit 1
fi
command="$python$applicfg flash --layout "$preprocess_bl2_file" -b encrypted -m  RE_ENCRYPTION --decimal "$data_img_gen
$command  >> $projectdir"/output.txt"
ret=$?
if [ $ret != 0 ]; then
echo "postbuild.sh failed"
exit 1
fi
command="$python$applicfg flash --layout "$preprocess_bl2_file" -b loader_ns_size -m  RE_LOADER_NS_CODE_SIZE --decimal "$postbuild_loader
$command  >> $projectdir"/output.txt"
ret=$?
if [ $ret != 0 ]; then
echo "postbuild.sh failed"
exit 1
fi
command="$python$applicfg flash --layout "$preprocess_bl2_file" -b loader_s_size -m  RE_LOADER_IMAGE_S_CODE_SIZE --decimal "$postbuild_loader
$command  >> $projectdir"/output.txt"
ret=$?
if [ $ret != 0 ]; then
echo "postbuild.sh failed"
exit 1
fi
command="$python$applicfg flash --layout "$preprocess_bl2_file" -b flash_area_scratch_size -m  RE_FLASH_AREA_SCRATCH_SIZE "$postbuild_appli
$command  >> $projectdir"/output.txt"
ret=$?
if [ $ret != 0 ]; then
echo "postbuild.sh failed"
exit 1
fi
command="$python$applicfg flash --layout "$preprocess_bl2_file" -b flash_area_scratch_size -m  RE_FLASH_AREA_SCRATCH_SIZE "$data_img_gen
$command  >> $projectdir"/output.txt"
ret=$?
if [ $ret != 0 ]; then
echo "postbuild.sh failed"
exit 1
fi

echo $regression" updated"
echo $hardening" updated"
echo $update" updated"
echo $postbuild_appli" updated"
echo $data_img_gen" updated"
echo $postbuild_loader" updated"
exit 0
