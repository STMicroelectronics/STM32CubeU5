#
# ****************************************************************************
# @file    main.py
# @author  MCD Application Team
# @brief   Main application file.
# ****************************************************************************
# @attention
#
# Copyright (c) 2023 STMicroelectronics.
# All rights reserved.
#
# This software is licensed under terms that can be found in the LICENSE file
# in the root directory of this software component.
# If no LICENSE file comes with this software, it is provided AS-IS.
#
# ****************************************************************************

import sys
import click
import os
import array
from struct import pack
from AppliCfg import ACutils
from AppliCfg import xml_parser

SOURCE_VERSION = "1.1.3"

MIN_PYTHON_VERSION = (3, 10)
AppliCfgLog=ACutils.LOG(False,True)

class BasedIntAllParamType(click.ParamType):
    name = 'integer'
    def __init__(self, logs:ACutils.LOG):
        super().__init__()
        self.__logs=logs
    def convert(self, value, param, ctx):
        try:
            
            s=ACutils.StringValue(self.__logs)
            s.set(value)
            return int(s)
        except ValueError:
            logs.info('(%s) is not a valid integer. Retype the data and/or '
                      'prefixed with 0b/0B, 0o/0O, or 0x/0X as necessary.' % value)

@click.argument('infile')
@click.option('-xml', '--xml', metavar='filename', default="",
              help='Location of the file that contains the RoT configuration')
@click.option('-nxml', '--xml_name', default=[], multiple=True,
              help='Tag <Name> content in the xml file. This will allow to '
              'filter the xml file and choose the value of the parameter to use'
              'It is possible to use more of one argument. Recommended to use with'
              'the --expression option')
@click.option('-l', '--layout', metavar='filename', default="",
              help='Location of the file that contains preprocessed macros')
@click.option('-m', '--macro', default="",
              help='macro symbol to search in the preprocessed file')
@click.option('-n', '--name', required=True,
              help='symbol to search in the linker file. If the symbol is found'
              'The symbol value will be modified')
@click.option('-e', '--expression', default="",
              help='Expression in order to modify the value found. The --constant'
              'argument is optional. The value found is declared as "value", the'
              'others values are declared as cons1, cons2....cons3'
              'Example 1: ((value + cons1)/cons2) - cons3'
              'If using multiple values they need to be defined as val1, val2 ...'
              'Example 2: ((val1 + cons1)/(cons2 + val2)) - cons3')
@click.option('-cons', '--constants', multiple=True,
              default=[], help='Constant values declared in the expression')
@click.option('--vb', default=False, is_flag=True,
              help='Define if debug traces will be displayed')
@click.command(help='Get the value from a C or xml variable in order to update a linker variable'
               'When using a c file the commands --layout and --macro are required'
               'When using a xml file the commands --xml and --xml_name are required'
               'More information in the README file')
def linker(infile, xml, xml_name, layout, macro, name, expression, constants, vb):
    if vb is False:
        logs=ACutils.LOG(True,True)
    else:
        logs=AppliCfgLog
    logs.begin("linker")
    logs.array(sys.argv)
    # Verify if the input files exist
    linkerfile=ACutils.SCRIPT_APPLI(infile, logs)
    readvalue=ACutils.StringValue(logs)
    values=[]
    if os.path.isfile(xml): # Recover value(s) from xml file
        if not xml_name:
            logs.error("--xml_name argument was not defined")
        # Initialize xml object
        xml_obj = xml_parser.XML_APPLI(xml, logs)
        xml_obj.get_items("Name")
        for xname in xml_name:
            readvalue.set(xml_obj.get_value(xname, "Value"))
            if readvalue.is_none():
                logs.error("Not value found for '%s'. Output file can not be modified" % xname)
            values.append(readvalue)
    elif os.path.isfile(layout): # Recover value from macro_preprocessed file
        if not macro:
            logs.error("--macro argument was not defined")
        # Compute and return value from macro file
        macrofile=ACutils.SCRIPT_APPLI(layout, logs)
        readvalue.set(macrofile.get_macro_value(macro))
        values.append(readvalue)
    else:
        logs.error("xml or layout file were not defined")
    # Different input values are only accepted when the expression argument is used
    # otherwise the last value of xml_values list will be used
    if len(values) > 1 and not expression:
        value=int(values[-1])
        logs.info("Using the value of the last content recovered %s" %value)
    else:
        value=int(values[0])
    # If expression is declared, the expression will be computed
    if expression :
        if "cons" in expression and not constants :
            logs.error("Expressions needs constants values")
        value = ACutils.compute_expression(expression, values, constants, logs)

    # For linker project the final value is required en hex format. The value
    # will be formatted
    logs.info("Value to be used %s" %hex(value))
    # Verify linker file type
    linker_pattern=linkerfile.linker_pattern(name)
    logs.info("File to update '%s'" % linkerfile.name())
    logs.info("Linker variable \"%s\" " % name +" is updated with value \"%s\"" % hex(value))
    # Modify linker value
    if not linkerfile.modify_file_value(linker_pattern,None, hex(value)):
        logs.error("Linker option (%s) not found in the file" % name)
    else:
        linkerfile.save()

@click.argument('infile')
@click.option('-l', '--layout', metavar='filename', default="",
              help='Location of the file that contains preprocessed macros')
@click.option('-m', '--macro', default="",
              help='macro symbol to search in the preprocessed file')
@click.option('-e', '--enable', default="", type=BasedIntAllParamType(AppliCfgLog),
              help='Define xml enable status 0 for disable or 1 for enable')
@click.option('-n', '--name', default="",
              help='Tag <Name> content in the xml file. This will allow to '
              'filter the xml file. The tag <Enable> will be modified')
@click.option('-c', '--command', default="",
              help='Tag <Command> content in the xml file. This will allow to '
              'filter the xml file. The tag <Enable> will be modified')
@click.option('--vb', default=False, is_flag=True,
              help='Define if debug traces will be displayed')
@click.command(help='Enable or disable xml options. The enable status could be '
                    'modified with the --layout and --macro arguments '
                    'or by defining the --enable argument.'
                    'More information in the README file')
def xmlen(infile, layout, macro, enable, name, command, vb):
    if vb is False:
        logs=LOG(True,True)
    else:
        logs=AppliCfgLog
    logs.begin("xmlen")
    logs.array(sys.argv)
    # Verify if the input files exist
    if not os.path.isfile(infile):
        AppliCfgLog.error("File (%s) not found" % infile)
    if os.path.isfile(layout):
        if not macro:
            logs.error("--macro argument was not defined")
        # if macro value layout is defined, the tag Enable will have the value
        # found in the file
        macrofile=ACutils.SCRIPT_APPLI(layout, logs)
        param_en_status = macrofile.get_macro_value(macro)
    else:
        if enable:
            param_en_status = enable
        else:
            logs.error("Enable option or layout were not defined")

    # If value is equals to zero the parameter will be disable otherwise it will
    # be enabled
    param_en_status = "0" if param_en_status == 0 else "1"
    # Initialize xml object
    if not name and not command:
        logs.error("Parameter tag <Name> or <Command> was not defined")
    # Define element to filter the parameters
    is_command = True if command else False
    element_name = "Command" if is_command else "Name"
    element_content = command if is_command else name
    xml_obj = xml_parser.XML_APPLI(infile, logs)
    xml_obj.get_items("Enable")
    xml_obj.get_filter_list(element_name)

    # Enable or disable the options
    if not xml_obj.modify_value(element_content, element_name, "Enable", param_en_status):
        tag_structure = "<%s>" % element_name + \
            element_content + "</%s>" % element_name
        logs.error("The xml value was not modified. Verify the parameter with the tag '%s'"
                 % tag_structure)
    # Modified and save the xml modified
    xml_obj.save_file()


@click.argument('infile')
@click.option('-l', '--layout', metavar='filename', default="",
              help='Location of the file that contains preprocessed macros')
@click.option('-m', '--macro', default="",
              help='macro symbol to search in the preprocessed file')
@click.option('-n', '--name', default="",
              help='Tag <Name> content in the xml file')
@click.option('-link', '--link', default=None,
              help='Tag <Link> content of the xml parameter')
@click.option('-v', '--value', default=None,
              help='Tag <Value> content of the xml parameter')
@click.option('-t', '--type', default=None,
              help='Tag <Type> content of the xml parameter')
@click.option('-c', '--command', default=None,
              help='Tag <Command> content in the xml file (with - or --)')
@click.option('-e', '--enable', default=None,
              help='Tag <Enable> content of the xml parameter  (0 disable or 1 enable)')
@click.option('-d', '--default', default=None,
              help='Tag <Default> content of the xml parameter')
@click.option('-h', '--hidden', default=None,
              help='Tag <Hidden> content of the xml parameter (0 no hidden, 1 hidden)')
@click.option('-ttip', '--tooltip', default=None,
              help='Tag <Tooltip> content of the xml parameter')
@click.option('-o', '--option', default="add",
              help='If not layout file is defined it is possible to add or remove'
                    'it is possible to add or remove a parameter manually : add or rm')
@click.option('-pp', '--parameter_position', type=BasedIntAllParamType(AppliCfgLog), default=2,
              help='Position of the new parameter. By default is 2')
@click.option('--vb', default=False, is_flag=True,
              help='Define if debug traces will be displayed')
