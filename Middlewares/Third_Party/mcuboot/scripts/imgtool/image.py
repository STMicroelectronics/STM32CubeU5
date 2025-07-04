# Copyright 2018 Nordic Semiconductor ASA
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

from imgtool.version import decode_version 
from imgtool.boot_record import create_sw_component_data
import click
from enum import Enum
from intelhex import IntelHex
import hashlib
import struct
import os.path
from imgtool.keys import rsa, ecdsa, x25519
from cryptography.hazmat.primitives.asymmetric import ec, padding
from cryptography.hazmat.primitives.asymmetric.x25519 import X25519PrivateKey
from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes
from cryptography.hazmat.primitives.kdf.hkdf import HKDF
from cryptography.hazmat.primitives.serialization import Encoding, PublicFormat
from cryptography.hazmat.backends import default_backend
from cryptography.hazmat.primitives import hashes, hmac
from cryptography.exceptions import InvalidSignature
from cryptography.hazmat.primitives.hashes import SHA256
from cryptography.hazmat.primitives.asymmetric import utils
import ctypes
import binascii
#specific for OTFDEC 
c_uint8 = ctypes.c_uint8
c_uint32 = ctypes.c_uint32
class Flags_bits(ctypes.LittleEndianStructure):
    _fields_ = [
            ("pic", c_uint32, 1),
            ("primary_only", c_uint32, 1),
            ("encrypted", c_uint32, 1),
            ("otfdec", c_uint32, 1),
            ("non_bootable", c_uint32, 1),
            ("ram_load", c_uint32, 1),
            ("unused_1", c_uint32, 2),
            ("rom_fixed", c_uint32, 1),
            ("unused_2", c_uint32, 7+14),
            ("chip_specific", c_uint32, 1),
            ("licence_file", c_uint32, 1)
        ]

class Flags(ctypes.Union):
    _fields_ = [("b", Flags_bits),
                ("asbyte", c_uint32)]
    def __repr__(self):
        return f"Flags: pic={self.b.pic}, primary_only={self.b.primary_only}, encrypted={self.b.encrypted}, otfdec={self.b.otfdec}, non_bootable={self.b.non_bootable}, ram_load={self.b.ram_load}, unused_1={self.b.unused_1}, rom_fixed={self.b.rom_fixed}, unused_2={self.b.unused_2}, chip_specific={self.b.chip_specific}, licence_file={self.b.licence_file}"
    def set_bit(self, field_name, value):
        if hasattr(self.b, field_name):
            setattr(self.b, field_name, 1 if value else 0)
        else:
            raise ValueError(f"Field {field_name} does not exist in Flags_bits")


IMAGE_HEADER_SIZE = 32
BIN_EXT = "bin"
INTEL_HEX_EXT = "hex"
DEFAULT_MAX_SECTORS = 128
MAX_ALIGN = 16
DEP_IMAGES_KEY = "images"
DEP_VERSIONS_KEY = "versions"

STiROT=[0x73ab1024] #, 0x55f7394e, 0xc1592ee9

# Image header flags.
IMAGE_F = {
        'PIC':                   0x00000001,
        'ENCRYPTED':             0x00000004,
        'NON_BOOTABLE':          0x00000010,
        'RAM_LOAD':              0x00000020,
        'ROM_FIXED':             0x00000100,
        'OTFDEC':                0x00000008,
        #Warning Primary only is used in mcuboot V1.8 to pass 
        #An encryption size, question need to support this size ?
        'PRIMARY_ONLY':          0x00000002,
        #New flag for licence file
        'LICENSE_FILE':          0x80000000,
        'CHIP_SPECIFIC':         0x40000000,
}

TLV_VALUES = {
        'KEYHASH': 0x01,
        'PUBKEY': 0x02,
        'SHA256': 0x10,
        'RSA2048': 0x20,
        'ECDSA224': 0x21,
        'ECDSA256': 0x22,
        'RSA3072': 0x23,
        'ED25519': 0x24,
        'ENCRSA2048': 0x30,
        'ENCKW128': 0x31,
        'ENCEC256': 0x32,
        'ENCX25519': 0x33,
        'DEPENDENCY': 0x40,
        'SEC_CNT': 0x50,
        'BOOT_RECORD': 0x60,
        'AUTH_TAG': 0x03A0,
        'MODULE_DEPENDENCY': 0x02A0,
        'MODULE_INFO': 0x00A0
}


TLV_SIZE = 4
TLV_INFO_SIZE = 4
TLV_INFO_MAGIC = 0x6907
TLV_PROT_INFO_MAGIC = 0x6908

OverWrite = {"SHORT_OVERWRITE":1, "LONG_OVERWRITE" : 2, "NO_OVERWRITE":3}



Boot_Magic = bytes([
    0x77, 0xc2, 0x95, 0xf3,
    0x60, 0xd2, 0xef, 0x7f,
    0x35, 0x52, 0x50, 0x0f,
    0x2c, 0xb6, 0x79, 0x80, ])

STRUCT_ENDIAN_DICT = {
        'little': '<',
        'big':    '>'
}


