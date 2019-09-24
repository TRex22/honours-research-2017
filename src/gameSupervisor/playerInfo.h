/*playerinfo class*/
/*This is info for a specific player
	Lists of cards

	CurrentDeck <- starts full
	DiscardPile <- starts empty

*/

class PlayerInfo
{
	private:
		int row = 7;
		int col = 15;

		std::vector<std::vector<Card>> currentBoard;
		std::vector<Card> faceDownPile;
		std::vector<Card> currentHand;
		std::vector<Card> discardPile;

		void addRoundPassCard();

	public:
		AttackPoints attackPoints;

		int handSize = 10;
		int roundWins = 0;
		bool medicSet = false;
		bool passedRound = false;

		~PlayerInfo();
		PlayerInfo();
		void copy(PlayerInfo pi);
		void resetRound();
		void initMedic();
		void resetMedic();
		int countHandSize();
		int countFaceDownSize();
		bool equals(PlayerInfo otherPlayerInfo);
		void print(int playerId);
		void assign(std::vector<Card> deck, int handSize);
		void addCardToHand(Card playCard); //they are different
		void addCardsToHandFromPile(Card playCard);
		void addWeatherCardToBoard(Card playCard);
		void addCardToBoard(Card playCard, int locationId);
		void removeCardFromHand(Card playCard);
		void removeCardFromDiscard(Card playCard);

		std::vector<std::vector<Card>> getCurrentBoard();
		std::vector<Card> getCurrentHand();
		std::vector<Card> getCurrentHand_ExcludingPass();
		void setCurrentHand(std::vector<Card> deck);
		std::vector<Card> getDiscardPile();
		std::vector<Card> getFaceDownPile();
		Card getCardFromHand(std::string cardName);
		Card getCardFromHand(int typeId);
		Card getCardFromHand(int typeId, int locationId);
		Card getCardFromHand(std::string cardName, int locationId);
		bool checkCardInHand(Card card);
		std::vector<Card> getBondCards(Card playCard, int locationId);
};

PlayerInfo::PlayerInfo() : currentBoard(3)
{
	this->currentBoard.reserve(3);
}

PlayerInfo::~PlayerInfo()
{
	faceDownPile.clear();
	currentHand.clear();
	discardPile.clear();
	currentBoard.clear();
	// std::cout<<"hello"<<std::endl;
}

void PlayerInfo::copy(PlayerInfo pi)
{
	this->currentBoard[0] = CopyDeck(pi.currentBoard[0]);
	this->currentBoard[1] = CopyDeck(pi.currentBoard[1]);
	this->currentBoard[2] = CopyDeck(pi.currentBoard[2]);

	this->faceDownPile = CopyDeck(pi.faceDownPile);
	this->currentHand = CopyDeck(pi.currentHand);
	this->discardPile = CopyDeck(pi.discardPile);

	this->attackPoints.copy(pi.attackPoints);
	this->handSize = pi.handSize;
	this->roundWins = pi.roundWins;
	this->medicSet = pi.medicSet;
	this->passedRound = pi.passedRound;

}

void PlayerInfo::resetRound()
{
	this->attackPoints.resetCurrentAttackPoints();
	this->passedRound = false;
	this->medicSet = false;

	for (int j = 0; j < 3; j++)
	{
		for (unsigned int i = 0; i < this->currentBoard[j].size(); i++)
		{
			this->discardPile.push_back(this->currentBoard[j][i]);
		}
		this->currentBoard[j].clear();
	}
	
	addRoundPassCard();	
}

void PlayerInfo::initMedic()
{
	if(discardPile.size() > 0)
	{
		this->medicSet = true;
		std::vector<Card> tmp = currentHand;
		currentHand = discardPile;
		discardPile = tmp;
	}
}

void PlayerInfo::resetMedic()
{
	if(this->medicSet)
	{
		this->medicSet = false;
		std::vector<Card> tmp = currentHand;
		currentHand = discardPile;
		discardPile = tmp;
	}
}

bool PlayerInfo::equals(PlayerInfo otherPlayerInfo)
{
	bool check = this->attackPoints.equals(otherPlayerInfo.attackPoints);
	check = check_board_equals(this->currentBoard, otherPlayerInfo.currentBoard);
	check = check_deck_equals(this->faceDownPile, otherPlayerInfo.faceDownPile);
	check = check_deck_equals(this->currentHand, otherPlayerInfo.currentHand);
	check = check_deck_equals(this->discardPile, otherPlayerInfo.discardPile);

	return check;
}

