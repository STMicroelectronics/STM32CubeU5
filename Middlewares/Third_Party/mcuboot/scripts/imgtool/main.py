#! /usr/bin/env python3
#
# Copyright 2017-2020 Linaro Limited
# Copyright 2019-2020 Arm Limited
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
import re
import click
import getpass
import sys
from imgtool import macro_parser
from imgtool import image
from imgtool.keys import *
from imgtool.version import decode_version
from imgtool.__init__ import imgtool_version
from imgtool.__init__ import imgtool_build
import fileinput
from struct import pack
from imgtool.keys.rsa import *
from imgtool.keys import ( 
    RSAUsageError, ECDSAUsageError, Ed25519UsageError, X25519UsageError)

MIN_PYTHON_VERSION = (3, 6)
if sys.version_info < MIN_PYTHON_VERSION:
    sys.exit("Python %s.%s or newer is required by imgtool."
             % MIN_PYTHON_VERSION)

STRUCT_ENDIAN_DICT = {
        'little': '<',
        'big':    '>'
}
def gen_rsa2048(keyfile, passwd, export):
    new_key=RSA.generate()
    new_key.export_private(path=keyfile, passwd=passwd)
    if export:
      new_key.export_public(path=export)

def gen_rsa3072(keyfile, passwd, export):
    new_key=RSA.generate(key_size=3072)
    new_key.export_private(path=keyfile,passwd=passwd)
    if export:
      new_key.export_public(path=export)

def gen_ecdsa_p256(keyfile, passwd, export):
    new_key=ECDSA256P1.generate()
    new_key.export_private(path=keyfile,passwd=passwd)
    if export:
      new_key.export_public(path=export)

def gen_ecdsa_p256_ssl(keyfile, passwd, export):
    new_key=ECDSA256P1_SSL.generate()
    new_key.export_private(path=keyfile,passwd=passwd)
    if export:
      new_key.export_public(path=export)

def gen_sym_32(keyfile, passwd, export):
    new_key=SYMKEY.generate()
    new_key.export_private(path=keyfile,passwd=passwd)

def gen_ecdsa_p224(keyfile, passwd):
    print("TODO: p-224 not yet implemented")


def gen_ed25519(keyfile, passwd):
    Ed25519.generate().export_private(path=keyfile, passwd=passwd)


def gen_x25519(keyfile, passwd):
    X25519.generate().export_private(path=keyfile, passwd=passwd)

def gen_aesgcm(keyfile, passwd, export):
    new_key=AESGCM.generate()
    new_key.export_private(path=keyfile,passwd=passwd)

valid_langs = ['c', 'rust']
keygens = {
    'rsa-2048':   gen_rsa2048,
    'rsa-3072':   gen_rsa3072,
    'ecdsa-p256': gen_ecdsa_p256,
    'ecdsa-p224': gen_ecdsa_p224,
    'ed25519':    gen_ed25519,
    'x25519':     gen_x25519,
    'ecdsa-p256-ssl': gen_ecdsa_p256_ssl,
    'sym32': gen_sym_32,
    'aesgcm': gen_aesgcm
}

def load_key(keyfile):
    # TODO: better handling of invalid pass-phrase
    key = load(keyfile)
    if key is not None:
        return key
    passwd = getpass.getpass("Enter key passphrase: ").encode('utf-8')
    return load(keyfile, passwd)


def get_password():
    while True:
        passwd = getpass.getpass("Enter key passphrase: ")
        passwd2 = getpass.getpass("Reenter passphrase: ")
        if passwd == passwd2:
            break
        print("Passwords do not match, try again")

    # Password must be bytes, always use UTF-8 for consistent
    # encoding.
    return passwd.encode('utf-8')


@click.option('-p', '--password', is_flag=True,
              help='Prompt for password to protect key')
@click.option('-t', '--type', metavar='type', required=True,
              type=click.Choice(keygens.keys()), prompt=True,
              help='{}'.format('One of: {}'.format(', '.join(keygens.keys()))))
@click.option('-k', '--key', metavar='filename', required=True)
@click.command(help='Generate pub/private keypair')
@click.option('-e', '--export', metavar='filename', required=False)
def keygen(type, key, password, export):
    password = get_password() if password else None
    keygens[type](key, password, export)


