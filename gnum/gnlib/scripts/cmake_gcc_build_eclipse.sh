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

LAUNCH_CMD=${0%/*}/cmake_gcc_build.sh

rm -f .project

declare -a CMAKE_DEFAULTS=(
    'CMAKE_ECLIPSE_VERSION:STRING=4.4'
    'CMAKE_ECLIPSE_MAKE_ARGUMENTS=-j1'
    )

### Optional defaults for CMAKE_DEFAULTS above:
#    'CMAKE_ECLIPSE_GENERATE_SOURCE_PROJECT=TRUE'
#    'CMAKE_EXPORT_COMPILE_COMMANDS=TRUE'

CMAKE_ECLIPSE_ARGS=""
for VAR in ${CMAKE_DEFAULTS[@]}
do
    CMAKE_ECLIPSE_ARGS+="-D${VAR} "
done

eval "${LAUNCH_CMD} ${BUILD_TYPE} -G\\'Eclipse CDT4 - Unix Makefiles\\' ${CMAKE_ECLIPSE_ARGS}"
