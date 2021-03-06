#!/bin/bash
# enable debug output
set -x

compareandshow() {
    if ! diff -qs "$1" "$2"; then
        diff -u "$1" "$2" | head -n 20
        exit -1
    fi
}

compile() {
    target=$1; shift;
    gcc -DLOG_TESTING -g -o $target $*  -I ../src
}

# compile testing software
gcc -DLOG_TESTING -g -o german_fuzzy_text_test german_fuzzy_text_test.c ../src/german_fuzzy_text_v*.c ../src/watchface_base/stringbuffer.c ../src/german_fuzzy_text_common.c -I ../src
gcc -DLOG_TESTING -g -o stringbuffer_test stringbuffer_test.c ../src/watchface_base/stringbuffer.c -I ../src

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

compile sort_test sort_test.c ../src/watchface_base/quicksort.c
./sort_test || (echo "sorting broken"; exit -1)
rm sort_test
