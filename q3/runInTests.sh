#!/bin/bash
for stem in $(cat $2); do
    tempFile=$(mktemp)
    
    $1 < ${stem}.in > ${tempFile} #storing the wc for the stem.in file in tempFile
    diff ${tempFile} ${stem}.out

    result=$?
    if [ $result -eq 0 ]; then
        echo "test ${stem} passed"
    fi
    if [ $result -eq 1 ]; then
        echo "Test ${stem} failed"
        echo "Expected output:"
        cat ${stem}.out
        echo "Actual output:"
        cat $tempFile
    fi
    
    rm $tempFile

done

