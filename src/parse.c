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

parser_t parser_create(lexer_t * lexer)
{
    byte * node_buffer[PARSER_NODE_LIMIT];

    arena_t node_arena;
    arena_init(&node_arena, node_buffer, PARSER_NODE_LIMIT);

    parser_t parser;
    parser.lexer = lexer;
    parser.node_arena = node_arena;
    parser_consume_token(&parser);
    parser_consume_token(&parser);
    return parser;
}

void parser_destroy(parser_t * parser)
{
    arena_free(&parser->node_arena);
}

void parser_consume_token(parser_t * parser)
{
    parser->current_token = parser->next_token;
    parser->next_token = lexer_consume_token(parser->lexer);
    return;
}

void parser_parse(parser_t * parser)
{
    parser_parse_program(parser);
    return;
}

void parser_parse_program(parser_t * parser)
{
    ast_program_t program = ast_create_program();

    if (parser->current_token.kind != TK_EOF) {
        printf("Yey! starting to parse!\n");
        ast_statement_t * stmt = parser_parse_statement(parser);
    }
    
    return;
}

ast_statement_t * parser_parse_statement(parser_t * parser)
{
    ast_statement_t * stmt = ast_create_node(&parser->node_arena);
    if (parser->current_token.kind == TK_MINUS) {
        // @TODO: check other ops
        return (ast_statement_t *)parser_parse_binary_op(parser);
    }

    return stmt;
}

ast_binary_op_t * parser_parse_binary_op(parser_t * parser)
{
    printf("HELLO FRIENDS! parsing binary op!\n");
    ast_binary_op_t * op = (ast_binary_op_t *)ast_create_node(&parser->node_arena);
    return op;
}
