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

#define is_digit(c) ((c) >= '0' && (c) <= '9')

#define LEXER_LITERAL_LIMIT 131072
static byte * literal_buffer[LEXER_LITERAL_LIMIT];

lexer_t lexer_create(string_t code)
{
    arena_t literal_arena;
    arena_init(&literal_arena, literal_buffer, LEXER_LITERAL_LIMIT);

    lexer_t lexer;
    lexer.code = code;
    lexer.literal_arena = literal_arena;
    lexer.cur_pos = 0;
    lexer.next_pos = 0;
    lexer.spaces = 0;
    lexer.after_newline = false;
    return lexer;
}

void lexer_destroy(lexer_t * lexer)
{
    arena_free(&lexer->literal_arena);
}

char lexer_peek(lexer_t * lexer)
{
    assert(lexer->code.data);
    return lexer->code.data[lexer->next_pos];
}

void lexer_read_char(lexer_t * lexer)
{
    assert(lexer->code.data);
    lexer->current = lexer->code.data[lexer->next_pos];
    lexer->cur_pos = lexer->next_pos++;
}

token_t lexer_consume_token(lexer_t * lexer)
{
    token_t token;
    token.kind = TK_UNKNOWN;
    token.literal = string("");

    assert(lexer->code.data);

    if (lexer->next_pos < lexer->code.len
        && lexer->code.data[lexer->next_pos] != '\0') {
        lexer_read_char(lexer);
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

        lexer_read_char(lexer);
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
            if (is_digit(lexer->current)) {
                token = lexer_consume_number(lexer);
                break;
            }

            token.kind = TK_ILLEGAL;
            break;
    }

    return token;
}

token_t lexer_consume_number(lexer_t * lexer)
{
    string_t number = string_from_char(lexer->current, &lexer->literal_arena);

    char next_digit = lexer_peek(lexer);
    while (is_digit(next_digit) || next_digit == '.') {
        number = string_extend(number, next_digit, &lexer->literal_arena);
        lexer_read_char(lexer);
        next_digit = lexer_peek(lexer);
    }

    token_t token;
    token.kind = TK_NUMBER;
    token.literal = number;
    return token;
}
