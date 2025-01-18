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
    ASTK_IDENTIFIER,
    ASTK_ASSIGNMENT,
};
typedef enum ASTKind ASTKind;

// @TODO: Check how can we make `token` a pointer?
struct ASTNode {
    ASTKind kind;
    Token token;
};
typedef struct ASTNode ASTNode;
typedef struct ASTNode ASTStatement;
typedef struct ASTNode ASTIdentifier;
typedef struct ASTNode ASTExpression;

#define MAX_STATEMENTS 1024
struct ASTProgram
{
    // @TODO: use arena
    uint16 statements_len;
    ASTStatement* statements[MAX_STATEMENTS];
};
typedef struct ASTProgram ASTProgram;

struct ASTBinaryOp
{
    ASTKind kind;
    Token token;

    ASTExpression* left;
    ASTExpression* right;
};
typedef struct ASTBinaryOp ASTBinaryOp;

struct ASTAssignment
{
    ASTKind kind;
    Token token;

    ASTIdentifier* identifier;
    ASTExpression* expression;
};
typedef struct ASTAssignment ASTAssignment;

/* Helpers */
const char* ast_get_node_id(ASTNode* node);
void ast_dump_node(ASTNode* node, uint8 depth, bool has_child);

#endif // AST_H
