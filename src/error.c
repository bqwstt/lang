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

scoped_error_t ERROR_MakeScoped()
{
    scoped_error_t scoped_error;
    scoped_error.error_kind = ERRORK_NO_ERROR;
    scoped_error.token = null;
    scoped_error.previous_error = null;
    scoped_error.next_error = null;
    return scoped_error;
}

void ERROR_PushScope(scoped_error_t* root, arena_t* scratch, error_kind_t kind, token_t* token)
{
    scoped_error_t* tail = root;
    while (tail != null && tail->next_error != null) {
        tail = tail->next_error;
    }

    scoped_error_t* new_error = cast(scoped_error_t*) ARENA_Alloc(scratch, sizeof(scoped_error_t));
    new_error->error_kind = kind;
    new_error->token = token;
    new_error->previous_error = tail;
    new_error->next_error = null;
    tail->next_error = new_error;
}

void ERROR_ReportScope(scoped_error_t* error)
{
    // @TODO: We want to output the error back to the user in a condensed form,
    // i.e., with all errors applied to a single message. If possible, we don't want
    // the user to think much about what the error is, but rather make it clearly visible
    // where the errors are.
    scoped_error_t* current_error = error;
    while (current_error != null) {
        ERROR_Report(current_error);
        current_error = current_error->next_error;
    }
}

void ERROR_Report(scoped_error_t* error)
{
    switch (error->error_kind) {
        case ERRORK_UNEXPECTED_TOKEN: {
            fprintf(stderr, "unexpected token: found %s.\n", token_names[error->token->kind]);
            break;
        }
        default:
            break;
    }
}
