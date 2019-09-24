TEST(RulesTest, UnitCloseRule) { 
    // TODO: fix rnd cards find right card
    GameState gs = SetupCleanGS();

    // Play rule
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
}

TEST(RulesTest, UnitRangedRule) { 
    // TODO: fix rnd cards find right card
    GameState gs = SetupCleanGS();

    // Play rule
    // player 1
    gs.currentPlayerId = 1;
    int P1_HAND_SIZE = gs.player1Info.countHandSize();
    Card p1Card = gs.player1Info.getCardFromHand(rules(unit), location(rangedId));
    ASSERT_EQ(gs.player1Info.countHandSize(), P1_HAND_SIZE);
    ASSERT_EQ(gs.player1Info.attackPoints.rangedAttackPoints, 0);
    ASSERT_EQ(gs.player1Info.attackPoints.getCurrentAttackPoints(), 0);
    gs = calculateCardPlay(gs, p1Card);
    ASSERT_EQ(gs.player1Info.attackPoints.rangedAttackPoints, p1Card.attackPoints);
    ASSERT_EQ(gs.player1Info.attackPoints.getCurrentAttackPoints(), p1Card.attackPoints);
    ASSERT_EQ(gs.player1Info.countHandSize(), P1_HAND_SIZE - 1);

    // player 2
    gs.currentPlayerId = 2;
    int P2_HAND_SIZE = gs.player2Info.countHandSize();
    Card p2Card = gs.player2Info.getCardFromHand(rules(unit), location(rangedId));
    ASSERT_EQ(gs.player2Info.countHandSize(), P2_HAND_SIZE);
    ASSERT_EQ(gs.player2Info.attackPoints.rangedAttackPoints, 0);
    ASSERT_EQ(gs.player2Info.attackPoints.getCurrentAttackPoints(), 0);
    gs = calculateCardPlay(gs, p2Card);
    ASSERT_EQ(gs.player2Info.attackPoints.rangedAttackPoints, p2Card.attackPoints);
    ASSERT_EQ(gs.player2Info.attackPoints.getCurrentAttackPoints(), p2Card.attackPoints);
    ASSERT_EQ(gs.player2Info.countHandSize(), P2_HAND_SIZE - 1);
    ASSERT_EQ(gs.player1Info.countHandSize(), P1_HAND_SIZE - 1); //make sure bad things are not happening (only needed in one place)
}

TEST(RulesTest, UnitSiegeRule) { 
    // TODO: fix rnd cards find right card
    GameState gs = SetupCleanGS();

    // Play rule
    // player 1
    gs.currentPlayerId = 1;
    int P1_HAND_SIZE = gs.player1Info.countHandSize();
    Card p1Card = gs.player1Info.getCardFromHand(rules(unit), location(siegeId));
    ASSERT_EQ(gs.player1Info.countHandSize(), P1_HAND_SIZE);
    ASSERT_EQ(gs.player1Info.attackPoints.siegeAttackPoints, 0);
    ASSERT_EQ(gs.player1Info.attackPoints.getCurrentAttackPoints(), 0);
    gs = calculateCardPlay(gs, p1Card);
    ASSERT_EQ(gs.player1Info.attackPoints.siegeAttackPoints, p1Card.attackPoints);
    ASSERT_EQ(gs.player1Info.attackPoints.getCurrentAttackPoints(), p1Card.attackPoints);
    ASSERT_EQ(gs.player1Info.countHandSize(), P1_HAND_SIZE - 1);

    // player 2
    gs.currentPlayerId = 2;
    int P2_HAND_SIZE = gs.player2Info.countHandSize();
    Card p2Card = gs.player2Info.getCardFromHand(rules(unit), location(siegeId));
    ASSERT_EQ(gs.player2Info.countHandSize(), P2_HAND_SIZE);
    ASSERT_EQ(gs.player2Info.attackPoints.siegeAttackPoints, 0);
    ASSERT_EQ(gs.player2Info.attackPoints.getCurrentAttackPoints(), 0);
    gs = calculateCardPlay(gs, p2Card);
    ASSERT_EQ(gs.player2Info.attackPoints.siegeAttackPoints, p2Card.attackPoints);
    ASSERT_EQ(gs.player2Info.attackPoints.getCurrentAttackPoints(), p2Card.attackPoints);
    ASSERT_EQ(gs.player2Info.countHandSize(), P2_HAND_SIZE - 1);
    ASSERT_EQ(gs.player1Info.countHandSize(), P1_HAND_SIZE - 1); //make sure bad things are not happening (only needed in one place)
}

TEST(RulesTest, HeroCloseRule) { 
    // TODO: fix rnd cards find right card
    GameState gs = SetupCleanGS();

    // Play rule
    // player 1
    gs.currentPlayerId = 1;
    int P1_HAND_SIZE = gs.player1Info.countHandSize();
    Card p1Card = gs.player1Info.getCardFromHand(rules(hero), location(closeId));
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
    Card p2Card = gs.player2Info.getCardFromHand(rules(hero), location(closeId));
    ASSERT_EQ(gs.player2Info.countHandSize(), P2_HAND_SIZE);
    ASSERT_EQ(gs.player2Info.attackPoints.closeAttackPoints, 0);
    ASSERT_EQ(gs.player2Info.attackPoints.getCurrentAttackPoints(), 0);
    gs = calculateCardPlay(gs, p2Card);
    ASSERT_EQ(gs.player2Info.attackPoints.closeAttackPoints, p2Card.attackPoints);
    ASSERT_EQ(gs.player2Info.attackPoints.getCurrentAttackPoints(), p2Card.attackPoints);
    ASSERT_EQ(gs.player2Info.countHandSize(), P2_HAND_SIZE - 1);
    ASSERT_EQ(gs.player1Info.countHandSize(), P1_HAND_SIZE - 1); //make sure bad things are not happening (only needed in one place)
}

