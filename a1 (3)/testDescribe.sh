#!/bin/bash
args=$#

if [ ${args} -eq 0 ]; then
    echo "Usage: ./testDescribe <testsetfile>" >&2
    exit 1 
fi 

for stem in $(cat $1); do  
    echo "Description for the test case ${stem}:"
    if [ ! -f "${stem}.desc" ]; then
        echo "${stem}: No test description"
    else
        echo $(cat ${stem}.desc)
    fi
done
    