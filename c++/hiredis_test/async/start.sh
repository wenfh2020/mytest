#!/bin/bash

pkill main
make clean; make
./main
