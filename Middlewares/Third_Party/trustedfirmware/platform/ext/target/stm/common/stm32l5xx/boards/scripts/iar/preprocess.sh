#!/bin/bash -
#  ******************************************************************************
#  * @attention
#  *
#  * Copyright (c) 2019 STMicroelectronics.
#  * All rights reserved.
#  *
#  * This software is licensed under terms that can be found in the LICENSE file
#  * in the root directory of this software component.
#  * If no LICENSE file comes with this software, it is provided AS-IS.
#  *
#  ******************************************************************************
# arg1 is the build directory
# arg2 is the file to preprocess
# arg3 is output file beeing preprocessed
function preprocess
(
iccarm --cpu=Cortex-M33  -I$1 $2  --silent --preprocess=ns $3
)