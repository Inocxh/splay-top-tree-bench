#!/bin/bash

echo "Building benchmarks..."
cd ..
bash ./build_benchmarks.sh
cd benchmark
bash benchmark_connectivity_random.sh
bash benchmark_mst_ogbl.sh
bash benchmark_mst_random.sh

