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
#define IS_ALPHA(c) (((c) >= 'A' && (c) <= 'Z') || ((c) >= 'a' && (c) <= 'z'))
#define IS_ALPHANUMERIC(c) (IS_DIGIT(c) || (IS_ALPHA(c)))

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
    assert(lexer->code.data);

    Token token;
    token.kind = TK_UNKNOWN;
    token.literal = STRING("");

    if (lexer->next_pos < lexer->code.len
        && lexer->code.data[lexer->next_pos] != '\0') {
        lexer_read_char(lexer);
    } else {
        token.kind = TK_EOF;
        return token;
    }

    while (lexer->current == ' ' || lexer->current == '\n') {
        lexer_read_char(lexer);
    }

    switch (lexer->current) {
        case '+':
            token.kind = TK_PLUS;
            token.literal = STRING("+");
            break;
        case '-':
            if (lexer_match(lexer, '>')) {
                token.kind = TK_THIN_ARROW;
                token.literal = STRING("->");
                break;
            }

            token.kind = TK_MINUS;
            token.literal = STRING("-");
            break;
        case '*':
            token.kind = TK_ASTERISK;
            token.literal = STRING("*");
            break;
        case '/':
            token.kind = TK_SLASH;
            token.literal = STRING("/");
            break;
        case '^':
            token.kind = TK_EXPONENT;
            token.literal = STRING("^");
            break;
        case '.':
            token.kind = TK_DOT;
            token.literal = STRING(".");
            break;
        case ',':
            token.kind = TK_COMMA;
            token.literal = STRING(",");
            break;
        case ':':
            if (lexer_match(lexer, '=')) {
                token.kind = TK_ASSIGNMENT_OPERATOR;
                token.literal = STRING(":=");
                break;
            }

            if (lexer_match(lexer, ':')) {
                token.kind = TK_DEFINITION_OPERATOR;
                token.literal = STRING("::");
                break;
            }

            token.kind = TK_COLON;
            token.literal = STRING(":");
            break;
        case ';':
            token.kind = TK_SEMICOLON;
            token.literal = STRING(";");
            break;
        case '?':
            token.kind = TK_QUESTION_MARK;
            token.literal = STRING("?");
            break;
        case '|':
            if (lexer_match(lexer, '|')) {
                token.kind = TK_LOGICAL_OR;
                token.literal = STRING("||");
                break;
            }

            // @TODO: bitwise OR
        case '&':
            if (lexer_match(lexer, '&')) {
                token.kind = TK_LOGICAL_AND;
                token.literal = STRING("&&");
                break;
            }

            // @TODO: bitwise AND
        case '!':
            if (lexer_match(lexer, '=')) {
                token.kind = TK_NOT_EQUALS;
                token.literal = STRING("!=");
                break;
            }

            token.kind = TK_EXCLAMATION_MARK;
            token.literal = STRING("!");
            break;
        case '=':
            if (lexer_match(lexer, '=')) {
                token.kind = TK_DOUBLE_EQUALS;
                token.literal = STRING("==");
                break;
            }

            if (lexer_match(lexer, '>')) {
                token.kind = TK_FAT_ARROW;
                token.literal = STRING("=>");
                break;
            }
            
            token.kind = TK_EQUALS;
            token.literal = STRING("=");
            break;
        case '>':
            if (lexer_match(lexer, '=')) {
                token.kind = TK_GREATER_OR_EQUALS_TO;
                token.literal = STRING(">=");
                break;
            }

            token.kind = TK_GREATER_THAN;
            token.literal = STRING(">");
            break;
        case '<':
            if (lexer_match(lexer, '=')) {
                token.kind = TK_LESS_OR_EQUALS_TO;
                token.literal = STRING("<=");
                break;
            }

            token.kind = TK_LESS_THAN;
            token.literal = STRING("<");
            break;
        case '{':
            token.kind = TK_CURLY_BRACE_OPEN;
            token.literal = STRING("{");
            break;
        case '}':
            token.kind = TK_CURLY_BRACE_CLOSE;
            token.literal = STRING("}");
            break;
        case '[':
            if (lexer_match(lexer, ']')) {
                token.kind = TK_ARRAY_BRACKETS;
                token.literal = STRING("[]");
                break;
            }

            token.kind = TK_SQUARE_BRACKET_OPEN;
            token.literal = STRING("[");
            break;
        case ']':
            token.kind = TK_SQUARE_BRACKET_CLOSE;
            token.literal = STRING("]");
            break;
        case '(':
            token.kind = TK_PARENTHESIS_OPEN;
            token.literal = STRING("(");
            break;
        case ')':
            token.kind = TK_PARENTHESIS_CLOSE;
            token.literal = STRING(")");
            break;
        default:
            if (IS_DIGIT(lexer->current)) {
                token = lexer_consume_number(lexer);
                break;
            } else if (IS_ALPHA(lexer->current)) {
                token = lexer_consume_string(lexer);
                break;
            }

            token.kind = TK_ILLEGAL;
            break;
    }

    token_dump(&token);
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
    String identifier = string_from_char(lexer->current, &lexer->literal_arena);
    char next_character = lexer_peek(lexer);
    while (IS_ALPHANUMERIC(next_character) || next_character == '_') {
        identifier = string_append(identifier, next_character, &lexer->literal_arena);
        lexer_read_char(lexer);
        next_character = lexer_peek(lexer);
    }

    Token token;
    token.kind = TK_IDENTIFIER;

    // @TODO: This may be a hashmap.
    // But for now, it works.
    if (0) {}
    else if (string_equals(&identifier, &STRING("struct"))) { token.kind = TK_STRUCT; }
    else if (string_equals(&identifier, &STRING("enum")))   { token.kind = TK_ENUM; }
    else if (string_equals(&identifier, &STRING("if")))     { token.kind = TK_IF; }
    else if (string_equals(&identifier, &STRING("else")))   { token.kind = TK_ELSE; }
    else if (string_equals(&identifier, &STRING("return"))) { token.kind = TK_RETURN; }
    else if (string_equals(&identifier, &STRING("for")))    { token.kind = TK_FOR; }

    token.literal = identifier;
    return token;
}

void token_dump(Token* token) {
    printf("Token [%s] (literal='%s')\n", token_names[token->kind], token->literal.data);
}