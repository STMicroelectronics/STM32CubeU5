"""
ECDSA key management
"""
import binascii
from cryptography.hazmat.backends import default_backend
from cryptography.hazmat.primitives import serialization
from cryptography.hazmat.primitives.asymmetric import ec
from cryptography.hazmat.primitives.hashes import SHA256
from cryptography.hazmat.primitives.asymmetric import utils
from .general import KeyClass

class ECDSAUsageError(Exception):
    pass

class ECDSA256P1Public(KeyClass):
    def __init__(self, key, extension="", priv=""):
        self.key = key
        self.extension=extension
        self.privname=priv
    def key_size(self):
        return self.key.key_size

    def shortname(self):
        return "ecdsa"

    def ext(self):
        return self.extension
 
    def name(self):
        return self.privname

    def _unsupported(self, name):
        raise ECDSAUsageError("Operation {} requires private key".format(name))

    def _get_public(self):
        return self.key

    def get_public_bytes(self):
        # The key is embedded into MBUboot in "SubjectPublicKeyInfo" format
        return self._get_public().public_bytes(
                encoding=serialization.Encoding.DER,
                format=serialization.PublicFormat.SubjectPublicKeyInfo)
    
    def get_public_bytes_raw(self):
        pn=  self._get_public().public_numbers()
        public_bytes = pn.x.to_bytes(32, byteorder='big') + pn.y.to_bytes(32, byteorder='big')
        return public_bytes

    def get_private_bytes(self, minimal):
        self._unsupported('get_private_bytes')

    def export_private(self, path, passwd=None):
        self._unsupported('export_private')

    def export_public(self, path):
        """Write the public key to the given file."""
        pem = self._get_public().public_bytes(
                encoding=serialization.Encoding.PEM,
                format=serialization.PublicFormat.SubjectPublicKeyInfo)
        with open(path, 'wb') as f:
            f.write(pem)

    def sig_type(self):
        return "ECDSA256_SHA256"

    def sig_tlv(self):
        return "ECDSA256"

    def sig_len(self):
        # Early versions of MCUboot (< v1.5.0) required ECDSA
        # signatures to be padded to 72 bytes.  Because the DER
        # encoding is done with signed integers, the size of the
        # signature will vary depending on whether the high bit is set
        # in each value.  This padding was done in a
        # not-easily-reversible way (by just adding zeros).
        #
        # The signing code no longer requires this padding, and newer
        # versions of MCUboot don't require it.  But, continue to
        # return the total length so that the padding can be done if
        # requested.
        return 72
 
    def _build_compress_signature(self, der):
        #DER format is
        #0 :ASN1 header (0x30)
        #1 :len
        #2 :tag (0x2)
        #3 :len R signature (including zero data)
        #4 :Zero data (0x0)
        #5 :R signature
        #4+ len R signature :tag (0x2)
        #4+ len R signature +1 :len S signature (including zero data)
        #4+ len R signature +2 :Zero data
        #4+ len R signature +3 :S signature
        #len of complete area
        offset_LEN=1
        #offset of length for R signature
        offset_LEN_R=3
        offset_LEN_S=der[offset_LEN_R]+5
        legend="Transform signature to be verify to "
        if der[offset_LEN] == 0x46 :
           #check that null bytes are present with first value < 128 , in this case
           #0 is not set because R and S are signed, 0 is set only to indicate that last
           #S is negative , else signature verification not working
           if  der[offset_LEN_R +1]== 0x0 and der[offset_LEN_R +2] > 128 and der[offset_LEN_S +1]== 0x0 and der[offset_LEN_S +2] > 128:
            b = der[0:offset_LEN]
            b += bytes([0x46, 2, 33]) + der[offset_LEN_R+1:offset_LEN_S-1]
            b += bytes([2, 33])+ der[offset_LEN_S+1:der[offset_LEN]+2]
            #print(f"{legend}size 0x46 : R (0x21) S (0x21)")
            return b
           elif  der[offset_LEN_R +1]== 0x0 and der[offset_LEN_R +2] < 128 and der[offset_LEN_S +1]== 0x0 and der[offset_LEN_S +2] < 128:
            b = der[0:offset_LEN]
            b += bytes([0x44, 2, 32]) + der[offset_LEN_R+2:offset_LEN_S-1]
            b += bytes([2, 32])+ der[offset_LEN_S+2:der[offset_LEN]+2]
            #print(f"{legend}size 0x44 : R (0x20) S (0x20)")
            return b
           elif der[offset_LEN_S +1]== 0x0 and der[offset_LEN_S +2] < 128:
            b = der[0:offset_LEN]
            b += bytes([0x45, 2, 33]) + der[offset_LEN_R+1:offset_LEN_S-1]
            b += bytes([2, 32])+ der[offset_LEN_S+2:der[offset_LEN]+2]
            #print(f"{legend}size 0x45 : R (0x21) S (0x20)")
            return b
           else :
            b = der[0:offset_LEN]
            b += bytes([0x45, 2, 32]) + der[offset_LEN_R+2:offset_LEN_S-1]
            b += bytes([2, 33])+ der[offset_LEN_S+1:der[offset_LEN]+2]
            #print(f"{legend}size 0x45 : R (0x20) S (0x21)")
            return b
        return der

    def _key_info_(self, der):
        offset_LEN=1
        offset_LEN_R=3
        if der[0] != 0x30:
            raise ValueError("Incorrect signature")
        else:
            # Signature size
            total_length = der[offset_LEN]
            
            # default initialization
            r_length = "not present"
            s_length = "not present"
            
            # extract and verify R
            if len(der) > 3 and der[2] == 0x02:
                r_length = der[3]
            
            # extract and verify S
            if len(der) > 5 + r_length and der[4 + r_length] == 0x02:
                s_length = der[5 + r_length]
            
            # display the result on the same line
            print(f"ECDSA size: 0x{total_length:02x}, R : ({hex(r_length)}), S: ({hex(s_length)})")           

    def verify(self, signature, payload, digest, header_size, img_size):
        self._key_info_(signature)
        # strip possible paddings added during sign
        signature=self._build_compress_signature(signature)
        #signature = signature[:signature[1] + 2]
        k = self.key
        if isinstance(self.key, ec.EllipticCurvePrivateKey):
            k = self.key.public_key()
        #use digest instead of payload
        return k.verify(bytes(signature), bytes(digest),ec.ECDSA(utils.Prehashed(SHA256())))

