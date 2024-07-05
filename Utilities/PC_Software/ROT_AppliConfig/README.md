# AppliCfg

AppliCfg is a tool for ROT projects, that allow to automatically configure projects files according to ROT configuration.

## Installation
The are two modes ways to execute this tool.
* .exe file 
* .py project With this project, it is possible to modify the sources. In this mode is HIGH recommended to run the setup.py script in order to get all the library dependencies installed

  ```bash
  python setup.py install
  ```

## Project execution
There are three ways to test the tool:
Using the python script :
* AppliCfg.py

Using the Windows exe :
* dist/AppliCfg/AppliCfg.exe  (one dir configuration)
* dist/AppliCfg.exe  (onefile configuration)

## XMl file function
To create an image, the Trust Package Creator (TPC) external tool can be used. To do his the TPC tools will use an XML file as an input.

The created image will take into account all the information that is present in the input file.

To modify the image configuration, it is possible to modify the he xml file manually or by using this tool.

The follow elements compose the xml file : 
```bash
	<Param>
		<Name></Name>
		<Link></Link>
		<Type></Type>
		<Command></Command>
		<Hidden></Hidden>
		<Enable></Enable>
		<Info></Info>
		<Default></Default>
	</Param>
```

Example :
```
	<Param>
		<Name>Header Size</Name>
		<Type>Data</Type>
		<Command>-H</Command>
		<Hidden>1</Hidden>
		<Enable>1<Enable>
		<Default>0x400</Default>
	</Param>
```
## Usages
### Enable/Disable XML options
It is possible to indicate when an xml parameter is available or not. To do that, it is necessary to use the "xmlen" function which needs the following options :

**xmlen**
* -l (--layout)  Input file image_macros_preprocessed_bl2.c
* -m (--macro)   Symbol to search in the macro preprocessed file

It is necessary to choose one option (only one): 
* -n (--name)    Filter xml parameters by \<Name\>. The parameter selected will be enable or disable
* -c (--command) Filter xml parameters by \<Command>. The parameter selected will be enable or disable
* The output xml file to be modified

Example :
```
xmlen -l image_macros_preprocessed_bl2.c -m RE_ENCRYPTION -c E OEMiRoT_Secure_Code.xml
```    
*Then the OEMiRoT_Secure_Code.xml will be filtered by the tag \<Command\>. If the command "E" is found,
the tag <Enable> content will be replaced by the value found in the image_macros_preprocessed_bl2 file.*

If the user want to force the Enable modification it is possible to use the following options :

**xmlen**
* -e (--enable)   0 for disable command or 1 for enable the command

It is necessary to choose one option : 
* -n (--name)    Filter xml parameters by \<Name\>. The parameter selected will be enable or disable
* -c (--command) Filter xml parameters by \<Command\>. The parameter selected will be enable or disable
* xml file to modified
Example :
```
xmlen -e 0 -c E OEMiRoT_Secure_Code.xml
```
*This means the OEMiRoT_Secure_Code.xml will be filtered by the tag \<Command\>. If the command "E" is found,
the tag <Enable> content will be replaced by 0*

### Modify xml content 
It is possible to modify the content of one Tag. To use this function, the following options must be defined:

#### Modifying the \<Name> content from a image_macros_preprocessed_bl2.c file
**xmlval**
* -l (--layout)  Path of image_macros_preprocessed_bl2.c file
* -m (--macro)   Value to get from macro file

It is necessary to choose one option (only one): 
* -n (--name)    Filter xml parameters by \<Name\>. The value of the parameter selected will be modified.
* -c (--command) Filter xml parameters by \<Command>. The value of the parameter selected will be modified.
* xml file to be modified

Example:
```
xmlval -l image_macros_preprocessed_bl2.c -m RE_IMAGE_FLASH_SECURE_IMAGE_SIZE -c -S OEMiRoT_Secure_Code.xml
```
*This means the RE_IMAGE_FLASH_SECURE_IMAGE_SIZE symbol will be searched in the image_macros_preprocessed_bl2.c file.
If the symbol is found, its value will be stored (otherwise an error will be displayed)*

*Then the OEMiRoT_Secure_Code.xml will be filtered by the tag \<Command\>. If the command "E" is found,
the tag \<Value\> content will be replaced by the value found in the image_macros_preprocessed_bl2 file.*

#### Modifying the \<Name\> content from another xml file
**xmlval**
* -xml (--xml)  Path of xml file
* -nxml (--xml_name) Filter xml parameters by \<Name\>

