#!/bin/bash

echo "gameplay rollout:"
for i in {1..10}
do
        echo "Game:\n"
        ./bin/game.out -rollout_game
        echo "Game:\n"
done
#./bin/game.out -run_rnd_exp
./bin/game.out -run_rollout
