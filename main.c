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

#include <sys/mman.h>
#include <fcntl.h>
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>

#include "base/types.h"
#include "base/libc.h"
#include "base/arena.h"
#include "base/string.h"
#include "base/io.h"

#include "lex.h"
#include "ast.h"
#include "parse.h"

#include "base/arena.c"
#include "base/string.c"
#include "base/io.c"
#include "lex.c"
#include "ast.c"
#include "parse.c"

int main(int argc, char** argv)
{
    if (argc != 2) {
        printf("usage: ./lang <filename>\n");
        return 1;
    }

    String code = io_read_file_from_path(argv[1]);
    Lexer lexer = lexer_create(code);
    Parser parser = parser_create(&lexer);
    parser_parse(&parser);
    parser_destroy(&parser);
    return 0;
}
