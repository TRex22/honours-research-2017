/*  Jason Chalom 711985 - Honours Research Project 2017 Wits University

This will be the main test program*/
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

#include "definitions.h"

#include "../../include/rapidjson/document.h"
#include "../../include/rapidjson/filereadstream.h"

int Rollout_limit = 1000;//2000;
int MCTS_limit = 100;//1000;//1000;
int SIM_LIMIT = 1;
int BEAMWIDTH = 4;

#include "gameSupervisor/gameSupervisor.h"
#include "helpers/helper.h"

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

int max_runs = 10;//1000000;//100000;//10000;//1000000;//1000000;//1000;//000
int max_count_complete = 10;//10000;
bool use_rnd = false;
bool save_to_file = false;
bool print_output = false;
bool print_path = false;
bool save_csv = false;
double time_limit = -1.0;

int p1_ID = ai(mcts);
int P2_ID = ai(rnd_MT);

/* Headers */
int main(int argc, char *argv[]);
int run_game(bool print, int p1, int p2);
void run_stats();
void run_experiment();
void run_rnd_experiment(int max, int iter);
void run_mcts_experiment();
void run_exp_2();
void process_args(int argc, char *argv[]);

int main(int argc, char *argv[])
{
    print_cmd_heading(app_name);
    // omp_set_num_threads(1);
// #ifdef _OPENMP
//     std::cout << "Parallel Operation" << std::endl;
// #else
//     std::cout << "Single Thread Operation" << std::endl;
//     omp_set_num_threads(1);
// #endif

#if (DEBUG)
    printf("DEBUG Mode is enabled!\n");
#endif

    if (argc == 1)
    {
        print_usage(argv);
        halt_execution();
    }

    process_args(argc, argv);
    return EXIT_SUCCESS;
}

int run_game(bool print = true, int p1 = ai(rnd_MT), int p2 = ai(mcts))
{
    std::vector<Card> northernDeck = convertJsonToVectorDeck(northernpath);
    std::vector<Card> nilfgaardDeck = convertJsonToVectorDeck(nilfgaardpath);

    GameState gameState(northernDeck, nilfgaardDeck);
    gameState.assignDeck(northernDeck, 1, 10);
    gameState.assignDeck(nilfgaardDeck, 2, 10);

    if (print)
    {
        std::cout << "Starting game..." << std::endl;
        print_deck(gameState.player1Info.getCurrentHand(), "Player 1 Hand:");
        print_deck(gameState.player2Info.getCurrentHand(), "Player 2 Hand:");
    }

    double start = omp_get_wtime();

    while (gameState.isComplete == false)
    {
        if (print)
            std::cout << std::endl << "Current Round: " << gameState.currentRound + 1 << " P1: " << gameState.player1Info.attackPoints.getCurrentAttackPoints() << " P2: " << gameState.player2Info.attackPoints.getCurrentAttackPoints() << "." << std::endl;

        gameState = playPlayer1(gameState, print, p1);
        // std::cout<<"p1 done."<<std::endl;
        gameState = playPlayer2(gameState, print, p2);
        // std::cout<<"p2 done."<<std::endl;

        gameState = determine_round_end_win(gameState, print);
        // int winner = gameState.roundWinnerId;
    }

    int game_winner = determine_winner(gameState, print);
    double time = omp_get_wtime() - start;

    if (print)
        std::cout << "Game Time: " << time << " seconds." << std::endl;

    return game_winner;
}

void run_experiment()
{
    write_results_to_file(results1_location, results1_header, "");
    // int player1 = 0, player2 = 0, draw = 0;
    // double total_time = 0.0, start = 0.0, time = 0.0;

    std::cout << "Starting Experiment... (" << max_runs / 2 << " runs)" << std::endl;
    for (int k = 0; k < 7; k++) //number ais
    {
        for (int l = 0; l < 7; l++)
        {
            int player1 = 0, player2 = 0, draw = 0;
            double total_time = 0.0, start = 0.0, time = 0.0;

            p1_ID = k;
            P2_ID = l;

            #pragma omp parallel
            {
                // std::cout<<"test."<<std::endl;
                #pragma omp for reduction(+:player1,player2,draw)
                for (int i = 0; i < max_runs / 2; i++)
                {
                    start = omp_get_wtime();
                    int winner = run_game(false, p1_ID, P2_ID);
                    time = omp_get_wtime() - start;

                    if (winner == 1)
                        player1 ++;
                    if (winner == 2)
                        player2 ++;
                    if (winner == 0)
                        draw ++;
                    total_time += time;

                    if ((i + 1) % (max_count_complete) == 0.0)
                        std::cout << i + 1 << " Complete." << std::endl;
                }
            }

            std::cout << "Starting Experiment Inverse... (" << max_runs / 2 << " runs)" << std::endl;
            #pragma omp parallel
            {
                // std::cout<<"test."<<std::endl;
                #pragma omp for reduction(+:player1,player2,draw)
                for (int i = 0; i < max_runs / 2; i++)
                {
                    start = omp_get_wtime();
                    int winner = run_game(false, P2_ID, p1_ID);
                    time = omp_get_wtime() - start;

                    if (winner == 1)
                        player1 ++;
                    if (winner == 2)
                        player2 ++;
                    if (winner == 0)
                        draw ++;
                    total_time += time;

                    if ((i + 1) % (max_count_complete) == 0.0)
                        std::cout << i + 1 << " Complete." << std::endl;
                }
            }

            double avg_p1 = ((double)player1 / max_runs) * 100.00;
            double avg_p2 = ((double)player2 / max_runs) * 100.00;
            double avg_draw = ((double)draw / max_runs) * 100.00;

            std::cout << "Run Complete." << std::endl;
            std::cout << std::endl << "Results: (" << "AI: " << k << " and " << l << ")" << std::endl;
            std::cout << "Count   :=\tPlayer 1: " << player1 << "\t\tPlayer 2: " << player2 << "\t\tDraw: " << draw << std::endl;
            std::cout << "Percent :=\tPlayer 1: " << avg_p1 << "%\t\tPlayer 2: " << avg_p2 << "%\t\tDraw: " << avg_draw << "%" << std::endl;
            std::cout << "Total Game Time: " << total_time << " seconds" << "\tAverage Game Time: " << (total_time / max_runs) << " seconds." << std::endl;
            // min and max times

            // print file line
            ostringstream out;
            out << k << "," << l << "," << player1 << "," << player2 << "," << draw << "," << max_runs << "," << total_time << "," << (total_time / max_runs) << std::endl;
            write_results_to_file(results1_location, out.str());
        }
    }
}

