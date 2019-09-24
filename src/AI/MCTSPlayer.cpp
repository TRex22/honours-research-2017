Node* playPlayer1(Node* currentState)
{
	if (currentState->gameState.player1Info.passedRound == false)
	{
		do
		{
			currentState->gameState.currentPlayerId = 1;
			Card playCard1 = RandomPlayer(currentState->gameState, 1);

			currentState->card = playCard1;
			GameState gs = calculateCardPlay(currentState->gameState, playCard1);
			currentState->gameState = gs;

			currentState->gameState.currentPlayerId = 1;
		}
		while (currentState->gameState.player1Info.medicSet);
	}

	return currentState;
}

Node* playPlayer2(Node* currentState)
{
	if (currentState->gameState.player2Info.passedRound == false)
	{
		do
		{
			currentState->gameState.currentPlayerId = 2;
			Card playCard2 = RandomPlayer(currentState->gameState, 2);
			currentState->card = playCard2;
			GameState gs = calculateCardPlay(currentState->gameState, playCard2);
			currentState->gameState = gs;

			currentState->gameState.currentPlayerId = 2;
		}
		while (currentState->gameState.player2Info.medicSet);
	}

	return currentState;
}

Node* randomAction(Node* currentState, int playerId)
{
	// Node* copyState = new Node(currentState);
	//todo check
	auto copyState = new Node(currentState);
	copyState->gameState = determine_round_end_win(copyState->gameState, false);

	// Random Opponent Play
	if (playerId == 1 && !copyState->gameState.player2Info.passedRound)
	{
		copyState = playPlayer2(copyState);
		copyState->gameState = determine_round_end_win(copyState->gameState, false);  //check if this make sense
	}
	else if (playerId == 2 && !copyState->gameState.player1Info.passedRound)
	{
		copyState = playPlayer1(copyState);
	}

	//player play random till end
	if (playerId == 1 && !copyState->gameState.player1Info.passedRound)
	{
		copyState = playPlayer1(copyState);
	}
	else if (playerId == 2 && !copyState->gameState.player2Info.passedRound)
	{
		copyState = playPlayer2(copyState);
	}

	return copyState;
}

Card rollout2(GameState startState, int playerId, int numberOfIterations)
{
	Card bestCard;
	std::vector<Card> currentHand;
	std::vector<Card> unknownHand;

	if (playerId == 1)
	{
		currentHand = startState.player1Info.getCurrentHand();
		unknownHand = GetUnknownHand(startState, 2);

	}
	else if (playerId == 2)
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
	// std::cout<<unknownHand.size()<<std::endl;

	#pragma omp parallel for
	for (unsigned int c = 0; c < currentHand.size(); c++)
	{
		for (int i = 0; i < numberOfIterations; i++)
		{
			GameState copyState(startState.northernDeck, startState.nilfgaardDeck);
			copyState.copy(startState);

			if (playerId == 1)
				copyState.player2Info.setCurrentHand(unknownHand);
			else if (playerId == 2)
				copyState.player1Info.setCurrentHand(unknownHand);

			copyState = calculateCardPlay(copyState, currentHand[c]);

			while (copyState.isComplete)
			{
				auto currentState = new Node(nullptr, copyState, currentHand[c]);
				copyState = randomAction(currentState, playerId)->gameState;
			}

			int game_winner = determine_winner(copyState, false);
			#pragma omp critical
			{
				if (game_winner == playerId)
				{
					maxAccumulativeWins[c] = maxAccumulativeWins[c] + 1;
				}
			}
		}
	}

	int id = std::distance(maxAccumulativeWins.begin(), std::max_element(maxAccumulativeWins.begin(), maxAccumulativeWins.end()));
	bestCard = currentHand[id];
	return bestCard;
}

Node* calculateCumulativeReward(Node* currentState, int playerId)
{
	int game_winner = determine_winner(currentState->gameState, false);
	if (playerId == 1 && game_winner == 1)
		currentState->cumulative_reward += 1.0;
	else if (playerId == 2 && game_winner == 2)
		currentState->cumulative_reward += 1.0;
	else if (game_winner == 0)
		currentState->cumulative_reward += 0.5;
	else
		currentState->cumulative_reward += -1.0;

	return currentState;
}

