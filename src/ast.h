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

typedef enum
{
    ASTK_UNKNOWN,

    ASTK_BINARY,
    ASTK_STMT,
    ASTK_EXPR,
} ast_kind_t;

// @TODO: we may want token to be a ptr
#define ast_base \
    token_t token; \
    ast_kind_t kind;

typedef struct {
    ast_base;
} ast_node_t;
typedef ast_node_t ast_statement_t;

#define MAX_STATEMENTS 1024
typedef struct
{
    // @TODO: use arena
    uint16 statements_len;
    ast_node_t * statements[MAX_STATEMENTS];
} ast_program_t;

typedef struct
{
    ast_base;
    ast_node_t * left;
    ast_node_t * right;
} ast_binary_op_t;

/* Helpers */
const char * ast_node_id(ast_node_t * node);
void ast_node_dump(ast_node_t * node);

#endif // AST_H