class TLV():
    def __init__(self, endian, magic=TLV_INFO_MAGIC):
        self.magic = magic
        self.buf = bytearray()
        self.endian = endian
        self.key_licence=False

    def clear(self):
        self.buf = bytearray()

    def __len__(self):
        return TLV_INFO_SIZE + len(self.buf)

    def add(self, kind, payload):
        """
        Add a TLV record.  Kind should be a string found in TLV_VALUES above.
        """
        tlv_value=None
        if isinstance(kind, int):
            tlv_value=kind
        elif kind in TLV_VALUES:
            tlv_value=TLV_VALUES[kind]
        if tlv_value is not None:
            e = STRUCT_ENDIAN_DICT[self.endian]
            buf = struct.pack(e + 'HH', tlv_value, len(payload))
            self.buf += buf
            self.buf += payload
            #print(kind+" "+hex(len(payload))+" / "+" ".join(f"{byte:02X}" for byte in payload))
        else:
            raise click.UsageError(f"add TLV {kind} error")

    def add_fake_tlv(self,length):
        if length:
            e = STRUCT_ENDIAN_DICT[self.endian]
            if length>=TLV_SIZE:
                padding=length -TLV_SIZE
            else:
                padding=length + 16
            buf = struct.pack(e + 'HH', TLV_VALUES['BOOT_RECORD'], padding)
            self.buf += buf+bytearray([0]*padding)
            #print(buf+bytearray([0]*padding))

    def set_key(self, key):
        # Calculate the hash of the public key
        self.key=key
    
    def is_key_public(self):
        if "Public" in self.key.__class__.__name__:
            return True
        else:
            return False

    def get_public_key(self):
        if hasattr(self,'key')  and self.key is not None:
            return self.key.get_public_bytes()
        else:
            return None
    
    def get_hashed_public_key_bytes(self):
        if hasattr(self,'key') and self.key is not None:
            publicKey = self.key.get_public_bytes()
            sha = hashlib.sha256()
            sha.update(publicKey)
            return sha.digest()
        else:
            return bytes(hashlib.sha256().digest_size)

    def get(self):
        if len(self.buf) == 0:
            return bytes()
        e = STRUCT_ENDIAN_DICT[self.endian]
        header = struct.pack(e + 'HH', self.magic, len(self))
        return header + bytes(self.buf)
        
    def sign(self, enckey, public_key_format, payload):
        # Note that ecdsa wants to do the hashing itself, which means
        # we get to hash it twice.
        sha = hashlib.sha256()
        sha.update(payload)
        self.digest = sha.digest()
        self.add('SHA256', self.digest)
        if hasattr(self,'key') and self.key is not None:
            pubkey=self.get_public_key()
            if enckey is not None and enckey.sig_type()=="AESGCM_SHA256":
                #The key identifies the third party authority
                #public key is set in AUTH_TAG
                #A signature is also put in AUTH TAG
                if hasattr(enckey, 'sign'):
                    sig = enckey.sign(bytes(payload))
                else:
                    sig = enckey.sign_digest(self.digest)
                
                if not self.is_key_public():
                    if hasattr(self.key, 'sign'):
                        sig_auth = self.key.sign(bytes(payload))
                    else:
                        sig_auth = self.key.sign_digest(self.digest)
                    self.add(enckey.sig_tlv(), sig+pubkey+sig_auth)
                else:
                    #add sig author and pubbbytes
                    self.add(enckey.sig_tlv(), sig+pubkey)
            else:
                if public_key_format == 'hash':
                    self.add('KEYHASH', self.get_hashed_public_key_bytes())
                else:
                    self.add('PUBKEY', pubkey) 
                # `sign` expects the full image payload (sha256 done internally),
                # while `sign_digest` expects only the digest of the payload

                if hasattr(self.key, 'sign'):
                    sig = self.key.sign(bytes(payload))
                else:
                    sig = self.key.sign_digest(self.digest)
                #print(payload)
                self.add(self.key.sig_tlv(), sig)

    def add_key(self,enckey, plainkey):
        if isinstance(enckey, rsa.RSAPublic):
            cipherkey = enckey._get_public().encrypt(
                plainkey, padding.OAEP(
                    mgf=padding.MGF1(algorithm=hashes.SHA256()),
                    algorithm=hashes.SHA256(),
                    label=None))
            #pk=enckey._get_private()
            #plainkey = pk.decrypt(cipherkey, padding.OAEP(mgf=padding.MGF1(algorithm=SHA256()), algorithm=SHA256(), label=None) )
            
            enctlv_len = len(cipherkey)
            self.add('ENCRSA2048', cipherkey)
        elif isinstance(enckey, (ecdsa.ECDSA256P1Public,
                                 x25519.X25519Public)):
            cipherkey, mac, pubk = TLV._ecies_hkdf(enckey, plainkey)
            enctlv = pubk + mac + cipherkey
            enctlv_len = len(enctlv)
            if isinstance(enckey, ecdsa.ECDSA256P1Public):
                self.add('ENCEC256', enctlv)
            else:
                #print("encode ENCX25519")
                self.add('ENCX25519', enctlv)
        elif self.key_licence:
            #licence fichier extern
            enctlv_len=0
        else:
            raise click.UsageError("unknown key")
        return enctlv_len

    def _ecies_hkdf(enckey, plainkey):
        if isinstance(enckey, ecdsa.ECDSA256P1Public):
            newpk = ec.generate_private_key(ec.SECP256R1(), default_backend())
            shared = newpk.exchange(ec.ECDH(), enckey._get_public())
        else:
            newpk = X25519PrivateKey.generate()
            shared = newpk.exchange(enckey._get_public())
        derived_key = HKDF(
            algorithm=hashes.SHA256(), length=48, salt=None,
            info=b'MCUBoot_ECIES_v1', backend=default_backend()).derive(shared)
        encryptor = Cipher(algorithms.AES(derived_key[:16]),
                           modes.CTR(bytes([0] * 16)),
                           backend=default_backend()).encryptor()
        cipherkey = encryptor.update(plainkey) + encryptor.finalize()
        mac = hmac.HMAC(derived_key[16:], hashes.SHA256(),
                        backend=default_backend())
        mac.update(cipherkey)
        ciphermac = mac.finalize()
        if isinstance(enckey, ecdsa.ECDSA256P1Public):
            pubk = newpk.public_key().public_bytes(
                encoding=Encoding.X962,
                format=PublicFormat.UncompressedPoint)
        else:
            pubk = newpk.public_key().public_bytes(
                encoding=Encoding.Raw,
                format=PublicFormat.Raw)
        return cipherkey, ciphermac, pubk

    def __str__(self):
        s=""
        for x in self.buf:
            s+=hex(x)+" "
        return s
 
