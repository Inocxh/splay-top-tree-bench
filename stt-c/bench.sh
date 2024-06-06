#!/bin/bash

REPEAT=10

for i in {0..6} {10..13}; do
  echo "++ VARIANT $i ++"
  make --silent bin/greedy_stt_var$i && ./bin/greedy_stt_var$i bench $REPEAT test_input.txt || echo "Error in build or execution"
  echo
done
