# SplayTopTreeBench
Benchmarking scripts and datageneration tools for SplayTopTrees 


## Compile, build and benchmark
Requires cmake, make, rust and gcc.
```
cd benchmark
./build_benchmarks.sh
./benchmark_all.sh
```
Results in ```benchmark/results/```.


## Plot
```
cd benchmark
python plot_tools/tabulate.py <mst|mst-ogbl|connectivity|all>
python plot_tools/plot.py <mst|mst-ogbl|connectivity|all>
```
## D-trees
The 'd-tree' folder is a copy of the dtree library by David Eisenstat and can be found on his personal website https://www.davideisenstat.com/dtree/ 

## STT-C
The search tree on tree implementation found in the 'stt-c' folder is a preliminary implementation kindly provided by Berendsohn.
