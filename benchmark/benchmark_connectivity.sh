#!/bin/bash

WARMUPS=1
ITERATIONS=3

SIZES="1000 2000 5000 10000 20000 50000 100000 200000 500000 1000000 2000000 5000000 10000000 1500000 2000000"
TEST_PATH="dataset/connectivity/mix/con_"
RESULT_PATH="results/connectivity/mix/" 
RUST_IMPLS="link-cut greedy-splay"

mkdir -p $RESULT_PATH
rm -f "$RESULT_PATH"splay-top-tree.jsonl

for n in $SIZES
do
    echo "Benchmark splay-top-tree full splay connectivity with $n vertices"...           
    ./bin/splay-top-trees/benchmark_connectivity $WARMUPS $ITERATIONS $TEST_PATH${n}_0.txt >>  "$RESULT_PATH"splay-top-tree.jsonl || exit   
done

for n in $SIZES
do
    echo "Benchmark splay-top-tree semi splay connectivity with $n vertices"...           
    ./bin/splay-top-trees/benchmark_connectivity_semi_splay $WARMUPS $ITERATIONS $TEST_PATH${n}_0.txt >> "$RESULT_PATH"splay-top-tree.jsonl || exit   
done


rm -f "$RESULT_PATH"stt-c.jsonl

for n in $SIZES
do
    echo "Benchmark stt-c connectivity with $n vertices"...
    ./bin/stt-c/greedy_stt bench 1 $WARMUPS $ITERATIONS $TEST_PATH${n}_0.txt >> "$RESULT_PATH"stt-c.jsonl || exit   
done


rm -f "$RESULT_PATH"stt-rs.jsonl

for n in $SIZES
do
    echo "Benchmark stt-rs connectivity with $n vertices"...
    ./bin/stt-rs/bench_queries_file --input-file $TEST_PATH${n}_0.txt --iterations $ITERATIONS --json $RUST_IMPLS >> "$RESULT_PATH"stt-rs.jsonl || exit
done

rm -f "$RESULT_PATH"d-trees.jsonl

for n in $SIZES
do
    echo "Benchmark d-trees connectivity with $n vertices"...
    ./bin/d-trees/connectivity $WARMUPS $ITERATIONS $TEST_PATH${n}_0.txt >> "$RESULT_PATH"d-trees.jsonl || exit
done