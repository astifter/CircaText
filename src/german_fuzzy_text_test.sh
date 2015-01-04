#!/bin/bash
gcc -o german_fuzzy_text_test german_fuzzy_text_test.c german_fuzzy_text.c
./german_fuzzy_text_test > german_fuzzy_text_test.out
if ! diff -qs german_fuzzy_text_test.ref german_fuzzy_text_test.out; then
    diff -u german_fuzzy_text_test.ref german_fuzzy_text_test.out;
fi
