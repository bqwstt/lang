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

#define ast_create_node(arena) arena_alloc(arena, sizeof(ast_node_t))
#define ast_create_node_sized(arena, sz) arena_alloc(arena, sz)

ast_program_t ast_create_program()
{
    ast_program_t program;
    program.statements_len = 0;
    return program;
}

const char * ast_node_id(ast_node_t * node)
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

void ast_node_dump(ast_node_t * node)
{
    switch (node->kind) {
        case ASTK_EXPR: {
            ast_node_t * expr = node;
            printf("%c", expr->token.literal);
            break;
        }
        case ASTK_BINARY: {
            ast_binary_op_t * binop = (ast_binary_op_t *)node;
            const char * op = token_names[binop->token.kind];
            printf("(");
            ast_node_dump(binop->left);
            printf(" %s ", op);
            ast_node_dump(binop->right);
            printf(")");
            break;
        }
        default:
            break;
    }
}
