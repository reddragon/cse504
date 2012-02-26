#!/bin/bash
function execute {   
	while read line; do 
    ./decafch <  "$line" > /dev/null
  	RET=$?; 
  	if [ $RET -ne 0 ]; then 
      echo "Error in file: $line"
      exit $RET
    fi
  done;
}

echo "Running tests"
ls tests/*.decaf | execute
echo "Successful"