It is necessary to choose one option (only one): 
* -n (--name)    Filter xml parameters by \<Name\>. The value of the parameter selected will be modified.
* -c (--command) Filter xml parameters by \<Command>. The value of the parameter selected will be modified.
* xml file to be modified
Example :
```
xmlval -xml OEMiRoT_Secure_Code.xml -nxml Firmware area size -c S OEMiRoT_NonSecure_Code.xml
```
*This means the OEMiRoT_Secure_Code.xml will be filtered by the tag \<Name\>. If the content "Firmware area size" is found,
the tag \<Value\> content will be stored.*

*The OEMiRoT_NonSecure_Code.xml will be filtered by the tag \<Command\>. If the command "S" is found. The content of \<Value\> will be
modified with the value found in the OEMiRoT_Secure_Code.xml file.*

#### Modifying he \<Name\> with a function option
If the user want to force the Value modification it is possible to use the following options :

**xmlname**
Modify the Tag <Name> content of an xml parameter. The modification could be executed with the --layout and --macro arguments. Or by defining the --name argument
-n (--name)     New name
-c (--command)  Tag <Command> content in the xml file. If the tag is found the name content will be modified
* xml file to modified

*Modify the Tag <Name> content of an xml parameter. The modification could be executed with the --layout and --macro arguments. Or by defining the --name argument

**xmlval**
* -v (--value)    New value input
It is necessary to choose one option : 
* -n (--name)    Filter xml parameters by \<Name\>. The parameter selected will be enable or disable
* -c (--command) Filter xml parameters by \<Command\>. The parameter selected will be enable or disable
* xml file to modified

Example :
```
xmval -v 0x6500 -c -S OEMiRoT_Secure_Code.xml
```
*This means the OEMiRoT_Secure_Code.xml will be filtered by the tag \<Command\>. If the command "S" is found,
the tag \<Value\> content will be replaced by 0x6500*

#### Modifying the xml content from a binary file

* -b           (--binary_file)  Binary input file (AreaAPayload)
* -b_el_idx    (--binary_element_index) Element to recover from the binary file
* -b_el_sz     (--binary_element_size) Size of the binary element (By default 48 bytes)
* -b_d_sz      (--binary_data_size) Data size to recover from the binary element
* -b_d_e       (--binary_data_endianness) Data binary endianness format (little or big)
* -nxml_el_idx (--xml_name_layout_index)' . Recover the value of the parameter with the \<\Name> selected.
                                            This will allow the element number to recover from the binary file
* -n (--name)    Filter xml parameters by \<Name\>. The value of the parameter selected will be modified
* xml file to be modified

Example :
```
 xmlval -b AreaAPayload.bin -b_el_idx 48 -b_d_sz 16 -b_el_sz 64 -b_d_e little -nxml_el_idx "Flash Layout configuration index" -n "NS FileSystem Size" SMAK_Config_part2.xml
```
*This means the SMAK_Config_part2.xml will be filtered by the tag \<Name\>. If the name "Flash Layout configuration index" is found,
the tag \<Value\> content will be used to recover the element from the AreaPayload.bin file.

The size of the element to extract will be 64 bytes. The start index of the data to recover will be the 48 and the size will be 16 bits (2 bytes) in little endian format :

Element extracted :
01 10 CE 26 36 B9 10 00 00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00 00 00 00 00 46 80 39 7F 
01 C9 00 C9 05 FF FF FF FF FF FF FF FF FF FF FF 
00 10 E5 1B 2B E3 00 2B E3 00 2B E3 00 2B E3 00 
2B E3 00 2B E3 00 2B E3 00 2B E3 00 2B 87 5C 87


Data recovered : 00 10 -- > 0x1000

This value will be used to replace the value existed in the "NS FileSystem Size" parameter.


#### Extra options 
It this possible to modify the input value. To do this, it is necessary to use the following options :
*Warning* : When using a binary file to modify the xml file the *val1* will be the value recovered from the binary file. The first nxml argument will be equals to val2, then val3 ....

* -cond (--condition)  Condition expression in order to define if the expression command need to be executed.
                       The conditions must be in python format (example : and, or, xor, not)
* -e    (--expression) Mathematical formula to modify the value. 
  * value : To define the value found
  * val"x" : When using multiple input values
  * const"x" : Define the value of the constant to use. 
* -xml_name it could be defined multiple times. This will help to search different values from a xml file and and use the values in the expression function
* -cons (--constant) Define the constant value. It is necessary to use the same argument to define multiple constants
Example :
```
xmlval -v 0x6500 -c -S -e "(value + cons1 + cons2)/cons3" -cons 0x50 -cons 0x200 -cons 0x2  OEMiRoT_Secure_Code.xml
```
*This means the OEMiRoT_Secure_Code.xml will be filtered by the tag \<Command\>. If the command "S" is found,
the tag \<Value\> content will be replaced by (0x6500 + 0x50 + 0x200)/0x2 = 0x32A8*

