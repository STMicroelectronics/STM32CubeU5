set "projectdir=%~dp0"
call "%projectdir%\..\..\env.bat"
:: end of updated field
pushd %projectdir%\..\..\SBSFU_Boot\Src
set sbsfu_key_dir=%cd%
popd

set "sbsfu_keys=%sbsfu_key_dir%\keys.c"

::lines for keys backup
for /f %%# in ('wmic os get localdatetime^|findstr .') do if "%%#" neq "" set date=%%#
set date=%date:~,4%_%date:~4,2%_%date:~6,2%_%date:~8,2%_%date:~10,2%_%date:~12,2%
set "$key_backup_dir=%sbsfu_key_dir%\%date%_keybcp"
set "cmdcreatedir=mkdir %$key_backup_dir%
%cmdcreatedir%
IF %ERRORLEVEL% NEQ 0 goto :error_key
set "cmdcpy=copy %sbsfu_keys% %$key_backup_dir%"
%cmdcpy%
IF %ERRORLEVEL% NEQ 0 goto :error_key
set "cmdcpypem=copy %sbsfu_key_dir%\*.pem %$key_backup_dir%\"
%cmdcpypem%
IF %ERRORLEVEL% NEQ 0 goto :error_key

:start
if exist %imgtool% (
echo Keygen with windows executable
goto Keygen
) else (
  echo ""
  echo "!!! WARNING : imgtool has not been found on your installation."
  echo ""
  echo "  Install CubeProgrammer on your machine in default path : C:\Program Files\STMicroelectronics\STM32Cube\STM32CubeProgrammer"
  echo "  or "
  echo "  Update your %projectdir%\..\..\env.bat with the proper path."
  echo ""
  exit 0
)


:keygen
::ecc 256 auth key
set "key_ecc_s=%sbsfu_key_dir%\root-ec-p256.pem"
set "command_key=%imgtool% keygen -k %key_ecc_s% -t ecdsa-p256"
%command_key%
IF %ERRORLEVEL% NEQ 0 goto :error_key
set "command_key=%imgtool% getpub -k %key_ecc_s%  > %sbsfu_keys%"
%command_key%
IF %ERRORLEVEL% NEQ 0 goto :error_key

::ecc 256 auth key
set "key_ecc_ns=%sbsfu_key_dir%\root-ec-p256_1.pem"
set "command_key=%imgtool% keygen -k %key_ecc_ns% -t ecdsa-p256"
%command_key%
IF %ERRORLEVEL% NEQ 0 goto :error_key
set "command_key=%imgtool% getpub -k %key_ecc_ns%  >> %sbsfu_keys%"
%command_key%
IF %ERRORLEVEL% NEQ 0 goto :error_key

:: priv key to encode images
set "key_ecc_enc_priv=%sbsfu_key_dir%\enc-ec256-priv.pem"
set "key_ecc_enc_pub=%sbsfu_key_dir%\enc-ec256-pub.pem"
set "command_key=%imgtool% keygen -k %key_ecc_enc_priv% -t ecdsa-p256 -e %key_ecc_enc_pub%"
%command_key%
IF %ERRORLEVEL% NEQ 0 goto :error_key
set "command_key=%imgtool% getpriv  --minimal -k %key_ecc_enc_priv%  >> %sbsfu_keys%"
%command_key%
IF %ERRORLEVEL% NEQ 0 goto :error_key
exit 0

:error_key
echo "%command_key% : failed" >> %projectdir%\\output.txt
exit 1