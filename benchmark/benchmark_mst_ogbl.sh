#!/bin/bash

WARMUPS=1
ITERATIONS=3


mkdir -p results/mst-ogbl/
rm -f results/mst-ogbl/splay-top-tree.jsonl
echo "Running splay-top-trees on ogbl-collab"
./bin/splay-top-trees/benchmark_mst $WARMUPS $ITERATIONS dataset/mst/ogbl-collab/ogbl-collab.txt >> results/mst-ogbl/splay-top-tree.jsonl || exit

mkdir -p results/mst-ogbl/
rm -f results/mst-ogbl/stt-rs.jsonl
echo "Running stt-rs on ogbl-collab"
./bin/stt-rs/bench_mst -i dataset/mst/ogbl-collab/ogbl-collab.txt --json --iterations $ITERATIONS >> results/mst-ogbl/stt-rs.jsonl || exit

