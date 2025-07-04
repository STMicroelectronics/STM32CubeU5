"""
AES GCM key management
"""
import binascii
from cryptography.hazmat.backends import default_backend

from cryptography.hazmat.primitives.ciphers import (
    Cipher, algorithms, modes
)

from .general import KeyClass

import hashlib
import os
import sys
from .general import KeyClass
class AESGCMUsageError(Exception):
    pass
    
class AESGCMPublic(KeyClass):
    def __init__(self, key, extension="", priv=""):
        self.key = key
        self.extension=extension
        self.privname=priv
    def key_size(self):
        return self.key.key_size

    def shortname(self):
        return "aesgcm"

    def ext(self):
        return self.extension
 
    def name(self):
        return self.privname

    def _unsupported(self, name):
        raise AESGCMusageError("Operation {} requires private key".format(name))

    def _get_public(self):
        self._unsupported('_get_public')

    def get_public_bytes(self):
        return self.key

    def get_private_bytes(self, minimal):
        self._unsupported('get_private_bytes')

    def export_private(self, path, passwd=None):
        self._unsupported('export_private')

    def export_public(self, path):
       self._unsupported('export_public')

    def sig_type(self):
        return "AESGCM_SHA256"

    def sig_tlv(self):
        return "AUTH_TAG"

    def sig_len(self):
        return 16

    def verify(self, signature, payload, digest, header_size, img_size):
        #digest is sha256
        #strip possible paddings added during sign
        #signature = signature[:signature[1] + 2]
        nonce=digest[16:]
        cipher = Cipher(algorithms.AES(self.key), modes.CTR(nonce),
                            backend=default_backend())
        decryptor = cipher.decryptor()
        #Decrypt 
        img = payload[:header_size]+decryptor.update(payload[header_size:header_size+img_size]) + decryptor.finalize()
        img = img +  payload[header_size+img_size:]
        #compute sha256 
        sha = hashlib.sha256()
        sha.update(img)
        hash = sha.digest()
        if hash != digest:
            print("hash decrypted does not match")
        #Construct a Cipher object, with the key, iv, and additionally the
        #GCM tag used for authenticating the message.
   
        # iv is sha256 1st 12 bytes 
        decryptor = Cipher(algorithms.AES(self.key),modes.GCM(digest[:12], signature)).decryptor()
        # We put associated_data back in or the tag will fail to verify
        # when we finalize the decryptor. associated data is sha256
        decryptor.authenticate_additional_data(digest[12:32])
        # Decryption gets us the authenticated plaintext.
        # If the tag does not match an InvalidTag exception will be raised.
        #return decryptor.finalize()
        # with sign_disgest, payload is sha256 (32 bytes) 
        if len(digest) != 32:
            self._unsupported("sign payload different 32")
        # IV is the 1st 12 bytes of payload
        iv = digest[:12]

        # Construct an AES-GCM Cipher object with the given key and IV from payload.
        encryptor = Cipher(algorithms.AES(self.key), modes.GCM(iv)).encryptor()

        # associated_data will be authenticated but not encrypted,
        # it must also be passed in on decryption.
        encryptor.authenticate_additional_data(digest[12:32])

        # Encrypt nothing and get the associated ciphertext (not used)
        # GCM does not require padding.
        ciphertext = encryptor.finalize()
        if signature !=encryptor.tag:
            print("signature incorrect")
        #If signature is not correct an error is triggered
        try:
            decryptor.finalize()
        except:
            print("signature incorrect")
 

class AESGCM(AESGCMPublic):
    """
    Wrapper around symetric key.
    """
    def __init__(self, key, extension="", priv=""):
        """key should be a random number """
        self.key = key
        self.extension=extension
        self.privname=priv

    @staticmethod
    def generate():
        pk = os.urandom(16)
        return AESGCM(pk)

    def _get_public(self):
        return self.key.public_key()

    def get_private_bytes(self, minimal=0):
        return self.key

    def export_private(self, path, passwd=None):
        """Write the symtric key to the given file, no protecting  with the optional password."""
        with open(path, 'wb') as f:
            f.write(self.key)
    def sign_digest(self, payload):
        # with sign_disgest, payload is sha256 (32 bytes) 
        if len(payload) != 32:
            self._unsupported("sign payload different 32")
        # IV is the 1st 12 bytes of payload
        iv = payload[:12]

        # Construct an AES-GCM Cipher object with the given key and IV from payload.
        encryptor = Cipher(algorithms.AES(self.key), modes.GCM(iv)).encryptor()

        # associated_data will be authenticated but not encrypted,
        # it must also be passed in on decryption.
        encryptor.authenticate_additional_data(payload[12:32])

        # Encrypt nothing and get the associated ciphertext (not used)
        # GCM does not require padding.
        ciphertext = encryptor.finalize()
        #return tag
        return encryptor.tag    
