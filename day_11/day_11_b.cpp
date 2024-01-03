#include <cmath>
#include <fstream>
#include <iostream>
#include <optional>
#include <set>
#include <string>
#include <vector>

int main()
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::ifstream input_file("space.txt");
    std::vector<std::string> input_scheme;

    if (!input_file.is_open())
    {
        std::cerr << "File cannot be opened!" << std::endl;
        return 1;
    }

    std::string line;
    std::vector<std::vector<std::optional<std::pair<int64_t, int64_t>>>> space;

    auto y = 0;
    while (std::getline(input_file, line))
    {
        std::vector<std::optional<std::pair<int64_t, int64_t>>> row;
        for (auto x = 0; x < line.length(); ++x)
        {
            if (line[x] == '#')
            {
                row.push_back(std::make_pair(x, y));
            }
            else
            {
                row.push_back({});
            }
        }

        space.push_back(row);
        ++y;
    }

    int64_t empty_lines = 0;
    for (int y = 0; y < space.size(); ++y)
    {
        std::set<std::optional<std::pair<int, int>>> line_set(space[y].begin(), space[y].end());
        if (line_set.size() > 1)
        {
            for (auto& s : space[y])
            {
                if (s)
                {
                    s = std::make_pair(s->first, s->second + empty_lines * 999999);
                }
            }
        }
        else
        {
            ++empty_lines;
        }
    }

    empty_lines = 0;
    for (int x = 0; x < space[0].size(); ++x)
    {
        std::vector<std::optional<std::pair<int, int>>> column;
        for (int y = 0; y < space.size(); ++y)
        {
            column.push_back(space[y][x]);
        }

        std::set<std::optional<std::pair<int, int>>> column_set(column.begin(), column.end());
        if (column_set.size() > 1)
        {
            for (int y = 0; y < space.size(); ++y)
            {
                if (space[y][x])
                {
                    space[y][x] = std::make_pair(space[y][x]->first + empty_lines * 999999,
                                                 space[y][x]->second);
                }
            }
        }
        else
        {
            ++empty_lines;
        }
    }

    std::vector<std::pair<int64_t, int64_t>> galaxies;
    for (auto& row : space)
    {
        for (auto& g : row)
        {
            if (g)
            {
                galaxies.push_back(*g);
            }
        }
    }

    int64_t sum_path_lengths = 0;
    for (size_t i = 0; i < galaxies.size(); ++i)
    {
        for (size_t j = i + 1; j < galaxies.size(); ++j)
        {
            sum_path_lengths += std::abs(galaxies[i].first - galaxies[j].first) + 
                                std::abs(galaxies[i].second - galaxies[j].second);
        }
    }

    std::cout << "The sum of all path lengths is " << sum_path_lengths << std::endl;

    return 0;
}