class Image():
    MAX_SW_TYPE_LENGTH = 12  # Bytes
    def STiROTlist():
        return " ".join(hex(x) for x in STiROT)
       
    def __init__(self, version=None, header_size=IMAGE_HEADER_SIZE,
                 pad_header=False, pad=False, confirm=False, align=1,
                 slot_size=0, max_sectors=DEFAULT_MAX_SECTORS,
                 overwrite_only=False, endian="little", load_addr=0,
                 rom_fixed=None, erased_val=None, save_enctlv=False,
                 security_counter=None, magic_val=0x96f3b83d, no_pad_tlv=True):

        if load_addr and rom_fixed:
            raise click.UsageError("Can not set rom_fixed and load_addr at the same time")
        self.version = version or decode_version("0")
        self.header_size = header_size
        self.pad_header = pad_header
        self.pad = pad
        self.confirm = confirm
        self.align = align
        self.slot_size = slot_size
        self.max_sectors = max_sectors
        if magic_val in STiROT:
            self.overwrite_only=OverWrite["SHORT_OVERWRITE"]
            #print("SHORT_OVERWRITE")
        elif overwrite_only:
            self.overwrite_only=OverWrite["LONG_OVERWRITE"]
            #print("LONG_OVERWRITE")
        else:
            self.overwrite_only=OverWrite["NO_OVERWRITE"]
        self.endian = endian
        self.base_addr = None
        self.load_addr = 0 if load_addr is None else load_addr
        self.rom_fixed = rom_fixed
        self.erased_val = 0xff if erased_val is None else int(erased_val, 0)
        self.payload = []
        self.enckey = None
        self.save_enctlv = save_enctlv    
        self.enctlv_len = 0
        self.magic_val= magic_val
        self.no_pad_tlv=no_pad_tlv

        if security_counter == 'auto':
            # Security counter has not been explicitly provided,
            # generate it from the version number
            self.security_counter = ((self.version.major << 24)
                                     + (self.version.minor << 16)
                                     + self.version.revision)
        else:
            self.security_counter = security_counter

    def __repr__(self):
        ow=list(OverWrite.keys())[list(OverWrite.values()).index(self.overwrite_only)]
        return "<Image version={}, header_size={}, security_counter={}, \
                base_addr={}, load_addr={}, align={}, slot_size={}, \
                max_sectors={}, overwrite_only={}, endian={} format={}, \
                payloadlen=0x{:x}>".format(
                    self.version,
                    self.header_size,
                    hex(self.security_counter),
                    hex(self.base_addr) if self.base_addr is not None else "N/A",
                    hex(self.load_addr),
                    self.align,
                    hex(self.slot_size),
                    self.max_sectors,
                    ow,
                    self.endian,
                    self.__class__.__name__,
                    len(self.payload))

    def load(self, path):
        """Load an image from a given file"""
        ext = os.path.splitext(path)[1][1:].lower()
        try:
            if ext == INTEL_HEX_EXT:
                ih = IntelHex(path)
                self.payload = ih.tobinarray()
                self.base_addr = ih.minaddr()
            else:
                with open(path, 'rb') as f:
                    self.payload = f.read()
        except FileNotFoundError:
            raise click.UsageError("Input file not found")

        # Add the image header if needed.
        if self.pad_header and self.header_size > 0:
            if self.base_addr:
                # Adjust base_addr for new header
                self.base_addr -= self.header_size
            self.payload = bytes([self.erased_val] * self.header_size) + \
                self.payload

        self.check_header()

    def save(self, path, hex_addr=None):
        """Save an image from a given file"""
        ext = os.path.splitext(path)[1][1:].lower()
        if ext == INTEL_HEX_EXT:
            # input was in binary format, but HEX needs to know the base addr
            if self.base_addr is None and hex_addr is None:
                raise click.UsageError("No address exists in input file "
                                       "neither was it provided by user")
            h = IntelHex()
            if hex_addr is not None:
                self.base_addr = hex_addr
            h.frombytes(bytes=self.payload, offset=self.base_addr)
            if self.pad:
                trailer_size = self._trailer_size(self.align, self.max_sectors,
                                                  self.overwrite_only,
                                                  self.enckey,
                                                  self.save_enctlv,
                                                  self.enctlv_len)
                trailer_addr = (self.base_addr + self.slot_size) - trailer_size
                padding = bytearray([self.erased_val] * 
                                    (trailer_size - len(Boot_Magic)))
                if self.confirm and OverWrite["NO_OVERWRITE"]==self.overwrite_only:
                    padding[-MAX_ALIGN] = 0x01  # image_ok = 0x01
                padding += Boot_Magic
                h.puts(trailer_addr, bytes(padding))
            h.tofile(path, 'hex')
        else:
            if self.pad:
                self.payload += self.pad_to(self.slot_size)
            with open(path, 'wb') as f:
                f.write(self.payload)

    def check_header(self):
        if self.header_size > 0 and not self.pad_header:
            if any(v != 0 for v in self.payload[0:self.header_size]):
                raise click.UsageError("Header padding was not requested and "
                                       "image does not start with zeros")

    def check_trailer(self):
        if self.slot_size > 0:
            tsize = self._trailer_size(self.align, self.max_sectors,
                                       self.overwrite_only, self.enckey,
                                       self.save_enctlv, self.enctlv_len)
            padding = self.slot_size - (len(self.payload) + tsize)
            if padding < 0:
                msg = "Image size (0x{:x}) + trailer (0x{:x}) exceeds " \
                      "requested size 0x{:x}".format(
                          len(self.payload), tsize, self.slot_size)
                raise click.UsageError(msg)

    def _alignment(self):
        pad_len = len(self.payload) % 16
        if pad_len > 0:
            pad = bytes(16 - pad_len)
            if isinstance(self.payload, bytes):
                self.payload += pad
            else:
                self.payload.extend(pad)

    def _build_prot_tlv(self, boot_record, dependencies, custom_tlvs, fake_padding) :
        prot_tlv = TLV(self.endian, TLV_PROT_INFO_MAGIC)

        e = STRUCT_ENDIAN_DICT[self.endian]

        if self.security_counter is not None:
            payload = struct.pack(e + 'I', self.security_counter)
            prot_tlv.add('SEC_CNT', payload)

        if fake_padding is not None and fake_padding:
            prot_tlv.add_fake_tlv(fake_padding)
        
        if boot_record is not None:
            prot_tlv.add('BOOT_RECORD', boot_record)

        if dependencies is not None:
            for i in range(len(dependencies[DEP_IMAGES_KEY])):
                payload = struct.pack(
                                e + 'B3x'+'BBHI',
                                int(dependencies[DEP_IMAGES_KEY][i]),
                                dependencies[DEP_VERSIONS_KEY][i].major,
                                dependencies[DEP_VERSIONS_KEY][i].minor,
                                dependencies[DEP_VERSIONS_KEY][i].revision,
                                dependencies[DEP_VERSIONS_KEY][i].build
                                )
                prot_tlv.add('DEPENDENCY', payload)

        if custom_tlvs is not None:
            for tag, value in custom_tlvs.items():
                prot_tlv.add(tag, value)
        return prot_tlv      

    def _crypt(self, plainkey, nonce):
        cipher = Cipher(algorithms.AES(plainkey), modes.CTR(nonce), backend=default_backend())
        encryptor = cipher.encryptor()
        img = bytes(self.payload[self.header_size:])
        #encrypt image
        #img_other = img
        #encrypted =  encryptor_other.encrypt(img_other)
        #nonce_other = encryptor_other.nonce
        #print("nonce encryptor"+bytes(nonce).hex())
        #print("encrypted "+bytes(encrypted[:16]).hex())
        img = encryptor.update(img) + encryptor.finalize()
        #img=encrypted
        #print("img "+bytes(img[:16]).hex())              

        #update image , to build image non encrypted with a key in header to allow swap, an option is
        #required their, this option
        return img

    def add_header(self, enckey, protected_tlv_size, force_encrypted=False, chip_licence=None, image_flag=False):
        """Install the image header."""
        flags = 0
        if enckey is not None:

            #encrypted primary image only has hash computed on a header without flag encrypted.
            if not image_flag:
                if enckey.sig_type()!="AESGCM_SHA256":
                    flags |= IMAGE_F['ENCRYPTED']
                else:
                    flags |= IMAGE_F['LICENSE_FILE']
                    if chip_licence == '1':
                        flags |= IMAGE_F['CHIP_SPECIFIC']
            if force_encrypted:
                flags |= IMAGE_F['ENCRYPTED']
        if self.load_addr != 0:
            # Indicates that this image should be loaded into RAM
            # instead of run directly from flash.
            flags |= IMAGE_F['RAM_LOAD']
        if self.rom_fixed:
            flags |= IMAGE_F['ROM_FIXED']
        if image_flag:
            flags |= IMAGE_F[image_flag]
        e = STRUCT_ENDIAN_DICT[self.endian]
        fmt = (e +
               # type ImageHdr struct {
               'I' +     # Magic    uint32
               'I' +     # LoadAddr uint32
               'H' +     # HdrSz    uint16
               'H' +     # PTLVSz   uint16
               'I' +     # ImgSz    uint32
               'I' +     # Flags    uint32
               'BBHI' +  # Vers     ImageVersion
               'I'       # Pad1     uint32
               )  # }
        assert struct.calcsize(fmt) == IMAGE_HEADER_SIZE
        header = struct.pack(fmt,
                self.magic_val,
                self.rom_fixed or self.load_addr,
                self.header_size,
                protected_tlv_size,  # TLV Info header + Protected TLVs
                self.endof_payload_section - self.header_size,  # ImageSz
                flags,
                self.version.major,
                self.version.minor or 0,
                self.version.revision or 0,
                self.version.build or 0,
                0)  # Pad1
        self.payload = bytearray(self.payload)
        self.payload[:len(header)] = header

    def _trailer_size(self, write_size, max_sectors, mode, enckey,
                      save_enctlv, enctlv_len):
        # NOTE: should already be checked by the argument parser
        magic_size = 16
        if mode==OverWrite["SHORT_OVERWRITE"]:
            return MAX_ALIGN  + magic_size
        if mode==OverWrite["LONG_OVERWRITE"]:
            #print("overwrite_only :"+str(overwrite_only))
            return MAX_ALIGN * 2 + magic_size
        else:
            if write_size not in set([1, 2, 4, 8, 16]):
                raise click.BadParameter("Invalid alignment: {}".format(
                    write_size))
            m = DEFAULT_MAX_SECTORS if max_sectors is None else max_sectors
            trailer = m * 3 * write_size  # status area
            if enckey is not None:
                if save_enctlv:
                    # TLV saved by the bootloader is aligned
                    keylen = (int((enctlv_len - 1) / MAX_ALIGN) + 1) * MAX_ALIGN
                else:
                    keylen = 16
                trailer += keylen * 2  # encryption keys
            trailer += MAX_ALIGN * 4  # image_ok/copy_done/swap_info/swap_size
            trailer += magic_size
            return trailer

    def pad_to(self, size):
        """Pad the image to the given size, with the given flash alignment."""
        tsize = self._trailer_size(self.align, self.max_sectors,
                                   self.overwrite_only, self.enckey,
                                   self.save_enctlv, self.enctlv_len)
        padding = size - (len(self.payload) + tsize)
        pbytes = bytearray([self.erased_val] * padding)
        pbytes += bytearray([self.erased_val] * (tsize - len(Boot_Magic)))
        if self.confirm and OverWrite["NO_OVERWRITE"]==self.overwrite_only:
            pbytes[-MAX_ALIGN] = 0x01  # image_ok = 0x01
        pbytes += Boot_Magic
        return pbytes #this way allow to count pad length
        
    @staticmethod            
    def resign(imgfile, key, public_key_format, hex_addr, overwrite_only,
               endian, align, confirm, pad, save_enctlv, erased_val, slot_size, max_sectors):
        #open file
        with open(imgfile, "rb") as f:
            b = f.read()
        #decode header
        magic, Load_addr, header_size, ptlvs, img_size, flags = struct.unpack('IIHHII', b[:20])
        version = struct.unpack('BBHI', b[20:28])
        decodedflags = Flags()
        decodedflags.asbyte = flags
        if not decodedflags.b.licence_file:
            raise click.UsageError("no licence flag "+ str(flags))
        #dummy parameter for image creation, image is created to use img save method
        img = Image(version=decode_version("0.0.0"), header_size=header_size,
                      pad_header=0, pad=pad, confirm=confirm,
                      align=int(align), slot_size=slot_size,
                      max_sectors=max_sectors, overwrite_only=overwrite_only,
                      endian=endian, load_addr=0x0, rom_fixed=False,
                      erased_val=erased_val, save_enctlv=save_enctlv,
                      security_counter='auto')
        #protected TLV header to modify is here
        tlv_nonprotected_off = header_size + img_size + ptlvs
        tlv_info=b[tlv_nonprotected_off:tlv_nonprotected_off + TLV_INFO_SIZE]
        magic, tlv_tot = struct.unpack('HH', tlv_info)
        if magic!=TLV_INFO_MAGIC:
            raise click.UsageError("magic error")
        #start of protected tlv
        tlv_off = tlv_nonprotected_off+ TLV_INFO_SIZE
        tlv_end = tlv_nonprotected_off + tlv_tot
        digest=None
        #parse all protected TLV to check 
        #sha256 is present, and no signature, no public key
        #recreate tlv unprotected with all info present
        unprot_tlv = TLV(endian, TLV_INFO_MAGIC)
        while tlv_off < tlv_end:
            tlv = b[tlv_off:tlv_off+TLV_SIZE]
            tlv_type, tlv_len = struct.unpack('HH', tlv)
            #get TLV string value
            off = tlv_off + TLV_SIZE
            if tlv_type == TLV_VALUES["SHA256"]:
                digest=b[off:off+tlv_len]
            if tlv_type == TLV_VALUES["KEYHASH"]:
                raise click.UsageError("Unexpected KEYHASH")
            if tlv_type == TLV_VALUES["PUBKEY"]:
                raise click.UsageError("Unexpected PUBKEY")
            if tlv_type == TLV_VALUES["RSA2048"]:
                raise click.UsageError("Unexpected RSA2048")
            if tlv_type == TLV_VALUES["ECDSA224"]:
                raise click.UsageError("Unexpected ECDSA224")
            if tlv_type == TLV_VALUES["ECDSA256"]:
                raise click.UsageError("Unexpected ECDSA256")
            if tlv_type == TLV_VALUES["RSA3072"]:
                raise click.UsageError("Unexpected RSA3072")
            if tlv_type == TLV_VALUES["ED25519"]:
                raise click.UsageError("Unexpected ED25519")
            unprot_tlv.add(tlv_type,b[off:off+tlv_len])
            tlv_off+= TLV_SIZE + tlv_len
        if digest is None:
            raise click.UsageError("No digest")
        #compute Key value to be put in tlv
        if key is not None:
            pub = key.get_public_bytes()
            sha = hashlib.sha256()
            sha.update(pub)
            pubbytes = sha.digest()
        else:
            pubbytes = bytes(hashlib.sha256().digest_size)
        #add key info to protlv
        if public_key_format == 'hash':
            unprot_tlv.add('KEYHASH', pubbytes)
        else:
            unprot_tlv.add('PUBKEY', pub)
        #compute signature from sha256 hash
        signature = key.sign_digest(digest)
        #add signature to protlv
        unprot_tlv.add(key.sig_tlv(),signature)
        #build img new payload file
        img.payload=b[:header_size + img_size + ptlvs]
        img.payload+=unprot_tlv.get()
        return img

    def clean(imgfile, binary, image_type):
        with open(imgfile, "rb") as f:
            b = f.read()
        outbinary=b''
        # Magic    uint32
        # LoadAddr uint32
        # HdrSz    uint16
        # PTLVSz   uint16
        # ImgSz    uint32
        # Flags    uint32
        # Vers :    
        # iv_major uint8;
        # iv_minor uint8;
        # iv_revision uint16;
        # iv_build_num uint32;
        magic, Load_addr, header_size, ptlvs, img_size, flags = struct.unpack('IIHHII', b[:20])
        #padd binary with 0 to align on 16 bytes

        pad_len = len(binary) % 16
        if pad_len > 0:
            pad = bytes(16 - pad_len)
            binary+=pad
        if magic in STiROT and img_size>len(binary):
            pad = bytes(img_size - len(binary))
            binary+=pad
        if img_size == len(binary):
            offset = header_size+img_size
            header=b[:header_size]+binary+b[header_size+len(binary):header_size+img_size]
            val_cleaned=b'\0'*32
            tlv_off=offset+TLV_INFO_SIZE
            tlv_info = b[offset:tlv_off]
            magic_tlv, tlv_len_size = struct.unpack('HH', tlv_info)      
            if image_type in [ 1, 2 ]: #see excel file 
                if magic_tlv == TLV_PROT_INFO_MAGIC:
                    outbinary = tlv_info
                    end_tlv_prot=tlv_off+tlv_len_size-TLV_INFO_SIZE
                    while tlv_off < end_tlv_prot:
                        tlv_info=b[tlv_off:tlv_off+TLV_SIZE]
                        tlv_type, tlv_len = struct.unpack('HH', tlv_info) 
                        end_tlv=tlv_off+tlv_len+TLV_SIZE
                        #get TLV string value
                        if tlv_type==TLV_VALUES['BOOT_RECORD']:
                            shift=tlv_len-0x5A
                            if image_type == 1:
                                end_version_position=tlv_off+16+shift+3 #CBOR: BL size=2 and key size=3
                            elif image_type == 2:
                                end_version_position=tlv_off+16+shift+3 #CBOR: SPE size=3 and key size=3
                            tlv_value = b[tlv_off:end_version_position] + b'\0'*32 + b[end_version_position+32:end_tlv]
                        else:
                            tlv_value = b[tlv_off:end_tlv]
                        outbinary += tlv_value
                        tlv_off=end_tlv
                        #print(hex(tlv_type)+":"+" ".join(f"{byte:02X}" for byte in tlv_value)+f" ({hex(len(tlv_value))})")
                    tlv_off = header_size+img_size+ ptlvs
                    tlv_info=b[tlv_off:tlv_off+TLV_SIZE]
                    magic_tlv, tlv_len_size = struct.unpack('HH', tlv_info) 
                if magic_tlv==TLV_INFO_MAGIC:
                    outbinary += tlv_info
                    end_tlv_np = tlv_off + tlv_len_size-TLV_INFO_SIZE
                    tlv_off+=TLV_SIZE
                    while tlv_off < end_tlv_np:
                        tlv_info = b[tlv_off:tlv_off+TLV_SIZE]
                        tlv_type, tlv_len = struct.unpack('HH', tlv_info)
                        end_tlv=tlv_off+tlv_len+TLV_SIZE
                        if tlv_type == TLV_VALUES['SHA256'] or (tlv_type == TLV_VALUES['KEYHASH'] and image_type == 1):
                            begin_position=tlv_off+TLV_SIZE
                            tlv_value = b[tlv_off:begin_position] + val_cleaned
                        elif tlv_type == TLV_VALUES['ECDSA256']:
                            r_position=tlv_off+TLV_SIZE+5
                            s_position=r_position+32+3
                            tlv_value = b[tlv_off:r_position] + val_cleaned+b[r_position+32:s_position]+val_cleaned
                        elif tlv_type == TLV_VALUES['PUBKEY'] and image_type==2:
                            begin_position=tlv_off+TLV_SIZE
                            tlv_value = b[tlv_off:begin_position]+b'\0'*91
                        elif tlv_type == TLV_VALUES['ENCEC256']:
                            begin_position=tlv_off+TLV_SIZE+1
                            tlv_value = b[tlv_off:begin_position]+b'\0'*112
                        else:
                            tlv_value = b[tlv_off:end_tlv] + val_cleaned
                        #print(hex(tlv_type)+":"+" ".join(f"{byte:02X}" for byte in tlv_value)+f" ({hex(len(tlv_value))})")
                        outbinary += tlv_value
                        tlv_off=end_tlv
                outbinary=header+outbinary+b[tlv_off:]
            else:
                outbinary=header+b[offset:]
        if img_size != len(binary):
            print("file not substituable "+str(len(binary))+" "+str(img_size))
            return None
        #print(" ".join(f"{byte:02X}" for byte in outbinary))
        #print("yyyyyyy")
        
        #print(" ".join(f"{byte:02X}" for byte in outbinary))  
        return outbinary

    def _create_boot_record(self, sw_type, tlv):
        if sw_type is not None:
            if len(sw_type) > self.MAX_SW_TYPE_LENGTH:
                msg = "'{}' is too long ({} characters) for sw_type. Its " \
                      "maximum allowed length is 12 characters.".format(
                       sw_type, len(sw_type))
                raise click.UsageError(msg)

            image_version = f"{self.version.major}.{self.version.minor}.{self.version.revision}"

            # The image hash is computed over the image header, the image
            # itself and the protected TLV area. However, the boot record TLV
            # (which is part of the protected area) should contain this hash
            # before it is even calculated. For this reason the script fills
            # this field with zeros and the bootloader will insert the right
            # value later.
            digest = bytes(hashlib.sha256().digest_size)

            # Create CBOR encoded boot record
            boot_record = create_sw_component_data(sw_type=sw_type, sw_version=image_version, sw_measurement_description="SHA256", sw_measurement_value=digest, sw_signer_id=tlv.get_hashed_public_key_bytes())
        else:
            boot_record=None
        return boot_record

    def _handle_encryption_key(self, enckey, tlv, licence):
        """
        Handle the encryption key assignment and generate the plainkey.
        :param enckey: The encryption key object.
        :param tlv: The TLV object, which may be modified within this method.
        :return: The plainkey generated based on the encryption key type.
        """
        self.enckey = enckey
        if self.enckey is not None:
            if self.enckey.sig_type() == "AESGCM_SHA256":
                # In this mode, the binary is always encrypted
                # Force enckey to symmetric key

                # plain_key is the AES-GCM key
                if not licence:
                    raise click.UsageError("inappropriate key")
                plainkey = self.enckey.get_private_bytes()
                tlv.key_licence = True
            else:
                # If not a licence file, then generate a random key
                plainkey = os.urandom(16)
            
            # Add padding if the image is not aligned to the 16-byte boundary in encrypted mode
            self._alignment()
        else:
            plainkey = None
        
        return plainkey

    def _padded_protected_tlv(self, boot_record, dependencies, custom_tlvs, len_tlv, len_tlv_prot, verbose=False):
        """
        Ensure that a protected TLV is present and handle the padding of the payload.
        """
        if not boot_record and not dependencies and not custom_tlvs and not self.security_counter:
            raise click.UsageError('Need protected TLV for this magic, error')
        trailer_size = self._trailer_size(self.align, self.max_sectors,
                                          self.overwrite_only, self.enckey,
                                          self.save_enctlv, self.enctlv_len)
        endof_payload_section=len(self.payload)
        free = self.slot_size - endof_payload_section - len_tlv_prot - len_tlv - trailer_size
        if verbose:
            print(f"Initial end of payload section: {hex(endof_payload_section)}")
            print(f"Header size: {hex(self.header_size)}")
            print(f"Trailer size: {hex(trailer_size)}")
            print(f"Free space: {hex(free)}")
        if self.no_pad_tlv:
            empty_zone = free % 16
            self.payload += bytearray([0] * (free - empty_zone))
            endof_payload_section += free - empty_zone
            prot_tlv = self._build_prot_tlv(boot_record, dependencies, custom_tlvs, None)
            if verbose:
                print(f"Padding with {hex(free - empty_zone)} bytes")
                print(f"New end of payload section: {hex(endof_payload_section)}")
        else:
            len_fake_tlv = free % 16
            if len_fake_tlv < 5:
                len_fake_tlv += 16
            self.payload += bytearray([0] * (free - len_fake_tlv))
            endof_payload_section += free - len_fake_tlv
            prot_tlv = self._build_prot_tlv(boot_record, dependencies, custom_tlvs, len_fake_tlv)
            if verbose:
                print(f"Adding fake TLV of length: {len_fake_tlv}")
                print(f"New end of payload section: {hex(endof_payload_section)}")
        self.endof_payload_section=endof_payload_section
        return prot_tlv
        
    def create(self, key, public_key_format, enckey, dependencies=None,
               sw_type=None, custom_tlvs=None, clear=False, licence=None): 

        tlv = TLV(self.endian)
        tlv.set_key(key)
        
        boot_record=self._create_boot_record(sw_type, tlv)
        
        # At this point the image is already on the payload
        self.enckey = enckey
        #print(enckey)
        plainkey = self._handle_encryption_key(enckey, tlv, licence)
        
        # This adds the header to the payload as well
        self.endof_payload_section=len(self.payload)

        # Protected TLVs must be added first, because they are also included
        # in the hash calculation
        if boot_record or dependencies or custom_tlvs or self.security_counter is not None:
            prot_tlv=self._build_prot_tlv(boot_record, dependencies, custom_tlvs, None)
            #header have to be set before sign
            self.add_header(enckey, len(prot_tlv),chip_licence = licence)
            tlv.sign(enckey, public_key_format, self.payload+prot_tlv.get())
        else:
            prot_tlv = TLV(self.endian, TLV_PROT_INFO_MAGIC)
            #header have to be set before sign
            self.add_header(enckey, len(prot_tlv),chip_licence = licence)
            tlv.sign(enckey, public_key_format, self.payload)
        if enckey is not None:
            self.enctlv_len=tlv.add_key(enckey, plainkey)
            if self.magic_val in STiROT and not tlv.key_licence: #no_pad_tlv
                prot_tlv = self._padded_protected_tlv(boot_record, dependencies, custom_tlvs, len(tlv), len(prot_tlv))
                    
                #then rebuild tlv with the new signature
                tlv.clear()
                self.add_header(enckey, len(prot_tlv),chip_licence = licence)
                tlv.sign(enckey,public_key_format, self.payload+prot_tlv.get())
                self.enctlv_len=tlv.add_key(enckey, plainkey)
                #print("tlv prot len after padding :"+hex(len(prot_tlv)))
                #print("tlv len :"+hex(len(tlv)))
            #print("av "+hex(len(self.payload)) + " "+hex(len(prot_tlv))+" "+hex(len(tlv)))
            #print("av total "+hex(len(self.payload)+len(prot_tlv)+len(tlv)))
            if tlv.key_licence:
               nonce=tlv.digest[16:]
            else:
               nonce = bytes([0] * 16)
            #crypt
            if type(self.payload)!=bytearray:
                    self.payload=bytearray(self.payload)
            img=self._crypt(plainkey, nonce)
            if clear == False:
                self.payload[self.header_size:] = img
        else:
            self.add_header(enckey, len(prot_tlv),chip_licence = licence)
        #print("real position "+hex(len(self.payload)) + " and len "+hex(len(prot_tlv))+" "+hex(len(tlv)))
        #print("ap t "+hex(len(self.payload)+len(prot_tlv)+len(tlv)))
        self.payload += prot_tlv.get()
        #print(", ".join(hex(b) for b in self.payload))
        self.payload += tlv.get()
        #print("trailer size :"+hex(self._trailer_size(self.align, self.max_sectors, self.overwrite_only, self.enckey,self.save_enctlv, self.enctlv_len)))
        #print("after add tlv, end of file:"+hex(len(self.payload)+self._trailer_size(self.align, self.max_sectors, self.overwrite_only, self.enckey,self.save_enctlv, self.enctlv_len)))
        self.check_trailer()
        
