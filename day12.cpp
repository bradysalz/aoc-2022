#include <algorithm>
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

class Graph
{
public:
    std::map<std::pair<int, int>, char> grid;
    std::map<std::pair<int, int>, int> cost;
    std::map<std::pair<int, int>, vector<std::pair<int, int>>> adj;
    std::pair<int, int> start;
    std::pair<int, int> end;
    int rowMax;
    int colMax;
};

void print_graph(Graph g)
{
    cout << endl;
    for (int row = 0; row < g.rowMax; row++)
    {
        for (int col = 0; col < g.colMax; col++)
        {
            cout << std::setfill(' ') << std::setw(3) << g.cost[{row, col}] << "-" << g.grid[{row, col}];
        }
        cout << endl;
    }
}

Graph build_graph(const char *fname)
{
    std::ifstream input(fname);
    Graph mygraph;

    int row = 0;
    for (string line; getline(input, line);)
    {
        for (string::size_type s = 0; s < line.size(); s++)
        {
            mygraph.grid[{row, s}] = line[s];
            if (line[s] == 'S')
            {
                mygraph.start = {row, s};
                mygraph.grid[{row, s}] = 'a';
            }
            if (line[s] == 'E')
            {
                mygraph.end = {row, s};
                mygraph.grid[{row, s}] = 'z';
            }
        }
        mygraph.colMax = line.size();
        row++;
    }
    mygraph.rowMax = row;

    // init all cost to max
    for (auto it : mygraph.grid)
    {
        mygraph.cost[it.first] = mygraph.rowMax * mygraph.colMax;
    }

    // build adjacency matrix
    for (int row = 0; row < mygraph.rowMax; row++)
    {
        for (int col = 0; col < mygraph.colMax; col++)
        {
            // up
            if (row > 0 && mygraph.grid[{row - 1, col}] - mygraph.grid[{row, col}] < 2)
            {
                mygraph.adj[{row, col}].push_back({row - 1, col});
            }

            // down
            if (row < mygraph.rowMax - 1 && mygraph.grid[{row + 1, col}] - mygraph.grid[{row, col}] < 2)
            {
                mygraph.adj[{row, col}].push_back({row + 1, col});
            }

            // left
            if (col > 0 && mygraph.grid[{row, col - 1}] - mygraph.grid[{row, col}] < 2)
            {
                mygraph.adj[{row, col}].push_back({row, col - 1});
            }

            // right
            if (col < mygraph.colMax - 1 && mygraph.grid[{row, col + 1}] - mygraph.grid[{row, col}] < 2)
            {
                mygraph.adj[{row, col}].push_back({row, col + 1});
            }
        }
    }
    return mygraph;
}

void clear_costs(Graph &g)
{
    for (int row = 0; row < g.rowMax; row++)
    {
        for (int col = 0; col < g.colMax; col++)
        {
            g.cost[{row, col}] = g.rowMax * g.colMax;
        }
    }
}

int get_best_cost(Graph graph)
{
    // BFS
    std::map<std::pair<int, int>, bool> visited;
    std::queue<std::pair<int, int>> to_visit;

    graph.cost[graph.start] = 0;
    visited[graph.start] = true;
    to_visit.push(graph.start);

    while (!to_visit.empty())
    {
        auto node = to_visit.front();
        to_visit.pop();

        // found the endpoint
        if (node == graph.end)
            continue;

        // update weights and push if new
        for (auto adj : graph.adj[node])
        {
            if (!visited[adj])
            {
                graph.cost[adj] = graph.cost[node] + 1;
                to_visit.push(adj);
                visited[adj] = true;
            }
        }
    }
    return graph.cost[graph.end];
}

int part_two(const char *fname)
{
    vector<std::pair<int, int>> starts;
    Graph graph = build_graph(fname);

    for (int row = 0; row < graph.rowMax; row++)
    {
        for (int col = 0; col < graph.colMax; col++)
        {
            if (graph.grid[{row, col}] == 'a')
                starts.push_back({row, col});
        }
    }

    int min_cost = graph.rowMax * graph.colMax;
    for (auto start : starts)
    {
        graph.start = start;
        clear_costs(graph);
        int attempt = get_best_cost(graph);
        if (attempt < min_cost)
            min_cost = attempt;
    }
    return min_cost;
}

int part_one(const char *fname)
{
    Graph graph = build_graph(fname);
    return get_best_cost(graph);
}

int main(int argc, char **argv)
{
    if (argc == 1)
    {
        std::cout << "Need to pass a filename\n";
        return -1;
    }

    cout << "Day 12 Part 1 " << part_one(argv[1]) << endl;
    cout << "Day 11 Part 2 " << part_two(argv[1]) << endl;
    // cout << "Day 11 Part 2" << endl;
    // part_two(argv[1]);

    // 5576 too high

    return 1;
}
