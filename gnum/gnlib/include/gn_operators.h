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
 * Created: Feb 8, 2015
 *
 * Description: Operators on packed encoded data vectors
 *========================================================================*/

#ifndef GN_OPERATORS_HEADER
#define GN_OPERATORS_HEADER

#include "gn_assert.h"
#include "gn_types.h"

#include "portable_endian.h"

#ifdef ENABLE_INTEL_BMI2
#	include <tmmintrin.h>
#	include <x86intrin.h>
#endif

GN_STATIC_ASSERT(BYTE_ORDER == LITTLE_ENDIAN, "Only little-endian architectures currently supported");

inline static
gn_vector32b_t gn_complement_vector32b(const gn_vector32b_t _vec32)
{
	return ~_vec32;
}

inline static
gn_vector32b_t gn_reverse_vector32b(gn_vector32b_t _vec32)
{
#ifdef ENABLE_INTEL_BMI2

	const uint64_t select_mask = GN_ENCODED_BIT_MASK * 0x0101010101010101ULL;

	uint16_t *vec16_ptr = (uint16_t*) &_vec32;

	const uint64_t vec128_hi = _bswap64(_pdep_u64(vec16_ptr[0], select_mask));
	const uint64_t vec128_lo = _bswap64(_pdep_u64(vec16_ptr[1], select_mask));

	vec16_ptr[0] = _pext_u64(vec128_lo, select_mask);
	vec16_ptr[1] = _pext_u64(vec128_hi, select_mask);

	return _vec32;

#else /* generic version of reversing operation */

	gn_vector32b_t byte_swapped_vec32 = _bswap(_vec32);

	return (((byte_swapped_vec32 & 0x30303030) >> 2)
			| ((byte_swapped_vec32 & 0x0C0C0C0C) << 2)
			| ((byte_swapped_vec32 & 0xC0C0C0C0) >> 6)
			| ((byte_swapped_vec32 & 0x03030303) << 6));
#endif
}

inline static
gn_vector32b_t gn_reverse_complement(gn_vector32b_t _vec32)
{
	_vec32 = gn_reverse_vector32b(_vec32);
	return gn_complement_vector32b(_vec32);
}


#endif /* GN_OPERATORS_HEADER */
