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
    TK_CONSTANT_DEFINITION_OPERATOR, // ::
    TK_LOGICAL_OR, // ||
    TK_LOGICAL_AND, // &&

    // @TODO: +=, -= etc.
    // @TODO: Maybe we should have a dereference operator for both `*` and `?` types.
    // @TODO: Should we have a "todo" symbol? Maybe `...`...?

    // Literals
    TK_NUMBER_LITERAL,
    TK_STRING_LITERAL,

    // Keywords
    TK_STRUCT,
    TK_ENUM,
    TK_IF,
    TK_ELSE,
    TK_RETURN,
    TK_FOR,

    TK_IDENTIFIER,

    TK_ILLEGAL,
    TK_EOF,
};
typedef enum TokenKind TokenKind;

static const char* token_names[] = {
    [TK_UNKNOWN] = "an unknown symbol",

    // One-character tokens
    [TK_PLUS] = "a plus sign",
    [TK_MINUS] = "a minus sign",
    [TK_ASTERISK] = "an asterisk",
    [TK_SLASH] = "a slash",
    [TK_EXPONENT] = "an exponent",
    [TK_DOT] = "a dot",
    [TK_COMMA] = "a comma",
    [TK_COLON] = "a colon",
    [TK_SEMICOLON] = "a semicolon",
    [TK_QUESTION_MARK] = "a question mark",
    [TK_EXCLAMATION_MARK] = "an exclamation mark",
    [TK_EQUALS] = "an equals sign",
    [TK_GREATER_THAN] = "a greater than sign",
    [TK_LESS_THAN] = "a less than sign",
    [TK_CURLY_BRACE_OPEN] = "an opening curly brace",
    [TK_CURLY_BRACE_CLOSE] = "a closing curly brace",
    [TK_SQUARE_BRACKET_OPEN] = "an opening square bracket",
    [TK_SQUARE_BRACKET_CLOSE] = "a closing square bracket",
    [TK_PARENTHESIS_OPEN] = "an opening parenthesis",
    [TK_PARENTHESIS_CLOSE] = "a closing parenthesis",

    // Two-character tokens
    [TK_THIN_ARROW] = "a return type specifier",
    [TK_FAT_ARROW] = "a type matching case",
    [TK_DOUBLE_EQUALS] = "a double equals sign",
    [TK_NOT_EQUALS] = "a not equals sign",
    [TK_GREATER_OR_EQUALS_TO] = "a greater than or equals to sign",
    [TK_LESS_OR_EQUALS_TO] = "a less than or equals to sign",
    [TK_ARRAY_BRACKETS] = "an array bracket specifier",
    [TK_ASSIGNMENT_OPERATOR] = "an assignment operator",
    [TK_CONSTANT_DEFINITION_OPERATOR] = "a definition operator",
    [TK_LOGICAL_OR] = "a logical OR operator",
    [TK_LOGICAL_AND] = "a logical AND operator",

    // Literals
    [TK_NUMBER_LITERAL] = "a number literal",
    [TK_STRING_LITERAL] = "a string literal",

    // Keywords
    [TK_STRUCT] = "a struct keyword",
    [TK_ENUM] = "an enum keyword",
    [TK_IF] = "an if keyword",
    [TK_ELSE] = "an else keyword",
    [TK_RETURN] = "a return keyword",
    [TK_FOR] = "a for keyword",

    [TK_IDENTIFIER] = "an identifier",

    [TK_EOF] = "the end of file",
    [TK_ILLEGAL] = "an illegal token",
};

struct Token
{
    TokenKind kind; 
    String literal;
    // @TODO: remove these and use a pointer to location instead
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

    String code;
};
typedef struct Lexer Lexer;

void token_dump(Token* token);

Lexer lexer_create(String code);
void  lexer_destroy(Lexer* lexer);
char  lexer_peek(Lexer* lexer);
void  lexer_read_char(Lexer* lexer);
bool  lexer_match(Lexer* lexer, char character);

Token lexer_consume_token(Lexer* lexer);
Token lexer_consume_number(Lexer* lexer);
Token lexer_consume_string(Lexer* lexer);

#endif // LEX_H