class Image_Primary_Only(Image):
    def __init__(self, version=None, header_size=IMAGE_HEADER_SIZE,
                 pad_header=False, pad=False, confirm=False, align=1,
                 slot_size=0, max_sectors=DEFAULT_MAX_SECTORS,
                 overwrite_only=False, endian="little", load_addr=0,
                 rom_fixed=None, erased_val=None, save_enctlv=False,
                 security_counter=None, magic_val=0x96f3b83d, no_pad_tlv=True):
        #self.flags = 'PRIMARY_ONLY'
        super().__init__(version=version, header_size=header_size, pad_header=pad_header, pad=pad, confirm=confirm, align=align, slot_size=slot_size, max_sectors=max_sectors, overwrite_only=overwrite_only, \
              endian=endian, load_addr=load_addr, rom_fixed=rom_fixed, erased_val=erased_val, save_enctlv=save_enctlv, security_counter=security_counter, magic_val=magic_val, no_pad_tlv=no_pad_tlv)

    def create(self, key, public_key_format, enckey, dependencies=None,
           sw_type=None, custom_tlvs=None, clear=False, licence=None): 
  
        # Calculate the hash of the public key
        tlv = TLV(self.endian)
        tlv.set_key(key)
        if clear:
            raise click.UsageError("Not supported command")
            
        boot_record=self._create_boot_record(sw_type, tlv)

        self.enckey = enckey

        plainkey = self._handle_encryption_key(enckey, tlv, licence)
            
        # This adds the header to the payload as well
        self.endof_payload_section=len(self.payload)

        # Protected TLVs must be added first, because they are also included
        # in the hash calculation
        if boot_record or dependencies or custom_tlvs or self.security_counter:
            prot_tlv=self._build_prot_tlv(boot_record, dependencies, custom_tlvs, None)
            self.add_header(enckey, len(prot_tlv), force_encrypted=False, chip_licence = licence, image_flag='PRIMARY_ONLY')
            tlv.sign(enckey, public_key_format, self.payload+prot_tlv.get())#sign with tlv_protect
        else:
            prot_tlv = TLV(self.endian, TLV_PROT_INFO_MAGIC)
            self.add_header(enckey, len(prot_tlv), force_encrypted=False, chip_licence = licence, image_flag='PRIMARY_ONLY')
            tlv.sign(enckey, public_key_format, self.payload)#sign with tlv_protect  
 
        if enckey is not None:
            self.enctlv_len=tlv.add_key(enckey, plainkey)
            
            if self.magic_val in STiROT and not tlv.key_licence: #no_pad_tlv
                prot_tlv= self._padded_protected_tlv(boot_record, dependencies, custom_tlvs, len(tlv), len(prot_tlv))

                tlv.clear()
                self.add_header(enckey, len(prot_tlv),chip_licence = licence, image_flag='PRIMARY_ONLY')
                tlv.sign(enckey,public_key_format, self.payload+prot_tlv.get())
                self.enctlv_len=tlv.add_key(enckey, plainkey)
                #print(hex(self.enctlv_len))
            nonce = bytes([0] * 16)
            img=self._crypt(plainkey, nonce)
            self.payload[self.header_size:] = img
        self.add_header(enckey, len(prot_tlv), force_encrypted=True, chip_licence = licence, image_flag='PRIMARY_ONLY')
        self.payload += prot_tlv.get()
        self.payload += tlv.get()
        self.check_trailer()

    def _trailer_size(self, write_size, max_sectors, mode, enckey, save_enctlv, enctlv_len):
        # NOTE: should already be checked by the argument parser
        magic_size = 16
        return magic_size

