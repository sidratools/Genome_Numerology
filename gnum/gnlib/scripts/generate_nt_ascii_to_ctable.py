#!/usr/bin/env python
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

from string import lower

output_type = 'const gn_nucleotide_t'
array_name = 'five_bit_ascii_to_nucleotide'

default_nucleotide = 'A'

four_nucleotides_to_tag = {
    'A': 'GN_NT_A',
    'C': 'GN_NT_C',
    'G': 'GN_NT_G',
    'T': 'GN_NT_T',
}

# every listed encoding generates a unique ID with their 5 least significant bits
four_nucleotides_to_upper_ascii_encoding = {
    'A': {'A', 'W', 'M', 'R', 'D', 'H', 'V', 'N'},
    'C': {'C', 'S', 'Y', 'B'},
    'G': {'G', 'K'},
    'T': {'T', 'U'},
}

five_bit_ascii_to_nucleotide = {}

for nucleotide, ascii_set in four_nucleotides_to_upper_ascii_encoding.iteritems():
    for ascii in ascii_set:
        five_bit_ascii = ord(ascii) & 0b11111
        print ascii, five_bit_ascii, nucleotide
        five_bit_ascii_to_nucleotide[five_bit_ascii] = nucleotide

array_values = []
for index in range(32):
    nucleotide = default_nucleotide
    if index in five_bit_ascii_to_nucleotide:
        nucleotide = five_bit_ascii_to_nucleotide[index]

    array_values.append(four_nucleotides_to_tag[nucleotide])

print output_type, array_name+'[32] = {'

count = 0
for value in array_values:
    if (count % 8) == 0:
        if count != 0:
            print ''
        print '     ',
    if (count+1) == len(array_values):
        print value
    else:
        print str(value)+',',
    count += 1

print '};'
