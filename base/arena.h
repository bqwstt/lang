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

#define DEFAULT_ARENA_ALIGNMENT (2*sizeof(void *))

typedef struct
{
    byte * buf;
    size buf_len;
    size prev_offset;
    size curr_offset;
} arena_t;

boolean uintptr_power_of_two(uintptr x);

void arena_init(arena_t * arena, void * buffer, size buffer_length);
void arena_free(arena_t * arena);
uintptr arena_align_forward(uintptr ptr, size align);
void * arena_alloc_align(arena_t * arena, size sz, size align);
void * arena_alloc(arena_t * arena, size sz);
void * arena_resize_align(arena_t * arena, void * old_memory, size old_sz, size new_sz, size align);
void * arena_resize(arena_t * arena, void * old_memory, size old_sz, size new_sz);

#endif // ARENA_H