@click.command(help='add or remove a xml parameter. The action (add/remove) can'
                    'be defined using  --layout and --macro arguments'
                    'or the --option argument'
                    'More information in the README file')
def xmlparam(infile, layout, macro, name, type, link, value, command, enable,
             default, hidden, tooltip, option, parameter_position, vb):
    # Verify if the input files exist
    if not os.path.isfile(infile):
        AppliCfgLog.error("File (%s) not found" % infile)
    if vb is False:
        logs=ACutils.LOG(True,True)
    else:
        logs=AppliCfgLog
    logs.begin("xmlparam")
    logs.array(sys.argv)
    if os.path.isfile(layout):
        if not macro:
            logs.error("--macro argument was not defined")
        # if layout is defined, the tag to be add/rm will
        # be get it from this file
        macrofile=ACutils.SCRIPT_APPLI(layout, logs)
        macro_value = macrofile.get_macro_value(macro)
        param_option = "add" if macro_value > 0 else "rm"
    else:
        if option:
            param_option = option
        else:
            logs.error("Option or layout file were not defined")
    if param_option == "rm":
        xml_obj = xml_parser.XML_APPLI(infile, logs)
        if not name and not command:
            logs.error("To remove a parameter it is necessary to define the name \
                    argument or the command argument")
        tag = "Command" if command else "Name"
        tag_content = command if command else name
        if not xml_obj.remove_param_item(tag, tag_content):
            tag_structure = f"<{tag}>{tag_content}</{tag}>"
            logs.error(f"The parameter '{tag_structure}' was not removed or not found")
        # Modified and save the xml modified
        xml_obj.save_file()
    elif param_option == "add":
        parameter_content = {}
        if not name is None:
            parameter_content["Name"] = name
        if not value is None:
            parameter_content["Value"] = value
        if not link is None:
            parameter_content["Link"] = link
        if not type is None:
            parameter_content["Type"] = type
        if not command is None:
            parameter_content["Command"] = command
        if not enable is None:
            parameter_content["Enable"] = enable
        if not hidden is None:
            parameter_content["Hidden"] = hidden
        if tooltip:
            parameter_content["Tooltip"] = tooltip
        if not default is None:
            parameter_content["Default"] = default

        if "Name" in parameter_content or "Command" in parameter_content:
            # Item to verify if Parameter already exists
            tag = "Command" if "Command" in parameter_content else "Name"
            xml_obj = xml_parser.XML_APPLI(infile, logs)
            if not xml_obj.add_param_item(tag, parameter_content, parameter_position):
                logs.error("Parameter was not added")
            # Modified and save the xml modified
            xml_obj.save_file()
    else:
        logs.error("Wrong option (%s) selected. Only add or rm are available" % option)


@click.argument('infile')
@click.option('-l', '--layout', metavar='filename', default="",
              help='Location of the file that contains preprocessed macros')
@click.option('-m', '--macro',
              help='macro symbol to search in the preprocessed file')
@click.option('-b', '--binary_file', metavar='filename', default="",
              help='Binary input file (AreaAPayload)')
@click.option('-b_el_idx', '--binary_element_index',
              type=BasedIntAllParamType(AppliCfgLog), default=None,
              help='Index of layout element')
@click.option('-b_el_sz', '--binary_element_size',
              type=BasedIntAllParamType(AppliCfgLog), default=48,
              help='Binary element size of layout element')
@click.option('-b_d_sz', '--binary_data_size',
              type=BasedIntAllParamType(AppliCfgLog), default=8,
              help='Binary data size in bits')
@click.option('-b_d_e', '--binary_data_endianness', default="big",
              help='Data binary endianness format (little or big)')
@click.option('-nxml_el_idx', '--xml_name_layout_index', default="",
              help='Tag <Name> content in the xml file. This will allow to '
              'define the Flash Layout configuration index')
@click.option('-xml', '--xml', metavar='filename', default="",
              help='Location of the file that contains the RoT configuration')
@click.option('-nxml', '--xml_name', default=[], multiple=True,
              help='Tag <Name> content in the xml file. If the parameter is '
                   'found the tag value content will be used')
@click.option('-txml', '--xml_tag', default="",
              help='Tag content in the xml file to modify. By default the content'
              'of tag <Value></Value> will be modified'
              'if --name or --command are not defined, --txml will be used to search'
              'the tag to be modified. Attention : The tag must be the only one existing'
              'in the xml file otherwise the first tag found will be modified')
@click.option('-sm', '--second_macro', default="",
              help='Use the second_macro value if value found with macro variable'
                  'is different to equals_to argument')
@click.option('-v', '--value', default=[], multiple=True,type=str,
              help='New value content of the parameter selected'
                   'If second_macro is defined, this arguments will help to define'
                   'witch value will be used. If the value found for macro symbol'
                   'is equals to the value argument, this value will be used.'
                   'Otherwise the value found for second_macro will be used')
@click.option('-n', '--name', default="",
              help='Tag <Name> content in the xml file. If the tag is found, '
                   'the tag <Value> content will be modified')
@click.option('-c', '--command', default="",
              help='Tag <Command> content in the xml file. If the tag is found'
                   'The tag <Value> content will be modified')
@click.option('-cond', '--condition', default="",
              help='Expression in order to verify if expression condition must be'
              'executed.'
              'Example : (val2 > 0 and val2  % 2== 0 )')
@click.option('-e', '--expression', default="",
              help='Expression in order to modify the value found. The --constant'
              'argument is optional. The value found is declared as "value", the'
              'others values are declared as cons1, cons2....cons3'
              'Example 1: ((value + cons1)/cons2) - cons3'
              'If using multiple values they need to be defined as val1, val2 ...'
              'Example 2: ((val1 + cons1)/(cons2 + val2)) - cons3')
@click.option('-cons', '--constants', multiple=True,
              default=[], help='Constant values declared in the expression')
@click.option('--decimal', default=False, is_flag=True,
              help='Define it if the value to be replaced is in decimal format'
                   'by default the value to replace is in hex format')
@click.option('--string', default=False, is_flag=True,
              help='Define it if the value to be replaced is in string format'
                   'by default the value to replace is in hex format')
@click.option('--vb', default=False, is_flag=True,
              help='Define if debug traces will be displayed')
@click.command(help='Modify the Tag <Value> content of an xml parameter'
               'When using a c file the commands --layout and --macro are required'
               'When using a xml file the commands --xml and --xml_name are required'
               'Or the final value could be defined using the --value argument'
               'More information in the README file')