void run_rnd_experiment(int max, int inter)
{
    write_results_to_file(results_rnd_loctaion, results1_header, "");
    // int player1 = 0, player2 = 0, draw = 0;
    // double total_time = 0.0, start = 0.0, time = 0.0;

    std::cout << "Starting Experiment... (" << max / 2 << " runs)" << std::endl;
    for (int k = 0; k < 4; k++) //number ais
    {
        for (int l = 0; l < 4; l++)
        {
            int player1 = 0, player2 = 0, draw = 0;
            double total_time = 0.0, start = 0.0, time = 0.0;

            p1_ID = k;
            P2_ID = l;
            #pragma omp parallel
            {
                // std::cout<<"test."<<std::endl;
                #pragma omp for reduction(+:player1,player2,draw)
                for (int i = 0; i < max / 2; i++)
                {
                    start = omp_get_wtime();
                    int winner = run_game(false, p1_ID, P2_ID);
                    time = omp_get_wtime() - start;

                    if (winner == 1)
                        player1 ++;
                    if (winner == 2)
                        player2 ++;
                    if (winner == 0)
                        draw ++;
                    total_time += time;

                    if ((i + 1) % (inter) == 0.0)
                        std::cout << i + 1 << " Complete." << std::endl;
                }
            }

            std::cout << "Starting Experiment Inverse... (" << max / 2 << " runs)" << std::endl;
            #pragma omp parallel
            {
                // std::cout<<"test."<<std::endl;
                #pragma omp for reduction(+:player1,player2,draw)
                for (int i = 0; i < max / 2; i++)
                {
                    start = omp_get_wtime();
                    int winner = run_game(false, P2_ID, p1_ID);
                    time = omp_get_wtime() - start;

                    if (winner == 1)
                        player2 ++;
                    if (winner == 2)
                        player1 ++;
                    if (winner == 0)
                        draw ++;
                    total_time += time;

                    if ((i + 1) % (inter) == 0.0)
                        std::cout << i + 1 << " Complete." << std::endl;
                }
            }

            double avg_p1 = ((double)player1 / max) * 100.00;
            double avg_p2 = ((double)player2 / max) * 100.00;
            double avg_draw = ((double)draw / max) * 100.00;

            std::cout << "Run Complete." << std::endl;
            std::cout << std::endl << "Results: (" << "AI: " << k << " and " << l << ")" << std::endl;
            std::cout << "Count   :=\tPlayer 1: " << player1 << "\t\tPlayer 2: " << player2 << "\t\tDraw: " << draw << std::endl;
            std::cout << "Percent :=\tPlayer 1: " << avg_p1 << "%\t\tPlayer 2: " << avg_p2 << "%\t\tDraw: " << avg_draw << "%" << std::endl;
            std::cout << "Total Game Time: " << total_time << " seconds" << "\tAverage Game Time: " << (total_time / max) << " seconds." << std::endl;
            // min and max times

            // print file line
            ostringstream out;
            out << k << "," << l << "," << player1 << "," << player2 << "," << draw << "," << max << "," << total_time << "," << (total_time / max) << std::endl;
            write_results_to_file(results_rnd_loctaion, out.str());
        }
    }
}

