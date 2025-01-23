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

ScopedError error_make_scoped()
{
    ScopedError scoped_error;
    scoped_error.error_kind = ERROR_NO_ERROR;
    scoped_error.token = null;
    scoped_error.previous_error = null;
    scoped_error.next_error = null;
    return scoped_error;
}

void error_push_scope(ScopedError* root, Arena* scratch, ErrorKind kind, Token* token)
{
    ScopedError* tail = root;
    while (tail != null && tail->next_error != null) {
        tail = tail->next_error;
    }

    ScopedError* new_error = cast(ScopedError*) arena_alloc(scratch, sizeof(ScopedError));
    new_error->error_kind = kind;
    new_error->token = token;
    new_error->previous_error = tail;
    new_error->next_error = null;
    tail->next_error = new_error;
}

void error_report_scope(ScopedError* error)
{
    // @TODO: We want to output the error back to the user in a condensed form,
    // i.e., with all errors applied to a single message. If possible, we don't want
    // the user to think much about what the error is, but rather make it clearly visible
    // where the errors are.
    ScopedError* current_error = error;
    while (current_error != null) {
        error_report(current_error);
        current_error = current_error->next_error;
    }
}

void error_report(ScopedError* error)
{
    switch (error->error_kind) {
        case ERROR_UNEXPECTED_TOKEN: {
            fprintf(stderr, "unexpected token: found %s.\n", token_names[error->token->kind]);
            break;
        }
        default:
            break;
    }
}