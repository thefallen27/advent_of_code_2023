#include <array>
#include <fstream>
#include <functional>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

template <> struct std::hash<std::array<size_t, 2>>
{
    static constexpr size_t rgr = 0x9E3779B97F6815; // reciprocal of the golder ratio
    size_t operator()(const std::array<size_t, 2>& s) const noexcept
    {
        size_t first_hash = std::hash<size_t>{}(s[0]);
        size_t second_hash = std::hash<size_t>{}(s[1]);

        return first_hash ^ (second_hash + rgr + (first_hash << 6) + (first_hash >> 2));
    }
};

static std::array<size_t, 2>
AnalyseMap(std::ifstream& input_file, std::vector<std::string>& map)
{
    std::string line;
    std::array<size_t, 2> position{};
    size_t row{ 0 };
    
    while (std::getline(input_file, line))
    {
        map.push_back(line);
        size_t starting_column{ line.find('S') };
        if (starting_column != std::string::npos)
        {
            position = { row, starting_column };
            map[row][starting_column] = '.';
        }

        ++row;
    }

    return position;
}

static void
UpdatePosition(std::unordered_set<std::array<size_t, 2>>& start_positions,
                const std::vector<std::string>& map)
{
    std::unordered_set<std::array<size_t, 2>> next_positions{};
    const std::array<std::pair<int, int>, 4> directions = { {{-1, 0}, {1, 0}, {0, -1}, {0, 1}} };

    for (const auto& position : start_positions)
    {
        for (const auto& [dx, dy] : directions)
        {
            size_t new_x = position[0] + dx;
            size_t new_y = position[1] + dy;

            if (new_x >= 0 && new_x < map.size() && new_y >= 0 && new_y < map[new_x].size())
            {
                if (map[new_x][new_y] != '#')
                {
                    next_positions.insert({ new_x, new_y });
                }
            }
        }
    }

    start_positions.swap(next_positions);
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::ifstream input_file("garden_plots.txt");
    if (!input_file.is_open())
    {
        std::cerr << "File cannot be opened!" << std::endl;
        return 1;
    }

    std::vector<std::string> map;
    std::array<size_t, 2> start_position = AnalyseMap(input_file, map);
    std::unordered_set<std::array<size_t, 2>> start_positions{ start_position };

    int steps = 64;
    while (steps--)
    {
        UpdatePosition(start_positions, map);
    }

    std::cout << "The number of plots that can be reached in 64 steps is " 
              << start_positions.size() << std::endl;

    return 0;
}
