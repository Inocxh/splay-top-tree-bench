#!/bin/bash

cd "$(dirname "$0")"

#First generate msf data.
echo "Generating dataset for mst"
mkdir -p ../dataset/mst/random/
python3 gen_MSF_random.py "mst" 1

#Then build generator for connec data
echo "Building generator connectivity dataset"
mkdir -p bin/
cd ../../stt-rs/stt-benchmarks/
RUSTFLAGS=-Awarnings
cargo build --bin gen_queries --release 
mv target/release/gen_queries ../../benchmark/data-generation/bin/gen_queries

#Now generate the connectivity data
echo "Generating data for connectivity"
cd ../../benchmark/data-generation/
mkdir -p ../dataset/connectivity/mix/
mkdir -p ../dataset/connectivity/query_only/
mkdir -p ../dataset/connectivity/update_only/
mkdir -p ../dataset/connectivity/zipf/
mkdir -p ../dataset/connectivity/zipf-query/
python3 gen_connec_data.py "con" 1

#Generate the 2-edge connectivity data
echo "Generating data for 2-edge connectivity"
mkdir -p ../dataset/2-edge-connectivity/
python3 gen_2_edge_data.py "2edge" 1

#Create a folder for ogbl-data
mkdir -p ../dataset/mst/ogbl-collab/
echo "Remember to download the ogbl-collab data and process it as described in stt-rs/data/ogbl-collab!"