#!/bin/bash
make clean; make
if [ -f "main" ]; then
./main b construct
fi
