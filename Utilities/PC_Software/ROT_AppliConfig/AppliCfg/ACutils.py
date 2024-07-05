import sys
import os.path
import re
import time
import hashlib
from AppliCfg import macro_parser
from array import array


class AppliCFGException(Exception):
    def __init__(self, message:str):
        self.message = message
        super().__init__(message)

    def __str__(self):
        return self.message

class LOG:
    def __init__(self, quiet:bool=True, have_timestamp:bool=True):
        """ quiet : quiet mode, do not print parameter 
            have_init : horodatage
        """
        self.have_init=have_timestamp
        self.quiet=quiet
    def __init_str(self)->str:
        if self.have_init and not self.quiet:
            return str(time.strftime("%c")) + " : "
        else:
            return ""
    def __clean(self, msg:str)->str:
        return msg.replace(r"\n","").replace(r"\r","")
    def info(self, msg)->None:
        if not self.quiet:
            initstr=self.__init_str()
            #utf8stdout = open(1, 'w', encoding='utf-8', closefd=False) # fd 1 is stdout
            #print(initstr+"[INF] " + self.__clean(msg), file=utf8stdout, flush=True)
            print(initstr+"[INF] " + self.__clean(msg))
    def error(self, msg)->None:
        initstr=self.__init_str()
        sys.exit(initstr+"[ERR] " + self.__clean(msg))
    def begin(self, message:str)->None:
        self.info("############## Executing "+message+" command ##############")
    
    def array(self, command:array)->None:
        if not self.quiet :
            new_cmd = []
            for cmd in command:
                cmd = cmd.split("\\")[-1] if "\\" in cmd else cmd
                new_cmd.append(cmd)
            self.info(" ".join(new_cmd))

class StringValue:
    def __init__(self, logs:LOG):
        self._logs=logs
        self.__value=None
    def set(self,value=None, order='little'):
        """Transform input data into integer notation"""
        try:
            if value is None:
                self.__value=None
            if type(value) is int:
                self.__value=value
            elif type(value) is str:
                if value[:2].lower() == "0x":
                    self.__value=int(value[2:], 16)
                elif value[:1] == "0":
                    self.__value=int(value, 8)
                elif value.isdigit():
                    self.__value=int(value, 10)
                else:
                    self.__value=None
            elif type(value)==float:
                self.__value=int(value)
            elif type(value)==bytearray or type(value)==bytes:
                self.__value=int.from_bytes(value, byteorder=order)
        except Exception as e:
            self._logs.info(str(e))
    def is_none(self)->bool:
        return (self.__value is None)
    def __int__(self)->int:
        if self.__value is not None:
            return self.__value
        else:
            return 0
    def getInt(self, size=None)->str:
        if self.__value is not None:
            s=str(self.__value)
            if size is not None and len(s) <= size :
                return s.zfill(size)
            else:
                return s
        else:
            return "0"
    def half(self):
         if self.__value is not None:
            self.__value//=2
    def getHex(self, size=None, upper_format=True)->str:
        """
            Transform value to hex number in upper mode
        Parameters:
            value - Value to transform
        Returns:
            hex_value - Value in hex mode
        """
        if self.__value is not None:
            hex_value=hex(self.__value)[2:]
        else:
            hex_value="0"
        if size is not None and len(hex_value) <= size :
            hex_value = hex_value.zfill(size)
        if upper_format :
            hex_value = hex_value.upper()
        return "0x" + hex_value
    def modify_hex_value(self, hex_to_replace:int, shift_value:int) :
        """ Modify the value of a hex number
            Parameters:
              hex_to_replace : Value to replace
              shift_value : Shift of the hex to replace
            """
        hex_value=hex(self.__value)[2:]
        replace_val = hex_to_replace << shift_value
        replace_mask = "".join([ "0" if value == "0" else "F" for value in hex(replace_val)[2:]])
        full_inv_mask = "".join([ "F" for value in hex_value])
        full_inv_mask_val = int(full_inv_mask,16)
        replace_mask_val = int(replace_mask,16)
        final_mask = full_inv_mask_val ^ replace_mask_val
        self.__value = (final_mask & self.__value) | replace_val

