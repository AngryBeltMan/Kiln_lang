#!/bin/sh
gcc -o hashmap.out src/hashmap/hashmap.c
valgrind ./hashmap.out
