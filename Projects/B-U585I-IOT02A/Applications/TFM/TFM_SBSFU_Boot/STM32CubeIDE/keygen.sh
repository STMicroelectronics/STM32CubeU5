#!/bin/bash - 
# Absolute path to this script
SCRIPT=$(readlink -f $0)
# Absolute path this script
projectdir=`dirname $SCRIPT`
current_directory=`pwd`
cd $projectdir"/../Src"
tfm_key_dir=`pwd`
cd $projectdir"/../../../../../../Middlewares/Third_Party/mcuboot"
mcuboot_dir=`pwd`
cd $current_directory
tfm_keys=$tfm_key_dir"/keys.c"

#lines for keys backup
key_backup_dir=$tfm_key_dir"/"$(date +'%Y_%m_%d_%H_%M_%S')"_keybcp"
mkdir $key_backup_dir
ret=$?
if [ $ret != 0 ]; then
echo $cmdcreatedir" : failed" >> $projectdir"/output.txt"
exit 1
fi
cp -f $tfm_keys $key_backup_dir
ret=$?
if [ $ret != 0 ]; then
echo $cmdcreatedir" : failed" >> $projectdir"/output.txt"
exit 1
fi
keys_files=$tfm_key_dir"/*.pem"
cp -rf $keys_files $key_backup_dir
ret=$?
if [ $ret != 0 ]; then
echo $cmdcreatedir" : failed" >> $projectdir"/output.txt"
exit 1
fi

imgtool=$mcuboot_dir"/scripts/dist/imgtool/imgtool.exe"
uname | grep -i -e windows -e mingw
if [ $? == 0 ] && [ -e "$imgtool" ]; then
#line for window executable
echo Keygen with windows executable
python=""
else
#line for python
echo Keygen with python script
imgtool=$mcuboot_dir"/scripts/imgtool.py"
python="python "
fi

key_rsa_s=$tfm_key_dir"/root-rsa-2048.pem"
command_key=$python$imgtool" keygen -k "$key_rsa_s" -t rsa-2048"
$command_key
ret=$?
if [ $ret != 0 ]; then
echo $command_key" : failed" >> $projectdir"/output.txt"
exit 1
fi
command_key=$python$imgtool" getpub -k "$key_rsa_s 
$command_key > $tfm_keys
ret=$?
if [ $ret != 0 ]; then
echo $command_key" : failed" >> $projectdir"/output.txt"
exit 1
fi
key_rsa_ns=$tfm_key_dir"/root-rsa-2048_1.pem"
command_key=$python$imgtool" keygen -k "$key_rsa_ns" -t rsa-2048"
$command_key
ret=$?
if [ $ret != 0 ]; then
echo $command_key" : failed" >> $projectdir"/output.txt"
exit 1
fi

command_key=$python$imgtool" getpub -k "$key_rsa_ns
$command_key >> $tfm_keys
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
key_rsa_enc_priv=$tfm_key_dir"/enc-rsa2048-priv.pem"
key_rsa_enc_pub=$tfm_key_dir"/enc-rsa2048-pub.pem"
command_key=$python$imgtool" keygen -k "$key_rsa_enc_priv" -t rsa-2048 -e "$key_rsa_enc_pub
$command_key
ret=$?
if [ $ret != 0 ]; then
echo $command_key" : failed" >> $projectdir"/output.txt"
fi

command_key=$python$imgtool" getpriv  --minimal -k "$key_rsa_enc_priv
$command_key  >> $tfm_keys
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
key_rsa_2_s=$tfm_key_dir"/root-rsa-3072.pem"
command_key=$python$imgtool" keygen -k "$key_rsa_2_s" -t rsa-3072"
$command_key
ret=$?
if [ $ret != 0 ]; then
echo $command_key" : failed" >> $projectdir"/output.txt"
exit 1
fi
command_key=$python$imgtool" getpub -k "$key_rsa_2_s
$command_key  >> $tfm_keys

ret=$?
if [ $ret != 0 ]; then
echo $command_key" : failed" >> $projectdir"/output.txt"
exit 1
fi

key_rsa_2_ns=$tfm_key_dir"/root-rsa-3072_1.pem"
command_key=$python$imgtool" keygen -k "$key_rsa_2_ns" -t rsa-3072"
$command_key
ret=$?
if [ $ret != 0 ]; then
echo $command_key" : failed" >> $projectdir"/output.txt"
exit 1
fi

command_key=$python$imgtool" getpub -k "$key_rsa_2_ns
$command_key  >> $tfm_keys
ret=$?
if [ $ret != 0 ]; then
echo $command_key" : failed" >> $projectdir"/output.txt"
exit 1
fi

#ecc 256 auth key
key_ecc_s=$tfm_key_dir"/root-ec-p256.pem"
command_key=$python$imgtool" keygen -k "$key_ecc_s" -t ecdsa-p256"
$command_key
ret=$?
if [ $ret != 0 ]; then
echo $command_key" : failed" >> $projectdir"/output.txt"
exit 1
fi

command_key=$python$imgtool" getpub -k "$key_ecc_s
$command_key >> $tfm_keys
ret=$?
if [ $ret != 0 ]; then
echo $command_key" : failed" >> $projectdir"/output.txt"
exit 1
fi

#ecc 256 auth key
key_ecc_ns=$tfm_key_dir"/root-ec-p256_1.pem"
command_key=$python$imgtool" keygen -k "$key_ecc_ns" -t ecdsa-p256"
$command_key
ret=$?
if [ $ret != 0 ]; then
echo $command_key" : failed" >> $projectdir"/output.txt"
exit 1
fi
command_key=$python$imgtool" getpub -k "$key_ecc_ns
$command_key  >> $tfm_keys
ret=$?
if [ $ret != 0 ]; then
echo $command_key" : failed" >> $projectdir"/output.txt"
exit 1
fi

#priv key to encode images
key_ecc_enc_priv=$tfm_key_dir"/enc-ec256-priv.pem"
key_ecc_enc_pub=$tfm_key_dir"/enc-ec256-pub.pem"
command_key=$python$imgtool" keygen -k "$key_ecc_enc_priv" -t ecdsa-p256 -e "$key_ecc_enc_pub
$command_key
ret=$?
if [ $ret != 0 ]; then
echo $command_key" : failed" >> $projectdir"/output.txt"
exit 1
fi
command_key=$python$imgtool" getpriv  --minimal -k "$key_ecc_enc_priv
$command_key >> $tfm_keys
ret=$?
if [ $ret != 0 ]; then
echo $command_key" : failed" >> $projectdir"/output.txt"
exit 1
fi
#eat key
key_eat=$tfm_key_dir"/tfm_initial_attestation_key.pem"
command_key=$python$imgtool" keygen -k "$key_eat" -t ecdsa-p256-ssl"
$command_key
ret=$?
if [ $ret != 0 ]; then
echo $command_key" : failed" >> $projectdir"/output.txt"
exit 1
fi
command_key=$python$imgtool" getpriv --minimal -k "$key_eat
$command_key >> $tfm_keys
if [ $ret != 0 ]; then
echo $command_key" : failed" >> $projectdir"/output.txt"
exit 1
fi

#huk key
key_huk=$tfm_key_dir"/huk.bin"
command_key=$python$imgtool" keygen -k "$key_huk" -t sym32"
$command_key
if [ $ret != 0 ]; then
echo $command_key" : failed" >> $projectdir"/output.txt"
exit 1
fi
command_key=$python$imgtool" getpriv -k "$key_huk
$command_key >> $tfm_keys
if [ $ret != 0 ]; then
echo $command_key" : failed" >> $projectdir"/output.txt"
exit 1
fi
