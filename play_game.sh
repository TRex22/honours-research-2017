#!/bin/bash
make clean
make

for i in {1..10}
do
# ./bin/game.out --run_game >> game_results/rollout_1000.txt
# ./bin/game.out --run_game >> game_results/mcts_100.txt
./bin/game.out --run_game >> game_results/mcts2_100.txt
# ./bin/game.out --run_game >> game_results/bmcts_10.txt
done
