TEST(RulesTest, MedicRuleNoPile) 
{ 
    GameState gs = SetupCleanGS();

    // Play Rule
    gs.currentPlayerId = 1;
    Card medicCard = gs.player1Info.getCardFromHand(rules(medic));
    std::vector<Card> tmpHand = gs.player1Info.getCurrentHand();
    std::vector<Card> tmpPile = gs.player1Info.getDiscardPile();

    int P1_HAND_SIZE = gs.player1Info.countHandSize();
    gs = calculateCardPlay(gs, medicCard);
    ASSERT_EQ(gs.player1Info.attackPoints.getCurrentAttackPoints(), medicCard.attackPoints);
    ASSERT_EQ(gs.player1Info.countHandSize(), P1_HAND_SIZE - 1);

    gs.currentPlayerId = 2;
    Card medicCard2 = gs.player2Info.getCardFromHand(rules(medic));
    std::vector<Card> tmpHand2 = gs.player2Info.getCurrentHand();
    std::vector<Card> tmpPile2 = gs.player2Info.getDiscardPile();

    int P2_HAND_SIZE = gs.player2Info.countHandSize();
    gs = calculateCardPlay(gs, medicCard2);
    ASSERT_EQ(gs.player2Info.attackPoints.getCurrentAttackPoints(), medicCard2.attackPoints);
    ASSERT_EQ(gs.player2Info.countHandSize(), P2_HAND_SIZE - 1);
}

TEST(RulesTest, MedicRule) 
{ 
    GameState gs = SetupCleanGS();

    // Play Rule
    gs.currentPlayerId = 1;
    Card discard = gs.player1Info.getCardFromHand(rules(unit), location(closeId));
    gs = calculateCardPlay(gs, discard);
    gs.player1Info.resetRound();

    int P1_HAND_SIZE = gs.player1Info.countHandSize();

    Card medicCard = gs.player1Info.getCardFromHand(rules(medic));
    std::vector<Card> tmpHand = gs.player1Info.getCurrentHand();
    std::vector<Card> tmpPile = gs.player1Info.getDiscardPile();

    gs = calculateCardPlay(gs, medicCard);
    gs = calculateCardPlay(gs, gs.player1Info.getCurrentHand()[0]);
    gs.player1Info.resetMedic();

    ASSERT_EQ(gs.player1Info.attackPoints.getCurrentAttackPoints(), discard.attackPoints + medicCard.attackPoints);
    ASSERT_EQ(gs.player1Info.countHandSize(), P1_HAND_SIZE - 1);

    gs.currentPlayerId = 2;
    Card discard2 = gs.player2Info.getCardFromHand(rules(unit), location(closeId));
    gs = calculateCardPlay(gs, discard2);
    gs.player2Info.resetRound();
    
    int P2_HAND_SIZE = gs.player2Info.countHandSize();

    Card medicCard2 = gs.player2Info.getCardFromHand(rules(medic));
    std::vector<Card> tmpHand2 = gs.player2Info.getCurrentHand();
    std::vector<Card> tmpPile2 = gs.player2Info.getDiscardPile();

    gs = calculateCardPlay(gs, medicCard2);
    gs = calculateCardPlay(gs, gs.player2Info.getCurrentHand()[0]);
    gs.player2Info.resetMedic();

    ASSERT_EQ(gs.player2Info.attackPoints.getCurrentAttackPoints(), discard2.attackPoints + medicCard2.attackPoints);
    ASSERT_EQ(gs.player2Info.countHandSize(), P2_HAND_SIZE - 1);
}