def xmlval(infile, xml, xml_name, layout, macro, second_macro, value, binary_file,
           binary_element_index, binary_element_size, binary_data_size,
           binary_data_endianness, xml_name_layout_index, xml_tag, name, command,
           condition, expression, constants, decimal, string, vb):
    if vb is False:
        logs=ACutils.LOG(True,True)
    else:
        logs=AppliCfgLog
    logs.begin("xmlval")
    logs.array(sys.argv)
    if decimal and string:
        logs.error("Choose only one format option")
    # Verify if the input files exist
    if not os.path.isfile(infile):
        logs.error("File (%s) not found" % infile)
    # Define value or values to use
    read_values = []
    do_compute_expression = True
    layout_config_index=ACutils.StringValue(logs)
    if os.path.isfile(layout): # Recover value from macro_preprocessed file
        if not macro:
            logs.error("--macro argument was not defined")
        # Compute and return value from macro file
        macrofile=ACutils.SCRIPT_APPLI(layout, logs)
        last_value=ACutils.StringValue(logs)
        v=ACutils.StringValue(logs)
        last_value.set(macrofile.get_macro_value(macro))
        if second_macro:
            found=False
            for x in value:
                v.set(x)
                if int(last_value)==int(v):
                    found=True
            
            # if macro arguments are defined, it is necessary to verify witch value use
            if found:
                logs.info("Value present, choosing second_macro instead")
                last_value.set(macrofile.get_macro_value(second_macro))
        read_values.append(last_value)
    elif os.path.isfile(xml):  # Recover value from xml file
        if not xml_name:
            logs.error("--xml_name argument was not defined")
        # Initialize xml object
        xml_obj = xml_parser.XML_APPLI(xml, logs)
        xml_obj.get_items("Name")
        logs.info("Search xml_name")
        for xname in xml_name:
            last_value=ACutils.StringValue(logs)
            last_value.set(xml_obj.get_value(xname, "Value", print_debug=True))
            if last_value.is_none():
                logs.error("No value found for '%s'. Output file can not be modified" % xname)
            read_values.append(last_value)
    elif os.path.isfile(binary_file):
        if binary_element_index is None:
            logs.error("xml_name_layout_index is mandatory")
        if not xml_name_layout_index:
            logs.error("Negative index are not available")
        if binary_element_index < 0 :
            logs.error("Wrong index. Available 1 to %s" % str(binary_element_size-1))
        xml_obj = xml_parser.XML_APPLI(infile, logs)
        xml_obj.get_items("Name")
        logs.info('Search xml_name_layout_index')
        readvalue:str=xml_obj.get_value(xml_name_layout_index, "Value")
        layout_config_index.set(readvalue)
        if layout_config_index.is_none():
            logs.error("No value found for '%s'" %readvalue)

        # Get the started index of the element
        binary_element_offset = int(binary_element_size * int(layout_config_index))

        # Read binary file and get element configuration
        with open(binary_file, 'rb') as f:
            data = f.read()

        if binary_element_offset + binary_element_size > len(data):
            logs.error("Binary element configuration is bigger than binary file size")
        binary_conf_element = data[binary_element_offset : binary_element_offset + binary_element_size]

        if binary_element_index >= len(binary_conf_element):
            logs.error("The index value is bigger than the element size")
        if binary_data_size % 8 != 0 :
            logs.error("Bits size not available")
        bytes_to_recover=binary_data_size//8
        
        start_idx = binary_element_index
        end_idx = binary_element_index + bytes_to_recover
        if end_idx > len(binary_conf_element) :
              logs.error("Data overflow (data to recover outside of element)")
        binary_data = binary_conf_element[start_idx : end_idx]
        last_value=ACutils.StringValue(logs)
        last_value.set(binary_data, binary_data_endianness)
        # Get extra xml input values
        read_values = [last_value] # The binary value will be always the first xml value
        logs.info("Search xml_name")
        for xname in xml_name:
            extra_last_value = xml_obj.get_value(xname, "Value", print_debug=True)
            if not extra_last_value:
                logs.error("Not value found for '%s'. Output file can not be modified"% xname)
            last_value=ACutils.StringValue(logs)
            last_value.set(extra_last_value)
            read_values.append(last_value)
    elif value:
        for i in value:
            last_value=ACutils.StringValue(logs)
            last_value.set(i)
            if last_value.is_none():
                if decimal:
                    logs.error("The value %s have to be a numeral" % i)
                read_values.append(value)#pure string
                last_value=i
            else:
                read_values.append(last_value)
    else:
        logs.error("Incorrect -v, -b, -xml, -l option")
    if len(read_values)==0:
        logs.error("Value option or xml/layout file were not defined")
    elif len(read_values) > 1 and not expression:
        # Different input values are only accepted when the expression argument is used
        # otherwise the last value of read_values list will be used
        if decimal:
            logs.info("Using the value of the last content recovered %s" %last_value.getInt())
        elif type(last_value)==str:
            logs.info("Using the value of the last content recovered %s "%last_value)
        else:
            logs.info("Using the value of the last content recovered %s" %last_value.getHex())
    # Post value treatment
    if condition and type(last_value)!=str:
        do_compute_expression = ACutils.compute_condition(condition, read_values,logs)
        if not do_compute_expression:
            logs.info("Condition is not met")
            return
    # If expression is declared, the expression will be computed
    if expression:
        if do_compute_expression:
            if "cons" in expression and not constants :
                logs.error("Expressions needs constants values")
            last_value.set(ACutils.compute_expression(expression, read_values, constants, logs))
    # For xml file the final value is required en hex format. The value
    # will be formatted
    if type(last_value)==str:
        choosenXmlValue=last_value
    elif decimal:
        choosenXmlValue:str=last_value.getInt()
    else:
        choosenXmlValue:str=last_value.getHex()
    logs.info("Value to be used %s" %choosenXmlValue)
    # Define element to filter the parameters
    tag_to_modify = "Value" if not xml_tag else xml_tag
    is_unique_tag = False
    if not command and not name :
        if not xml_tag:
            logs.error("Tag to modify its content was not defined")
        element_name = xml_tag
        element_content = ""
        is_unique_tag = True
    else :
        element_name = "Command" if command else "Name"
        element_content = command if command else name
    # Initialize xml object
    xml_obj = xml_parser.XML_APPLI(infile, logs)
    xml_obj.get_items(element_name)
    
    # Enable or disable the xml options
    if is_unique_tag :
        ret = xml_obj.modify_value(tag_content=element_name, new_sub_tag_content=choosenXmlValue)
    else:
        ret = xml_obj.modify_value(tag_content=element_content, tag=element_name, sub_tag=tag_to_modify, new_sub_tag_content=choosenXmlValue)
    if not ret:
        tag_structure = "<%s>" % element_name + \
            element_content + "</%s>" % element_name
        logs.error("The xml value was not modified. Verify the parameter with the tag '%s'" % tag_structure)
    # Modified and save the xml modified
    xml_obj.save_file()

@click.argument('infile')
@click.option('-l', '--layout', metavar='filename', default="",
              help='Location of the file that contains preprocessed macros')
@click.option('-m', '--macro',
              help='macro symbol to search in the preprocessed file')
@click.option('-v', '--value', default=None, type=BasedIntAllParamType(AppliCfgLog),
              help='If --second_name is defined, this arguments will help to define'
                   'witch tag <Name> will be used. If the value found for macro symbol'
                   'is equals to the --value argument, the --second_name will be used.'
                   'Otherwise the --name will be used')
@click.option('-n', '--name', default="", required=True,
              help='New Tag <Name> content in the xml file')
@click.option('-sn', '--second_name', default="",
              help='Define a second option of tag <Name> content. If the value'
              'found in the macro preprocessed file is different to --value'
              'argument the second_name will be used')
@click.option('-c', '--command', default="", required=True,
              help='Tag <Command> content in the xml file. If the tag is found'
                   'The name content will be modified')
@click.option('--vb', default=False, is_flag=True,
              help='Define if debug traces will be displayed')
@click.command(help='Modify the Tag <Name> content of an xml parameter'
                    'The modification could be executed with the --layout and'
                    '--macro arguments. Or by defining the --name argument'
                    'More information in the README file')
def xmlname(infile, layout, macro, value, name, second_name, command, vb):
    # Verify if the input files exist
    if vb is False:
        logs=ACutils.LOG(True,True)
    else:
        logs=AppliCfgLog
    logs.begin("xmlname")
    logs.array(sys.argv)
    if not os.path.isfile(infile):
        logs.error("File (%s) not found" % infile)
    if os.path.isfile(layout): # Recover value from macro_preprocessed file
        if not macro:
            logs.error("--macro argument was not defined")
        if value is None:
            logs.error("--value argument was not defined")
        # Compute and return value from macro file
        macrofile=ACutils.SCRIPT_APPLI(layout, logs)
        xml_value = macrofile.get_macro_value(macro)
        # if two macro arguments are defined, it is necessary to verify witch value use
        new_name = name
        if second_name :
            new_name = second_name if xml_value == value else new_name
        else :
            if xml_value != value :
                str_debug = "Macro value not equals to "+ \
                            "expected value . " + \
                            "Name modification will not be executed"
                logs.info(str_debug)
                sys.exit(0)
        if not new_name:
            logs.error("Tag <Name>%s</Name> will not be modified" % name)
    else:
        if name:
            new_name = name
        else:
            logs.error("name option or layout file were not defined")
    # Initialize xml object
    xml_obj = xml_parser.XML_APPLI(infile, logs)
    xml_obj.get_items("Command")
    # Enable or disable the xml options
    tag = "Command"
    ret = xml_obj.modify_value(command, tag, "Name", new_name)
    if not ret:
        tag_structure = "<%s>" % tag + command + "</%s>" % tag
        logs.error("The xml value was not modified. Verify the parameter with the tag '%s'" % tag_structure)
    # Modified and save the xml modified
    xml_obj.save_file()


@click.argument('infile')
@click.option('-xml', '--xml', metavar='filename', default="", required=True,
              help='Location of the file that contains the ST-iRoT configuration')
@click.option('-ob', '--option_byte', required=True,
              help='Define the Rot protection : SECWM, SRAM2_RST, WRPS, SRAM2_ECC, ECC_ON_SRAM')
@click.option('-cond', '--condition', required=False,type=BasedIntAllParamType(AppliCfgLog),
              help='value to check for SRAM2_RST, SRAM2_ECC, ECC_ON_SRAM, Default : x8, x40, x1')
@click.option('-sp', '--secure_pattern', required=True,
              help='Principal Tag <Name> content of the Rot protection')
@click.option('-co', '--code_offset', default="",
              help='Tag <Name> content for the primary code offset.information. '
              'To set SECWM it is necessary to get the value of the parameter '
              'with tag selected')
@click.option('-cs', '--code_size', default="",type=str,
              help='Tag <Name> content for the primary code size information. '
              'To set SECWM it is necessary to get the value of the parameter '
              'with tag selected')
@click.option('-fs', '--full_sec', default="",
              help='Tag <Name> content for the full secure information.'
              'To set SECWM it is necessary to get the value of the parameter '
              'with tag selected')
@click.option('-d', '--division', type=BasedIntAllParamType(AppliCfgLog),
              help='Define the microcontroller sector size')
@click.option('-bs', '--bank_size', type=BasedIntAllParamType(AppliCfgLog),
              help='Define the microcontroller bank size')
@click.option('-b', '--begin', default="stm32programmercli",
              help='begin of line to replace ')
@click.option('-img', '--image_number', default="",
              help='number of managed images')
@click.option('--vb', default=False, is_flag=True,
              help='Define if debug traces will be displayed')
@click.command(help='Modify the protections of the RoT script'
                    'When the SECWM protection is selected the arguments'
                    '--code_size, --code_offset and --full_secure are required'
                    'More information in the README file')
