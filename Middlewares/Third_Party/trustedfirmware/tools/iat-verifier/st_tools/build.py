import sys
import argparse
import os
from struct import pack
def intparse(text):
    """Parse a command line argument as an integer.

    Accepts 0x and other prefixes to allow other bases to be used."""
    return int(text, 0)

def do_boot(args):
    print("boot")
    big_binary=b''
    with open(args.infile, 'rb') as f:
        big_binary=f.read()
        big_binary=big_binary[0x2000:]
    with open(args.outfile, 'wb') as f:
       f.write(big_binary)
def do_ass(args):
    print("boot")
    big_binary=b''
    with open(args.infile, 'rb') as f:
        binary=f.read()
        binary+=(args.input_size-len(binary))*pack("B",0xff)
    with open(args.file, 'rb') as f:
       outbinary=f.read()
       outbinary+=(args.output_size-len(outbinary))*pack("B",0xff)
    with open(args.outfile, 'wb') as f:
       big_binary=binary+outbinary
       f.write(big_binary)

def do_cbor(args):
    print("cbor")
    big_binary=b''
    with open(args.infile, 'rb') as f:
        all_line=f.readlines()
        for line in all_line:

            for i in range(0,len(line)>>1):
                hexavalue=line[2*i:2*i+2];
                try:
                    byte=int(hexavalue,16)
                    big_binary +=pack("B",byte)
                except:
                    endline=True
    with open(args.outfile, 'wb') as f:
       f.write(big_binary)

subcmds = {
        'cbor':do_cbor,
        'boot':do_boot,
        'ass':do_ass,
        }
def args():
    parser = argparse.ArgumentParser()
    subs = parser.add_subparsers(help='subcommand help', dest='subcmd')
    cbor= subs.add_parser('cbor', help='build cbor')
    cbor.add_argument("infile", help="input filename of text file" )
    cbor.add_argument("outfile", help="output filename cbor file" )
    boot = subs.add_parser('boot', help='build boot')
    boot.add_argument("infile", help="input filename of text file" )
    boot.add_argument("outfile", help="output filename cbor file" )
    ass = subs.add_parser('ass', help='build boot')
    ass.add_argument("infile", help="input filename" )
    ass.add_argument("-f", '--file', metavar='filename', required=True)
    ass.add_argument("outfile", help="output filename" )
    ass.add_argument("-i", "--input-size", type=intparse, required=True)
    ass.add_argument("-o", "--output-size", type=intparse, required=True)
    args = parser.parse_args()

    if args.subcmd is None:
        print('Must specify a subcommand')
        sys.exit(1)
    subcmds[args.subcmd](args)
if __name__ == '__main__':
    args()
