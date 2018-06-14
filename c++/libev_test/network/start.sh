#!/bin/bash

pkill main
rm -f main
gcc -g -lev main.cpp -o main
./main
