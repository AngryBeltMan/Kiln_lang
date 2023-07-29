#!/bin/sh
clang -o kiln.out -fshort-enums src/main.c
NO_DEBUG=1
while getopts ":v:s:g" opt; do
    case $opt in
        v)
            valgrind ./kiln.out
            NO_DEBUG=$(0)
            ;;
        s)
            # makes the previous build worthless, wasting time.
            clang -o kiln.out -fshort-enums src/main.c
            ;;
        g)
            # makes the previous build worthless, wasting time.
            # uses gcc to compile the code
            gcc -o kiln.out -fshort-enums src/main.c
            ;;
    esac
done
if [ $NO_DEBUG = 1 ] ; then
    ./kiln.out
fi
