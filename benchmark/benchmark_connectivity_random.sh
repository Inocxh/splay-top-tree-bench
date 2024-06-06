#!/bin/bash

DATA_FILE=connec.jsonl
REPEAT=1
SETS=1
WARMUPS=1
ITERATIONS=3


if [ -f $INPUT_FILE ]
then
    mkdir -p results/splay_top_tree/
    rm -f results/splay_top_tree/$DATA_FILE
    
    for n in 500 1000 2000 3000 4000 5000 6000 7000 8000
    do
        echo "Benchmark Connectivity with $n vertices"...
        for i in $(eval echo {0..$(($SETS - 1))})
        do
            echo "Set $i" 
            for _ in $(eval echo {1..$REPEAT})
            do
                ./../build/benchmark_connectivity $WARMUPS $ITERATIONS dataset/connectivity/con_${n}_${i}.txt >> results/splay_top_tree/$DATA_FILE || exit
            done
        done
    done
fi