class ECDSA256P1(ECDSA256P1Public):
    """
    Wrapper around an ECDSA private key.
    """

    def __init__(self, key, extension="", priv=""):
        self.key = key
        self.extension=extension
        self.privname=priv
        self.pad_sig = False

    @staticmethod
    def generate():
        pk = ec.generate_private_key(
                ec.SECP256R1(),
                backend=default_backend())
        return ECDSA256P1(pk)

    def _get_private(self):
        return self.key

    def _get_public(self):
        return self.key.public_key()

    def _build_minimal_ecdsa_privkey(self, der):
        '''
        Builds a new DER that only includes the EC private key, removing the
        public key that is added as an "optional" BITSTRING.
        '''
        offset_PUB = 68
        EXCEPTION_TEXT = "Error parsing ecdsa key. Please submit an issue!"
        if der[offset_PUB] != 0xa1:
            raise ECDSAUsageError(EXCEPTION_TEXT)
        len_PUB = der[offset_PUB + 1]
        b = bytearray(der[:-offset_PUB])
        offset_SEQ = 29
        if b[offset_SEQ] != 0x30:
            raise ECDSAUsageError(EXCEPTION_TEXT)
        b[offset_SEQ + 1] -= len_PUB
        offset_OCT_STR = 27
        if b[offset_OCT_STR] != 0x04:
            raise ECDSAUsageError(EXCEPTION_TEXT)
        b[offset_OCT_STR + 1] -= len_PUB
        if b[0] != 0x30 or b[1] != 0x81:
            raise ECDSAUsageError(EXCEPTION_TEXT)
        b[2] -= len_PUB
        return b
    
    def _build_uncompress_signature(self, der):
        #DER format is
        #0 :ASN1 header (0x30)
        #1 :len
        #2 :tag (0x2)
        #3 :len R signature (including zero data)
        #4 :Zero data (0x0)
        #5 :R signature
        #4+ len R signature :tag (0x2)
        #4+ len R signature +1 :len S signature (including zero data)
        #4+ len R signature +2 :Zero data
        #4+ len R signature +3 :S signature
        #len of complete area
        offset_LEN=1
        #offset of length for R signature
        offset_LEN_R=3
        offset_LEN_S=der[offset_LEN_R]+5
        if der[offset_LEN] != 0x46:
           byte_R_to_add =  0x21 - der[offset_LEN_R]
           byte_R_to_get =  der[offset_LEN_R]
           #get bytes up to tag
           b = bytearray(der[0:offset_LEN])
           b += bytes([70, 2, 33])+bytes(byte_R_to_add)
           # zero data must be taken
           b += bytearray(der[offset_LEN_R+1:offset_LEN_R+byte_R_to_get+1])
           byte_S_to_add = 0x21 - der[offset_LEN_S]
           byte_S_to_get = der[offset_LEN_S]
           #add tag and len
           b += bytes([2,33])
           b += bytes(byte_S_to_add)
           b += bytearray(der[offset_LEN_S+1:offset_LEN_S+byte_S_to_get+2])
           return b
        return der

    def get_private_bytes(self, minimal):
        priv = self.key.private_bytes(
                encoding=serialization.Encoding.DER,
                format=serialization.PrivateFormat.PKCS8,
                encryption_algorithm=serialization.NoEncryption())
        if minimal:
            priv = self._build_minimal_ecdsa_privkey(priv)
        return priv

    def export_private(self, path, passwd=None):
        """Write the private key to the given file, protecting it with the optional password."""
        if passwd is None:
            enc = serialization.NoEncryption()
        else:
            enc = serialization.BestAvailableEncryption(passwd)
        pem = self.key.private_bytes(
                encoding=serialization.Encoding.PEM,
                format=serialization.PrivateFormat.PKCS8,
                encryption_algorithm=enc)
        with open(path, 'wb') as f:
            f.write(pem)

    def raw_sign(self, payload):
        """Return the actual signature"""
        return self.key.sign(
                data=payload,
                signature_algorithm=ec.ECDSA(SHA256()))

    def sign(self, payload):
        sig = self.raw_sign(payload)
        if self.pad_sig:
            sig = self._build_uncompress_signature(sig)
        return sig

    def sign_digest(self, digest):
       chosen_hash = SHA256()
       sig = self.key.sign(digest,ec.ECDSA(utils.Prehashed(chosen_hash)))
       return sig
       
