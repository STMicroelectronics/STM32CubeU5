"""
SYM Key management
"""
import os
import sys
from .general import KeyClass

class SYMKEYUsageError(Exception):
    pass
   
class SYMKEYPublic(KeyClass):
    def __init__(self, key, extension="", priv=""):
        self.key = key
        self.extension=extension
        self.privname=priv

    def shortname(self):
        return "sym"

    def name(self):
        return self.privname

    def _unsupported(self, name):
        raise SYMKEYUsageError("Operation {} requires private key".format(name))

    def get_public_bytes(self):
        self._unsupported('get_public_bytes')
        
    def get_private_bytes(self, minimal):
        self._unsupported('get_private_bytes')

    def export_private(self, path, passwd=None):
        self._unsupported('export_private')

    def export_public(self, path):
        self._unsupported("export_public")

    def sig_tlv(self):
        self._unsupported("sig_tlv")

    def sig_len(self):
        self._unsupported("sig_len")

    def verify(self, signature, payload):
        self._unsupported("verify")


class SYMKEY(SYMKEYPublic):
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
        pk = os.urandom(32)
        return SYMKEY(pk)

    def _get_public(self):
        return self.key.public_key()

    def get_private_bytes(self, minimal):
        return self.key

    def export_private(self, path, passwd=None):
        """Write the symtric key to the given file, no protecting  with the optional password."""
        with open(path, 'wb') as f:
            f.write(self.key)