It is also possible to modify the output format. By default the value will be in hex format
--decimal : The value found will be converted in decimal format
--string : The value found will be converted to string

* -txml (--xml_tag) By default, the tag content to modify is <Value>. But it is possible to define the content tag to modify.
Example :
```
xmval -v resign -c sign -xml_tag Command --string OEMiRoT_Secure_Code.xml
```
*This means the OEMiRoT_Secure_Code.xml will be filtered by the tag \<Command\>. If the command "sign" is found,
the tag \<Command\> content will be replaced by sign*

### Add or remove parameter
It is possible to add or remove and entire parameter. 

To use this function, the following options must be defined:

**xmlparam**
* -l (--layout)  Path of image_macros_preprocessed_bl2.c file
* -m (--macro)   Value to get from macro file
* -n (--name) Tag \<Name\> content of the xml parameter (required)
* -link (--link) Tag \<Link\> content of the xml parameter
* -v (--value) Tag \<Value\> content of the xml parameter
* -t (--type) Tag \<Type\> content of the xml parameter
* -c (--command)Tag \<Command\> content of the xml parameter (with - or --)
* -e (--enable) Tag \<Enable\> content of the xml parameter (0 disable or 1 enable)
* -d (--default)Tag \<Default\> content of the xml parameter
* -h (--hidden) Tag \<Hidden\> content of the xml parameter (0 no hidden, 1 hidden)
* -ttip (--tooltip) Tag \<ToolTip\> content of the xml parameter
* xml file to modified
Example :
```
xmlparam -l  image_macros_preprocessed_bl2.c -m RE_CRYPTO_SCHEME -n "Encryption key" -link GetPublic -t File -c -E -h 0 OEMiRoT_Secure_Code.xml
```
*This means the RE_CRYPTO_SCHEME symbol will be searched in the image_macros_preprocessed_bl2.c file.
If the symbol is found, its value will be stored (otherwise an error will be displayed)*

*The OEMiRoT_NonSecure_Code.xml will be filtered by the tag \<Name\>. If the name content "Encryption key" is found. The parameter will be stored*

*Four options are possible :*
* *If the value of RE_CRYPTO_SCHEME is equals to 0 and the parameter was found. The entire parameter will be erased.*
```bash
	<Param>
		<Name>Encryption key</Name>
      ....
        ....
	</Param>
```
* *If the value of RE_CRYPTO_SCHEME is equals to 0 and the parameter was not found. Nothing will be erased*
* *If the value of RE_CRYPTO_SCHEME is different to 0 and the parameter was not found. Nothing will be added*
* *If the value of RE_CRYPTO_SCHEME is different to 0 and the parameter was found. The follow structure will be added*
```bash
	<Param>
		<Name>Encryption key</Name>
		<Link>GetPublic</Link>
		<Type>File</Type>
		<Command>-E</Command>
		<Hidden>0</Hidden>
	</Param>
```
**Note : By default, all the tags are empty. Only the tag with content will be saved in the xml file.**

If the user want to add or remove a parameter. It is possible to use the following options :

**xmlparam**
* -o (--option)  add or rm
* -n (--name) Tag \<Name\> content of the xml parameter (required)
* -link (--link) Tag \<Link\> content of the xml parameter
* -v (--value) Tag \<Value\> content of the xml parameter
* -t (--type) Tag \<Type\> content of the xml parameter
* -c (--command)Tag \<Command\> content of the xml parameter (with - or --)
* -e (--enable) Tag \<Enable\> content of the xml parameter (0 disable or 1 enable)
* -d (--default)Tag \<Default\> content of the xml parameter
* -h (--hidden) Tag \<Hidden\> content of the xml parameter (0 no hidden, 1 hidden)
* -ttip (--tooltip) Tag \<ToolTip\> content of the xml parameter
* xml file to modified
Example :
```
xmlparam -o rm -n "Encryption key" OEMiRoT_Secure_Code.xml
```
*This means if the parameter with the tag <Name> content "Encryption key" exists
the parameter will be deleted.*

### Modify linker values
It is possible to modify the variable value of a linker file **(This option is only valid for icf files)**. 
The following options must be defined:

