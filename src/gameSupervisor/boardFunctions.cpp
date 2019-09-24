#include "../AI/randomPlayer.cpp"

int determine_winner(GameState gameState, bool print);
GameState determine_round_end_win(GameState gameState, bool print);
std::vector<Card> GetUnknownHand(GameState currentState, int playerId);
GameState playRndPlayer1(GameState gameState, bool print);
GameState playRndPlayer2(GameState gameState, bool print);
std::vector<Card> RemoveDeckFromDeck(std::vector<Card> deck, std::vector<Card> rm_deck);

int determine_winner(GameState gameState, bool print=true)
{
    gameState.isComplete = true;
    
	//determine winner
    if(gameState.player1Info.roundWins > gameState.player2Info.roundWins)
    {
        if(print)
            std::cout<<"Player 1 Wins!"<<std::endl;
        return 1;
    }
    else if (gameState.player2Info.roundWins > gameState.player1Info.roundWins)
    {
        if(print)
            std::cout<<"Player 2 Wins!"<<std::endl;
        return 2;
    }
    else // they equal
    {
        //no one wins
        if(print)
            std::cout<<"It's a draw!"<<std::endl;
        return 0;
    }

    return 0;
}

GameState determine_round_end_win(GameState gameState, bool print=true)
{
    int winner = -1;
    if(gameState.player1Info.passedRound && gameState.player2Info.passedRound)
    {
        //end round and find round winner
        if(gameState.player1Info.attackPoints.getCurrentAttackPoints() > gameState.player2Info.attackPoints.getCurrentAttackPoints())
        {
            gameState.player1Info.roundWins ++;
            if(print)
                std::cout<<"Player 1 Wins round "<<gameState.currentRound+1<<std::endl<<std::endl;
            winner = 1;
        }
        else if (gameState.player2Info.attackPoints.getCurrentAttackPoints() > gameState.player1Info.attackPoints.getCurrentAttackPoints())
        {
            gameState.player2Info.roundWins ++;
            if(print)
                std::cout<<"Player 2 Wins round "<<gameState.currentRound+1<<std::endl<<std::endl;
            winner = 2;
        }
        else // they equal
        {
            //no one wins
            if(print)
                std::cout<<"Round draw for round "<<gameState.currentRound+1<<std::endl<<std::endl;
            winner = 0;
        }
        
        gameState.roundReset();

        if(gameState.currentRound > 2 || gameState.player1Info.roundWins == 2 || gameState.player1Info.roundWins == 2) //>2 because it loops one more?
        {
            gameState.isComplete = true;
        }
    } 

    gameState.roundWinnerId = winner;
    return gameState;
}

std::vector<Card> GetUnknownHand(GameState currentState, int playerId)
{
    std::vector<Card> unknown;

    if(playerId == 1)
    {
        // Dont know player 1
        unknown = CopyDeck(currentState.northernDeck);
        unknown = RemoveDeckFromDeck(unknown, currentState.player1Info.getDiscardPile());
        
        unknown = RemoveDeckFromDeck(unknown, currentState.player1Info.getCurrentBoard()[0]);
        unknown = RemoveDeckFromDeck(unknown, currentState.player1Info.getCurrentBoard()[1]);
        unknown = RemoveDeckFromDeck(unknown, currentState.player1Info.getCurrentBoard()[2]);
    }
    else if(playerId == 2)
    {   
        unknown = CopyDeck(currentState.nilfgaardDeck);
        unknown = RemoveDeckFromDeck(unknown, currentState.player2Info.getDiscardPile());

        unknown = RemoveDeckFromDeck(unknown, currentState.player2Info.getCurrentBoard()[0]);
        unknown = RemoveDeckFromDeck(unknown, currentState.player2Info.getCurrentBoard()[1]);
        unknown = RemoveDeckFromDeck(unknown, currentState.player2Info.getCurrentBoard()[2]);
    }
    else
    {
        printf("Invalid player id. (in Board Functions Unknown Hand)\n");
    }

    unknown = RemoveDeckFromDeck(unknown, currentState.weatherCards);

    std::string cardName = "Pass Round";
    int cardId = unknown.size()+1;
    int locationId = location(closeId);
    int typeId = rules(roundPass);
    int attackPoints = 0;
    int extraCards = 0;

    Card passCard(cardName, cardId, locationId, typeId, attackPoints, extraCards, ""); // todo: if things go bad fix here

    unknown.push_back(passCard);

    return unknown;
}

GameState playRndPlayer1(GameState gameState, bool print=true)
{
    if(gameState.player1Info.passedRound == false)
    {
        do
        {
            gameState.currentPlayerId = 1;
            Card playCard1 = RandomPlayer(gameState, 1);
            gameState = calculateCardPlay(gameState, playCard1);
            if(print)
                std::cout<<"Player 1 Plays: "<<playCard1.cardName<<" ap: "<<playCard1.attackPoints<<std::endl;
            gameState.currentPlayerId = 1;
        }
        while (gameState.player1Info.medicSet);
    }   

    return gameState; 
}

GameState playRndPlayer2(GameState gameState, bool print=true)
{
    if(gameState.player2Info.passedRound == false)
    {
        do
        {
            gameState.currentPlayerId = 2;
            Card playCard2 = RandomPlayer(gameState, 2);
            gameState = calculateCardPlay(gameState, playCard2);
            if(print)
                std::cout<<"Player 2 Plays: "<<playCard2.cardName<<" ap: "<<playCard2.attackPoints<<std::endl;
            gameState.currentPlayerId = 2;
        }
        while (gameState.player2Info.medicSet);
    } 

    return gameState;
}