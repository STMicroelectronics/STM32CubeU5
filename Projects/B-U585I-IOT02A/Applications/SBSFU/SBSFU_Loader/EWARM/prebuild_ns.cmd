iccarm.exe --cpu=Cortex-M33 -D%2 -DBL2 -I%1\..\..\Linker %1\stm32u585xx_flash_ns.icf --silent --preprocess=ns %1\flash_ns.icf.i >> %1\output.txt 2>&1
