/*
 * Copyright (c) 2015, Eric R. Schendel.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * - Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * - Neither the name of gnum nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*========================================================================
 *  Author: Eric R. Schendel <eschendel@sidra.org>
 * Created: Nov 10, 2014
 *
 * Description: Global data types and constants.
 *========================================================================*/

#ifndef GN_TYPES_HEADER
#define GN_TYPES_HEADER

#include "gn_assert.h"

#include <stddef.h>
#include <stdint.h>

/****************************************
 * Declare Platform Dependent Data Types
 ****************************************/

typedef unsigned __int128 uint128_t;

/*******************
 * Define Constants
 *******************/

/* nucleotide (NT) encoded values */

#define GN_NT_A 0x00
#define GN_NT_C 0x01
#define GN_NT_G 0x02
#define GN_NT_T 0x03
#define GN_NT_U GN_NT_T

/* encoding information */

#define GN_ENCODED_BIT_COUNT 2
#define GN_ENCODED_BIT_MASK  (~(~0 << GN_ENCODED_BIT_COUNT))

GN_STATIC_ASSERT(GN_ENCODED_BIT_COUNT == 2, "Only 2-bit encoding currently supported");

/******************************
 * Library Specific Data Types
 ******************************/

typedef uint8_t gn_nucleotide_t;
typedef char    gn_ascii_t;

typedef uint8_t   gn_vector8b_t;
typedef uint16_t  gn_vector16b_t;
typedef uint32_t  gn_vector32b_t;
typedef uint64_t  gn_vector64b_t;
typedef uint128_t gn_vector128b_t;

#endif /* GN_TYPES_HEADER */