@click.option('-l', '--lang', metavar='lang', default=valid_langs[0],
              type=click.Choice(valid_langs))
@click.option('-k', '--key', metavar='filename', required=True)
@click.command(help='Dump public key from keypair')
def getpub(key, lang):
    key = load_key(key)
    if key is None:
        print("Invalid passphrase")
    elif lang == 'c':
        key.emit_c_public()
    elif lang == 'rust':
        key.emit_rust_public()
    else:
        raise ValueError("BUG: should never get here!")


@click.option('--minimal', default=False, is_flag=True,
              help='Reduce the size of the dumped private key to include only '
                   'the minimum amount of data required to decrypt. This '
                   'might require changes to the build config. Check the docs!'
              )
@click.option('--raw', default=False, is_flag=True,
              help='dumped private key in mode raw'
              )
@click.option('-k', '--key', metavar='filename', required=True)

@click.command(help='Dump private key from keypair')
def getpriv(key, minimal, raw):
    key = load_key(key)
    if key is None:
        print("Invalid passphrase")
    try:
        key.emit_private(minimal, raw)
    except (RSAUsageError, ECDSAUsageError, Ed25519UsageError,
            X25519UsageError) as e:
        raise click.UsageError(e)

@click.option('--minimal', default=False, is_flag=True,
              help='Reduce the size of the dumped private key to include only '
                   'the minimum amount of data required to decrypt. This '
                   'might require changes to the build config. Check the docs!'
              )
@click.option('--raw', default=False, is_flag=True,
              help='dumped private key in mode raw'
              )
@click.option('-k', '--key', metavar='filename', required=True)
@click.command(help='Dump private key from keypair, in binary format')
def getprivbin(key, minimal, raw):
    key = load_key(key)
    if key is None:
        print("Invalid passphrase")
    try:
        key.emit_private_bin(minimal, raw)
    except (RSAUsageError, ECDSAUsageError, Ed25519UsageError,
            X25519UsageError) as e:
        raise click.UsageError(e)

@click.option('--sha', default=False, is_flag=True,
              help='Get public sha256 of public key'
              )
@click.option('--raw', default=False, is_flag=True,
              help='dumped public key in mode raw'
              )
@click.option('-k', '--key', metavar='filename', required=True)
@click.command(help='Dump public key in binary format')
def getpubbin(key, sha, raw):
    key = load_key(key)
    if key is None:
        print("Invalid passphrase")
    try:
        key.emit_public_bin(sha,raw)
    except (RSAUsageError, ECDSAUsageError, Ed25519UsageError,
            X25519UsageError) as e:
        raise click.UsageError(e)

class BasedIntAllParamType(click.ParamType):
    name = 'integer'

    def convert(self, value, param, ctx):
        if isinstance(value, int):
            return value
        try:
            if value[:2].lower() == "0x":
                return int(value[2:], 16)
            elif value[:1] == "0":
                return int(value, 8)
            return int(value, 10)
        except ValueError:
            self.fail('%s is not a valid integer. '
                      'prefixed with 0b/0B, 0o/0O, or 0x/0X as necessary.'
                      % value, param, ctx)

def validate_version(ctx, param, value):
    try:
        decode_version(value)
        return value
    except ValueError as e:
        raise click.BadParameter("{}".format(e))


def validate_security_counter(ctx, param, value):
    if value is not None:
        if value.lower() == 'auto':
            return 'auto'
        else:
            try:
                return int(value, 0)
            except ValueError:
                raise click.BadParameter(
                    "{} is not a valid integer. Please use code literals "
                    "prefixed with 0b/0B, 0o/0O, or 0x/0X as necessary."
                    .format(value))


def validate_header_size(ctx, param, value):
    min_hdr_size = image.IMAGE_HEADER_SIZE
    if value < min_hdr_size:
        raise click.BadParameter(
            "Minimum value for -H/--header-size is {}".format(min_hdr_size))
    return value