void run_rollout_experiment(int max, int inter)
{
    write_results_to_file(results1_location, results1_header, "");
    // int player1 = 0, player2 = 0, draw = 0;
    // double total_time = 0.0, start = 0.0, time = 0.0;

    std::cout << "Starting Experiment... (" << max / 2 << " runs)" << std::endl;
    for (int l = 0; l < 3; l++)
    {
        int player1 = 0, player2 = 0, draw = 0;
        double total_time = 0.0, start = 0.0, time = 0.0;

        p1_ID = ai(rollout);
        P2_ID = l;
        #pragma omp parallel
        {
            // std::cout<<"test."<<std::endl;
            #pragma omp for reduction(+:player1,player2,draw)
            for (int i = 0; i < max / 2; i++)
            {
                start = omp_get_wtime();
                int winner = run_game(false, p1_ID, P2_ID);
                time = omp_get_wtime() - start;

                if (winner == 1)
                    player1 ++;
                if (winner == 2)
                    player2 ++;
                if (winner == 0)
                    draw ++;
                total_time += time;

                if ((i + 1) % (inter) == 0.0)
                    std::cout << i + 1 << " Complete." << std::endl;
            }
        }

        std::cout << "Starting Experiment Inverse... (" << max / 2 << " runs)" << std::endl;
        #pragma omp parallel
        {
            // std::cout<<"test."<<std::endl;
            #pragma omp for reduction(+:player1,player2,draw)
            for (int i = 0; i < max / 2; i++)
            {
                start = omp_get_wtime();
                int winner = run_game(false, P2_ID, p1_ID);
                time = omp_get_wtime() - start;

                if (winner == 1)
                    player1 ++;
                if (winner == 2)
                    player2 ++;
                if (winner == 0)
                    draw ++;
                total_time += time;

                if ((i + 1) % (inter) == 0.0)
                    std::cout << i + 1 << " Complete." << std::endl;
            }
        }

        double avg_p1 = ((double)player1 / max) * 100.00;
        double avg_p2 = ((double)player2 / max) * 100.00;
        double avg_draw = ((double)draw / max) * 100.00;

        std::cout << "Run Complete." << std::endl;
        std::cout << std::endl << "Results: (" << "AI: " << ai(rollout) << " and " << l << ")" << std::endl;
        std::cout << "Count   :=\tPlayer 1: " << player1 << "\t\tPlayer 2: " << player2 << "\t\tDraw: " << draw << std::endl;
        std::cout << "Percent :=\tPlayer 1: " << avg_p1 << "%\t\tPlayer 2: " << avg_p2 << "%\t\tDraw: " << avg_draw << "%" << std::endl;
        std::cout << "Total Game Time: " << total_time << " seconds" << "\tAverage Game Time: " << (total_time / max) << " seconds." << std::endl;
        // min and max times

        // print file line
        ostringstream out;
        out << ai(rollout) << "," << l << "," << player1 << "," << player2 << "," << draw << "," << max << "," << total_time << "," << (total_time / max) << std::endl;
        write_results_to_file(results_rnd_loctaion, out.str());
    }
}

void run_mcts_experiment()
{
    write_results_to_file(results1_location, results1_header, "");
    // int player1 = 0, player2 = 0, draw = 0;
    // double total_time = 0.0, start = 0.0, time = 0.0;

    std::cout << "Starting Experiment... (" << max_runs / 2 << " runs)" << std::endl;
    for (int k = 0; k < 4; k++) //number ais
    {
        for (int l = 0; l < 1; l++)
        {
            int player1 = 0, player2 = 0, draw = 0;
            double total_time = 0.0, start = 0.0, time = 0.0;

            p1_ID = ai(mcts);
            P2_ID = k;
            #pragma omp parallel
            {
                // std::cout<<"test."<<std::endl;
                #pragma omp for reduction(+:player1,player2,draw)
                for (int i = 0; i < max_runs / 2; i++)
                {
                    start = omp_get_wtime();
                    int winner = run_game(false, p1_ID, P2_ID);
                    time = omp_get_wtime() - start;

                    if (winner == 1)
                        player1 ++;
                    if (winner == 2)
                        player2 ++;
                    if (winner == 0)
                        draw ++;
                    total_time += time;

                    if ((i + 1) % (max_count_complete) == 0.0)
                        std::cout << i + 1 << " Complete." << std::endl;
                }
            }

            std::cout << "Starting Experiment Inverse... (" << max_runs / 2 << " runs)" << std::endl;
            #pragma omp parallel
            {
                // std::cout<<"test."<<std::endl;
                #pragma omp for reduction(+:player1,player2,draw)
                for (int i = 0; i < max_runs / 2; i++)
                {
                    start = omp_get_wtime();
                    int winner = run_game(false, P2_ID, p1_ID);
                    time = omp_get_wtime() - start;

                    if (winner == 1)
                        player2 ++;
                    if (winner == 2)
                        player1 ++;
                    if (winner == 0)
                        draw ++;
                    total_time += time;

                    if ((i + 1) % (max_count_complete) == 0.0)
                        std::cout << i + 1 << " Complete." << std::endl;
                }
            }

            double avg_p1 = ((double)player1 / max_runs) * 100.00;
            double avg_p2 = ((double)player2 / max_runs) * 100.00;
            double avg_draw = ((double)draw / max_runs) * 100.00;

            std::cout << "Run Complete." << std::endl;
            std::cout << std::endl << "Results: (" << "AI: " << p1_ID << " and " << P2_ID << ")" << std::endl;
            std::cout << "Count   :=\tPlayer 1: " << player1 << "\t\tPlayer 2: " << player2 << "\t\tDraw: " << draw << std::endl;
            std::cout << "Percent :=\tPlayer 1: " << avg_p1 << "%\t\tPlayer 2: " << avg_p2 << "%\t\tDraw: " << avg_draw << "%" << std::endl;
            std::cout << "Total Game Time: " << total_time << " seconds" << "\tAverage Game Time: " << (total_time / max_runs) << " seconds." << std::endl;
            // min and max times

            // print file line
            ostringstream out;
            out << p1_ID << "," << P2_ID << "," << player1 << "," << player2 << "," << draw << "," << max_runs << "," << total_time << "," << (total_time / max_runs) << std::endl;
            write_results_to_file(results1_location, out.str());
        }
    }
}

void run_mcts_experiment_2()
{
}

