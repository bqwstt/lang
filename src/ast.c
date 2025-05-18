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

#define AST_CREATE_NODE(arena)           ARENA_Alloc(arena, sizeof(ast_node_t))
#define AST_CREATE_NODE_SIZED(arena, sz) ARENA_Alloc(arena, sz)

ast_program_t AST_CreateProgramNode()
{
    ast_program_t program;
    program.statements_len = 0;
    return program;
}

// @FIXME: Maybe follow the same approach as we do for tokens,
// where this is a plain array. Check what's faster.
const char* AST_GetNodeID(ast_node_t* node)
{
    switch (node->kind) {
        case ASTK_EXPR:
        case ASTK_BINARY:
            return "Expression";
        case ASTK_STMT:
            return "Statement";
        case ASTK_IDENTIFIER:
            return "Identifier";
        case ASTK_VARIABLE_ASSIGNMENT:
            return "Assignment";
        case ASTK_FUNCTION_DECLARATION:
            return "Function";
        case ASTK_FUNCTION_PARAMETER:
            return "Function parameter";
        case ASTK_FUNCTION_RETURN_TYPE:
            return "Function return type";
        default:
            return "Unknown";
    }
}

void AST_DumpNode(ast_node_t* node, uint8 depth, bool has_child)
{
    if (depth > 0) {
        printf("\n");
        uint spaces = depth * 4 - depth;
        for (uint i = 0; i < spaces; ++i) {
            printf(" ");
        }

        if (has_child) {
            printf("└──│[%s] ", AST_GetNodeID(node));
        } else {
            printf("└───[%s] ", AST_GetNodeID(node));
        }
    }

    switch (node->kind) {
        case ASTK_EXPR:
        case ASTK_IDENTIFIER:
        case ASTK_FUNCTION_PARAMETER:
        case ASTK_FUNCTION_RETURN_TYPE: {
            string_t literal = node->token.literal;
            printf("%s", literal.data);
            break;
        }
        case ASTK_BINARY: {
            ast_binary_op_t* binop = cast(ast_binary_op_t*) node;
            const char* op = cast(const char*) binop->token.literal.data;
            AST_DumpNode(binop->left, 0, false);
            printf(" %s ", op);
            AST_DumpNode(binop->right, 0, false);
            break;
        }
        case ASTK_VARIABLE_ASSIGNMENT: {
            ast_declaration_t* decl = cast(ast_declaration_t*) node;
            ast_variable_declaration_t variable_decl = cast(ast_variable_declaration_t) decl->variable;
            AST_DumpNode(variable_decl.name_with_type->name, depth+1, false);
            // @TODO:
            // AST_DumpNode(assignment->name_with_type->type, depth+1, false);
            AST_DumpNode(variable_decl.expression, depth+1, false);
            break;
        }
        case ASTK_FUNCTION_DECLARATION: {
            ast_declaration_t* decl = cast(ast_declaration_t*) node;
            ast_function_declaration_t function_decl = cast(ast_function_declaration_t) decl->function;
            AST_DumpNode(function_decl.name, depth+1, true);

            ast_name_with_type_t* next_parameter = function_decl.signature->parameters[0];
            uint i = 0;
            while (next_parameter != null) {
                // @TODO: dump for types
                AST_DumpNode(next_parameter->name, depth+2, false);
                i += 1;
                next_parameter = function_decl.signature->parameters[i];
            }

            AST_DumpNode(function_decl.signature->return_type, depth+1, false);
            break;
        }
        default:
            break;
    }
}
