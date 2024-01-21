#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>

struct Point
{
    long long x, y;
    Point operator + (const Point& addend) const { return { x + addend.x, y + addend.y }; }
};

static long long
LagoonArea(const std::vector<Point>& entries)
{
    long long area = 0;
    long long n = entries.size();
    for (long long i = 0; i < n; ++i)
    {
        area += entries[i].x * (entries[(i + 1) % n].y - entries[(i - 1 + n) % n].y);
    }

    return std::abs(area) / 2;
}

static long long
LagoonVolume(const std::vector<Point>& entries, int count)
{
    long long area = LagoonArea(entries);

    return area + 1 - (count / 2);
}

static std::pair<std::vector<Point>, int>
ParseEntriesFromPlan(const std::vector<std::pair<Point, int>>& instructions)
{
    Point current_position = { 0, 0 };
    int count = 0;
    std::vector<Point> entries;
    for (const auto& [direction, number] : instructions)
    {
        current_position = current_position + Point{ direction.x * number, direction.y * number };
        entries.push_back(current_position);
        count += number;
    }
    
    return { entries, count };
}

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
    std::vector<std::pair<Point, int>> parsed_entries;

    std::map<char, Point> directions = { {'R', {0, 1}}, {'L', {0, -1}}, {'U', {-1, 0}}, {'D', {1, 0}} };

    while (std::getline(input_file, line))
    {
        std::istringstream iss(line);
        std::string direction, cubic_metres;
        iss >> direction >> cubic_metres;
        parsed_entries.push_back({ directions[direction[0]], std::stoi(cubic_metres) });
    }

    auto [nodes, count] = ParseEntriesFromPlan(parsed_entries);
    std::cout << "The laggon could hold " << LagoonVolume(nodes, count) + count 
              << " cubic metres of lava." << std::endl;
    
    return 0;
}
