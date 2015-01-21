#!/bin/bash
# enable debug output
set -x

compareandshow() {
    if ! diff -qs "$1" "$2"; then
        diff -u "$1" "$2" | head -n 20
    fi
}

# compile testing software
gcc -g -o german_fuzzy_text_test german_fuzzy_text_test.c ../src/german_fuzzy_text_v*.c ../src/stringbuffer.c -I ../src
gcc -g -o stringbuffer_test stringbuffer_test.c ../src/stringbuffer.c -I ../src

# for both time to text methods do a run of the program, capture output and
# compare to reference output. if output differs then show differences
for i in 1 2; do
    ./german_fuzzy_text_test -$i > german_fuzzy_text_test.out
    compareandshow german_fuzzy_text_test_v${i}.ref german_fuzzy_text_test.out
    rm german_fuzzy_text_test.out
done

# also do this for stringbuffer tests
./stringbuffer_test > stringbuffer_test.out
compareandshow stringbuffer_test.ref stringbuffer_test.out
rm stringbuffer_test.out
