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
 * Created: Jan 18, 2015
 *
 * Description: Unpack encoded nucleotides from packed data containers.
 *========================================================================*/

#ifndef GN_UNPACK_HEADER
#define GN_UNPACK_HEADER

#include "gn_assert.h"
#include "gn_types.h"

#include "portable_endian.h"

#ifdef ENABLE_INTEL_BMI2
#	include <emmintrin.h>
#	include <tmmintrin.h>
#	include <x86intrin.h>
#endif

#define GN_UNPACK_TILED_SIZE_INPUT  sizeof(vector32b_t)
#define GN_UNPACK_TILED_SIZE_OUTPUT sizeof(vector128b_t)

GN_STATIC_ASSERT(BYTE_ORDER == LITTLE_ENDIAN, "Only little-endian architectures currently supported");

/***
 * Unpack/Decompress 32-bit packed nucleotide encoded vector into larger vector
 * (i.e. 128-bit vector for 2-bit encodings).
 *
 * See gn_packed_vector128b function (gn_pack.h) for more information.
 */
inline static
gn_vector128b_t* gn_unpack_vector32b(void* _dst_vec128, const void* _src_vec32)
{
#ifdef ENABLE_INTEL_BMI2

	const __m128i offsets = _mm_set_epi8(12,13,14,15,8,9,10,11,4,5,6,7,0,1,2,3);
	const uint64_t select_mask = GN_ENCODED_BIT_MASK * 0x0101010101010101ULL;

	const uint64_t vec128_lo = _pdep_u64(((uint16_t*) _src_vec32)[0], select_mask);
	const uint64_t vec128_hi = _pdep_u64(((uint16_t*) _src_vec32)[1], select_mask);

	const __m128i vec128 = _mm_set_epi64x(vec128_hi, vec128_lo);

	*((__m128i*) _dst_vec128) = _mm_shuffle_epi8(vec128, offsets);

#else /* generic version of unpacking 16-byte nucleotide encodings */

	const gn_vector8b_t* packed_nts = (const gn_vector8b_t*) _src_vec32;
	gn_nucleotide_t* nts = (gn_nucleotide_t*) _dst_vec128;

	for(int i=0; i<4; ++i)
	{
		nts[4*i+0] = (packed_nts[i] >> (3 * GN_ENCODED_BIT_COUNT)) & GN_ENCODED_BIT_MASK;
		nts[4*i+1] = (packed_nts[i] >> (2 * GN_ENCODED_BIT_COUNT)) & GN_ENCODED_BIT_MASK;
		nts[4*i+2] = (packed_nts[i] >> (1 * GN_ENCODED_BIT_COUNT)) & GN_ENCODED_BIT_MASK;
		nts[4*i+3] = (packed_nts[i] >> (0 * GN_ENCODED_BIT_COUNT)) & GN_ENCODED_BIT_MASK;
	}

#endif

	return _dst_vec128;
}

#if 0 /* needs to be tested */
#include <assert.h>
inline static
int gn_unpack_ascii_string_tiled(void *_dst_string,
                                 const void *_src_buffer, const size_t _string_size)
{
	assert( (((size_t) _dst_string & 0xF) == 0)
			&& (((size_t) _src_buffer & 0x3) == 0) );

	gn_vector128b_t encoded_vec128;

	gn_vector128b_t* outputs_vec128 = (gn_vector128b_t*) _dst_string;
	const gn_vector32b_t* inputs_vec32 = (const gn_vector32b_t*) _src_buffer;

	const size_t inputs_count = _string_size / sizeof(gn_vector128b_t);

	for(size_t idx=0; idx<inputs_count; ++idx)
	{
		gn_unpack_vector32b(&encoded_vec128, inputs_vec32+idx);
		gn_decode_ascii_vector128b(outputs_vec128+idx, &encoded_vec128);
	}

	return 0;
}
#endif

#endif /* GN_PACK_HEADER */
