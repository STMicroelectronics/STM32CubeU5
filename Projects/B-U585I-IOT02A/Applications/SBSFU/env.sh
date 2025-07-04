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

if [[ "$OS" == "Windows_NT" ]]; then
  stm32programmercli="C:\Program Files\STMicroelectronics\STM32Cube\STM32CubeProgrammer\bin\STM32_Programmer_CLI.exe"
  stm32tpccli="C:\Program Files\STMicroelectronics\STM32Cube\STM32CubeProgrammer\bin\STM32TrustedPackageCreator_CLI.exe"
  imgtool="C:\Program Files\STMicroelectronics\STM32Cube\STM32CubeProgrammer\bin\Utilities\Windows\imgtool.exe"
elif [[ "$OS" == "Linux" ]]; then
  stm32programmercli_path=~/STMicroelectronics/STM32Cube/STM32CubeProgrammer/bin/
  PATH=$stm32programmercli_path:$PATH
  stm32programmercli="STM32_Programmer_CLI"
  stm32tpccli="STM32TrustedPackageCreator_CLI"
  imgtool="${stm32programmercli_path}Utilities/Linux/imgtool"
elif [[ "$OS" == "Darwin" ]]; then
  echo "OS MAC-OS detected"
  stm32programmercli=/Applications/STMicroelectronics/STM32Cube/STM32CubeProgrammer/STM32CubeProgrammer.app/Contents/MacOs/bin/STM32_Programmer_CLI
  stm32tpccli=/Applications/STMicroelectronics/STM32Cube/STM32CubeProgrammer/STM32TrustedPackageCreator.app/Contents/MacOs/bin/STM32TrustedPackageCreator_CLI
  imgtool=/Applications/STMicroelectronics/STM32Cube/STM32CubeProgrammer/STM32TrustedPackageCreator.app/Contents/MacOs/bin/Utilities/Mac/imgtool
else
  echo "OS not supported : >$OS<"
  return
fi
