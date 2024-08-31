#!/bin/sh

COMPILER_FLAGS="-O0 -ggdb3 -Wall -Wno-unused-variable -Wno-discarded-qualifiers"
INCLUDE_FLAGS="-Isrc"

set -x
time gcc $INCLUDE_FLAGS $COMPILER_FLAGS main.c -o lang || exit 1
set +x

echo
echo "All done."
echo "========="
