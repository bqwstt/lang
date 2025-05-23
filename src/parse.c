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
static byte* node_buffer[PARSER_NODE_LIMIT];

parser_t PARSER_Create(lexer_t* lexer)
{
    arena_t node_arena;
    ARENA_Initialize(&node_arena, node_buffer, PARSER_NODE_LIMIT);

    parser_t parser;
    parser.lexer = lexer;
    parser.node_arena = node_arena;
    PARSER_ConsumeToken(&parser);
    PARSER_ConsumeToken(&parser);
    return parser;
}

void PARSER_Destroy(parser_t* parser)
{
    LEXER_Destroy(parser->lexer);
    ARENA_Free(&parser->node_arena);
}

bool PARSER_TokenKindIsOperator(token_kind_t kind)
{
    return kind == TK_PLUS
        || kind == TK_MINUS
        || kind == TK_ASTERISK
        || kind == TK_SLASH
        || kind == TK_EXPONENT;
}

uint PARSER_OperatorPrecedence(token_kind_t op)
{
    // Based on Pratt's parser logic.
    switch (op) {
        case TK_MINUS:
        case TK_PLUS:
            return 1;
        case TK_ASTERISK:
        case TK_SLASH:
            return 2;
        case TK_EXPONENT:
            return 3;
        default:
            return 0;
    }
}

operator_associativity_type_t PARSER_OperatorAssociativity(token_kind_t op)
{
    switch (op) {
        case TK_MINUS:
        case TK_PLUS:
        case TK_ASTERISK:
        case TK_SLASH:
            return ASSOC_LEFT;
        case TK_EXPONENT:
            return ASSOC_RIGHT;
        default:
            return ASSOC_UNKNOWN;
    }
}

void PARSER_ConsumeToken(parser_t* parser)
{
    parser->current_token = parser->next_token;
    if (parser->current_token.kind == TK_ILLEGAL || parser->current_token.kind == TK_EOF)
        return;

    parser->next_token = LEXER_ConsumeToken(parser->lexer);
}

void PARSER_Parse(parser_t* parser)
{
    ast_program_t program = AST_CreateProgramNode();

    while (parser->current_token.kind != TK_EOF) {
        ast_statement_t* stmt = PARSER_ParseStatement(parser);

        if (stmt != NULL) {
            program.statements[program.statements_len++] = stmt;
        }

        // @TODO: we may want to reduce all tokens, not just 1?
        PARSER_ConsumeToken(parser);
    }
    
    PARSER_DumpAST(parser, &program);
    return;
}

ast_statement_t* PARSER_ParseStatement(parser_t* parser)
{
    ast_statement_t* stmt = AST_CREATE_NODE(&parser->node_arena);
    assert(stmt);

    stmt->kind = ASTK_STMT;

    // @TODO: Check if these micro allocations are a bit too much.
    byte* scratch_node_buffer[16384];
    arena_t scratch;
    ARENA_Initialize(&scratch, scratch_node_buffer, 16384);

    if (parser->current_token.kind == TK_NUMBER_LITERAL) {
        stmt = PARSER_ParseExpression(parser, 0, &scratch);
    } else if (parser->next_token.kind == TK_ASSIGNMENT_OPERATOR) {
        stmt = PARSER_ParseAssignment(parser, &scratch);
    }

    return stmt;
}

ast_statement_t* PARSER_ParseExpression(parser_t* parser, uint8 prec_limit, arena_t* scratch)
{
    // Implementation of a Pratt parser.
    // Wonderful article explaining this algorithm:
    // https://martin.janiczek.cz/2023/07/03/demystifying-pratt-parsers.html
    ast_expression_t* expr = AST_CREATE_NODE(&parser->node_arena);
    assert(expr);

    expr->kind = ASTK_EXPR;
    expr->token = parser->current_token;

    while (PARSER_TokenKindIsOperator(parser->next_token.kind)) {
        uint8 prec = PARSER_OperatorPrecedence(parser->next_token.kind);
        uint8 final_prec = prec;
        if (PARSER_OperatorAssociativity(parser->next_token.kind) == ASSOC_RIGHT) {
            final_prec -= 1;
        }

        if (prec <= prec_limit) {
            return expr;
        }

        token_t op_token = parser->next_token;

        // Consume both the number and the operator.
        PARSER_ConsumeToken(parser);
        PARSER_ConsumeToken(parser);

        ast_expression_t* right = PARSER_ParseExpression(parser, final_prec, scratch);
        ast_binary_op_t* binop = AST_CREATE_NODE_SIZED(scratch, sizeof(ast_binary_op_t));
        assert(binop);

        binop->kind = ASTK_BINARY;
        binop->left = expr;
        binop->right = right;
        binop->token = op_token;

        expr = cast(ast_expression_t*) binop;
    };

    return expr;
}