def get_dependencies(ctx, param, value):
    if value is not None:
        versions = []
        images = []
        dependencies = dict()
        
        for dep in value:
            match = re.match(r'^\((\d+),\s*([0-9.]+)\)$', dep) #multiple
            if match:
                image_id, raw_version = match.groups()
                images.append(image_id)
                try:
                    versions.append(decode_version(raw_version))
                except ValueError as e:
                    raise click.BadParameter("{}".format(e))
            else:                                               #one line
                dep_images = re.findall(r"\((\d+)", dep)
                if len(dep_images) == 0:
                    raise click.BadParameter(
                        "Image dependency format is invalid: {}".format(dep))
                raw_versions = re.findall(r",\s*([0-9.]+)\)", dep)
                if len(dep_images) != len(raw_versions):
                    raise click.BadParameter(
                        '''There's a mismatch between the number of dependency images
                        and versions in: {}'''.format(dep))
                for raw_version in raw_versions:
                    try:
                        versions.append(decode_version(raw_version))
                    except ValueError as e:
                        raise click.BadParameter("{}".format(e))
                images.extend(dep_images)
        
        dependencies[image.DEP_IMAGES_KEY] = images
        dependencies[image.DEP_VERSIONS_KEY] = versions
        return dependencies

class BasedIntParamType(click.ParamType):
    name = 'integer'

    def convert(self, value, param, ctx):
        try:
            return int(value, 0)
        except ValueError:
            self.fail('%s is not a valid integer. Please use code literals '
                      'prefixed with 0b/0B, 0o/0O, or 0x/0X as necessary.'
                      % value, param, ctx)

@click.argument('outfile')
@click.argument('infile')
@click.option('-k', '--key', metavar='filename')
@click.option('--public-key-format', type=click.Choice(['hash', 'full']),
              default='hash', help='In what format to add the public key to '
              'the image manifest: full key or hash of the key.')
@click.option('-x', '--hex-addr', type=BasedIntParamType(), required=False,
              help='Adjust address in hex output file.')
@click.option('--overwrite-only', default=False, is_flag=True,
              help='Use overwrite-only instead of swap upgrades')
@click.option('-e', '--endian', type=click.Choice(['little', 'big']),
              default='little', help="Select little or big endian")
@click.option('--align', type=click.Choice(['1', '2', '4', '8', '16']),
              required=True)
@click.option('--confirm', default=False, is_flag=True,
              help='When padding the image, mark it as confirmed (implies '
                   '--pad)')           
@click.option('--pad', default=False, is_flag=True,
              help='Pad image to --slot-size bytes, adding trailer magic')
@click.option('-R', '--erased-val', type=click.Choice(['0', '0xff']),
              required=False,
              help='The value that is read back from erased flash.')
@click.option('-S', '--slot-size', type=BasedIntParamType(), required=True,
              help='Size of the slot. If the slots have different sizes, use '
              'the size of the secondary slot.')
@click.option('-M', '--max-sectors', type=int,
              help='When padding allow for this amount of sectors (defaults '
                   'to 128)')
@click.option('--save-enctlv', default=False, is_flag=True,
              help='When upgrading, save encrypted key TLVs instead of plain '
                   'keys. Enable when BOOT_SWAP_SAVE_ENCTLV config option '
                   'was set.')              

@click.command(help='''resign a licence image file \n
               INFILE and OUTFILE are parsed as Intel HEX if the params have
               .hex extension, otherwise binary format is used''')
def resign(key, public_key_format, infile, outfile, hex_addr, overwrite_only,
           endian, align, confirm, pad, save_enctlv, erased_val, slot_size, max_sectors):
    key = load_key(key)
    #an image object to save is return
    #if None is return resign is not done
    img = image.Image.resign(infile, key, public_key_format, hex_addr, overwrite_only,endian, 
                                 align, confirm, pad, save_enctlv, erased_val, slot_size, max_sectors)
    if img is not None:
        img.save(outfile, hex_addr)


@click.argument('outfile')
@click.argument('infile')
@click.option('--custom-tlv', required=False, nargs=2, default=[],
              multiple=True, metavar='[tag] [value]',
              help='Custom TLV that will be placed into protected area. '
                   'Add "0x" prefix if the value should be interpreted as an '
                   'integer, otherwise it will be interpreted as a string. '
                   'Specify the option multiple times to add multiple TLVs.')
@click.option('-R', '--erased-val', type=click.Choice(['0', '0xff']),
              required=False,
              help='The value that is read back from erased flash.')
@click.option('-x', '--hex-addr', type=BasedIntParamType(), required=False,
              help='Adjust address in hex output file.')
@click.option('-L', '--load-addr', type=BasedIntParamType(), required=False,
              help='Load address for image when it should run from RAM.')
