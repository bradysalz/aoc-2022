#include <algorithm>
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

int part_one(const char *fname)
{
    std::ifstream input(fname);

    int count = 1;
    int reg = 1;
    int strength = 0;
    vector<int> big_counts = {20, 60, 100, 140, 180, 220};

    for (string line; getline(input, line);)
    {
        if (std::count(big_counts.begin(), big_counts.end(), count))
            strength += count * reg;

        if (line.rfind("noop", 0) != 0) // parse addx
        {
            count++;
            if (std::count(big_counts.begin(), big_counts.end(), count))
                strength += count * reg;
            reg += std::stoi(line.substr(4));
        }
        count++;
    }
    return strength;
}

void print_char(int reg, int &count)
{
    if (count > 39)
    {
        cout << endl;
        count -= 40;
    }
    if ((count == (reg - 1)) || (count == (reg)) || (count == (reg + 1)))
        cout << "#";
    else
        cout << " ";
}
void part_two(const char *fname)
{
    std::ifstream input(fname);
    int reg = 1;
    int count = 0;
    print_char(reg, count);

    for (string line; getline(input, line);)
    {
        if (line.rfind("noop", 0) != 0) // parse addx
        {
            count++;
            print_char(reg, count);
            reg += std::stoi(line.substr(4));
        }
        count++;
        print_char(reg, count);
    }
}

int main(int argc, char **argv)
{
    if (argc == 1)
    {
        std::cout << "Need to pass a filename\n";
        return -1;
    }

    cout << "Day 10 Part 1 " << part_one(argv[1]) << endl;
    cout << "Day 10 Part 2" << endl;
    part_two(argv[1]);

    return 1;
}