#### Modifying the values with image_macros_preprocessed_bl2.c file
**linker**
* -l (--layout)  Path of image_macros_preprocessed_bl2.c file
* -m (--macro)   Value to get from macro file
* -n (--name) linker variable to modified
* linker file to be modified
```
linker -l image_macros_preprocessed_bl2.c -m RE_IMAGE_FLASH_NON_SECURE_IMAGE_SIZE -link S_CODE_SIZE stm32h573xx_flash_s.icf
```
*This means the RE_IMAGE_FLASH_NON_SECURE_IMAGE_SIZE symbol will be searched in the image_macros_preprocessed_bl2.c file.
If the symbol is found, its value will be stored (otherwise an error will be displayed)*.

*The variable S_CODE_SIZE in the icf file will be updated with the value found in the image_macros_preprocessed_bl2 file.*

#### Modifying the values with a xml file
**linker**
* -xml (--xml)  Path of xml file
* -nxml (--xml_name) Filter xml parameters by \<Name\>.
* -n (--name) linker variable to modified
* linker file to be modified


```
linker -xml OEMiRoT_Secure_Code.xml -nxml Firmware area size -n CODE_SIZE stm32h573xx_flash_s.icf
```
*This means the OEMiRoT_Secure_Code.xml will be filtered by the tag \<Name\>. If the content "Firmware area size" is found,
the tag \<Value\> content will be used to update the value of "CODE_SIZE" variable in the stm32h573xx_flash_s.icf file.*

#### Extra options
It this possible to modify the input value. To do this, it is necessary to use the following options :
* -e    (--expression) Mathematical formula to modify the value. 
  * value : To define the value found
  * val"x" : When using multiple input values
  * const"x" : Define the value of the constant to use. 
* -cons (--constant) Define the constant value. It is necessary to use the same argument to define multiple constants
* -xml_name it could be defined multiple times. This will help to search different values from a xml file and and use the values in the expression function

```
linker -xml OEMiRoT_Secure_Code.xml -nxml Firmware area size -nxml Firmware area size -e "(val1 + val2)/cons1" -c 0x50 -n CODE_SIZE stm32h573xx_flash_s.icf
```
*This means the OEMiRoT_Secure_Code.xml will be filtered by the tag \<Name\>.*

 *If the content "Firmware area size" is found,the tag \<Value\> content will be used. The value will be used in the expression function.
 In this case the value will be added by itself. The the value results will be divided by 0x50. The final result will be used to update the "CODE_SIZE" variable in the stm32h573xx_flash_s.icf file.*


### Modify define variable value
It is possible to modify the values of an include file. The following options must be defined:

#### Modifying the values with image_macros_preprocessed_bl2.c file
**definevalue**
* -l (--layout)  Path of image_macros_preprocessed_bl2.c file
* -m (--macro)   Value to get from macro file
* -n (--name) Variable in the .h file to be modified
* Include file to be modified
Example : 
```
definevalue -l image_macros_preprocessed_bl2.c -m RE_IMAGE_FLASH_ADDRESS_SECURE -n FLASH_ADDRESS_SECURE rot_variables.h
```
*This means the RE_IMAGE_FLASH_ADDRESS_SECURE symbol will be searched in the image_macros_preprocessed_bl2.c file.
If the symbol is found, its value will be stored (otherwise an error will be displayed).*

*The variable FLASH_ADDRESS_SECURE in the include file will be updated with the value found in the image_macros_preprocessed_bl2 file.*

#### Modifying the values with a xml file
**definevalue**
* -xml (--xml)  Path of a xml file
* -nxml (--xml_name) Filter xml parameters by \<Name\>.
* -n (--name) Variable in the include file to be modified
* Include file to be modified
```
definevalue -xml OEMiRoT_Secure_Code.xml -nxml Firmware area size -n FLASH_ADDRESS_SECURE rot_variables.h
```
*This means the OEMiRoT_Secure_Code.xml will be filtered by the tag \<Name\>. If the content "Firmware area size" is found,
the tag \<Value\> content will be used to update the value of "FLASH_ADDRESS_SECURE" variable in the rot_variables.h file*

#### Extra options : 
It this possible to modify the value. To do this, it is necessary to use the following options :
* -e    (--expression) Mathematical formula to modify the value. 
  * value : To define the value found
  * val"x" : When using multiple input values
  * const"x" : Define the value of the constant to use. 
* -xml_name it could be defined multiple times. This will help to search different values from a xml file and and use the values in the expression function

It is also possible to modify the output format. By default the value will be in hex format
* -cons (--constant) Define the constant value. It is necessary to use the same argument to define multiple constants
* --decimal : Optional flag, used when the format value is en decimal. By default the format is in hex
* --string : The value found will be converted to string

