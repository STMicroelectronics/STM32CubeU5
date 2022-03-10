iccarm.exe --cpu=Cortex-M33 -D%2 -DBL2  -I%1\..\..\Linker  %1\stm32u585xx_bl2.icf  --silent --preprocess=ns %1\bl2.icf.i > %1\output.txt 2>&1
iccarm.exe --cpu=Cortex-M33 -D%2 -DBL2  -I%1\..\..\Linker -I%1\..\Inc %1\..\src\image_macros_to_preprocess_bl2.c  --silent --preprocess=ns %1\image_macros_preprocessed_bl2.c >> %1\output.txt 2>&1
