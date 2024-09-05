#!/bin/sh

# @TODO: separate debug & build
# @TODO: add support for clang
# - Try adding -fsanitize-trap as well.

COMPILER_FLAGS="-O0 -ggdb3 -Wall -Wno-unused-variable -Wno-discarded-qualifiers"
SANITIZER_FLAGS="-fsanitize=undefined"
INCLUDE_FLAGS="-Isrc"

set -x
time gcc $INCLUDE_FLAGS $COMPILER_FLAGS $SANITIZER_FLAGS main.c -o lang || exit 1
set +x

echo
echo "All done."
echo "========="
