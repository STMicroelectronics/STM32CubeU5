#-------------------------------------------------------------------------------
# Copyright (c) 2019, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

#A script to fill out the "toctree" file lists in index.rst.in to put design
#documents into the correct section based on their state.
#
#First rst files are parsed under a defined directory, to identify document
#status based on value of the ":Status:" field. Each state will have a list
#being generated to which matching documents are added. These lists are used
#then to fill template values using configure_file()
#
#Functions are used whenever possible to avoid global variable name space
#pollution.
#

Include(CMakeParseArguments)

#This function will search for .rst files in a given directory, read them and
#check if the “:Status:” field is defined in them. Then will add each file to a
#list with a name matching the status value.
#See the definition of _STATE_VALUES below for a list of valid state values.
#Files with missing or invalid state value will be placed on the "unknown" list.
#State value comparison is case insensitive.
#
#The output lists will have the prefix specified in the PREFIX parameter.
#
#Inputs:
#    DIR    Directory to scan for rst files.
#    PREFIX The prefix of output list variables.
#
#Outputs:
#   <prefix>_<state>       - list; will hold all files with a valid state value.
#   <prefix>_unknown       - list; all files with missing or invalid state
#                            value.
#Examples
#   sphinx_categorize_rst(DIR ${TFM_ROOT_DIR}/docs/design_documents
#                          PREFIX "DESIGN_DOCS")
#
function(sphinx_categorize_rst)
	#Valid state values. "unknown" is used as a quard to detect invalid status
	#values.
	set(_STATE_VALUES "draft" "rejected" "accepted" "detailed" "unknown")

	#No option (on/off) arguments
	set( _OPTIONS_ARGS )
	#Single option arguments (e.g. PROJ_NAME "bubu_project")
	set( _ONE_VALUE_ARGS DIR PREFIX)
	#List arguments (e.g. LANGUAGES C ASM CXX)
	set( _MULTI_VALUE_ARGS )

	cmake_parse_arguments(_MY_PARAMS "${_OPTIONS_ARGS}" "${_ONE_VALUE_ARGS}"
							"${_MULTI_VALUE_ARGS}" ${ARGN} )

	#Check parameters
	foreach(_PARAM IN LISTS _ONE_VALUE_ARGS)
		if (NOT DEFINED _MY_PARAMS_${_PARAM})
			message(FATAL_ERROR "Parameter ${_PARAM} is missing!")
		endif()
	endforeach()

	#Search for .rst files
	file(GLOB_RECURSE _RST_FILES
		LIST_DIRECTORIES False
		${_MY_PARAMS_DIR}/**.rst)

	#Loop over .rst files
	foreach(_FILE IN LISTS _RST_FILES)
		#Read the file and return the first status line.
		file(STRINGS ${_FILE} _CONTENT REGEX ":Status:" LIMIT_COUNT 1)

		#Nothing read -> the field is missing
		if (_CONTENT STREQUAL "")
			list(APPEND _STATUS_UNKNOWN ${_FILE})
		else()
			#convert to upper case for case insensitive matching.
			string(TOUPPER ${_CONTENT} _CONTENT)

			#loop over status values
			foreach(_STATUS IN LISTS _STATE_VALUES)
				#convert to upper case for case insensitive matching.
				string(TOUPPER ${_STATUS} _STATUS)
				#does status match the value from the file?
				if (_CONTENT MATCHES ":STATUS:[ ]*${_STATUS}")
					#add it to the list
					list(APPEND _STATUS_${_STATUS} ${_FILE})
					#and exit the loop
					break()
				endif()
				#"unknown" status is used as a quard and is an invalid value.
				#If we reach it the file has invalid status value.
				if (_STATUS STREQUAL "UNKNOWN")
					#add the file to the unknown list
					list(APPEND _STATUS_${_STATUS} ${_FILE})
				endif()
			endforeach()
		endif()
	endforeach()

	#return the lists to the caller
	foreach(_STATUS IN LISTS _STATE_VALUES)
		string(TOUPPER ${_STATUS} _STATUS)

		if (DEFINED _STATUS_${_STATUS})
			set(${_MY_PARAMS_PREFIX}_${_STATUS} ${_STATUS_${_STATUS}}
					PARENT_SCOPE)
		endif()
	endforeach()
endfunction()

#Configure (fill in) the Sphinx index.rst.in template file.
#Call sphinx_categorize_rst() to get the .rst files sorted to lists, and then
#create the list of files for each design document section (convert the CMake
#lists to properly formatted text).
#Finally call configure_file() to fill in the values.
#
#Inputs:
#    SRC      Full path to template index file
#    DST      Full patch to configured output file.
#....DOC_DIR  Path to design documents directory.
#    DOC_ROOT Path to root directory of documentation
#
#Outputs:
#   Configured <DST> file.
#
#Examples
#   sphinx_categorize_rst(DIR ${TFM_ROOT_DIR}/docs/design_documents
#                          PREFIX "DESIGN_DOCS")
#
function(sphinx_configure_index)
	set(_STATE_VALUES "draft" "rejected" "accepted" "detailed" "unknown")

	#No option (on/off) arguments
	set( _OPTIONS_ARGS )
	 #Single option arguments (e.g. PROJ_NAME "bubu_project")
	set( _ONE_VALUE_ARGS SRC DST DOC_ROOT DOC_DIR)
	#List arguments (e.g. LANGUAGES C ASM CXX)
	set( _MULTI_VALUE_ARGS )

	cmake_parse_arguments(_MY_PARAMS "${_OPTIONS_ARGS}" "${_ONE_VALUE_ARGS}"
							"${_MULTI_VALUE_ARGS}" ${ARGN} )

	#Check parameters
	foreach(_PARAM IN LISTS _ONE_VALUE_ARGS)
		if (NOT DEFINED _MY_PARAMS_${_PARAM})
			message(FATAL_ERROR "Parameter ${_PARAM} is missing!")
		endif()
	endforeach()

	#Assign design documents to lists based on their status
	sphinx_categorize_rst(DIR ${_MY_PARAMS_DOC_DIR} PREFIX "_DD")

	#Look for invalid documents
	if (DEFINED _DD_UNKNOWN)
		string(REPLACE ";" "\n \t" _DD_UNKNOWN "${_DD_UNKNOWN}")
		message(FATAL_ERROR " The following documents have no or invalid status:\n \t${_DD_UNKNOWN}")
	endif()

	#The document root must be an absolute path
	get_filename_component(_MY_PARAMS_DOC_ROOT "${_MY_PARAMS_DOC_ROOT}"
							ABSOLUTE)

	#Loop over status lists
	foreach(_STATUS IN ITEMS "DRAFT" "DETAILED" "ACCEPTED" "REJECTED")
		#Create an empty file list for this status
		set(${_STATUS}_DD_LIST "")
		#If the source list is empty go to next iteration
		if (NOT DEFINED _DD_${_STATUS})
			continue()
		endif()

		#Loop over files on the list of this status
		foreach(_FILE IN LISTS _DD_${_STATUS})
			#Get the path of the file relative to the document root
			file(RELATIVE_PATH _REL_FILE ${_MY_PARAMS_DOC_ROOT} ${_FILE})
			#Detailed and Draft files go to the same section
			if (_STATUS STREQUAL "DETAILED")
				set(_STATUS "DRAFT")
			endif()

			#Append the file to the output string
			string(APPEND ${_STATUS}_DD_LIST "\n    ${_REL_FILE}")
		endforeach()
	endforeach()

	#Call configure file to fill out the template.
	configure_file(${_MY_PARAMS_SRC} ${_MY_PARAMS_DST} @ONLY)
endfunction()

#If being run in script mode (cmake -P) and not included.
if (DEFINED CMAKE_SCRIPT_MODE_FILE
			 AND (CMAKE_CURRENT_LIST_FILE STREQUAL CMAKE_SCRIPT_MODE_FILE))
	#Check input variables.
	foreach(_PARAM IN ITEMS SPHINX_TEMPLATE_INDEX_FILE SPHINX_CONFIGURED_INDEX_FILE
							SPHINX_DESIGN_DOC_ROOT TFM_ROOT_DIR)
		if (NOT DEFINED ${_PARAM})
			message(FATAL_ERROR "Parameter ${_PARAM} is not set!")
		endif()
	endforeach()

	sphinx_configure_index(SRC ${SPHINX_TEMPLATE_INDEX_FILE}
					DST ${SPHINX_CONFIGURED_INDEX_FILE}
					DOC_DIR ${SPHINX_DESIGN_DOC_ROOT}
					DOC_ROOT ${TFM_ROOT_DIR})
endif()
