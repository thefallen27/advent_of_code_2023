#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

struct Beam
{
    int x, y, x_direction, y_direction;

    bool operator < (const Beam& other) const
    {
        return std::tie(x, y, x_direction, y_direction) < std::tie(other.x, other.y, other.x_direction, other.y_direction);
    }
};

static std::vector<Beam>
Update(int x, int y, int x_direction, int y_direction, char beam)
{
    switch (beam)
    {
    case '|':
        return y_direction == 0 ? std::vector<Beam>{{x, y, 0, -1}, { x, y, 0, 1 }} : std::vector<Beam>{ {x, y, x_direction, y_direction} };
    case '-':
        return x_direction == 0 ? std::vector<Beam>{{x, y, -1, 0}, { x, y, 1, 0 }} : std::vector<Beam>{ {x, y, x_direction, y_direction} };
    case '/':
        if (y_direction == 0)
            return x_direction > 0 ? std::vector<Beam>{{x, y, 0, -1}} : std::vector<Beam>{ {x, y, 0, 1} };
        else
            return y_direction > 0 ? std::vector<Beam>{{x, y, -1, 0}} : std::vector<Beam>{ {x, y, 1, 0} };
    case '\\':
        if (y_direction == 0)
            return x_direction > 0 ? std::vector<Beam>{{x, y, 0, 1}} : std::vector<Beam>{ {x, y, 0, -1} };
        else
            return y_direction > 0 ? std::vector<Beam>{{x, y, 1, 0}} : std::vector<Beam>{ {x, y, -1, 0} };
    case '.':
        return std::vector<Beam>{{x, y, x_direction, y_direction}};
    }
    
    return {};
}

static int64_t 
Energize(const std::map<std::pair<int, int>, char>& mirror_layout, int maximum_x, int maximum_y, Beam light_path)
{
    std::set<Beam> energized_state;
    std::vector<Beam> light_paths = { light_path };
    while (!light_paths.empty())
    {
        auto& [x, y, x_direction, y_direction] = light_paths.back();
        light_paths.pop_back();
        x += x_direction, y += y_direction;

        if (x < 0 || x > maximum_x || y < 0 || y > maximum_y || energized_state.find({ x, y, x_direction, y_direction }) != energized_state.end())
        {
            continue;
        }
        
        energized_state.insert({ x, y, x_direction, y_direction });
        auto it = mirror_layout.find({ x, y });
        if (it != mirror_layout.end())
        {
            char current = it->second;
            auto updates = Update(x, y, x_direction, y_direction, current);
            light_paths.insert(light_paths.end(), updates.begin(), updates.end());
        }
    }

        std::set<std::pair<int, int>> unique_positions;
    for (const auto& e : energized_state)
    {
        unique_positions.insert({ e.x, e.y });
    }
    
    return unique_positions.size();
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::ifstream input_file("tiles.txt");

    if (!input_file.is_open())
    {
        std::cerr << "File cannot be opened!" << std::endl;
        return 1;
    }

    std::map<std::pair<int, int>, char> mirror_layout;
    std::string line;
    int y = 0;
    while (std::getline(input_file, line))
    {
        for (int x = 0; x < static_cast<int>(line.size()); ++x)
        {
            mirror_layout[{x, y}] = line[x];
        }
        
        ++y;
    }

    int maximum_x = 0, maximum_y = 0;
    for (const auto& [key, _] : mirror_layout)
    {
        maximum_x = std::max(maximum_x, key.first);
        maximum_y = std::max(maximum_y, key.second);
    }

    auto energized_tiles = Energize(mirror_layout, maximum_x, maximum_y, { -1, 0, 1, 0 });
	std::cout << "Number of energized tiles is: " << energized_tiles << std::endl;
    
    return 0;
}