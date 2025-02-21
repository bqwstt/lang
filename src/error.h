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

#ifndef ERROR_H
#define ERROR_H

enum ErrorKind
{
    ERROR_NO_ERROR,
    ERROR_UNEXPECTED_TOKEN,
};
typedef enum ErrorKind ErrorKind;

struct ScopedError
{
    ErrorKind error_kind;
    Token*    token;

    struct ScopedError* previous_error;
    struct ScopedError* next_error;
};
typedef struct ScopedError ScopedError;

ScopedError error_make_scoped();
void error_push_scope(ScopedError* error, Arena* scratch, ErrorKind kind, Token* token);
void error_report_scope(ScopedError* error);
void error_report(ScopedError* error);

#endif // ERROR_H