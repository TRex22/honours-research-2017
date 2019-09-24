double calculate_reward(GameState currentState, int playerId)
{
	double RW = 0.0;
	if(playerId == 1)
	{
		RW = 0.2*(currentState.player1Info.attackPoints.getCurrentAttackPoints() - currentState.player2Info.attackPoints.getCurrentAttackPoints())
			+ 0.2*(currentState.player1Info.countHandSize() - currentState.player2Info.countHandSize())
			+ 0.6*currentState.player1Info.roundWins;
	}
	else if(playerId == 2)
	{	
		RW = 0.2*(currentState.player2Info.attackPoints.getCurrentAttackPoints() - currentState.player1Info.attackPoints.getCurrentAttackPoints())
			+ 0.2*(currentState.player2Info.countHandSize() - currentState.player1Info.countHandSize())
			+ 0.6*currentState.player2Info.roundWins;
	}
	else
	{
		printf("Invalid player id.\n");
	}

	return RW;
}

double calculate_completed_reward(GameState currentState, int playerId)
{
	double RW = 0.0;
	
	if(playerId == 1)
	{
		RW = 0.2*(currentState.player1Info.attackPoints.getCurrentAttackPoints() - currentState.player2Info.attackPoints.getCurrentAttackPoints())
			+ 0.2*(currentState.player1Info.countHandSize() - currentState.player2Info.countHandSize())
			+ 0.6*currentState.player1Info.roundWins;
	}
	else if(playerId == 2)
	{	
		RW = 0.2*(currentState.player2Info.attackPoints.getCurrentAttackPoints() - currentState.player1Info.attackPoints.getCurrentAttackPoints())
			+ 0.2*(currentState.player2Info.countHandSize() - currentState.player1Info.countHandSize())
			+ 0.6*currentState.player2Info.roundWins;
	}
	else
	{
		printf("Invalid player id.\n");
	}

	return RW;
}