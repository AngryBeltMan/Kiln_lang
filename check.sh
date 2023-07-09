#!/bin/sh
gcc -o kiln.out -fshort-enums src/main.c
valgrind -s ./kiln.out
