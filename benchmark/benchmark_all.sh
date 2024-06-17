#!/bin/bash

cd "$(dirname "$0")"
echo "Building benchmarks..."
bash build_benchmarks.sh
bash benchmark_connectivity.sh
bash benchmark_mst_ogbl.sh
bash benchmark_mst.sh
bash benchmark_connectivity_queries.sh
bash benchmark_connectivity_updates.sh
bash benchmark_connectivity_zipf.sh
bash benchmark_connectivity_zipf_query.sh

