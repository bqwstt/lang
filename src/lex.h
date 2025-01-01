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

    // One-character tokens
    TK_PLUS, // +
    TK_MINUS, // -
    TK_ASTERISK, // *
    TK_SLASH, // /
    TK_EXPONENT, // ^
    TK_DOT, // .
    TK_COMMA, // ,
    TK_COLON, // :
    TK_SEMICOLON, // ;
    TK_QUESTION_MARK, // ?
    TK_EXCLAMATION_MARK, // !
    TK_EQUALS, // =
    TK_GREATER_THAN, // >
    TK_LESS_THAN, // <
    TK_CURLY_BRACE_OPEN, // {
    TK_CURLY_BRACE_CLOSE, // }
    TK_SQUARE_BRACKET_OPEN, // [
    TK_SQUARE_BRACKET_CLOSE, // ]
    TK_PARENTHESIS_OPEN, // (
    TK_PARENTHESIS_CLOSE, // )

    // Two-character tokens
    TK_THIN_ARROW, // ->
    TK_FAT_ARROW, // =>
    TK_DOUBLE_EQUALS, // ==
    TK_NOT_EQUALS, // !=
    TK_GREATER_OR_EQUALS_TO, // >=
    TK_LESS_OR_EQUALS_TO, // <=
    TK_ARRAY_BRACKETS, // []
    TK_ASSIGNMENT_OPERATOR, // :=

    // @TODO: +=, -= etc.
    // @TODO: Maybe we should have a dereference operator for both `*` and `?` types.
    // @TODO: Should we have a "todo" symbol? Maybe `...`...?

    // Literals
    TK_NUMBER_LITERAL,
    TK_STRING_LITERAL,

    // Keywords
    TK_FUNCTION,
    TK_STRUCT,
    TK_ENUM,
    TK_IF,
    TK_ELSE,
    TK_RETURN,
    TK_FOR,
    TK_CASE,

    TK_ILLEGAL,
    TK_EOF,
};
typedef enum TokenKind TokenKind;

static const char* token_names[] = {
    [TK_UNKNOWN] = "unknown symbol",

    // One-character tokens
    [TK_PLUS] = "plus sign",
    [TK_MINUS] = "minus sign",
    [TK_ASTERISK] = "asterisk",
    [TK_SLASH] = "slash",
    [TK_EXPONENT] = "exponent",
    [TK_DOT] = "dot",
    [TK_COMMA] = "comma",
    [TK_COLON] = "colon",
    [TK_SEMICOLON] = "semicolon",
    [TK_QUESTION_MARK] = "question mark",
    [TK_EXCLAMATION_MARK] = "exclamation mark",
    [TK_EQUALS] = "equals sign",
    [TK_GREATER_THAN] = "greather than sign",
    [TK_LESS_THAN] = "less than sign",
    [TK_CURLY_BRACE_OPEN] = "opening curly brace",
    [TK_CURLY_BRACE_CLOSE] = "closing curly brace",
    [TK_SQUARE_BRACKET_OPEN] = "opening square bracket",
    [TK_SQUARE_BRACKET_CLOSE] = "closing square bracket",
    [TK_PARENTHESIS_OPEN] = "opening parenthesis",
    [TK_PARENTHESIS_CLOSE] = "closing parenthesis",

    // Two-character tokens
    [TK_THIN_ARROW] = "thin arrow",
    [TK_FAT_ARROW] = "fat arrow",
    [TK_DOUBLE_EQUALS] = "double equals sign",
    [TK_NOT_EQUALS] = "not equals sign",
    [TK_GREATER_OR_EQUALS_TO] = "greater than or equals to sign",
    [TK_LESS_OR_EQUALS_TO] = "less than or equals to sign",
    [TK_ARRAY_BRACKETS] = "array bracket specifier",
    [TK_ASSIGNMENT_OPERATOR] = "assignment operator",

    // Literals
    [TK_NUMBER_LITERAL] = "number literal",
    [TK_STRING_LITERAL] = "string literal",

    // Keywords
    [TK_FUNCTION] = "function keyword",
    [TK_STRUCT] = "struct keyword",
    [TK_ENUM] = "enum keyword",
    [TK_IF] = "if keyword",
    [TK_ELSE] = "else keyword",
    [TK_RETURN] = "return keyword",
    [TK_FOR] = "for keyword",
    [TK_CASE] = "case keyword",

    [TK_EOF] = "end of file",
    [TK_ILLEGAL] = "illegal token",
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

    char current;
    bool after_newline;

    String code;
};
typedef struct Lexer Lexer;

Lexer Lexer_Create(String code);
void  Lexer_Destroy(Lexer* lexer);
char  Lexer_Peek(Lexer* lexer);
void  Lexer_ReadChar(Lexer* lexer);
bool  Lexer_Match(Lexer* lexer, char character);

Token Lexer_ConsumeToken(Lexer* lexer);
Token Lexer_ConsumeNumber(Lexer* lexer);

#endif // LEX_H
