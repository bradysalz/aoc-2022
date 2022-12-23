#include <string>
#include <string.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;

int part_one(const char *fname)
{
    std::ifstream input(fname);
    int overlap = 0;
    for (string(line); getline(input, line);)
    {
        int first = std::stoi(line.substr(0, line.find('-')));
        int second = std::stoi(line.substr(line.find('-') + 1, line.find(",")));
        line.erase(0, line.find(',') + 1);

        int third = std::stoi(line.substr(0, line.find('-')));
        int fourth = std::stoi(line.substr(line.find('-') + 1));

        if ((third <= first) && (second <= fourth))
        {
            overlap++;
            continue;
        }

        if ((first <= third) && (fourth <= second))
        {
            overlap++;
            continue;
        }
    }
    return overlap;
}

int part_two(const char *fname)
{
    std::ifstream input(fname);
    int overlap = 0;
    for (string(line); getline(input, line);)
    {
        int first = std::stoi(line.substr(0, line.find('-')));
        int second = std::stoi(line.substr(line.find('-') + 1, line.find(",")));
        line.erase(0, line.find(',') + 1);

        int third = std::stoi(line.substr(0, line.find('-')));
        int fourth = std::stoi(line.substr(line.find('-') + 1));

        if ((second < third) || (fourth < first))
            continue;
        overlap++;
    }
    return overlap;
}

int main(int argc, char **argv)
{
    if (argc == 1)
    {
        std::cout << "Need to pass a filename\n";
        return 1;
    }

    cout << "Day 4 Part 1 " << part_one(argv[1]) << "\n";
    cout << "Day 4 Part 2 " << part_two(argv[1]) << "\n";
    return 1;
}