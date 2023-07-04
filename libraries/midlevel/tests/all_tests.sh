#!/bin/sh

runtest_range() {
    test_passed=0

    for i in $(seq 0 $2)
    do
        printf "\rRunning test ${1}: $i/$(bc <<< "${2}+1")"

        ./test${1} $i >logs/test${1}_$i.log 2>logs/test${1}_err_$i.log

        if [ $? -eq 0 ]; then
            test_passed=$(bc <<< "$test_passed+1")
        fi

        if [ ! -s logs/test${1}_err_$i.log ]; then
            rm logs/test${1}_err_$i.log
        fi
    done

    printf "\nTest ${1}: $test_passed/$(bc <<< "${2}+1") passed\n"
}

runtest_range 1 1023
runtest_range 2 1020
runtest_range 3 1023
