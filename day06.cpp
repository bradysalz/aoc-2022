#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <string>
#include <string.h>
#include <sstream>

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;

bool all_char_unique(std::vector<char> letters)
{
    std::set<char> my_set;
    for (char &c : letters)
    {
        auto ret = my_set.insert(c);
        if (ret.second == false)
            return false;
    }
    return true;
}

int part_one(const char *fname, bool is_part_one)
{
    int unique_len = is_part_one ? 4 : 14;
    std::ifstream input(fname);

    // be lazy and hard code
    std::vector<char> last_four;
    int char_found = 0;

    // parse starting stack
    for (string line; getline(input, line);)
    {
        for (string::size_type i = 0; i < line.size(); i++)
        {
            if (i < unique_len)
            {
                last_four.push_back(line[i]);
                continue;
            }
            last_four.erase(last_four.begin());
            last_four.push_back(line[i]);

            if (all_char_unique(last_four))
                return i + 1;
        }
    }
    return -1;
}

int main(int argc, char **argv)
{
    if (argc == 1)
    {
        std::cout << "Need to pass a filename\n";
        return -1;
    }

    cout << "Day 5 Part 1 " << part_one(argv[1], true) << endl;
    cout << "Day 5 Part 2 " << part_one(argv[1], false) << endl;

    return 1;
}
