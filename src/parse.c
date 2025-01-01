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

Parser Parser_Create(Lexer* lexer)
{
    Arena node_arena;
    Arena_Initialize(&node_arena, node_buffer, PARSER_NODE_LIMIT);

    Parser parser;
    parser.lexer = lexer;
    parser.node_arena = node_arena;
    Parser_ConsumeToken(&parser);
    Parser_ConsumeToken(&parser);
    return parser;
}

void Parser_Destroy(Parser* parser)
{
    Lexer_Destroy(parser->lexer);
    Arena_Free(&parser->node_arena);
}

bool Parser_TokenKindIsOperator(TokenKind kind)
{
    return kind == TK_PLUS
        || kind == TK_MINUS
        || kind == TK_ASTERISK
        || kind == TK_SLASH
        || kind == TK_EXPONENT;
}

uint Parser_OperatorPrecedence(TokenKind op)
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

OperatorAssociativityType Parser_OperatorAssociativity(TokenKind op)
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

void Parser_ConsumeToken(Parser* parser)
{
    parser->current_token = parser->next_token;
    if (parser->current_token.kind == TK_ILLEGAL || parser->current_token.kind == TK_EOF)
        return;

    parser->next_token = Lexer_ConsumeToken(parser->lexer);
}

void Parser_Parse(Parser* parser)
{
    ASTProgram program = AST_CreateProgramNode();

    while (parser->current_token.kind != TK_EOF) {
        ASTStatement* stmt = Parser_ParseStatement(parser);

        if (stmt != NULL) {
            program.statements[program.statements_len++] = stmt;
        }

        // @TODO: we may want to reduce all tokens, not just 1?
        Parser_ConsumeToken(parser);
    }
    
    Parser_DumpAST(parser, &program);
    return;
}

ASTStatement* Parser_ParseStatement(Parser* parser)
{
    ASTStatement* stmt = AST_CREATE_NODE(&parser->node_arena);
    assert(stmt);

    stmt->kind = ASTK_STMT;

    if (parser->current_token.kind == TK_NUMBER_LITERAL) {
        byte* scratch_node_buffer[16384];
        Arena scratch;
        Arena_Initialize(&scratch, scratch_node_buffer, 16384);
        stmt = Parser_ParseExpression(parser, 0, &scratch);
    }

    return stmt;
}

ASTStatement* Parser_ParseExpression(Parser* parser, uint8 prec_limit, Arena* scratch)
{
    // Implementation of a Pratt parser.
    // Wonderful article explaining this algorithm:
    // https://martin.janiczek.cz/2023/07/03/demystifying-pratt-parsers.html
    ASTStatement* expr = AST_CREATE_NODE(scratch);
    assert(expr);

    expr->kind  = ASTK_EXPR;
    expr->token = parser->current_token;

    while (Parser_TokenKindIsOperator(parser->next_token.kind)) {
        uint8 prec = Parser_OperatorPrecedence(parser->next_token.kind);
        uint8 final_prec = prec;
        if (Parser_OperatorAssociativity(parser->next_token.kind) == ASSOC_RIGHT) {
            final_prec -= 1;
        }

        if (prec <= prec_limit) {
            return expr;
        }

        Token op_token = parser->next_token;

        // Consume both the number and the operator.
        Parser_ConsumeToken(parser);
        Parser_ConsumeToken(parser);

        ASTStatement* right = Parser_ParseExpression(parser, final_prec, scratch);
        ASTBinaryOp*  binop = AST_CREATE_NODE_SIZED(scratch, sizeof(ASTBinaryOp));
        assert(binop);

        binop->kind  = ASTK_BINARY;
        binop->left  = expr;
        binop->right = right;
        binop->token = op_token;

        expr = (ASTStatement*)binop;
    };

    return expr;
}

void Parser_DumpAST(Parser* parser, ASTProgram* root)
{
    printf("│[Program]\n");
    for (uint i = 0; i < root->statements_len; ++i) {
        ASTNode* node = root->statements[i];
        printf("└──│[Statement]\n");
        printf("   └───[%s] ", AST_GetNodeID(node));
        AST_DumpNode(node);
        printf("\n");
    }
}