void run_mcts_experiment_3()
{
    int player1 = 0, player2 = 0, draw = 0;
    p1_ID = ai(mcts);
    P2_ID = ai(rnd_xorshf);
    double start = omp_get_wtime();
    int winner = run_game(true, p1_ID, P2_ID);
    double time = omp_get_wtime() - start;

    if (winner == 1)
        player1 ++;
    if (winner == 2)
        player2 ++;
    if (winner == 0)
        draw ++;

    start = omp_get_wtime();
    winner += run_game(true, P2_ID, p1_ID);
    time += omp_get_wtime() - start;
    time = time / 2;
    // winner += run_game(true, p1_ID, p1_ID);
    // winner += run_game(true, p1_ID, p1_ID);
    // winner += run_game(true, p1_ID, p1_ID);
    // winner += run_game(true, p1_ID, p1_ID);
    // winner += run_game(true, p1_ID, p1_ID);
    // winner += run_game(true, p1_ID, p1_ID);
    // winner += run_game(true, p1_ID, p1_ID);
    // winner += run_game(true, p1_ID, p1_ID);

    std::cout << "Avg Winner: " << (double)winner / 2 << std::endl;
    if (winner == 1)
        player2 ++;
    if (winner == 2)
        player1 ++;
    if (winner == 0)
        draw ++;
    // #define results_mcts_header "AI ID,AI ID,Player 1 Wins,Player 2 Wins,Draws,Max Runs,Time"
    // #define results_mcts_loctaion "./results/results_mcts.csv"

    ostringstream out;
    out << p1_ID << "," << P2_ID << "," << player1 << "," << player2 << "," << draw << "," << 2 << "," << time << std::endl;
    write_results_to_file(results_mcts_loctaion, out.str());
}

void run_mcts2_experiment()
{
    write_results_to_file(results1_location, results1_header, "");
    // int player1 = 0, player2 = 0, draw = 0;
    // double total_time = 0.0, start = 0.0, time = 0.0;

    std::cout << "Starting Experiment... (" << max_runs / 2 << " runs)" << std::endl;
    for (int k = 0; k < 4; k++) //number ais
    {
        for (int l = 0; l < 1; l++)
        {
            int player1 = 0, player2 = 0, draw = 0;
            double total_time = 0.0, start = 0.0, time = 0.0;

            p1_ID = ai(mcts2);
            P2_ID = k;
            #pragma omp parallel
            {
                // std::cout<<"test."<<std::endl;
                #pragma omp for reduction(+:player1,player2,draw)
                for (int i = 0; i < max_runs / 2; i++)
                {
                    start = omp_get_wtime();
                    int winner = run_game(false, p1_ID, P2_ID);
                    time = omp_get_wtime() - start;

                    if (winner == 1)
                        player1 ++;
                    if (winner == 2)
                        player2 ++;
                    if (winner == 0)
                        draw ++;
                    total_time += time;

                    if ((i + 1) % (max_count_complete) == 0.0)
                        std::cout << i + 1 << " Complete." << std::endl;
                }
            }

            std::cout << "Starting Experiment Inverse... (" << max_runs / 2 << " runs)" << std::endl;
            #pragma omp parallel
            {
                // std::cout<<"test."<<std::endl;
                #pragma omp for reduction(+:player1,player2,draw)
                for (int i = 0; i < max_runs / 2; i++)
                {
                    start = omp_get_wtime();
                    int winner = run_game(false, P2_ID, p1_ID);
                    time = omp_get_wtime() - start;

                    if (winner == 1)
                        player2 ++;
                    if (winner == 2)
                        player1 ++;
                    if (winner == 0)
                        draw ++;
                    total_time += time;

                    if ((i + 1) % (max_count_complete) == 0.0)
                        std::cout << i + 1 << " Complete." << std::endl;
                }
            }

            double avg_p1 = ((double)player1 / max_runs) * 100.00;
            double avg_p2 = ((double)player2 / max_runs) * 100.00;
            double avg_draw = ((double)draw / max_runs) * 100.00;

            std::cout << "Run Complete." << std::endl;
            std::cout << std::endl << "Results: (" << "AI: " << p1_ID << " and " << P2_ID << ")" << std::endl;
            std::cout << "Count   :=\tPlayer 1: " << player1 << "\t\tPlayer 2: " << player2 << "\t\tDraw: " << draw << std::endl;
            std::cout << "Percent :=\tPlayer 1: " << avg_p1 << "%\t\tPlayer 2: " << avg_p2 << "%\t\tDraw: " << avg_draw << "%" << std::endl;
            std::cout << "Total Game Time: " << total_time << " seconds" << "\tAverage Game Time: " << (total_time / max_runs) << " seconds." << std::endl;
            // min and max times

            // print file line
            ostringstream out;
            out << p1_ID << "," << P2_ID << "," << player1 << "," << player2 << "," << draw << "," << max_runs << "," << total_time << "," << (total_time / max_runs) << std::endl;
            write_results_to_file(results1_location, out.str());
        }
    }
}

