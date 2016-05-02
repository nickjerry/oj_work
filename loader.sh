#/bin/bash

files=`echo "$*" | sed 's/\//\\\\\//g'`
echo "load file(s):$files"
sed -i "s/CFILES :=.*$/CFILES := $files/g" Makefile