void PlayerInfo::print(int playerId)
{
	printf("Player Info (ID = %d):\n", playerId);
	printf("Round Wins: %d", this->roundWins);
	printf("Passed Current Round: ");
	printf(this->passedRound ? "true\n" : "false\n");
	this->attackPoints.print();
	printf(SINGLE_LINE);

	print_board(this->currentBoard);
	print_deck(this->currentHand, "Current Hand:\n");
	print_deck(this->discardPile, "Discard Pile:\n");
	printf(DOUBLE_LINE);
}

void PlayerInfo::addCardToHand(Card c)
{
	if(c.locationId != location(agileId))
	{
		this->currentHand.push_back(c);
	}
	else if(c.locationId == location(agileId))
	{
		// Hack to deal with this card
		c.checkAgile = true;
		c.locationId = location(closeId);
		this->currentHand.push_back(c);
		c.locationId = location(rangedId);
		this->currentHand.push_back(c);
		c.locationId = location(siegeId);
		this->currentHand.push_back(c);
		c.locationId = location(agileId);
	}
}

void PlayerInfo::addRoundPassCard()
{
	// Hack for pass round
	std::string cardName = "Pass Round";
	int cardId = this->currentHand.size()+1;
	int locationId = location(closeId);
	int typeId = rules(roundPass);
	int attackPoints = 0;
	int extraCards = 0;

	Card passCard(cardName, cardId, locationId, typeId, attackPoints, extraCards, ""); // todo: if things go bad fix here
	addCardToHand(passCard);
}

void PlayerInfo::assign(std::vector<Card> deck, int handSize)
{
	this->handSize = handSize;
	// auto rng = std::default_random_engine {}; //todo change to mersenne twister
	std::random_device d;
	std::mt19937 rng(d());
	std::shuffle(std::begin(deck), std::end(deck), rng);
	// std::random_shuffle (deck.begin(), deck.end());
	for (unsigned int i = 0; i < deck.size(); i++)
	{
		if ((int)i < this->handSize) //asign 10 cards to hand
		{
			addCardToHand(deck[i]);
		}
		else
		{
			this->faceDownPile.push_back(deck[i]);
		}
	}

	addRoundPassCard();
}

void PlayerInfo::addCardsToHandFromPile(Card playCard)
{
	for (int i = 0; i < playCard.extraCards; i++)
	{
		std::vector<Card>::iterator it = this->faceDownPile.begin();
		if(it != this->faceDownPile.end())
		{
			addCardToHand(*it);
			this->faceDownPile.erase(it);

			it++;
		}		
	}
}

int PlayerInfo::countHandSize()
{
	return this->currentHand.size();
}

int PlayerInfo::countFaceDownSize()
{
	return this->faceDownPile.size();
}

std::vector<std::vector<Card>> PlayerInfo::getCurrentBoard()
{
	return this->currentBoard;
}

std::vector<Card> PlayerInfo::getCurrentHand()
{
	if (this->currentHand.size() == 0)
	{
		addRoundPassCard();
	}

	return this->currentHand;
}

std::vector<Card> PlayerInfo::getCurrentHand_ExcludingPass()
{
	std::vector<Card> copyHand = CopyDeck(this->currentHand);
	copyHand.erase(std::remove_if(copyHand.begin(), 
	                        copyHand.end(),
	                        [&](Card x){return x.cardName == "Pass Round";}),
           					copyHand.end());
	return copyHand;
}

void PlayerInfo::setCurrentHand(std::vector<Card> deck)
{
	this->currentHand = CopyDeck(deck);
}

std::vector<Card> PlayerInfo::getDiscardPile()
{
	return this->discardPile;
}

std::vector<Card> PlayerInfo::getFaceDownPile()
{
	return this->faceDownPile;
}

void PlayerInfo::addWeatherCardToBoard(Card playCard)
{
	//this only updates the ap modifiers
	if(playCard.typeId == rules(frost))
	{
		this->attackPoints.closeWeatherOn = true;
		this->attackPoints.closeCardCount = this->currentBoard[0].size();
	}
	else if(playCard.typeId == rules(fog))
	{
		this->attackPoints.rangedWeatherOn = true;
		this->attackPoints.rangedCardCount = this->currentBoard[1].size();	
	}
	else if(playCard.typeId == rules(rain))
	{
		this->attackPoints.siegeWeatherOn = true;
		this->attackPoints.siegeCardCount = this->currentBoard[2].size();
	}
	else if(playCard.typeId == rules(clear))
	{
		this->attackPoints.clearAttackPointsModifiers();
	}
}

