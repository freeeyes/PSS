#!/bin/bash
for filename in `find ../Common -type f -name '*.cpp'`; 
do 
	echo $filename
	obj=$(echo $filename | sed -e s ../Common/build/ -e s .obj/o/)
	echo $obj
	gcov-5 -r -p -o $obj $filename
done