class MANAGE_FILE:
    def __init__(self, filename, logs:LOG, launch=True):
        """By default, read the file"""
        self._filename=filename
        self._logs=logs
        if launch:
            if not os.path.isfile(filename):
                self._logs.error("File (%s) not found" % self.name())
            else:
                self.read()
    def name(self)->str:
        return self._filename.split("\\")[-1] if "\\" in self._filename else self._filename
    def read(self):
        pass

class SCRIPT_APPLI(MANAGE_FILE):
    def __init__(self, filename, logs:LOG, checkIfExist=True):
        """By default, read the file"""
        super().__init__(filename, logs, checkIfExist)

    def is_batch(self)->bool:
        if self._filename is not None and self._filename.endswith('.bat'):
            return True
        else:
            return False
    def is_shell(self)->bool:
        if self._filename is not None and self._filename.endswith('.sh'):
            return True
        else:
            return False
    def is_header(self)->bool:
        if self._filename is not None and self._filename.endswith('.h'):
            return True
        else:
            return False
    def is_icf(self)->bool: 
        if self._filename is not None and self._filename.endswith('.icf'):
            return True
        else:
            return False
    def is_sct(self)->bool:
        if self._filename is not None and self._filename.endswith('.sct'):
            return True
        else:
            return False
    def is_c(self)->bool:
        if self._filename is not None and self._filename.endswith('.c'):
            return True
        else:
            return False
    def is_ld(self)->bool:
        if self._filename is not None and self._filename.endswith('.ld'):
            return True
        else:
            return False    
    def read(self)->bool:
        try:
            f = open(self._filename, "r")
            self.__lines = f.readlines()
            f.close()
            self._logs.info("open file (%s)" % self.name())
            return True
        except:
            self._logs.info("unable to open file (%s)" % self.name())
            return False
    def save(self)->bool:
        try:
            f = open(self._filename, "w")
            f.write("".join(self.__lines))
            f.close()
            self._logs.info("File (%s) saved" % self.name())
            return True
        except:
            self._logs.info("unable to save file (%s)" % self.name())
            return False
    def pattern(self, name:str, delimiter:str="", parameters:bool=False)->str:
        pattern=""
        if not parameters:
            pattern=r"^\s*"
            if self.is_batch():
                pattern+= r"set\s+"
            elif self.is_header() or self.is_c() or self.is_sct():
                pattern+= r"#define\s+"
        pattern += name
        if delimiter is not None and delimiter.strip()!="":
            pattern += r"\s*"+delimiter+r"\s*"
        else:
            pattern+=r"\s+" #default delimiter is space
        return pattern
        
    def linker_pattern(self, name:str)->str:
        if self.is_ld():
            return name + r"\s*=\s*"
        elif self.is_icf():
            return r"^\s*define\s+(?:\w+\s+)?symbol\s+" + name + r"\s*=\s*"
        elif self.is_sct():
            return r"^\s*#define\s+" + name + r"\s+"
        else:
            self._logs.error("Linker value not recognized")

    def get_file_value(self, pattern_line:str, search_option:str="\"?((0x[0-9a-fA-F]+)|([0-9]+))\"?", occurrence=1)->str:
        """
        Get the value of an existing variable in a file
        Parameters:
            pattern_line - Pattern to choose the line to be modified
            search_option = Search regex option (By default search hex an decimal numbers)
        """
        # Modify value
        # Read and modify a specific line
        regfv=re.compile(pattern_line+search_option)
        for i, line in enumerate(self.__lines):
            # Search line to modify
            replace_resp=regfv.search(line)
            if replace_resp is not None:
                # Get the value to be replaced
                if occurrence==1:
                    return replace_resp.group(1)
                else:
                    occurrence-=1
        self._logs.error("Pattern not found")
        return ""
        
 
    def toggle_comment_line(self, name, action="comment")->bool:
        """ 
        Modify the value of an existing variable in a file 
        Parameters:
            name -   Define variable to comment or uncomment
            action - comment or uncomment the file line
        Returns:
            line_is_modified - Boolean status (True line with correction action)
        """
        line_is_modified = False
        reqdef1=re.compile(r'#define\s* (\w+)\s* (\w+)')
        reqdef2=re.compile(r'#define\s* (\w+)')
        for i, line in enumerate(self.__lines):
            # Search define with value
            pattern_in_line = reqdef1.search(line)

            if pattern_in_line is None:
                pattern_in_line = reqdef2.search(line)

            if pattern_in_line:
                if name in pattern_in_line.group(1):
                    str_idx, end_indx = pattern_in_line.span()
                    if action == "comment":
                        if str_idx == 0:
                            new_line = "/*" + \
                                       line[str_idx:end_indx] + \
                                       "*/" + line[end_indx:]
                            self._logs.info("The #define variable is now commented")
                            line_is_modified = True
                        elif "/*" in line[:str_idx]:
                            self._logs.info("The #define variable is already commented")
                            new_line = line
                        else:
                            new_line = line[:str_idx] + "/*" + \
                                line[str_idx:end_indx] + "*/" + line[end_indx:]
                            self._logs.info("The #define variable is now commented")
                            line_is_modified = True
                    else:
                        if str_idx == 0:
                            new_line = line
                            self._logs.info("The variable is already defined")
                        elif "/*" in line[:str_idx]:
                            new_line = line.replace(
                                "/*", "", 1).replace("*/", "", 1)
                            self._logs.info("The variable is now defined")
                            line_is_modified = True
                        else:
                            new_line = line
                            self._logs.info("The variable is already defined")
                    if new_line in self.__lines[i]:
                        self._logs.info("The modification is not necessary")
                        # The line has te correct define configuration
                        # The file modification is not necessary
                        return False
                    else : 
                        self.__lines[i] = new_line
                        # Line to modify has been found (modify only the first line
                        # with the pattern )
                        break
        return line_is_modified
    
    def modify_file_value(self, pattern_line:str, search_value, new_value, cpt=1)->bool:
        """
        Modify the int value of an existing variable in a file 
        Parameters:
            pattern_line - Pattern to choose the line to be modified
            search_value - value to search in the line if present. Every present value is changed if None
            new_value - New value
            cpt: counter, change the value cpt only, if 0 then all
        Returns:
            value_is_modified - Boolean status (New value is now present)
        """
        # Modify value
        # Read and modify a specific line
        value_is_modified=False
        nv=StringValue(self._logs)
        nv.set(new_value)
        rv=StringValue(self._logs)
        sv=StringValue(self._logs)
        if search_value is not None:
            sv.set(search_value)
            if int(nv)==int(sv):
                self._logs.info("The modification is not necessary")
                self._logs.info("The variable has already the correct value (%s)" % str(new_value))
                return True
            pattern_line+=r"\(?((0x[0-9a-fA-F]+)|([0-9]+))\)?"
        else:
            pattern_line+=r"(\(?((0x[0-9a-fA-F]+)|([0-9]+))\)?|$)" #the double parenthesis force to get value only
        reqextract = re.compile(pattern_line) #To know the value to replace, extract the number from line
        for i, line in enumerate(self.__lines):
            # Search line to modify
            replace_resp=reqextract.search(line)
            if replace_resp:
                if cpt<=1:
                    readvalue=replace_resp.group(1)
                    str_idx, end_indx = replace_resp.span()
                    if readvalue is not None and readvalue!="":
                        rv.set(readvalue)
                        if int(rv) == int(nv):
                            self._logs.info("The modification is not necessary")
                            self._logs.info("The variable has already the correct value (%s)" % str(new_value))
                            return True
                        if search_value is None or int(sv)==int(rv):#if readvalue == search value, search value is a restrict access value
                            cmd_len=0
                            if readvalue[0]=="(":
                                new_value="("+str(new_value)+")"
                            else:
                                if line[end_indx-1]==")":
                                    cmd_len=-2
                                else:
                                    new_value=str(new_value)
                            cmd_len+=end_indx-len(readvalue)
                            str_to_replace = line[cmd_len:end_indx].replace(readvalue, new_value).replace("\n","").replace("\r",'')
                            self.__lines[i] = line[str_idx:cmd_len] + str_to_replace + line[end_indx:]
                            self._logs.info("The variable value is modified to %s" % new_value)
                            if cpt==1:
                                return True
                            else:
                                value_is_modified=True
                    elif search_value is None:
                        debut=re.sub("\r?\n","",line[:end_indx])
                        self.__lines[i]=debut+ new_value + line[end_indx:]+"\n"
                        if cpt==1:
                            return True
                        else:
                            value_is_modified=True
                else:
                    if cpt>0:
                        cpt-=1
        if not value_is_modified:
            self._logs.info("No pattern found in file")
        return value_is_modified
    
    def modify_file_strvalue(self, pattern_line:str, search_value, new_value)->bool:
        if type(new_value)==str:
            new_value=new_value.strip()
            if search_value is None:
                pattern_line+=r"(\(?[\w+|+|*|\/|//|.|\d]*\)?)" #capture parenthesis if present, string string+integer, string+hex, string*value
            elif search_value == new_value:
                self._logs.info("The modification is not necessary")
                self._logs.info("Same value than those to replace" % new_value)
                return True
            else:
                pattern_line+=f"({search_value})"
            reqextract = re.compile(pattern_line) #To know the value to replace, extract the number from line
            for i, line in enumerate(self.__lines):
               replace_resp=reqextract.search(line)
               if replace_resp:
                    readvalue=replace_resp.group(1)
                    if readvalue == new_value:
                        self._logs.info("The modification is not necessary")
                        self._logs.info("The variable has already the correct value (%s)" % str(new_value))
                        return True
                    else:
                        str_idx, end_indx = replace_resp.span()
                        if (line[end_indx-1]==")"):
                            cmd_len=end_indx-len(readvalue)-1
                        else:
                            cmd_len=end_indx-len(readvalue)
                        str_to_replace = line[cmd_len:end_indx].replace(readvalue, str(new_value)).replace("\n","").replace("\r",'')
                        self.__lines[i] = line[str_idx:cmd_len] + str_to_replace + line[end_indx:]
                        self._logs.info("The variable value is modified to %s" % new_value)
                        return True
            self._logs.info("No pattern found in file")
            return False
        else:
            self._logs.error("value have to be a string")

    def get_macro_value(self, macro:str, indice:int=0)->int:
        """ 
        Search one variable in the macro document and return an integer value 
        Parameters:
            macro  - variable to search and get the value
            vb - Debug option (True or False)
            indice - indice of macro, default 0 (the first, -1=the last)
        Returns :
            value - value found (None if no value)
        """
        if self.is_header() or self.is_sct():
            pattern = r"^\s*#define\s+%s\s*(.*)"
        elif self.is_icf():
            pattern = r"^define symbol\s+%s\s*=\s*(.*)"
        else:
            pattern=r"^\s*%s\s*=\s*(.*)"
        image_value_re = re.compile(pattern %macro)
        self._logs.info(f"Opening the '{self._filename}' file")
        value=StringValue(self._logs)
        macroValue = None
        for line in self.__lines:
            line=line.replace("(","")
            line=line.replace(")","")
            m = image_value_re.match(line)
            if m is not None:
                try:
                    macroValue = macro_parser.parse_and_sum(m.group(1)) #not eval because C syntax /* or ;, etc.
                except Exception as e:
                    self._logs.error(str(e))
        if macroValue is not None:
            self._logs.info(f"The value computed for '{macro}' is '{hex(macroValue)}'")
            return macroValue
        else:
            self._logs.info(f"Macro variable ({macro}) has no value in {self.name()}")
            return None

    def save(self):
        if hasattr(self,'_SCRIPT_APPLI__lines'):
            self._logs.info("Saving file modification")
            with open(self._filename, "w") as f:
                f.write("".join(self.__lines))
                f.close()
        else:
            self._logs.info("No data in %s"%self.name())
    def logs(self):
        if hasattr(self,'_SCRIPT_APPLI__lines'):
            for i, line in enumerate(self.__lines):
                self._logs.info(str(i)+" "+line)

