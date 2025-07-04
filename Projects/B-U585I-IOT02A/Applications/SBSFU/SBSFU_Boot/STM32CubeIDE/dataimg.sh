#!/bin/bash
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
# args are optional
# arg1 is the build directory
# arg2 is the version i.e "2.1.0" (default is "1.0.0")
# arg3 is the binary type (nonsecure, secure) (default is both)
# arg4 : dependency with the other image
# "1.0.0" : version dependency with appli image
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

# Absolute path this script
projectdir=`dirname $SCRIPT`

signing=$3

userAppBinary=$projectdir/../../SBSFU_Appli/Binary

version="1.0.0"
dep="nodep"
dep_name=""
vername="noversion"

# default option value
if [ $# -ge 2 ] ; then
    version=$2
fi
if [ $# -ge 4 ]; then
    dep=$4
fi
if [ $dep != "nodep" ]; then
    depname="_"$dep
    if [ $signing == "nonsecure" ]; then
        option="-d (1,$dep)"
    fi
    if [ $signing == "secure" ]; then
        option="-d (0,$dep)"
    fi
fi
if [ $# -ge 5 ]; then
    vername=$5
fi
if [ $vername == "version" ] ; then
    ver="_"$version$depname
fi

sbsfu_s_init=$userAppBinary/sbsfu_s_data_init$ver.bin
sbsfu_ns_init=$userAppBinary/sbsfu_ns_data_init$ver.bin
sbsfu_s_sign=$userAppBinary/sbsfu_s_data_sign$ver.bin
sbsfu_ns_sign=$userAppBinary/sbsfu_ns_data_sign$ver.bin
sbsfu_s_enc_sign=$userAppBinary/sbsfu_s_data_enc_sign$ver.bin
sbsfu_ns_enc_sign=$userAppBinary/sbsfu_ns_data_enc_sign$ver.bin

# Field updated with sbsfu_boot postbuild
image_ns_size=
image_s_size=
primary_only=
crypto_scheme=
encrypted=
over_write=
flash_area_scratch_size=
# End of updated field

current_directory=`pwd`
echo $current_directory
cd $projectdir/../Src
sbsfu_key_dir=`pwd`
cd $current_directory
cd $projectdir/../../
envdir=`pwd`
cd $current_directory
#Make sure we have a Binary sub-folder in UserApp folder
if [ ! -e $userAppBinary ]; then
    mkdir $userAppBinary
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
if [ $crypto_scheme == 2 ]; then
    mode="ec256"
fi

#keys selection
if [ $mode == "ec256" ]; then
    key_s=$sbsfu_key_dir"/root-ec-p256.pem"
    key_ns=$sbsfu_key_dir"/root-ec-p256_1.pem"
    key_enc_pub=$sbsfu_key_dir"/enc-ec256-pub.pem"
fi

sbsfu_s=$projectdir"/../Src/s_data.bin"
sbsfu_ns=$projectdir"/../Src/ns_data.bin"

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
echo $signing" "$mode" "$option

val=16

if [ $over_write == "1" ]; then
    image_ns_sectors=""
    image_s_sectors=""
    flag=""
else
    let image_ns_sectors="((($image_ns_size+1) / $flash_area_scratch_size)+1)"
    let image_s_sectors="((($image_s_size+1) / $flash_area_scratch_size)+1)"
    flag="-M"
fi

if [ "$signing" != "secure" ]; then
    echo $signing init ns image signing
    command_init_ns=" sign -k "$key_ns" "$encrypt" -e little -S "$image_ns_size" "$flag" "$image_ns_sectors" -H 0x20 --pad-header "$option" -v "$version" --confirm --pad -s auto --align "$val" "$sbsfu_ns" "$sbsfu_ns_init
    "$imgtool" $command_init_ns >> $projectdir"/output.txt"
    ret=$?
    if [ $ret != 0 ]; then
        echo "dataimg.sh failed"
        exit 1
    fi
    echo $signing clear ns image signing
    command_clear_ns=" sign -k "$key_ns" -e little -S "$image_ns_size" "$flag" "$image_ns_sectors" -H 0x20 --pad-header "$option" -v "$version" -s auto --align "$val" "$sbsfu_ns" "$sbsfu_ns_sign
    "$imgtool" $command_clear_ns >> $projectdir"/output.txt"
    ret=$?
    if [ $ret != 0 ]; then
        echo "dataimg.sh failed"
        exit 1
    fi
    if [ $encrypted != "0" ]; then
        echo $signing enc ns image encrypting and signing
        command_enc_ns=" sign -k "$key_ns" -E "$key_enc_pub" -e little -S "$image_ns_size" "$flag" "$image_ns_sectors" -H 0x20 --pad-header "$option" -v "$version" -s auto --align "$val" "$sbsfu_ns" "$sbsfu_ns_enc_sign
        "$imgtool" $command_enc_ns >> $projectdir"/output.txt"
        ret=$?
        if [ $ret != 0 ]; then
            echo "dataimg.sh failed"
            exit 1
        fi
    fi
fi

if [ "$signing" != "nonsecure" ]; then
    echo $signing init s image signing
    command_init_s=" sign -k "$key_s" "$encrypt" -e little -S "$image_s_size" "$flag" "$image_s_sectors" -H 0x20 --pad-header "$option" -v "$version" --confirm --pad -s auto --align "$val" "$sbsfu_s" "$sbsfu_s_init
    "$imgtool" $command_init_s >> $projectdir"/output.txt"
    ret=$?
    if [ $ret != 0 ]; then
        echo "dataimg.sh failed"
        exit 1
    fi
    echo $signing clear s image signing
    command_clear_s=" sign -k "$key_s" -e little -S "$image_s_size" "$flag" "$image_s_sectors" -H 0x20 --pad-header "$option" -v "$version" -s auto --align "$val" "$sbsfu_s" "$sbsfu_s_sign
    "$imgtool" $command_clear_s >> $projectdir"/output.txt"
    ret=$?
    if [ $ret != 0 ]; then
        echo "dataimg.sh failed"
        exit 1
    fi
    if [ $encrypted != "0" ]; then
        echo $signing enc s image encrypting and signing
        command_enc_s=" sign -k "$key_s" -E "$key_enc_pub" -e little -S "$image_s_size" "$flag" "$image_s_sectors" -H 0x20 --pad-header "$option" -v "$version" -s auto --align "$val" "$sbsfu_s" "$sbsfu_s_enc_sign
        "$imgtool" $command_enc_s >> $projectdir"/output.txt"
        ret=$?
        if [ $ret != 0 ]; then
            echo "dataimg.sh failed"
            exit 1
        fi
    fi
fi

exit 0
