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

#ifndef LEX_H
#define LEX_H

typedef enum
{
    TK_UNKNOWN,

    TK_SEP,
    TK_MINUS,
    TK_PLUS,
    TK_MULT,
    TK_DIV,

    TK_NUMBER,

    TK_ILLEGAL,
    TK_EOF,
} token_kind_t;

static const char * token_names[] = {
    [TK_ILLEGAL] = "illegal",

    [TK_SEP] = "separation",
    [TK_MINUS] = "minus",
    [TK_PLUS] = "plus",
    [TK_MULT] = "multiply",
    [TK_DIV] = "division",

    [TK_NUMBER] = "number",

    [TK_EOF] = "end of file",
    [TK_UNKNOWN] = "unknown",
};

typedef struct
{
    token_kind_t kind; 
    string_t literal;
    uint column;
    uint line;
} token_t;

typedef struct
{
    uint64 cur_pos;
    uint64 next_pos;

    arena_t literal_arena;

    uint8 spaces;
    char current;
    boolean after_newline;

    string_t code;
} lexer_t;

lexer_t lexer_create(string_t code);
void lexer_destroy(lexer_t * lexer);
char lexer_peek(lexer_t * lexer);
void lexer_read_char(lexer_t * lexer);

token_t lexer_consume_token(lexer_t * lexer);
token_t lexer_consume_number(lexer_t * lexer);

#endif // LEX_H
