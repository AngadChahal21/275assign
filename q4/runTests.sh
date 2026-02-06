#!/bin/bash

if [[ "$#" -ne 2 ]]; then
    echo "./runInTests commandToTest testSetFile" >&2
    exit 1
fi

command="$1"
testset="$2"

for stem in $(cat "$testset"); do
    tmpfile=$(mktemp)
    $command $(cat "$stem.args")< "${stem}.in" > "$tmpfile"
    diff "$tmpfile" "${stem}.out" > /dev/null
    if [[ $? -eq 0 ]]; then
        echo "Test ${stem} passed"
    else
        echo "Test ${stem} failed"
        echo "Expected output:"
        cat "${stem}.out"
        echo "Actual output:"
        cat "$tmpfile"
    fi
    rm "$tmpfile"
done