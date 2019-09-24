// 1 = 1ap
// 2 = 2(ap+ap)
// 3 = 3(ap+ap+ap)

// n*sum(ap) from 1 to n
// 1*1, 2*(1+1), 3*(1+1+1)
// 3*(8+8+8) = 72 for siege

// No round increment because testing card rules not round enforcement
TEST(RulesTest, BondCloseRule) { 
    // TODO: fix rnd cards find right card
    GameState gs = SetupCleanGS();

    // Play rule
    // player 1
    gs.currentPlayerId = 1;
    int P1_HAND_SIZE = gs.player1Info.countHandSize();
    Card p1Card = gs.player1Info.getCardFromHand(rules(bond), location(closeId));
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
    Card p2Card = gs.player2Info.getCardFromHand(rules(bond), location(closeId));
    ASSERT_EQ(gs.player2Info.countHandSize(), P2_HAND_SIZE);
    ASSERT_EQ(gs.player2Info.attackPoints.closeAttackPoints, 0);
    ASSERT_EQ(gs.player2Info.attackPoints.getCurrentAttackPoints(), 0);
    gs = calculateCardPlay(gs, p2Card);
    // std::cerr<<":(.: "<<p2Card.cardName<<std::endl;
    ASSERT_EQ(gs.player2Info.attackPoints.closeAttackPoints, p2Card.attackPoints);
    ASSERT_EQ(gs.player2Info.attackPoints.getCurrentAttackPoints(), p2Card.attackPoints);
    ASSERT_EQ(gs.player2Info.countHandSize(), P2_HAND_SIZE - 1);
    ASSERT_EQ(gs.player1Info.countHandSize(), P1_HAND_SIZE - 1); //make sure bad things are not happening (only needed in one place)

    //player 1
    gs.currentPlayerId = 1;
    P1_HAND_SIZE = gs.player1Info.countHandSize();
    Card p1Card2 = gs.player1Info.getCardFromHand(rules(bond), location(closeId));
    ASSERT_EQ(gs.player1Info.countHandSize(), P1_HAND_SIZE);
    ASSERT_EQ(gs.player1Info.attackPoints.closeAttackPoints, p1Card.attackPoints);
    ASSERT_EQ(gs.player1Info.attackPoints.getCurrentAttackPoints(), p1Card.attackPoints);
    gs = calculateCardPlay(gs, p1Card2);
    ASSERT_EQ(gs.player1Info.attackPoints.closeAttackPoints, 2*(p1Card.attackPoints + p1Card2.attackPoints));
    ASSERT_EQ(gs.player1Info.attackPoints.getCurrentAttackPoints(), 2*(p1Card.attackPoints + p1Card2.attackPoints));
    ASSERT_EQ(gs.player1Info.countHandSize(), P1_HAND_SIZE - 1);
    
    //player 1 again
    gs.currentPlayerId = 1;
    P1_HAND_SIZE = gs.player1Info.countHandSize();
    Card p1Card3 = gs.player1Info.getCardFromHand(rules(bond), location(closeId));
    ASSERT_EQ(gs.player1Info.countHandSize(), P1_HAND_SIZE);
    ASSERT_EQ(gs.player1Info.attackPoints.closeAttackPoints, 2*(p1Card.attackPoints + p1Card2.attackPoints));
    ASSERT_EQ(gs.player1Info.attackPoints.getCurrentAttackPoints(), 2*(p1Card.attackPoints + p1Card2.attackPoints));
    gs = calculateCardPlay(gs, p1Card3);
    ASSERT_EQ(gs.player1Info.attackPoints.closeAttackPoints, 3*(p1Card.attackPoints + p1Card2.attackPoints + p1Card3.attackPoints));
    ASSERT_EQ(gs.player1Info.attackPoints.getCurrentAttackPoints(), 3*(p1Card.attackPoints + p1Card2.attackPoints + p1Card3.attackPoints));
    ASSERT_EQ(gs.player1Info.countHandSize(), P1_HAND_SIZE - 1);
}