// new reference: https://www.cs.swarthmore.edu/~bryce/cs63/s16/slides/2-15_MCTS.pdf
Node* UCB1Sample(Node* parent, int playerId)
{
	double C = 1.0; // Do We Need? TODO train
	std::vector<double> weights;
	auto children = parent->children;
	double sumWeights = 0.0;

	// std::cout<<children.size()<<std::endl;
	for (auto c : children)
	{
		double weight = (c->value + C) * sqrt(log(parent->ns) / c->ns);
		weights.push_back(weight);
		sumWeights += weight;
	}

	// instead of storing every distribution value just find the max
	auto maxChild = randomAction(parent, playerId);
	double maxDist = 0.0;
	// std::cout << parent->children.size() << std::endl;
	for (unsigned int i = 0; i < weights.size(); i++)
	{
		double dist = weights[i] / sumWeights;
		// std::cout << dist << "max: " << maxDist << std::endl;
		if (dist > maxDist)
		{
			maxDist = dist;
			delete maxChild;
			maxChild = children[i];

			// remove the card from the gamestate ...
			if (playerId == 1)
				maxChild->gameState.currentPlayerId = 1;
			else if (playerId == 2)
				maxChild->gameState.currentPlayerId = 2;

			maxChild->gameState = calculateCardPlay(maxChild->gameState, maxChild->card);
		}
	}

	return maxChild;
}

Card MCTSPlayer(GameState startState, int playerId, int numberOfIterations)
{
	startState.currentPlayerId = playerId;

	//todo optimize
	GameState copyState(startState.northernDeck, startState.nilfgaardDeck);
	copyState.copy(startState);
	auto tree = new Tree(copyState);

	std::vector<Card> currentHand;
	std::vector<Card> unknownHand;

	if (playerId == 1)
	{
		currentHand = startState.player1Info.getCurrentHand();
		unknownHand = GetUnknownHand(startState, 2);

	}
	else if (playerId == 2)
	{
		currentHand = startState.player2Info.getCurrentHand();
		unknownHand = GetUnknownHand(startState, 1);
	}
	else
	{
		printf("Invalid player id. (in heuristic AI)\n");
	}

	Card bestCard;
	int bestResult = -1;
	std::vector<Node*> bestSolution;
	std::vector<Node*> delete_tracker;

	// Todo optimize
	// Parallel
	for (int s = 0; s < numberOfIterations; s++)
	{
		Node* currentState = tree->root;
		std::vector<Node*> solution;
		Node* previousState = currentState;

		if (playerId == 1)
			currentState->gameState.player2Info.setCurrentHand(unknownHand);
		else if (playerId == 2)
			currentState->gameState.player1Info.setCurrentHand(unknownHand);

		// std::cout<<"Selection"<<std::endl;
		while (tree->contains(currentState))
		{
			previousState = currentState;
			currentState = UCB1Sample(previousState, playerId);
			solution.push_back(currentState);
		}

		// std::cout<<"Expansion"<<std::endl;
		tree->add(previousState, currentState);

		// std::cout<<"Simulation"<<std::endl;
		while (!currentState->gameState.isComplete)
		{
			currentState = randomAction(currentState, playerId);
			solution.push_back(currentState);
			delete_tracker.push_back(currentState);
		}

		// std::cout<<"Back Prop Reward"<<std::endl;
		currentState = calculateCumulativeReward(currentState, playerId);

		// std::cout<<"Backpropogation"<<std::endl;
		for (auto s : solution)
		{
			s->ns ++;
			s->cumulative_reward = currentState->cumulative_reward;
			s->value = s->cumulative_reward / s->ns;
		}

		if (currentState->cumulative_reward >= bestResult)
		{
			bestResult = currentState->cumulative_reward;
			bestSolution = solution;
			bestCard = Card(bestSolution[0]->card);
		}

		// for (Node* n : delete_tracker)
		// 	delete n;
		// delete_tracker.clear();
		solution.clear();
	}

	for (Node* n : delete_tracker)
		delete n;
	delete_tracker.clear();
	bestSolution.clear();

	delete tree;

	if (playerId == 1)
		bestCard = startState.player1Info.getCardFromHand(bestCard.cardName, bestCard.locationId);
	else if (playerId == 2)
		bestCard = startState.player2Info.getCardFromHand(bestCard.cardName, bestCard.locationId);

	return bestCard;
}

