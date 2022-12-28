#include <algorithm>
#include <any>
#include <iostream>
#include <fstream>
#include <map>
#include <queue>
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

enum material
{
    SAND = 1,
    ROCK = 2,
};

class Grid
{
public:
    int MIN_X = 500;
    int MAX_X = INT32_MIN;
    int MIN_Y = 0; // sand starts at (500, 0)
    int MAX_Y = INT32_MIN;

    std::map<std::pair<int, int>, material> occupied;
};

void print_grid(Grid g)
{
    for (int yy = g.MIN_Y; yy <= g.MAX_Y; yy++)
    {
        for (int xx = g.MIN_X; xx <= g.MAX_X; xx++)
        {
            if (xx == 500 && yy == 0)
                cout << "+";
            else if (g.occupied.count({xx, yy}) && g.occupied[{xx, yy}] == material::ROCK)
                cout << "#";
            else if (g.occupied.count({xx, yy}) && g.occupied[{xx, yy}] == material::SAND)
                cout << "o";
            else
                cout << ".";
        }
        cout << endl;
    }
}

void check_and_update_limits(Grid &g, std::pair<int, int> head, std::pair<int, int> tail)
{
    if (head.first < g.MIN_X)
        g.MIN_X = head.first;
    if (head.first > g.MAX_X)
        g.MAX_X = head.first;
    if (tail.first < g.MIN_X)
        g.MIN_X = tail.first;
    if (tail.first > g.MAX_X)
        g.MAX_X = tail.first;

    if (head.second < g.MIN_Y)
        g.MIN_Y = head.second;
    if (head.second > g.MAX_Y)
        g.MAX_Y = head.second;
    if (tail.second < g.MIN_Y)
        g.MIN_Y = tail.second;
    if (tail.second > g.MAX_Y)
        g.MAX_Y = tail.second;
}

void draw_line(Grid &g, std::pair<int, int> head, std::pair<int, int> tail)
{
    if (head.first == tail.first) // update y
    {
        int incr = head.second > tail.second ? -1 : 1;
        for (int jj = head.second; jj != tail.second; jj += incr)
            g.occupied[{head.first, jj}] = material::ROCK;
    }
    else
    {
        int incr = head.first > tail.first ? -1 : 1;
        for (int jj = head.first; jj != tail.first; jj += incr)
            g.occupied[{jj, head.second}] = material::ROCK;
    }
    g.occupied[{tail.first, tail.second}] = material::ROCK;
}

void parse_rock_line(Grid &g, string line)
{
    // x,y -> a,b -> c,d
    int idx;
    string point;
    std::pair<int, int> head;
    std::pair<int, int> tail;

    while (!line.empty())
    {
        idx = line.find('-');
        if (idx == string::npos)
            break;
        point = line.substr(0, idx);
        head.first = std::stoi(point.substr(0, line.find(',')));
        head.second = std::stoi(point.substr(line.find(',') + 1));
        line.erase(0, idx + 3);

        tail.first = std::stoi(line.substr(0, line.find(',')));
        tail.second = std::stoi(line.substr(line.find(',') + 1, line.find(' ')));

        check_and_update_limits(g, head, tail);
        draw_line(g, head, tail);
    }
}

std::pair<int, int> propagate_sand(Grid &g, std::pair<int, int> start, bool is_part_two = false)
{
    std::pair<int, int> out;

    // check to extend floor as needed for part two
    if (is_part_two && (start.first == g.MIN_X || start.first == g.MAX_X))
    {
        int range = g.MAX_X - g.MIN_X;
        g.MAX_X += range;
        g.MIN_X -= range;
        draw_line(g, {g.MIN_X, g.MAX_Y}, {g.MAX_X, g.MAX_Y});
        return propagate_sand(g, start, is_part_two);
    }

    if (start.second == g.MAX_Y) // fall down forever?
    {
        return start;
    }
    else if ( // fall down?
        start.second < g.MAX_Y &&
        g.occupied[{start.first, start.second + 1}] < material::SAND)
    {
        out = {start.first, start.second + 1};
        return propagate_sand(g, out, is_part_two);
    }
    else if ( // fall left?
        start.first >= g.MIN_X &&
        start.second < g.MAX_Y &&
        g.occupied[{start.first - 1, start.second + 1}] < material::SAND)
    {
        out = {start.first - 1, start.second + 1};
        return propagate_sand(g, out, is_part_two);
    }
    else if ( // fall right?
        start.first <= g.MAX_X &&
        start.second < g.MAX_Y &&
        g.occupied[{start.first + 1, start.second + 1}] < material::SAND)
    {
        out = {start.first + 1, start.second + 1};
        return propagate_sand(g, out, is_part_two);
    }
    return start;
}

bool add_and_place_sand(Grid &g, bool is_part_two = false)
{
    std::pair<int, int> start = {500, 0};

    auto endpoint = propagate_sand(g, start, is_part_two);
    g.occupied[endpoint] = material::SAND;

    if (start == endpoint)
        return false;
    if (endpoint.second == g.MAX_Y)
        return false;

    return true;
}

int part_one(const char *fname)
{
    std::ifstream input(fname);
    string line;
    Grid g;

    for (; getline(input, line);)
    {
        parse_rock_line(g, line);
    }

    // print_grid(g);
    int sand_grains = 0;
    bool more_sand = true;
    while (more_sand)
    {
        sand_grains++;

        // cout << endl
        //      << "GRAINS OF SAND " << sand_grains << endl;
        more_sand = add_and_place_sand(g);
        // print_grid(g);
        // cout << endl;
    }
    return sand_grains - 1;
}

int part_two(const char *fname)
{
    std::ifstream input(fname);
    string line;
    Grid g;

    for (; getline(input, line);)
    {
        parse_rock_line(g, line);
    }
    string ymax = std::to_string(g.MAX_Y + 2);
    line = std::to_string(g.MIN_X) + "," + ymax + " -> " + std::to_string(g.MAX_X) + "," + ymax;
    parse_rock_line(g, line);

    int sand_grains = 0;
    bool more_sand = true;
    while (more_sand)
    {
        sand_grains++;
        more_sand = add_and_place_sand(g, true);
    }
    return sand_grains;
}

int main(int argc, char **argv)
{
    if (argc == 1)
    {
        std::cout << "Need to pass a filename\n";
        return -1;
    }

    cout << "Day 14 Part 1 " << endl
         << part_one(argv[1]) << endl;
    cout << "Day 14 Part 2 " << endl
         << part_two(argv[1]) << endl;

    return 1;
}
