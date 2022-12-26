#include <cmath>
#include <iostream>
#include <fstream>
#include <map>
#include <set>
#include <string>
#include <string.h>
#include <sstream>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;

int part_one(const char *fname)
{
    std::map<std::pair<int, int>, bool> trail;
    int headX = 0, headY = 0, tailX = 0, tailY = 0;
    trail[{0, 0}] = true;

    std::ifstream input(fname);
    for (string line; getline(input, line);)
    {
        char dir = line[0];
        int val = std::stoi(line.substr(2));

        for (int ii = 0; ii < val; ii++)
        {
            if (dir == 'R')
                headX++;
            else if (dir == 'L')
                headX--;
            else if (dir == 'U')
                headY++;
            else
                headY--;

            if ((std::abs(headX - tailX) > 1) || (std::abs(headY - tailY) > 1))
            {
                // tail catchups
                if (headX == tailX)
                    tailY += (headY > tailY) ? 1 : -1;
                else if (headY == tailY)
                    tailX += (headX > tailX) ? 1 : -1;
                else if (std::abs(headX - tailX) > 1)
                {
                    tailX += (headX > tailX) ? 1 : -1;
                    tailY = headY;
                }
                else if (std::abs(headY - tailY) > 1)
                {
                    tailX = headX;
                    tailY += (headY > tailY) ? 1 : -1;
                }
                else
                {
                    cout << "ruh roh" << endl;
                }

                trail[{tailX, tailY}] = true;
            }
        }
    }
    return trail.size();
}

void print_grid(vector<std::pair<int, int>> knots)
{
    int GRID_MIN = -13;
    int GRID_MAX = 13;
    // print backwards becuase (0,0) is bottom left
    for (int ii = GRID_MAX; ii >= GRID_MIN; ii--)
    {
        for (int jj = GRID_MIN; jj < GRID_MAX; jj++)
        {
            string c = ".";
            for (int it = knots.size() - 1; it >= 0; it--)
            {
                // if (ii == 0 && jj == 0)
                //     c = 's';
                if ((knots[it].first == jj) && (knots[it].second == ii))
                {
                    c = std::to_string(it);
                    if (it == 0)
                        c = 'H';
                }
            }
            cout << c;
        }
        cout << endl;
    }
    cout << endl;
}
int part_two(const char *fname)
{
    std::map<std::pair<int, int>, bool> trail;
    vector<std::pair<int, int>> knots = {
        {0, 0}, // head
        {0, 0}, // 1
        {0, 0}, // 2
        {0, 0}, // 3
        {0, 0}, // 4
        {0, 0}, // 5
        {0, 0}, // 6
        {0, 0}, // 7
        {0, 0}, // 8
        {0, 0}, // 9
    };

    trail[{0, 0}] = true;

    std::ifstream input(fname);
    for (string line; getline(input, line);)
    {
        char dir = line[0];
        int val = std::stoi(line.substr(2));
        cout << line << endl;
        for (int ii = 0; ii < val; ii++)
        {
            // update head
            if (dir == 'R')
                knots[0].first++;
            else if (dir == 'L')
                knots[0].first--;
            else if (dir == 'U')
                knots[0].second++;
            else
                knots[0].second--;

            // update tails
            // i'm so soryr. this is so bad. but it's an easy copy/paste "lift" from part_one
            // headX -> knots[jj].first
            // headY -> knots[jj].second
            // tailX -> knots[jj+1].first
            // tailY -> knots[jj+1].second
            for (int jj = 0; jj < knots.size() - 1; jj++)
            {
                if ((std::abs(knots[jj].first - knots[jj + 1].first) > 1) || (std::abs(knots[jj].second - knots[jj + 1].second) > 1))
                {
                    // tail catchups
                    if (knots[jj].first == knots[jj + 1].first)
                        knots[jj + 1].second += (knots[jj].second > knots[jj + 1].second) ? 1 : -1;
                    else if (knots[jj].second == knots[jj + 1].second)
                        knots[jj + 1].first += (knots[jj].first > knots[jj + 1].first) ? 1 : -1;
                    else if ((std::abs(knots[jj].first - knots[jj + 1].first) > 1) && (std::abs(knots[jj].second - knots[jj + 1].second) > 1))
                    {
                        // fancy diagonal move, hot damn, this one got me
                        // this is the special case that doesn't show up the first go-around (requires a diagonal move)
                        knots[jj + 1].first += (knots[jj].first > knots[jj + 1].first) ? 1 : -1;
                        knots[jj + 1].second += (knots[jj].second > knots[jj + 1].second) ? 1 : -1;
                    }
                    else if (std::abs(knots[jj].first - knots[jj + 1].first) > 1)
                    {
                        knots[jj + 1].first += (knots[jj].first > knots[jj + 1].first) ? 1 : -1;
                        knots[jj + 1].second = knots[jj].second;
                    }
                    else if (std::abs(knots[jj].second - knots[jj + 1].second) > 1)
                    {
                        knots[jj + 1].first = knots[jj].first;
                        knots[jj + 1].second += (knots[jj].second > knots[jj + 1].second) ? 1 : -1;
                    }
                    else
                    {
                        cout << "ruh roh" << endl;
                    }

                    //  only update for the tail node
                    if (jj == 8)
                    {

                        trail[{knots[jj + 1].first, knots[jj + 1].second}] = true;
                    }
                }
            }
        }
        // print_grid(knots);
        // cout << endl;
    }
    return trail.size();
}

int main(int argc, char **argv)
{
    if (argc == 1)
    {
        std::cout << "Need to pass a filename\n";
        return -1;
    }

    cout << "Day 9 Part 1 " << part_one(argv[1]) << endl;
    cout << "Day 9 Part 2 " << endl;
    cout << part_two(argv[1]) << endl;

    return 1;
}
