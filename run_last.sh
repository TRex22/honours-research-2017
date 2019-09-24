#!/bin/bash
make clean
make
./bin/game.out --special1
./bin/game.out --heuristic
#./bin/game.out --special1
./bin/game.out --special2
