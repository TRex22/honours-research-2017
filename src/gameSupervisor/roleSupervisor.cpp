/*Processes Abilities*/
/*
	 unit = 0,
	 horn = 1,
	 scorch = 2,
	 frost = 3,
	 fog = 4,
	 rain = 5,
	 clear = 6,
	 medic = 7,
	 decoy = 8,
	 spy = 9,
	 hero = 10,
	 bond = 11
*/

/* 	Basic structure:
	****************
	
	input: GameState,* player ID bool (0,1), assumed that the rule function implies card has been played.
	processing: update board, add card
				update player reward
				

*/

/*headers*/
GameState calculateCardPlay(GameState currentState, Card playCard);
GameState processUnitCard(GameState currentState, Card playCard);
GameState processHornCard(GameState currentState, Card playCard);
GameState processScorchCard(GameState currentState);
GameState processWeatherCard(GameState currentState, Card playCard);
GameState processMedicCard(GameState currentState, Card playCard);
GameState processDecoyCard(GameState currentState);
GameState processSpyCard(GameState currentState, Card playCard);
GameState processHeroCard(GameState currentState, Card playCard);
GameState processBondCard(GameState currentState, Card playCard);
GameState processRoundPass(GameState currentState);

GameState calculateCardPlay(GameState currentState, Card playCard)
{
	int typeId = playCard.typeId;

	if(currentState.currentPlayerId == 1)
	{
		currentState.player1Info.removeCardFromHand(playCard);
		// if(!currentState.player1Info.checkCardInHand(playCard))
		// {
		// 	std::cout<<"Invalid move. (before card play) " << "Card: "<<playCard.cardName<<std::endl;
		// 	print_deck(currentState.player1Info.getCurrentHand(), "P1");
		// 	return currentState;
		// }
	}
	else if(currentState.currentPlayerId == 2)
	{	
		currentState.player2Info.removeCardFromHand(playCard);
		// if(!currentState.player2Info.checkCardInHand(playCard))
		// {
		// 	std::cout<<"Invalid move. (before card play) " << "Card: "<<playCard.cardName<<std::endl;
		// 	print_deck(currentState.player1Info.getCurrentHand(), "P2");
		// 	return currentState;
		// }
	}
	else
	{
		std::cout<<"Invalid player id. (before card play) " << "Card: "<<playCard.cardName<<std::endl;
		return currentState;
	}

	// hack to fix medic ....
	// this is before the second card plays
	if(currentState.player1Info.medicSet)
	{
		currentState.player1Info.resetMedic();
	}
	else if(currentState.player2Info.medicSet)
	{	
		currentState.player2Info.resetMedic();
	}

	switch(typeId)
	{
		case rules(unit):
			currentState = processUnitCard(currentState, playCard);
			break;
		case rules(roundPass):
			currentState = processRoundPass(currentState);
		case rules(hero):
			currentState = processHeroCard(currentState, playCard);
			break;
		case rules(bond):
			currentState = processBondCard(currentState, playCard);
			break;
		case rules(medic):
			currentState = processMedicCard(currentState, playCard);
			break;
		case rules(horn):
			currentState = processHornCard(currentState, playCard);
			break;
		case rules(scorch):
			currentState = processScorchCard(currentState);
			break;
		case rules(frost):
			currentState = processWeatherCard(currentState, playCard);
			break;
		case rules(fog):
			currentState = processWeatherCard(currentState, playCard);
			break;
		case rules(rain):
			currentState = processWeatherCard(currentState, playCard);
			break;
		case rules(clear):
			currentState = processWeatherCard(currentState, playCard);
			break;
		case rules(decoy):
			currentState = processHeroCard(currentState, playCard);
			break;
		case rules(spy):
			currentState = processSpyCard(currentState, playCard);
			break;

		default: 
			// Just do nothing I guess
			break;
	}

	return currentState;
}

