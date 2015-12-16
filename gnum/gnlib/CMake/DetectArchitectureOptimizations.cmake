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

if(NOT (DEFINED ${CMAKE_PROJECT_NAME}_TARGET_ARCHITECTURE))

  if(NOT (CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64"))
    message(WARNING "Detecting optimizations currently available for x86_64 architectures. "
                    "Architecture detected: ${CMAKE_SYSTEM_PROCESSOR}")
  endif()

  set(${CMAKE_PROJECT_NAME}_TARGET_ARCHITECTURE "native" CACHE STRING
      "Generate binaries for targetted CPU architecture. Options are 'default' (compiler's default), 'native' (host architecture), and possile options listed in GNU GCC manual for -march argument.")

endif(NOT (DEFINED ${CMAKE_PROJECT_NAME}_TARGET_ARCHITECTURE))

include(ConfigureCompilerSanity)

if(COMPILER_TYPE_GNU)

  include(CheckCSourceCompiles)

  if(NOT (DEFINED ${CMAKE_PROJECT_NAME}_ENABLE_INTEL_BMI2))
    set(CMAKE_REQUIRED_FLAGS "-mbmi2 ${${CMAKE_PROJECT_NAME}_C_FLAGS}")

    check_c_source_compiles("#include <x86intrin.h>
                             int main() { return _pext_u64(0,0) | _pdep_u64(0,0); }"
                             Use_Intel_BMI2)

    set(${CMAKE_PROJECT_NAME}_ENABLE_INTEL_BMI2 ${Use_Intel_BMI2} CACHE BOOL
        "Use supported Intel BMI2 processor instruction set (default determined during configuration).")
  endif(NOT (DEFINED ${CMAKE_PROJECT_NAME}_ENABLE_INTEL_BMI2))

  if(NOT (DEFINED ${CMAKE_PROJECT_NAME}_ENABLE_INTEL_AVX2))
    set(CMAKE_REQUIRED_FLAGS "-mavx2 ${${CMAKE_PROJECT_NAME}_C_FLAGS}")

    check_c_source_compiles("#include <immintrin.h>
                             int main() { __m128i x=_mm_set1_epi64x(1); _mm_srlv_epi64(x,x); return 0;}"
                             Use_Intel_AVX2)

    set(${CMAKE_PROJECT_NAME}_ENABLE_INTEL_AVX2 ${Use_Intel_AVX2} CACHE BOOL
        "Use supported Intel AVX2 processor instruction set (default determined during configuration).")
  endif(NOT (DEFINED ${CMAKE_PROJECT_NAME}_ENABLE_INTEL_AVX2))

endif(COMPILER_TYPE_GNU)

