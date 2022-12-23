#include <algorithm>
#include <iostream>
#include <fstream>
#include <functional>
#include <string>
#include <sstream>
#include <vector>

using std::string;

std::vector<int> turnFileToCalories(const char *fname)
{
    std::vector<int> calories;
    int curr_sum = 0;

    std::ifstream input(fname);
    for (string line; getline(input, line);)
    {
        if (line.empty())
        {
            calories.push_back(curr_sum);
            curr_sum = 0;
        }
        else
        {
            curr_sum += stoi(line);
        }
    }
    return calories;
}

int main(int argc, char **argv)
{
    if (argc == 1)
    {
        std::cout << "Need to pass a filename\n";
        return 1;
    }

    std::vector<int> calories = turnFileToCalories(argv[1]);
    int max = *std::max_element(calories.begin(), calories.end());
    std::cout << "Day 1 Part 1 " << max << "\n";

    // Part two - find top 3 items
    std::sort(calories.begin(), calories.end(), std::greater<int>());
    std::cout << "Day 1 Part 2 " << calories[0] + calories[1] + calories[2] << "\n";
    return 0;
}