def obscript(infile, xml, begin, option_byte, secure_pattern, code_size, code_offset,
             full_sec, division, bank_size, image_number, vb, condition):
    if vb is False:
        logs=ACutils.LOG(True,True)
    else:
        logs=AppliCfgLog
    logs.begin("obscript")
    logs.array(sys.argv)
    # Verify if the input files exist
    output=ACutils.SCRIPT_APPLI(infile, logs)
    if not os.path.isfile(xml):
        logs.error("The xml file (%s) was not found" % xml)
    # Options available for execution
    found=False
    if option_byte in ["SRAM2_RST", "SRAM2_ECC", "ECC_ON_SRAM"]:
        found=True
    else:
        for i in ["SECWM", "WRPS"]:
            if option_byte.startswith(i):
                found=True
                break
    if not found:
        logs.error("Invalid option byte (%s)" % option_byte)

    # Initialize xml object
    replace_val = {}
    xml_obj = xml_parser.XML_APPLI(xml, logs)
    xml_obj.get_items("Name")
    # Get the value of the Rot protection
    sec_val_size=ACutils.StringValue(logs)
    found=False
    if secure_pattern:
        logs.info("Search secure_pattern")
        sec_val_size.set(xml_obj.get_value(secure_pattern, "Value"))
        if not sec_val_size.is_none():
            found=True
    if not found:
        logs.error("Not value found for '%s'" % secure_pattern)
   
    if "WRPS" in option_byte:
        if not code_offset:
            logs.error("Tag <Name> content to get the code offset was not defined")
        if not code_size:
            logs.error("Tag <Name> content to get the code size was not defined")
        logs.info("Search code_offset")
        code_offset = xml_obj.get_value(code_offset, "Value")

        ## If the firmware is full secure, it this necessary to get the size
        ## of "Firmware area size" instead the "Size of the secure area"
        if division is None:
            division=0x2000
        if bank_size is None:
            bank_size=0x100000
        if code_offset :
            replace_val["WRPS"] = stirot_compute_wrps(code_offset, code_size, division, bank_size, logs)
        else :
            logs.error("Code primary offset or secure value size are empty")
        
    elif "SECWM" in option_byte:
      try:
        if not code_offset:
            logs.error("Tag <Name> content to get the primary code offset was not defined")
        if not full_sec:
            logs.error("Tag <Name> content to get the full secure was not defined")
        if not code_size:
            logs.error("Tag <Name> content to get the primary code size was not defined")
        is_full_sec=ACutils.StringValue(logs)
        code_primary_offset=ACutils.StringValue(logs)
        logs.info("Search full_sec")
        is_full_sec.set(xml_obj.get_value(full_sec, "Value"))
        logs.info("Search code_offset")
        code_primary_offset.set(xml_obj.get_value(code_offset, "Value"))
        #sector size calculation the water marck
        if division is None:
            division=ACutils.StringValue(logs)
            elem = xml_obj.get_info('FlashSectorSize')
            if elem is not None:
                division.set(elem[0].text)
            else:
                division.set(0x2000)
            logs.info(f"Read division ({elem[0].text}) from XML")
        if bank_size is None:
            bank_size=ACutils.StringValue(logs)
            elem=xml_obj.get_info('FlashSize')
            if elem is not None:
                bank_size.set(elem[0].text)
                bank_size.half()
            else:
                bank_size.set(0x100000)
            logs.info(f"Read bank_size ({elem[0].text}) from XML")
        # If the firmware is full secure, it this necessary to get the size
        # of "Firmware area size" instead the "Size of the secure area"
        if not is_full_sec.is_none() and int(is_full_sec) != 0:
            logs.info("Search code_size")
            val=xml_obj.get_value(code_size, "Value")
            if val:
                sec_val_size.set(val)
            else:
                logs.error("Code size not found")
        if not code_primary_offset.is_none() :
            replace_val = ACutils.stirot_compute_wm(int(code_primary_offset), secure_area_size=int(sec_val_size), pagesize=int(division), bankSize=int(bank_size), logs=logs)
        else :
            logs.error("Code primary offset or secure value size are empty")
      except Exception as e: 
        raise ACutils.AppliCFGException("SECWM error, "+str(e))
    elif "SRAM2_RST" in option_byte:
        if condition is None: condition=0x8 #for H5
        try:
          if condition==int(sec_val_size):
           replace_val["SRAM2_RST"] = "1"
          else:
           replace_val["SRAM2_RST"] = "0"
        except ACutils.Exception as e: 
          raise AppliCFGException("SRAM2_RST error, "+str(e))
    elif "SRAM2_ECC" in option_byte:
        if condition is None: condition=0x40 #for H5
        try:
          if condition==int(sec_val_size):
           replace_val["SRAM2_ECC"] = "1"
          else:
           replace_val["SRAM2_ECC"] = "0"
        except ACutils.Exception as e: 
          raise AppliCFGException("SRAM2_ECC error, "+str(e))
    elif "ECC_ON_SRAM" in option_byte:
        if condition is None: condition=0x1 #for H7
        try:
          if condition==int(sec_val_size):
           replace_val["ECC_ON_SRAM"] = "1"
          else:
           replace_val["ECC_ON_SRAM"] = "0"
        except Exception as e: 
          raise ACutils.AppliCFGException("ECC_ON_SRAM error, "+str(e))

    ret = False
    protections_error = []
    # Routine that allows to replace all the values contained in the dictionary
    logs.info("File to update '%s'" % output.name())
    if output.is_batch():
        begin=fr"^\s*%{begin}%[\s+[\w\-=%]*]*"
    elif output.is_shell:
        begin=fr"^\s*\"?\${begin}\"?[\s+[\$\w\-=]*]*"
    else:
        logs.error("file type error")
    
    for opt in replace_val:
        pattern=fr"{begin}\s+{opt}\s*="
        # There are several places where the variables to be replaced are defined,
        # however it was found that the place where they should be replaced
        # the string "stm32programmercli" is always there, if there is a change
        # in the script, this pattern must be modified.
        # Sequence that allows us to find the value that will be replaced
        new_value = replace_val[opt]
        status = output.modify_file_value(pattern, None, new_value)
        if not status:
            protections_error.append(opt)
        else:
            logs.info(f"Protection variable \"{opt}\" is updated with value \"{new_value}\"")
    if protections_error:
        logs.error("Protections (%s) not found in the file" %
                 ",".join(protections_error))
    else:
         output.save()


@click.argument('infile')
@click.option('-xml', '--xml', metavar='filename', default="", required=True,
              help='Location of the file that contains the ST-iRoT configuration')
@click.option('-bxml', '--begin_xml', required=True,
              help='Macro symbol for the start protection value. The symbol'
              'will be searched in the preprocessed file')
@click.option('-m', '--memory', required=True,
              help='Macro symbol for the end protection value. The symbol'
              'will be searched in the preprocessed file')
@click.option('-sxml', '--size_xml', default="", required=True,
               help='Macro symbol for the sector number value. The symbol'
              'will be searched in the preprocessed file')
@click.option('-s', '--slot', default="", required=True,
              help='Line start pattern to replace ')
@click.option('-d', '--division', type=BasedIntAllParamType(AppliCfgLog),
              help='Sector group size')
@click.option('--vb', default=False, is_flag=True,
              help='Define if debug traces will be displayed')
@click.command(help='command to set specific Option Byte'
                    'More information in the README file')
def sectorerase(infile, xml, slot, begin_xml, size_xml, memory, division, vb):
    # Verify if the input files exist
    if vb is False:
        logs=ACutils.LOG(True,True)
    else:
        logs=AppliCfgLog
    logs.begin("sectorerase")
    logs.array(sys.argv)
    output=ACutils.SCRIPT_APPLI(infile, logs)
    if not slot.startswith("primary") and not slot.startswith("secondary"):
        logs.error("Slot '%s' not supported" % slot)
    if not memory.startswith("ext_nvm"):
        logs.error("Memory type '%s' not supported" % memory)

    start_value=ACutils.StringValue(logs)
    size_value=ACutils.StringValue(logs)
    erase = {"start": begin_xml,
             "size": size_xml}
    logs.info("Obtaining the input values to calculate the sectors to erase '%s' area" %slot)
    logs.info("Searching for '%s' area" %erase["start"])

    if not os.path.isfile(xml): # Recover values from xml file
        logs.error("xml or layout file were not defined")

    xml_obj = xml_parser.XML_APPLI(xml, logs)
    xml_obj.get_items("Name")
    logs.info("Search begin_xml")
    xml_obj.get_value(begin_xml, "Value")
    start_value.set(xml_obj.get_value(erase["start"], "Value"))
    size_value.set(xml_obj.get_value(erase["size"], "Value"))

    if start_value.is_none():
        logs.error("Variable (%s) not found or not computed" % erase["start"])
    if start_value.is_none():
        logs.error("Variable (%s) not found or not computed" % erase["size"])

    # When using .bat files the variable needs to be set
    erase_line_start=f"{memory}_{slot}_start"
    pattern_start=output.pattern(erase_line_start,"=")
    erase_line_stop=f"{memory}_{slot}_stop"
    pattern_stop=output.pattern(erase_line_stop,"=")
    
    erase_slots_area  = ACutils.compute_sector_area( int(start_value), int(size_value), logs, int(division))
    
    op1=output.modify_file_value(pattern_start, None, str(erase_slots_area[0]))
    op2=output.modify_file_value(pattern_stop, None, str(erase_slots_area[1]))

    if not op1:
        logs.error("Initial pattern (%s) not found" % begin_line)
    else:
        str_debug = "erase slot \"%s_start\" is updated with value \"%s\"" % (erase_line_start, erase_slots_area[0])
        logs.info(str_debug)
    if not op2:
        logs.error("Initial pattern (%s) not found" % begin_line)
    else:
        str_debug = "erase slot \"%s_stop\" is updated with value \"%s\"" % (erase_line_stop, erase_slots_area[1])
        logs.info(str_debug)
    if op1 and op2:
        output.save()