```
definevalue -xml OEMiRoT_Secure_Code.xml -nxml "Firmware area size" -nxml "Firmware area size" -e "(val1 * val2)/0x500" -n FLASH_ADDRESS_SECURE rot_variables.h
```
*This means the OEMiRoT_Secure_Code.xml will be filtered by the tag \<Name\>.*

 *If the content "Firmware area size" is found,the tag \<Value\> content will be used. The value will be used in the expression function.
 In this case the value will be multiplied by itself. The the value results will be divided by 0x500. The final result will be used to update the "FLASH_ADDRESS_SECURE" variable in the rot_variables.h file.*

### Comment or uncomment define variable
It is possible to comment or uncomment the # defines variables in a include file. The following options must be defined:

#### Comment/Uncomment with image_macros_preprocessed_bl2.c file
**setdefine**
* -l (--layout)  Path of image_macros_preprocessed_bl2.c file
* -m (--macro)   Value to get from macro file
* -n (--name) linker variable to modified
* -v (--value) IF value == to the value existed in the xml file the define variable will be uncommented otherwise it will be commented
* Include file to be modified

```
setdefine -l image_macros_preprocessed_bl2.c -m RE_OVER_WRITE -name MCUBOOT_OVERWRITE_ONLY -v 1 File.h
```
*This means the RE_OVER_WRITE symbol will be searched in the image_macros_preprocessed_bl2.c file.
If the symbol is found, its value will be stored (otherwise an error will be displayed).*

*The flag MCUBOOT_OVERWRITE_ONLY will be commented if the value found in the image_macros_preprocessed_bl2.c file is equals to the --value option. Otherwise the flag be uncommented.*

#### Comment/Uncomment with xml file
**setdefine**
* -xml (--xml)  Path of xml file
* -nxml (--xml_name) Filter xml parameters by \<Name\>.
* -n (--name) Variable in the .h file to be commented/uncommented
* -v (--value) IF value == to the value existed in the xml file the define variable will be uncommented otherwise it will be commented
* Include file to be modified
```
setdefine -xml STiRoT_Config.xml -nxml Number of images managed -n DATA_IMAGE_EN -v 0x02 File.h
```
*This means the STiRoT_Config.xml will be filtered by the tag \<Name\>. If the content "Number of images managed" is found,
the tag \<Value\> content will be stored. If the value is equals to --value option the flag "CODE_SIZE" will be commented (otherwise it will be uncommented)*

#### ***Limitation*** :
It is possible to comment a variable with or without value. In the case of commenting a define with a value. The value must not be between parenthesis. 

### Modify ST-iRoT protections
It is possible to modify the protections of the ST-iRoT Project. The values 
will be taken from STiRoT_Config.xml file. To update the values the following options must be defined :
**obscript**
* -xml (--xml)  Path of STiRoT_Config.xml file
* -ob (--optionbyte) Protection to be modified. Only three options are available:
  * SECWM   : Modify watermarks protections
  * SRAM2_RST : Modify SRAM2 erased in case of reset
  * SRAM2_ECC : Modify hack tentative detection
  * WPRS : Modify write protection memory area
* -sp (--secure_pattern) : Principal secure information. This is the tag \<Name\> content in order to filter the xml file and get the tag \<Value\> content
* -b (--begin) : Pattern to choose one line in the script file. Default "stm32programmercli"

**Note : Only when using the SECWM option**
* -co (--code_offset) : Primary Code Offset. This is the tag \<Name\> content in order to filter the xml file and get the tag \<Value\> content
* -cs (--code_size) Primary Code size. This is the tag \<Name\> content in order to filter the xml file and get the tag \<Value\> content
* -fs (--full_sec) Full secure. This is the tag \<Name\> content in order to filter the xml file and get the tag \<Value\> content
* ST-iRoT script to be modified

**Note**
Possible values of SRAM2_RST and SRAM2_ECC
* SRAM2_RST:
 * 0x00000000 --> SRAM2_RST=0
 * 0x00000008 --> SRAM2_RST=1
* SRAM2_ECC for H5
 * 0x00000000 --> SRAM2_ECC=0
 * 0x00000040 --> SRAM2_ECC=1
* ECC_ON_SRAM for H7
 *0x00 --> ECC_ON_SRAM=0
 *0x01 --> ECC_ON_SRAM=1
 

Example : 
```
obscript --xml STiRoT_Config.xml -ob SRAM2_RST -sp "SRAM2 erasing in case of reset" -b "" programming.bat
```
*This means the SRAM2_RST protections will be modified. The STiRoT_Config.xml will be filtered by the tag \<Name\>. If the content "SRAM2 erasing in case of reset" is found,
the tag \<Value\> content will be stored. Depending of the value, the option byte protection SRAM2_RST will be modified in the programming.bat file*


