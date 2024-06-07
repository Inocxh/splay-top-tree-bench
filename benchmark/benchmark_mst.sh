#!/bin/bash

WARMUPS=1
ITERATIONS=3

mkdir -p results/mst/
rm -f results/mst/splay-top-tree.jsonl

for n in 1000 2000 5000 10000 20000 50000 100000 200000 #500000 1000000
do
    echo "Benchmark splay-top-tree MST with $n vertices"...
    ./bin/splay-top-trees/benchmark_mst $WARMUPS $ITERATIONS dataset/mst/random/mst_${n}_${i}.txt >> results/mst/splay-top-tree.jsonl || exit
done

mkdir -p results/mst/
rm -f results/mst/stt-rs.jsonl

for n in 1000 2000 5000 10000 20000 50000 100000 200000 #500000 1000000
do
    echo "Benchmark stt-rs MST with $n vertices"...
    ./bin/stt-rs/bench_mst -i dataset/mst/random/mst_${n}_${i}.txt --json --iterations $ITERATIONS >> results/mst/stt-rs.jsonl || exit
done

