#!/bin/bash

gen=mincut/gen.cpp

mkdir -p obj
rm -rf ans.txt
g++ $gen -o obj/test
obj/test > tmp.txt

for file in mincut/*;do
	if [ "`basename $file`" == "`basename $gen`" ];then
		continue
	fi
	if [ "`basename $file`" != "RRC.cpp" ];then
		continue
	fi
	algorithm=`basename $file | sed 's/.cpp//g'`
	printf "%20s : " $algorithm
	g++ $file -o obj/tar
	echo $algorithm >> ans.txt
	/usr/bin/time -f '%e' bash cmd.sh
	echo >> ans.txt
done

cat ans.txt
