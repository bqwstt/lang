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

lexer_t lexer_create(stringview_t code)
{
    lexer_t lexer;
    lexer.code = code;
    lexer.cur_pos = 0;
    lexer.next_pos = 0;
    lexer.spaces = 0;
    lexer.after_newline = false;
    return lexer;
}

void lexer_read_char(lexer_t * lexer)
{
    if (lexer->code.contents[lexer->next_pos] == '\0') {
        //  @TODO: I don't think this gets hit.
        lexer->current = 0;
        return;
    }

    lexer->current = lexer->code.contents[lexer->next_pos];
    lexer->cur_pos = lexer->next_pos++;
}

token_t lexer_consume_token(lexer_t * lexer)
{
    if (lexer->next_pos < lexer->code.len)
        lexer_read_char(lexer);

    token_t token;
    switch (lexer->current) {
        case '\n':
            lexer->after_newline = true;
            break;
        case ' ':
            lexer->spaces++;

            if (lexer->after_newline && lexer->spaces == 4) {
                token.kind = TK_SEP;
                lexer->after_newline = false;
                lexer->spaces = 0;
                lexer->next_pos++;
            }
            return token;
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
        default:
            break;
    }

    return token;
}
