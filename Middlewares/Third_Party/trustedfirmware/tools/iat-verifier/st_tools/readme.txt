-1-Python Installation : (use python 3)
in directory ./tools/iat_verifier 
python setup.py install

-2-Set sbsfu binary path in checkiat.sh,
according to :
- board beeing tested.
- IDE used for build.

-3-Check Perso area offset in build.py,
and update it if not correct.
        big_binary=big_binary[0x900:]

-4-Check initial attestation token

The script checkiat.sh is used to check initialattestion response
according to IDE selected for test uncomment the line in ./checkiat.sh to sbsfu binary
then execute in sub menu 6 (TFM - Test EAT) from menu 3 ( Test TFM) 

recopy token in eat.txt (selection on teraterm) file form ./tools/iat/verifier/st-tools

and execute ./checkiat.sh

cbor
Signature OK
Token format OK
Token:
{
    "CHALLENGE": "00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",
    "BOOT_SEED": "ec25cee56f9d8580bcb313e2dab04bf168906340dd59fafc7a5635360448faf0",
    "INSTANCE_ID": "01fa58755f658627ce5460f29b75296713248cae7ad9e2984b90280efcbcb50248",
    "IMPLEMENTATION_ID": "1c91381a2079c328d7372253bdb706f10937243047a577945e1d2d2c024913b5",
    "CLIENT_ID": -1,
    "SECURITY_LIFECYCLE": "SL_SECURED",
    "SW_COMPONENTS": [
        {
            "SW_COMPONENT_TYPE": "SPE",
            "SW_COMPONENT_VERSION": "1.0.0",
            "MEASUREMENT_VALUE": "5dfa910873fd3ca0ec134822101c3495d2108c710e7086defdccb74701810a69",
            "MEASUREMENT_DESCRIPTION": "SHA256",
            "SIGNER_ID": "bfe6d86f8826f4ff97fb96c4e6fbc4993e4619fc565da26adf34c329489adc38"
        },
        {
            "SW_COMPONENT_TYPE": "NSPE",
            "SW_COMPONENT_VERSION": "1.0.0",
            "MEASUREMENT_VALUE": "db0635cd666ac2e02ff2d8853a94f52e5ecea1493380d2146bd6a148b7298296",
            "MEASUREMENT_DESCRIPTION": "SHA256",
            "SIGNER_ID": "b360caf5c98c6b942a4882fa9d4823efb166a9ef6a6e4aa37c1919ed1fccc049"
        }
    ],
    "HARDWARE_ID": "1\u0000\u000b\u0000\u0004P62N51 "
}
boot
1c91381a2079c328d7372253bdb706f10937243047a577945e1d2d2c024913b5 *./sbsfu.bin

implementation ID should be the sha256 of ./sbsfu.bin
