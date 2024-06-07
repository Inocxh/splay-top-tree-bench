# SplayTopTreeBench
Benchmarking scripts and datageneration tools for SplayTopTrees 


## Compile, build and benchmark
Requires cmake, make, rust, g++.
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