// TEST(RulesTest, HeroRangedRule) { 
//     // TODO: fix rnd cards find right card
//     GameState gs = SetupCleanGS();

//     // Play rule
//     // player 1
//     gs.currentPlayerId = 1;
//     int P1_HAND_SIZE = gs.player1Info.countHandSize();
//     Card p1Card = gs.player1Info.getCardFromHand(rules(hero), location(rangedId));
//     // std::cerr<<":(.: "<<p1Card.cardName<<std::endl;
//     ASSERT_EQ(gs.player1Info.countHandSize(), P1_HAND_SIZE);
//     ASSERT_EQ(gs.player1Info.attackPoints.rangedAttackPoints, 0);
//     ASSERT_EQ(gs.player1Info.attackPoints.getCurrentAttackPoints(), 0);
//     gs = calculateCardPlay(gs, p1Card);
//     ASSERT_EQ(gs.player1Info.attackPoints.rangedAttackPoints, p1Card.attackPoints);
//     ASSERT_EQ(gs.player1Info.attackPoints.getCurrentAttackPoints(), p1Card.attackPoints);
//     ASSERT_EQ(gs.player1Info.countHandSize(), P1_HAND_SIZE - 1);

//     // player 2
//     gs.currentPlayerId = 2;
//     int P2_HAND_SIZE = gs.player2Info.countHandSize();
//     Card p2Card = gs.player2Info.getCardFromHand(rules(hero), location(rangedId));
//     ASSERT_EQ(gs.player2Info.countHandSize(), P2_HAND_SIZE);
//     ASSERT_EQ(gs.player2Info.attackPoints.rangedAttackPoints, 0);
//     ASSERT_EQ(gs.player2Info.attackPoints.getCurrentAttackPoints(), 0);
//     gs = calculateCardPlay(gs, p2Card);
//     ASSERT_EQ(gs.player2Info.attackPoints.rangedAttackPoints, p2Card.attackPoints);
//     ASSERT_EQ(gs.player2Info.attackPoints.getCurrentAttackPoints(), p2Card.attackPoints);
//     ASSERT_EQ(gs.player2Info.countHandSize(), P2_HAND_SIZE - 1);
//     ASSERT_EQ(gs.player1Info.countHandSize(), P1_HAND_SIZE - 1); //make sure bad things are not happening (only needed in one place)
// }

TEST(RulesTest, HeroSiegeRule) { 
    // TODO: fix rnd cards find right card
    GameState gs = SetupCleanGS();

    // Play rule
    // no player 1 (northern) card ...
    gs.currentPlayerId = 1;
    int P1_HAND_SIZE = gs.player1Info.countHandSize();
    Card p1Card = gs.player1Info.getCardFromHand(rules(hero), location(siegeId));
    // std::cerr<<"JKJHJKHJHKJHKJ: "<<p1Card.cardName<<std::endl;
    ASSERT_EQ(gs.player1Info.countHandSize(), P1_HAND_SIZE);
    ASSERT_EQ(gs.player1Info.attackPoints.siegeAttackPoints, 0);
    ASSERT_EQ(gs.player1Info.attackPoints.getCurrentAttackPoints(), 0);
    gs = calculateCardPlay(gs, p1Card);
    ASSERT_EQ(gs.player1Info.attackPoints.siegeAttackPoints, p1Card.attackPoints);
    ASSERT_EQ(gs.player1Info.attackPoints.getCurrentAttackPoints(), p1Card.attackPoints);
    ASSERT_EQ(gs.player1Info.countHandSize(), P1_HAND_SIZE - 1);
    ASSERT_EQ(gs.player1Info.countHandSize(), P1_HAND_SIZE - 1); //make sure bad things are not happening (only needed in one place)

    // player 2
    gs.currentPlayerId = 2;
    int P2_HAND_SIZE = gs.player2Info.countHandSize();
    Card p2Card = gs.player2Info.getCardFromHand(rules(hero), location(siegeId));
    ASSERT_EQ(gs.player2Info.countHandSize(), P2_HAND_SIZE);
    ASSERT_EQ(gs.player2Info.attackPoints.siegeAttackPoints, 0);
    ASSERT_EQ(gs.player2Info.attackPoints.getCurrentAttackPoints(), 0);
    gs = calculateCardPlay(gs, p2Card);
    ASSERT_EQ(gs.player2Info.attackPoints.siegeAttackPoints, p2Card.attackPoints);
    ASSERT_EQ(gs.player2Info.attackPoints.getCurrentAttackPoints(), p2Card.attackPoints);
    ASSERT_EQ(gs.player2Info.countHandSize(), P2_HAND_SIZE - 1);
    // ASSERT_EQ(gs.player1Info.countHandSize(), P1_HAND_SIZE - 1); //make sure bad things are not happening (only needed in one place)
}