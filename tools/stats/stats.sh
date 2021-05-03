#!/bin/bash
for filename in ../../../student-code/myalf2012/*.c; do
    name=$(basename "$filename" .c)
    expand -t4 $filename > files/$name.c
    # ../../build/stylemarker $filename --config config.xml --outfile output/$name.json
done

for filename in ../../../student-code/wifistats2017/*.c; do
    name=$(basename "$filename" .c)
    expand -t4 $filename > files/$name.c
    # ../../build/stylemarker $filename --config config.xml --outfile output/$name.json
done

for filename in files/*.c; do
    name=$(basename "$filename" .c)
    echo $name:
    ../../build/stylemarker $filename --config config.xml --outfile output/$name.json
done