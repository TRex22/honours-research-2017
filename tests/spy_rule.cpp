TEST(RulesTest, SpyRule) { 
    // TODO: fix rnd cards find right card
    GameState gs = SetupCleanGS();

    // Play rule
    // player 1
    gs.currentPlayerId = 1;
    int P1_HAND_SIZE = gs.player1Info.countHandSize();
    int P1_face_down_size = gs.player1Info.countFaceDownSize();
    Card p1Card = gs.player1Info.getCardFromHand("Thaler");
    ASSERT_EQ(gs.player1Info.countHandSize(), P1_HAND_SIZE);
    // std::cerr<<"jhjkhjh: "<<P1_face_down_size<<std::endl;
    ASSERT_EQ(gs.player1Info.attackPoints.getCurrentAttackPoints(), 0);
    ASSERT_EQ(gs.player2Info.attackPoints.getCurrentAttackPoints(), 0);
    gs = calculateCardPlay(gs, p1Card);
    ASSERT_EQ(gs.player1Info.attackPoints.getCurrentAttackPoints(), 0);
    ASSERT_EQ(gs.player2Info.attackPoints.getCurrentAttackPoints(), p1Card.attackPoints);
    // issue may happen if not agile cards
    // std::cerr<<"jhjkhjh: "<<gs.player1Info.countFaceDownSize()<<std::endl;
    ASSERT_EQ(gs.player1Info.countHandSize(), P1_HAND_SIZE + 2);
    ASSERT_EQ(gs.player1Info.countFaceDownSize(), P1_face_down_size - 3);

    // player 2
    gs.currentPlayerId = 2;
    int P2_HAND_SIZE = gs.player2Info.countHandSize();
    int P2_face_down_size = gs.player2Info.countFaceDownSize();
    Card p2Card = gs.player2Info.getCardFromHand("Stefan Skellen");
    ASSERT_EQ(gs.player2Info.countHandSize(), P2_HAND_SIZE);
    ASSERT_EQ(gs.player2Info.attackPoints.getCurrentAttackPoints(), p1Card.attackPoints);
    gs = calculateCardPlay(gs, p2Card);
    ASSERT_EQ(gs.player1Info.attackPoints.getCurrentAttackPoints(), p2Card.attackPoints);
    ASSERT_EQ(gs.player2Info.attackPoints.getCurrentAttackPoints(), p1Card.attackPoints);
    // issue may happen if not agile cards
    ASSERT_EQ(gs.player2Info.countHandSize(), P2_HAND_SIZE + 2);
    ASSERT_EQ(gs.player1Info.countHandSize(), P1_HAND_SIZE + 2); 
    ASSERT_EQ(gs.player2Info.countFaceDownSize(), P2_face_down_size - 3);
}