void PlayerInfo::addCardToBoard(Card playCard, int locationId)
{
	//TODO hack for extra cards and weather ... into weather deck!!!
	std::vector<Card> bondList;
	// std::cerr<<"sdgg: "<<playCard.cardName<<std::endl;
	int attackPoints = playCard.attackPoints;

	if (playCard.typeId == rules(bond))
	{
		bondList = getBondCards(playCard, locationId);
		
		if(bondList.size() > 0)
		{
			attackPoints = (bondList.size()+1)*attackPoints;

			for (unsigned int i = 0; i < bondList.size(); i++)
			{
				attackPoints += bondList[i].attackPoints;
			}
		}
	}

	if (playCard.typeId == rules(horn))
	{
		this->currentBoard[locationId].push_back(playCard);
		if(locationId == location(closeId))
		{
			this->attackPoints.closeHorn = true;
		}
		else if(locationId == location(rangedId))
		{
			this->attackPoints.rangedHorn = true;
		}
		else if(locationId == location(siegeId))
		{
			this->attackPoints.siegeHorn = true;
		}  
	}
	else if (playCard.typeId != rules(roundPass))
	{
		this->currentBoard[locationId].push_back(playCard);
		if(locationId == location(closeId))
		{
			this->attackPoints.closeCardCount++; 
			this->attackPoints.closeAttackPoints += attackPoints;
		}
		else if(locationId == location(rangedId))
		{
			this->attackPoints.rangedCardCount++;
			this->attackPoints.rangedAttackPoints += attackPoints;
		}
		else if(locationId == location(siegeId))
		{
			this->attackPoints.siegeCardCount++;
			this->attackPoints.siegeAttackPoints += attackPoints;
		}
	}  
}

void PlayerInfo::removeCardFromHand(Card playCard)
{
	// Using lambda to fix am issue where an element is skipped when there are multiple elements
	this->currentHand.erase(std::remove_if(this->currentHand.begin(), 
	                        this->currentHand.end(),
	                        [&](Card x){return x.cardId == playCard.cardId;}),
           					this->currentHand.end());
}

void PlayerInfo::removeCardFromDiscard(Card playCard)
{
	this->discardPile.erase(std::remove_if(this->discardPile.begin(), 
	                        this->discardPile.end(),
	                        [&](Card x){return x.cardId == playCard.cardId;}),
           					this->discardPile.end());
}

Card PlayerInfo::getCardFromHand(std::string cardName)
{
	std::vector<Card>::iterator it = this->currentHand.begin();
	while(it != this->currentHand.end())
	{
		if(it->cardName == cardName)
			return (*it);
		it++;
	}

	//will return end
	return Card();
}

Card PlayerInfo::getCardFromHand(int typeId)
{
	std::vector<Card>::iterator it = this->currentHand.begin();
	while(it != this->currentHand.end())
	{
		if(it->typeId == typeId)
			return (*it);
		it++;
	}

	//will return end
	return Card();
}

Card PlayerInfo::getCardFromHand(std::string cardName, int locationId)
{
	std::vector<Card>::iterator it = this->currentHand.begin();
	while(it != this->currentHand.end())
	{
		if(it->cardName == cardName && it->locationId == locationId)
			return (*it);
		it++;
	}

	//will return end
	return Card();
}

Card PlayerInfo::getCardFromHand(int typeId, int locationId)
{
	std::vector<Card>::iterator it = this->currentHand.begin();
	while(it != this->currentHand.end())
	{
		if(it->typeId == typeId && it->locationId == locationId)
			return (*it);
		it++;
	}

	//will return end
	return Card();
}

bool PlayerInfo::checkCardInHand(Card card)
{
	if (card.typeId == rules(roundPass))
		return true;

	std::vector<Card>::iterator it = this->currentHand.begin();
	while(it != this->currentHand.end())
	{
		if(it->cardName == card.cardName && it->cardId == card.cardId)
			return true;
		it++;
	}

	//will return end
	return false;
}

std::vector<Card> PlayerInfo::getBondCards(Card playCard, int locationId)
{
	std::vector<Card> vector;
	std::vector<Card>::iterator it = this->currentBoard[locationId].begin();
	while(it != this->currentBoard[locationId].end())
	{
		if(it->typeId == playCard.typeId && it->locationId == playCard.locationId && it->cardName == playCard.cardName)
			vector.push_back(*it);
		it++;
	}

	return vector;
}
