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
 * Created: Jan 4, 2015
 *
 * Description: Pack nucleotides into data container.
 *========================================================================*/

#ifndef GN_PACK_HEADER
#define GN_PACK_HEADER

#include "gn_assert.h"
#include "gn_types.h"

#include "portable_endian.h"

#ifdef ENABLE_INTEL_BMI2
#	include <emmintrin.h>
#	include <tmmintrin.h>
#	include <x86intrin.h>
#endif

#define GN_PACK_TILED_SIZE_INPUT  sizeof(vector128b_t)
#define GN_PACK_TILED_SIZE_OUTPUT sizeof(vector32b_t)

GN_STATIC_ASSERT(BYTE_ORDER == LITTLE_ENDIAN, "Only little-endian architectures currently supported");

/***
 * Pack/Compress 16 distinct nucleotides (128-bit vector of byte-sized
 * nucleotide encodings) to smallest possible vector (i.e. 32-bit vector
 * for 2-bit encodings).
 *
 * Output of packing keeps the byte stream ordering of the input.
 */
inline static
gn_vector32b_t* gn_pack_vector32b(void* _dst_vec32, const void* _src_vec128)
{
#ifdef ENABLE_INTEL_BMI2

	const __m128i offsets = _mm_set_epi8(12,13,14,15,8,9,10,11,4,5,6,7,0,1,2,3);
	const uint64_t select_mask = GN_ENCODED_BIT_MASK * 0x0101010101010101ULL;

	const __m128i vec128 = _mm_shuffle_epi8(*((__m128i*) _src_vec128), offsets);

	const gn_vector32b_t vec32_lo = _pext_u64(((gn_vector64b_t*) &vec128)[0], select_mask);
	const gn_vector32b_t vec32_hi = _pext_u64(((gn_vector64b_t*) &vec128)[1], select_mask);

	((gn_vector16b_t*) _dst_vec32)[0] = vec32_lo;
	((gn_vector16b_t*) _dst_vec32)[1] = vec32_hi;

#else /* generic version of packing 16-byte nucleotide encodings */

	const gn_nucleotide_t* nts = (const gn_nucleotide_t*) _src_vec128;
	gn_vector8b_t* packed_nts = (gn_vector8b_t*) _dst_vec32;

	for(int i=0; i<4; ++i)
	{
		packed_nts[i] = (nts[4*i+0] << (3 * GN_ENCODED_BIT_COUNT))
		                | (nts[4*i+1] << (2 * GN_ENCODED_BIT_COUNT))
		                | (nts[4*i+2] << (1 * GN_ENCODED_BIT_COUNT))
		                | (nts[4*i+3] << (0 * GN_ENCODED_BIT_COUNT));
	}

#endif

	return _dst_vec32;
}

inline static
gn_vector32b_t* gn_pack_reverse_vector32b(void* _dst_vec32, const void* _src_vec128)
{
#ifdef ENABLE_INTEL_BMI2

	const __m128i offsets = _mm_set_epi8(3,2,1,0,7,6,5,4,11,10,9,8,15,14,13,12);
	const uint64_t select_mask = GN_ENCODED_BIT_MASK * 0x0101010101010101ULL;

	const __m128i vec128 = _mm_shuffle_epi8(*((__m128i*) _src_vec128), offsets);

	const gn_vector32b_t vec32_lo = _pext_u64(((gn_vector64b_t*) &vec128)[0], select_mask);
	const gn_vector32b_t vec32_hi = _pext_u64(((gn_vector64b_t*) &vec128)[1], select_mask);

	((gn_vector16b_t*) _dst_vec32)[0] = vec32_lo;
	((gn_vector16b_t*) _dst_vec32)[1] = vec32_hi;

#else /* generic version of reverse packing 16-byte nucleotide encodings */

	const gn_nucleotide_t* nts = (const gn_nucleotide_t*) _src_vec128;
	gn_vector8b_t* packed_nts = (gn_vector8b_t*) _dst_vec32;

	for(int i=0; i<4; ++i)
	{
		packed_nts[3-i] = (nts[4*i+0] << (0 * GN_ENCODED_BIT_COUNT))
		                  | (nts[4*i+1] << (1 * GN_ENCODED_BIT_COUNT))
		                  | (nts[4*i+2] << (2 * GN_ENCODED_BIT_COUNT))
		                  | (nts[4*i+3] << (3 * GN_ENCODED_BIT_COUNT));
	}

#endif

	return _dst_vec32;
}

#if 0 /* needs to be tested */
#include <assert.h>
inline static
int gn_pack_ascii_string_tiled(void *_dst_buffer,
                               const void *_src_string, const size_t _string_size)
{
	assert( (((size_t) _dst_buffer & 0x3) == 0)
			&& (((size_t) _src_string & 0xF) == 0)
	        && ((_string_size & 0xF) == 0) );

	gn_vector128b_t encoded_vec128;

	gn_vector32b_t* outputs_vec32 = (gn_vector32b_t*) _dst_buffer;
	const gn_vector128b_t* inputs_vec128 = (const gn_vector128b_t*) _src_string;

	const size_t inputs_count = _string_size / sizeof(gn_vector128b_t);

	for(size_t idx=0; idx<inputs_count; ++idx)
	{
		gn_encode_ascii_vector128b(&encoded_vec128, inputs_vec128+idx);
		gn_pack_vector32b(outputs_vec32+idx, &encoded_vec128);
	}

	return 0;
}
#endif

#endif /* GN_PACK_HEADER */
