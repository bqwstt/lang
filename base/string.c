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

String String_Extend(String string, uint8 c, Arena* arena)
{
    uint8* new_data = Arena_Alloc(arena, string.len+1);
    assert(new_data);

    for (size i = 0; i < string.len; ++i) {
        new_data[i] = string.data[i];
    }

    new_data[string.len] = c;
    return STRING_SIZED(new_data, string.len+1);
}

String String_Clone(String string, Arena* arena)
{
    uint8* new_data = Arena_Alloc(arena, string.len);
    assert(new_data);

    for (size i = 0; i < string.len; ++i) {
        new_data[i] = string.data[i];
    }

    return STRING_SIZED(new_data, string.len);
}

String String_FromChar(char c, Arena* arena)
{
    uint8* data = Arena_Alloc(arena, 1);
    assert(data);

    data[0] = (uint8)c;
    return STRING_SIZED(data, 1);
}
