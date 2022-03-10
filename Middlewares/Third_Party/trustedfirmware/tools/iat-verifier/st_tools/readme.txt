-1-Python Installation : (use python 3)
in directory ./tools/iat_verifier
python setup.py install

-2-Set sbsfu binary path in checkiat.sh,
according to :
- board beeing tested.
- IDE used for build.

-3-Check Perso area offset in build.py,
and update it if not correct.
        big_binary=big_binary[0x2000:]

-4-Check initial attestation token

The script checkiat.sh is used to check initialattestion response
according to IDE selected for test uncomment the line in ./checkiat.sh to sbsfu binary
then execute in sub menu 7 (TFM - Test EAT) from menu 2 (Test TFM)

recopy token in eat.txt (selection on teraterm) file form ./tools/iat/verifier/st-tools

and execute ./checkiat.sh

cbor
Signature OK
Token format OK
Token:
{
    "CHALLENGE": "b'00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000'",
    "BOOT_SEED": "b'DE5761733287E4A0F427E731930578934A6183490F11F1205896E1AFD27A4404'",
    "INSTANCE_ID": "b'01FA58755F658627CE5460F29B75296713248CAE7AD9E2984B90280EFCBCB50248'",
    "IMPLEMENTATION_ID": "b'E397AA8BFB40C8A188643D39AF142AFF1FCC2EA08F5B8957C3D3CC93C4E4E8E4'",
    "CLIENT_ID": -1,
    "SECURITY_LIFECYCLE": "SL_SECURED",
    "SW_COMPONENTS": [
        {
            "SW_COMPONENT_TYPE": "SPE",
            "SW_COMPONENT_VERSION": "1.0.0",
            "SIGNER_ID": "b'05B081D953FA2DA76219365256A21A457B52001E5CA8296D907B13519C9540D3'",
            "MEASUREMENT_DESCRIPTION": "SHA256",
            "MEASUREMENT_VALUE": "b'8268C4F12D16E24D0AD9BA09B1090216B2186E1DC91B54D7F80F44324532F910'"
        },
        {
            "SW_COMPONENT_TYPE": "NSPE",
            "SW_COMPONENT_VERSION": "1.0.0",
            "SIGNER_ID": "b'3F6C9B372408CC68A8C2CD4E289135766DF5782DE44D24DC4BDF7E691C2A11FD'",
            "MEASUREMENT_DESCRIPTION": "SHA256",
            "MEASUREMENT_VALUE": "b'2599215D6961E1728D8C9058A5A78ADE24345CFA62F229A2FC88945458F44602'"
        }
    ],
    "ORIGINATOR": "www.trustedfirmware.org",
    "PROFILE_ID": "psa-tfm-profile-1.md",
    "HARDWARE_ID": "04080202000000"
}
boot
e397aa8bfb40c8a188643d39af142aff1fcc2ea08f5b8957c3d3cc93c4e4e8e4 *./sbsfu.bin

implementation ID should be the sha256 of ./sbsfu.bin
