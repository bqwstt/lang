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

struct parser
{
    lexer_t* lexer;
    arena_t node_arena;

    token_t current_token;
    token_t next_token;
};
typedef struct parser parser_t;

enum operator_associativity_type
{
    ASSOC_UNKNOWN,
    ASSOC_RIGHT,
    ASSOC_LEFT,
};
typedef enum operator_associativity_type operator_associativity_type_t;

parser_t PARSER_Create(lexer_t* lexer);
void PARSER_Destroy(parser_t* parser);
void PARSER_ConsumeToken(parser_t* parser);
void PARSER_Parse(parser_t* parser);

/* Helpers */
bool PARSER_TokenKindIsOperator(token_kind_t kind);
uint PARSER_OperatorPrecedence(token_kind_t op);
operator_associativity_type_t PARSER_OperatorAssociativity(token_kind_t op);

/* Parsing functions */
ast_statement_t* PARSER_ParseStatement(parser_t* parser);
ast_expression_t* PARSER_ParseExpression(parser_t* parser, uint8 prec_limit, arena_t* scratch);
ast_statement_t* PARSER_ParseIdentifier(parser_t* parser);
ast_statement_t* PARSER_ParseAssignment(parser_t* parser, arena_t* scratch);
ast_declaration_t* PARSER_ParseFunction(parser_t* parser, arena_t* scratch);
ast_name_with_type_t* PARSER_ParseNameWithType(parser_t* parser, arena_t* scratch);

void PARSER_DumpAST(parser_t* parser, ast_program_t* root);

#endif // PARSE_H
