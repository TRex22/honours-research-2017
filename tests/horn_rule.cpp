TEST(RulesTest, HornCloseRule) 
{ 
    // TODO: fix rnd cards find right card
    GameState gs = SetupCleanGS();

    // Play rule
    // player 1
    gs.currentPlayerId = 1;
    int P1_HAND_SIZE = gs.player1Info.countHandSize();
    Card p1Card = gs.player1Info.getCardFromHand("Commander's Horn", location(closeId));
    ASSERT_EQ(gs.player1Info.countHandSize(), P1_HAND_SIZE);
    ASSERT_EQ(gs.player1Info.attackPoints.closeAttackPoints, 0);
    ASSERT_EQ(gs.player1Info.attackPoints.getCurrentAttackPoints(), 0);
    gs = calculateCardPlay(gs, p1Card);
    ASSERT_EQ(gs.player1Info.attackPoints.closeAttackPoints, 0);
    ASSERT_EQ(gs.player1Info.attackPoints.getCurrentAttackPoints(), 0);
    ASSERT_EQ(gs.player1Info.countHandSize(), P1_HAND_SIZE - 3);

    gs.player1Info.attackPoints.closeAttackPoints = 5;
    gs.player1Info.attackPoints.rangedAttackPoints = 5;
    gs.player1Info.attackPoints.siegeAttackPoints = 5;

    gs.player1Info.attackPoints.closeCardCount = 1;
    gs.player1Info.attackPoints.rangedCardCount = 1;
    gs.player1Info.attackPoints.siegeCardCount = 1;

    ASSERT_EQ(gs.player1Info.attackPoints.closeAttackPoints, 5);
    ASSERT_EQ(gs.player1Info.attackPoints.getCurrentAttackPoints(), 20);

    // player 2
    gs.player2Info.attackPoints.closeAttackPoints = 5;
    gs.player2Info.attackPoints.rangedAttackPoints = 0;
    gs.player2Info.attackPoints.siegeAttackPoints = 0;

    gs.player2Info.attackPoints.closeCardCount = 1;
    gs.player2Info.attackPoints.rangedCardCount = 0;
    gs.player2Info.attackPoints.siegeCardCount = 0;

    gs.currentPlayerId = 2;
    int P2_HAND_SIZE = gs.player2Info.countHandSize();
    Card p2Card = gs.player2Info.getCardFromHand("Commander's Horn", location(closeId));
    ASSERT_EQ(gs.player2Info.countHandSize(), P2_HAND_SIZE);
    ASSERT_EQ(gs.player2Info.attackPoints.closeAttackPoints, 5);
    ASSERT_EQ(gs.player2Info.attackPoints.getCurrentAttackPoints(), 5);
    gs = calculateCardPlay(gs, p2Card);
    ASSERT_EQ(gs.player2Info.attackPoints.closeAttackPoints, 5); //should be 0
    ASSERT_EQ(gs.player2Info.attackPoints.getCurrentAttackPoints(), 10);
    ASSERT_EQ(gs.player2Info.countHandSize(), P2_HAND_SIZE - 3);
    ASSERT_EQ(gs.player1Info.countHandSize(), P1_HAND_SIZE - 3); //make sure bad things are not happening (only needed in one place
}