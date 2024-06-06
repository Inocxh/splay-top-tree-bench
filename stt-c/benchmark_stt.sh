#!/bin/bash

DATA_FILE=connec.jsonl
REPEAT=1
SETS=1
WARMUPS=1
ITERATIONS=3


if [ -f $INPUT_FILE ]
then
    mkdir -p stt-c/
    rm -f stt-c/$DATA_FILE
    
    for n in 500 1000 2000 3000 4000 5000 6000 7000 8000
    do
        echo "Benchmark stt-c connectivity with $n vertices"...
        for i in $(eval echo {0..$(($SETS - 1))})
        do
            echo "Set $i" 
            for _ in $(eval echo {1..$REPEAT})
            do
                ./bin/greedy_stt bench 1 $WARMUPS $ITERATIONS dataset/connectivity/con_${n}_${i}.txt >> stt-c/$DATA_FILE || exit
            done
        done
    done
fi
