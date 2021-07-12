#-------------------------------------------------------------------------------
# Copyright (c) 2017, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

@echo off
PATH=%PATH%;C:\cygwin64\bin
bash.exe %~dp0/log.sh %*