void run_bmcts_experiment()
{
    write_results_to_file(results1_location, results1_header, "");
    // int player1 = 0, player2 = 0, draw = 0;
    // double total_time = 0.0, start = 0.0, time = 0.0;

    std::cout << "Starting Experiment... (" << max_runs / 2 << " runs)" << std::endl;
    for (int k = 0; k < 4; k++) //number ais
    {
        for (int l = 0; l < 1; l++)
        {
            int player1 = 0, player2 = 0, draw = 0;
            double total_time = 0.0, start = 0.0, time = 0.0;

            p1_ID = ai(bmcts);
            P2_ID = k;
            #pragma omp parallel
            {
                // std::cout<<"test."<<std::endl;
                #pragma omp for reduction(+:player1,player2,draw)
                for (int i = 0; i < max_runs / 2; i++)
                {
                    start = omp_get_wtime();
                    int winner = run_game(false, p1_ID, P2_ID);
                    time = omp_get_wtime() - start;

                    if (winner == 1)
                        player1 ++;
                    if (winner == 2)
                        player2 ++;
                    if (winner == 0)
                        draw ++;
                    total_time += time;

                    if ((i + 1) % (max_count_complete) == 0.0)
                        std::cout << i + 1 << " Complete." << std::endl;
                }
            }

            std::cout << "Starting Experiment Inverse... (" << max_runs / 2 << " runs)" << std::endl;
            #pragma omp parallel
            {
                // std::cout<<"test."<<std::endl;
                #pragma omp for reduction(+:player1,player2,draw)
                for (int i = 0; i < max_runs / 2; i++)
                {
                    start = omp_get_wtime();
                    int winner = run_game(false, P2_ID, p1_ID);
                    time = omp_get_wtime() - start;

                    if (winner == 1)
                        player2 ++;
                    if (winner == 2)
                        player1 ++;
                    if (winner == 0)
                        draw ++;
                    total_time += time;

                    if ((i + 1) % (max_count_complete) == 0.0)
                        std::cout << i + 1 << " Complete." << std::endl;
                }
            }

            double avg_p1 = ((double)player1 / max_runs) * 100.00;
            double avg_p2 = ((double)player2 / max_runs) * 100.00;
            double avg_draw = ((double)draw / max_runs) * 100.00;

            std::cout << "Run Complete." << std::endl;
            std::cout << std::endl << "Results: (" << "AI: " << p1_ID << " and " << P2_ID << ")" << std::endl;
            std::cout << "Count   :=\tPlayer 1: " << player1 << "\t\tPlayer 2: " << player2 << "\t\tDraw: " << draw << std::endl;
            std::cout << "Percent :=\tPlayer 1: " << avg_p1 << "%\t\tPlayer 2: " << avg_p2 << "%\t\tDraw: " << avg_draw << "%" << std::endl;
            std::cout << "Total Game Time: " << total_time << " seconds" << "\tAverage Game Time: " << (total_time / max_runs) << " seconds." << std::endl;
            // min and max times

            // print file line
            ostringstream out;
            out << p1_ID << "," << P2_ID << "," << player1 << "," << player2 << "," << draw << "," << max_runs << "," << total_time << "," << (total_time / max_runs) << std::endl;
            write_results_to_file(results1_location, out.str());
        }
    }
}

void run_rollout_experiment()
{
    write_results_to_file(results1_location, results1_header, "");
    // int player1 = 0, player2 = 0, draw = 0;
    // double total_time = 0.0, start = 0.0, time = 0.0;

    std::cout << "Starting Experiment... (" << max_runs / 2 << " runs)" << std::endl;
    for (int k = 0; k < 4; k++) //number ais
    {
        for (int l = 0; l < 1; l++)
        {
            int player1 = 0, player2 = 0, draw = 0;
            double total_time = 0.0, start = 0.0, time = 0.0;

            p1_ID = ai(mcts);
            P2_ID = k;
            #pragma omp parallel
            {
                // std::cout<<"test."<<std::endl;
                #pragma omp for reduction(+:player1,player2,draw)
                for (int i = 0; i < max_runs / 2; i++)
                {
                    start = omp_get_wtime();
                    int winner = run_game(false, p1_ID, P2_ID);
                    time = omp_get_wtime() - start;

                    if (winner == 1)
                        player1 ++;
                    if (winner == 2)
                        player2 ++;
                    if (winner == 0)
                        draw ++;
                    total_time += time;

                    if ((i + 1) % (max_count_complete) == 0.0)
                        std::cout << i + 1 << " Complete." << std::endl;
                }
            }

            std::cout << "Starting Experiment Inverse... (" << max_runs / 2 << " runs)" << std::endl;
            #pragma omp parallel
            {
                // std::cout<<"test."<<std::endl;
                #pragma omp for reduction(+:player1,player2,draw)
                for (int i = 0; i < max_runs / 2; i++)
                {
                    start = omp_get_wtime();
                    int winner = run_game(false, P2_ID, p1_ID);
                    time = omp_get_wtime() - start;

                    if (winner == 1)
                        player2 ++;
                    if (winner == 2)
                        player1 ++;
                    if (winner == 0)
                        draw ++;
                    total_time += time;

                    if ((i + 1) % (max_count_complete) == 0.0)
                        std::cout << i + 1 << " Complete." << std::endl;
                }
            }

            double avg_p1 = ((double)player1 / max_runs) * 100.00;
            double avg_p2 = ((double)player2 / max_runs) * 100.00;
            double avg_draw = ((double)draw / max_runs) * 100.00;

            std::cout << "Run Complete." << std::endl;
            std::cout << std::endl << "Results: (" << "AI: " << p1_ID << " and " << P2_ID << ")" << std::endl;
            std::cout << "Count   :=\tPlayer 1: " << player1 << "\t\tPlayer 2: " << player2 << "\t\tDraw: " << draw << std::endl;
            std::cout << "Percent :=\tPlayer 1: " << avg_p1 << "%\t\tPlayer 2: " << avg_p2 << "%\t\tDraw: " << avg_draw << "%" << std::endl;
            std::cout << "Total Game Time: " << total_time << " seconds" << "\tAverage Game Time: " << (total_time / max_runs) << " seconds." << std::endl;
            // min and max times

            // print file line
            ostringstream out;
            out << p1_ID << "," << P2_ID << "," << player1 << "," << player2 << "," << draw << "," << max_runs << "," << total_time << "," << (total_time / max_runs) << std::endl;
            write_results_to_file(results1_location, out.str());
        }
    }
}

