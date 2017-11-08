#!/bin/bash
for filename in `find ../Common -type f -name '*.cpp'`; 
do 
	echo $filename
	obj=$(echo $filename | sed -e s/..\/Common/.obj/ -e s/cpp/o/)
	echo $obj
	gcov-5 -r -p -o $obj $filename
done