@click.argument('infile')
@click.option('-l', '--layout', metavar='filename',
              help='Location of the file that contains preprocessed macros')
@click.option('-m', '--macro', required=True, default="",
              help='macro symbol to search in the preprocessed file')
@click.option('-xml', '--xml', metavar='filename',
              help='Location of the file that contains the RoT configuration')
@click.option('-v', '--value', type=BasedIntAllParamType(AppliCfgLog), multiple=False,
              help='new value for macro')
@click.option('-nxml', '--xml_name', default=[], multiple=True,
              help='Tag <Name> content in the xml file. If the parameter is'
                   'found the tag value content will be used')
@click.option('-n', '--name', required=True,
              help='#define variable to be modified')
@click.option('--parenthesis', default=False, is_flag=True,
              help='Define it if the value to be replaced is between parenthesis'
                   'by default the value to replace is between white spaces')
@click.option('--decimal', default=False, is_flag=True,
              help='Define it if the value to be replaced is in decimal format'
                   'by default the value to replace is in hex format')
@click.option('--string', default=False, is_flag=True,
              help='Define it if the value to be replaced is in string format'
                   'by default the value to replace is in hex format')
@click.option('-e', '--expression', default="",
              help='Expression in order to modify the value found. The --constant'
              'argument is optional. The value found is declared as "value", the'
              'others values are declared as cons1, cons2....cons3'
              'Example 1: ((value + cons1)/cons2) - cons3'
              'If using multiple values they need to be defined as val1, val2 ...'
              'Example 2: ((val1 + cons1)/(cons2 + val2)) - cons3')
@click.option('-cons', '--constants', multiple=True,
              default=[], help='Constant values declared in the expression')
@click.option('-o', '--occurrence', type=int, default=1, required=False, help='occurrence to be modified')
@click.option('--vb', default=False, is_flag=True,
              help='Define if debug traces will be displayed')

@click.command(help='Modify the value of one #define variable in a ".h" file'
                    'More information in the README file')
#do not launch error if layout, xml files not exist
def definevalue(infile, layout, macro, xml, value, xml_name, name, parenthesis, expression, constants, occurrence, string, decimal, vb):
    # Verify if the input files exist
    if vb is False:
        logs=ACutils.LOG(True,True)
    else:
        logs=AppliCfgLog
    logs.begin("definevalue")
    logs.array(sys.argv)
    cpt=0
    if xml is not None:
        cpt+=1
    if layout is not None:
        cpt+=1
    if value is not None:
        cpt+=1
    if cpt!=1:
        logs.error("One and one only parameter among xml, layout, value is needed")
    if decimal and string:
        logs.error("Choose only one format option")
    values=[]
    output=ACutils.SCRIPT_APPLI(infile, logs)
    if layout is not None and os.path.isfile(layout): #Recover values from macro_preprocessed file
        if not macro:
            logs.error("--macro argument was not defined")
        # Compute and return value from macro file
        macrofile=ACutils.SCRIPT_APPLI(layout, logs)
        data=ACutils.StringValue(logs)
        data.set(macrofile.get_macro_value(macro))
        values.append(data)
    elif xml is not None and os.path.isfile(xml): # Recover values from xml file
        if xml_name == []:
            logs.error("--xml_name argument was not defined")
        # Initialize xml object
        xml_obj = xml_parser.XML_APPLI(xml, logs)
        xml_obj.get_items("Name")
        logs.info("Search xml_name")
        for xname in xml_name:
            data=ACutils.StringValue(logs)
            data.set(xml_obj.get_value(xname, "Value", print_debug=True))
            if data.is_none():
                logs.error("No value found for '%s'. Output file can not be modified" % xml_name)
            values.append(data)
    else:
        data=ACutils.StringValue(logs)
        data.set(value)
        values.append(data)

    # Different input values are only accepted when the expression argument is used
    # otherwise the last value of values list will be used
    if len(values) > 1 and not expression:
        if decimal:
            logs.info("Using the value of the last content recovered %s" %data.getInt())
        else:
            logs.info("Using the value of the last content recovered %s" %data.getHex())
    
    # If expression is declared, the expression will be computed
    if expression :
        if "cons" in expression and not constants :
            logs.error("Expressions needs constants values")
        data.set(ACutils.compute_expression(expression, values, constants, logs))
    # For xml file the final value is required en hex format. The value will be formatted

    if decimal:
        choosenvalue=data.getInt()
    else:
        choosenvalue=data.getHex()

    # Select the way to obtain the value to be replaced
    # white spaces #define tests       5   /*test value*/
    # or
    # parenthesis  #define tests      (5)  /*test value*/
    logs.info("File to update '%s'" % output.name())
    if output.is_batch() or output.is_shell():
        pattern=output.pattern(name, "=",False)
    else:
        pattern=output.pattern(name, "",False)
    if parenthesis:
        pattern+=r"\(\s*"

    oldval=output.get_file_value(pattern, occurrence=occurrence)
    ret = output.modify_file_value(pattern, oldval , choosenvalue, occurrence)
    if not ret:
        logs.error("Define variable (%s) not found in the file" % name)
    else:
        str_debug = "Define variable \"%s\" is updated with value \"%s\"" %(name, choosenvalue)
        logs.info(str_debug)
        output.save()

@click.argument('infile')
@click.option('-xml', '--xml', metavar='filename', default="",
              help='Location of the file that contains the RoT configuration')
@click.option('-l', '--layout', metavar='filename', default="",
              help='Location of the file that contains preprocessed macros')
@click.option('-m', '--macro', default="",
              help='macro symbol to search in the preprocessed file')
@click.option('-v', '--value', type=BasedIntAllParamType(AppliCfgLog), default="",
              help='Expected value int the input file( c or xml)'
                   'If value is equals : Uncomment the define variable'
                   'otherwise : Comment the define variable')
@click.option('-a', '--action', default="",
              help='User can define if the variable will be comment or uncomment')
@click.option('-n', '--name', required=True,
              help='Define variable to be modified in the H file')
@click.option('-nxml', '--xml_name', default="",
              help='Tag <Name> content in the xml file. If the parameter is'
                   'found the tag value content will be used')
@click.option('--vb', default=False, is_flag=True,
              help='Define if debug traces will be displayed')
@click.command(help='Comment/Uncomment a #define variable in a project file'
               'When using a c file the commands --layout and --macro are required'
               'When using a xml file the commands --xml and --xml_name are required'
               'More information in the README file')
def setdefine(infile, xml, xml_name, layout, macro, action, value, name, vb):
    # Verify if the input files exist
    if vb is False:
        logs=ACutils.LOG(True,True)
    else:
        logs=AppliCfgLog
    logs.begin("setdefine")
    logs.array(sys.argv)
    xml_value=ACutils.StringValue(logs)
    macro_value=ACutils.StringValue(logs)
    output=ACutils.SCRIPT_APPLI(infile, logs)
    
    cpt=0
    if xml:
        cpt+=1
    if layout:
        cpt+=1
    if cpt==2:
        logs.error("One, and one only parameter among xml, layout is needed")
    if os.path.isfile(xml): # Recover value from xml file
        if not xml_name:
            logs.error("--xml_name argument was not defined")
        # Initialize xml object
        xml_obj = xml_parser.XML_APPLI(xml, logs)
        xml_obj.get_items("Name")
        logs.info("Search xml_name")
        xml_value.set(xml_obj.get_value(xml_name, "Value"))
        if xml_value.is_none():
            logs.error("No value found for '%s'. Include file can not be modified" % xml_name)
        elif int(xml_value) == value:
            action = "uncomment"
        else:
            action="comment"
    elif os.path.isfile(layout): # Recover value from macro_preprocessed file
        if not macro:
            logs.error("--macro argument was not defined")
        # Compute and return value from macro file
        macrofile=ACutils.SCRIPT_APPLI(layout, logs)
        macro_value.set(macrofile.get_macro_value(macro))
        if macro_value.is_none():
            logs.error("No value found for '%s'. Include file can not be modified" % xml_name)
        elif int(macro_value) == value:
            action = "uncomment"
        else:
            action="comment"
    else:
        if not action :
            logs.error("xml or layout file were not defined")
        action = action.lower()
        if not action in ["uncomment", "comment"] :
            logs.error("Unknown action %s" %action)
    logs.info("File to update '%s'" % output.name())
    if not output.toggle_comment_line(name, action):
        logs.info("Define '%s' action was not performed" % action)
    else:
        str_debug = "The define '%s' variable " %name + "will be %sed" %action
        logs.info(str_debug)
        output.save()
    # Modify define status


@click.argument('infile')
@click.option('-l', '--layout', metavar='filename', default="", required=True,
              help='Location of the file that contains preprocessed macros')
@click.option('-m', '--macro', required=True, default="",
              help='macro symbol to search in the preprocessed file')
@click.option('-n', '--name', required=True,
              help='Tag <Name> content in the xml file.  If the tag is found'
                   'The tag <KeyType> content will be modified')
