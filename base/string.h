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

// String as mentioned in: https://nullprogram.com/blog/2023/10/08/
// Note that there is no distinction between "string" and "string view" here.

struct String
{
    uint8* data;
    size   len;
};
typedef struct String String;

#define STRING_SIZED(s, l) cast(String) {(uint8*)s, l}
#define STRING(s)          cast(String) {(uint8*)s, lengthof(s)}

String string_append(String string, uint8 c, Arena* arena);
String string_clone(String string, Arena* arena);
String string_from_char(char c, Arena* arena);
bool string_equals(String* string1, String* string2);

#endif // STRING_H
