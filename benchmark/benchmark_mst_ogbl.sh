#!/bin/bash

WARMUPS=1
ITERATIONS=3


mkdir -p results/mst-ogbl/
rm -f results/mst-ogbl/splay-top-tree.jsonl
echo "Running splay-top-trees..."
./bin/splay-top-trees/benchmark_mst $WARMUPS $ITERATIONS dataset/mst/ogbl/ogbl-collab.txt >> results/mst-ogbl/splay-top-tree.jsonl || exit

mkdir -p results/mst-ogbl/
rm -f results/mst-ogbl/stt-rs.jsonl
echo "Running stt-rs..."
./bin/stt-rs/bench_mst -i dataset/mst/ogbl/ogbl-collab.txt --json --iterations $ITERATIONS >> results/mst-ogbl/stt-rs.jsonl || exit

