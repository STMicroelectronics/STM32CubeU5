# Copyright 2017 Linaro Limited
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

"""
Cryptographic key management for imgtool.
"""
import sys
import os
from cryptography.hazmat.backends import default_backend
from cryptography.hazmat.primitives import serialization
from cryptography.hazmat.primitives.asymmetric.rsa import (
    RSAPrivateKey, RSAPublicKey)
from cryptography.hazmat.primitives.asymmetric.ec import (
    EllipticCurvePrivateKey, EllipticCurvePublicKey)
from cryptography.hazmat.primitives.asymmetric.ed25519 import (
    Ed25519PrivateKey, Ed25519PublicKey)
from cryptography.hazmat.primitives.asymmetric.x25519 import (
    X25519PrivateKey, X25519PublicKey)

from .rsa import RSA, RSAPublic, RSAUsageError, RSA_KEY_SIZES
from .ecdsa import ECDSA256P1, ECDSA256P1Public, ECDSAUsageError, ECDSA256P1_SSL
from .ed25519 import Ed25519, Ed25519Public, Ed25519UsageError
from .x25519 import X25519, X25519Public, X25519UsageError
from .symkey import SYMKEY, SYMKEYPublic, SYMKEYUsageError


class PasswordRequired(Exception):
    """Raised to indicate that the key is password protected, but a
    password was not specified."""
    pass


def load(path, passwd=None):
    """Try loading a key from the given path.  Returns None if the password wasn't specified."""
    with open(path, 'rb') as f:
        raw_pem = f.read()
    filename = os.path.basename(path)
    (file, ext) = os.path.splitext(filename)
    if ext == ".bin":
        pk = raw_pem
    else :
        try:
            pk = serialization.load_pem_private_key(
                    raw_pem,
                    password=passwd,
                    backend=default_backend())
        # Unfortunately, the crypto library raises unhelpful exceptions,
        # so we have to look at the text.
        except TypeError as e:
            msg = str(e)
            if "private key is encrypted" in msg:
                return None
            raise e
        except ValueError:
            # This seems to happen if the key is a public key, let's try
            # loading it as a public key.
            pk = serialization.load_pem_public_key(
                    raw_pem,
                    backend=default_backend())

    if file[len(file)-2:len(file)]=="_1":
        extension="_1"
    else:
        extension=""
    if ext == ".bin":
        priv_name=file
    else:
        priv_name="enc_ec256"
    if "initial_attestation" in file:
        priv_name="initial_attestation"
    if "enc-rsa" in file:
        priv_name="enc_rsa"
    if ext == ".bin":
         return SYMKEY(pk,extension,priv_name)
    if isinstance(pk, RSAPrivateKey):
        if pk.key_size not in RSA_KEY_SIZES:
            raise Exception("Unsupported RSA key size: " + pk.key_size)
        return RSA(pk,extension,priv_name)
    elif isinstance(pk, RSAPublicKey):
        if pk.key_size not in RSA_KEY_SIZES:
            raise Exception("Unsupported RSA key size: " + pk.key_size)
        return RSAPublic(pk,extension,priv_name)
    elif isinstance(pk, EllipticCurvePrivateKey):
        if pk.curve.name != 'secp256r1':
            raise Exception("Unsupported EC curve: " + pk.curve.name)
        if pk.key_size != 256:
            raise Exception("Unsupported EC size: " + pk.key_size)
        return ECDSA256P1(pk,extension,priv_name)
    elif isinstance(pk, EllipticCurvePublicKey):
        if pk.curve.name != 'secp256r1':
            raise Exception("Unsupported EC curve: " + pk.curve.name)
        if pk.key_size != 256:
            raise Exception("Unsupported EC size: " + pk.key_size)
        return ECDSA256P1Public(pk,extension,priv_name)
    elif isinstance(pk, Ed25519PrivateKey):
        return Ed25519(pk)
    elif isinstance(pk, Ed25519PublicKey):
        return Ed25519Public(pk)
    elif isinstance(pk, X25519PrivateKey):
        return X25519(pk)
    elif isinstance(pk, X25519PublicKey):
        return X25519Public(pk)
    else:
        raise Exception("Unknown key type: " + str(type(pk)))
