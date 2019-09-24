#define results_optimal_header "AI ID,Total Games,Optimal Choice Count"
#define results_optimal_loctaion "./results/results_optimal.csv"

void run_optimal_choice_test(int max, int total_count)
{	
	double multiplier = max/total_count;
	std::cout<<multiplier<<std::endl;
	std::vector<Card> p1Deck();
	std::vector<Card> p2Deck();
	
	Card c1("1 pass", 1, location(agileId), rules(roundPass), 0, 0, "imgURI");
	Card c2("2 spy 1", 2, location(closeId), rules(spy), 1, 3, "imgURI");
	Card c3("3 close soldier 200", 3, location(closeId), rules(unit), 200, 3, "imgURI");
	Card c4("4 ranged soldat 200", 4, location(rangedId), rules(unit), 200, 3, "imgURI");
	Card c5("5 siege soldat 200", 5, location(siegeId), rules(unit), 200, 3, "imgURI");
	Card c6("6 frost", 6, location(siegeId), rules(frost), 0, 0, "imgURI");
	// Card c7("7", 7, location(), rules(), int attackPoints, int extraCards, "imgURI");
	// Card c8("8", 8, location(), rules(), int attackPoints, int extraCards, "imgURI");
	// Card c9("9", 9, location(), rules(), int attackPoints, int extraCards, "imgURI");
	// Card c10("10", 10, location(), rules(), int attackPoints, int extraCards, "imgURI");

	// std::vector<Card> northernDeck = convertJsonToVectorDeck(northernpath);
 //    std::vector<Card> nilfgaardDeck = convertJsonToVectorDeck(nilfgaardpath);
	


	// GameState gameState(p1Deck, p2Deck);
 //    gameState.assignDeck(p1Deck, 1, 10);
 //    gameState.assignDeck(p2Deck, 2, 10);

 //    print_deck(gameState.player1Info.getCurrentHand(), "player1");
 //    print_deck(gameState.player2Info.getCurrentHand(), "player2");


}