@click.option('--vb', default=False, is_flag=True,
              help='Define if debug traces will be displayed')
@click.command(help='Modify key type and key file value'
                    'More information in the README file')
def keyconfig(infile, layout, macro, name, vb):
    if vb is False:
        logs=ACutils.LOG(True,True)
    else:
        logs=AppliCfgLog
    logs.begin("keyconfig")
    logs.array(sys.argv)
    # Verify if the input files exist
    
    if not os.path.isfile(infile):
        logs.error("File (%s) not found" % infile)
    if not os.path.isfile(layout): # Recover value from macro_preprocessed file
        logs.error("File (%s) not found" % layout)
    # Compute and return value from macro file
    
    macrofile=ACutils.SCRIPT_APPLI(layout, logs)
    value=macrofile.get_macro_value(macro)

    cryptoscheme = str(value)

    cs = {"0": "rsa_2048", "1": "rsa_3072", "2": "ecdsa-p256"}

    if not cryptoscheme in cs:
        logs.error("Wrong crypto scheme value (%s)" % cryptoscheme)

    # Initialize xml object
    xml_obj = xml_parser.XML_APPLI(infile, logs)
    xml_obj.get_items("Name")
    ret = xml_obj.modify_value( name, "Name", "KeyType", cs[cryptoscheme])
    if not ret:
        logs.error("The key modification was not successfully performed")

    # Modified and save the xml modified
    xml_obj.save_file()


@click.argument('infile')
@click.option('-l', '--layout', metavar='filename',
              help='Location of the file that contains preprocessed macros')
@click.option('-m', '--macro',
              help='macro symbol to search in the preprocessed file')
@click.option('-xml', '--xml', metavar='filename', default="",
              help='Location of the file that contains the RoT configuration')
@click.option('-n', '-nxml','--name', "--xml_name", default=[], multiple=True,
              help='Tag <Name> content in the xml file. If the parameter is'
                  'found the tag value content will be used'
                  'It is possible to use more of one argument. Recommended to '
                  'use with the --expression option')
@click.option('-b', '--begin', required=True,
              help='Line start pattern to replace ')
@click.option('-sr', '--save_result',  metavar='filename', default="",
              help='Save result in an external file')
@click.option('-d', '--division', type=BasedIntAllParamType(AppliCfgLog), default=None,
              help='divide the value')
@click.option('--decimal', default=False, is_flag=True,
              help='Define it if the value to be replaced es in decimal format'
                   'by default the value to replace is in hex format')
@click.option('-e', '--expression', default="",
              help='Expression in order to modify the value found. The --constant'
              'argument is optional. The value found is declared as "value", the'
              'others values are declared as cons1, cons2....cons3'
              'Example 1: ((value + cons1)/cons2) - cons3'
              'If using multiple values they need to be defined as val1, val2 ...'
              'Example 2: ((val1 + cons1)/(cons2 + val2)) - cons3')
@click.option('-cons', '--constants', multiple=True,
              default=[], help='Constant values declared in the expression')
@click.option('--vb', default=False, is_flag=True,
              help='Define if debug traces will be displayed')
@click.command(help='Modify bash /batch variable value with layout file content'
                    'More information in the README file')
def flash(infile, xml, layout, macro, name, begin, save_result, division, decimal,
          expression, constants, vb):
    if vb is False:
        logs=ACutils.LOG(True,True)
    else:
        logs=AppliCfgLog
    logs.begin("flash")
    logs.array(sys.argv)
    # Verify if the input files exist
    output=ACutils.SCRIPT_APPLI(infile, logs)
    values=[]
    value=ACutils.StringValue(logs)
    if os.path.isfile(xml): # Recover value from xml file
        # Initialize xml object
        xml_obj = xml_parser.XML_APPLI(xml, logs)
        xml_obj.get_items("Name")
        logs.info("Search xml_name")
        for xml_name in name:
            value.set(xml_obj.get_value(xml_name, "Value", print_debug=True))
            if value.is_none():
                logs.error("No value found for '%s'. Output file can not be modified"% xml_name)
            values.append(int(value))
    elif os.path.isfile(layout): # Recover value from macro_preprocessed file
        # Compute and return value from macro file
        macrofile=ACutils.SCRIPT_APPLI(layout, logs)
        value.set(macrofile.get_macro_value(macro))
        values.append(int(value))
    else:
        logs.error("xml or layout file were not defined")

    if expression and division:
        '''
            It is recommended to use the --expression option. The division option
            allow to keep compatibility with old scripts
        '''
        str_debug = "Only one operation at the time is valid." + \
                    "Please choose between --expression or --division options"
        logs.error(str_debug)
    # Different input values are only accepted when the expression argument is used
    # otherwise the last value of xml_values list will be used
    if len(values) > 1 and not expression:
        if decimal:
            logs.info("Using the value of the last content recovered %s" %values[-1])
        else:
            logs.info("Using the value of the last content recovered %s" %hex(values[-1]))
    # If expression is declared, the expression will be computed
    if expression :
        if "cons" in expression and not constants :
            logs.error("Expressions needs constants values")
        value.set(ACutils.compute_expression(expression, values, constants, logs))
    # Compute the value to use
    elif division is not None : # Division by 0 is not taken in consideration
        if int(division) == 0:
              logs.error("Division by 0 is not possible")
        # Division operation
        val=int(value)//int(division) #integer division, whole part.
        value.set(val)
        str_debug = "The value computed will by divided by '%s'." % division
        if decimal:
            str_debug += "The new value is '%s'" % value.getInt()
        else:
            str_debug += "The new value is '%s'" % value.getHex()
        logs.info(str_debug)
    # Choose the value format
    choosenValue=value.getInt() if decimal else value.getHex()

    logs.info("Value to be used %s" %choosenValue)
    # Define pattern to search
    # When using .bat files the variable needs to be set
    logs.info("File to update '%s'" % output.name())
    pattern=output.pattern(begin, "=")
    if not output.modify_file_value(pattern, None, choosenValue):
        logs.error("Initial pattern (%s) not found in File" % begin)
    else:
        str_debug = "Script variable \"%s\" is updated with value \"%s\"" %(begin,choosenValue)
        logs.info(str_debug)
        output.save()
    if save_result :
        logs.info("The value %s is saved in the %s file" % (choosenValue, save_result))
        f = open(save_result, "w")
        f.write(choosenValue)
        f.close()

@click.argument('infile')
@click.option('-l', '--layout', metavar='filename', required=True,
              help='Location of the file that contains preprocessed macros')
@click.option('-ms', '--macro_start', required=True,
              help='Macro symbol for the start protection value. The symbol'
              'will be searched in the preprocessed file')
@click.option('-me', '--macro_end', required=True,
              help='Macro symbol for the end protection value. The symbol'
              'will be searched in the preprocessed file')
@click.option('-msec', '--macro_sectors', required=True,
               help='Macro symbol for the sector number value. The symbol'
              'will be searched in the preprocessed file')
@click.option('-b', '--begin', required=True,
              help='Line start pattern to replace ')
@click.option('-d', '--division', type=BasedIntAllParamType(AppliCfgLog), default=0x2000,
              help='Sector group size')
@click.option('-pa', '--protected_area', type=BasedIntAllParamType(AppliCfgLog), default=0x10000,
              help='Define the protected area size')
@click.option('--vb', default=False, is_flag=True,
              help='Define if debug traces will be displayed')
@click.command(help='command to set specific Option Byte'
                    'More information in the README file')
