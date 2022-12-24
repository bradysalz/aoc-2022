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

class File
{
public:
    string name;
    int size;
    File(string name, int size);
};

File::File(string my_name, int my_size)
{
    name = my_name;
    size = my_size;
}

class Directory
{
public:
    string name;
    int size;  // recursively found!
    int depth; // what level of the tre
    std::map<string, Directory> dirs;
    vector<File> files;
    Directory();
    Directory(string name, int depth);
};

Directory::Directory() {}
Directory::Directory(string my_name, int my_depth)
{
    name = my_name;
    depth = my_depth;
    size = 0;
}

void print_tree(Directory curr_root)
{
    string prefix;
    for (int i = 0; i < curr_root.depth; i++)
        prefix += "    ";
    prefix += " - ";

    cout << prefix << curr_root.name << " " << curr_root.size << endl;

    for (auto file : curr_root.files)
        cout << "    " << prefix << file.name << " " << file.size << endl;

    for (auto it : curr_root.dirs)
        print_tree(it.second);
}

int calc_max(Directory curr_root)
{
    int max = 0;
    for (auto it : curr_root.dirs)
    {
        max += calc_max(it.second);
    }
    if (curr_root.size > 100000)
        return max;
    else
        return max + curr_root.size;
}

int update_sizes(Directory &curr_root)
{
    int curr_size = 0;
    for (auto file : curr_root.files)
        curr_size += file.size;

    for (auto it : curr_root.dirs)
    {
        curr_root.dirs[it.first].size = update_sizes(curr_root.dirs[it.first]);
        curr_size += curr_root.dirs[it.first].size;
    }

    return curr_size;
}

void build_tree(std::ifstream &input, Directory &curr_root)
{
    for (string line; getline(input, line);)
    {
        if (line.rfind("$ cd ..", 0) == 0) // done with this node
        {
            return;
        }
        else if (line.rfind("$ cd", 0) == 0) // entering a folder
        {
            string dir_name = line.substr(5); // 5 = "$ cd "
            build_tree(input, curr_root.dirs[dir_name]);
        }
        else if (line.rfind("$ ls", 0) == 0) // nothing
        {
            continue;
        }
        else if (line.rfind("dir", 0) == 0) // folder
        {
            string dir_name = line.substr(4);
            curr_root.dirs[dir_name] = Directory(dir_name, curr_root.depth + 1);
        }
        else // file
        {
            int idx = line.find(' ');
            int size = std::stoi(line.substr(0, idx));
            string name = line.substr(idx + 1);
            curr_root.files.push_back(File(name, size));
        }
    }
    return;
}

int part_one(const char *fname)
{
    std::ifstream input(fname);
    string line;
    getline(input, line);
    Directory root("/", 0);

    build_tree(input, root);
    root.size = update_sizes(root);
    return calc_max(root);
}

int find_minimum_dir(Directory root, int curr_best, int req_size)
{
    int best_size = curr_best;

    // if we're not big enough, no children will be big enough
    if (root.size < req_size)
        return curr_best;

    if (root.size < curr_best)
        best_size = root.size;

    for (auto dir : root.dirs)
    {
        int best_child = find_minimum_dir(dir.second, best_size, req_size);
        if (best_child < best_size)
            best_size = best_child;
    }
    return best_size;
}

int part_two(const char *fname)
{
    std::ifstream input(fname);
    string line;
    getline(input, line);
    Directory root("/", 0);

    build_tree(input, root);
    root.size = update_sizes(root);

    int to_remove = root.size - 40000000;
    return find_minimum_dir(root, root.size, to_remove);
}
int main(int argc, char **argv)
{
    if (argc == 1)
    {
        std::cout << "Need to pass a filename\n";
        return -1;
    }

    cout << "Day 7 Part 1 " << endl;
    cout << part_one(argv[1]) << endl;
    cout << "Day 7 Part 2 " << part_two(argv[1]) << endl;

    return 1;
}
