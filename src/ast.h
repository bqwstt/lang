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

enum ASTKind
{
    ASTK_UNKNOWN,

    ASTK_BINARY,
    ASTK_STMT,
    ASTK_EXPR,
};
typedef enum ASTKind ASTKind;

// @TODO: we may want token to be a ptr
#define ASTBase \
    Token token; \
    ASTKind kind;

struct ASTNode {
    ASTBase;
};
typedef struct ASTNode ASTNode;
typedef struct ASTNode ASTStatement;

#define MAX_STATEMENTS 1024
struct ASTProgram
{
    // @TODO: use arena
    uint16 statements_len;
    ASTNode* statements[MAX_STATEMENTS];
};
typedef struct ASTProgram ASTProgram;

struct ASTBinaryOp
{
    ASTBase;
    ASTNode* left;
    ASTNode* right;
};
typedef struct ASTBinaryOp ASTBinaryOp;

/* Helpers */
const char* ast_get_node_id(ASTNode* node);
void ast_dump_node(ASTNode* node);

#endif // AST_H