### Modify WRP and HDP protections (OEMirot/OEMuRoT)
It is possible to modify the protections of the Rot Project. To update the protections the following options must be defined :

**setob**
* -l (--layout)  Path of image_macros_preprocessed_bl2.c file
* -ms (--macro_start) Macro symbol to define the start protection
* -me (--macro_end) Macro symbol to define the end protection
* -msec (--macro_sector) Macro symbol to define number of sector in the board
* -b (--begin)   Value to modify in the script file (wrpgrp1,wrpgrp2, hdp_start, hdp_end, hdp1_start, hdp1_end, ...)
* --division :   Sector group size
* Rot script to be modified

Example : 
```
setob --layout image_macros_preprocessed_bl2.c -b wrpgrp1 -ms RE_BL2_WRP_START -me RE_BL2_WRP_END -msec RE_FLASH_PAGE_NBR -d 0x8000  ob_flash_programming.bat
```
*This means the wrpgrp1 protection will be calculated. The macro symbols, RE_BL2_WRP_START, RE_BL2_WRP_END and RE_FLASH_PAGE_NBR will be
searched in the image_macros_preprocessed_bl2.c file.*

*With those values setob function will compute the protection wrpgrp1 and wrpgrp2. The option "--begin" will select the protection to use (in this case wrpgrp1).
Finally the wrpgrp1 protection will be updated in the ob_flash_programming.bat script*

### Modify the script variable value
It is possible to modify the value of one variable in the Rot scripts.

To update the values the following options must be defined :

#### Modifying the values with image_macros_preprocessed_bl2.c file
**flash**
* -l (--layout)  Path of image_macros_preprocessed_bl2.c file
* -m (--macro)   Value to get from macro file
* -b (--begin)   Value to modify in the script file
* OEMxRot script to be modified

Example :
```
flash --layout image_macros_preprocessed_bl2.c -m RE_BL2_BOOT_ADDRESS -b nsbootadd hardening.bat
```
*This means the RE_BL2_BOOT_ADDRESS symbol will be searched in the image_macros_preprocessed_bl2.c file.
If the symbol is found, its value will be stored (otherwise an error will be displayed).*

*The value of the nsbootadd variable in the hardening.bat file will be updated.*

#### Modifying the values with STiRoT_Config.xml file
**flash**
* -xml (--xml)  Path of STiRoT_Config.xml file
* -n (--name) | -nxml (--xml_name) Filter xml parameters by \<Name\>.
* -b (--begin)   Value to modify in the script file
* OEMxRot script to be modified

```
flash -xml OEMiRoT_Secure_Code.xml -n "Image output file" -b s_code_image ob_flash_programming.bat
```
*This means the OEMiRoT_Secure_Code.xml will be filtered by the tag \<Name\>. If the content "Image output file" is found,
the tag \<Value\> content will be used to update the value of "s_code_image" in the ob_flash_programming.bat file*

#### Extra options
* --division : Divide the value (optional)
* --decimal : Define if the value to modify will be in decimal. By default, all the values are in Hex (optional)

It this possible to modify the input value. To do this, it is necessary to use the following options :
* -e    (--expression) Mathematical formula to modify the value. 
  * value : To define the value found
  * val"x" : When using multiple input values
  * const"x" : Define the value of the constant to use. 
* -cons (--constant) Define the constant value. It is necessary to use the same argument to define multiple constants
* -xml_name it could be defined multiple times. This will help to search different values from a xml file and and use the values in the expression function

```
flash -xml OEMiRoT_Secure_Code.xml -n "Firmware area size" -nxml "Firmware area size" -e "(val1 * val2)/0x500" -b s_code_image ob_flash_programming.bat
```
*This means the OEMiRoT_Secure_Code.xml will be filtered by the tag \<Name\>.*

 *If the content "Firmware area size" is found,the tag \<Value\> content will be used. The value will be used in the expression function.
 In this case the value will be multiplied by itself. The the value results will be divided by 0x500. The final result will be used to update the "s_code_image" variable ob_flash_programming.bat file.*


### Modify a file variable value
It is possible to modify the value of one variable in the Rot scripts.

To update the values the following options must be defined :

#### Modifying the values with image_macros_preprocessed_bl2.c file
**modifyfilevalue**
* -xml (--xml)  Path of xml file configuration
* -xml (--name_name) The value of the parameter with tag \<Name\> will be used to modify the file value.
* -var (--variable) The variable name to modify
* -sft (--shift) The value recovered from the xml will be shifted to the left (in bit unit)
* File to be modified

