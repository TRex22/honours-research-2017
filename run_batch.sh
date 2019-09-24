#!/bin/bash
make clean
make main

for i in {1..100}
do
./bin/game.out --run_mcts_exp
done
