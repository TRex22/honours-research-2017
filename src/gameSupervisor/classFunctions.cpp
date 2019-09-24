std::vector<Card> CopyDeck(std::vector<Card> deck);
void printDeckStats(std::vector<Card> deck, std::string name);
void print_deck(std::vector<Card> vect, std::string name);
void print_board(std::vector<std::vector<Card>> vect);
bool check_deck_equals(std::vector<std::vector<Card>> deck1, std::vector<std::vector<Card>> deck2);
std::string get_hashable_card_name(Card card, int playerId);
bool check_if_in_deck(std::vector<Card> deck, Card card);

std::vector<Card> CopyDeck(std::vector<Card> deck)
{
	//for optimization
	std::vector<Card> copy_deck(deck);
	return copy_deck;	
}

void printDeckStats(std::vector<Card> deck, std::string name)
{
	int deckSize = deck.size();
	int unit = 0, horn = 0, scorch = 0, frost = 0, fog = 0, rain = 0, clear = 0, medic = 0, decoy = 0, spy = 0, hero = 0, bond = 0;
	int agile = 0, close = 0, ranged = 0, siege = 0;

	for (int i = 0; i < deckSize; i++)
	{
		if(deck[i].typeId == rules(unit))
			unit ++;
		if(deck[i].typeId == rules(horn))
			horn ++;
		if(deck[i].typeId == rules(scorch))
			scorch ++;
		if(deck[i].typeId == rules(frost))
			frost ++;
		if(deck[i].typeId == rules(fog))
			fog ++;
		if(deck[i].typeId == rules(rain))
			rain ++;
		if(deck[i].typeId == rules(clear))
			clear ++;
		if(deck[i].typeId == rules(medic))
			medic ++;
		if(deck[i].typeId == rules(decoy))
			decoy ++;
		if(deck[i].typeId == rules(spy))
			spy ++;
		if(deck[i].typeId == rules(hero))
			hero ++;
		if(deck[i].typeId == rules(bond))
			bond ++;

		if(deck[i].locationId == location(agileId))
			agile ++;
		if(deck[i].locationId == location(closeId))
			close ++;
		if(deck[i].locationId == location(rangedId))
			ranged ++;
		if(deck[i].locationId == location(siegeId))
			siege ++;
	}

	printf("Deck: %s\n", name.c_str());
	printf("%d Cards.\n", deckSize);
	printf("location counts: %d agile, %d close, %d ranged, %d siege.\n", agile, close, ranged, siege);
	printf("rule counts: %d unit, %d horn, %d scorch, %d frost\n", unit, horn, scorch, frost);
	printf("%d fog, %d rain, %d clear, %d medic, %d decoy, %d spy, %d hero, %d bond.\n---\n\n", fog, rain, clear, medic, decoy, spy, hero, bond);
}

void print_deck(std::vector<Card> vect, std::string name)
{   
	printf("\n");
	std::cout << name << std::endl;
	printf(DOUBLE_LINE);
	for (unsigned int i = 0; i < vect.size(); i++)
	{
		vect[i].print();
	}
	printf("\n");
	printf(DOUBLE_LINE);
}

void print_board(std::vector<std::vector<Card>> vect)
{
	printf("Game Board:\n");
	printf(SINGLE_LINE);
	for (unsigned int i = 0; i < vect.size(); i++)
	{
		print_deck(vect[i], "");
		printf(SINGLE_LINE);
	}
	printf(DOUBLE_LINE);
}

bool check_deck_equals(std::vector<Card> deck1, std::vector<Card> deck2)
{
	//TODO maybe ensure if decks are randomized that they equal?
	if (deck1.size() != deck2.size())
		return false;

	for (unsigned int i = 0; i < deck1.size(); i++)
	{
		if ((deck1[i].cardId != deck2[i].cardId))
			return false;
	}

	return true;
}

bool check_board_equals(std::vector<std::vector<Card>> board1, std::vector<std::vector<Card>> board2)
{
	//TODO maybe ensure if decks are randomized that they equal?
	if (board1.size() != board2.size())
		return false;

	bool check = true;
	for (unsigned int i = 0; i < board1.size(); i++)
	{
		check = check_deck_equals(board1[i], board2[i]);
	}

	return check;
}

std::vector<Card> RemoveDeckFromDeck(std::vector<Card> deck, std::vector<Card> rm_deck)
{
	for(unsigned int i = 0; i<rm_deck.size(); i++)
	{
		deck.erase(std::remove_if(deck.begin(), 
	        deck.end(), [&](Card x){return x.cardId == rm_deck[i].cardId;}),
				deck.end());
	}

	return deck;
}

std::string get_hashable_card_name(Card card, int playerId)
{
	// todo check
	std::string name;
	name.append(std::to_string(playerId));
	name.append(card.cardName);

	return name;
}

bool check_if_in_deck(std::vector<Card> deck, Card card)
{
	return std::any_of(deck.begin(), deck.end(), [&](Card x){return x.cardName == card.cardName;});
}

std::vector<Card> get_unique_decks(std::vector<Card> deck)
{
	std::sort(deck.begin(), deck.end(), [&](Card x, Card y){return ((x.cardName == y.cardName) || (x.cardId == y.cardId));});
	std::unique(deck.begin(), deck.end(), [&](Card x, Card y){return ((x.cardName == y.cardName) || (x.cardId == y.cardId));});
	return deck;
}

