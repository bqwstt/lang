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

#define AST_CREATE_NODE(arena)           arena_alloc(arena, sizeof(ASTNode))
#define AST_CREATE_NODE_SIZED(arena, sz) arena_alloc(arena, sz)

ASTProgram AST_CreateProgramNode()
{
    ASTProgram program;
    program.statements_len = 0;
    return program;
}

// @FIXME: Maybe follow the same approach as we do for tokens,
// where this is a plain array. Check what's faster.
const char* ast_get_node_id(ASTNode* node)
{
    switch (node->kind) {
        case ASTK_EXPR:
        case ASTK_BINARY:
            return "Expression";
        case ASTK_STMT:
            return "Statement";
        case ASTK_IDENTIFIER:
            return "Identifier";
        case ASTK_ASSIGNMENT:
            return "Assignment";
        default:
            return "Unknown";
    }
}

void ast_dump_node(ASTNode* node, uint8 depth, bool has_child)
{
    if (depth > 0) {
        printf("\n");
        uint spaces = depth * 4 - depth;
        for (uint i = 0; i < spaces; ++i) {
            printf(" ");
        }

        if (has_child) {
            printf("└──│[%s] ", ast_get_node_id(node));
        } else {
            printf("└───[%s] ", ast_get_node_id(node));
        }
    }

    switch (node->kind) {
        case ASTK_EXPR:
        case ASTK_IDENTIFIER: {
            String literal = node->token.literal;
            printf("%s", literal.data);
            break;
        }
        case ASTK_BINARY: {
            ASTBinaryOp* binop = cast(ASTBinaryOp*) node;
            const char* op = cast(const char*) binop->token.literal.data;
            ast_dump_node(binop->left, 0, false);
            printf(" %s ", op);
            ast_dump_node(binop->right, 0, false);
            break;
        }
        case ASTK_ASSIGNMENT: {
            ASTAssignment* assignment = cast(ASTAssignment*) node;
            ast_dump_node(assignment->identifier, depth+1, false);
            ast_dump_node(assignment->expression, depth+1, false);
            break;
        }
        default:
            break;
    }
}
