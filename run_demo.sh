#!/bin/bash
for i in {1..1000}
do
 ./bin/demo_rnd_rnd.out
 ./bin/demo_mcts_rnd.out
 ./bin/demo_mcts_fast.out
done