TEST(RulesTest, BondRangedRule) { 
    // TODO: fix rnd cards find right card
    GameState gs = SetupCleanGS();

    // Play rule
    // player 1
    gs.currentPlayerId = 1;
    int P1_HAND_SIZE = gs.player1Info.countHandSize();
    Card p1Card = gs.player1Info.getCardFromHand(rules(bond), location(rangedId));
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
    Card p2Card = gs.player2Info.getCardFromHand(rules(bond), location(rangedId));
    // std::cerr<<":(.: "<<p2Card.cardName<<std::endl;
    ASSERT_EQ(gs.player2Info.countHandSize(), P2_HAND_SIZE);
    ASSERT_EQ(gs.player2Info.attackPoints.rangedAttackPoints, 0);
    ASSERT_EQ(gs.player2Info.attackPoints.getCurrentAttackPoints(), 0);
    gs = calculateCardPlay(gs, p2Card);
    ASSERT_EQ(gs.player2Info.attackPoints.rangedAttackPoints, p2Card.attackPoints);
    ASSERT_EQ(gs.player2Info.attackPoints.getCurrentAttackPoints(), p2Card.attackPoints);
    ASSERT_EQ(gs.player1Info.countHandSize(), P1_HAND_SIZE - 1); //make sure bad things are not happening (only needed in one place)
    ASSERT_EQ(gs.player2Info.countHandSize(), P2_HAND_SIZE - 1);

    //player 1
    gs.currentPlayerId = 1;
    P1_HAND_SIZE = gs.player1Info.countHandSize();
    Card p1Card2 = gs.player1Info.getCardFromHand(rules(bond), location(rangedId));
    // std::cerr<<":(.: "<<p2Card.cardName<<std::endl;
    ASSERT_EQ(gs.player1Info.countHandSize(), P1_HAND_SIZE);
    ASSERT_EQ(gs.player1Info.attackPoints.rangedAttackPoints, p1Card.attackPoints);
    ASSERT_EQ(gs.player1Info.attackPoints.getCurrentAttackPoints(), p1Card.attackPoints);
    gs = calculateCardPlay(gs, p1Card2);
    ASSERT_EQ(gs.player1Info.attackPoints.rangedAttackPoints, 2*(p1Card.attackPoints + p1Card2.attackPoints));
    ASSERT_EQ(gs.player1Info.attackPoints.getCurrentAttackPoints(), 2*(p1Card.attackPoints + p1Card2.attackPoints));
    ASSERT_EQ(gs.player1Info.countHandSize(), P1_HAND_SIZE - 1);
}

TEST(RulesTest, BondSiegeRule) { 
    // TODO: fix rnd cards find right card
    GameState gs = SetupCleanGS();

    // Play rule
    // player 1
    gs.currentPlayerId = 1;
    int P1_HAND_SIZE = gs.player1Info.countHandSize();
    Card p1Card = gs.player1Info.getCardFromHand(rules(bond), location(siegeId));
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
    Card p2Card = gs.player2Info.getCardFromHand(rules(bond), location(siegeId));
    ASSERT_EQ(gs.player2Info.countHandSize(), P2_HAND_SIZE);
    ASSERT_EQ(gs.player2Info.attackPoints.siegeAttackPoints, 0);
    ASSERT_EQ(gs.player2Info.attackPoints.getCurrentAttackPoints(), 0);
    gs = calculateCardPlay(gs, p2Card);
    ASSERT_EQ(gs.player2Info.attackPoints.siegeAttackPoints, p2Card.attackPoints);
    ASSERT_EQ(gs.player2Info.attackPoints.getCurrentAttackPoints(), p2Card.attackPoints);
    ASSERT_EQ(gs.player2Info.countHandSize(), P2_HAND_SIZE - 1);
    ASSERT_EQ(gs.player1Info.countHandSize(), P1_HAND_SIZE - 1); //make sure bad things are not happening (only needed in one place)

    //player 1
    gs.currentPlayerId = 1;
    P1_HAND_SIZE = gs.player1Info.countHandSize();
    Card p1Card2 = gs.player1Info.getCardFromHand(rules(bond), location(siegeId));
    ASSERT_EQ(gs.player1Info.countHandSize(), P1_HAND_SIZE);
    ASSERT_EQ(gs.player1Info.attackPoints.siegeAttackPoints, p1Card.attackPoints);
    ASSERT_EQ(gs.player1Info.attackPoints.getCurrentAttackPoints(), p1Card.attackPoints);
    gs = calculateCardPlay(gs, p1Card2);
    ASSERT_EQ(gs.player1Info.attackPoints.siegeAttackPoints, 2*(p1Card.attackPoints + p1Card2.attackPoints));
    ASSERT_EQ(gs.player1Info.attackPoints.getCurrentAttackPoints(), 2*(p1Card.attackPoints + p1Card2.attackPoints));
    ASSERT_EQ(gs.player1Info.countHandSize(), P1_HAND_SIZE - 1);
}