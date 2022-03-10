############################
Initial Attestation Verifier
############################
This is a set of utility scripts for working with PSA Initial Attestation
Token, the structure of which is described here:

   https://tools.ietf.org/html/draft-tschofenig-rats-psa-token-05

The following utilities are provided:

check_iat
   Verifies the structure, and optionally the signature, of a token.

compile_token
   Creates a (optionally, signed) token from a YAML descriptions of the claims.

decompile_token
   Generates a YAML descriptions of the claims contained within a token. (Note:
   this description can then be compiled back into a token using compile_token.)


************
Installation
************
You can install the script using pip:

.. code:: bash

   # Inside the directory containg this README
   pip3 install .

This should automatically install all the required dependencies. Please
see ``setup.py`` for the list of said dependencies.

*****
Usage
*****

.. note::
   You can use ``-h`` flag with any of the scripts to see their usage help.

check_iat
---------

After installing, you should have ``check_iat`` script in your ``PATH``. The
script expects a single parameter – a path to the signed IAT in COSE
format.

You can find an example in the “sample” directory.

The script will extract the COSE payload and make sure that it is a
valid IAT (i.e. all mandatory fields are present, and all known
fields have correct size/type):

.. code:: bash

   $ check_iat sample/cbor/iat.cbor
   Token format OK

If you want the script to verify the signature, you need to specify the
file containing the signing key in PEM format using -k option. The key
used to sign sample/iat.cbor is inside sample/key.pem.

::

   $ check_iat -k sample/key.pem  sample/cbor/iat.cbor
   Signature OK
   Token format OK

You can add a -p flag to the invocation in order to have the script
print out the decoded IAT in JSON format. It should look something like
this:

.. code:: json

   {
       "INSTANCE_ID": "\u0001\u0007\u0006\u0005\u0004\u0003\u0002\u0001\u0000\u000f\u000e\r\f\u000b\n\t\b\u0017\u0016\u0015\u0014\u0013\u0012\u0011\u0010\u001f\u001e\u001d\u001c\u001b\u001a\u0019\u0018",
       "IMPLEMENTATION_ID": "\u0007\u0006\u0005\u0004\u0003\u0002\u0001\u0000\u000f\u000e\r\f\u000b\n\t\b\u0017\u0016\u0015\u0014\u0013\u0012\u0011\u0010\u001f\u001e\u001d\u001c\u001b\u001a\u0019\u0018",
       "CHALLEGE": "\u0007\u0006\u0005\u0004\u0003\u0002\u0001\u0000\u000f\u000e\r\f\u000b\n\t\b\u0017\u0016\u0015\u0014\u0013\u0012\u0011\u0010\u001f\u001e\u001d\u001c\u001b\u001a\u0019\u0018",
       "CLIENT_ID": 2,
       "SECURITY_LIFECYCLE": 2,
       "VERSION": 1,
           "BOOT_SEED": "\u0007\u0006\u0005\u0004\u0003\u0002\u0001\u0000\u000f\u000e\r\f\u000b\n\t\b\u0017\u0016\u0015\u0014\u0013\u0012\u0011\u0010\u001f\u001e\u001d\u001c\u001b\u001a\u0019\u0018"
       "SUBMOD": [
       {
           "SUBMOD_NAME": "BL",
           "SIGNER_ID": "\u0007\u0006\u0005\u0004\u0003\u0002\u0001\u0000\u000f\u000e\r\f\u000b\n\t\b\u0017\u0016\u0015\u0014\u0013\u0012\u0011\u0010\u001f\u001e\u001d\u001c\u001b\u001a\u0019\u0018",
           "SUBMOD_VERSION": "3.4.2",
           "MEASUREMENT": "\u0007\u0006\u0005\u0004\u0003\u0002\u0001\u0000\u000f\u000e\r\f\u000b\n\t\b\u0017\u0016\u0015\u0014\u0013\u0012\u0011\u0010\u001f\u001e\u001d\u001c\u001b\u001a\u0019\u0018"
       },
       {
           "SUBMOD_NAME": "M1",
           "SIGNER_ID": "\u0007\u0006\u0005\u0004\u0003\u0002\u0001\u0000\u000f\u000e\r\f\u000b\n\t\b\u0017\u0016\u0015\u0014\u0013\u0012\u0011\u0010\u001f\u001e\u001d\u001c\u001b\u001a\u0019\u0018",
           "SUBMOD_VERSION": "3.4.2",
           "MEASUREMENT": "\u0007\u0006\u0005\u0004\u0003\u0002\u0001\u0000\u000f\u000e\r\f\u000b\n\t\b\u0017\u0016\u0015\u0014\u0013\u0012\u0011\u0010\u001f\u001e\u001d\u001c\u001b\u001a\u0019\u0018"
       },
       {
           "SUBMOD_NAME": "M2",
           "SIGNER_ID": "\u0007\u0006\u0005\u0004\u0003\u0002\u0001\u0000\u000f\u000e\r\f\u000b\n\t\b\u0017\u0016\u0015\u0014\u0013\u0012\u0011\u0010\u001f\u001e\u001d\u001c\u001b\u001a\u0019\u0018",
           "SUBMOD_VERSION": "3.4.2",
           "MEASUREMENT": "\u0007\u0006\u0005\u0004\u0003\u0002\u0001\u0000\u000f\u000e\r\f\u000b\n\t\b\u0017\u0016\u0015\u0014\u0013\u0012\u0011\u0010\u001f\u001e\u001d\u001c\u001b\u001a\u0019\u0018"
       },
       {
           "SUBMOD_NAME": "M3",
           "SIGNER_ID": "\u0007\u0006\u0005\u0004\u0003\u0002\u0001\u0000\u000f\u000e\r\f\u000b\n\t\b\u0017\u0016\u0015\u0014\u0013\u0012\u0011\u0010\u001f\u001e\u001d\u001c\u001b\u001a\u0019\u0018",
           "SUBMOD_VERSION": "3.4.2",
           "MEASUREMENT": "\u0007\u0006\u0005\u0004\u0003\u0002\u0001\u0000\u000f\u000e\r\f\u000b\n\t\b\u0017\u0016\u0015\u0014\u0013\u0012\u0011\u0010\u001f\u001e\u001d\u001c\u001b\u001a\u0019\u0018"
       }
       ]
   }

