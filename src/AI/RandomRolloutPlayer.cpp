Card RandomRolloutPlayer(GameState startState, int playerId, int numberOfIterations)
{
	startState.currentPlayerId = playerId;
	std::random_device d;
	std::mt19937 rng(d());

	Card bestCard;
	std::vector<Card> currentHand;
	std::vector<Card> unknownHand;

	if(playerId == 1)
	{
		currentHand = startState.player1Info.getCurrentHand();
		unknownHand = GetUnknownHand(startState, 2);
	
	}
	else if(playerId == 2)
	{	
		currentHand = startState.player2Info.getCurrentHand();
		unknownHand = GetUnknownHand(startState, 1);
	}
	else
	{
		printf("Invalid player id. (in heuristic AI)\n");
	}

	std::vector<int> maxAccumulativeWins(currentHand.size(), 0);
	maxAccumulativeWins.reserve(currentHand.size());

	// #pragma omp for nowait
	for (unsigned int c=0; c<currentHand.size(); c++)
	{
		#pragma omp parallel 
		{
			#pragma omp for nowait
			for(int i = 0; i<numberOfIterations; i++)
			{
				// std::cout<<"i: "<<i<<std::endl;

				GameState copyState(startState.northernDeck, startState.nilfgaardDeck);
				copyState.copy(startState);

				if(playerId == 1)
					copyState.player2Info.setCurrentHand(unknownHand);
				else if(playerId == 2)
					copyState.player1Info.setCurrentHand(unknownHand);

				// Play card
				copyState = calculateCardPlay(copyState, currentHand[c]);
				while(copyState.isComplete == false)
				{
					copyState = determine_round_end_win(copyState, false); 

					// Random Opponent Play
					if(playerId == 1 && !copyState.player2Info.passedRound)
					{
						copyState = playRndPlayer2(copyState, false);
						copyState = determine_round_end_win(copyState, false);  //check if this make sense
					}
					else if (playerId == 2 && !copyState.player1Info.passedRound)
					{
						copyState = playRndPlayer1(copyState, false);
					}

					//player play random till end
					if(playerId == 1 && !copyState.player1Info.passedRound)
					{	
						copyState = playRndPlayer1(copyState, false);
					}
					else if (playerId == 2 && !copyState.player2Info.passedRound)
					{
						copyState = playRndPlayer2(copyState, false);
					}
				}

				int game_winner = determine_winner(copyState, false);
				#pragma omp critical
				{
					if(game_winner == playerId)
					{
						maxAccumulativeWins[c] = maxAccumulativeWins[c]+1;
					}
				}
			}
		}
	}


	int id = std::distance(maxAccumulativeWins.begin(), std::max_element(maxAccumulativeWins.begin(), maxAccumulativeWins.end()));
	bestCard = currentHand[id];
	return bestCard;
}