class BIN_FILE(MANAGE_FILE):
    def __init__(self, filename:str, logs:LOG, checkIfExist:bool=False):
        """By default, do not read the file. Because file is not created yet"""
        super().__init__(filename, logs, checkIfExist)

    def modify_bin_content(self,offset:int, val:bytes)->bool:
        """ Modify or add byte in the  binary file. If offset is empty, 
            one byte is added at the end of the file otherwise the byte is added at 
            offset position"""
        if hasattr(self,'data'):
            if offset<len(self.data):
                self.data[offset]=val
                return True
            else:
                return False
        else:
            self._logs.error("No data read")
    def read(self):
        if os.path.isfile(self._filename):
            with open(self._filename, 'rb') as f:
                self.data=bytearray(f.read())
                f.close()
            self._logs.info("read  %s" % self.name())
        else:
            self._logs.info("File (%s) not exist" % self.name())
            self.data=b''
    def compare(self, s:bytearray)->bool:
        try:
            for i in range(1, len(self.data)):
                if self.data[i]!=s[i]:
                    return False
        except:
            return False
        finally:
            return True
       
    def save(self, data=None)->None:
        try:
            with open(self._filename, 'wb') as f:
                if data is None:
                    if hasattr(self,'data'):
                        f.write(self.data)
                else:
                    f.write(data)
                    self.data=data
        except FileNotFoundError as e:
            absp=os.path.dirname(self._filename)
            if not os.path.exists(absp):
                self._logs.error(f"Path {absp} not exists, impossible to save file")
            else:
                self._logs.error(str(e))
        except Exception as e:
            self._logs.error(str(e))

    def __len__(self)->int:
        if hasattr(self,'data'):
            return len(self.data)
        else:
            return 0
    def compute_sha256(self, data:'BIN_FILE')->None:
        """ Compute the sha256. Return the hex or byte value, depending the
           value of get_hex_val """
        if hasattr(data,'data'):
            sha256_list = bytearray()
            #Compute Hash256
            hash256 = hashlib.sha256()
            hash256.update(data.data)
            hash256 = hash256.hexdigest();
            #Transform hash hex values to bytes
            self._logs.info("SHA256 computed : " + hash256)
            for i in range(0,len(hash256),2):
              sha256_list.append(int(hash256[i:i+2],16))
            
            self.data=sha256_list
        else:
            self._logs.info("no data in " % data.name())

    def gethex()->str:
        return self.data.hex()