void run_heuristic_experiment()
{
    write_results_to_file(results1_location, results1_header, "");
    // int player1 = 0, player2 = 0, draw = 0;
    // double total_time = 0.0, start = 0.0, time = 0.0;

    std::cout << "Starting Experiment... (" << max_runs / 2 << " runs)" << std::endl;
    for (int k = 4; k < 7; k++) //number ais
    {
        for (int l = 0; l < 1; l++)
        {
            int player1 = 0, player2 = 0, draw = 0;
            double total_time = 0.0, start = 0.0, time = 0.0;

            p1_ID = ai(heuristic);
            P2_ID = k;
            #pragma omp parallel
            {
                // std::cout<<"test."<<std::endl;
                #pragma omp for reduction(+:player1,player2,draw)
                for (int i = 0; i < max_runs / 2; i++)
                {
                    start = omp_get_wtime();
                    int winner = run_game(false, p1_ID, P2_ID);
                    time = omp_get_wtime() - start;

                    if (winner == 1)
                        player1 ++;
                    if (winner == 2)
                        player2 ++;
                    if (winner == 0)
                        draw ++;
                    total_time += time;

                    if ((i + 1) % (max_count_complete) == 0.0)
                        std::cout << i + 1 << " Complete." << std::endl;
                }
            }

            std::cout << "Starting Experiment Inverse... (" << max_runs / 2 << " runs)" << std::endl;
            #pragma omp parallel
            {
                // std::cout<<"test."<<std::endl;
                #pragma omp for reduction(+:player1,player2,draw)
                for (int i = 0; i < max_runs / 2; i++)
                {
                    start = omp_get_wtime();
                    int winner = run_game(false, P2_ID, p1_ID);
                    time = omp_get_wtime() - start;

                    if (winner == 1)
                        player2 ++;
                    if (winner == 2)
                        player1 ++;
                    if (winner == 0)
                        draw ++;
                    total_time += time;

                    if ((i + 1) % (max_count_complete) == 0.0)
                        std::cout << i + 1 << " Complete." << std::endl;
                }
            }

            double avg_p1 = ((double)player1 / max_runs) * 100.00;
            double avg_p2 = ((double)player2 / max_runs) * 100.00;
            double avg_draw = ((double)draw / max_runs) * 100.00;

            std::cout << "Run Complete." << std::endl;
            std::cout << std::endl << "Results: (" << "AI: " << p1_ID << " and " << P2_ID << ")" << std::endl;
            std::cout << "Count   :=\tPlayer 1: " << player1 << "\t\tPlayer 2: " << player2 << "\t\tDraw: " << draw << std::endl;
            std::cout << "Percent :=\tPlayer 1: " << avg_p1 << "%\t\tPlayer 2: " << avg_p2 << "%\t\tDraw: " << avg_draw << "%" << std::endl;
            std::cout << "Total Game Time: " << total_time << " seconds" << "\tAverage Game Time: " << (total_time / max_runs) << " seconds." << std::endl;
            // min and max times

            // print file line
            ostringstream out;
            out << p1_ID << "," << P2_ID << "," << player1 << "," << player2 << "," << draw << "," << max_runs << "," << total_time << "," << (total_time / max_runs) << std::endl;
            write_results_to_file(results1_location, out.str());
        }
    }
}

