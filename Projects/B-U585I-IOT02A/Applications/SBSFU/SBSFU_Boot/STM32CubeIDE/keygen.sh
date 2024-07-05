#!/bin/bash -
# Absolute path to this script
SCRIPT=$(readlink -f $0)
# Absolute path this script
projectdir=`dirname $SCRIPT`
current_directory=`pwd`
cd $projectdir"/../Src"
sbsfu_key_dir=`pwd`
cd $projectdir/../../
envdir=`pwd`
cd $current_directory
sbsfu_keys=$sbsfu_key_dir"/keys.c"

#lines for keys backup
key_backup_dir=$sbsfu_key_dir"/"$(date +'%Y_%m_%d_%H_%M_%S')"_keybcp"
mkdir $key_backup_dir
ret=$?
if [ $ret != 0 ]; then
echo $cmdcreatedir" : failed" >> $projectdir"/output.txt"
exit 1
fi
cp -f $sbsfu_keys $key_backup_dir
ret=$?
if [ $ret != 0 ]; then
echo $cmdcreatedir" : failed" >> $projectdir"/output.txt"
exit 1
fi
keys_files=$sbsfu_key_dir"/*.pem"
cp -rf $keys_files $key_backup_dir
ret=$?
if [ $ret != 0 ]; then
echo $cmdcreatedir" : failed" >> $projectdir"/output.txt"
exit 1
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

key_rsa_s=$sbsfu_key_dir"/root-rsa-2048.pem"
command_key=" keygen -k "$key_rsa_s" -t rsa-2048"
"$imgtool" $command_key
ret=$?
if [ $ret != 0 ]; then
echo $command_key" : failed" >> $projectdir"/output.txt"
exit 1
fi
command_key=" getpub -k "$key_rsa_s
"$imgtool" $command_key > $sbsfu_keys
ret=$?
if [ $ret != 0 ]; then
echo $command_key" : failed" >> $projectdir"/output.txt"
exit 1
fi
key_rsa_ns=$sbsfu_key_dir"/root-rsa-2048_1.pem"
command_key=" keygen -k "$key_rsa_ns" -t rsa-2048"
"$imgtool" $command_key
ret=$?
if [ $ret != 0 ]; then
echo $command_key" : failed" >> $projectdir"/output.txt"
exit 1
fi

command_key=" getpub -k "$key_rsa_ns
"$imgtool" $command_key >> $sbsfu_keys
ret=$?
if [ $ret != 0 ]; then
echo $command_key" : failed" >> $projectdir"/output.txt"
exit 1
fi

#priv key to encode images
cnt=0
ret=1
while [ "$cnt" -lt "2" ] && [ $ret != 0 ]
do
key_rsa_enc_priv=$sbsfu_key_dir"/enc-rsa2048-priv.pem"
key_rsa_enc_pub=$sbsfu_key_dir"/enc-rsa2048-pub.pem"
command_key=" keygen -k "$key_rsa_enc_priv" -t rsa-2048 -e "$key_rsa_enc_pub
"$imgtool" $command_key
ret=$?
if [ $ret != 0 ]; then
echo $command_key" : failed" >> $projectdir"/output.txt"
fi

command_key=" getpriv  --minimal -k "$key_rsa_enc_priv
"$imgtool" $command_key  >> $sbsfu_keys
ret=$?
if [ $ret != 0 ]; then
echo $command_key" : failed" >> $projectdir"/output.txt"
fi
cnt=$((cnt+1))
done
if [ $ret != 0 ]; then
exit 1
fi
#rsa 3072 auth key
key_rsa_2_s=$sbsfu_key_dir"/root-rsa-3072.pem"
command_key=" keygen -k "$key_rsa_2_s" -t rsa-3072"
"$imgtool" $command_key
ret=$?
if [ $ret != 0 ]; then
echo $command_key" : failed" >> $projectdir"/output.txt"
exit 1
fi
command_key=" getpub -k "$key_rsa_2_s
"$imgtool" $command_key  >> $sbsfu_keys

ret=$?
if [ $ret != 0 ]; then
echo $command_key" : failed" >> $projectdir"/output.txt"
exit 1
fi

key_rsa_2_ns=$sbsfu_key_dir"/root-rsa-3072_1.pem"
command_key=" keygen -k "$key_rsa_2_ns" -t rsa-3072"
"$imgtool" $command_key
ret=$?
if [ $ret != 0 ]; then
echo $command_key" : failed" >> $projectdir"/output.txt"
exit 1
fi

command_key=" getpub -k "$key_rsa_2_ns
"$imgtool" $command_key  >> $sbsfu_keys
ret=$?
if [ $ret != 0 ]; then
echo $command_key" : failed" >> $projectdir"/output.txt"
exit 1
fi

#ecc 256 auth key
key_ecc_s=$sbsfu_key_dir"/root-ec-p256.pem"
command_key=" keygen -k "$key_ecc_s" -t ecdsa-p256"
"$imgtool" $command_key
ret=$?
if [ $ret != 0 ]; then
echo $command_key" : failed" >> $projectdir"/output.txt"
exit 1
fi

command_key=" getpub -k "$key_ecc_s
"$imgtool" $command_key >> $sbsfu_keys
ret=$?
if [ $ret != 0 ]; then
echo $command_key" : failed" >> $projectdir"/output.txt"
exit 1
fi

#ecc 256 auth key
key_ecc_ns=$sbsfu_key_dir"/root-ec-p256_1.pem"
command_key=" keygen -k "$key_ecc_ns" -t ecdsa-p256"
"$imgtool" $command_key
ret=$?
if [ $ret != 0 ]; then
echo $command_key" : failed" >> $projectdir"/output.txt"
exit 1
fi
command_key=" getpub -k "$key_ecc_ns
"$imgtool" $command_key  >> $sbsfu_keys
ret=$?
if [ $ret != 0 ]; then
echo $command_key" : failed" >> $projectdir"/output.txt"
exit 1
fi

#priv key to encode images
key_ecc_enc_priv=$sbsfu_key_dir"/enc-ec256-priv.pem"
key_ecc_enc_pub=$sbsfu_key_dir"/enc-ec256-pub.pem"
command_key=" keygen -k "$key_ecc_enc_priv" -t ecdsa-p256 -e "$key_ecc_enc_pub
"$imgtool" $command_key
ret=$?
if [ $ret != 0 ]; then
echo $command_key" : failed" >> $projectdir"/output.txt"
exit 1
fi
command_key=" getpriv  --minimal -k "$key_ecc_enc_priv
"$imgtool" $command_key >> $sbsfu_keys
ret=$?
if [ $ret != 0 ]; then
echo $command_key" : failed" >> $projectdir"/output.txt"
exit 1
fi
