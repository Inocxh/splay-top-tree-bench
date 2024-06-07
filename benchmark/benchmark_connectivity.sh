#!/bin/bash

WARMUPS=1
ITERATIONS=3

mkdir -p results/connectivity/
rm -f results/connectivity/splay-top-tree.jsonl

for n in 500 1000 2000 3000 4000 5000 6000 7000 8000
do
    echo "Benchmark splay-top-tree connectivity with $n vertices"...           
    ./bin/splay-top-trees/benchmark_connectivity $WARMUPS $ITERATIONS dataset/connectivity/con_${n}_0.txt >> results/connectivity/splay-top-tree.jsonl || exit   
done


mkdir -p results/connectivity/
rm -f results/connectivity/stt-c.jsonl

for n in 500 1000 2000 3000 4000 5000 6000 7000 8000
do
    echo "Benchmark stt-c connectivity with $n vertices"...
    ./bin/stt-c/greedy_stt bench 1 $WARMUPS $ITERATIONS dataset/connectivity/con_${n}_0.txt >> results/connectivity/stt-rs.jsonl || exit   
done

for n in 500 1000 2000 3000 4000 5000 6000 7000 8000
do
    echo "Benchmark stt-rs trees with $n vertices"...
    for i in $(eval echo {0..$(($SETS - 1))})
    do
        echo "Set $i" 
        for _ in $(eval echo {1..$REPEAT})
        do
            bin/stt-rs/bench_queries_file --input-file dataset/con/con_${n}_0.txt --iterations $ITERATIONS >> results/con/$STT_RS_FILE || exit
        done
    done
done
