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

#define AST_CREATE_NODE(arena)           Arena_Alloc(arena, sizeof(ASTNode))
#define AST_CREATE_NODE_SIZED(arena, sz) Arena_Alloc(arena, sz)

ASTProgram AST_CreateProgramNode()
{
    ASTProgram program;
    program.statements_len = 0;
    return program;
}

const char* AST_GetNodeID(ASTNode* node)
{
    switch (node->kind) {
        case ASTK_EXPR:
        case ASTK_BINARY:
            return "Expr";
        case ASTK_STMT:
            return "Statement";
        default:
            return "Unknown";
    }
}

void AST_DumpNode(ASTNode* node)
{
    switch (node->kind) {
        case ASTK_EXPR: {
            ASTNode* expr = node;
            String literal = expr->token.literal;
            // @TODO: check if we have to do this instead?
            /* printf("%.*s", (int32)literal.len, literal.data); */
            printf("%s", literal.data);
            break;
        }
        case ASTK_BINARY: {
            ASTBinaryOp* binop = (ASTBinaryOp*)node;
            const char* op = token_names[binop->token.kind];
            printf("(");
            AST_DumpNode(binop->left);
            printf(" %s ", op);
            AST_DumpNode(binop->right);
            printf(")");
            break;
        }
        default:
            break;
    }
}
