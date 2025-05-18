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

string_t STRING_Append(string_t string, uint8 c, arena_t* arena)
{
    // @FIXME: This is not the fastest approach.
    uint8* new_data = cast(uint8*) ARENA_Alloc(arena, string.len+1);
    assert(new_data);

    for (size i = 0; i < string.len; ++i) {
        new_data[i] = string.data[i];
    }

    new_data[string.len] = c;
    return STRING_SIZED(new_data, string.len+1);
}

string_t STRING_Clone(string_t string, arena_t* arena)
{
    uint8* new_data = cast(uint8*) ARENA_Alloc(arena, string.len);
    assert(new_data);

    for (size i = 0; i < string.len; ++i) {
        new_data[i] = string.data[i];
    }

    return STRING_SIZED(new_data, string.len);
}

string_t STRING_FromChar(char c, arena_t* arena)
{
    uint8* data = cast(uint8*) ARENA_Alloc(arena, 1);
    assert(data);

    data[0] = cast(uint8) c;
    return STRING_SIZED(data, 1);
}

bool STRING_Equals(string_t* string1, string_t* string2) {
    if (string1->len != string2->len) return false;
    for (size i = 0; i < string1->len; ++i) {
        if (string1->data[i] != string2->data[i]) {
            return false;
        }
    }

    return true;
}
