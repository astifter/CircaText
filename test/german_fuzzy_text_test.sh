#!/bin/bash
set -x
gcc -g -o german_fuzzy_text_test german_fuzzy_text_test.c ../src/german_fuzzy_text_v*.c -I ../src
for i in 1 2; do
    ./german_fuzzy_text_test -$i > german_fuzzy_text_test.out
    if ! diff -qs german_fuzzy_text_test_v${i}.ref german_fuzzy_text_test.out; then
        diff -u german_fuzzy_text_test_v${i}.ref german_fuzzy_text_test.out;
    fi
    rm german_fuzzy_text_test.out
done
