#! /usr/bin/env python3
#
# Copyright 2017 Linaro Limited
# Copyright (c) 2018-2019, Arm Limited.
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

from __future__ import print_function
import os
import re
import argparse

import sys
import macro_parser
import fileinput


def do_flash(args):
    image_value_re = re.compile(r"^\s*"+args.macro+"\s*=\s*(.*)")
    value = macro_parser.evaluate_macro(args.layout, image_value_re, 0, 1,
                                        True)
    if args.setting == 1:
        begin_line="set "+args.begin
    else:
        begin_line=args.begin

    for line in fileinput.input(args.infile, inplace=True):
        if line.startswith(begin_line):
            if args.division:
                value = int(value/int(args.division))
            if args.phexa == 0:
                line = begin_line+"="+str(value)+"\n"
            else:
                line = begin_line+"="+hex(value)+"\n"
        sys.stdout.write(line)

subcmds = {
        'flash': do_flash, }


def intparse(text):
    """Parse a command line argument as an integer.

    Accepts 0x and other prefixes to allow other bases to be used."""
    return int(text, 0)

def args():
    parser = argparse.ArgumentParser()
    subs = parser.add_subparsers(help='subcommand help', dest='subcmd')


    flash = subs.add_parser('flash', help='modify flash script')
    flash.add_argument("infile")
    flash.add_argument('-l', '--layout', required=True,
                      help='Location of the file that contains preprocessed macros')
    flash.add_argument('-m', '--macro', required =True,
                      help='macro symbol string to grep in preprocessed file')
    flash.add_argument('-b', '--begin', required=True,
                      help='begin of line to replace ')
    flash.add_argument('-s', '--setting',type=intparse,required=False,default=0,
                      help='search for window batch set variable')
    flash.add_argument('-d', '--division',
                       required=False,type=intparse,default=0,
                      help='search for window batch set variable')
    flash.add_argument('-p', '--phexa',
                       required=False,type=intparse,default=1,
                      help='print value in hexa')

    args = parser.parse_args()
    if args.subcmd is None:
        print('Must specify a subcommand', file=sys.stderr)
        sys.exit(1)

    subcmds[args.subcmd](args)

if __name__ == '__main__':
    args()
