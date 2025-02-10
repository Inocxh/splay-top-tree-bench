# SplayTopTreeBench
Benchmarking scripts and datageneration tools for SplayTopTrees 

## Requirements
The following software is required to run this software:
-   cmake version 3.31.5
-   rustc 1.84.1
-   cargo 1.84.1
-   gcc (GCC) 14.2.1
-   Python 3.13.1
-   pandas 2.2.3
-   matokitkub 3.10.0


## Compile, build and benchmark
To generate the tests execute the following commands:
```
cd 
cd benchmark/data-generation
./generate_data.sh
```
Download the ogbl-collab dataset from http://snap.stanford.edu/ogb/data/linkproppred/collab.zip.
Then extract ```edge.csv, edge_weight.csv, edge_year.csv``` from the ```collab``` folder. Place these in ```stt-rs/data/ogbl-collab```
Run ```./process.py``` and place ```ogbl-collab.txt``` in ```dataset/mst/ogbl-collab/```

To run the tests execute the follwing commands:
```
cd benchmark
./build_benchmarks.sh
./benchmark_all.sh
```
Results will be placed in in ```benchmark/results/```.


## Plot
To generate plots for the data use the following commands:
```
cd benchmark
python plot_tools/tabulate.py <mst|mst-ogbl|connectivity|all>
python plot_tools/plot.py <mst|mst-ogbl|connectivity|all>
```
## D-trees
The 'd-tree' folder is a copy of the dtree library by David Eisenstat and can be found on his personal website https://www.davideisenstat.com/dtree/ 

## STT-C
The search tree on tree implementation found in the 'stt-c' folder is a preliminary implementation kindly provided by Berendsohn.
