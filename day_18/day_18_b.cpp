#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>

struct Point { int x, y; };

int main()
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::ifstream input_file("dig_plan.txt");

    if (!input_file.is_open())
    {
        std::cerr << "File cannot be opened!" << std::endl;
        return 1;
    }

    std::string line;
    std::vector<Point> nodes;
    std::map<char, Point> directions = 
    {
        {'U', {0, -1}},
        {'D', {0, 1}},
        {'L', {-1, 0}},
        {'R', {1, 0}}
    };

    int x = 0, y = 0;
    size_t perimeter = 0;
    nodes.push_back({ x, y });

    while (std::getline(input_file, line))
    {
        std::istringstream iss(line);
        std::string direction, number, colours;
        iss >> direction >> number >> colours;
        int steps = std::stoi(colours.substr(2, 5), nullptr, 16);
        int current_direction = colours[7] - '0';
        char parsed_direction;
        switch (current_direction)
        {
            case 0: parsed_direction = 'R'; break;
            case 1: parsed_direction = 'D'; break;
            case 2: parsed_direction = 'L'; break;
            case 3: parsed_direction = 'U'; break;
        }

        x += directions[parsed_direction].x * steps;
        y += directions[parsed_direction].y * steps;
        perimeter += steps;
        nodes.push_back({ x, y });
    }

    long long area = 0;
    for (size_t i = 1; i < nodes.size(); ++i)
    {
        area += static_cast<long long>(nodes[i].x) * nodes[static_cast<long long>(i) - 1].y 
                - static_cast<long long>(nodes[i].y) * nodes[i - 1].x;
    }

    area = std::abs(area / 2) + perimeter / 2 + 1;
    
    std::cout << "The laggon could hold " << area << " cubic metres of lava." << std::endl;

    return 0;
}
