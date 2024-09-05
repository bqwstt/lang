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

// @TODO: may want to use dynamic alloc
#define PARSER_NODE_LIMIT 131072
static byte * node_buffer[PARSER_NODE_LIMIT];

parser_t parser_create(lexer_t * lexer)
{
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
    lexer_destroy(parser->lexer);
    arena_free(&parser->node_arena);
}

boolean parser_kind_is_operator(token_kind_t kind)
{
    return kind == TK_PLUS
        || kind == TK_MINUS
        || kind == TK_MULT
        || kind == TK_DIV;
}

uint parser_operator_precedence(token_kind_t op)
{
    // @TODO: add more ops as we go (^)
    switch (op) {
        case TK_MINUS:
        case TK_PLUS:
            return 1;
        case TK_MULT:
        case TK_DIV:
            return 2;
        default:
            return 0;
    }
}

op_assoc_t parser_operator_associativity(token_kind_t op)
{
    switch (op) {
        case TK_MINUS:
        case TK_PLUS:
        case TK_MULT:
        case TK_DIV:
            return ASSOC_LEFT;
        /* case ^ ... ASSOC_RIGHT */
        default:
            return ASSOC_UNKNOWN;
    }
}

void parser_consume_token(parser_t * parser)
{
    parser->current_token = parser->next_token;
    if (parser->current_token.kind == TK_ILLEGAL || parser->current_token.kind == TK_EOF)
        return;

    parser->next_token = lexer_consume_token(parser->lexer);
}

void parser_parse(parser_t * parser)
{
    ast_program_t program = ast_create_program();

    while (parser->current_token.kind != TK_EOF) {
        ast_statement_t * stmt = parser_parse_statement(parser);

        if (stmt != NULL) {
            program.statements[program.statements_len++] = stmt;
        }

        // @TODO: we may want to reduce all tokens, not just 1?
        parser_consume_token(parser);
    }
    
    parser_dump_ast(parser, &program);
    return;
}

ast_statement_t * parser_parse_statement(parser_t * parser)
{
    ast_statement_t * stmt = ast_create_node(&parser->node_arena);
    assert(stmt);

    stmt->kind = ASTK_STMT;

    if (parser->current_token.kind == TK_NUMBER) {
        byte * scratch_node_buffer[16384];
        arena_t scratch;
        arena_init(&scratch, scratch_node_buffer, 16384);
        stmt = parser_parse_expression(parser, 0, &scratch);
    }

    return stmt;
}

ast_statement_t * parser_parse_expression(parser_t * parser, uint8 prec_limit, arena_t * scratch)
{
    ast_statement_t * expr = ast_create_node(scratch);
    assert(expr);

    expr->kind  = ASTK_EXPR;
    expr->token = parser->current_token;

    while (parser_kind_is_operator(parser->next_token.kind)) {
        uint8 current_prec = parser_operator_precedence(parser->next_token.kind); 
        if (current_prec <= prec_limit) {
            return expr;
        }

        token_t op_token = parser->next_token;

        // Consume both the number and the operator.
        parser_consume_token(parser);
        parser_consume_token(parser);

        ast_statement_t * right = parser_parse_expression(parser, current_prec, scratch);
        ast_binary_op_t * binop = ast_create_node_sized(scratch, sizeof(ast_binary_op_t));
        assert(binop);

        binop->kind  = ASTK_BINARY;
        binop->left  = expr;
        binop->right = right;
        binop->token = op_token;

        expr = (ast_statement_t *)binop;
    };

    return expr;
}

void parser_dump_ast(parser_t * parser, ast_program_t * root)
{
    printf("│[Program]\n");
    for (uint i = 0; i < root->statements_len; ++i) {
        ast_node_t * node = root->statements[i];
        printf("└──│[Statement]\n");
        printf("   └───[%s] ", ast_node_id(node));
        ast_node_dump(node);
        printf("\n");
    }
}
