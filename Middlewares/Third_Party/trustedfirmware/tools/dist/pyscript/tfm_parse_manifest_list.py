#-------------------------------------------------------------------------------
# Copyright (c) 2018-2021, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

import os
import io
import sys
import argparse
from jinja2 import Environment, BaseLoader, select_autoescape, TemplateNotFound

try:
    import yaml
except ImportError as e:
    print (str(e) + " To install it, type:")
    print ("pip install PyYAML")
    exit(1)

donotedit_warning = \
                    "/*********** " + \
                    "WARNING: This is an auto-generated file. Do not edit!" + \
                    " ***********/"

OUT_DIR = None # The root directory that files are generated to

class TemplateLoader(BaseLoader):
    """
    Template loader class.

    An instance of this class is passed to the template engine. It is
    responsible for reading the template file
    """
    def __init__(self):
        pass

    def get_source(self, environment, template):
        """
        This function reads the template files.
        For detailed documentation see:
        http://jinja.pocoo.org/docs/2.10/api/#jinja2.BaseLoader.get_source

        Please note that this function always return 'false' as 'uptodate'
        value, so the output file will always be generated.
        """
        
        if not os.path.isfile(template):
            raise TemplateNotFound(template)
        with open(template) as f:
            source = f.read()
        return source, template, False

def process_manifest(manifest_list_files):
    """
    Parse the input manifest, generate the data base for genereated files
    and generate manifest header files.

    Parameters
    ----------
    manifest_list_files:
        The manifest lists to parse.

    Returns
    -------
    The partition data base.
    """

    partition_db = []
    manifest_list = []

    for f in manifest_list_files:
        with open(f) as manifest_list_yaml_file:
            manifest_dic = yaml.safe_load(manifest_list_yaml_file)
            manifest_list.extend(manifest_dic["manifest_list"])

    manifesttemplate = ENV.get_template("secure_fw/partitions/manifestfilename.template")
    memorytemplate = ENV.get_template("secure_fw/partitions/partition_intermedia.template")

    print("Start to generate PSA manifests:")
    for manifest_item in manifest_list:
        # Replace environment variables in the manifest path
        manifest_path = os.path.expandvars(manifest_item['manifest'])
        file = open(manifest_path)
        manifest = yaml.safe_load(file)

        utilities = {}
        utilities['donotedit_warning']=donotedit_warning

        context = {}
        context['manifest'] = manifest
        context['attr'] = manifest_item
        context['utilities'] = utilities

        manifest_dir, manifest_name = os.path.split(manifest_path)
        outfile_name = manifest_name.replace('yaml', 'h').replace('json', 'h')
        context['file_name'] = outfile_name.replace('.h', '')
        outfile_name = os.path.join(manifest_dir, "psa_manifest", outfile_name).replace('\\', '/')
        intermediafile_name = os.path.join(manifest_dir, "auto_generated", 'intermedia_' + context['file_name'] + '.c').replace('\\', '/')

        """
        Remove the `source_path` portion of the filepaths, so that it can be
        interpreted as a relative path from the OUT_DIR.
        """
        if 'source_path' in manifest_item:
            # Replace environment variables in the source path
            source_path = os.path.expandvars(manifest_item['source_path'])
            outfile_name = os.path.relpath(outfile_name, start = source_path)
            intermediafile_name = os.path.relpath(intermediafile_name, start = source_path)

        partition_db.append({"manifest": manifest, "attr": manifest_item, "header_file": outfile_name})

        if OUT_DIR is not None:
            outfile_name = os.path.join(OUT_DIR, outfile_name)
            intermediafile_name = os.path.join(OUT_DIR, intermediafile_name)

        outfile_path = os.path.dirname(outfile_name)
        if not os.path.exists(outfile_path):
            os.makedirs(outfile_path)

        print ("Generating " + outfile_name)

        outfile = io.open(outfile_name, "w", newline=None)
        outfile.write(manifesttemplate.render(context))
        outfile.close()

        intermediafile_path = os.path.dirname(intermediafile_name)
        if not os.path.exists(intermediafile_path):
            os.makedirs(intermediafile_path)

        print ("Generating " + intermediafile_name)

        memoutfile = io.open(intermediafile_name, "w", newline=None)
        memoutfile.write(memorytemplate.render(context))
        memoutfile.close()

    return partition_db

def gen_files(context, gen_file_lists):
    """
    Generate files according to the gen_file_list

    Parameters
    ----------
    gen_file_lists:
        The lists of files to generate
    """
    file_list = []

    for f in gen_file_lists:
        with open(f) as file_list_yaml_file:
            file_list_yaml = yaml.safe_load(file_list_yaml_file)
            file_list.extend(file_list_yaml["file_list"])

    print("Start to generate file from the generated list:")
    for file in file_list:
        # Replace environment variables in the output filepath
        outfile_name = os.path.expandvars(file["output"])
        # Replace environment variables in the template filepath
        templatefile_name = os.path.expandvars(file["template"])

        if OUT_DIR is not None:
            outfile_name = os.path.join(OUT_DIR, outfile_name)

        print ("Generating " + outfile_name)

        outfile_path = os.path.dirname(outfile_name)
        if not os.path.exists(outfile_path):
            os.makedirs(outfile_path)

        template = ENV.get_template(templatefile_name)

        outfile = io.open(outfile_name, "w", newline=None)
        outfile.write(template.render(context))
        outfile.close()

    print ("Generation of files done")

