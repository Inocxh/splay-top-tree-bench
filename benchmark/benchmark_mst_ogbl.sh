#!/bin/bash


INPUT_FILE=dataset/mst/ogbl-collab/incremental_mst.txt
DATA_FILE=mst_ogbl.jsonl
REPEAT=5
WARMUPS=1
ITERATIONS=3


if [ -f $INPUT_FILE ]
then
    mkdir -p results/splay_top_tree/
    rm -f results/splay_top_tree/$DATA_FILE
    
    echo "Benchmark MST on ogbl-collab dataset..."
    for _ in $(eval echo {1..$REPEAT})
    do
        ./../build/benchmark_mst $WARMUPS $ITERATIONS $INPUT_FILE >> results/splay_top_tree/$DATA_FILE || exit
    done
else
    echo "Dataset file $INPUT_FILE does not exist"
fi
