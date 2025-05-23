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

lexer_t LEXER_Create(string_t code)
{
    arena_t literal_arena;
    ARENA_Initialize(&literal_arena, literal_buffer, LEXER_LITERAL_LIMIT);

    lexer_t lexer;
    lexer.code = code;
    lexer.literal_arena = literal_arena;
    lexer.cur_pos = 0;
    lexer.next_pos = 0;
    return lexer;
}

void LEXER_Destroy(lexer_t* lexer)
{
    ARENA_Free(&lexer->literal_arena);
}

char LEXER_Peek(lexer_t* lexer)
{
    assert(lexer->code.data);
    return lexer->code.data[lexer->next_pos];
}

void LEXER_ReadChar(lexer_t* lexer)
{
    assert(lexer->code.data);
    lexer->current = lexer->code.data[lexer->next_pos];
    lexer->cur_pos = lexer->next_pos++;
}

bool LEXER_Match(lexer_t* lexer, char character)
{
    bool matched = character == LEXER_Peek(lexer);
    if (matched) LEXER_ReadChar(lexer);
    return matched;
}

token_t LEXER_ConsumeToken(lexer_t* lexer)
{
    assert(lexer->code.data);

    token_t token;
    token.kind = TK_UNKNOWN;
    token.literal = STRING("");

    if (lexer->next_pos < lexer->code.len
        && lexer->code.data[lexer->next_pos] != '\0') {
        LEXER_ReadChar(lexer);
    } else {
        token.kind = TK_EOF;
        return token;
    }

    while (lexer->current == ' ' || lexer->current == '\n') {
        LEXER_ReadChar(lexer);
    }

    switch (lexer->current) {
        case '+':
            token.kind = TK_PLUS;
            token.literal = STRING("+");
            break;
        case '-':
            if (LEXER_Match(lexer, '>')) {
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
            if (LEXER_Match(lexer, '=')) {
                token.kind = TK_ASSIGNMENT_OPERATOR;
                token.literal = STRING(":=");
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
            if (LEXER_Match(lexer, '|')) {
                token.kind = TK_LOGICAL_OR;
                token.literal = STRING("||");
                break;
            }

            // @TODO: bitwise OR
        case '&':
            if (LEXER_Match(lexer, '&')) {
                token.kind = TK_LOGICAL_AND;
                token.literal = STRING("&&");
                break;
            }

            // @TODO: bitwise AND
        case '!':
            if (LEXER_Match(lexer, '=')) {
                token.kind = TK_NOT_EQUALS;
                token.literal = STRING("!=");
                break;
            }
        case '=':
            if (LEXER_Match(lexer, '=')) {
                token.kind = TK_DOUBLE_EQUALS;
                token.literal = STRING("==");
                break;
            }

            if (LEXER_Match(lexer, '>')) {
                token.kind = TK_FAT_ARROW;
                token.literal = STRING("=>");
                break;
            }
            
            token.kind = TK_EQUALS;
            token.literal = STRING("=");
            break;
        case '>':
            if (LEXER_Match(lexer, '=')) {
                token.kind = TK_GREATER_OR_EQUALS_TO;
                token.literal = STRING(">=");
                break;
            }

            token.kind = TK_GREATER_THAN;
            token.literal = STRING(">");
            break;
        case '<':
            if (LEXER_Match(lexer, '=')) {
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
            if (LEXER_Match(lexer, ']')) {
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
                token = LEXER_ConsumeNumber(lexer);
                break;
            } else if (IS_ALPHA(lexer->current)) {
                token = LEXER_ConsumeString(lexer);
                break;
            }

            token.kind = TK_ILLEGAL;
            break;
    }

    TOKEN_Dump(&token);
    return token;
}

token_t LEXER_ConsumeNumber(lexer_t* lexer)
{
    string_t number = STRING_FromChar(lexer->current, &lexer->literal_arena);

    char next_digit = LEXER_Peek(lexer);
    uint num_dots = 0;
    while (IS_DIGIT(next_digit) || next_digit == '.') {
        number = STRING_Append(number, next_digit, &lexer->literal_arena);
        LEXER_ReadChar(lexer);
        next_digit = LEXER_Peek(lexer);
        num_dots += cast(int) (next_digit == '.');
    }

    token_t token;
    token.kind = num_dots > 1 ? TK_ILLEGAL : TK_NUMBER_LITERAL;
    token.literal = number;
    return token;
}

token_t LEXER_ConsumeString(lexer_t* lexer)
{
    string_t identifier = STRING_FromChar(lexer->current, &lexer->literal_arena);
    char next_character = LEXER_Peek(lexer);
    while (IS_ALPHANUMERIC(next_character) || next_character == '_') {
        identifier = STRING_Append(identifier, next_character, &lexer->literal_arena);
        LEXER_ReadChar(lexer);
        next_character = LEXER_Peek(lexer);
    }

    token_t token;
    token.kind = TK_IDENTIFIER;

    // @PERFORMANCE: This may be a hashmap. But for now, it works.
    if (0) {}
    else if (STRING_Equals(&identifier, &STRING("struct"))) { token.kind = TK_STRUCT; }
    else if (STRING_Equals(&identifier, &STRING("enum")))   { token.kind = TK_ENUM; }
    else if (STRING_Equals(&identifier, &STRING("if")))     { token.kind = TK_IF; }
    else if (STRING_Equals(&identifier, &STRING("else")))   { token.kind = TK_ELSE; }
    else if (STRING_Equals(&identifier, &STRING("return"))) { token.kind = TK_RETURN; }
    else if (STRING_Equals(&identifier, &STRING("for")))    { token.kind = TK_FOR; }
    else if (STRING_Equals(&identifier, &STRING("var")))    { token.kind = TK_VAR; }
    else if (STRING_Equals(&identifier, &STRING("fun")))    { token.kind = TK_FUN; }

    token.literal = identifier;
    return token;
}

void TOKEN_Dump(token_t* token) {
    printf("token_t [%s] (literal='%s')\n", token_names[token->kind], token->literal.data);
}
