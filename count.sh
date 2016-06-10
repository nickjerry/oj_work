#!/bin/bash

gen=mincut/gen.cpp
dat=mincut.dat
ans=ans.txt
cal=obj/cal
tmp=obj/tmp

v_bound=800
e_bound=3000
vertices=(200 200 200)
edges=(80 200 400)

mkdir -p obj
rm -rf $ans

printf "# "
for file in mincut/*;do
	if [ "`basename $file`" == "`basename $gen`" ];then
		continue
	fi
	algorithm=`basename $file | sed 's/.cpp//g'`
	printf "%s  " $algorithm
done

echo

for i in $(seq 0 $((${#vertices[@]}-1)));do
	sed -i "s/n = [0-9]\+/n = ${vertices[$i]}/g" $gen
	sed -i "s/m = [0-9]\+/m = ${edges[$i]}/g" $gen
	g++ $gen -o $cal
	./$cal > tmp.txt

	printf "%d  %d  " ${vertices[$i]} ${edges[$i]}

	echo > $tmp
	for file in mincut/*;do
		if [ "`basename $file`" == "`basename $gen`" ];then
			continue
		fi
		if [ "`basename $file`" == "RRC.cpp" ] && ([ ${vertices[$i]} -gt $v_bound ] || [ ${edges[$i]} -gt $e_bound  ]);then
			continue
		fi
		g++ $file -o obj/tar
		/usr/bin/time -f '%e' bash cmd.sh >> $tmp 2>&1
	done
	echo `cat $tmp` | tr "\n" "  "
	echo
done