GameState processUnitCard(GameState currentState, Card playCard)
{
	if(currentState.currentPlayerId == 1)
	{
		if(playCard.locationId == location(closeId))
		{
			currentState.player1Info.addCardToBoard(playCard, location(closeId));
		}
		else if(playCard.locationId == location(rangedId))
		{
			currentState.player1Info.addCardToBoard(playCard, location(rangedId));
		}
		else if(playCard.locationId == location(siegeId))
		{
			currentState.player1Info.addCardToBoard(playCard, location(siegeId));
		}
	}
	else if(currentState.currentPlayerId == 2)
	{
		if(playCard.locationId == location(closeId))
		{
			currentState.player2Info.addCardToBoard(playCard, location(closeId));
		}
		else if(playCard.locationId == location(rangedId))
		{
			currentState.player2Info.addCardToBoard(playCard, location(rangedId));
		}
		else if(playCard.locationId == location(siegeId))
		{
			currentState.player2Info.addCardToBoard(playCard, location(siegeId));
		}
	}
	else
	{
		printf("Invalid player id. (unit card)\n");
	}

	return currentState;
}

GameState processHornCard(GameState currentState, Card playCard)
{
	currentState = processUnitCard(currentState, playCard);
	return currentState;
}

GameState processScorchCard(GameState currentState)
{
	/*Find Strongest Cards on both sides*/
	/*Remove the highest ap and update attackpoints for both players*/
	printf("Scorch has not been implemented\n");

	return currentState;
}

GameState processWeatherCard(GameState currentState, Card playCard)
{
	currentState.player1Info.addWeatherCardToBoard(playCard);
	currentState.player2Info.addWeatherCardToBoard(playCard);
	
	if(playCard.typeId == rules(clear))
	{
		currentState.weatherCards.clear();
		return currentState;
	}
	
	//else
	currentState.weatherCards.push_back(playCard);
	return currentState;
}

GameState processMedicCard(GameState currentState, Card playCard)
{
	currentState = processUnitCard(currentState, playCard);

	if(currentState.currentPlayerId == 1)
	{
		currentState.player1Info.initMedic();
	}
	else if(currentState.currentPlayerId == 2)
	{	
		currentState.player2Info.initMedic();
	}
	else
	{
		printf("Invalid player id. (medic card)\n");
	}

	return currentState;
}

GameState processDecoyCard(GameState currentState)
{
	// disabled for now
	printf("Decoy has not been implemented\n");
	return currentState;
}

GameState processSpyCard(GameState currentState, Card playCard)
{
	if(currentState.currentPlayerId == 1)
	{
		if(playCard.locationId == location(closeId))
		{
			currentState.player2Info.addCardToBoard(playCard, location(closeId));
		}
		else if(playCard.locationId == location(rangedId))
		{
			currentState.player2Info.addCardToBoard(playCard, location(rangedId));
		}
		else if(playCard.locationId == location(siegeId))
		{
			currentState.player2Info.addCardToBoard(playCard, location(siegeId));
		}

		currentState.player1Info.addCardsToHandFromPile(playCard);
	}
	else if(currentState.currentPlayerId == 2)
	{
		if(playCard.locationId == location(closeId))
		{
			currentState.player1Info.addCardToBoard(playCard, location(closeId));
		}
		else if(playCard.locationId == location(rangedId))
		{
			currentState.player1Info.addCardToBoard(playCard, location(rangedId));
		}
		else if(playCard.locationId == location(siegeId))
		{
			currentState.player1Info.addCardToBoard(playCard, location(siegeId));
		}
		currentState.player2Info.addCardsToHandFromPile(playCard);
	}
	else
	{
		printf("Invalid player id. (spy card)\n");
	}

	return currentState;
}

GameState processHeroCard(GameState currentState, Card playCard)
{
	// Lol hack
	currentState = processUnitCard(currentState, playCard);
	return currentState;
}

GameState processBondCard(GameState currentState, Card playCard)
{
	// normal but if there are other cards which are the same then double the sum of their ap
	currentState = processUnitCard(currentState, playCard);
	return currentState;
}

GameState processRoundPass(GameState currentState)
{
	if(currentState.currentPlayerId == 1)
	{
		currentState.player1Info.passedRound = true;
		currentState.player1Info.medicSet = false;
	}
	else if(currentState.currentPlayerId == 2)
	{	
		currentState.player2Info.passedRound = true;
		currentState.player2Info.medicSet = false;
	}
	else
	{
		printf("Invalid player id. (round pass)\n");
	}
	return currentState;
}