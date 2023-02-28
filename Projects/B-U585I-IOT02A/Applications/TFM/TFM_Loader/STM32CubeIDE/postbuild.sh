#!/bin/bash -
# arg1 is the build directory
# arg2 is operation
projectdir=$1
operation=$2
secure_nsc=$projectdir"/../../../Secure_nsclib/secure_nsclib.o"
maxbytesize=350
loader_s=$projectdir"/../../Secure/Release/TFM_Loader_Secure.bin"
binarydir=$projectdir"/../../../Binary"
loader_ns=$projectdir"/TFM_Loader_NonSecure.bin"
loader=$binarydir"/loader.bin"
loader_ns_size=
loader_s_size=
current_directory=`pwd`
echo $current_directory
cd $projectdir"/../../../../../../../../Middlewares/Third_Party/mcuboot"
basedir=`pwd`
cd $current_directory
#Make sure we have a Binary sub-folder in UserApp folder
if [ ! -e $binarydir ]; then
echo "create Binary dir"
mkdir $binarydir
fi
imgtool=$basedir"/scripts/dist/imgtool/imgtool.exe"
uname | grep -i -e windows -e mingw
if [ $? == 0 ] && [   -e "$imgtool" ]; then
#line for window executable
echo Postbuild with windows executable
cmd=""
else
#line for python
echo Postbuild with python script
imgtool=$basedir"/scripts/imgtool/main.py"
cmd="python "
fi

if [ $operation == "secure" ]; then
ret=0
elif [ $operation == "nonsecure" ]; then
#according to secure_nsclib.o size select build with or without MCUBOOT_PRIMARY_ONLY
filesize=$(stat -c%s "$secure_nsc")

if [ $filesize -ge $maxbytesize ]; then
echo "loader with secure part (MCUBOOT_PRIMARY_ONLY defined)" >> $projectdir/output.txt
command=$cmd" "$imgtool" ass -f "$loader_s" -o "$loader_s_size" -i "$loader_ns_size" "$loader_ns" "$loader
$command  >> $projectdir"/output.txt"
ret=$?
else
#loader without secure part
echo "loader without secure part (MCUBOOT_PRIMARY_ONLY not defined)" >> $projectdir/output.txt
command=$cmd" "$imgtool" ass  -i "$loader_ns_size" "$loader_ns" "$loader
$command  >> $projectdir"/output.txt"
ret=$?
fi
fi
if [ $ret == 0 ]; then
echo "postbuild.sh done" $operation

#end of generate version for anti roll back test
else
echo "postbuild.sh failed"
fi
