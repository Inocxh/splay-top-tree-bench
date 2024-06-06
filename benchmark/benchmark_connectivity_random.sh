#!/bin/bash

SPLAY_FILE=splay-top-tree.jsonl
STT_RS_FILE=stt-rs.jsonl
REPEAT=1
SETS=1
WARMUPS=1
ITERATIONS=3


if [ -f $INPUT_FILE ]
then
    mkdir -p results/con/
    rm -f results/con/$SPLAY_FILE
    
    for n in 500 1000 2000 3000 4000 5000 6000 7000 8000
    do
        echo "Benchmark Connectivity with $n vertices"...
        for i in $(eval echo {0..$(($SETS - 1))})
        do
            echo "Set $i" 
            for _ in $(eval echo {1..$REPEAT})
            do
                bin/splay-top-trees/benchmark_connectivity $WARMUPS $ITERATIONS dataset/con/con_${n}_${i}.txt >> results/splay_top_tree/$SPLAY_FILE || exit
            done
        done
    done
fi

if [ -f $INPUT_FILE ]
then
    rm -f results/con/$STT_RS_FILE
    
    for n in 500 1000 2000 3000 4000 5000 6000 7000 8000
    do
        echo "Benchmark Connectivity with $n vertices"...
        for i in $(eval echo {0..$(($SETS - 1))})
        do
            echo "Set $i" 
            for _ in $(eval echo {1..$REPEAT})
            do
                bin/stt-rs/bench_queries_file --input-file dataset/con/con_${n}_${i}.txt --iterations $ITERATIONS >> results/con/$STT_RS_FILE || exit
            done
        done
    done
fi