Example :
```
flash -xml "SMAK_Config_General.xml" -n "Product state minimal allowed" -var "OPTSR_PRG" -sft "8" Option_Bytes.csv
```
*This means the SMAK_Config_General.xml will be filtered by the tag \<Name\>. If the content "Product state minimal allowed" is found,
the tag \<Value\> content will be used to update the value of "OPTSR_PRG" in the Option_Bytes.csv file*

*The shift value will shift to the left the value recovered from the xml file. The value shifted will be replaced the value in the Option_Bytes.csv file. Example :*

  CSV file :
  ```
  OPTSR_PRG,0x30f0edf8
  ```

  XML file :
  ```
  <List>
      <Name>Product state minimal allowed</Name>
      <Value>0xC6</Value>
      .....
      ....
  ```

  Shift value
  ```
  8
  ```

  New CSV file value
  ```
  OPTSR_PRG,0x30f0c6f8
  ```


#### Extra options
* -del (--delimiter) Special char between variable name and value. Ex : (,), (;), (=).... By default the delimiter (,) is used.
* --upper . Special flag to define if the output value will be in capital letter. By default the value is false

## Assembly secure and non secure images
It is possible to assembly two binaries in one. This steps are required when 
the project configuration use only one image. The following options must be defined :
**oneimage**
* -fb (--first_binary) First binary path
* -sb (--second_binary) Second binary path
* -o (--optional_size) Add padding to first binary
* -i (--input_size) Add padding to the second binary
* File binary output name
Example : 
```
oneimage -fb ...\Secure\STM32H573I-DK_S\Exe\Project.bin -sb ...\NonSecure\STM32H573I-DK_NS\Exe\Project.bin -o 0x6000 rot_app.bin
```

*This means the image size of the first image must be 0x6000, if the size of the first binary is lower than 0x6000 a padding will be added.
Then the second binary will be concatenated. The binary output will be renamed as rot_app.bin*


## Update Appli project to one image configuration
When one image configuration is selected the postbuild command for the secure and non secure application must be modified.

Example for the IAR project :
```
<settings>
    <name>BUILDACTION</name>
    <archiveVersion>1</archiveVersion>
    <data>
        <prebuild></prebuild>
        \<postbuild\>"STM32TrustedPackageCreator_CLI.exe" -pb "OEMiRoT_S_Code_Image.xml"\</postbuild\>
    </data>
</settings>
```

## Update input and output project binaries 
Depending the project configuration the input and output binaries will be different. 

For example when the project use only the signed images, the output file must be renamed:
```
rot_tz_s_app_enc_sign.hex --> rot_tz_s_app_sign.hex
```
When one image is used the input must be modified to :
```
ROT_Appli_TrustZone/EWARM/Secure/STM32H573I-DK_S/Exe/Project.bin --> ROT_Appli_TrustZone/EWARM/Binary/rot_app.bin
```
The output must be also modified :
```
rot_tz_ns_app_enc_sign.hex to --> rot_tz_app_enc_sign.hex 
```
**Note** : The output modifications impact two files the xml and the ob_flash_programming file.

The follow options are required :

**iofile**
* -l (--layout) Location of the file that contains preprocessed macros
* -me (--macro_encrypted) Macro symbol for the encryption mode. The symbol will be searched in the preprocessed file
* -mi (--macro_image) Macro symbol for the image number value. The symbol will be searched in the preprocessed file
* -xml (--xml) Location of the XML file of the Rot Application
* -i (--input_bin) Project binary to use as input for the TPC tool
* -in (--input_name) Tag <Name> content to select the input binary of the xml file
* -on (--output_name) Tag <Name> content to select the output binary of the xml file
* -en (--enc_name) Tag <Name> content to select the authentication mode of the xml file
* -b (--begin) begin of line to replace in the script file
* ob_flash_programming file

**Note** The input/output modification will not be executed if the configuration don't needed.
Example:
* Secure data image disable --> No modification
* Non Secure data image disable --> No modification
* Two code image configuration
  * If path of input binary contains (Binary : path of one image) then no modification
* One code image configuration:
  * Secure code image : No modification
  * NonSecure code image : Modify only if "Binary" pattern exists in input bin path

Example :
```
iofile --layout image_macros_preprocessed_bl2.c -mi RE_APP_IMAGE_NUMBER -me RE_ENCRYPTION --xml OEMiRoT_NonSecure_Code.xml -in "Firmware binary input file" -i ../../ROT_Appli_TrustZone/Binary/rot_app.bin -on "Image output file" -en "Encryption key" -b ns_code_image ob_flash_programming.bat
```