Card MCTSPlayer2(GameState startState, int playerId, int numberOfIterations)
{
	startState.currentPlayerId = playerId;

	//todo optimize
	GameState copyState(startState.northernDeck, startState.nilfgaardDeck);
	copyState.copy(startState);
	auto tree = new Tree(copyState);

	std::vector<Card> currentHand;
	std::vector<Card> unknownHand;

	if (playerId == 1)
	{
		currentHand = startState.player1Info.getCurrentHand();
		unknownHand = GetUnknownHand(startState, 2);

	}
	else if (playerId == 2)
	{
		currentHand = startState.player2Info.getCurrentHand();
		unknownHand = GetUnknownHand(startState, 1);
	}
	else
	{
		printf("Invalid player id. (in heuristic AI)\n");
	}

	Card bestCard;
	int bestResult = -1;
	std::vector<Node*> bestSolution;
	std::vector<Node*> delete_tracker;

	// Todo optimize
	// Parallel
	for (int s = 0; s < numberOfIterations; s++)
	{
		Node* currentState = tree->root;
		std::vector<Node*> solution;
		Node* previousState = currentState;

		if (playerId == 1)
			currentState->gameState.player2Info.setCurrentHand(unknownHand);
		else if (playerId == 2)
			currentState->gameState.player1Info.setCurrentHand(unknownHand);

		// std::cout<<"Selection"<<std::endl;
		while (tree->contains(currentState))
		{
			previousState = currentState;
			currentState = UCB1Sample(previousState, playerId);
			solution.push_back(currentState);
		}

		// std::cout<<"Expansion"<<std::endl;
		tree->add(previousState, currentState);

		// std::cout<<"Simulation"<<std::endl;
		while (!currentState->gameState.isComplete)
		{
			currentState = randomAction(currentState, playerId);
			solution.push_back(currentState);
			delete_tracker.push_back(currentState);
		}

		// std::cout<<"Back Prop Reward"<<std::endl;
		currentState = calculateCumulativeReward(currentState, playerId);

		// std::cout<<"Backpropogation"<<std::endl;
		for (auto s : solution)
		{
			s->ns ++;
			s->cumulative_reward = currentState->cumulative_reward;
			s->value = s->cumulative_reward / s->ns;
		}

		if (currentState->cumulative_reward >= bestResult)
		{
			bestResult = currentState->cumulative_reward;
			bestSolution = solution;
			bestCard = Card(bestSolution[0]->card);
		}

		for (Node* n : delete_tracker)
			delete n;
		delete_tracker.clear();
		solution.clear();
	}

	for (Node* n : delete_tracker)
		delete n;
	delete_tracker.clear();
	bestSolution.clear();

	delete tree;

	if (playerId == 1)
		bestCard = startState.player1Info.getCardFromHand(bestCard.cardName, bestCard.locationId);
	else if (playerId == 2)
		bestCard = startState.player2Info.getCardFromHand(bestCard.cardName, bestCard.locationId);

	return bestCard;
}

std::vector<Node*> mostVistedTreeNodes(std::vector<Node*> nodeSet, int BEAMWIDTH)
{
	std::sort(nodeSet.begin(), nodeSet.end(), [](Node * one, Node * two) {return one->ns > two->ns;});
	std::vector<Node*> nodeSetWidth;

	if (BEAMWIDTH < (int)nodeSet.size())
	{
		for (int i = 0; i < BEAMWIDTH; i++)
		{
			nodeSetWidth.push_back(nodeSet[i]);
		}

		return nodeSetWidth;
	}

	return nodeSet;
}

void cleanUpTree(Tree* tree, std::vector<Node*> nodeSet)
{
	std::vector<Node*> keepNodes;
	// keepNodes.push_back(tree->root);

	for (auto n : nodeSet)
	{
		if (n != tree->root)
		{
			if (n->parent == tree->root)
			{
				keepNodes.push_back(n);
			}
			else
			{
				Node* check = n->parent;
				while (check != tree->root)
				{
					if (check->parent == tree->root)
					{
						keepNodes.push_back(check);
					}

					check = check->parent;
				}
			}
		}
	}

	// std::cout<<"keep: "<<tree->root->children.size()<<" nodest: "<<nodeSet.size()<<std::endl;

	if (tree->root->children.size() > 2)
	{
		std::vector<Node*> deleteNodes = tree->root->children;
		for (auto c : tree->root->children)
		{
			bool check = false;
			for (auto k : keepNodes)
			{
				if (c == k)
				{
					check = true;
				}
			}

			if (!check)
			{
				deleteNodes.erase(std::remove_if(deleteNodes.begin(),
				                                 deleteNodes.end(),
				[&](Node * x) {return x == c;}),
				deleteNodes.end());
				delete c;
			}
		}
		tree->root->children = deleteNodes;
	}
	// std::cout << "R children: " << tree->root->children.size() << " delsize: " << deleteNodes.size() << std::endl;
}

