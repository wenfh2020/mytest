#!/bin/sh

if [ $# -lt 1 ]; then
    echo 'input pid'
    exit 1
fi
perf record -F 99 -p $1 -g -- sleep 10
perf script -i perf.data &> perf.unfold
stackcollapse-perf.pl perf.unfold &> perf.folded
flamegraph.pl perf.folded > perf.svg
