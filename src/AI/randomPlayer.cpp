Card RandomPlayer(GameState currentState, int playerId)
{
	currentState.currentPlayerId = playerId;
	
	std::random_device d;
	std::mt19937 rng(d());

	if(playerId == 1)
	{
		std::vector<Card> currentHand = currentState.player1Info.getCurrentHand();
		// std::shuffle(std::begin(currentHand), std::end(currentHand), rng);
		std::uniform_int_distribution<int> distribution(0,currentHand.size()-1);
		return currentHand[distribution(rng)];
	}
	else if(playerId == 2)
	{	
		std::vector<Card> currentHand = currentState.player2Info.getCurrentHand();
		// std::shuffle(std::begin(currentHand), std::end(currentHand), rng);
		std::uniform_int_distribution<int> distribution(0,currentHand.size()-1);
		return currentHand[distribution(rng)];
	}
	else
	{
		printf("Invalid player id.\n");
	}

	Card empty;
	return empty;
}

Card FastRandomPlayer(GameState currentState, int playerId)
{
	currentState.currentPlayerId = playerId;

	if(playerId == 1)
	{
		std::vector<Card> currentHand = currentState.player1Info.getCurrentHand();
		std::shuffle(std::begin(currentHand), std::end(currentHand), FastRandRNG());
		return currentHand[0];
	}
	else if(playerId == 2)
	{	
		std::vector<Card> currentHand = currentState.player2Info.getCurrentHand();
		std::shuffle(std::begin(currentHand), std::end(currentHand), FastRandRNG());
		return currentHand[0];
	}
	else
	{
		printf("Invalid player id.\n");
	}

	Card empty;
	return empty;
}

Card XorshfRandomPlayer(GameState currentState, int playerId)
{
	currentState.currentPlayerId = playerId;

	if(playerId == 1)
	{
		std::vector<Card> currentHand = currentState.player1Info.getCurrentHand();
		std::shuffle(std::begin(currentHand), std::end(currentHand), MarsagliaXorshfRNG());
		return currentHand[0];
	}
	else if(playerId == 2)
	{	
		std::vector<Card> currentHand = currentState.player2Info.getCurrentHand();
		std::shuffle(std::begin(currentHand), std::end(currentHand), MarsagliaXorshfRNG());
		return currentHand[0];
	}
	else
	{
		printf("Invalid player id.\n");
	}

	Card empty;
	return empty;
}