void run_special1()
{
    write_results_to_file(results1_location, results1_header, "");
    // int player1 = 0, player2 = 0, draw = 0;
    // double total_time = 0.0, start = 0.0, time = 0.0;

    std::cout << "Starting Experiment... (" << max_runs / 2 << " runs)" << std::endl;
    for (int k = 5; k < 7; k++) //number ais
    {
        for (int l = 0; l < 1; l++)
        {
            int player1 = 0, player2 = 0, draw = 0;
            double total_time = 0.0, start = 0.0, time = 0.0;

            p1_ID = ai(rollout);
            P2_ID = k;
            #pragma omp parallel
            {
                // std::cout<<"test."<<std::endl;
                #pragma omp for reduction(+:player1,player2,draw)
                for (int i = 0; i < max_runs / 2; i++)
                {
                    start = omp_get_wtime();
                    int winner = run_game(false, p1_ID, P2_ID);
                    time = omp_get_wtime() - start;

                    if (winner == 1)
                        player1 ++;
                    if (winner == 2)
                        player2 ++;
                    if (winner == 0)
                        draw ++;
                    total_time += time;

                    if ((i + 1) % (max_count_complete) == 0.0)
                        std::cout << i + 1 << " Complete." << std::endl;
                }
            }

            std::cout << "Starting Experiment Inverse... (" << max_runs / 2 << " runs)" << std::endl;
            #pragma omp parallel
            {
                // std::cout<<"test."<<std::endl;
                #pragma omp for reduction(+:player1,player2,draw)
                for (int i = 0; i < max_runs / 2; i++)
                {
                    start = omp_get_wtime();
                    int winner = run_game(false, P2_ID, p1_ID);
                    time = omp_get_wtime() - start;

                    if (winner == 1)
                        player2 ++;
                    if (winner == 2)
                        player1 ++;
                    if (winner == 0)
                        draw ++;
                    total_time += time;

                    if ((i + 1) % (max_count_complete) == 0.0)
                        std::cout << i + 1 << " Complete." << std::endl;
                }
            }

            double avg_p1 = ((double)player1 / max_runs) * 100.00;
            double avg_p2 = ((double)player2 / max_runs) * 100.00;
            double avg_draw = ((double)draw / max_runs) * 100.00;

            std::cout << "Run Complete." << std::endl;
            std::cout << std::endl << "Results: (" << "AI: " << p1_ID << " and " << P2_ID << ")" << std::endl;
            std::cout << "Count   :=\tPlayer 1: " << player1 << "\t\tPlayer 2: " << player2 << "\t\tDraw: " << draw << std::endl;
            std::cout << "Percent :=\tPlayer 1: " << avg_p1 << "%\t\tPlayer 2: " << avg_p2 << "%\t\tDraw: " << avg_draw << "%" << std::endl;
            std::cout << "Total Game Time: " << total_time << " seconds" << "\tAverage Game Time: " << (total_time / max_runs) << " seconds." << std::endl;
            // min and max times

            // print file line
            ostringstream out;
            out << p1_ID << "," << P2_ID << "," << player1 << "," << player2 << "," << draw << "," << max_runs << "," << total_time << "," << (total_time / max_runs) << std::endl;
            write_results_to_file(results1_location, out.str());
        }
    }
}

void run_special2()
{
    write_results_to_file(results1_location, results1_header, "");
    // int player1 = 0, player2 = 0, draw = 0;
    // double total_time = 0.0, start = 0.0, time = 0.0;

    std::cout << "Starting Experiment... (" << max_runs / 2 << " runs)" << std::endl;
    for (int l = 0; l < 1; l++)
    {
        int player1 = 0, player2 = 0, draw = 0;
        double total_time = 0.0, start = 0.0, time = 0.0;

        p1_ID = ai(mcts);
        P2_ID = ai(mcts2);
        #pragma omp parallel
        {
            // std::cout<<"test."<<std::endl;
            #pragma omp for reduction(+:player1,player2,draw)
            for (int i = 0; i < max_runs / 2; i++)
            {
                start = omp_get_wtime();
                int winner = run_game(false, p1_ID, P2_ID);
                time = omp_get_wtime() - start;

                if (winner == 1)
                    player1 ++;
                if (winner == 2)
                    player2 ++;
                if (winner == 0)
                    draw ++;
                total_time += time;

                if ((i + 1) % (max_count_complete) == 0.0)
                    std::cout << i + 1 << " Complete." << std::endl;
            }
        }

        std::cout << "Starting Experiment Inverse... (" << max_runs / 2 << " runs)" << std::endl;
        #pragma omp parallel
        {
            // std::cout<<"test."<<std::endl;
            #pragma omp for reduction(+:player1,player2,draw)
            for (int i = 0; i < max_runs / 2; i++)
            {
                start = omp_get_wtime();
                int winner = run_game(false, P2_ID, p1_ID);
                time = omp_get_wtime() - start;

                if (winner == 1)
                    player2 ++;
                if (winner == 2)
                    player1 ++;
                if (winner == 0)
                    draw ++;
                total_time += time;

                if ((i + 1) % (max_count_complete) == 0.0)
                    std::cout << i + 1 << " Complete." << std::endl;
            }
        }

        double avg_p1 = ((double)player1 / max_runs) * 100.00;
        double avg_p2 = ((double)player2 / max_runs) * 100.00;
        double avg_draw = ((double)draw / max_runs) * 100.00;

        std::cout << "Run Complete." << std::endl;
        std::cout << std::endl << "Results: (" << "AI: " << p1_ID << " and " << P2_ID << ")" << std::endl;
        std::cout << "Count   :=\tPlayer 1: " << player1 << "\t\tPlayer 2: " << player2 << "\t\tDraw: " << draw << std::endl;
        std::cout << "Percent :=\tPlayer 1: " << avg_p1 << "%\t\tPlayer 2: " << avg_p2 << "%\t\tDraw: " << avg_draw << "%" << std::endl;
        std::cout << "Total Game Time: " << total_time << " seconds" << "\tAverage Game Time: " << (total_time / max_runs) << " seconds." << std::endl;
        // min and max times

        // print file line
        ostringstream out;
        out << p1_ID << "," << P2_ID << "," << player1 << "," << player2 << "," << draw << "," << max_runs << "," << total_time << "," << (total_time / max_runs) << std::endl;
        write_results_to_file(results1_location, out.str());
    }
}

