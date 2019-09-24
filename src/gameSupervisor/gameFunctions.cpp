GameState playPlayer1(GameState gameState, bool print, int ai_id);
GameState playPlayer2(GameState gameState, bool print, int ai_id);

GameState playPlayer1(GameState gameState, bool print = true, int ai_id = ai(rnd_MT))
{
    if (gameState.player1Info.passedRound == false)
    {
        do
        {
            gameState.currentPlayerId = 1;
            Card playCard1 = gameState.player1Info.getCurrentHand()[0];

            if (gameState.player1Info.getCurrentHand().size() > 1)
            {
                if (ai_id == ai(rnd_MT))
                {
                    playCard1 = RandomPlayer(gameState, 1);
                }
                else if (ai_id == ai(rnd_fast))
                {
                    playCard1 = FastRandomPlayer(gameState, 1);
                }
                else if (ai_id == ai(rnd_xorshf))
                {
                    playCard1 = XorshfRandomPlayer(gameState, 1);
                }
                else if (ai_id == ai(heuristic))
                {
                    playCard1 = HeuristicPlayer(gameState, 1);
                }
                else if (ai_id == ai(rollout))
                {
                    playCard1 = rollout2(gameState, 1, Rollout_limit);
                }
                else if (ai_id == ai(mcts))
                {
                    playCard1 = MCTSPlayer(gameState, 1, MCTS_limit);
                }
                else if (ai_id == ai(mcts2))
                {
                    playCard1 = MCTSPlayer2(gameState, 1, MCTS_limit);
                }
                else if (ai_id == ai(bmcts))
                {
                    playCard1 = BMCTSPlayer(gameState, 1, MCTS_limit, SIM_LIMIT, BEAMWIDTH);
                }
                else if (ai_id == ai(human))
                {
                    std::cout << "Not Implemented" << std::endl;
                }
            }

            gameState = calculateCardPlay(gameState, playCard1);
            if (print)
                std::cout << "Player 1 Plays: " << playCard1.cardName << " ap: " << playCard1.attackPoints << std::endl;
            gameState.currentPlayerId = 1;
            gameState.played_card = playCard1;
        }
        while (gameState.player1Info.medicSet);
    }

    return gameState;
}

GameState playPlayer2(GameState gameState, bool print = true, int ai_id = ai(rnd_MT))
{
    if (gameState.player2Info.passedRound == false)
    {
        do
        {
            gameState.currentPlayerId = 2;
            Card playCard2 = gameState.player2Info.getCurrentHand()[0];

            if (gameState.player2Info.getCurrentHand().size() > 1)
            {
                if (ai_id == ai(rnd_MT))
                {
                    playCard2 = RandomPlayer(gameState, 2);
                }
                else if (ai_id == ai(rnd_fast))
                {
                    playCard2 = FastRandomPlayer(gameState, 2);
                }
                else if (ai_id == ai(rnd_xorshf))
                {
                    playCard2 = XorshfRandomPlayer(gameState, 2);
                }
                else if (ai_id == ai(heuristic))
                {
                    playCard2 = HeuristicPlayer(gameState, 2);
                }
                else if (ai_id == ai(rollout))
                {
                    playCard2 = rollout2(gameState, 2, Rollout_limit);
                }
                else if (ai_id == ai(mcts))
                {
                    playCard2 = MCTSPlayer(gameState, 2, MCTS_limit);
                }
                else if (ai_id == ai(mcts2))
                {
                    playCard2 = MCTSPlayer2(gameState, 2, MCTS_limit);
                }
                else if (ai_id == ai(bmcts))
                {
                    playCard2 = BMCTSPlayer(gameState, 2, MCTS_limit, SIM_LIMIT, BEAMWIDTH);
                }
                else if (ai_id == ai(human))
                {
                    std::cout << "Not Implemented" << std::endl;
                }
            }
            gameState = calculateCardPlay(gameState, playCard2);
            if (print)
                std::cout << "Player 2 Plays: " << playCard2.cardName << " ap: " << playCard2.attackPoints << std::endl;
            gameState.currentPlayerId = 2;
            gameState.played_card = playCard2;
        }
        while (gameState.player2Info.medicSet);
    }

    return gameState;
}



