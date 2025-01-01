// Copyright 2024 Benjamín García Roqués <benjamingarciaroques@gmail.com>

// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:

// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#ifndef LIBC_H
#define LIBC_H

// These are memset() and memmove() implementations from gcc's libc.
// This file exists just because I don't want to include string.h.
void* memset(void* dest, int val, size len)
{
    byte* ptr = dest;
    while (len-- > 0)*ptr++ = val;

    return dest;
}

void* memmove(void* dest, const void* src, size len)
{
    char* d = dest;
    const char* s = src;

    if (d < s)
        while (len--)*d++ =*s++;
    else {
        char* lasts = s + (len-1);
        char* lastd = d + (len-1);

        while (len--)*lastd-- = *lasts--;
    }

    return dest;
}

#endif // LIBC_H
