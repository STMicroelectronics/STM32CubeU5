#board name
board=B-U585I-IOT02A
#EWARM PATH
tfm_sbsfu_binary="../../../../../../Projects/"$board"/Applications/TFM/TFM_SBSFU_Boot/EWARM/"$board"/Exe/Project.bin"
#Cube IDE PATh
#tfm_sbsfu_binary="../../../../../../Projects/"$board"/Applications/TFM/TFM_SBSFU_Boot/STM32CubeIDE/Release/TFM_SBSFU_Boot.bin"
# KEIL PATH
#tfm_sbsfu_binary="../../../../../../Projects/"$board"/Applications/TFM/TFM_SBSFU_Boot/MDK-ARM/"$board"/Exe/Project.bin"
key="../../../../../../Projects/"$board"/Applications/TFM/TFM_SBSFU_Boot/Src/tfm_initial_attestation_key.pem"
python build.py cbor ./eat.txt ./eat.cbor
../scripts/check_iat -k $key  ./eat.cbor -p
python build.py boot $tfm_sbsfu_binary ./sbsfu.bin
sha256sum ./sbsfu.bin