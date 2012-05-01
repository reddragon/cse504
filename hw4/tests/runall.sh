#!/bin/bash

DIR=`dirname $0`
pushd $DIR

function runtest {
    # echo $1;
    echo ""
    echo -ne "Running test for $1\n"
    echo -ne "---------------------------\n\n"
    ../src/demo $1
}

ls *.decaf | while read name; do runtest $name; done;
popd
