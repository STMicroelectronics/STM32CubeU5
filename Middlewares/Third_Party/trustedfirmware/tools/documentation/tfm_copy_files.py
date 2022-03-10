# -----------------------------------------------------------------------------
# Copyright (c) 2020, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
# -----------------------------------------------------------------------------

# Interface module for pre-processing the documentation content
# before sphynx-build is called
#
# It collects files from multiple sources in a intermediate location
# before calling sphinx-build, thus overriding the limitation
# of having the documentation share a common root directory.
#
# It can be triggered by simply importing the module
import os
from shutil import copy2, copytree, rmtree, move
from glob import glob
from tfm_cmake_defaults import *
from subprocess import call


def tfm_copy_files():
    doc_files = []

    # Recursively list all files with extensions and add them
    for _path in document_scan_dirs:
        _path = os.path.abspath(os.path.join(tfm_def_root_dir, _path))
        for ext in document_scan_ext:
                doc_files.extend([f for f in glob(os.path.join(_path,
                                                               "**/*%s" % ext),
                                  recursive=True)])

    # Add the extra files
    for _doc_file in documents_extra:
        _doc_file = os.path.abspath(os.path.join(tfm_def_root_dir, _doc_file))
        if os.path.isfile(_doc_file):
            doc_files.append(_doc_file)

    # Clean up all files in target dir except conf.py and tfm_env.py
    files = [f for f in
             glob("*", recursive=False) if f not in ["conf.py",
                                                     tfm_def_conf_out_file,
                                                     os.path.basename(
                                                        tfm_def_doxy_output_dir),
                                                     "tfm_env.py"]]
    for f in files:
        if os.path.isfile(f):
            os.remove(f)
        elif os.path.isdir(f):
            rmtree(f)

    # Copy the documentation folder as is
    copytree(tfm_def_doc_root, tfm_def_copy_doc_root)

    # Move the index to the intermediate build directory
    # docs/index.rst --> ./index.rst
    move(os.path.join(tfm_def_copy_doc_root, "index.rst"), tfm_def_copy_dir)

    for df in list(doc_files):
        # Set the target filename to be cwd + relative to root path of origin
        target_f = os.path.relpath(df, tfm_def_root_dir)
        target_f = os.path.join(tfm_def_copy_dir, target_f)
        # Create path for file (nested) without exception if exists
        os.makedirs(os.path.dirname(target_f), exist_ok=True)
        # Copy the file to new location
        copy2(df, target_f)


# Build Doxygen Documnetation
if tfm_def_build_doxygen:
    # if conf file is not provided by cmake
    if tfm_def_render_cmake:
        render_cmake_file(cmake_env,
                          tfm_def_doxygen_in_file,
                          tfm_def_doxygen_out_file)
    # Call doxygen to generate the documentation
    doxygen_bin = find_package("doxygen")
    call([doxygen_bin, tfm_def_doxygen_out_file])

# Only act if requested by defaults
if tfm_def_copy_files:
    tfm_copy_files()

if tfm_def_render_cmake:
    # Render the conf_py file
    render_cmake_file(cmake_env, tfm_def_conf_in_file, tfm_def_conf_out_file)
