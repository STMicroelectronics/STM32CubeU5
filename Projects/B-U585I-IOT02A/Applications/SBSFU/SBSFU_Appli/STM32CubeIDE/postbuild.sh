#!/bin/bash -
#=================================================================================================
# Managing HOST OS diversity : begin 
#=================================================================================================
OS=$(uname)

echo ${OS} | grep -i -e windows -e mingw >/dev/null
if [[ $? == 0 ]]; then
  echo "HOST OS : Windows detected"
  OS="Windows_NT"
elif [[ "$OS" == "Linux" ]]; then
  echo "HOST OS : Linux detected"
elif [[ "$OS" == "Darwin" ]]; then
  echo "HOST OS : MacOS detected"
else
  echo "!!!HOST OS not supported : >$OS<!!!"
  exit 1
fi

#=================================================================================================
# Managing HOST OS diversity : end 
#=================================================================================================
# arg1 is the build directory
# arg2 is the version
# arg3 is the binary type (1 nonsecure, 2 secure)
# arg4 dependency with the other image
# "1.0.0" : version dependency with other image
# "nodep": no dependency
# arg5 is version and dependency in output filename
# "version" : filename contains the version and dependency
# arg6 is to force python execution
# "python" : execute with python script

# Get Absolute path to this script use "readlink" for linux/windows and "stat" for Mac OS
if [[ "$OS" == "Darwin" ]]; then
  SCRIPT=$(stat -f $0)
else
  SCRIPT=$(readlink -f $0)
fi

projectdir=`dirname $SCRIPT`

