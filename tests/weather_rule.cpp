TEST(RulesTest, FrostRule) 
{ 
    // TODO: fix rnd cards find right card
    GameState gs = SetupCleanGS();

    // player 1
    gs.currentPlayerId = 1;
    int P1_HAND_SIZE = gs.player1Info.countHandSize();
    Card p1Card = gs.player1Info.getCardFromHand(rules(unit), location(closeId));
    ASSERT_EQ(gs.player1Info.countHandSize(), P1_HAND_SIZE);
    ASSERT_EQ(gs.player1Info.attackPoints.closeAttackPoints, 0);
    ASSERT_EQ(gs.player1Info.attackPoints.getCurrentAttackPoints(), 0);
    gs = calculateCardPlay(gs, p1Card);
    ASSERT_EQ(gs.player1Info.attackPoints.closeAttackPoints, p1Card.attackPoints);
    ASSERT_EQ(gs.player1Info.attackPoints.getCurrentAttackPoints(), p1Card.attackPoints);
    ASSERT_EQ(gs.player1Info.countHandSize(), P1_HAND_SIZE - 1);

    // player 2
    gs.currentPlayerId = 2;
    int P2_HAND_SIZE = gs.player2Info.countHandSize();
    Card p2Card = gs.player2Info.getCardFromHand(rules(unit), location(closeId));
    ASSERT_EQ(gs.player2Info.countHandSize(), P2_HAND_SIZE);
    ASSERT_EQ(gs.player2Info.attackPoints.closeAttackPoints, 0);
    ASSERT_EQ(gs.player2Info.attackPoints.getCurrentAttackPoints(), 0);
    gs = calculateCardPlay(gs, p2Card);
    ASSERT_EQ(gs.player2Info.attackPoints.closeAttackPoints, p2Card.attackPoints);
    ASSERT_EQ(gs.player2Info.attackPoints.getCurrentAttackPoints(), p2Card.attackPoints);
    ASSERT_EQ(gs.player2Info.countHandSize(), P2_HAND_SIZE - 1);
    ASSERT_EQ(gs.player1Info.countHandSize(), P1_HAND_SIZE - 1); //make sure bad things are not happening (only needed in one place)

    // Play Rule
    Card frostCard = gs.player1Info.getCardFromHand(rules(frost));
    ASSERT_EQ(gs.player1Info.attackPoints.closeWeatherOn, false);
    gs = calculateCardPlay(gs, frostCard);
    ASSERT_EQ(gs.player1Info.attackPoints.closeWeatherOn, true);
    ASSERT_EQ(gs.player1Info.attackPoints.closeAttackPoints, p1Card.attackPoints);
    ASSERT_EQ(gs.player1Info.attackPoints.getCurrentAttackPoints(), 1);
    ASSERT_EQ(gs.player1Info.countHandSize(), P1_HAND_SIZE - 1);

    Card frostCard2 = gs.player2Info.getCardFromHand(rules(frost));
    P2_HAND_SIZE = gs.player2Info.countHandSize();
    // ASSERT_EQ(gs.player2Info.attackPoints.closeWeatherOn, false); //TODO fix this
    gs = calculateCardPlay(gs, frostCard2);
    ASSERT_EQ(gs.player2Info.attackPoints.closeWeatherOn, true);
    ASSERT_EQ(gs.player2Info.attackPoints.closeAttackPoints, p2Card.attackPoints);
    ASSERT_EQ(gs.player2Info.attackPoints.getCurrentAttackPoints(), 1);
    ASSERT_EQ(gs.player2Info.countHandSize(), P2_HAND_SIZE - 1);

    gs.currentPlayerId = 1;
    P1_HAND_SIZE = gs.player1Info.countHandSize();
    p1Card = gs.player1Info.getCardFromHand(rules(unit), location(closeId));
    ASSERT_EQ(gs.player1Info.countHandSize(), P1_HAND_SIZE);
    ASSERT_EQ(gs.player1Info.attackPoints.getCurrentAttackPoints(), 1);
    gs = calculateCardPlay(gs, p1Card);
    ASSERT_EQ(gs.player1Info.attackPoints.getCurrentAttackPoints(), 2);  //fix weather cards if in play then add size()-1 to the modifier
    ASSERT_EQ(gs.player1Info.countHandSize(), P1_HAND_SIZE - 1);

    // player 2
    gs.currentPlayerId = 2;
    P2_HAND_SIZE = gs.player2Info.countHandSize();
    p2Card = gs.player2Info.getCardFromHand(rules(unit), location(closeId));
    ASSERT_EQ(gs.player2Info.countHandSize(), P2_HAND_SIZE);
    ASSERT_EQ(gs.player2Info.attackPoints.getCurrentAttackPoints(), 1);
    gs = calculateCardPlay(gs, p2Card);

    ASSERT_EQ(gs.player2Info.attackPoints.closeCardCount, 2);
    ASSERT_EQ(gs.player2Info.attackPoints.getCurrentAttackPoints(), 2);

    gs.player2Info.attackPoints.closeAttackPoints = 5;
    gs.player2Info.attackPoints.closeCardCount = 2;

    ASSERT_EQ(gs.player2Info.attackPoints.getCurrentAttackPoints(), 2);
}

// TEST(RulesTest, FogRule) 
// { 
//     GameState gs = SetupCleanGS();

//     // Play Rule
//     gs.currentPlayerId = 1;
//     Card fogCard = gs.player1Info.getCardFromHand(rules(fog));
//     // std::cerr<<"ggg: "<<fogCard.cardName<<std::endl;
//     int P1_HAND_SIZE = gs.player1Info.countHandSize();
//     ASSERT_EQ(gs.player1Info.attackPoints.rangedWeatherOn, false); 
//     gs = calculateCardPlay(gs, fogCard);
//     ASSERT_EQ(gs.player1Info.attackPoints.rangedWeatherOn, true);
//     ASSERT_EQ(gs.player1Info.attackPoints.rangedAttackPoints, 0);
//     ASSERT_EQ(gs.player1Info.attackPoints.getCurrentAttackPoints(), 1);
//     ASSERT_EQ(gs.player1Info.countHandSize(), P1_HAND_SIZE - 1);

//     gs.currentPlayerId = 2;
//     Card fogCard2 = gs.player2Info.getCardFromHand(rules(fog));
//     int P2_HAND_SIZE = gs.player2Info.countHandSize();
//     ASSERT_EQ(gs.player2Info.attackPoints.rangedWeatherOn, false);
//     gs = calculateCardPlay(gs, fogCard2);
//     ASSERT_EQ(gs.player2Info.attackPoints.rangedWeatherOn, true);
//     ASSERT_EQ(gs.player2Info.attackPoints.rangedAttackPoints, 0);
//     ASSERT_EQ(gs.player2Info.attackPoints.getCurrentAttackPoints(), 1);
//     ASSERT_EQ(gs.player2Info.countHandSize(), P2_HAND_SIZE - 1);

// }

// TEST(RulesTest, FogRule) 
// TEST(RulesTest, RainRule)
// TEST(RulesTest, ClearRule)