def process_stateless_services(partitions, static_handle_max_num):
    """
    This function collects all stateless services together, and allocates
    stateless handle for them.
    If the stateless handle is set to a valid value in yaml file, it is used as
    the index directly, if the stateless handle is set as "auto" or not set,
    framework will allocate a valid index for the service.
    After that, framework puts each service into a stateless service list at
    position of its "index". Other elements in list are left None.
    """
    stateless_services = []

    # Collect all stateless services first.
    for partition in partitions:
        # Skip the FF-M 1.0 partitions
        if partition['manifest']['psa_framework_version'] < 1.1:
            continue
        # Skip the Non-IPC partitions
        if partition['attr']['tfm_partition_ipc'] is not True:
            continue
        for service in partition['manifest']['services']:
            if 'connection_based' not in service:
                raise Exception("'connection_based' is mandatory in FF-M 1.1 service!")
            if service['connection_based'] is False:
                stateless_services.append(service)

    if len(stateless_services) == 0:
        return []

    if len(stateless_services) > static_handle_max_num:
        raise Exception("Stateless service numbers range exceed.")

    """
    Allocate an empty stateless service list to store services and find the
    service easily by handle.
    Use service stateless handle values as indexes. Put service in the list
    at index "handle - 1", since handle value starts from 1 and list index
    starts from 0.
    """
    reordered_stateless_list = [None] * static_handle_max_num

    # Fill in services with specified stateless handle, index is "handle - 1".
    for service in stateless_services:
        if service['stateless_handle'] == "auto":
            continue
        try:
            if reordered_stateless_list[service['stateless_handle']-1] is not None:
                raise Exception("Duplicated stateless service handle.")
            reordered_stateless_list[service['stateless_handle']-1] = service
        except IndexError:
            raise Exception("Stateless service index out of range.")
        # Remove recorded node from the existing list
        stateless_services.remove(service)

    # Auto-allocate stateless handle
    for i in range(0, static_handle_max_num):
        if reordered_stateless_list[i] == None and len(stateless_services) > 0:
            service = stateless_services.pop(0)
            service['stateless_handle'] = i + 1
            reordered_stateless_list[i] = service
        """
        Encode stateless flag and version into stateless handle
        bit 30: stateless handle indicator
        bit 15-8: stateless service version
        bit 7-0: stateless handle index
        """
        if reordered_stateless_list[i] != None:
            stateless_handle_value = reordered_stateless_list[i]['stateless_handle']
            stateless_flag = 1 << 30
            stateless_handle_value |= stateless_flag
            stateless_version = (reordered_stateless_list[i]['version'] & 0xFF) << 8
            stateless_handle_value |= stateless_version
            reordered_stateless_list[i]['stateless_handle'] = '0x%08x' % stateless_handle_value

    return reordered_stateless_list

def parse_args():
    parser = argparse.ArgumentParser(description='Parse secure partition manifest list and generate files listed by the file list',
                                     epilog='Note that environment variables in template files will be replaced with their values')

    parser.add_argument('-o', '--outdir'
                        , dest='outdir'
                        , required=False
                        , default=None
                        , metavar='out_dir'
                        , help='The root directory for generated files, the default is TF-M root folder.')

    parser.add_argument('-m', '--manifest'
                        , nargs='+'
                        , dest='manifest_args'
                        , required=True
                        , metavar='manifest'
                        , help='A set of secure partition manifest lists to parse')

    parser.add_argument('-f', '--file-list'
                        , nargs='+'
                        , dest='gen_file_args'
                        , required=True
                        , metavar='file-list'
                        , help='These files descripe the file list to generate')

    args = parser.parse_args()
    manifest_args = args.manifest_args
    gen_file_args = args.gen_file_args

    return args

ENV = Environment(
        loader = TemplateLoader(),
        autoescape = select_autoescape(['html', 'xml']),
        lstrip_blocks = True,
        trim_blocks = True,
        keep_trailing_newline = True
    )

def main():
    """
    The entry point of the script.

    Generates the output files based on the templates and the manifests.
    """

    global OUT_DIR

    args = parse_args()

    manifest_args = args.manifest_args
    gen_file_args = args.gen_file_args

    OUT_DIR = args.outdir

    manifest_list = [os.path.abspath(x) for x in args.manifest_args]
    gen_file_list = [os.path.abspath(x) for x in args.gen_file_args]

    # Arguments could be relative path.
    # Convert to absolute path as we are going to change diretory later
    if OUT_DIR is not None:
        OUT_DIR = os.path.abspath(OUT_DIR)

    """
    Relative path to TF-M root folder is supported in the manifests
    and default value of manifest list and generated file list are relative to TF-M root folder as well,
    so first change directory to TF-M root folder.
    By doing this, the script can be executed anywhere
    The script is located in <TF-M root folder>/tools, so sys.path[0]<location of the script>/.. is TF-M root folder.
    """
    print(os.path.dirname(sys.argv[0]))
    os.chdir(os.path.join( os.path.dirname(sys.argv[0]), "..","..",".."))

    partition_db = process_manifest(manifest_list)

    utilities = {}
    context = {}

    utilities['donotedit_warning'] = donotedit_warning

    context['partitions'] = partition_db
    context['utilities'] = utilities
    context['stateless_services'] = process_stateless_services(partition_db, 32)

    gen_files(context, gen_file_list)

if __name__ == "__main__":
    main()
