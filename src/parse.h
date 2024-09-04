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

#ifndef PARSE_H
#define PARSE_H

// @TODO: may want to use dynamic alloc
#define PARSER_NODE_LIMIT 131072
static byte * node_buffer[PARSER_NODE_LIMIT];

typedef struct
{
    lexer_t * lexer;
    arena_t node_arena;

    token_t current_token;
    token_t next_token;
} parser_t;

typedef enum
{
    ASSOC_UNKNOWN,
    ASSOC_RIGHT,
    ASSOC_LEFT,
} op_assoc_t;

parser_t parser_create(lexer_t * lexer);
void parser_destroy(parser_t * parser);
void parser_consume_token(parser_t * parser);
void parser_parse(parser_t * parser);

/* Helpers */
boolean parser_kind_is_operator(token_kind_t kind);
uint parser_operator_precedence(token_kind_t op);
op_assoc_t parser_operator_associativity(token_kind_t op);

/* Parsing functions */
ast_statement_t * parser_parse_statement(parser_t * parser);
ast_statement_t * parser_parse_expression(parser_t * parser, uint8 prec_limit, arena_t * scratch);

void parser_dump_ast(parser_t * parser, ast_program_t * root);

#endif // PARSE_H
