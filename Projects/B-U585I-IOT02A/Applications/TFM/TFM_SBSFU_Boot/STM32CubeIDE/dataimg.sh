#!/bin/bash
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

# Absolute path to this script
SCRIPT=$(readlink -f $0)
# Absolute path this script
projectdir=`dirname $SCRIPT`

signing=$3

userAppBinary=$projectdir/../../TFM_Appli/Binary

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
    if [ $signing == "secure" ]; then
        option="-d (0,$dep)"
    fi
    if [ $signing == "nonsecure" ]; then
        option="-d (1,$dep)"
    fi
fi
if [ $# -ge 5 ]; then
    vername=$5
fi
if [ $vername == "version" ] ; then
    ver="_"$version$depname
fi

sbsfu_s_init=$userAppBinary/tfm_s_data_init$ver.bin
sbsfu_ns_init=$userAppBinary/tfm_ns_data_init$ver.bin
sbsfu_s_sign=$userAppBinary/tfm_s_data_sign$ver.bin
sbsfu_ns_sign=$userAppBinary/tfm_ns_data_sign$ver.bin
sbsfu_s_enc_sign=$userAppBinary/tfm_s_data_enc_sign$ver.bin
sbsfu_ns_enc_sign=$userAppBinary/tfm_ns_data_enc_sign$ver.bin

# Field updated with tfm_boot postbuild
image_ns_size=
image_s_size=
primary_only=
crypto_scheme=
encrypted=
over_write=
# End of updated field

current_directory=`pwd`
echo $current_directory
cd $projectdir/../Src
sbsfu_key_dir=`pwd`
cd $current_directory
cd $projectdir/../../../../../../Middlewares/Third_Party/mcuboot
mcuboot_dir=`pwd`
cd $current_directory
#Make sure we have a Binary sub-folder in UserApp folder
if [ ! -e $userAppBinary ]; then
    mkdir $userAppBinary
fi

imgtool=$mcuboot_dir"/scripts/dist/imgtool/imgtool.exe"
uname | grep -i -e windows -e mingw
if [ $? == 0 ] && [ -e "$imgtool" ]; then
#line for window executable
    echo dataimg with windows executable
    python=
else
#line for python
    echo dataimg with python script
    imgtool=$mcuboot_dir"/scripts/imgtool/main.py"
    python="python "
fi

#sign mode
if [ $crypto_scheme == 0 ]; then
    mode="rsa2048"
fi
if [ $crypto_scheme == 1 ]; then
    mode="rsa3072"
fi
if [ $crypto_scheme == 2 ]; then
    mode="ec256"
fi

#keys selection
if [ $mode == "rsa2048" ]; then
    key_s=$sbsfu_key_dir"/root-rsa-2048.pem"
    key_ns=$sbsfu_key_dir"/root-rsa-2048_1.pem"
    key_enc_pub=$sbsfu_key_dir"/enc-rsa2048-pub.pem"
fi
if [ $mode == "rsa3072" ]; then
    key_s=$sbsfu_key_dir"/root-rsa-3072.pem"
    key_ns=$sbsfu_key_dir"/root-rsa-3072_1.pem"
    key_enc_pub=$sbsfu_key_dir"/enc-rsa2048-pub.pem"
fi
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

let "image_ns_sectors=$image_ns_size/0x2000"
let "image_s_sectors=$image_s_size/0x2000"

if [ "$signing" != "secure" ]; then
    echo $signing init ns image signing
    command_init_ns=$python$imgtool" sign -k "$key_ns" "$encrypt" -e little -S "$image_ns_size" -M "$image_ns_sectors" -H 0x20 --pad-header "$option" -v "$version" -s auto --align "$val" "$sbsfu_ns" "$sbsfu_ns_init
    $command_init_ns >> $projectdir"/output.txt"
    ret=$?
    if [ $ret != 0 ]; then
        echo "dataimg.sh failed"
        exit 1
    fi
    echo $signing clear ns image signing
    command_clear_ns=$python$imgtool" sign -k "$key_ns" -e little -S "$image_ns_size" -M "$image_ns_sectors" -H 0x20 --pad-header "$option" -v "$version" -s auto --align "$val" "$sbsfu_ns" "$sbsfu_ns_sign
    $command_clear_ns >> $projectdir"/output.txt"
    ret=$?
    if [ $ret != 0 ]; then
        echo "dataimg.sh failed"
        exit 1
    fi
    if [ $encrypted != "0" ]; then
        echo $signing enc ns image encrypting and signing
        command_enc_ns=$python$imgtool" sign -k "$key_ns" -E "$key_enc_pub" -e little -S "$image_ns_size" -M "$image_ns_sectors" -H 0x20 --pad-header "$option" -v "$version" -s auto --align "$val" "$sbsfu_ns" "$sbsfu_ns_enc_sign
        $command_enc_ns >> $projectdir"/output.txt"
        ret=$?
        if [ $ret != 0 ]; then
            echo "dataimg.sh failed"
            exit 1
        fi
    fi
fi

if [ "$signing" != "nonsecure" ]; then
    echo $signing init s image signing
    command_init_s=$python$imgtool" sign -k "$key_s" "$encrypt" -e little -S "$image_s_size" -M "$image_s_sectors" -H 0x20 --pad-header "$option" -v "$version" -s auto --align "$val" "$sbsfu_s" "$sbsfu_s_init
    $command_init_s >> $projectdir"/output.txt"
    ret=$?
    if [ $ret != 0 ]; then
        echo "dataimg.sh failed"
        exit 1
    fi
    echo $signing clear s image signing
    command_clear_s=$python$imgtool" sign -k "$key_s" -e little -S "$image_s_size" -M "$image_s_sectors" -H 0x20 --pad-header "$option" -v "$version" -s auto --align "$val" "$sbsfu_s" "$sbsfu_s_sign
    $command_clear_s >> $projectdir"/output.txt"
    ret=$?
    if [ $ret != 0 ]; then
        echo "dataimg.sh failed"
        exit 1
    fi
    if [ $encrypted != "0" ]; then
        echo $signing enc s image encrypting and signing
        command_enc_s=$python$imgtool" sign -k "$key_s" -E "$key_enc_pub" -e little -S "$image_s_size" -M "$image_s_sectors" -H 0x20 --pad-header "$option" -v "$version" -s auto --align "$val" "$sbsfu_s" "$sbsfu_s_enc_sign
        $command_enc_s >> $projectdir"/output.txt"
        ret=$?
        if [ $ret != 0 ]; then
            echo "dataimg.sh failed"
            exit 1
        fi
    fi
fi

exit 0