def setob(infile, layout, begin, macro_start, macro_end, macro_sectors,
          division, vb, protected_area):
    if vb is False:
        logs=ACutils.LOG(True,True)
    else:
        logs=AppliCfgLog
    logs.begin("setob")
    logs.array(sys.argv)
    # Verify if the input files exist
    output=ACutils.SCRIPT_APPLI(infile, logs)
    
    if not begin.startswith("wrp") and not begin.startswith("hdp") and not begin.startswith("OEMiLoader_address"):
        logs.error("Option Byte '%s' not supported" % begin)

    ob = {"start": macro_start,
          "end": macro_end,
          "secnbr": macro_sectors}

    # Compute and return values from macro file
    logs.info("Obtaining the input values to calculate the protection of '%s'" %begin)
    start_value=ACutils.StringValue(logs)
    end_value =ACutils.StringValue(logs)
    secnbr_value=ACutils.StringValue(logs)
    macrofile=ACutils.SCRIPT_APPLI(layout, logs)
    start_value.set(macrofile.get_macro_value(ob["start"]))
    end_value.set(macrofile.get_macro_value(ob["end"]))
    secnbr_value.set(macrofile.get_macro_value(ob["secnbr"]))
    logs.info("File to update '%s'" % output.name())
    # Calculation of Value for `wrpgrp`
    if begin == "wrpgrp1" or begin == "wrpgrp2":
        # When using .bat files the variable needs to be set
        pattern=output.pattern(begin, "=")
        wrpgrp1, wrpgrp2 = ACutils.compute_wrp_protections(int(start_value), int(end_value), division,
                                                   int(secnbr_value), logs)
        wrp_value = wrpgrp1 if begin == "wrpgrp2" else wrpgrp2
        
        logs.info("File to update '%s'" % output.name())
        # Output the stdout modified hardening
        if not output.modify_file_value(pattern, None, hex(wrp_value)):
            logs.error("Initial pattern (%s) not found" % begin)
        else:
            str_debug = f"Protection variable \"{begin}\" is updated with value \"{hex(wrp_value)}\""
            logs.info(str_debug)
    elif begin == "wrps":
        wrps_group_nb = protected_area // division
        protected_sector_size = (int(secnbr_value) - division) // division
        wrps_str = int(start_value) // division
        wrps_end = wrps_str + protected_sector_size
        logs.info("Flash sectors to set under WRP from %d to %d" % (wrps_str, wrps_end))
        wrps_enable = 0
        wrps_disable = 1
        
        for i in range(wrps_group_nb):
            pattern=output.pattern(begin+ str(i), "=")
            # When using .bat files the variable needs to be set
            
            if (i >= wrps_str) and (i <= wrps_end):
                wrps_state = wrps_enable
            else:
                wrps_state = wrps_disable
            if not output.modify_file_value(pattern, None, str(wrps_state)):
                logs.error("Initial pattern (%s) not found" % begin)
    # Calculation of value for hdp start/end
    elif (begin == "hdp1_start" or begin == "hdp1_end" or begin == "hdp2_start" or begin == "hdp2_end") or (begin == "hdp_start" or begin == "hdp_end"):
        # When using .bat files the variable needs to be set
        pattern=output.pattern(begin, "=")
        hdp1, hdp2 = ACutils.compute_hdp_protection( int(start_value), int(end_value), int(secnbr_value), logs, division)
        if begin in ["hdp1_start", "hdp_start"]:
            hdp_value = hdp1[0]
        elif begin in ["hdp1_end", "hdp_end"]:
            hdp_value = hdp1[1]
        elif begin == "hdp2_start":
            hdp_value = hdp2[0]
        elif begin == "hdp2_end":
            hdp_value = hdp2[1]
        else:
            hdp_value = 0
        if not output.modify_file_value(pattern, None, hex(hdp_value)):
            logs.error("Initial pattern (%s) not found" % begin)
        else:
            str_debug = "Protection variable \"%s\" " % begin +\
            "is updated with value \"%s\"" % hex(hdp_value)
            logs.info(str_debug)
    elif begin == "OEMiLoader_address":
        iLoader_add = 0x08000000 + int(start_value)
        pattern=output.pattern(begin,"=")
        if not output.modify_file_value(pattern, None, hex(iLoader_add)):
            logs.error("Initial pattern (%s) not found" % begin)
        else:
            str_debug = "Protection variable \"%s\" " % begin +\
                    "is updated with value \"%s\"" % hex(iLoader_add)
            logs.info(str_debug)
    else:
        logs.error("Error: OB (%s) not supported" % begin)
    output.save()

@click.option('-fb', '--first_bin', metavar='filename', required=False,
              default="", help='First binary path (Normally secure image)')
@click.option('-o', '--optional_size', '--expected_f_binary_size',
              type=BasedIntAllParamType(AppliCfgLog), required=False,
              default=0, help='Expected first binary size '
                              '(padding will be added if necessary)')
@click.option('-sb', '--second_bin', metavar='filename', required=False,
              default="", help='Second binary path (Normally nonsecure image)')
@click.option('-i', '--input_size', '--expected_s_binary_size',
              type=BasedIntAllParamType(AppliCfgLog), required=False,
              default=0, help='Expected second binary size '
                              '(padding will be added if necessary)')
@click.option('-ob', '--output_bin', required=False,
              default="", help='Define the name of the assembled binary')
@click.option('--vb', default=False, is_flag=True,
              help='Define if debug traces will be displayed')
@click.command(help='Create one image'
                    'More information in the README file')
def oneimage(first_bin, second_bin, input_size, optional_size, output_bin, vb):
    # Get the input paths
    if vb is False:
        logs=ACutils.LOG(True,True)
    else:
        logs=AppliCfgLog
    logs.begin("oneimage")
    logs.array(sys.argv)
    binary_1=ACutils.BIN_FILE(first_bin, logs, True)
    binary_2=ACutils.BIN_FILE(second_bin, logs, True)
    output=ACutils.BIN_FILE(output_bin, logs)

    if optional_size != 0:
        logs.info("Extra padding will be added at the end of the first binary")
        optional_binary = binary_1.data+(optional_size-len(binary_1))*pack("B", 0xff)
    else:
        optional_binary=binary_1.data
    if input_size != 0:
        logs.info("Extra padding will be added at the end of the second binary")
        binary = binary_2.data+ (input_size-len(binary_2))*pack("B", 0xff)
    else:
        binary=binary_2.data

    output.save(optional_binary+binary)
    logs.info("Image assembly success")
    logs.info("Final image size '%s'" % len(output))


@click.argument('infile')
@click.option('-l', '--layout', metavar='filename', default="", required=True,
              help='Location of the file that contains preprocessed macros')
@click.option('-me', '--macro_encrypted', default="", required=True,
               help='Macro symbol for the encryption mode. The symbol'
              'will be searched in the preprocessed file')
@click.option('-mi', '--macro_image', default="", required=True,
               help='Macro symbol for the image number value. The symbol'
              'will be searched in the preprocessed file')
@click.option('-xml', '--xml', metavar='filename', default="", required=True,
              help='Location of the XML file of the Rot Application')
@click.option('-i', '--input_bin', default="", required=True,
              help='Project binary to use as input for the TPC tool')
@click.option('-in', '--input_name',required=True,
              help='Tag <Name> content to select the input binary of the xml file')
@click.option('-on', '--output_name', required=True,
              help='Tag <Name> content to select the output binary of the xml file')
@click.option('-en', '--enc_name', required=True,
              help='Tag <Name> content to select the authentication mode of the xml file')
@click.option('-b', '--begin', required=True,
              help='begin of line to replace in the script file')
@click.option('--vb', default=False, is_flag=True,
              help='Define if debug traces will be displayed')
@click.command(help='Modify the project input/output files in order to use the correct images'
                    'More information in the README file')
def iofile(infile, layout, macro_encrypted, macro_image, xml, input_bin,
           input_name, output_name, begin, enc_name, vb):
    # Compute and return value from macro file
    # 0 non image, 1 (for data that means enable, for app that mean one image cfg)
    # 2 impossible for data images, only available for app
    if vb is False:
        logs=ACutils.LOG(True,True)
    else:
        logs=AppliCfgLog
    logs.begin("iofile")
    logs.array(sys.argv)
    logs.info("Obtaining the input values to get the input/output binary name")
    image_cfg=ACutils.StringValue(logs)
    enc_mode=ACutils.StringValue(logs)
    output=ACutils.SCRIPT_APPLI(infile, logs)
    macrofile=ACutils.SCRIPT_APPLI(layout, logs)
    image_cfg.set(macrofile.get_macro_value(macro_image))
    enc_mode.set(macrofile.get_macro_value(macro_encrypted))
    
    image_cfg.set(get_macro_value(macro_image, layout, logs))
    enc_mode.set(get_macro_value(macro_encrypted, layout, logs))
    is_oneimage = False
    is_nsimage = False
    if int(enc_mode):
        img_enc_mode = "_enc_sign.hex"
        enc_status = "1"
    else:
        img_enc_mode ="_sign.hex"
        enc_status = "0"

    input_bin_path=input_bin.replace("\\","/")
    if "Code" in xml:
        img_type = "app"
        if "_S_" in xml:
            if int(image_cfg) != 2:
                #Configuration with one code image
                logs.info("Nothing to modify. Configuration in one image")
                sys.exit(0)
        elif "_NS_" in xml:
            if int(image_cfg) == 1 and "Binary" in input_bin_path:
                is_oneimage = True
            elif int(image_cfg) == 2 and "NonSecure" in input_bin_path:
                is_nsimage = True
            else:
                if int(image_cfg) == 1:
                    logs.info("Nothing to modify. Configuration in one image")
                    sys.exit(0)
                elif int(image_cfg) == 2:
                    logs.info("Nothing to modify. Configuration in two image")
                    sys.exit(0)
                else :
                    str_debug = "Wrong image configuration '%s'" %image_cfg.getInt() + \
                                "Wrong format for the input path '%s'" %input_bin_path
                    logs.error(str_debug)
        else:
            logs.error("Wrong xml image")
    else:
        img_type = "data"
        if "_NS_" in xml:
            if int(image_cfg) == 0:
                # Configuration with non secure data image disable
                logs.info("Nothing to modify. Secure data image is disable")
                sys.exit(0)
        elif "_S_" in xml:
            if int(image_cfg) == 0:
                # Configuration with secure data image disable
                logs.info("Nothing to modify. Non secure data image is disable")
                sys.exit(0)
        else:
            logs.error("Wrong xml image")

   # Initialize xml object
    logs.info("Modifying input/output binaries from the xml file")
    xml_obj = xml_parser.XML_APPLI(xml, logs)
    xml_obj.get_items("Name")
    logs.info("Search output_name")
    xml_name = xml_obj.get_value(output_name, "Value")
    file_name = xml_name.split("/")[-1] if "/" in xml_name else xml_name
    file_prefix = file_name.split(img_type)[0]
    if is_oneimage :
        replace_ns_pattern = "ns" if file_prefix.startswith("ns") else "_ns"
        if "ns" in file_prefix :
            logs.info("One image configuration replacing the prefix '_ns' ")
            file_prefix = file_prefix.replace(replace_ns_pattern,"")
    if is_nsimage:
        if not "ns" in file_prefix:
            logs.info("Two image configuration adding the prefix 'ns' ")
            file_prefix  += "ns_"

    new_file_name =  file_prefix + img_type + img_enc_mode

    # Save xml modification
    save_modifications = False # If one value is True then the file must be saved
    if file_name != new_file_name:
        logs.info("Replacing '%s' " % file_name + "by '%s'" % new_file_name)
        new_xml_name = xml_name.replace(file_name, new_file_name)
        # Modify output file
        logs.info("Modify Binary output name by '%s'" % new_xml_name)
        xml_obj.modify_value(output_name, "Name", "Value", new_xml_name)
        save_modifications |= xml_obj.do_save_file
    else :
        logs.info("The binary output content has the correct file")

    logs.info("Modify Binary input path by '%s'" % input_bin_path)
    xml_obj.modify_value(input_name, "Name", "Value", input_bin_path)
    save_modifications |= xml_obj.do_save_file
    logs.info("%s encryption mode " % ("Enable" if int(enc_mode) else "Disable"))
    xml_obj.modify_value(enc_name, "Name", "Enable", enc_status)
    save_modifications |= xml_obj.do_save_file
    if save_modifications:
        xml_obj.do_save_file = True
    xml_obj.save_file()

    logs.info("Modifying input binaries from the script file", logs)
    # When using .bat files the variable needs to be set
    pattern=output.pattern(begin,"=")

    new_value = new_file_name
    if not output.modify_file_value(pattern, None, new_value):
        logs.error("Initial pattern (%s) not found" % begin)
    else:
        output.save()