*This means the RE_APP_IMAGE_NUMBER symbol will be searched in the image_macros_preprocessed_bl2.c file.
If the symbol is found, its value will be stored (otherwise an error will be displayed).
The RE_ENCRYPTION symbol will be also searched in the image_macros_preprocessed_bl2.c file.
If the symbol is found, its value will be stored (otherwise an error will be displayed).*

*The OEMiRoT_NonSecure_Code file will be filtered by \<Name\> in order to get the follow elements :*
```
	<Param>
		<Name>Encryption key</Name>
		<Link>GetPublic</Link>
		<Type>File</Type>
		<Command>-E</Command>
		<Enable>1</Enable>
		<Hidden>1</Hidden>
		<Default></Default>
	</Param>
	<Param>
		<Name>Firmware binary input file</Name>
		<Value>../../ROT_Appli_TrustZone/EWARM/NonSecure/STM32H573I-DK_NS/Exe/Project.bin</Value>
		<Type>File</Type>
		<Default>../../ROT_Appli_TrustZone/EWARM/NonSecure/STM32H573I-DK_NS/Exe/Project.bin</Default>
		<Tooltip>Select the firmware binary file to be processed for the image generation</Tooltip>
	</Param>
	<Output>
		<Name>Image output file</Name>
		<Value>../../ROT_Appli_TrustZone/Binary/rot_tz_ns_app_enc_sign.he</Value>
		<Default>../../ROT_Appli_TrustZone/Binary/rot_tz_ns_app_enc_sign.hex</Default>
	</Output>
```
*If the value found for RE_APP_IMAGE_NUMBER is equals to "1" and the value found for RE_ENCRYPTION is equals to "0", the elements will be updated :*
```
	<Param>
		<Name>Encryption key</Name>
		<Link>GetPublic</Link>
		<Type>File</Type>
		<Command>-E</Command>
		<Enable>0</Enable>
		<Hidden>1</Hidden>
		<Default></Default>
	</Param>
	<Param>
		<Name>Firmware binary input file</Name>
		<Value>../../ROT_Appli_TrustZone/Binary/rot_app.bin</Value>
		<Type>File</Type>
		<Default>../../ROT_Appli_TrustZone/EWARM/NonSecure/STM32H573I-DK_NS/Exe/Project.bin</Default>
		<Tooltip>Select the firmware binary file to be processed for the image generation</Tooltip>
	</Param>
	<Output>
		<Name>Image output file</Name>
		<Value>../../ROT_Appli_TrustZone/Binary/rot_tz_app_enc_sign.hex</Value>
		<Default>../../ROT_Appli_TrustZone/Binary/rot_tz_ns_app_enc_sign.hex</Default>
	</Output>
```

*The ns_code_image variable in the ob_flash_programming.bat script will be updated with rot_tz_app_enc_sign.hex value*

### Hash a binary content
It is possible to compute the HASH 256 content of a binary file and save the result in another binary file.

To update a file. The following options must be defined:

**hashcontent**
* -i (--input_data)  File to which the HASH 256 will be calculated.
* -o (--offset) Index to which the information will be added
* File to update

Example:
```
hashcontent -i user_password.bin -o 0x8 password.bin
```
This means the HASH256 will be computed with the content of the user_password.bin, 
the result will update the password.bin file. The index 0x8 to 0x8 + size of hash256, will be modified.

To create a file. The following options must be defined:

**hashcontent**
* -i (--input_data)  File to which the HASH 256 will be calculated.
* -o (--offset) Index to which the information will be added
* File to update

Example:
```
hashcontent -i user_password.bin password.bin
```
This means the HASH256 will be computed with the content of the user_password.bin, 
The result will be written in the the password.bin file.

#### Extra options
It is possible to define data after the HASH256, this data will be called tail.
The option -t (--tail) could be used.

Example
```
hashcontent --tail AABBCCDDEEFF -i user_password.bin password.bin
```
This means the HASH256 will be computed with the content of the user_password.bin, 
The tail data, in little endian and the result will be written in the the password.bin file. 

## Executable creation

The pyinstaller must be installed :

if python 3.10 (or greater) is recognized by windows
```
pip install pyinstaller
```
otherwise

```
python3 -m pip install pyinstaller
```

In the root folder, execute the command :

```
pyinstaller AppliCfg.py --onedir --name AppliCfg
```

The executable file will be found in the dist folder.

For more information,  see the pyinstaller documentation
