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

Lexer lexer_create(String code)
{
    Arena literal_arena;
    arena_initialize(&literal_arena, literal_buffer, LEXER_LITERAL_LIMIT);

    Lexer lexer;
    lexer.code = code;
    lexer.literal_arena = literal_arena;
    lexer.cur_pos = 0;
    lexer.next_pos = 0;
    lexer.after_newline = false;
    return lexer;
}

void lexer_destroy(Lexer* lexer)
{
    arena_free(&lexer->literal_arena);
}

char lexer_peek(Lexer* lexer)
{
    assert(lexer->code.data);
    return lexer->code.data[lexer->next_pos];
}

void lexer_read_char(Lexer* lexer)
{
    assert(lexer->code.data);
    lexer->current = lexer->code.data[lexer->next_pos];
    lexer->cur_pos = lexer->next_pos++;
}

bool lexer_match(Lexer* lexer, char character)
{
    bool matched = character == lexer_peek(lexer);
    if (matched) lexer_read_char(lexer);
    return matched;
}

Token lexer_consume_token(Lexer* lexer)
{
    Token token;
    token.kind = TK_UNKNOWN;
    token.literal = STRING("");

    assert(lexer->code.data);

    if (lexer->next_pos < lexer->code.len
        && lexer->code.data[lexer->next_pos] != '\0') {
        lexer_read_char(lexer);
    } else {
        token.kind = TK_EOF;
        return token;
    }

    while (lexer->current == ' ') {
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
            if (lexer_match(lexer, '>')) {
                token.kind = TK_THIN_ARROW;
                break;
            }

            token.kind = TK_MINUS;
            break;
        case '*':
            token.kind = TK_ASTERISK;
            break;
        case '/':
            token.kind = TK_SLASH;
            break;
        case '^':
            token.kind = TK_EXPONENT;
            break;
        case '.':
            token.kind = TK_DOT;
            break;
        case ',':
            token.kind = TK_COMMA;
            break;
        case ':':
            if (lexer_match(lexer, '=')) {
                token.kind = TK_ASSIGNMENT_OPERATOR;
                break;
            }

            token.kind = TK_COLON;
            break;
        case ';':
            token.kind = TK_SEMICOLON;
            break;
        case '?':
            token.kind = TK_QUESTION_MARK;
            break;
        case '!':
            if (lexer_match(lexer, '=')) {
                token.kind = TK_NOT_EQUALS;
                break;
            }

            token.kind = TK_EXCLAMATION_MARK;
            break;
        case '=':
            if (lexer_match(lexer, '=')) {
                token.kind = TK_DOUBLE_EQUALS;
                break;
            }

            if (lexer_match(lexer, '>')) {
                token.kind = TK_FAT_ARROW;
                break;
            }
            
            token.kind = TK_EQUALS;
            break;
        case '>':
            if (lexer_match(lexer, '=')) {
                token.kind = TK_GREATER_OR_EQUALS_TO;
                break;
            }

            token.kind = TK_GREATER_THAN;
            break;
        case '<':
            if (lexer_match(lexer, '=')) {
                token.kind = TK_LESS_OR_EQUALS_TO;
                break;
            }

            token.kind = TK_LESS_THAN;
            break;
        case '{':
            token.kind = TK_CURLY_BRACE_OPEN;
            break;
        case '}':
            token.kind = TK_CURLY_BRACE_CLOSE;
            break;
        case '[':
            if (lexer_match(lexer, ']')) {
                token.kind = TK_ARRAY_BRACKETS;
                break;
            }

            token.kind = TK_SQUARE_BRACKET_OPEN;
            break;
        case ']':
            token.kind = TK_SQUARE_BRACKET_CLOSE;
            break;
        case '(':
            token.kind = TK_PARENTHESIS_OPEN;
            break;
        case ')':
            token.kind = TK_PARENTHESIS_CLOSE;
            break;
        default:
            if (IS_DIGIT(lexer->current)) {
                token = lexer_consume_number(lexer);
                break;
            }

            token.kind = TK_ILLEGAL;
            break;
    }

    return token;
}

Token lexer_consume_number(Lexer* lexer)
{
    String number = string_from_char(lexer->current, &lexer->literal_arena);

    char next_digit = lexer_peek(lexer);
    uint num_dots = 0;
    while (IS_DIGIT(next_digit) || next_digit == '.') {
        number = string_append(number, next_digit, &lexer->literal_arena);
        lexer_read_char(lexer);
        next_digit = lexer_peek(lexer);
        num_dots += cast(int) (next_digit == '.');
    }

    Token token;
    token.kind = num_dots > 1 ? TK_ILLEGAL : TK_NUMBER_LITERAL;
    token.literal = number;
    return token;
}

Token lexer_consume_string(Lexer* lexer)
{
    // @TODO: consume identifiers + keywords
    Token token;
    return token;
}
