#!/bin/bash

SIZES="1000 2000 5000 10000 20000 50000 100000 200000 500000 100000 200000 500000"
WARMUPS=1
ITERATIONS=3

mkdir -p results/2-edge-connectivity/
rm -f results/2-edge-connectivity/splay-top-tree.jsonl

for n in $SIZES
do
    echo "Benchmark splay-top-tree with full splay on 2-edge-connectivity with $n vertices"...
    ./bin/splay-top-trees/benchmark_2_edge $WARMUPS $ITERATIONS dataset/2-edge-connectivity/2edge_${n}_0.txt >> results/2-edge-connectivity/splay-top-tree.jsonl || exit
done

for n in $SIZES
do
    echo "Benchmark splay-top-tree with semi splay on 2-edge-connectivity with $n vertices"...
    ./bin/splay-top-trees/benchmark_2_edge_semi_splay $WARMUPS $ITERATIONS dataset/2-edge-connectivity/2edge_${n}_0.txt >> results/2-edge-connectivity/splay-top-tree.jsonl || exit
done