@click.argument('infile')
@click.option('-xml', '--xml', metavar='filename', default="",
              help='Location of the file that contains the RoT configuration')
@click.option('-nxml', '--xml_name', default="",
              help='Tag <Name> content in the xml file. If the parameter is'
                   'found the tag value content will be used')
@click.option('-v', '--value', default="",type=str,
              help='New value to replace in file')
@click.option('-var', '--variable', required=True,
              help='Name of the variable whose value will be modified')
@click.option('-sft', '--shift', type=BasedIntAllParamType(AppliCfgLog), default=None,
              help='Shift to define value position')
@click.option('-del', '--delimiter', type=str,
              help='String delimiter to determine where variable end')
@click.option('--upper', default=False, is_flag=True,
              help='Define value output in capital letters')
@click.option('--str', 'str_in', default=False, is_flag=True,
              help='string replacement, do not interpret the value, authorized : alphanumeric character/.+*\\ and optional enclosing parenthesis')
@click.option('--vb', default=False, is_flag=True,
              help='Define if debug traces will be displayed')
@click.command(help='Modify the variable value of an external file')
def modifyfilevalue(infile, xml, xml_name, value, variable, shift, delimiter,
                    upper, str_in, vb):
    if vb is False:
        logs=ACutils.LOG(True,True)
    else:
        logs=AppliCfgLog
    # Verify if the input files exist
    logs.begin("modifyfilevalue")
    logs.array(sys.argv)
    output=ACutils.SCRIPT_APPLI(infile, logs)
    
    search_value=output.pattern(variable,delimiter)

    if str_in :
        # Define pattern to search
        # When using .bat files the variable needs to be set
        logs.info("File to update '%s'" % output.name())
        if not output.modify_file_strvalue(search_value, None, value):
           logs.error(f"File variable ({infile}) not modified, {variable} not set")
        else:
            str_debug = f"Variable \\{variable}\\ in script is updated with value \\{value}\\"
            logs.info(str_debug)
            output.save()
        sys.exit(0)
    
    elif shift is not None:
        new_hex_value=ACutils.StringValue(logs)
        logs.info("Searching old variable %s value in file '%s'" % (variable, output.name()))
        new_hex_value.set(output.get_file_value(search_value, logs))
        lenvalue=len(new_hex_value.getHex())
        new_hex_value.modify_hex_value(int(xml_value), shift)
        choosenValue = new_hex_value.getHex(size=lenvalue, upper_format=upper)
    else :
        xml_value=ACutils.StringValue(logs)
        # Define value or values to use
        if os.path.isfile(xml):  # Recover value from xml file
            if not xml_name:
                logs.error("--xml_name argument was not defined")
            # Initialize xml object
            xml_obj = xml_parser.XML_APPLI(xml, logs)
            xml_obj.get_items("Name")
            logs.info("Search xml_name")
            xml_value.set(xml_obj.get_value(xml_name, "Value"))
            if xml_value.is_none():
                logs.error("Not value found for '%s'. Output file can not be modified" % xml_name)
        elif value is None:
            logs.error("Value option or xml file were not defined")
        else:
            xml_value.set(value)
        if xml_value.is_none():
            logs.error("No data to modify")
        choosenValue = xml_value.getHex()
        logs.info("File to update '%s'" % output.name())

    if not output.modify_file_value(search_value, None, choosenValue):
        logs.error("Variable (%s) not found in the file" % variable)
    else:
        str_debug = f"Variable \\{variable}\\ in file will be updated with value \\{choosenValue}\\"
        logs.info(str_debug)
        output.save()

@click.argument('infile')
@click.option('-o', '--offset', type=BasedIntAllParamType(AppliCfgLog), default=0,
              help='Index start where the HASH will be added')
@click.option('-t', '--tail', default=None,
              help='Add tail after Hash (in hex format and without 0x)'
                    'Exemple : AABBCCDDEEFF')
@click.option('-h', '--header', default=None,
              help='Add before Hash (in hex format and without 0x)'
                    'Exemple : AABBCCDDEEFF')
@click.option('-i', '--input_data', metavar='filename', default="", required=True,
              help='Data which a Hash 256 will be calculate')
@click.option('-d', '--da_bin_file', metavar='filename', default="", required=True,
              help='depend on password option')
@click.option('--vb', default=False, is_flag=True,
              help='Define if debug traces will be displayed')
@click.option('-p','--password', default=False, is_flag=True,
              help='')
@click.command(help='Calculate the HASH256 of the content of a file. '
                    'The calculated hash will be stored in another binary file at '
                    'the location defined by the user.'
                    'More information in the README file')
def hashcontent(infile, input_data, da_bin_file, header, tail, offset, password, vb):
    if vb is False:
        logs=ACutils.LOG(True,True)
    else:
        logs=AppliCfgLog
    logs.begin("hashcontent")
    logs.array(sys.argv)
    # Function input verification
    #if offset and create :
    #    logs.error("Offset and Create option can not be used at the same time")
    #if not offset and not create:
    #    logs.error("At least Offset and Create option must be defined")
    if header and tail:
        logs.error("can't have a header and a tail in the same time")

    # Opening data input
    shafile=ACutils.BIN_FILE(infile, logs, False)
    output=ACutils.BIN_FILE(input_data, logs, True)
    logs.info("Getting data from %s" % output.name())
    output.read()
    
    #Compute HASH256
    shafile.compute_sha256(output)
    # Create binary
    logs.info("Creating the binary '%s'" %shafile.name())
    shafile.save()
    #Format header information
    if da_bin_file:
        if tail:
          # Format user password size in little endian
          if tail.startswith("0x"):
            tail = tail[2:]
          if len(tail) %2!=0:
            tail="0"+tail
          da_fin_file=ACutils.BIN_FILE(da_bin_file, logs)
          logs.info("Creating the binary '%s'" %da_fin_file.name())
          if password:
              da_fin_file.save(output.data+bytearray.fromhex(tail))
          else:
              da_fin_file.save(shafile.data + len(output).to_bytes(length=4, byteorder='little') + bytearray.fromhex(tail))
        elif header:
          if header.startswith("0x"):
            header = header[2:]
          if len(header) %2!=0:
            header="0"+header
          da_fin_file=ACutils.BIN_FILE(da_bin_file, logs)
          logs.info("Creating the binary '%s'" %da_fin_file.name())
          if password:
              da_fin_file.save(bytearray.fromhex(header)+output.data)
          else:
              da_fin_file.save(bytearray.fromhex(header)+ len(output).to_bytes(length=4, byteorder='little')+shafile.data)

@click.group(invoke_without_command=True)
@click.option('-v', '--version', is_flag=True, help="version of AppliCfg tool.")
def AppliCfg(version):
    """AppliCfg is a tool for ROT projects, that allow to automatically configure projects files according to ROT configuration."""
    if(version): click.echo(SOURCE_VERSION)
    if sys.version_info < MIN_PYTHON_VERSION:
        AppliCfgLog.error("Python %s.%s or newer is required by AppliCfg."% MIN_PYTHON_VERSION)
    pass

AppliCfg.add_command(xmlen)
AppliCfg.add_command(xmlval)
AppliCfg.add_command(xmlparam)
AppliCfg.add_command(xmlname)
AppliCfg.add_command(linker)
AppliCfg.add_command(obscript)
AppliCfg.add_command(definevalue)
AppliCfg.add_command(setdefine)
AppliCfg.add_command(keyconfig)
AppliCfg.add_command(flash)
AppliCfg.add_command(setob)
AppliCfg.add_command(oneimage)
AppliCfg.add_command(iofile)
AppliCfg.add_command(hashcontent)
AppliCfg.add_command(modifyfilevalue)
AppliCfg.add_command(sectorerase)
