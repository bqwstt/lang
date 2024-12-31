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

enum TokenKind
{
    TK_UNKNOWN,

    TK_SEP,
    TK_MINUS,
    TK_PLUS,
    TK_MULT,
    TK_DIV,
    TK_EXP,

    TK_NUMBER,

    TK_ILLEGAL,
    TK_EOF,
};
typedef enum TokenKind TokenKind;

static const char* token_names[] = {
    [TK_ILLEGAL] = "illegal",

    [TK_SEP] = "separation",
    [TK_MINUS] = "minus",
    [TK_PLUS] = "plus",
    [TK_MULT] = "multiply",
    [TK_DIV] = "division",
    [TK_EXP] = "exponent",

    [TK_NUMBER] = "number",

    [TK_EOF] = "end of file",
    [TK_UNKNOWN] = "unknown",
};

struct Token
{
    TokenKind kind; 
    String literal;
    uint column;
    uint line;
};
typedef struct Token Token;

struct Lexer
{
    uint64 cur_pos;
    uint64 next_pos;

    Arena literal_arena;

    uint8 spaces;
    char current;
    bool after_newline;

    String code;
};
typedef struct Lexer Lexer;

Lexer Lexer_Create(String code);
void  Lexer_Destroy(Lexer* lexer);
char  Lexer_Peek(Lexer* lexer);
void  Lexer_ReadChar(Lexer* lexer);

Token Lexer_ConsumeToken(Lexer* lexer);
Token Lexer_ConsumeNumber(Lexer* lexer);

#endif // LEX_H