void pruneTree(Tree* tree, int depth, int BEAMWIDTH)
{
	// std::cout << "Prune Tree" << std::endl;
	std::vector<Node*> nodeSet = tree->treeNodesAtDepth(depth);
	nodeSet = mostVistedTreeNodes(nodeSet, BEAMWIDTH);
	cleanUpTree(tree, nodeSet);
}

Card BMCTSPlayer(GameState startState, int playerId, int numberOfIterations, int SIMLIMIT, int BEAMWIDTH)
{
	startState.currentPlayerId = playerId;

	//todo optimize
	GameState copyState(startState.northernDeck, startState.nilfgaardDeck);
	copyState.copy(startState);
	auto tree = new Tree(copyState);

	std::vector<Card> currentHand;
	std::vector<Card> unknownHand;

	if (playerId == 1)
	{
		currentHand = startState.player1Info.getCurrentHand();
		unknownHand = GetUnknownHand(startState, 2);

	}
	else if (playerId == 2)
	{
		currentHand = startState.player2Info.getCurrentHand();
		unknownHand = GetUnknownHand(startState, 1);
	}
	else
	{
		printf("Invalid player id. (in heuristic AI)\n");
	}

	Card bestCard;
	int bestResult = -1;
	std::vector<Node*> bestSolution;
	std::vector<Node*> delete_tracker;

	// Todo optimize
	// Parallel
	for (int s = 0; s < numberOfIterations; s++)
	{
		Node* currentState = tree->root;
		currentState->number_rollouts++;
		std::vector<Node*> solution;
		Node* previousState = currentState;

		if (playerId == 1)
			currentState->gameState.player2Info.setCurrentHand(unknownHand);
		else if (playerId == 2)
			currentState->gameState.player1Info.setCurrentHand(unknownHand);

		// std::cout<<"Selection"<<std::endl;
		while (tree->contains(currentState))
		{
			previousState = currentState;
			currentState = UCB1Sample(previousState, playerId);
			currentState->number_rollouts++;
			solution.push_back(currentState);
		}

		// std::cout<<"Expansion"<<std::endl;
		tree->add(previousState, currentState);

		// std::cout<<"Simulation"<<std::endl;
		while (!currentState->gameState.isComplete)
		{
			currentState = randomAction(currentState, playerId);
			solution.push_back(currentState);
			delete_tracker.push_back(currentState);
		}

		// std::cout<<"Back Prop Reward"<<std::endl;
		currentState = calculateCumulativeReward(currentState, playerId);

		// std::cout<<"Backpropogation"<<std::endl;
		// int max_sims = 0;
		for (auto s : solution)
		{
			s->ns ++;
			s->cumulative_reward = currentState->cumulative_reward;
			s->value = s->cumulative_reward / s->ns;

			// prune --check
			// if(s->number_rollouts > max_sims)
			// 	max_sims = s->number_rollouts;

			if (s->number_rollouts >= SIMLIMIT && tree->root->children.size() > 2)
			{
				pruneTree(tree, s->number_rollouts, BEAMWIDTH);
			}
		}
		// std::cout<<max_sims<<std::endl;

		if (currentState->cumulative_reward >= bestResult)
		{
			bestResult = currentState->cumulative_reward;
			bestSolution = solution;
			bestCard = Card(bestSolution[0]->card);
		}

		solution.clear();
	}

	for (Node* n : delete_tracker)
		delete n;
	delete_tracker.clear();
	bestSolution.clear();

	delete tree;

	if (playerId == 1)
		bestCard = startState.player1Info.getCardFromHand(bestCard.cardName, bestCard.locationId);
	else if (playerId == 2)
		bestCard = startState.player2Info.getCardFromHand(bestCard.cardName, bestCard.locationId);

	return bestCard;
}