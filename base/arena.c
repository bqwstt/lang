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

boolean uintptr_power_of_two(uintptr x)
{
    return (x & (x-1)) == 0;
}

void arena_init(arena_t * arena, void * buffer, size buffer_length)
{
    arena->buf = (byte *)buffer;
    arena->buf_len = buffer_length;
    arena->curr_offset = 0;
    arena->prev_offset = 0;
}

void arena_free(arena_t * arena)
{
    arena->curr_offset = 0;
    arena->prev_offset = 0;
}

uintptr arena_align_forward(uintptr ptr, size align)
{
    // @TODO: debug_assert(uintptr_power_of_two(align));
    uintptr p = ptr;
    uintptr a = (uintptr)align;
    uintptr modulo = p & (a - 1);

    if (modulo != 0) {
        p += a - modulo;
    }

    return p;
}

void * arena_alloc_align(arena_t * arena, size sz, size align)
{
    uintptr curr_ptr = (uintptr)arena->buf + (uintptr)arena->curr_offset;
    uintptr offset = arena_align_forward(curr_ptr, align);
    offset -= (uintptr)arena->buf;

    // Check for space left
    if (offset + sz <= arena->buf_len) {
        void * ptr = &arena->buf[offset];
        arena->prev_offset = offset;
        arena->curr_offset = offset+sz;

        memset(ptr, 0, sz);
        return ptr;
    }

    return NULL;
}

void * arena_alloc(arena_t * arena, size sz)
{
    return arena_alloc_align(arena, sz, DEFAULT_ARENA_ALIGNMENT);
}

void * arena_resize_align(arena_t * arena, void * old_memory, size old_sz, size new_sz, size align) {
    // @TODO: debug_assert(uintptr_power_of_two(align));
    byte * old_mem = (byte *)old_memory;

    if (old_mem == NULL || old_sz == 0) {
        return arena_alloc_align(arena, new_sz, align);
    } else if (arena->buf <= old_mem && old_mem < arena->buf + arena->buf_len) {
        if (arena->buf + arena->prev_offset == old_mem) {
            arena->curr_offset = arena->prev_offset + new_sz;

            if (new_sz > old_sz)
                memset(&arena->buf[arena->curr_offset], 0, new_sz-old_sz);

            return old_memory;
        } else {
            void * new_memory = arena_alloc_align(arena, new_sz, align);
            size_t copy_sz = 0;
            if (old_sz < new_sz)
                copy_sz = old_sz;
            else
                copy_sz = new_sz;

            memmove(new_memory, old_memory, copy_sz);
            return new_memory;
        }
    } else {
        // @TODO: throw an error
        // "Memory is out of bounds of the buffer in this arena"
        return NULL;
    }
}

void * arena_resize(arena_t * arena, void * old_memory, size old_sz, size new_sz) {
    return arena_resize_align(arena, old_memory, old_sz, new_sz, DEFAULT_ARENA_ALIGNMENT);
}
