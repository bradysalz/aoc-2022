// LOG
// got stuck on rows vs columns again :sob:
// got part 1 input done
// realized that hashing into dicts is terrible with massive int performance (1e7++ keys)
// think i found the smarter way (checking circle sizes)
// 5694886 too high: yolo'd new try
// 3831375 too low: added checking for beacons locations
// 3831375 too low: added <expand g.MIN/MAX by dist> to increase row search space. had no effect, interesting.
// 5181556 lmfao i used the wrong row
// PART TWO TIME
// just gonna yolo multithread it to see haha

#include <cmath>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <map>
#include <queue>
#include <thread>
#include <string>
#include <string.h>
#include <sstream>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;

struct solution
{
    bool found = false;
    std::pair<int, int> point;
};

class Grid
{
public:
    vector<std::pair<int, int>> sensors;
    vector<std::pair<int, int>> beacons;
    vector<int> distances;

    long MIN_Y = INT64_MAX - 1;
    long MIN_X = INT64_MAX - 1;
    long MAX_Y = INT64_MIN;
    long MAX_X = INT64_MIN;
};

void maybe_update_limits(Grid &g, std::pair<int, int> point)
{
    if (point.first < g.MIN_X)
        g.MIN_X = point.first;
    if (point.first > g.MAX_X)
        g.MAX_X = point.first;
    if (point.second < g.MIN_Y)
        g.MIN_Y = point.second;
    if (point.second > g.MAX_Y)
        g.MAX_Y = point.second;
}

void parse_line(Grid &g, string line)
{
    // input looks like:
    // Sensor at x=2, y=18: closest beacon is at x=-2, y=15
    // parse to <2,18>, <-2, 15>
    g.sensors.push_back({std::stoi(line.substr(line.find("x=") + 2, line.find(","))),
                         std::stoi(line.substr(line.find("y=") + 2, line.find(":")))});
    line.erase(0, line.find(":"));

    g.beacons.push_back({std::stoi(line.substr(line.find("x=") + 2, line.find(","))),
                         std::stoi(line.substr(line.find("y=") + 2))});

    maybe_update_limits(g, g.sensors.back());
    maybe_update_limits(g, g.beacons.back());
}

void calculate_distance(Grid &g)
{
    for (int ii = 0; ii < g.sensors.size(); ii++)
    {
        int dist = std::abs(g.sensors[ii].first - g.beacons[ii].first) + std::abs(g.sensors[ii].second - g.beacons[ii].second);
        g.distances.push_back(dist);

        // could be in so many more places :(
        maybe_update_limits(g, {g.sensors[ii].first - dist, g.sensors[ii].second - dist});
        maybe_update_limits(g, {g.sensors[ii].first + dist, g.sensors[ii].second + dist});
    }
}

int count_known_sites(Grid g, int y_fixed)
{
    int count = 0;
    for (int ii = g.MIN_X; ii < g.MAX_X; ii++)
    {
        std::pair<int, int> target = {ii, y_fixed};
        for (int jj = 0; jj < g.sensors.size(); jj++)
        {
            auto sensor = g.sensors[jj];
            auto dist = g.distances[jj];

            // this is dumb
            bool skip_check = false;
            for (auto beacon : g.beacons)
            {
                if (beacon.first == target.first && beacon.second == target.second)
                {
                    skip_check = true;
                    break;
                }
            }
            if (skip_check)
                continue;

            if ((std::abs(target.first - sensor.first) + std::abs(target.second - sensor.second)) <= dist)
            {
                // cout << "Yes: " << target.first << ", " << target.second << endl;
                count++;
                break;
            }
        }
    }
    return count;
}

int part_one(const char *fname)
{
    std::ifstream input(fname);

    Grid g;

    for (string line; getline(input, line);)
        parse_line(g, line);

    calculate_distance(g);
    int count = count_known_sites(g, 2000000);
    return count;
}

void check_if_solution(Grid g, std::pair<int, int> start, std::pair<int, int> end, bool &found, solution &sol)
{
    for (int ii = start.first; ii < end.first; ii++)
    {
        for (int jj = start.second; jj < end.second; jj++)
        {
            bool is_solution = true;
            std::pair<int, int> target = {ii, jj};
            for (int kk = 0; kk < g.sensors.size(); kk++)
            {
                auto sensor = g.sensors[kk];
                auto dist = g.distances[kk];

                // this is dumb
                bool skip_check = false;
                for (auto beacon : g.beacons)
                {
                    if (beacon.first == target.first && beacon.second == target.second)
                    {
                        is_solution = false;
                        break;
                    }
                }
                if (!is_solution)
                    break;

                if ((std::abs(target.first - sensor.first) + std::abs(target.second - sensor.second)) <= dist)
                {
                    is_solution = false;
                    break;
                }

                if (found)
                    return;
            }

            if (is_solution)
            {
                found = true;
                sol.found = true;
                sol.point = target;
                return;
            }
        }
    }
}

int part_two(const char *fname)
{
    std::ifstream input(fname);
    Grid g;
    for (string line; getline(input, line);)
        parse_line(g, line);

    // Y O L O
    vector<std::thread> my_threads;
    bool found = false;
    solution tmp;
    for (int ii = 0; ii < 4; ii++)
    {

        std::thread t1(check_if_solution,
                       g,
                       std::pair<int, int>{0, (ii + 1) * 1000000},
                       std::pair<int, int>{4000000, (ii + 1) * 1000000},
                       found,
                       tmp);
        my_threads.push_back(t1);
    }

    // for (int ii = 0; ii < my_threads.size(); ii++)
    // my_threads[ii].join();
    cout << "got em" << endl;
    return tmp.point.first;
}

int main(int argc, char **argv)
{
    if (argc == 1)
    {
        std::cout << "Need to pass a filename\n";
        return -1;
    }

    cout << "Day 15 Part 1 " << endl
         << part_one(argv[1]) << endl;
    cout << "Day 15 Part 2 " << endl
         << part_two(argv[1]) << endl;

    return 1;
}
