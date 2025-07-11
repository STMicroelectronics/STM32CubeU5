#!/usr/bin/env python3
#
# Copyright The Mbed TLS Contributors
# SPDX-License-Identifier: Apache-2.0

"""
This script generates a file called identifiers that contains all Mbed TLS
identifiers found on internal headers. This is the equivalent of what was
previously `list-identifiers.sh --internal`, and is useful for generating an
exclusion file list for ABI/API checking, since we do not promise compatibility
for them.

It uses the CodeParser class from framework/scripts/check_names.py to perform
the parsing.

The script returns 0 on success, 1 if there is a script error.
Must be run from Mbed TLS root.
"""

import argparse
import logging
import scripts_path # pylint: disable=unused-import
from check_names import CodeParser

def main():
    parser = argparse.ArgumentParser(
        formatter_class=argparse.RawDescriptionHelpFormatter,
        description=(
            "This script writes a list of parsed identifiers in internal "
            "headers to \"identifiers\". This is useful for generating a list "
            "of names to exclude from API/ABI compatibility checking. "))

    parser.parse_args()

    name_check = CodeParser(logging.getLogger())
    result = name_check.parse_identifiers([
        "include/mbedtls/*_internal.h",
        "library/*.h"
    ])[0]
    result.sort(key=lambda x: x.name)

    identifiers = ["{}\n".format(match.name) for match in result]
    with open("identifiers", "w", encoding="utf-8") as f:
        f.writelines(identifiers)

if __name__ == "__main__":
    main()
