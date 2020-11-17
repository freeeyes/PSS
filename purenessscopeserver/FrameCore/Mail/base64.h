/*
 * Copyright (c) 2006 Ryan Martell. (rdm4@martellventures.com)
 *
 * This file is part of FFmpeg.
 *
 * FFmpeg is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * FFmpeg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with FFmpeg; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef BASE64_H
#define BASE64_H

#include <stdio.h>

/**
 * Decode a base64-encoded string.
 *
 * @param out      buffer for decoded data
 * @param in       nullptr-terminated input string
 * @param out_size size in bytes of the out buffer, must be at
 *                 least 3/4 of the length of in
 * @return         number of bytes written, or a negative value in case of
 *                 invalid input
 */
int base64_decode(unsigned char* out, const char* in, int out_size);

/**
 * Encode data to base64 and nullptr-terminate.
 *
 * @param out      buffer for encoded data
 * @param out_size size in bytes of the output buffer, must be at
 *                 least BASE64_SIZE(in_size)
 * @param in_size  size in bytes of the 'in' buffer
 * @return         'out' or nullptr in case of error
 */
char* base64_encode(char* out, int out_size, const unsigned char* in, int in_size);

/**
 * Calculate the output size needed to base64-encode x bytes.
 */
#define BASE64_SIZE(x)  (((x)+2) / 3 * 4 + 1)

#define SIZEOF_ARRAY(arr)        (sizeof(arr) / sizeof(arr[0]))


#endif /* BASE64_H */
