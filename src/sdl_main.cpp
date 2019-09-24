/*  Jason Chalom 711985 - Honours Research Project 2017 Wits University

This will be the demo program*/

/*
todo:
=====

SDL renderer
commandline parameters
bash script
Sound
score
effects:
    glow
    border
key input:
    - display decks
    - show simulation??
    - game controller inputs
cleanup objects
readable text
timed plays
resizeable stuff
pass round card / button
*/

#include <stdio.h>
#include <cstdio>
#include <stdlib.h>
#include <limits.h>
#include <iostream>
#include <algorithm>
#include <omp.h>
#include <time.h>
#include <cmath>
#include <string>
#include <fstream>
#include <sstream>
#include <random>
#include <memory>
#include <unordered_map>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "definitions.h"

#include "../../include/rapidjson/document.h"
#include "../../include/rapidjson/filereadstream.h"

// ai paramters
int Rollout_limit = 1000;//2000;
int MCTS_limit = 100;//1000;//1000;
int SIM_LIMIT = 1;
int BEAMWIDTH = 4;

bool print = true;
float WAIT_TIME = 2.00;

#include "gameSupervisor/gameSupervisor.h"
#include "helpers/helper.h"
#include "sdl/window.h"

/* Global variables */
#define app_name "Survey of AI Methods in the Card Game Gwent"

#define results1_header "AI ID,AI ID,Player 1 Wins,Player 2 Wins,Draws,Max Runs,Total Time,Average Time"
#define results1_location "./results/results_exp1.csv"
#define results_rnd_loctaion "./results/results_rnd.csv"
#define results_mcts_header "AI ID,AI ID,Player 1 Wins,Player 2 Wins,Draws,Max Runs,Time"
#define results_mcts_loctaion "./results/results_mcts.csv"

string initial_state_path = "";
string northernpath = "./bin/northernDeck.json";
string nilfgaardpath = "./bin/nilfgaardDeck.json";

#include "optimal_play_tests.cpp"

int p1_ID = ai(mcts);
int P2_ID = ai(rnd_MT);

using namespace std;

int run_game(bool print = true, int p1 = ai(rnd_MT), int p2 = ai(mcts))
{
    vector<Card> northernDeck = convertJsonToVectorDeck(northernpath);
    vector<Card> nilfgaardDeck = convertJsonToVectorDeck(nilfgaardpath);

    GameState gameState(northernDeck, nilfgaardDeck);
    gameState.assignDeck(northernDeck, 1, 10);
    gameState.assignDeck(nilfgaardDeck, 2, 10);

    if (print)
    {
        cout << "Starting game..." << endl;
        print_deck(gameState.player1Info.getCurrentHand(), "Player 1 Hand:");
        print_deck(gameState.player2Info.getCurrentHand(), "Player 2 Hand:");
    }

    double start = omp_get_wtime();

    while (gameState.isComplete == false)
    {
        if (print)
            cout << endl << "Current Round: " << gameState.currentRound + 1 << " P1: " << gameState.player1Info.attackPoints.getCurrentAttackPoints() << " P2: " << gameState.player2Info.attackPoints.getCurrentAttackPoints() << "." << endl;

        gameState = playPlayer1(gameState, print, p1);
        // cout<<"p1 done."<<endl;
        gameState = playPlayer2(gameState, print, p2);
        // cout<<"p2 done."<<endl;

        gameState = determine_round_end_win(gameState, print);
        // int winner = gameState.roundWinnerId;
    }

    int game_winner = determine_winner(gameState, print);
    double time = omp_get_wtime() - start;

    if (print)
        cout << "Game Time: " << time << " seconds." << endl;

    return game_winner;
}

int main(/*int argc, char *argv[]*/)
{
    // General ints
    int posX = 100, posY = 100, card_width = 46, card_height = 86, hand_size = 10;
    // int screen_width = card_width*hand_size, screen_height = card_height*3;
    int screen_width = 1600, screen_height = 900;

    vector<Card> northernDeck = convertJsonToVectorDeck(northernpath);
    vector<Card> nilfgaardDeck = convertJsonToVectorDeck(nilfgaardpath);
    GameState gameState(northernDeck, nilfgaardDeck);
    gameState.assignDeck(northernDeck, 1, 10);
    gameState.assignDeck(nilfgaardDeck, 2, 10);

    if (print)
    {
        cout << "Starting game..." << endl;
        print_deck(gameState.player1Info.getCurrentHand(), "Player 1 Hand:");
        print_deck(gameState.player2Info.getCurrentHand(), "Player 2 Hand:");
    }

    string window_name = "GWENT Game";

    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "2" );

    Window window(gameState, window_name, hand_size, posX, posY, screen_width, screen_height);
    window.Render(gameState, card_width, card_height);

    float start = omp_get_wtime();
    bool exec = true;
    while (true)
    {
        SDL_Event e;
        if (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                break;
            }
            else if (e.type == SDL_KEYDOWN)
            {
                auto key = e.key;

                if (key.keysym.sym == 'Q' || key.keysym.sym == 'q' || key.keysym.sym == 27) //esc=27
                    break;
            }
        }

        float time = omp_get_wtime() - start;
        if ((time >= WAIT_TIME || exec) && gameState.isComplete == false)
        {
            exec = false;
            // window.Clear(); //is this needed?
            start = omp_get_wtime();

            if (print)
                cout << endl << "Current Round: " << gameState.currentRound + 1 << " P1: " << gameState.player1Info.attackPoints.getCurrentAttackPoints() << " P2: " << gameState.player2Info.attackPoints.getCurrentAttackPoints() << "." << endl;

            gameState = playPlayer1(gameState, print, p1_ID);
            window.Render(gameState, card_width, card_height);
            if (gameState.played_card.typeId == rules(spy))
            {
                window.RenderView(gameState.played_card, 1);
            }
            else
            {
                window.RenderView(gameState.played_card, 1);
            }

            // Pause for WAIT_TIME
            SDL_Delay(WAIT_TIME * 1000);
            gameState = playPlayer2(gameState, print, P2_ID);
            window.Render(gameState, card_width, card_height);
            if (gameState.played_card.typeId == rules(spy))
            {
                window.RenderView(gameState.played_card, 1);
            }
            else
            {
                window.RenderView(gameState.played_card, 1);
            }

            gameState = determine_round_end_win(gameState, print);
            window.Clear();
        }
        else
        {
            int game_winner = determine_winner(gameState, print);
            gameState.winPlayerId = game_winner;
            window.Clear();
            window.Render(gameState, card_width, card_height);

            SDL_Delay(7000);
            break;
        }
    }

    double time = omp_get_wtime() - start;

    if (print)
        cout << "Game Time: " << time << " seconds." << endl;



    window.Clear();
    TTF_Quit();
    SDL_Quit();

    return 0;
}