ast_statement_t* PARSER_ParseAssignment(parser_t* parser, arena_t* scratch)
{
    // @TODO: specifying types (e.g. a: uint = 42)
    ast_declaration_t* decl = AST_CREATE_NODE_SIZED(&parser->node_arena, sizeof(ast_declaration_t));
    assert(decl);

    decl->kind = ASTK_VARIABLE_ASSIGNMENT;
    decl->token = parser->next_token;
    decl->variable.name_with_type = PARSER_ParseNameWithType(parser, scratch);
    PARSER_ConsumeToken(parser); // Consume the assignment operator.
    decl->variable.expression = PARSER_ParseExpression(parser, 0, scratch);

    // Consume the semicolon.
    // @TODO: Throw an error if the semicolon is not found.
    PARSER_ConsumeToken(parser);

    return cast(ast_statement_t*) decl;
}

ast_declaration_t* PARSER_ParseFunction(parser_t* parser, arena_t* scratch)
{
    scoped_error_t scoped_error;
    ast_identifier_t* name = AST_CREATE_NODE(scratch);
    assert(name);

    name->kind = ASTK_IDENTIFIER;
    name->token = parser->current_token;

    ast_declaration_t* decl = AST_CREATE_NODE_SIZED(&parser->node_arena, sizeof(ast_declaration_t));
    decl->kind = ASTK_FUNCTION_DECLARATION;
    decl->token = parser->next_token;

    // @TODO: Make this arena part of some scope node, maybe?
    // @TODO: Check if this arena is needed, as we already have one created in parse_statement?
    byte* scope_arena_buffer[4096];
    arena_t scope_arena;
    ARENA_Initialize(&scope_arena, scope_arena_buffer, 4096);

    // @TODO: Expect parenthesis before consuming the token.
    // Consume name + definition operator + opening parenthesis.
    PARSER_ConsumeToken(parser);
    PARSER_ConsumeToken(parser);
    if (parser->current_token.kind != TK_PARENTHESIS_OPEN) {
        // @FIXME: Provide some kind of "synchronization" to skip to the next valid token.
        ERROR_PushScope(&scoped_error, &scope_arena, ERRORK_UNEXPECTED_TOKEN, &parser->current_token);
    }

    PARSER_ConsumeToken(parser);

    ast_type_signature_t* signature = AST_CREATE_NODE_SIZED(&scope_arena, sizeof(ast_type_signature_t));
    if (parser->current_token.kind != TK_PARENTHESIS_CLOSE) {
        // Parse parameters.
        uint8 i = 0;
        while (true) {
            signature->parameters[i] = PARSER_ParseNameWithType(parser, &scope_arena);
            // @FIXME: we can set kind in name_with_type directly maybe?
            signature->parameters[i++]->name->kind = ASTK_FUNCTION_PARAMETER;

            if (i == MAX_PARAMETERS) {
                // @TODO: Throw an error for missing parenthesis...?
                break;
            }

            if (parser->current_token.kind == TK_PARENTHESIS_CLOSE) {
                PARSER_ConsumeToken(parser);
                break;
            }

            if (parser->current_token.kind != TK_COMMA) {
                ERROR_PushScope(&scoped_error, &scope_arena, ERRORK_UNEXPECTED_TOKEN, &parser->current_token);
            }
            PARSER_ConsumeToken(parser);
        }
    } else {
        PARSER_ConsumeToken(parser);
    }

    // Consume the arrow.
    PARSER_ConsumeToken(parser);

    ast_identifier_t* return_type = AST_CREATE_NODE(scratch);
    return_type->kind = ASTK_FUNCTION_RETURN_TYPE;
    return_type->token = parser->current_token;

    signature->return_type = return_type;

    decl->function.name = name;
    decl->function.signature = signature;
    decl->function.body = NULL; // @TODO: Implement body.

    ERROR_ReportScope(&scoped_error);
    return decl;
}

ast_name_with_type_t* PARSER_ParseNameWithType(parser_t* parser, arena_t* scratch)
{
    ast_identifier_t* name = AST_CREATE_NODE(scratch);
    assert(name);

    name->kind = ASTK_IDENTIFIER;
    name->token = parser->current_token;

    ast_name_with_type_t* name_with_type = AST_CREATE_NODE_SIZED(scratch, sizeof(ast_name_with_type_t));
    assert(name_with_type);

    name_with_type->name = name;
    name_with_type->type = NULL;

    // Consume the name.
    // @TODO: Also expect tokens.
    PARSER_ConsumeToken(parser);

    // In case the type is specified (after colon), try to set it.
    if (parser->current_token.kind == TK_COLON) {
        PARSER_ConsumeToken(parser);

        ast_identifier_t* type = AST_CREATE_NODE(scratch);
        assert(type);

        type->kind = ASTK_IDENTIFIER;
        type->token = parser->current_token;

        PARSER_ConsumeToken(parser);

        name_with_type->type = type;
    }
    return name_with_type;
}

void PARSER_DumpAST(parser_t* parser, ast_program_t* root)
{
    printf("│[Program]\n");
    for (uint i = 0; i < root->statements_len; ++i) {
        ast_node_t* node = root->statements[i];
        printf("└──│[Statement]");
        AST_DumpNode(node, 1, true);
        printf("\n");
    }
}
