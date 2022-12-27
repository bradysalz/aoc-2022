#include <algorithm>
#include <any>
#include <iomanip>
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

class Node
{
public:
    int val;
    vector<Node> vals;
    bool is_int;
    Node(int v)
    {
        val = v;
        is_int = true;
    }
    Node(vector<Node> vs)
    {
        vals = vs;
        is_int = false;
    }
};

vector<Node> parse(string &input)
{
    vector<Node> out;

    while (!input.empty())
    {
        char val = input[0];
        if (val == ',')
        {
            input.erase(0, 1);
        }
        else if (val == '[')
        {
            input.erase(0, 1);
            Node tmp(parse(input));
            out.push_back(tmp);
        }
        else if (val == ']')
        {
            input.erase(0, 1);
            break;
        }
        else
        {
            // got stuck forever due to a bug here...
            // only parsed "single-digit" ints, not vals > 9. oof.
            int idx = 1;
            while (input[idx] != ',' && input[idx] != ']')
                idx++;

            out.push_back(Node(std::stoi(input.substr(0, idx))));
            input.erase(0, idx);
        }
    }
    return out;
}

bool check_if_equal(vector<Node> first, vector<Node> second)
{
    if (first.size() != second.size())
        return false;

    for (int ii = 0; ii < first.size(); ii++)
    {
        if (first[ii].val != second[ii].val)
            return false;
    }
    return true;
}

int are_pairs_ordered(vector<Node> first, vector<Node> second)
{
    while (true)
    {
        // bit of a hack, but needed for when we have an int->vec promotion
        // in a promotion, you don't see the rest of the elements since we only pass head
        // down recurisvely. this happens if you have, 8 vs [8] or [8] vs 8
        if (first.empty() && second.empty())
            return -1;

        // left side ran out first, ordered
        if (first.empty())
            return 1;

        // right side ran out first, not ordered
        if (second.empty())
            return 0;

        Node left = first.front();
        Node right = second.front();
        first.erase(first.begin());
        second.erase(second.begin());

        if (left.is_int && right.is_int) // both ints
        {
            if (left.val < right.val)
                return 1;
            if (left.val > right.val)
                return 0;
            continue;
        }
        else if (!left.is_int && !right.is_int) // both vec
        {
            // if vectors are equal, move to next input
            if (check_if_equal(left.vals, right.vals))
                continue;
            int rval = are_pairs_ordered(left.vals, right.vals);
            if (rval == -1)
                continue;
            return rval;
        }
        else
        {
            vector<Node> new_vec;
            int rval;
            if (left.is_int)
            {
                new_vec = {Node{left.val}};
                if (check_if_equal(new_vec, right.vals))
                    continue;
                rval = are_pairs_ordered(new_vec, right.vals);
            }
            else
            {
                new_vec = {Node{right.val}};
                if (check_if_equal(left.vals, new_vec))
                    continue;
                rval = are_pairs_ordered(left.vals, new_vec);
            }
            if (rval == -1)
                continue;
            return rval;
        }
    }
    return 1;
}

void print_node(vector<Node> in)
{
    for (int ii = 0; ii < in.size(); ii++)
    {
        if (in[ii].is_int)
        {
            cout << in[ii].val;
        }
        else
        {
            cout << "[";
            print_node(in[ii].vals);
            cout << "]";
        }
        if (ii < in.size() - 1)
            cout << ",";
    }
}

int part_one(const char *fname)
{
    std::ifstream input(fname);
    string first_val;
    string second_val;

    int ordered = 0;
    int pair_count = 1;
    for (; getline(input, first_val);)
    {
        getline(input, second_val);

        vector<Node> first = parse(first_val);
        vector<Node> second = parse(second_val);
        getline(input, second_val); // newline

        int increment = are_pairs_ordered(first, second);
        print_node(first);
        cout << endl;
        print_node(second);
        cout << endl;
        if (increment)
        {
            ordered += pair_count;
            cout << "ordered" << endl
                 << endl;
        }
        else
        {
            cout << "not ordered" << endl
                 << endl;
        }

        pair_count++;
    }
    return ordered;
}

int part_two(const char *fname)
{
    std::ifstream input(fname);
    vector<vector<Node>> items;

    for (string(line); getline(input, line);)
    {
        if (line.empty())
            continue;
        items.push_back(parse(line));
    }

    // add special keys
    string special = "[[2]]";
    vector<Node> key = parse(special);
    int final_location = 1;

    // (PLS DON'T READ THIS RECRUITERS)
    // i couldn't get bubble sort working because I was somehow returning -1
    // from my comp function. it's possible to return -1 but never did in part 1
    // so i got kinda stumped. tried to implement another reddit solution (don't sort,
    // just count the number greater) and woohoo it worked. yolo
    for (auto item : items)
    {
        if (are_pairs_ordered(item, key))
        {
            final_location++;
        }
    }

    special = "[[6]]";
    key = parse(special);
    int final_location2 = 2;
    for (auto item : items)
    {
        if (are_pairs_ordered(item, key))
        {
            final_location2++;
        }
    }

    return final_location * final_location2;
}

int main(int argc, char **argv)
{
    if (argc == 1)
    {
        std::cout << "Need to pass a filename\n";
        return -1;
    }

    cout << "Day 13 Part 1 "
         << part_one(argv[1]) << endl;
    cout << "Day 13 Part 2 " << endl
         << part_two(argv[1]) << endl;

    return 1;
}
