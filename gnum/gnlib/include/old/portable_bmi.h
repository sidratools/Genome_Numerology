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
 * Created: Nov 22, 2014
 *
 * Description: Portable bit manipulation instructions (BMI).
 *========================================================================*/

#ifndef GN_PORTABLE_BMI
#define GN_PORTABLE_BMI

#ifdef ENABLE_INTEL_BMI2

#include <x86intrin.h>

#define bit_gather_u64(vec64b,mask64b) _pext_u64(vec64b,mask64b)
#define bit_scatter_u64(vec64b,mask64b) _pdep_u64(vec64b,mask64b)

#else /* USE_INTEL_BMI2 */

#include <stdint.h>

inline static
uint64_t bit_gather_u64(const uint64_t _value, const uint64_t _mask)
{
	uint64_t gathered = 0;
	int value_offset = 0;

	for(int bit_pos=0; bit_pos<64; ++bit_pos)
	{
		if((_mask >> bit_pos) & 0x01)
		{
			const int value_bit = (_value >> bit_pos) & 0x01;
			gathered |= value_bit << value_offset;
			++value_offset;
		}
	}

	return gathered;
}

inline static
uint64_t bit_scatter_u64(const uint64_t _value, const uint64_t _mask)
{
	uint64_t scattered = 0;
	int value_offset = 0;

	for(int i=0; i<64; ++i)
	{
		if((_mask >> i) & 0x01)
		{
			const uint64_t value_bit = (_value >> value_offset) & 0x01;
			scattered |= value_bit << i;
			++value_offset;
		}
	}

	return scattered;
}

#endif /* USE_INTEL_BMI2 */

#endif /* GN_PORTABLE_BMI */
