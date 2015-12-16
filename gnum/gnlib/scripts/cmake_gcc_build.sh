#!/bin/bash
## Copyright (c) 2015, Eric R. Schendel.
## All rights reserved.
##
## Redistribution and use in source and binary forms, with or without
## modification, are permitted provided that the following conditions are met:
##
## - Redistributions of source code must retain the above copyright notice, this
##   list of conditions and the following disclaimer.
##
## - Redistributions in binary form must reproduce the above copyright notice,
##   this list of conditions and the following disclaimer in the documentation
##   and/or other materials provided with the distribution.
##
## - Neither the name of gnum nor the names of its
##   contributors may be used to endorse or promote products derived from
##   this software without specific prior written permission.
##
## THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
## AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
## IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
## DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
## FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
## DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
## SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
## CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
## OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
## OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

if [ "${*}" = "" ]; then
    BUILD_TYPE=Release
else
    BUILD_TYPE=${1}
fi

BUILD_TYPE_LOWER=`echo ${BUILD_TYPE} | tr '[A-Z]' '[a-z]'`
ARGS_REST=${*:2}

CMAKE_C_COMPILER="gcc"
CMAKE_CXX_COMPILER="g++"

CMAKE_CPP_FLAGS="-Wall -fverbose-asm"

declare -a CMAKE_DEFAULTS=(
    "CMAKE_BUILD_TYPE=${BUILD_TYPE}"
    "CMAKE_C_FLAGS='${CMAKE_CPP_FLAGS}'"
)

DIR_BUILD_NAME=build_${PWD##*/}_${CMAKE_C_COMPILER}_${BUILD_TYPE_LOWER}

CMAKE_ARGS="${ARGS_REST} "
for i in $(seq 0 $((${#CMAKE_DEFAULTS[*]} - 1)))
do
    CMAKE_ARGS+="-D${CMAKE_DEFAULTS[$i]} "
done

DIR_ABS_PARENT=${PWD%/*}
DIR_ABS_BUILD=${DIR_ABS_PARENT}/${DIR_BUILD_NAME}
DIR_REL_SOURCE=../${PWD##*/}
DIR_REL_BUILD=../${DIR_BUILD_NAME}

LAUNCH_CMAKE="CC='${CMAKE_C_COMPILER}' CXX='${CMAKE_CXX_COMPILER}' cmake ${CMAKE_ARGS} ${DIR_REL_SOURCE}"

rm -rf ${DIR_REL_BUILD}
mkdir ${DIR_REL_BUILD}
cd ${DIR_REL_BUILD}

echo "Launching CMake within '${DIR_ABS_BUILD}/':"
echo ""
echo "${LAUNCH_CMAKE}"
echo ""

eval ${LAUNCH_CMAKE}

