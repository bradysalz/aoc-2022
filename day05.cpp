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

const int STACK_LENGTH = 10;

void part_one(const char *fname, bool is_part_one)
{
    std::ifstream input(fname);

    // be lazy and hard code
    std::vector<char> stacks[STACK_LENGTH];
    string line;
    bool input_done = false;

    // parse starting stack
    for (; getline(input, line);)
    {
        if (input_done)
            break;
        for (std::string::size_type i = 0; i < line.size(); i += 4)
        {
            char val = line[i + 1];
            if (val == ' ')
            {
                continue;
            }
            else if (val == '1')
            {
                // hit the numeric line, bail out
                input_done = true;
                break;
            }
            else
            {
                stacks[i / 4].insert(stacks[i / 4].begin(), val);
            }
        }
    }

    // parse movements
    for (; getline(input, line);)
    {
        int count = std::stoi(line.substr(5, line.find("from")));
        // one indexing is bad this problem is bad
        int source = std::stoi(line.substr(line.find("from") + 4, line.find("to"))) - 1;
        int dest = std::stoi(line.substr(line.find("to") + 2)) - 1;

        if (is_part_one)
        {
            for (int i = 0; i < count; ++i)
            {

                char val = stacks[source].back();
                stacks[source].pop_back();
                stacks[dest].push_back(val);
            }
        }
        else
        {
            for (int i = (count - 1); i >= 0; --i)
            {
                int idx = stacks[source].size() - i - 1;
                char val = stacks[source].at(
                    idx);
                stacks[dest].push_back(val);
            }
            stacks[source].erase(
                stacks[source].end() - count,
                stacks[source].end());
        }
    }

    for (int i = 0; i < STACK_LENGTH; i++)
    {
        if (!stacks[i].empty())
            cout << stacks[i].back();
    }
    cout << endl;
    return;
}

int part_two(const char *fname)
{
    return 1;
}

int main(int argc, char **argv)
{
    if (argc == 1)
    {
        std::cout << "Need to pass a filename\n";
        return 1;
    }

    cout << "Day 5 Part 1 ";
    part_one(argv[1], true);

    cout << "Day 5 Part 2 ";
    part_one(argv[1], false);
    return 1;
}