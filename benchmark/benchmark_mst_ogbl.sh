#!/bin/bash

WARMUPS=1
ITERATIONS=3

if [ -f $INPUT_FILE ]
then
    mkdir -p results/mst-ogbl-collab/
    rm -f results/mst-ogbl-collab/splay-top-tree.jsonl
    echo "starting"
    ./bin/splay-top-trees/benchmark_mst $WARMUPS $ITERATIONS dataset/mst/ogbl-collab/incremental_mst.txt >> results/mst-ogbl-collab/splay-top-tree.jsonl || exit

fi
if [ -f $INPUT_FILE ]
then
    mkdir -p results/mst/
    rm -f results/mst/stt-rs.jsonl
   echo "starting"
    ./bin/stt-rs/bench_mst -i dataset/mst/ogbl-collab/incremental_mst.txt --json --iterations $ITERATIONS >> results/mst-ogbl-collab/stt-rs.jsonl || exit
fi
