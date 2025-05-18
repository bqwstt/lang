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

enum error_kind
{
    ERRORK_NO_ERROR,
    ERRORK_UNEXPECTED_TOKEN,
};
typedef enum error_kind error_kind_t;

struct scoped_error
{
    error_kind_t error_kind;
    token_t* token;

    struct scoped_error* previous_error;
    struct scoped_error* next_error;
};
typedef struct scoped_error scoped_error_t;

scoped_error_t ERROR_MakeScoped();
void ERROR_PushScope(scoped_error_t* error, arena_t* scratch, error_kind_t kind, token_t* token);
void ERROR_ReportScope(scoped_error_t* error);
void ERROR_Report(scoped_error_t* error);

#endif // ERROR_H
