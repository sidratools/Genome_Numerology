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

# Author: Eric R. Schendel <eschendel@sidra.org>

#TODOs:
# - add check for __int128
# - add check for _Static_assert
# - SSE2 used for AVX2 & BMI (emmintrin.h, _mm_set1_epi64x, _mm_set_epi8)
# - SSSE3 used for BMI (tmmintrin.h, _mm_shuffle_epi8)
# - bswap (x86intrin.h, _bswap, _bswap64

project(GNumLib C)
cmake_minimum_required(VERSION 3.1)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake/")

include(RequireC11Compiler)
include(ConfigureArchitectureOptimizations)

include_directories(${PROJECT_SOURCE_DIR}/include)

add_subdirectory(src)
add_executable(example test/example.c)
target_link_libraries(example gnum)

#enable_testing()
#add_test(TestExample example)

mark_as_advanced(CMAKE_ECLIPSE_EXECUTABLE)
