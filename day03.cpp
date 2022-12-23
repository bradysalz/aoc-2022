#include <algorithm>
#include <istream>
#include <iostream>
#include <iterator>
#include <fstream>
#include <functional>
#include <map>
#include <ostream>
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

int letterPriority(char letter)
{
    int ascii_val = int(letter);
    if (ascii_val > 96)
    {
        // a == 97, convert to a == 1
        ascii_val -= 96;
    }
    else
    {
        // A == 65, cover to A == 27
        ascii_val -= 64;
        ascii_val += 26;
    }

    return ascii_val;
}

vector<char> string_to_char_vec(string in)
{
    vector<char> out(in.begin(), in.end());
    std::sort(out.begin(), out.end());
    return out;
}

int findOverlap(const char *fname)
{
    std::ifstream input(fname);
    int score = 0;
    for (string line; getline(input, line);)
    {
        string left = line.substr(0, line.length() / 2);
        string right = line.substr(line.length() / 2);
        vector<char> left_vec = string_to_char_vec(left);
        vector<char> right_vec = string_to_char_vec(right);

        vector<char> overlap(26); // I"m lazy, no idea how long this could be
        std::set_intersection(left_vec.begin(),
                              left_vec.end(),
                              right_vec.begin(),
                              right_vec.end(),
                              overlap.begin());
        // int ascii_val = int(overlap[0]);
        score += letterPriority(int(overlap[0]));
    }
    return score;
}

int find_common_letter(const char *fname)
{
    std::ifstream input(fname);
    int score = 0;

    for (string line; getline(input, line);)
    {

        vector<char> first_line = string_to_char_vec(line);
        getline(input, line);
        vector<char> second_line = string_to_char_vec(line);
        getline(input, line);
        vector<char> third_line = string_to_char_vec(line);

        vector<char> overlap(26);
        vector<char> final(26);
        std::set_intersection(first_line.begin(),
                              first_line.end(),
                              second_line.begin(),
                              second_line.end(),
                              overlap.begin());
        std::sort(overlap.begin(), overlap.end());
        std::set_intersection(overlap.begin(),
                              overlap.end(),
                              third_line.begin(),
                              third_line.end(),
                              final.begin());
        score += letterPriority(int(final[0]));
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

    cout << "Day 3 Part 1 " << findOverlap(argv[1]) << "\n";
    cout << "Day 3 Part 2 " << find_common_letter(argv[1]) << "\n";
    return 1;
}