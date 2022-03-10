#!/bin/bash -
#  ******************************************************************************
#  * @attention
#  *
#  * Copyright (c) 2019 STMicroelectronics.
#  * All rights reserved.
#  *
#  * This software is licensed under terms that can be found in the LICENSE file
#  * in the root directory of this software component.
#  * If no LICENSE file comes with this software, it is provided AS-IS.
#  *
#  ******************************************************************************

# Absolute path to this script
SCRIPT=$(readlink -f $0)
# Absolute path this script
projectdir=`dirname $SCRIPT`
source $projectdir/preprocess.sh
# the file to preprocess is generated and present outside of install dir
bl2_file_to_preprocess=$projectdir/image_macros_to_preprocess_bl2.c
preprocess_bl2_file=$projectdir/image_macros_preprocessed_bl2.c
regressionsh=$projectdir/regression.sh
updatesh=$projectdir/TFM_UPDATE.sh

basedir=$projectdir
echo preprocess bl2 file
preprocess $projectdir $bl2_file_to_preprocess $preprocess_bl2_file
stm_tool=$basedir"/scripts/stm_tool.py"
#determine/check python version command
cmd="python3"
$cmd --version  &> /dev/null
ret=$?
if [ $ret != 0 ]; then
  cmd="python"
  $cmd --version  &> /dev/null
  ret=$?
  if [ $ret != 0 ]; then
    echo "This script requires python 3.0 or greater"
    exit 1
  fi
  ver=$(python -V 2>&1 | sed 's/.* \([0-9]\).\([0-9]\).*/\1\2/')
  if [ "$ver" -lt "30" ]; then
    echo "This script requires python 3.0 or greater"
    exit 1
  fi
fi

command=$cmd" "$stm_tool" flash --layout "$preprocess_bl2_file" -b secbootadd0 -m  RE_BL2_BOOT_ADDRESS  -d 0x80  -s 0 "$regressionsh
$command  >> $projectdir"/output.txt"
ret=$?
if [ $ret != 0 ]; then
echo "postbuild.sh failed"
echo $command
exit 1
fi

command=$cmd" "$stm_tool" flash --layout "$preprocess_bl2_file" -b boot -m  RE_BL2_PERSO_ADDRESS -s 0 "$updatesh
$command  >> $projectdir"/output.txt"
ret=$?
if [ $ret != 0 ]; then
echo "postbuild.sh failed"
echo $command
exit 1
fi

command=$cmd" "$stm_tool" flash --layout "$preprocess_bl2_file" -b slot0 -m  RE_IMAGE_FLASH_ADDRESS_SECURE -s 0 "$updatesh
$command  >> $projectdir"/output.txt"
ret=$?
if [ $ret != 0 ]; then
echo "postbuild.sh failed"
echo $command
exit 1
fi

echo $updatebat" updated"
command=$cmd" "$stm_tool" flash --layout "$preprocess_bl2_file" -b slot1 -m  RE_IMAGE_FLASH_ADDRESS_NON_SECURE -s 0 "$updatesh
$command  >> $projectdir"/output.txt"
ret=$?
if [ $ret != 0 ]; then
echo "postbuild.sh failed"
echo $command
exit 1
fi
echo $updatebat" updated"
command=$cmd" "$stm_tool" flash --layout "$preprocess_bl2_file" -b slot2 -m  RE_IMAGE_FLASH_SECURE_UPDATE -s 0 "$updatesh
$command  >> $projectdir"/output.txt"
ret=$?
if [ $ret != 0 ]; then
echo "postbuild.sh failed"
echo $command
exit 1
fi
echo $updatebat" updated"
command=$cmd" "$stm_tool" flash --layout "$preprocess_bl2_file" -b slot3 -m  RE_IMAGE_FLASH_NON_SECURE_UPDATE -s 0 "$updatesh
$command  >> $projectdir"/output.txt"
ret=$?
if [ $ret != 0 ]; then
echo "postbuild.sh failed"
echo $command
exit 1
fi
echo $updatebat" updated"
command=$cmd" "$stm_tool" flash --layout "$preprocess_bl2_file" -b scratch -m  RE_IMAGE_FLASH_SCRATCH -s 0 "$updatesh
$command  >> $projectdir"/output.txt"
ret=$?
if [ $ret != 0 ]; then
echo "postbuild.sh failed"
echo $command
exit 1
fi
echo $updatebat" updated"
command=$cmd" "$stm_tool" flash --layout "$preprocess_bl2_file" -b nvcounter -m  RE_IMAGE_FLASH_NV_COUNTERS -s 0 "$updatesh
$command  >> $projectdir"/output.txt"
ret=$?
if [ $ret != 0 ]; then
echo "postbuild.sh failed"
echo $command
exit 1
fi
echo $updatebat" updated"
command=$cmd" "$stm_tool" flash --layout "$preprocess_bl2_file" -b sst -m  RE_IMAGE_FLASH_NV_PS -s 0 "$updatesh
$command  >> $projectdir"/output.txt"
ret=$?
if [ $ret != 0 ]; then
echo "postbuild.sh failed"
echo $command
exit 1
fi
echo $updatebat" updated"
command=$cmd" "$stm_tool" flash --layout "$preprocess_bl2_file" -b its -m  RE_IMAGE_FLASH_NV_ITS -s 0 "$updatesh
$command  >> $projectdir"/output.txt"
ret=$?
if [ $ret != 0 ]; then
echo "postbuild.sh failed"
echo $command
exit 1
fi
echo $updatebat" updated"
command=$cmd" "$stm_tool" flash --layout "$preprocess_bl2_file" -b unused -m  RE_IMAGE_FLASH_UNUSED -s 0 "$updatesh
$command  >> $projectdir"/output.txt"
ret=$?
if [ $ret != 0 ]; then
echo "postbuild.sh failed"
echo $command
exit 1
fi