@click.option('-F', '--rom-fixed', type=BasedIntParamType(), required=False,
              help='Set flash address the image is built for.')
@click.option('--save-enctlv', default=False, is_flag=True,
              help='When upgrading, save encrypted key TLVs instead of plain '
                   'keys. Enable when BOOT_SWAP_SAVE_ENCTLV config option '
                   'was set.')
@click.option('-E', '--encrypt', metavar='filename',
              help='Encrypt image using the provided public key. '
                   '(Not supported in direct-xip or ram-load mode.)')
@click.option('-e', '--endian', type=click.Choice(['little', 'big']),
              default='little', help="Select little or big endian")
@click.option('--overwrite-only', default=False, is_flag=True,
              help='Use overwrite-only instead of swap upgrades')
@click.option('--boot-record', metavar='sw_type', help='Create CBOR encoded '
              'boot record TLV. The sw_type represents the role of the '
              'software component (e.g. CoFM for coprocessor firmware). '
              '[max. 12 characters]')
@click.option('-M', '--max-sectors', type=int,
              help='When padding allow for this amount of sectors (defaults '
                   'to 128)')
@click.option('--confirm', default=False, is_flag=True,
              help='When padding the image, mark it as confirmed (implies '
                   '--pad)')
@click.option('--pad', default=False, is_flag=True,
              help='Pad image to --slot-size bytes, adding trailer magic')
@click.option('-S', '--slot-size', type=BasedIntParamType(), required=True,
              help='Size of the slot. If the slots have different sizes, use '
              'the size of the secondary slot.')
@click.option('--pad-header', default=False, is_flag=True,
              help='Add --header-size zeroed bytes at the beginning of the '
                   'image')
@click.option('-H', '--header-size', callback=validate_header_size,
              type=BasedIntParamType(), required=True)
@click.option('-o', '--otfdec',
              type=BasedIntParamType(), required=False)
@click.option('-P','--primary-only',  default=False, is_flag=True,
              help='when encrypted image hash and signature are computed with header '
                    'without encrypted flag')
@click.option('--pad-sig', default=False, is_flag=True,
              help='create uncompress ECDSA signature '
                   '(for mcuboot <1.5 and for STuRoT and SecureManager Image)')
@click.option('--no-pad-tlv', is_flag=True, default=False,
              help='disable tlv padding (in case of magic in %s)' % image.Image.STiROTlist())
@click.option('-d', '--dependencies', callback=get_dependencies, multiple=True,
              required=False, help='''Add dependence on another image, format:
              "(<image_ID>,<image_version>), ... " or multiple "-d (1,1.0.0) -d (2,1.0.0)"''')
@click.option('-s', '--security-counter', callback=validate_security_counter,
              help='Specify the value of security counter. Use the `auto` '
              'keyword to automatically generate it from the image version.')
@click.option('-v', '--version', callback=validate_version,  required=True)
@click.option('--align', type=click.Choice(['1', '2', '4', '8', '16']),
              required=True)
@click.option('--public-key-format', type=click.Choice(['hash', 'full']),
              default='hash', help='In what format to add the public key to '
              'the image manifest: full key or hash of the key.')
@click.option('-k', '--key', metavar='filename', default=None, 
              help='optional key for signature '
              'if key not present TLV signature is not present.')
@click.option('-c', '--clear', required=False, is_flag=True,
              default=False, help='Output is not Encrypted, So that it can be written in primary slot')
@click.option('-l','--licence', type=click.Choice(['0', '1']),
              required=False, default='0', help='Fix Licence : 0 (global licence) 1(chip licence)')
@click.option('-C','--core', callback=validate_version, required=False,
               default='0.0.0')
@click.option('-p','--profile', type=click.Choice(['1', '2', '3','4','5','6','7' ]),
              required=False, default='7', help='Fix Secure Manager Profile : 1 (small) | 2 (medium) | 4 (large) ')
@click.option('-u','--urot', callback=validate_version, required=False,
               default='0.0.0')
@click.option('-ma', '--magic-val', type=BasedIntAllParamType(), required=False,
              help='fix magic value in hex output file.', default="0x96f3b83d")
                      
@click.command(help='''Create a signed or unsigned image\n
               INFILE and OUTFILE are parsed as Intel HEX if the params have
               .hex extension, otherwise binary format is used''')
