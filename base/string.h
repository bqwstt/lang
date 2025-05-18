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

#ifndef STRING_H
#define STRING_H

// string_t as mentioned in: https://nullprogram.com/blog/2023/10/08/
// Note that there is no distinction between "string" and "string view" here.

struct string
{
    uint8* data;
    size   len;
};
typedef struct string string_t;

#define STRING_SIZED(s, l) cast(string_t) {(uint8*)s, l}
#define STRING(s)          cast(string_t) {(uint8*)s, lengthof(s)}

string_t STRING_Append(string_t string, uint8 c, arena_t* arena);
string_t STRING_Clone(string_t string, arena_t* arena);
string_t STRING_FromChar(char c, arena_t* arena);
bool STRING_Equals(string_t* string1, string_t* string2);

#endif // STRING_H
