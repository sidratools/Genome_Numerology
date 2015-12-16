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
 * Created: Nov 11, 2014
 *
 * Description: Encoders to produce nucleotide data types.
 *========================================================================*/

#ifndef GN_ENCODE_HEADER
#define GN_ENCODE_HEADER

#include "gn_types.h"

#ifdef ENABLE_INTEL_AVX2
#   include <emmintrin.h>
#	include <immintrin.h>
#endif

/***
 * Encode single nucleotide value (see gn_types.h) from ASCII character.
 *
 * Expected input characters are as follows (others are randomly encoded):
 *     'A', 'C', 'G', 'N', 'T', 'U'.
 *
 * Character 'N' will generate the same encoded value as 'A'.
 * Character 'U' will generate the same encoded value as 'T'.
 * NULL characters with value of 0 will encode to value 'A'.
 */
inline static
gn_nucleotide_t gn_encode_ascii_nucleotide(gn_ascii_t _acgt_ascii)
{
	/*
	 * This function is intentionally compute based and does not use logical
	 * comparators (if/else, switch, etc) and lookup tables. This allows
	 * intense compiler optimizations especially for vectorizing and
	 * parallelizing operations for variety of hardware architectures
	 * (CPUs, GPUs, etc).
	 */

	/* using ASCII character bit hacks to generate encoded values */
	_acgt_ascii = _acgt_ascii >> 1;
	_acgt_ascii ^= _acgt_ascii >> 1;
	return _acgt_ascii & GN_ENCODED_BIT_MASK;
}

/***
 * Encode set of 16 nucleotide values (128-bit vector) from ASCII
 * characters, see gn_encode_ascii_nucleotide function. Each encoded
 * value will be contained within a single byte (8-bits).
 */
inline static
gn_vector128b_t* gn_encode_ascii_vector128b(void* _dst_vec128, const void* _src_ascii_buffer)
{
#ifdef ENABLE_INTEL_AVX2

	const __m128i shift_by  = _mm_set1_epi64x(1);
	const __m128i byte_mask128 = _mm_set1_epi64x(GN_ENCODED_BIT_MASK * 0x0101010101010101ULL);

	__m128i encoded_vec128 = *((__m128i*) _src_ascii_buffer);

	encoded_vec128 = _mm_srlv_epi64(encoded_vec128, shift_by);
	encoded_vec128 ^= _mm_srlv_epi64(encoded_vec128, shift_by);
	encoded_vec128 &= byte_mask128;

	*((__m128i*) _dst_vec128) = encoded_vec128;

#else /* generic version of 16-byte encoding */

	const gn_ascii_t* ascii_seq = (const gn_ascii_t*) _src_ascii_buffer;

	gn_vector128b_t encoded_vec128;
	gn_nucleotide_t *nt_seq = (gn_nucleotide_t*) &encoded_vec128;

	/* Modern compilers should not have any issues with optimizing this for-loop
	 * such as vectoring the encoding of each nucleotide. */

	for(int i=0; i<16; ++i)
		nt_seq[i] = gn_encode_ascii_nucleotide(ascii_seq[i]);

	*((gn_vector128b_t*) _dst_vec128) = encoded_vec128;

#endif

	return (gn_vector128b_t*) _dst_vec128;
}

#endif /* GN_ENCODE_HEADER */
