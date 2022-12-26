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

class Tree
{
public:
    int height;
    bool visible;
    int score;
    Tree()
    {
        height = 0;
        visible = false;
        score = 1;
    };
    Tree(int h, bool v)
    {
        height = h;
        visible = v;
        score = 1;
    };
    Tree(const Tree &t)
    {
        height = t.height;
        visible = t.visible;
        score = t.score;
    };
};

void print_grid(vector<vector<Tree>> grid, bool use_height)
{
    cout << endl;
    for (auto row : grid)
    {
        for (auto tree : row)
        {
            cout << (use_height ? tree.score : tree.visible);
        }
        cout << endl;
    }
}

vector<vector<Tree>> build_grid(const char *fname)
{
    vector<vector<Tree>> grid;
    std::ifstream input(fname);

    for (string line; getline(input, line);)
    {
        vector<Tree> new_row;
        for (string::size_type i = 0; i < line.size(); i++)
        {
            // set the first and last element to visible
            new_row.push_back(Tree(int(line[i] - '0'), (i == 0)));
        }
        new_row.back().visible = true;
        grid.push_back(new_row);
    }

    // set first row to visile
    for (int i = 0; i < grid.front().size(); i++)
    {
        grid.front()[i].visible = true;
    }
    // set last row to visible
    for (int i = 0; i < grid.back().size(); i++)
    {
        grid.back()[i].visible = true;
    }
    return grid;
}

bool check_left(vector<vector<Tree>> grid, int row, int col)
{
    for (int ii = 0; ii < row; ii++)
    {
        if (grid[ii][col].height >= grid[row][col].height)
            return false;
    }
    return true;
}
bool check_right(vector<vector<Tree>> grid, int row, int col)
{
    for (int ii = grid.size() - 1; ii > row; ii--)
    {
        if (grid[ii][col].height >= grid[row][col].height)
            return false;
    }
    return true;
}
bool check_top(vector<vector<Tree>> grid, int row, int col)
{
    for (int ii = 0; ii < col; ii++)
    {
        if (grid[row][ii].height >= grid[row][col].height)
            return false;
    }
    return true;
}

bool check_bottom(vector<vector<Tree>> grid, int row, int col)
{
    for (int ii = grid[0].size() - 1; ii > col; ii--)
    {
        if (grid[row][ii].height >= grid[row][col].height)
            return false;
    }
    return true;
}

void set_visible_status(vector<vector<Tree>> &grid)
{
    for (int ii = 1; ii < grid.size() - 1; ii++)
    {
        for (int jj = 1; jj < grid[ii].size() - 1; jj++)
        {
            grid[ii][jj].visible |= check_left(grid, ii, jj);
            grid[ii][jj].visible |= check_right(grid, ii, jj);
            grid[ii][jj].visible |= check_top(grid, ii, jj);
            grid[ii][jj].visible |= check_bottom(grid, ii, jj);
        }
    }
}

int count_visible(const vector<vector<Tree>> grid)
{
    int count = 0;
    for (int ii = 0; ii < grid.size(); ii++)
    {
        for (int jj = 0; jj < grid[0].size(); jj++)
        {
            if (grid[ii][jj].visible)
                count++;
        }
    }
    return count;
}

int score_tree(vector<vector<Tree>> grid, int row, int col)
{
    int total = 1;

    // check left
    int count = 0;
    for (int ii = row - 1; ii >= 0; ii--)
    {
        count++;
        if (grid[ii][col].height >= grid[row][col].height)
            break;
    }
    total *= count;

    // check right
    count = 0;
    for (int ii = row + 1; ii < grid.size(); ii++)
    {
        count++;
        if (grid[ii][col].height >= grid[row][col].height)
            break;
    }
    total *= count;

    // check top
    count = 0;
    for (int ii = col - 1; ii >= 0; ii--)
    {
        count++;
        if (grid[row][ii].height >= grid[row][col].height)
            break;
    }
    total *= count;

    // check bottom
    count = 0;
    for (int ii = col + 1; ii < grid[0].size(); ii++)
    {
        count++;
        if (grid[row][ii].height >= grid[row][col].height)
            break;
    }
    total *= count;

    return total;
}

void set_scenic_score(vector<vector<Tree>> &grid)
{
    for (int ii = 1; ii < grid.size() - 1; ii++)
    {
        for (int jj = 1; jj < grid[ii].size() - 1; jj++)
        {
            grid[ii][jj].score = score_tree(grid, ii, jj);
        }
    }
}

int most_scenic(const vector<vector<Tree>> grid)
{
    int max = 0;
    for (int ii = 0; ii < grid.size(); ii++)
    {
        for (int jj = 0; jj < grid[0].size(); jj++)
        {
            if (grid[ii][jj].score > max)
                max = grid[ii][jj].score;
        }
    }
    return max;
}

int part_one(const char *fname)
{
    vector<vector<Tree>> grid = build_grid(fname);
    // print_grid(grid, true);

    set_visible_status(grid);
    return count_visible(grid);
}

int part_two(const char *fname)
{
    vector<vector<Tree>> grid = build_grid(fname);

    set_scenic_score(grid);
    print_grid(grid, true);
    return most_scenic(grid);
}

int main(int argc, char **argv)
{
    if (argc == 1)
    {
        std::cout << "Need to pass a filename\n";
        return -1;
    }

    cout << "Day 7 Part 1 " << part_one(argv[1]) << endl;
    cout << "Day 7 Part 2 " << part_two(argv[1]) << endl;

    return 1;
}
