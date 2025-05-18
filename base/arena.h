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

#ifndef ARENA_H
#define ARENA_H

/// Arena implementation from:
/// * https://www.gingerbill.org/article/2019/02/08/memory-allocation-strategies-002/

#define DEFAULT_ARENA_ALIGNMENT (2*sizeof(void*))

struct arena
{
    byte* buf;
    size buf_len;
    size prev_offset;
    size curr_offset;
};
typedef struct arena arena_t;

bool ptr_is_power_of_two(uintptr x);

void ARENA_Initialize(arena_t* arena, void* buffer, size buffer_length);
void ARENA_Free(arena_t* arena);

uintptr ARENA_AlignForward(uintptr ptr, size align);
void* ARENA_AllocAligned(arena_t* arena, size sz, size align);
void* ARENA_Alloc(arena_t* arena, size sz);
void* ARENA_ResizeAligned(arena_t* arena, void* old_memory, size old_sz, size new_sz, size align);
void* ARENA_Resize(arena_t* arena, void* old_memory, size old_sz, size new_sz);

#endif // ARENA_H
