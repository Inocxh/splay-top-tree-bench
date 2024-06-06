#DATA_FILE=mst.jsonl
REPEAT=1
SETS=3
WARMUPS=1
ITERATIONS=3


if [ -f $INPUT_FILE ]
then
    mkdir -p results/mst/
    rm -f results/mst/splay-top-tree.jsonl
    
    for n in 1000 2000 5000 10000 20000 50000 100000 200000 500000 1000000
    do
        echo "Benchmark splay-top-tree MST with $n vertices"...
        for i in $(eval echo {0..$(($SETS - 1))})
        do
            echo "Set $i" 
            for _ in $(eval echo {1..$REPEAT})
            do
                ./bin/splay-top-trees/benchmark_mst $WARMUPS $ITERATIONS dataset/mst/random/mst_${n}_${i}.txt >> results/mst/splay-top-tree.jsonl || exit
            done
        done
    done
fi
if [ -f $INPUT_FILE ]
then
    mkdir -p results/mst/
    rm -f results/mst/stt-rs.jsonl
    
    for n in 1000 2000 5000 10000 20000 50000 100000 200000 500000 1000000
    do
        echo "Benchmark stt-rs MST with $n vertices"...
        for i in $(eval echo {0..$(($SETS - 1))})
        do
            echo "Set $i" 
            for _ in $(eval echo {1..$REPEAT})
            do
                ./bin/stt-rs -i dataset/mst/random/mst_${n}_${i}.txt --json >> results/mst/stt-rs.jsonl || exit
            done
        done
    done
fi