compile_token
-------------

You can use this script to compile a YAML claims description into a COSE-wrapped
CBOR token:

.. code:: bash

   $ compile_token -k sample/key.pem sample/yaml/iat.yaml > sample_token.cbor

*No validation* is performed as part of this, so there is no guarantee that a
valid IAT will be produced.

You can omit the ``-k`` option, in which case, the resulting token will not be
signed, however it will still be wrapped in COSE "envelope". If you would like
to produce a pure CBOR encoding of the claims without a COSE wrapper, you can
use ``-r`` flag.


decompile_token
---------------

Decompile an IAT (or any COSE-wrapped CBOR object -- *no validation* is performed
as part of this) into a YAML description of its claims.


.. code:: bash

   $decompile_token  sample/cbor/iat.cbor
   boot_seed: !!binary |
     BwYFBAMCAQAPDg0MCwoJCBcWFRQTEhEQHx4dHBsaGRg=
   challenge: !!binary |
     BwYFBAMCAQAPDg0MCwoJCBcWFRQTEhEQHx4dHBsaGRg=
   client_id: 2
   implementation_id: !!binary |
     BwYFBAMCAQAPDg0MCwoJCBcWFRQTEhEQHx4dHBsaGRg=
   instance_id: !!binary |
     AQcGBQQDAgEADw4NDAsKCQgXFhUUExIREB8eHRwbGhkY
   profile_id: http://example.com
   security_lifecycle: SL_SECURED
   sw_components:
   - measurement_description: TF-M_SHA256MemPreXIP
     measurement_value: !!binary |
       BwYFBAMCAQAPDg0MCwoJCBcWFRQTEhEQHx4dHBsaGRg=
     signer_id: !!binary |
       BwYFBAMCAQAPDg0MCwoJCBcWFRQTEhEQHx4dHBsaGRg=
     sw_component_type: BL
     sw_component_version: 3.4.2
   - measurement_value: !!binary |
       BwYFBAMCAQAPDg0MCwoJCBcWFRQTEhEQHx4dHBsaGRg=
     signer_id: !!binary |
       BwYFBAMCAQAPDg0MCwoJCBcWFRQTEhEQHx4dHBsaGRg=
     sw_component_type: M1
     sw_component_version: 1.2
   - measurement_value: !!binary |
       BwYFBAMCAQAPDg0MCwoJCBcWFRQTEhEQHx4dHBsaGRg=
     signer_id: !!binary |
       BwYFBAMCAQAPDg0MCwoJCBcWFRQTEhEQHx4dHBsaGRg=
     sw_component_type: M2
     sw_component_version: 1.2.3
   - measurement_value: !!binary |
       BwYFBAMCAQAPDg0MCwoJCBcWFRQTEhEQHx4dHBsaGRg=
     signer_id: !!binary |
       BwYFBAMCAQAPDg0MCwoJCBcWFRQTEhEQHx4dHBsaGRg=
     sw_component_type: M3
     sw_component_version: 1

This description can then be compiled back into CBOR using ``compile_token``.


***********
Mac0Message
***********

By default, the expectation is that the message will be wrapped using
Sign1Message  COSE structure, however, the alternative Mac0Message structure
that uses HMAC with SHA256 algorithm rather than a signature is supported via
the ``-m mac`` flag:

::

    $ check_iat -m mac -k sample/hmac.key sample/iat-hmac.cbor
    Signature OK
    Token format OK

*******
Testing
*******
Tests can be run using ``nose2``:

.. code:: bash

   pip install nose2

Then run by executing ``nose2`` in the root directory.


*******************
Development Scripts
*******************
The following utility scripts are contained within ``dev_scripts``
subdirectory and were utilized in development of this tool. They are not
need to use the iat-verifier script, and can generally be ignored.

.. code:: bash

   ./dev_scripts/generate-key.py OUTFILE

Generate an ECDSA (NIST256p curve) signing key and write it in PEM
format to the specified file.

.. code:: bash

   ./dev_scripts/generate-sample-iat.py KEYFILE OUTFILE

Generate a sample token, signing it with the specified key, and writing
the output to the specified file.

.. note::
   This script is deprecated -- use ``compile_token`` (see above) instead.

--------------

*Copyright (c) 2019-2020, Arm Limited. All rights reserved.*
