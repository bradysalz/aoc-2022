#include <algorithm>
#include <iostream>
#include <fstream>
#include <functional>
#include <map>
#include <string>
#include <string.h>
#include <sstream>
#include <vector>

using std::cin;
using std::cout;
using std::string;
using std::vector;

const int LOSE = 0;
const int DRAW = 3;
const int WIN = 6;

std::map<char, int> SCORE_MAP = {
    {'X', 1},  // rock
    {'Y', 2},  // paper
    {'Z', 3}}; // scissors

int calcOutput(const char *fname)
{
    std::ifstream input(fname);

    int round_score;
    char hand;
    int score = 0;
    for (string line; getline(input, line);)
    {
        // rock
        if ((line[0] == 'A') && (line[2] == 'X'))
        {
            round_score = LOSE;
            hand = 'Z';
        }
        if ((line[0] == 'A') && (line[2] == 'Y'))
        {
            round_score = DRAW;
            hand = 'X';
        }
        if ((line[0] == 'A') && (line[2] == 'Z'))
        {
            round_score = WIN;
            hand = 'Y';
        }

        // Paper
        if ((line[0] == 'B') && (line[2] == 'X'))
        {
            round_score = LOSE;
            hand = 'X';
        }
        if ((line[0] == 'B') && (line[2] == 'Y'))
        {
            round_score = DRAW;
            hand = 'Y';
        }
        if ((line[0] == 'B') && (line[2] == 'Z'))
        {
            round_score = WIN;
            hand = 'Z';
        }

        // Scissors
        if ((line[0] == 'C') && (line[2] == 'X'))
        {
            round_score = LOSE;
            hand = 'Y';
        }
        if ((line[0] == 'C') && (line[2] == 'Y'))
        {
            round_score = DRAW;
            hand = 'Z';
        }
        if ((line[0] == 'C') && (line[2] == 'Z'))
        {
            round_score = WIN;
            hand = 'X';
        }
        score += round_score + SCORE_MAP[hand];
    }
    return score;
}

int scoreRounds(const char *fname)
{
    std::ifstream input(fname);

    int round_score;
    int score = 0;
    for (string line; getline(input, line);)
    {
        // Rock -> Rock/Paper/Scissors
        if ((line[0] == 'A') && (line[2] == 'X'))
            round_score = DRAW;
        if ((line[0] == 'A') && (line[2] == 'Y'))
            round_score = WIN;
        if ((line[0] == 'A') && (line[2] == 'Z'))
            round_score = LOSE;

        // Paper -> Rock/Paper/Scissors
        if ((line[0] == 'B') && (line[2] == 'X'))
            round_score = LOSE;
        if ((line[0] == 'B') && (line[2] == 'Y'))
            round_score = DRAW;
        if ((line[0] == 'B') && (line[2] == 'Z'))
            round_score = WIN;

        // Scissors -> Rock/Paper/Scissors
        if ((line[0] == 'C') && (line[2] == 'X'))
            round_score = WIN;
        if ((line[0] == 'C') && (line[2] == 'Y'))
            round_score = LOSE;
        if ((line[0] == 'C') && (line[2] == 'Z'))
            round_score = DRAW;

        int hand_score = SCORE_MAP[line[2]];
        score += (hand_score + round_score);
    }
    return score;
}

int main(int argc, char **argv)
{
    if (argc == 1)
    {
        std::cout << "Need to pass a filename\n";
        return 1;
    }

    cout << "Day 2 Part 1 " << scoreRounds(argv[1]) << "\n";
    cout << "Day 2 Part 2 " << calcOutput(argv[1]) << "\n";
    return 1;
}