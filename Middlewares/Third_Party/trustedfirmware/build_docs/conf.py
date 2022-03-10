# -----------------------------------------------------------------------------
# Copyright (c) 2020, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
# -----------------------------------------------------------------------------
#
# Configuration file override for the Sphinx documentation builder.
#
# This file is used when Sphinx build is  invoked directly at this level.
# It will trigger a copy-files operation and render a new configuration
# using either auto-detected or cmake provided  parameters.
import os
import sys

# Attempt to find the tools directory by recursing up to five levels of parents
root_path = os.path.dirname(os.path.abspath(__file__))

for i in range(5):
    root_path = os.path.dirname(root_path)
    doc_path = os.path.join(root_path, "tools", "documentation")
    if os.path.isdir(doc_path):
        sys.path.insert(0, os.path.abspath(doc_path))
sys.path.append("./")

# Trigger the copy operation logic
import tfm_copy_files

# Import the rendered configuration into global scope
from tfm_cmake_defaults import *

from conf_rendered import *
