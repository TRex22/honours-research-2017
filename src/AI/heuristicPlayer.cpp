Card HeuristicPlayer(GameState currentState, int playerId)
{
	currentState.currentPlayerId = playerId;
	GameState copyGameState(currentState.northernDeck, currentState.nilfgaardDeck);
	copyGameState.copy(currentState);
	Card maxRWCard;
	double maxRW = 0;

	std::vector<Card> currentHand;

	if(playerId == 1)
	{
		currentHand = currentState.player1Info.getCurrentHand();
	
	}
	else if(playerId == 2)
	{	
		currentHand = currentState.player2Info.getCurrentHand();
	}
	else
	{
		printf("Invalid player id. (in heuristic AI)\n");
	}

	maxRWCard = currentHand[0];

	for (Card c : currentHand)
	{
		copyGameState = calculateCardPlay(copyGameState, c);
		double rw = calculate_reward(copyGameState, playerId);

		if(rw > maxRW)
		{
			maxRW = rw;
			maxRWCard = c;
		}

		copyGameState.copy(currentState);
	}

	return maxRWCard;
}