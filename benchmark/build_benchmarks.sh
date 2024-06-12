#!/bin/bash

cd "$(dirname "$0")"
#Splay top trees
mkdir -p ../splay-top-trees/build
cd ../splay-top-trees/build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build . --target benchmark_mst benchmark_connectivity benchmark_2_edge benchmark_connectivity_semi_splay benchmark_2_edge_semi_splay --config Release
mkdir -p ../../benchmark/bin/splay-top-trees/
mv benchmark_mst ../../benchmark/bin/splay-top-trees/benchmark_mst
mv benchmark_connectivity ../../benchmark/bin/splay-top-trees/benchmark_connectivity
mv benchmark_2_edge ../../benchmark/bin/splay-top-trees/benchmark_2_edge
mv benchmark_connectivity_semi_splay ../../benchmark/bin/splay-top-trees/benchmark_connectivity_semi_splay
mv benchmark_2_edge_semi_splay ../../benchmark/bin/splay-top-trees/benchmark_2_edge_semi_splay
# stt-c
cd ../../stt-c
make
mkdir -p ../benchmark/bin/stt-c
mv bin/greedy_stt ../benchmark/bin/stt-c/greedy_stt

# stt-rs
cd ../stt-rs/stt-benchmarks
cargo build --bin bench_queries_file --release
cargo build --bin bench_mst --release
mkdir -p ../../benchmark/bin/stt-rs
mv target/release/bench_queries_file ../../benchmark/bin/stt-rs/bench_queries_file
mv target/release/bench_mst ../../benchmark/bin/stt-rs/bench_mst


cd ../../d-trees
make bin/connectivity
mkdir -p ../benchmark/bin/d-trees
mv bin/connectivity ../benchmark/bin/d-trees/connectivity

cd ../benchmark/


