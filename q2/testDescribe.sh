#!/bin/bash
args=$#

if [ ${args} -eq 0 ]; then
    echo "Usage:No arguments were given"
    exit 1 
fi 

for stem in $(cat $1); do  
    echo "Description for the test case ${stem}:"
    if [ ! -e "${stem}.desc" ]; then
        echo "${stem}: No test description"
    fi
    echo $(cat ${stem}.desc)
done
    