void run_stats()
{
    printf("Running system checks...\n\n");

    printf("1. Loading decks...\n");
    std::vector<Card> northernDeck = convertJsonToVectorDeck(northernpath);
    std::vector<Card> nilfgaardDeck = convertJsonToVectorDeck(nilfgaardpath);

    printf("2. Deck Statistics...\n");
    printDeckStats(northernDeck, "Northern Realm");
    printDeckStats(nilfgaardDeck, "Nilfgaard");

    printf("3. Randomise deck...\n");
    std::vector<Card> smallDeck(5);
    for (int i = 0; i < 5; i++)
    {
        smallDeck[i] = northernDeck[i];
    }
    print_deck(smallDeck, "Not Shuffled Deck:\n");
    std::random_device d;
    std::mt19937 rng(d());
    std::shuffle(std::begin(smallDeck), std::end(smallDeck), rng);
    print_deck(smallDeck, "Shuffled Deck:\n");

    printf("4. Player Info...\n");
    PlayerInfo playerInfo;
    playerInfo.resetRound();

    printf("5. Game State... (Assign Decks)\n");
    GameState gameState(northernDeck, nilfgaardDeck);
    gameState.assignDeck(northernDeck, 1, 10);
    print_deck(northernDeck, "Northern Deck:\n");
    gameState.assignDeck(nilfgaardDeck, 2, 10);
    print_deck(nilfgaardDeck, "Nilfgaard Deck:\n");
    // gameState.print();

    // printf("6. 1-Card:\n");
    // Card Geralt = gameState.player1Info->getCardFromHand("Geralt of Rivia");
    // printf(Geralt.cardName.c_str());
    // printf("\n%d", Geralt.cardId);
    // printf("\n%d", Geralt.attackPoints);

    printf("6. Random Game Times...\n");
    int num_runs_rnd = max_runs;
    double time_mt = 0.0;
    double time_fast = 0.0;
    double time_xor = 0.0;
    double time_mcts = 0.0;
    double time_mcts2 = 0.0;
    double time_rollout = 0.0;
    double total_time = omp_get_wtime();

    std::cout << "Iterations: ";

    for (int i = 0; i < num_runs_rnd; ++i)
    {
        std::cout << (i + 1) << " ";
        double start = omp_get_wtime();
        run_game(false, ai(rnd_MT), ai(rnd_MT));
        time_mt += omp_get_wtime() - start;

        start = omp_get_wtime();
        run_game(false, ai(rnd_fast), ai(rnd_fast));
        time_fast += omp_get_wtime() - start;

        start = omp_get_wtime();
        run_game(false, ai(rnd_xorshf), ai(rnd_xorshf));
        time_xor += omp_get_wtime() - start;

        start = omp_get_wtime();
        run_game(false, ai(mcts), ai(mcts));
        time_mcts += omp_get_wtime() - start;

        start = omp_get_wtime();
        run_game(false, ai(mcts2), ai(mcts2));
        time_mcts2 += omp_get_wtime() - start;

        start = omp_get_wtime();
        run_game(false, ai(rollout), ai(rollout));
        time_rollout += omp_get_wtime() - start;
    }

    total_time = omp_get_wtime() - total_time;

    printf("\n\nTimes:\n");
    printf("MT: %f secs.\n", time_mt / num_runs_rnd);
    printf("rnd_fast: %f secs.\n", time_fast / num_runs_rnd);
    printf("rnd_xorshf: %f secs.\n", time_xor / num_runs_rnd);
    printf("rollout: %f secs.\n", time_rollout / num_runs_rnd);
    printf("mcts: %f secs.\n", time_mcts / num_runs_rnd);
    printf("mcts2: %f secs.\n", time_mcts2 / num_runs_rnd);
    printf("Total Time: %f secs.\n", total_time);


    printf("\n\n");

}

void run_exp_2()
{
    // int million = 1000000;
    // run_rnd_experiment(million, 100000);


}

void process_args(int argc, char *argv[])
{
    for (int i = 1; i < argc; i++)
    {
        string str = string(argv[i]);
        if (contains_string(str, "run_stats"))
        {
            run_stats();
        }

        if (contains_string(str, "run_game"))
        {
            run_game(true, p1_ID, P2_ID);
        }

        if (contains_string(str, "rollout_game"))
        {
            run_game(true, ai(rollout), ai(rnd_MT));
        }

        if (contains_string(str, "mcts_game"))
        {
            run_game(true, ai(mcts), ai(rnd_MT));
        }

        if (contains_string(str, "run_exp"))
        {
            run_experiment();
        }

        if (contains_string(str, "run_rnd_exp"))
        {
            run_rnd_experiment(10000, 1000);
        }

        if (contains_string(str, "run_mcts_exp"))
        {
            run_mcts_experiment();
            // run_mcts_experiment_2();
            // run_mcts_experiment_3();
        }

        if (contains_string(str, "run_exp_2"))
        {
            run_exp_2();
        }

        if (contains_string(str, "run_rollout"))
        {
            run_rollout_experiment(500, 100);
        }

        if (contains_string(str, "run_optimal"))
        {
            run_optimal_choice_test(1000, 100);
        }


        if (contains_string(str, "mcts2"))
        {
            run_mcts2_experiment();
        }

        if (contains_string(str, "bmcts"))
        {
            run_bmcts_experiment();
        }

        if (contains_string(str, "rollout"))
        {
            run_rollout_experiment();
        }


        if (contains_string(str, "heuristic"))
        {
            run_heuristic_experiment();
        }

        if (contains_string(str, "special1"))
        {
            run_special1();
        }

        if (contains_string(str, "special2"))
        {
            run_special2();
        }
    }
}
