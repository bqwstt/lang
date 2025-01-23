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

Parser parser_create(Lexer* lexer)
{
    Arena node_arena;
    arena_initialize(&node_arena, node_buffer, PARSER_NODE_LIMIT);

    Parser parser;
    parser.lexer = lexer;
    parser.node_arena = node_arena;
    parser_consume_token(&parser);
    parser_consume_token(&parser);
    return parser;
}

void parser_destroy(Parser* parser)
{
    lexer_destroy(parser->lexer);
    arena_free(&parser->node_arena);
}

bool parser_token_kind_is_operator(TokenKind kind)
{
    return kind == TK_PLUS
        || kind == TK_MINUS
        || kind == TK_ASTERISK
        || kind == TK_SLASH
        || kind == TK_EXPONENT;
}

uint parser_operator_precedence(TokenKind op)
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

OperatorAssociativityType parser_operator_associativity(TokenKind op)
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

void parser_consume_token(Parser* parser)
{
    parser->current_token = parser->next_token;
    if (parser->current_token.kind == TK_ILLEGAL || parser->current_token.kind == TK_EOF)
        return;

    parser->next_token = lexer_consume_token(parser->lexer);
}

void parser_parse(Parser* parser)
{
    ASTProgram program = ast_create_program_node();

    while (parser->current_token.kind != TK_EOF) {
        ASTStatement* stmt = parser_parse_statement(parser);

        if (stmt != NULL) {
            program.statements[program.statements_len++] = stmt;
        }

        // @TODO: we may want to reduce all tokens, not just 1?
        parser_consume_token(parser);
    }
    
    parser_dump_ast(parser, &program);
    return;
}

ASTStatement* parser_parse_statement(Parser* parser)
{
    ASTStatement* stmt = AST_CREATE_NODE(&parser->node_arena);
    assert(stmt);

    stmt->kind = ASTK_STMT;

    // @TODO: Check if these micro allocations are a bit too much.
    byte* scratch_node_buffer[16384];
    Arena scratch;
    arena_initialize(&scratch, scratch_node_buffer, 16384);

    if (parser->current_token.kind == TK_NUMBER_LITERAL) {
        stmt = parser_parse_expression(parser, 0, &scratch);
    } else if (parser->next_token.kind == TK_ASSIGNMENT_OPERATOR) {
        stmt = parser_parse_assignment(parser, &scratch);
    } else if (parser->next_token.kind == TK_CONSTANT_DEFINITION_OPERATOR) {
        stmt = parser_parse_constant(parser, &scratch);
    }

    return stmt;
}

ASTStatement* parser_parse_expression(Parser* parser, uint8 prec_limit, Arena* scratch)
{
    // Implementation of a Pratt parser.
    // Wonderful article explaining this algorithm:
    // https://martin.janiczek.cz/2023/07/03/demystifying-pratt-parsers.html
    ASTExpression* expr = AST_CREATE_NODE(&parser->node_arena);
    assert(expr);

    expr->kind  = ASTK_EXPR;
    expr->token = parser->current_token;

    while (parser_token_kind_is_operator(parser->next_token.kind)) {
        uint8 prec = parser_operator_precedence(parser->next_token.kind);
        uint8 final_prec = prec;
        if (parser_operator_associativity(parser->next_token.kind) == ASSOC_RIGHT) {
            final_prec -= 1;
        }

        if (prec <= prec_limit) {
            return expr;
        }

        Token op_token = parser->next_token;

        // Consume both the number and the operator.
        parser_consume_token(parser);
        parser_consume_token(parser);

        ASTExpression* right = parser_parse_expression(parser, final_prec, scratch);
        ASTBinaryOp* binop = AST_CREATE_NODE_SIZED(scratch, sizeof(ASTBinaryOp));
        assert(binop);

        binop->kind = ASTK_BINARY;
        binop->left = expr;
        binop->right = right;
        binop->token = op_token;

        expr = cast(ASTExpression*) binop;
    };

    return expr;
}

ASTStatement* parser_parse_assignment(Parser* parser, Arena* scratch)
{
    // @TODO: specifying types (e.g. a: uint = 42)
    ASTDeclaration* decl = AST_CREATE_NODE_SIZED(&parser->node_arena, sizeof(ASTDeclaration));
    assert(decl);

    decl->kind = ASTK_VARIABLE_ASSIGNMENT;
    decl->token = parser->next_token;
    decl->variable.name_with_type = parser_parse_name_with_type(parser, scratch);
    parser_consume_token(parser); // Consume the assignment operator.
    decl->variable.expression = parser_parse_expression(parser, 0, scratch);

    // Consume the semicolon.
    // @TODO: Throw an error if the semicolon is not found.
    parser_consume_token(parser);

    return cast(ASTStatement*) decl;
}