#########################################
# business method
#########################################
def compute_condition(condition:str, values:array, logs:LOG)->bool:
    """
        Compute a conditional expression (string to integer)
    Parameters:
        condition - Value to transform
        value - Value found in the input file. The value must be integer
        vb - Debug option (True or False)
    Returns:
        conditional_value : Return True or False
    """
    # Replace constants string patterns to real values
    logs.info("Condition to be computed '%s' " %condition)
    # Replace value string pattern to real values
    i=1
    for value in values:
        if len(values)==1:
            value_pattern = "value"
        else:
            pattern = "val%s" %i
            i+=1
        condition = condition.replace(pattern, value.getInt())
    logs.info("Condition patterns were replaced to '%s' " %condition)
    try :
        return eval(condition)
    except Exception as e:
        logs.error("Evaluation condition error '%s'" % str(e))


def compute_expression(expression:str, values:array, constants, logs:LOG)->int:
    """
        Compute a mathematical expression (string to integer)
    Parameters:
        expression - Value to transform
        value - Array of values found in the input file. The value must be integer
        constants - Constant list. The constant must be integers
        vb - Debug option (True or False)
    Returns:
        computed_value : Computed value in integer format
    """
    # Replace constants string patterns to real values
    logs.info("Expression to be computed '%s' " %expression)
    i = 0
    for cons in constants:
        i += 1
        pattern = "cons%s" %i
        expression = expression.replace(pattern, str(cons))
    # Replace value string pattern to real values
    if values is None or len(values)==0:
        logs.error("have to compute a value at least")
        return
    elif len(values)==1:
        new_value =str(int(values[0]))
        """ use "value" as search pattern for the single val
        to keep compatibility with all script using 
        a single value , then try replacement with val1 if new syntax is used
        """
        value_pattern = "value" if "value" in expression else "val1"
        expression = expression.replace(value_pattern, new_value)
    else:
        i = 0
        for value in values:
            i +=1
            pattern = "val%s" %i
            expression = expression.replace(pattern, value.getInt())
    logs.info("Expression patterns were replaced to '%s' " %expression)
    try :
        return int(eval(expression))
    except Exception as e:
        logs.error("Evaluation expression error '%s'" % str(e))

