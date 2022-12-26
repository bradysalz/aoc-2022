#include <algorithm>
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

class Monkey
{
public:
    std::vector<long> items;
    string operation;
    int test_factor;
    int test_pass;
    int test_fail;
    int inspections;
    Monkey()
    {
        inspections = 0;
    };
};

void print_round(vector<Monkey> monkeys, int round)
{
    cout << endl
         << "After round " << round + 1 << endl;
    for (int mm = 0; mm < monkeys.size(); mm++)
    {
        cout << "Monkey " << mm << ": ";
        for (auto it : monkeys[mm].items)
        {
            cout << it << ", ";
        }
        cout << endl;
    }
}

vector<Monkey> parse_monkeys(const char *fname)
{
    std::ifstream input(fname);
    vector<Monkey> monkeys;

    for (string line; getline(input, line);)
    {
        Monkey new_guy;

        // starting
        getline(input, line);
        line.erase(0, line.find(":") + 1);
        std::istringstream ss(line);
        string token;
        while (getline(ss, token, ','))
        {
            new_guy.items.push_back(std::stoi(token));
        }

        // operation
        getline(input, line);
        line.erase(0, line.find("= ") + 2);
        new_guy.operation = line;

        // test
        getline(input, line);
        new_guy.test_factor = std::stoi(line.substr(line.find("by ") + 3));

        // pass/fail
        getline(input, line);
        new_guy.test_pass = std::stoi(line.substr(line.find("key ") + 4));
        getline(input, line);
        new_guy.test_fail = std::stoi(line.substr(line.find("key ") + 4));

        // newline
        getline(input, line);

        monkeys.push_back(new_guy);
    }
    return monkeys;
}

long long parse_op(Monkey monk, int worry)
{
    bool do_add = (monk.operation.find("+") != string::npos);
    string op = do_add ? "+" : "*";

    string arg = monk.operation.substr(0, monk.operation.find(op) - 1);
    int left_val = arg == "old" ? worry : std::stoi(arg);

    arg = monk.operation.substr(monk.operation.find(op) + 2);
    int right_val = arg == "old" ? worry : std::stoi(arg);

    long long to_return = do_add ? (left_val + right_val) : ((long long)left_val * (long long)right_val);

    return to_return;
}

long part_one(const char *fname, bool is_part_one)
{
    vector<Monkey> monkeys = parse_monkeys(fname);

    // this had me totally stumped, had to reddit it :(
    int mod_all = 1;
    for (auto it : monkeys)
        mod_all *= it.test_factor;

    int NUM_ROUNDS = is_part_one ? 4 : 10000;
    for (int ii = 0; ii < NUM_ROUNDS; ii++)
    {
        for (int mm = 0; mm < monkeys.size(); mm++)
        {
            while (!monkeys[mm].items.empty())
            {

                long old_val = monkeys[mm].items.front();
                monkeys[mm].items.erase(monkeys[mm].items.begin());
                monkeys[mm].inspections++;

                long long worry = parse_op(monkeys[mm], old_val);
                if (is_part_one)
                    worry /= 3;
                else
                    worry %= mod_all;

                if (worry % monkeys[mm].test_factor == 0)
                    monkeys[monkeys[mm].test_pass].items.push_back(worry);
                else
                    monkeys[monkeys[mm].test_fail].items.push_back(worry);
            }
        }
        // print_round(monkeys, ii);
    }

    vector<long> inspects;
    for (auto it : monkeys)
        inspects.push_back(it.inspections);
    std::sort(inspects.begin(), inspects.end());
    return inspects[inspects.size() - 1] * inspects[inspects.size() - 2];
}

int main(int argc, char **argv)
{
    if (argc == 1)
    {
        std::cout << "Need to pass a filename\n";
        return -1;
    }

    // cout << "Day 11 Part 1 " << part_one(argv[1], true) << endl;
    cout << "Day 11 Part 2 " << part_one(argv[1], false) << endl;
    // cout << "Day 11 Part 2" << endl;
    // part_two(argv[1]);

    return 1;
}