class ECDSA256P1_SSL(ECDSA256P1Public):
    """
    Wrapper around an ECDSA private key.
    """

    def __init__(self, key, extension="", priv=""):
        """key should be an instance of EllipticCurvePrivateKey"""
        self.key = key
        self.pad_sig = False
        self.extension=extension
        self.privname=priv
    @staticmethod
    def generate():
        pk = ec.generate_private_key(
                ec.SECP256R1(),
                backend=default_backend())
        return ECDSA256P1_SSL(pk)

    def _get_public(self):
        return self.key.public_key()

    def _build_minimal_ecdsa_privkey(self, der):
        '''
        Builds a new DER that only includes the EC private key, removing the
        public key that is added as an "optional" BITSTRING.
        '''
        offset_PUB = 68
        EXCEPTION_TEXT = "Error parsing ecdsa key. Please submit an issue!"
        if der[offset_PUB] != 0xa1:
            raise ECDSAUsageError(EXCEPTION_TEXT)
        len_PUB = der[offset_PUB + 1]
        b = bytearray(der[:-offset_PUB])
        offset_SEQ = 29
        if b[offset_SEQ] != 0x30:
            raise ECDSAUsageError(EXCEPTION_TEXT)
        b[offset_SEQ + 1] -= len_PUB
        offset_OCT_STR = 27
        if b[offset_OCT_STR] != 0x04:
            raise ECDSAUsageError(EXCEPTION_TEXT)
        b[offset_OCT_STR + 1] -= len_PUB
        if b[0] != 0x30 or b[1] != 0x81:
            raise ECDSAUsageError(EXCEPTION_TEXT)
        b[2] -= len_PUB
        return b

    def get_private_bytes(self, minimal):
        priv = self.key.private_bytes(
                encoding=serialization.Encoding.DER,
                format=serialization.PrivateFormat.TraditionalOpenSSL,
                encryption_algorithm=serialization.NoEncryption())
        if minimal:
            priv = self._build_minimal_ecdsa_privkey(priv)
        return priv

    def export_private(self, path, passwd=None):
        """Write the private key to the given file, protecting it with the optional password."""
        if passwd is None:
            enc = serialization.NoEncryption()
        else:
            enc = serialization.BestAvailableEncryption(passwd)
        pem = self.key.private_bytes(
                encoding=serialization.Encoding.PEM,
                format=serialization.PrivateFormat.TraditionalOpenSSL,
                encryption_algorithm=enc)
        with open(path, 'wb') as f:
            f.write(pem)

    def raw_sign(self, payload):
        """Return the actual signature"""
        return self.key.sign(
                data=payload,
                signature_algorithm=ec.ECDSA(SHA256()))

    def sign(self, payload):
        sig = self.raw_sign(payload)
        if self.pad_sig:
            # To make fixed length, pad with one or two zeros.
            sig += b'\000' * (self.sig_len() - len(sig))
            return sig
        else:
            return sig
