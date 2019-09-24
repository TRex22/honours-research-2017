#include "../include/rapidjson/document.h"
#include "../include/rapidjson/filereadstream.h"
#include "../src/gameSupervisor/gameSupervisor.h"
#include "../src/helpers/helper.h"
#include <gtest/gtest.h>

std::string northernpath = "../tests/northernDeck.json";
std::string nilfgaardpath = "../tests/nilfgaardDeck.json";

std::vector<Card> addBondCards(std::vector<Card> deck)
{
    Card* bondCard = new Card("Blue Stripes Commando", 990, location(closeId), rules(bond), 4, 0, "");
    deck.push_back(*bondCard);
    bondCard = new Card("Blue Stripes Commando", 991, location(closeId), rules(bond), 4, 0, "");
    deck.push_back(*bondCard);
    bondCard = new Card("Blue Stripes Commando", 992, location(closeId), rules(bond), 4, 0, "");
    deck.push_back(*bondCard);

    bondCard = new Card("Blue Stripes Commando", 993, location(rangedId), rules(bond), 4, 0, "");
    deck.push_back(*bondCard);
    bondCard = new Card("Blue Stripes Commando", 994, location(rangedId), rules(bond), 4, 0, "");
    deck.push_back(*bondCard);
    bondCard = new Card("Blue Stripes Commando", 995, location(rangedId), rules(bond), 4, 0, "");
    deck.push_back(*bondCard);

    bondCard = new Card("Blue Stripes Commando", 996, location(siegeId), rules(bond), 4, 0, "");
    deck.push_back(*bondCard);
    bondCard = new Card("Blue Stripes Commando", 997, location(siegeId), rules(bond), 4, 0, "");
    deck.push_back(*bondCard);
    bondCard = new Card("Blue Stripes Commando", 998, location(siegeId), rules(bond), 4, 0, "");
    deck.push_back(*bondCard);

    return deck;
}

GameState SetupCleanGS()
{
    std::vector<Card> northernDeck = convertJsonToVectorDeck(northernpath);
    std::vector<Card> nilfgaardDeck = convertJsonToVectorDeck(nilfgaardpath);

    GameState gameState;
    int handSize = northernDeck.size() - 5;
    gameState.assignDeck(northernDeck, 1, false, handSize);
    gameState.assignDeck(nilfgaardDeck, 2, false, handSize);

    gameState.player1Info.setCurrentHand(addBondCards(gameState.player1Info.getCurrentHand()));
    gameState.player2Info.setCurrentHand(addBondCards(gameState.player2Info.getCurrentHand()));

    return gameState;
}


// Tests
#include "unit_rule.cpp"
#include "bond_rule.cpp"
#include "spy_rule.cpp"
#include "horn_rule.cpp"
#include "weather_rule.cpp"
#include "medic_rule.cpp"

/*Multiple card plays
Detect win
Detect game end
round pass
clean player hidden info enforecement
clear round
hero rnaged fix
spy count agile cards fix
horn initial duplication
horn with ap
add agile cards to hand
facedownpile agile hack when put in hand ... check spy and medic
horn + weather
bond+weather
bond+horn
fix horn tests
hero and weather?
bond hron and weather
remove card test
reset functions

     roundPass = -1 ??,
     scorch = 2, 3 - types
     frost = 3,
     fog = 4,
     rain = 5,
     clear = 6,
     medic = 7,
     NO: decoy = 8
*/

TEST(BoardFunctionsTest, BoardEquals) { 
    std::vector<Card> northernDeck = convertJsonToVectorDeck(northernpath);
    std::vector<Card> nilfgaardDeck = convertJsonToVectorDeck(nilfgaardpath);

    GameState gs;
    gs.assignDeck(northernDeck, 1, 10);
    gs.assignDeck(nilfgaardDeck, 2, 10);

    ASSERT_EQ(check_deck_equals(northernDeck, northernDeck), true);
    ASSERT_EQ(check_deck_equals(northernDeck, nilfgaardDeck), false);

    ASSERT_EQ(check_deck_equals(gs.player1Info.getCurrentHand(), gs.player1Info.getCurrentHand()), true);
    ASSERT_EQ(check_deck_equals(gs.player2Info.getCurrentHand(), gs.player1Info.getCurrentHand()), false);
}

TEST(AttackPointsTest, AttackPointsEquals) { 
    AttackPoints attackPoints;
    AttackPoints attackPoints2;

    ASSERT_EQ(attackPoints.equals(attackPoints2), true);

    attackPoints.closeAttackPoints = 5;
    attackPoints.rangedAttackPoints = 5;
    attackPoints.siegeAttackPoints = 5;

    attackPoints.closeCardCount = 1;
    attackPoints.rangedCardCount = 1;
    attackPoints.siegeCardCount = 1;
    
    attackPoints.closeWeatherOn = true;
    attackPoints.rangedWeatherOn = true;
    attackPoints.siegeWeatherOn = true;

    ASSERT_EQ(attackPoints.getBaseAttackPoints(), 15);
    ASSERT_EQ(attackPoints.getCurrentAttackPoints(), 3);

    attackPoints.closeHorn = true;
    attackPoints.rangedHorn = true;
    attackPoints.siegeHorn = true;

    ASSERT_EQ(attackPoints.getCurrentAttackPoints(), 6);

    attackPoints.resetCurrentAttackPoints();
    ASSERT_EQ(attackPoints.getCurrentAttackPoints(), attackPoints.getBaseAttackPoints());
    ASSERT_EQ(attackPoints.getCurrentAttackPoints(), 0);

    attackPoints2.siegeWeatherOn = true;
    ASSERT_EQ(attackPoints.equals(attackPoints2), false);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}