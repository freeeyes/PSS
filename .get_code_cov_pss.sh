#!/bin/bash
for filename in `find ../Common -type f -name '*.cpp'`; 
do 
	echo $filename
	obj=$(echo $filename | sed -e s/..\\\/Common/.obj/ -e s/cpp/o/)
	echo $obj
	gcov-5 -r -p -o $obj $filename
done

for filename in `find ../Console -type f -name '*.cpp'`; 
do 
	echo $filename
	obj=$(echo $filename | sed -e s/..\\\/Console/.obj/ -e s/cpp/o/)
	echo $obj
	gcov-5 -r -p -o $obj $filename
done

for filename in `find ../Encrypt -type f -name '*.cpp'`; 
do 
	echo $filename
	obj=$(echo $filename | sed -e s/..\\\/Encrypt/.obj/ -e s/cpp/o/)
	echo $obj
	gcov-5 -r -p -o $obj $filename
done

for filename in `find ../FileTest -type f -name '*.cpp'`; 
do 
	echo $filename
	obj=$(echo $filename | sed -e s/..\\\/FileTest/.obj/ -e s/cpp/o/)
	echo $obj
	gcov-5 -r -p -o $obj $filename
done

for filename in `find ../LogSystem -type f -name '*.cpp'`; 
do 
	echo $filename
	obj=$(echo $filename | sed -e s/..\\\/LogSystem/.obj/ -e s/cpp/o/)
	echo $obj
	gcov-5 -r -p -o $obj $filename
done

for filename in `find ../Mail -type f -name '*.cpp'`; 
do 
	echo $filename
	obj=$(echo $filename | sed -e s/..\\\/Mail/.obj/ -e s/cpp/o/)
	echo $obj
	gcov-5 -r -p -o $obj $filename
done

for filename in `find ../Message -type f -name '*.cpp'`; 
do 
	echo $filename
	obj=$(echo $filename | sed -e s/..\\\/Message/.obj/ -e s/cpp/o/)
	echo $obj
	gcov-5 -r -p -o $obj $filename
done

for filename in `find ../PacketParse -type f -name '*.cpp'`; 
do 
	echo $filename
	obj=$(echo $filename | sed -e s/..\\\/PacketParse/.obj/ -e s/cpp/o/)
	echo $obj
	gcov-5 -r -p -o $obj $filename
done

for filename in `find ../Proactor -type f -name '*.cpp'`; 
do 
	echo $filename
	obj=$(echo $filename | sed -e s/..\\\/Proactor/.obj/ -e s/cpp/o/)
	echo $obj
	gcov-5 -r -p -o $obj $filename
done

for filename in `find ../Reactor -type f -name '*.cpp'`; 
do 
	echo $filename
	obj=$(echo $filename | sed -e s/..\\\/Reactor/.obj/ -e s/cpp/o/)
	echo $obj
	gcov-5 -r -p -o $obj $filename
done

for filename in `find ../TinyXML -type f -name '*.cpp'`; 
do 
	echo $filename
	obj=$(echo $filename | sed -e s/..\\\/TinyXML/.obj/ -e s/cpp/o/)
	echo $obj
	gcov-5 -r -p -o $obj $filename
done

for filename in `find ../UDP -type f -name '*.cpp'`; 
do 
	echo $filename
	obj=$(echo $filename | sed -e s/..\\\/UDP/.obj/ -e s/cpp/o/)
	echo $obj
	gcov-5 -r -p -o $obj $filename
done

for filename in `find ./ -type f -name '*.cpp'`; 
do 
	echo $filename
	obj=$(echo $filename | sed -e s/.\\\//.obj/ -e s/cpp/o/)
	echo $obj
	gcov-5 -r -p -o $obj $filename
done