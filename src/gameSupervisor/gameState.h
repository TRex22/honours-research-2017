/*Gamestate header*/
/*Will contain all elements which every ai can see*/
/*The current game state*/

/*the board is an array on ints*/
/*7rows x 15cols*/
/*0-2 is player 1 rows 1 - 3*/
/*3-5 is player 3 rows 1 - 3*/
/*6 is the weather row*/
/*each row can hold 15 cards*/

class GameState
{
private:
	// todo: implement parameters
	int row = 7;
	int col = 15;
	int stepCount = 0;

public:
	std::vector<Card> northernDeck;
	std::vector<Card> nilfgaardDeck;

	int currentPlayerId = -1;
	int currentRound = 0;
	int winPlayerId = -1;
	int roundWinnerId = -1;

	PlayerInfo player1Info;
	PlayerInfo player2Info;
	std::vector<Card> weatherCards;

	Card played_card;

	bool isComplete = false;

	/*Method headers*/
	GameState();
	~GameState();
	GameState(std::vector<Card> northernDeck, std::vector<Card> nilfgaardDeck);

	bool equals(GameState state);
	void copy(GameState state);
	void update(Card playCard, int player);
	void incrementIteration();
	void endgame();
	void checkRound();
	void roundReset();
	void nextRound();
	int getStepCount();
	void assignDeck(std::vector<Card> deck, int playerId, int handSize);
	void assignDeck(std::vector<Card> deck, int playerId, bool shuffle, int handSize);
	void print();
};

GameState::GameState()
{
}
GameState::~GameState()
{
	northernDeck.clear();
	nilfgaardDeck.clear();
	weatherCards.clear();
	// std::cout<<"hello"<<std::endl;
}

GameState::GameState(std::vector<Card> northernDeck, std::vector<Card> nilfgaardDeck)
{
	this->northernDeck = northernDeck;
	this->nilfgaardDeck = nilfgaardDeck;
	weatherCards.reserve(15);
}

bool GameState::equals(GameState state)
{
	// assume the same
	// std::vector<Card> northernDeck;
	// std::vector<Card> nilfgaardDeck;
	if (this->currentPlayerId != state.currentPlayerId)
		return false;

	if (this->currentRound != state.currentRound)
		return false;

	if (this->winPlayerId != state.winPlayerId)
		return false;

	if (this->roundWinnerId != state.roundWinnerId)
		return false;

	if (this->isComplete != state.isComplete)
		return false;

	if (!check_deck_equals(this->weatherCards, state.weatherCards))
		return false;

	if (!this->player1Info.equals(state.player1Info))
		return false;

	if (!this->player2Info.equals(state.player2Info))
		return false;

	return true;
}

void GameState::copy(GameState state)
{
	this->player1Info.copy(state.player1Info);
	this->player2Info.copy(state.player2Info);

	this->isComplete = state.isComplete;
	this->winPlayerId = state.winPlayerId;
	this->currentPlayerId = state.currentPlayerId;
	this->currentRound = state.currentRound;

	this->weatherCards = CopyDeck(state.weatherCards);
	this->northernDeck = CopyDeck(state.northernDeck);
	this->nilfgaardDeck = CopyDeck(state.nilfgaardDeck);
}

void GameState::incrementIteration()
{
	this->stepCount++;
}

void GameState::endgame()
{
	this->isComplete = true;
	if (this->player1Info.roundWins == 2 && this->player2Info.roundWins == 2)
	{
		// in case of a tie
		// count number of cards
		if (this->player1Info.countHandSize() > this->player2Info.countHandSize())
		{
			this->winPlayerId = 1;
		}
		else if (this->player2Info.countHandSize() > this->player1Info.countHandSize())
		{
			this->winPlayerId = 2;
		}
		else
		{
			// Everyone loses
			this->winPlayerId = -1;
		}
	}
	else if (this->player1Info.roundWins == 2)
	{
		this->winPlayerId = 1;
	}
	else if (this->player2Info.roundWins == 2)
	{
		this->winPlayerId = 2;
	}
}

void GameState::checkRound()
{
	// Calculate the final attack points for both players and compare them
	if (this->player1Info.attackPoints.getCurrentAttackPoints() > this->player2Info.attackPoints.getCurrentAttackPoints())
	{
		this->player1Info.roundWins++;
		if (this->player1Info.roundWins == 2)
		{
			this->winPlayerId = 1;
			this->isComplete = true;
		}
	}
	else if (this->player2Info.attackPoints.getCurrentAttackPoints() > this->player1Info.attackPoints.getCurrentAttackPoints())
	{
		this->player2Info.roundWins++;
		if (this->player2Info.roundWins == 2)
		{
			this->winPlayerId = 2;
			this->isComplete = true;
		}
	}
	else
	{
		this->player1Info.roundWins++;
		this->player2Info.roundWins++;
	}
	this->currentRound++;

	if (this->currentRound == 4) //i.e. round 3 is complete
	{
		this->winPlayerId = -1;
		this->isComplete = true;
	}
}

void GameState::roundReset()
{
	// Reset player Ap mods
	// Clear board
	// --> Discard
	this->player1Info.resetRound();
	this->player2Info.resetRound();
	this->currentPlayerId = 0;	//reset the player
	this->currentRound++;

	this->weatherCards.clear();
	this->weatherCards.resize(15);
}

void GameState::nextRound()
{
	checkRound();
	roundReset();
}

int GameState::getStepCount()
{
	return this->stepCount;
}

void GameState::assignDeck(std::vector<Card> deck, int playerId, int handSize = 10)
{
	// std::random_shuffle(deck.begin(), deck.end());
	std::random_device d;
	std::mt19937 rng(d());
	std::shuffle(std::begin(deck), std::end(deck), rng);

	if (playerId == 1)
	{
		this->player1Info.assign(deck, handSize);
	}
	else if (playerId == 2)
	{
		this->player2Info.assign(deck, handSize);
	}
}

void GameState::assignDeck(std::vector<Card> deck, int playerId, bool shuffle = true, int handSize = 10)
{
	if (shuffle)
	{
		std::random_device d;
		std::mt19937 rng(d());
		std::shuffle(std::begin(deck), std::end(deck), rng);
	}

	if (playerId == 1)
	{
		this->player1Info.assign(deck, handSize);
	}
	else if (playerId == 2)
	{
		this->player2Info.assign(deck, handSize);
	}
}

void GameState::print()
{
	printf("Game iteration (%d):\n", getStepCount());
	printf("Current Round: %d", this->currentRound);
	printf("Passed Current Round: ");
	printf(this->isComplete ? "true\n" : "false\n");

	std::ostringstream oss;
	oss << "Win Player ID: " << this->winPlayerId;

	if (this->isComplete)
		std::cout << oss.str();
	else
		std::cout << "";

	print_deck(this->weatherCards, "Weather Cards:\n");
	this->player1Info.print(1);
	this->player2Info.print(2);
}