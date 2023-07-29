#!/bin/sh
gcc -o hashmap.out src/hashmap/hashmap_tests.c
NORMALMODE=1
while getopts ":v" opt; do
    case $opt in
        v)
            valgrind ./hashmap.out
            NORMALMODE=$(0)
            ;;
    esac
done
if [ $NORMALMODE = 1 ] ; then
    ./hashmap.out
fi
