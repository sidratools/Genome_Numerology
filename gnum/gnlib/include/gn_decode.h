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
 * Description: Decoders to produce ASCII text from nucleotide data types.
 *========================================================================*/

#ifndef GN_DECODE_HEADER
#define GN_DECODE_HEADER

#include "gn_types.h"

/***
 * Decode single nucleotide value (see gn_types.h) to ASCII character.
 *
 * Input is expected to be a nucleotide encoded value, see GN_NT_{A,C,G,T}.
 * Otherwise, the output is unspecified. Expected output characters are:
 * 'A', 'C', 'G', 'T'.
 *
 * See gn_encode_ascii_nucleotide function (gn_encode.h) for more details.
 */
inline static
gn_ascii_t gn_decode_ascii_nucleotide(gn_nucleotide_t _nt)
{
	/*
	 * This function is intentionally compute based and does not use
	 * logical comparators (if/else, switch, etc) and lookup tables.
	 * This allows intense compiler optimizations especially for
	 * vectorizing and parallelizing operations for variety of
	 * hardware architectures (CPUs, GPUs, etc).
	 */

	gn_ascii_t ascii = 'A';

	ascii |= (_nt << 1) + _nt; /* generates 'A', 'C', 'G' */

	/* Following statement is correction for 'T' and most compilers
	 * will optimize out the jump statement. If issue, then instead
	 * recommend using: ascii += ((ascii >> 3) & 1) * 11;
	 */
	ascii += (_nt == GN_NT_T) ? 11 : 0;  /* correction for 'T' */

	return ascii;
}

/***
 * Decode set of 16 nucleotide values (128-bit vector) to ASCII
 * characters, see gn_decode_ascii_nucleotide function. Each decoded
 * value ('A', 'C', 'G', 'T') will be a single byte (8-bits).
 */
inline static
gn_ascii_t* gn_decode_ascii_vector128b(void* _dst_ascii_buffer, const void* _src_vec128)
{
	const gn_nucleotide_t *nt_seq = (const gn_nucleotide_t*) _src_vec128;

	gn_vector128b_t ascii_vec128;
	gn_ascii_t* ascii_seq = (gn_ascii_t*) &ascii_vec128;

	for(int i=0; i<16; ++i)
		ascii_seq[i] = gn_decode_ascii_nucleotide(nt_seq[i]);

	*((gn_vector128b_t*) _dst_ascii_buffer) = ascii_vec128;

	return (gn_ascii_t*) _dst_ascii_buffer;
}

#endif /* GN_DECODE_HEADER */