def sign(key, public_key_format, align, version, pad_sig, header_size,
         pad_header, slot_size, pad, confirm, max_sectors, overwrite_only,
         endian, encrypt, infile, outfile, dependencies, load_addr, hex_addr,
         erased_val, save_enctlv, security_counter, boot_record, custom_tlv,
         rom_fixed, otfdec, primary_only, clear, licence, core, profile, urot, magic_val, no_pad_tlv):

    if confirm:
        # Confirmed but non-padded images don't make much sense, because
        # otherwise there's no trailer area for writing the confirmed status.
        pad = True
    if magic_val==image.STiROT[0] and primary_only:
        raise click.UsageError("primary-only not compatible with this magic value")
    elif magic_val in image.STiROT and otfdec:
        raise click.UsageError("otfdec not compatible with this magic value")
    elif primary_only and otfdec:
        raise click.UsageError("primary-only and otfdec not compatible")
    elif primary_only:
        img = image.Image_Primary_Only(version=decode_version(version), header_size=header_size,
                          pad_header=pad_header, pad=pad, confirm=confirm,
                          align=int(align), slot_size=slot_size,
                          max_sectors=max_sectors, overwrite_only=overwrite_only,
                          endian=endian, load_addr=load_addr, rom_fixed=rom_fixed,
                          erased_val=erased_val, save_enctlv=save_enctlv,
                          security_counter=security_counter, magic_val=magic_val, no_pad_tlv=no_pad_tlv)
    elif otfdec:   
        img = image.Image_OTFDEC(otfdec, version=decode_version(version), header_size=header_size,
                          pad_header=pad_header, pad=pad, confirm=confirm,
                          align=int(align), slot_size=slot_size,
                          max_sectors=max_sectors, overwrite_only=overwrite_only,
                          endian=endian, load_addr=load_addr, rom_fixed=rom_fixed,
                          erased_val=erased_val, save_enctlv=save_enctlv,
                          security_counter=security_counter, magic_val=magic_val)
    else:
        img = image.Image(version=decode_version(version), header_size=header_size,
                          pad_header=pad_header, pad=pad, confirm=confirm,
                          align=int(align), slot_size=slot_size,
                          max_sectors=max_sectors, overwrite_only=overwrite_only,
                          endian=endian, load_addr=load_addr, rom_fixed=rom_fixed,
                          erased_val=erased_val, save_enctlv=save_enctlv,
                          security_counter=security_counter, magic_val=magic_val, no_pad_tlv=no_pad_tlv)
    img.load(infile)
    key = load_key(key) if key else None
    enckey = load_key(encrypt) if encrypt else None
    if enckey and key and not (isinstance(enckey, AESGCM)and isinstance(key, ECDSA256P1)):         
           if    ((isinstance(key, ECDSA256P1) and not isinstance(enckey, ECDSA256P1Public))
              or (isinstance(key, RSA) and not isinstance(enckey, RSAPublic))):
             raise click.UsageError("Signing and encryption must use the same type of key")

    if (pad_sig and hasattr(key, 'pad_sig')) or (magic_val in image.STiROT):
        key.pad_sig = True

    # Get list of custom protected TLVs from the command-line
    custom_tlvs = {}
    for tlv in custom_tlv:
        tag = int(tlv[0], 0)
        if tag in custom_tlvs:
            raise click.UsageError('Custom TLV %s already exists.' % hex(tag))
        if tag in image.TLV_VALUES.values():
            raise click.UsageError(
                'Custom TLV %s conflicts with predefined TLV.' % hex(tag))

        value = tlv[1]
        if value.startswith('0x'):
            if len(value[2:]) % 2:
                raise click.UsageError('Custom TLV length is odd.')
            custom_tlvs[tag] = bytes.fromhex(value[2:])
        else:
            custom_tlvs[tag] = value.encode('utf-8')
    img.create(key, public_key_format, enckey, dependencies=dependencies, sw_type=boot_record,
               custom_tlvs=custom_tlvs, clear=clear, licence=licence)
    img.save(outfile, hex_addr)
    #print(img)


