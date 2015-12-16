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

if(NOT (DEFINED ${CMAKE_PROJECT_NAME}_C11_STANDARD_FLAGS))

  include(CheckCCompilerFlag)

  if(NOT ((CMAKE_C_COMPILER_ID STREQUAL "GNU")
          OR (CMAKE_C_COMPILER_ID MATCHES "Clang")))
    message(WARNING "Only GNU and Clag compilers have been currently tested "
                    "for determing C11 standard support - use at your own discretion. "
                    "Compiler detected: ${CMAKE_C_COMPILER_ID}")
  endif()

  set(POSSIBLE_C11_FLAG "-std=c11")
  message(STATUS "Try C11 standard flag = [${POSSIBLE_C11_FLAG}]")

  check_c_compiler_flag(${POSSIBLE_C11_FLAG} Try_C11_Flag)

  if(Try_C11_Flag)
    set(${CMAKE_PROJECT_NAME}_C11_STANDARD_FLAGS "${POSSIBLE_C11_FLAG}")
  else()
    message(STATUS "No compiler option determined for C11 standard.")
  endif()

  set(${CMAKE_PROJECT_NAME}_C11_STANDARD_FLAGS "${${CMAKE_PROJECT_NAME}_C11_STANDARD_FLAGS}" CACHE STRING
      "Extra flag required to compile with C11 standard (default determined during configuration).")

endif(NOT (DEFINED ${CMAKE_PROJECT_NAME}_C11_STANDARD_FLAGS))


include(CheckCSourceCompiles)

set(CMAKE_REQUIRED_FLAGS "${${CMAKE_PROJECT_NAME}_C11_STANDARD_FLAGS}")

check_c_source_compiles("#if __STDC_VERSION__ < 201112L
                         #error Not C11 Standard Version
                         #endif
                         int main() { return 0; }"
                         Require_C11_Compiler)

if(NOT Require_C11_Compiler)
  message(FATAL_ERROR "Compiler does not support C11 standard!")
endif(NOT Require_C11_Compiler)

add_compile_options(${${CMAKE_PROJECT_NAME}_C11_STANDARD_FLAGS})