def stirot_compute_wm(code_primary_offset:int, secure_area_size:int, 
                      pagesize:int, bankSize:int, logs:LOG)->dict:
    """
    Compute the value of the OptionByte "Secure WaterMark". Two values are
    necessary to compute this protection :
    Parameters:
        code_primary_offset - code primary offset
        secure_area_size - secure area size
        pagesize - Board page size (Defined in the reference manual)
        bankSize - Banks size for HDP protection (Defined in the reference manual)
        vb - Debug option (True or False)
    Returns:
        wm_config - Dictionary with Watermarks values
    """
    wm_config = {}
    if not isinstance(bankSize, int):
        raise AppliCFGException("bankSize is not an int, it's a %s" % type(bankSize))
    if not isinstance(secure_area_size, int):
        raise AppliCFGException("secure_area_size is not an int, it's a %s" % type(secure_area_size))
    if not isinstance(pagesize, int):
        raise AppliCFGException("pagesize is not an int, it's a %s" % type(pagesize))
    if not isinstance(code_primary_offset, int):
        raise AppliCFGException("code_primary_offset is not an int, it's a %s" % type(code_primary_offset))
    page_number = (bankSize // pagesize)-1
    secure_sector_size = (secure_area_size // pagesize)-1

    if secure_sector_size < 0:
        logs.error("Wrong secure area size '%s'" % hex(secure_sector_size))
    # Compute WM's start and end protections
    wm_str=code_primary_offset // pagesize
    wm_end=wm_str + secure_sector_size
    # Protection only in bank 1
    s=StringValue(logs)
    if wm_end <= page_number:
        wm_config["SECWM1_STRT"] = hex(wm_str)
        wm_config["SECWM1_END"] = hex(wm_end)
        wm_config["SECWM2_STRT"] = hex(page_number)
        wm_config["SECWM2_END"] = "0x0"
    else:
        if int(wm_str) <= page_number:
            # Protection between bank 1 and bank 2
            wm_config["SECWM1_STRT"] = hex(wm_str)
            wm_config["SECWM1_END"] = hex(page_number)
            wm_config["SECWM2_STRT"] = "0x0"
            wm_config["SECWM2_END"] = "0x0"
        else:
            # Disable protection in the bank 1
            wm_config["SECWM1_STRT"] = hex(page_number)
            wm_config["SECWM1_END"] = "0x0"
            # Protection in bank 2
            wm_str -= page_number# + 1
            #wm_end -= 1 
            wm_config["SECWM2_STRT"] = hex(wm_str)
        wm_end -= page_number
        wm_config["SECWM2_END"] = hex(wm_end)

    return wm_config


def compute_wrp_protections(wrp_address_start:int, wrp_address_end:int, wrp_sector_sz:int,
                            sector_nb:int, logs:LOG, sector_size:int=0x2000):
    """
        Compute Sector group write-protection (WRPSG). Protecting up to 
                (sector_nb/(wrp_sector_size/sector_size)) groups of 
                (wrp_sector_size/sector_size) sectors 
    Parameters:
        wrp_address_start - Begin address of wrp protection
        wrp_address_end - End address of wrp protection
        wrp_sector_sz -Size of the wrp group
        sector_nb - Board sector number in one bank
        vb - Debug option (True or False)
        sector_size - Board sector size
    Returns:
        wrpsg1 - Write sector group protection for Bank 1
        wrpsg2 - Write sector group protection for Bank 2
    """
    # Transform initial values to decimal
    logs.info("Computing the wrp protection."+ \
             " See the reference manual for more information")
    #sector_sz = int(sector_size, 16)
    # Compute the start and end sectors (with the wrp group size)

    wrp_start = int(wrp_address_start / wrp_sector_sz)
    wrp_end = int(wrp_address_end / wrp_sector_sz)
    # Total number of sectors group in bank 1 and 2
    wrp_group_nb = (
        int((sector_nb + 1) / int(wrp_sector_sz / sector_size)) * 2) - 1

    wrp = ""
    for i in range(wrp_group_nb, -1, -1):
        # 0: the group is write protected; 1: the group is not write protected
        wrp += "0" if wrp_start <= i <= wrp_end else "1"

    # Transform binary to dec
    wrpg1_end = int((wrp_group_nb + 1)/2)
    try:
        wrpsg1 = int(wrp[:wrpg1_end], 2)
        wrpsg2 = int(wrp[wrpg1_end:], 2)
    except Exception as e:
        logs.error("group:"+str(wrp_group_nb)+" "+str(e))
        raise AppliCFGException(str(e))
    return wrpsg1, wrpsg2


def compute_hdp_protection(hdp_address_start:int, hdp_address_end:int, sector_nb:int, logs:LOG, hdp_sector_sz:int=0x2000):
    """
        Compute the HDP protection
    Parameters:
        hdp_address_start - Begin address of hdp protection
        hdp_address_end - End address of hdp protection
        sector_nb - Board sector number in one bank
        vb - Debug option (True or False)
        hdp_sector_sz - Board sector size (Defined in the reference manual)
    Returns:
        hdp1 - Tuple with start and and values
        hdp2 - Tuple with start and and values
    """
    # Compute the start and end sectors (with the wrp group size)
    logs.info("Computing the hdp protection." + \
            " See the reference manual for more information")
    #hdp_sector_sz = int(sector_size, 16)
    hdp_start = int(hdp_address_start / hdp_sector_sz)
    hdp_end = int(hdp_address_end / hdp_sector_sz)
    # Compute sectors to protect
    if hdp_start > sector_nb:
        hdp1 = sector_nb, 0
        hdp2 = sector_nb - hdp_start, sector_nb - hdp_end
    else:
        if hdp_end > sector_nb:
            hdp1 = hdp_start, sector_nb
            hdp2 = 0, hdp_end - sector_nb - 1
        else:
            hdp1 = hdp_start, hdp_end
            hdp2 = sector_nb, 0
    return hdp1, hdp2

def compute_sector_area(sector_address_start:int, area_size:int, logs:LOG, sector_size:int)->tuple:
    """
        Compute the HDP protection
    Parameters:
        hdp_address_start - Begin address of hdp protection
        hdp_address_end - End address of hdp protection
        sector_nb - Board sector number in one bank
        vb - Debug option (True or False)
        sector_size - Board sector size (Defined in the reference manual)
    Returns:
        hdp1 - Tuple with start and and values
        hdp2 - Tuple with start and and values
    """
    offset_1=0x70000000
    offset_2=0x90000000

    if (offset_1 & sector_address_start == offset_1):
        sel_offset=offset_1
    elif (offset_2 & sector_address_start == offset_2):
        sel_offset=offset_2
    else:
        LOG.error("Sector address start is unknown '%s'" % sector_address_start)

    if area_size < sector_size:
        decrement = 0
    else:
        decrement = 1

    # Compute the start and end sectors (with the wrp group size)
    logs.info("Computing the erase slot area.")
    slot_sector_sz = sector_size
    logs.info("Sector size %s" % hex(sector_size))
    slot_start = (sector_address_start - sel_offset) //  slot_sector_sz
    logs.info("Slot start %s" % hex(slot_start))
    slot_end = (sector_address_start- sel_offset + area_size) // slot_sector_sz - decrement
    logs.info("Slot end %s with sector_size %s" % (hex(slot_end),hex(slot_sector_sz)))
    # Compute sectors to erase
    erase = int(slot_start), int(slot_end)
    return erase
  


