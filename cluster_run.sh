#!/bin/bash
# pssh -i -h pssh_hosts rm /scratch/jchalom/
make main
echo "start..."
for i in {1..10}
do
	echo $1
	./bin/game.out -run_mcts_exp
done