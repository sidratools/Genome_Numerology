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

#include "gn_fasta_read.h"

#include <stdio.h>
#include <string.h>

GN_FASTA_STREAM* gn_fasta_read_open_file(const char *restrict _filename)
{
	return fopen(_filename, "r");
}

/*
 * Possible return:
 * 	- sequence size greater than 0
 * 	- 0 if no more readable sequences due to end-of-stream or error
 */
size_t gn_fasta_read_sequence(GN_FASTA_STREAM *restrict _stream, char *restrict _buffer, const size_t _limit)
{
	size_t size;
	const char *line;

	do
	{
		line = fgetln(_stream, &size);
	}
	while((line != NULL) && (size > 0) && (line[0] == '>'));

	size_t new_limit = _limit;

	while((line != NULL) && (size > 0) && (line[0] != '>'))
	{
		if((size > new_limit) || (size == new_limit && line[size-1] != '\n'))
			return 0;

		if(line[size-1] == '\n')
			--size;

		memcpy(_buffer, line, size);

		new_limit -= size;
		_buffer += size;

		line = fgetln(_stream, &size);
	}

	*_buffer = 0;

    return _limit-new_limit;
}

int gn_fasta_read_close(GN_FASTA_STREAM *_stream)
{
	return fclose(_stream);
}
