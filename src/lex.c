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

bool Lexer_Match(Lexer* lexer, char character)
{
    bool matched = character == Lexer_Peek(lexer);
    if (matched) Lexer_ReadChar(lexer);
    return matched;
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

    while (lexer->current == ' ') {
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
            if (Lexer_Match(lexer, '>')) {
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
            if (Lexer_Match(lexer, '=')) {
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
            if (Lexer_Match(lexer, '=')) {
                token.kind = TK_NOT_EQUALS;
                break;
            }

            token.kind = TK_EXCLAMATION_MARK;
            break;
        case '=':
            if (Lexer_Match(lexer, '=')) {
                token.kind = TK_DOUBLE_EQUALS;
                break;
            }

            if (Lexer_Match(lexer, '>')) {
                token.kind = TK_FAT_ARROW;
                break;
            }
            
            token.kind = TK_EQUALS;
            break;
        case '>':
            if (Lexer_Match(lexer, '=')) {
                token.kind = TK_GREATER_OR_EQUALS_TO;
                break;
            }

            token.kind = TK_GREATER_THAN;
            break;
        case '<':
            if (Lexer_Match(lexer, '=')) {
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
            if (Lexer_Match(lexer, ']')) {
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
    token.kind = TK_NUMBER_LITERAL;
    token.literal = number;
    return token;
}