class Image_OTFDEC(Image):
    def __init__(self,otfdec,version=None, header_size=IMAGE_HEADER_SIZE,
                 pad_header=False, pad=False, confirm=False, align=1,
                 slot_size=0, max_sectors=DEFAULT_MAX_SECTORS,
                 overwrite_only=False, endian="little", load_addr=0,
                 rom_fixed=None, erased_val=None, save_enctlv=False,
                 security_counter=None, magic_val=0x96f3b83d):
        self.address = 0 if otfdec is None else otfdec
        self.flags = 'OTFDEC' if otfdec is not None else otfdec
        super().__init__(version=version, header_size=header_size, pad_header=pad_header, pad=pad, confirm=confirm, align=align, slot_size=slot_size, max_sectors=max_sectors, overwrite_only=overwrite_only, \
              endian=endian, load_addr=load_addr, rom_fixed=rom_fixed, erased_val=erased_val, save_enctlv=save_enctlv, security_counter=security_counter, magic_val=magic_val)
    
    def _swap_bytes_in_blocks(self, data, block_size):
        """
        Swap bytes within each block of the given data.

        This function takes a byte array and a block size, and swaps the bytes
        within each block of the specified size. For example, if the block size
        is 4 and the data is [1, 2, 3, 4, 5, 6, 7, 8], the function will return
        [4, 3, 2, 1, 8, 7, 6, 5].

        Args:
            data (bytearray): The input data to be processed.
            block_size (int): The size of each block within which bytes will be swapped.

        Returns:
            bytearray: The data with bytes swapped within each block.
        """
        swapped_data = bytearray()
        for i in range(0, len(data), block_size):
            block = data[i:i + block_size]
            swapped_data.extend(block[::-1])
        return swapped_data

    def _crypt(self, plainkey, nonce):
        cipher = Cipher(algorithms.AES(plainkey), modes.CTR(nonce),
                        backend=default_backend())
        encryptor = cipher.encryptor()
        img = bytes(self.payload[self.header_size:])
        #add bytes to reach 16 bytes
        #print("initial len img"+hex(len(img)))
        toadd = (16-len(img)%16)
        img += toadd*b'0'
        #Swap bytes inside 16 bytes block for OTFDEC
        img = self._swap_bytes_in_blocks(img,16)
        #print("modified len img"+hex(len(img)))
        #encrypt image 
        #img_other = img
        #encrypted =  encryptor_other.encrypt(img_other)
        #nonce_other = encryptor_other.nonce
        #print("nonce encryptor"+bytes(nonce_other).hex())
        #print("encrypted "+bytes(encrypted[:16]).hex())
        img = encryptor.update(img) + encryptor.finalize()
        #print("img "+bytes(img[:16]).hex())
        #img=encrypted
        #print("img "+bytes(img[:16]).hex())
        #Swap bytes inside 16 bytes block for OTFDEC
        img = self._swap_bytes_in_blocks(img,16)
        #print("modify len img"+hex(len(img)))
        img = img[:-toadd]   
        #print("final len img"+hex(len(img)))
        return img
        
        
    def create(self, key, public_key_format, enckey, dependencies=None,
               sw_type=None, custom_tlvs=None, clear=False, licence=None): 
      
        # Calculate the hash of the public key
        tlv = TLV(self.endian)
        tlv.set_key(key)

        boot_record=self._create_boot_record(sw_type, tlv)

        self.enckey = enckey

        plainkey = self._handle_encryption_key(enckey, tlv, licence)
        
        # This adds the padding if image is not aligned to the 16 Bytes
        # in encrypted mode
        self._alignment()                
            
        # This adds the header to the payload as well
        self.endof_payload_section=len(self.payload)

        # Protected TLVs must be added first, because they are also included
        # in the hash calculation
        
        if boot_record or dependencies or custom_tlvs or self.security_counter:
            prot_tlv=self._build_prot_tlv(boot_record, dependencies, custom_tlvs, None)
        else:
            prot_tlv = TLV(self.endian, TLV_PROT_INFO_MAGIC)
        if enckey is not None:
            self.enctlv_len=tlv.add_key(enckey, plainkey)
            nonce=bytes([0] * 16)
            img=self._crypt(plainkey, nonce)
            if clear==False:
                if type(self.payload)!=bytearray:
                    self.payload=bytearray(self.payload) 
                self.payload[self.header_size:] = img
        self.add_header(enckey, len(prot_tlv), chip_licence = licence, image_flag='OTFDEC')
        # add the protected TLV 
        self.payload += prot_tlv.get()
        tlv.sign(enckey, public_key_format, self.payload)
        self.payload += tlv.get()
        self.check_trailer()