class AliasesGroup(click.Group):

    _aliases = {
        "create": "sign",
    }

    def list_commands(self, ctx):
        cmds = [k for k in self.commands]
        aliases = [k for k in self._aliases]
        return sorted(cmds + aliases)

    def get_command(self, ctx, cmd_name):
        rv = click.Group.get_command(self, ctx, cmd_name)
        if rv is not None:
            return rv
        if cmd_name in self._aliases:
            return click.Group.get_command(self, ctx, self._aliases[cmd_name])
        return None


@click.command(help='Print imgtool version information')
@click.option('-b', '--build', default=True, is_flag=True, help='build number')
def version(build):
    if build:
        print(imgtool_version)
    else:
        print(f"{imgtool_version} {imgtool_build}")


@click.argument('infile')
@click.option('-l', '--layout', metavar='filename',
              required=True,
              help='Location of the file that contains preprocessed macros')
@click.option('-m', '--macro',
              required=True,
              help='macro symbol string to grep in preprocessed file')
@click.option('-b', '--begin',
              required=True,
              help='begin of line to replace ')
@click.option('-s', '--setting',type=click.Choice(['0','1']),
              required=True,
              help='search for window batch set variable')
@click.option('-d', '--division',type=BasedIntAllParamType(),
              required=False,default='0',
              help='divide the value')  
@click.option('-p', '--phexa',type=click.Choice(['0','1']),
              required=False,default='1',
              help='print the value in hexa')
@click.command(help='modify bash /batch variable value with layout file content')              
def flash(infile, layout, macro, begin, setting, division, phexa ):
    image_value_re = re.compile(r"^\s*"+macro+"\s*=\s*(.*)")
    try:
        value = macro_parser.evaluate_macro(layout, image_value_re, 0, 1)
    except:  
         pass
    if value is not None:      
        if setting == '1':
            begin_line="set "+begin
        else:
            begin_line=begin

        for line in fileinput.input(infile, inplace=True):
            if line.startswith(begin_line):
                if division:
                    value = int(value/int(division))
                if phexa == '0':
                    line = begin_line+"="+str(value)+"\n"
                else:
                    line = begin_line+"="+hex(value)+"\n"
            sys.stdout.write(line)
@click.argument('outfile')
@click.argument('infile')
@click.option('-i', '--input-size', type=BasedIntAllParamType(), required=True,
              help='input padding size')
@click.option('-o', '--optional-size', type=BasedIntAllParamType(), required=False,default='0',
              help='optional padding size')
@click.option('-f', '--file', metavar='filename',default=None,
              help='optional file to place before inputfile and to padd up to --optional-size')
@click.command(help='''Assemble and padd 1 or 2 binaries''')             
def ass(file, input_size, optional_size, infile, outfile ):
    big_binary=b''
    with open(infile, 'rb') as f:
        binary=f.read()
        if input_size != 0:
            binary+=(input_size-len(binary))*pack("B",0xff)
    if file is not None :
        with open(file, 'rb') as f:
            optional_binary=f.read()
    else:
        optional_binary=b''
    if optional_size != 0:
        optional_binary+=(optional_size-len(optional_binary))*pack("B",0xff)
    with open(outfile, 'wb') as f:
       big_binary=optional_binary+binary
       f.write(big_binary) 

@click.option('-i', '--image-num', type=int,default=0,
              help='Fix image number to clean 1 for STuRot, 2 for SecMng, 3 for STiRot, 0 image clear only')
@click.argument('outfile')
@click.argument('infile')
@click.option('-f', '--file', metavar='filename',default=None,
              help='code in clear to replace')
@click.command(help='''clean an ST file''')
def clean(file, infile, outfile, image_num ):
    binary=b''
    
    try:
        with open(file, 'rb') as f:
            binary=f.read()
        outputdata = image.Image.clean(infile, binary,image_num)
        if outputdata is not None:
            with open(outfile, 'wb') as f:
               f.write(outputdata)
    except Exception as e:
        print(str(e)+", Error")
        
@click.command(cls=AliasesGroup,
               context_settings=dict(help_option_names=['-h', '--help']))
def imgtool():
    pass


imgtool.add_command(keygen)
imgtool.add_command(getpub)
imgtool.add_command(getpriv)
imgtool.add_command(getprivbin)
imgtool.add_command(getpubbin)
imgtool.add_command(sign)
imgtool.add_command(resign)
imgtool.add_command(version)
imgtool.add_command(flash)
imgtool.add_command(ass)
imgtool.add_command(clean)

#imgtool()
