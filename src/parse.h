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

#ifndef PARSE_H
#define PARSE_H

struct Parser
{
    Lexer* lexer;
    Arena node_arena;

    Token current_token;
    Token next_token;
};
typedef struct Parser Parser;

enum OperatorAssociativityType
{
    ASSOC_UNKNOWN,
    ASSOC_RIGHT,
    ASSOC_LEFT,
};
typedef enum OperatorAssociativityType OperatorAssociativityType;

Parser Parser_CreateParser(Lexer* lexer);
void   Parser_DestroyParser(Parser* parser);
void   Parser_ConsumeToken(Parser* parser);
void   Parser_Parse(Parser* parser);

/* Helpers */
bool Parser_TokenKindIsOperator(TokenKind kind);
uint Parser_OperatorPrecedence(TokenKind op);
OperatorAssociativityType Parser_OperatorAssociativity(TokenKind op);

/* Parsing functions */
ASTStatement* Parser_ParseStatement(Parser* parser);
ASTStatement* Parser_ParseExpression(Parser* parser, uint8 prec_limit, Arena scratch);

void Parser_DumpAST(Parser* parser, ASTProgram* root);

#endif // PARSE_H