ASTStatement* parser_parse_constant(Parser* parser, Arena* scratch)
{
    // Let's just assume what follows is a function declaration...
    return cast(ASTStatement*) parser_parse_function(parser, scratch);
}

ASTDeclaration* parser_parse_function(Parser* parser, Arena* scratch)
{
    ScopedError scoped_error;
    ASTIdentifier* name = AST_CREATE_NODE(scratch);
    assert(name);

    name->kind = ASTK_IDENTIFIER;
    name->token = parser->current_token;

    ASTDeclaration* decl = AST_CREATE_NODE_SIZED(&parser->node_arena, sizeof(ASTDeclaration));
    decl->kind = ASTK_FUNCTION_DECLARATION;
    decl->token = parser->next_token;

    // @TODO: Make this arena part of some scope node, maybe?
    // @TODO: Check if this arena is needed, as we already have one created in parse_statement?
    byte* scope_arena_buffer[4096];
    Arena scope_arena;
    arena_initialize(&scope_arena, scope_arena_buffer, 4096);

    // @TODO: Expect parenthesis before consuming the token.
    // Consume name + definition operator + opening parenthesis.
    parser_consume_token(parser);
    parser_consume_token(parser);
    if (parser->current_token.kind != TK_PARENTHESIS_OPEN) {
        // @FIXME: Provide some kind of "synchronization" to skip to the next valid token.
        error_push_scope(&scoped_error, &scope_arena, ERROR_UNEXPECTED_TOKEN, &parser->current_token);
    }

    parser_consume_token(parser);

    ASTTypeSignature* signature = AST_CREATE_NODE_SIZED(&scope_arena, sizeof(ASTTypeSignature));
    if (parser->current_token.kind != TK_PARENTHESIS_CLOSE) {
        // Parse parameters.
        uint8 i = 0;
        while (true) {
            signature->parameters[i] = parser_parse_name_with_type(parser, &scope_arena);
            // @FIXME: we can set kind in name_with_type directly maybe?
            signature->parameters[i++]->name->kind = ASTK_FUNCTION_PARAMETER;

            if (i == MAX_PARAMETERS) {
                // @TODO: Throw an error for missing parenthesis...?
                break;
            }

            if (parser->current_token.kind == TK_PARENTHESIS_CLOSE) {
                parser_consume_token(parser);
                break;
            }

            if (parser->current_token.kind != TK_COMMA) {
                error_push_scope(&scoped_error, &scope_arena, ERROR_UNEXPECTED_TOKEN, &parser->current_token);
            }
            parser_consume_token(parser);
        }
    } else {
        parser_consume_token(parser);
    }

    // Consume the arrow.
    parser_consume_token(parser);

    ASTIdentifier* return_type = AST_CREATE_NODE(scratch);
    return_type->kind = ASTK_FUNCTION_RETURN_TYPE;
    return_type->token = parser->current_token;

    signature->return_type = return_type;

    decl->function.name = name;
    decl->function.signature = signature;
    decl->function.body = NULL; // @TODO: Implement body.

    error_report_scope(&scoped_error);
    return decl;
}

ASTNameWithType* parser_parse_name_with_type(Parser* parser, Arena* scratch)
{
    ASTIdentifier* name = AST_CREATE_NODE(scratch);
    assert(name);

    name->kind = ASTK_IDENTIFIER;
    name->token = parser->current_token;

    ASTNameWithType* name_with_type = AST_CREATE_NODE_SIZED(scratch, sizeof(ASTNameWithType));
    assert(name_with_type);

    name_with_type->name = name;
    name_with_type->type = NULL;

    // Consume the name.
    // @TODO: Also expect tokens.
    parser_consume_token(parser);

    // In case the type is specified (after colon), try to set it.
    if (parser->current_token.kind == TK_COLON) {
        parser_consume_token(parser);

        ASTIdentifier* type = AST_CREATE_NODE(scratch);
        assert(type);

        type->kind = ASTK_IDENTIFIER;
        type->token = parser->current_token;

        parser_consume_token(parser);

        name_with_type->type = type;
    }
    return name_with_type;
}

void parser_dump_ast(Parser* parser, ASTProgram* root)
{
    printf("│[Program]\n");
    for (uint i = 0; i < root->statements_len; ++i) {
        ASTNode* node = root->statements[i];
        printf("└──│[Statement]");
        ast_dump_node(node, 1, true);
        printf("\n");
    }
}
