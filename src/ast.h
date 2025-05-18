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

#ifndef AST_H
#define AST_H

enum ast_kind
{
    ASTK_UNKNOWN,

    ASTK_BINARY,
    ASTK_STMT,
    ASTK_EXPR,
    ASTK_IDENTIFIER,
    ASTK_VARIABLE_ASSIGNMENT,
    ASTK_FUNCTION_DECLARATION,
    ASTK_FUNCTION_PARAMETER,
    ASTK_FUNCTION_RETURN_TYPE,
};
typedef enum ast_kind ast_kind_t;

// @TODO: Check how can we make `token` a pointer?
struct ast_node {
    ast_kind_t kind;
    token_t token;
};
typedef struct ast_node ast_node_t;
typedef struct ast_node ast_statement_t;
typedef struct ast_node ast_identifier_t;
typedef struct ast_node ast_expression_t;

#define MAX_STATEMENTS 1024
struct ast_program
{
    // @TODO: use arena
    uint16 statements_len;
    ast_statement_t* statements[MAX_STATEMENTS];
};
typedef struct ast_program ast_program_t;

struct ast_binary_op
{
    ast_kind_t kind;
    token_t token;

    ast_expression_t* left;
    ast_expression_t* right;
};
typedef struct ast_binary_op ast_binary_op_t;

struct ast_name_with_type
{
    ast_kind_t kind;
    ast_identifier_t* name;
    ast_identifier_t* type; // @TODO: Change this to a proper type?
};
typedef struct ast_name_with_type ast_name_with_type_t;

struct ast_variable_declaration
{
    ast_name_with_type_t* name_with_type;
    ast_expression_t* expression;
    // @TODO: Modifiers (constants etc.)
};
typedef struct ast_variable_declaration ast_variable_declaration_t;

#define MAX_PARAMETERS 127
struct ast_type_signature
{
    ast_name_with_type_t* parameters[MAX_PARAMETERS];
    ast_identifier_t* return_type;
};
typedef struct ast_type_signature ast_type_signature_t;

struct ast_function_declaration
{
    ast_identifier_t* name;
    ast_type_signature_t* signature;
    ast_node_t* body;
};
typedef struct ast_function_declaration ast_function_declaration_t;

struct ast_declaration
{
    ast_kind_t kind;
    token_t token;

    union {
        ast_function_declaration_t function;
        ast_variable_declaration_t variable;
    };
};
typedef struct ast_declaration ast_declaration_t;

/* Helpers */
const char* AST_GetNodeID(ast_node_t* node);
void AST_DumpNode(ast_node_t* node, uint8 depth, bool has_child);

#endif // AST_H
