#!/bin/bash

WARMUPS=1
ITERATIONS=3

mkdir -p results/2-edge-connectivity/
rm -f results/2-edge-connectivity/splay-top-tree.jsonl

for n in 500 1000 2000 3000 4000 5000 6000 7000 8000
do
    echo "Benchmark splay-top-tree 2-edge-connectivity with $n vertices"...
    ./bin/splay-top-trees/benchmark_2_edge $WARMUPS $ITERATIONS dataset/2-edge-connectivity/2edge_${n}_0.txt >> results/2-edge-connectivity/splay-top-tree.jsonl || exit
done

