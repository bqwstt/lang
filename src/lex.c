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

#define IS_DIGIT(c) ((c) >= '0' && (c) <= '9')

#define LEXER_LITERAL_LIMIT 131072
static byte* literal_buffer[LEXER_LITERAL_LIMIT];

Lexer Lexer_Create(String code)
{
    Arena literal_arena;
    Arena_Initialize(&literal_arena, literal_buffer, LEXER_LITERAL_LIMIT);

    Lexer lexer;
    lexer.code = code;
    lexer.literal_arena = literal_arena;
    lexer.cur_pos = 0;
    lexer.next_pos = 0;
    lexer.spaces = 0;
    lexer.after_newline = false;
    return lexer;
}

void Lexer_Destroy(Lexer* lexer)
{
    Arena_Free(&lexer->literal_arena);
}

char Lexer_Peek(Lexer* lexer)
{
    assert(lexer->code.data);
    return lexer->code.data[lexer->next_pos];
}

void Lexer_ReadChar(Lexer* lexer)
{
    assert(lexer->code.data);
    lexer->current = lexer->code.data[lexer->next_pos];
    lexer->cur_pos = lexer->next_pos++;
}

Token Lexer_ConsumeToken(Lexer* lexer)
{
    Token token;
    token.kind = TK_UNKNOWN;
    token.literal = STRING("");

    assert(lexer->code.data);

    if (lexer->next_pos < lexer->code.len
        && lexer->code.data[lexer->next_pos] != '\0') {
        Lexer_ReadChar(lexer);
    } else {
        token.kind = TK_EOF;
        return token;
    }

    // @TODO: handle blank lines?
    while (lexer->current == ' ') {
        lexer->spaces++;

        if (lexer->after_newline && lexer->spaces == 4) {
            token.kind = TK_SEP;
            lexer->after_newline = false;
            lexer->spaces = 0;
            lexer->next_pos++;
            return token;
        }

        Lexer_ReadChar(lexer);
    }

    switch (lexer->current) {
        case '\n':
            lexer->after_newline = true;
            break;
        case ' ':
            break;
        case '+':
            token.kind = TK_PLUS;
            break;
        case '-':
            token.kind = TK_MINUS;
            break;
        case '*':
            token.kind = TK_MULT;
            break;
        case '/':
            token.kind = TK_DIV;
            break;
        case '^':
            token.kind = TK_EXP;
            break;
        default:
            if (IS_DIGIT(lexer->current)) {
                token = Lexer_ConsumeNumber(lexer);
                break;
            }

            token.kind = TK_ILLEGAL;
            break;
    }

    return token;
}

Token Lexer_ConsumeNumber(Lexer* lexer)
{
    String number = String_FromChar(lexer->current, &lexer->literal_arena);

    char next_digit = Lexer_Peek(lexer);
    while (IS_DIGIT(next_digit) || next_digit == '.') {
        number = String_Extend(number, next_digit, &lexer->literal_arena);
        Lexer_ReadChar(lexer);
        next_digit = Lexer_Peek(lexer);
    }

    Token token;
    token.kind = TK_NUMBER;
    token.literal = number;
    return token;
}
