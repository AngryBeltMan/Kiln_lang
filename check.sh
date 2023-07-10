#!/bin/sh
gcc -o kiln.out -fshort-enums src/main.c
valgrind -s --track-origins=yes ./kiln.out
