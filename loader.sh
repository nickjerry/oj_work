#/bin/bash

echo "load file(s):$*"
sed -i "s/CFILES := [^$]\+$/CFILES := $*/g" Makefile