version=$2
signing=$3
dep="nodep"
dep_name=""
vername="noversion"
# default option value
option="-e little -H 0x400 --pad-header -s auto --align 16"
if [ $# -eq 6 ] || [ $# -eq 4 ] || [ $# -eq 5 ]; then
dep=$4
fi
if [ $dep != "nodep" ]; then
    depname="_"$dep
    if [ $signing == "nonsecure" ]; then
    option=$option" -d (0,$dep)"
    fi
    if [ $signing == "secure" ]; then
    option=$option" -d (1,$dep)"
    fi
fi
if [ $# -eq 6 ] || [ $# -eq 5 ] ; then
vername=$5
fi
if [ $vername == "version" ] ; then
ver="_"$version$depname
fi

userAppBinary=$projectdir"/../Binary"
sbsfu_ns_init=$userAppBinary"/sbsfu_ns_app_init"$ver".bin"
sbsfu_s_init=$userAppBinary"/sbsfu_s_app_init"$ver".bin"
sbsfu_ns_sign=$userAppBinary"/sbsfu_ns_app_sign"$ver".bin"
sbsfu_s_sign=$userAppBinary"/sbsfu_s_app_sign"$ver".bin"
sbsfu_s_enc_sign=$userAppBinary"/sbsfu_s_app_enc_sign"$ver".bin"
sbsfu_ns_enc_sign=$userAppBinary"/sbsfu_ns_app_enc_sign"$ver".bin"
# variable for app_image_number = 1
sbsfu_init=$userAppBinary"/sbsfu_app_init"$ver".bin"
sbsfu_enc_sign=$userAppBinary"/sbsfu_app_enc_sign"$ver".bin"
sbsfu_sign=$userAppBinary"/sbsfu_app_sign"$ver".bin"
sbsfu=$userAppBinary"/sbsfu_app.bin"

# field updated with sbsfu_boot postbuild
image_ns_size=
image_s_size=
primary_only=
app_image_number=
crypto_scheme=
external_flash_enable=
ns_code_start=
encrypted=
over_write=
flash_area_scratch_size=
# end of updated field

current_directory=`pwd`
echo $current_directory
cd $projectdir"/../../SBSFU_Boot/Src"
sbsfu_key_dir=`pwd`
cd $current_directory
cd $projectdir/../../
envdir=`pwd`
cd "$current_directory"
#Make sure we have a Binary sub-folder in UserApp folder
if [ ! -e $userAppBinary ]; then
mkdir $userAppBinary
fi
if [ $signing == "secure" ]; then
if [ $app_image_number == 1 ]; then
# when image number is 1, image is assemble and sign during non secure postbuild
exit 0
fi
fi

source $envdir/env.sh

if [ ! -e "$imgtool" ];then
  echo ""
  echo "!!! WARNING : imgtool has not been found on your installation."
  echo ""
  echo "  Install CubeProgrammer on your machine in default path : ~/STMicroelectronics/STM32Cube/STM32CubeProgrammer"
  echo "  or "
  echo "  Update your $envdir/env.sh with the proper path."
  echo ""
  exit 0
fi

#sign mode
if [ $crypto_scheme == 2  ]; then
mode="ec256"
fi
#keys selection
if [ $mode == "ec256" ]; then
key_s=$sbsfu_key_dir"/root-ec-p256.pem"
key_ns=$sbsfu_key_dir"/root-ec-p256_1.pem"
key_enc_pub=$sbsfu_key_dir"/enc-ec256-pub.pem"
fi
sbsfu_s=$projectdir"/Secure/Release/SBSFU_Appli_Secure.bin"
sbsfu_ns=$projectdir"/NonSecure/Release/SBSFU_Appli_NonSecure.bin"
#encrypt option is for init image in swap mode
if [ $encrypted == "1" ]; then
encrypt="-E $key_enc_pub -c"
fi
if [ $primary_only == "1" ]; then
option=$option" --primary-only"
encrypt=""
fi

if [ $over_write == "1" ]; then
option=$option" --overwrite-only"
encrypt=""
fi

#signing
echo $signing" "$mode" "$option" app_image_number="$app_image_number

if [ $app_image_number == 1 ]; then
echo assemble image
command_ass=" ass -f "$sbsfu_s" -o "$image_s_size" -i 0x0 "$sbsfu_ns" "$sbsfu
echo "$imgtool" $command_ass
"$imgtool" $command_ass >> $projectdir"/output.txt"
ret=$?
if [ $ret != 0 ]; then
echo "postbuild.sh failed"
exit 1
fi
# sign assemble binary as secure image, slot size is image_ns_size
let "image_s_size=$image_ns_size"
signing=
sbsfu_s=$sbsfu
sbsfu_s_init=$sbsfu_init
sbsfu_s_sign=$sbsfu_sign
sbsfu_s_enc_sign=$sbsfu_enc_sign
fi

#nb sectors in image areas
if [ $over_write == "1" ]; then
    image_ns_sectors=""
    image_s_sectors=""
    flag=""
else
    let image_ns_sectors="((($image_ns_size+1) / $flash_area_scratch_size)+1)"
    let image_s_sectors="((($image_s_size+1) / $flash_area_scratch_size)+1)"
    flag="-M"
fi

echo $signing signing
if [ "$signing" == "nonsecure" ]; then
command_init=" sign -k "$key_ns" "$encrypt" -S "$image_ns_size" "$flag" "$image_ns_sectors" "$option" -v "$version" --confirm --pad "$sbsfu_ns" "$sbsfu_ns_init
else
command_init=" sign -k "$key_s" "$encrypt" -S "$image_s_size" "$flag" "$image_s_sectors" "$option" -v "$version" --confirm --pad "$sbsfu_s" "$sbsfu_s_init
fi
"$imgtool" $command_init >> $projectdir"/output.txt"
ret=$?
if [ $ret != 0 ]; then
echo "postbuild.sh failed"
exit 1
fi
if [ "$signing" == "nonsecure" ]; then
command_sign=" sign -k "$key_ns" -S "$image_ns_size" "$flag" "$image_ns_sectors" "$option" -v "$version" "$sbsfu_ns" "$sbsfu_ns_sign
else
command_sign=" sign -k "$key_s" -S "$image_s_size" "$flag" "$image_s_sectors" "$option" -v "$version" "$sbsfu_s" "$sbsfu_s_sign
fi
"$imgtool" $command_sign >> $projectdir"/output.txt"
ret=$?
if [ $ret != 0 ]; then
echo "postbuild.sh failed"
exit 1
fi
echo $signing encrypting
if [ "$signing" == "nonsecure" ]; then
command_enc=" sign -k "$key_ns" -E "$key_enc_pub" -S "$image_ns_size" "$flag" "$image_ns_sectors" "$option" -v "$version" "$sbsfu_ns" "$sbsfu_ns_enc_sign
else
command_enc=" sign -k "$key_s" -E "$key_enc_pub" -S "$image_s_size" "$flag" "$image_s_sectors" "$option" -v "$version" "$sbsfu_s" "$sbsfu_s_enc_sign
fi
"$imgtool" $command_enc >> $projectdir"/output.txt"
ret=$?
if [ $ret != 0 ]; then
echo "postbuild.sh failed"
exit 1
fi
exit 0
