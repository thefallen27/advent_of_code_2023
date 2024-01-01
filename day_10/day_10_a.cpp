#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

struct Point
{
    int x, y;

    Point operator+(const Point& other) const
    {
        return { x + other.x, y + other.y };
    }

    bool operator==(const Point& other) const
    {
        return x == other.x && y == other.y;
    }
};

struct PointHash
{
    size_t operator()(const Point& p) const
    {
        return std::hash<int>()(p.x) ^ (std::hash<int>()(p.y) << 1);
    }
};

const Point N = { 0, -1 }, S = { 0, 1 }, E = { 1, 0 }, W = { -1, 0 };

std::unordered_map<char, std::pair<Point, Point>> MapDirections = 
{
    {'|', {N, S}},
    {'-', {E, W}},
    {'L', {N, E}},
    {'J', {N, W}},
    {'7', {S, W}},
    {'F', {S, E}}
};

static Point
StartingDirections(const Point& start, const std::vector<std::string>& input_data)
{
    std::unordered_map<Point, std::string, PointHash> expected_adjacent_chars =
    {
        {N, "|7F"},
        {S, "|LJ"},
        {E, "-7J"},
        {W, "-LF"}
    };

    for (const auto& [direction, chars] : expected_adjacent_chars)
    {
        Point next_point = start + direction;
        if (chars.find(input_data[next_point.y][next_point.x]) != std::string::npos)
        {
            return direction;
        }
    }

    return { 0, 0 };
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::ifstream input_file("pipes.txt");
    if (!input_file)
    {
        std::cerr << "File cannot be opened!" << std::endl;
        return 1;
    }

    std::vector<std::string> input_data;
    std::unordered_map<Point, std::pair<Point, Point>, PointHash> graph;
    Point starting_position;
    bool start_tile_found = false;
    int y = 0;

    std::string line;
    while (std::getline(input_file, line))
    {
        input_data.push_back(line);
        for (int x = 0; x < line.size(); ++x)
        {
            char ch = line[x];
            if (MapDirections.count(ch))
            {
                graph[{x, y}] = MapDirections[ch];
            }
            
            if (ch == 'S')
            {
                starting_position = { x, y };
                start_tile_found = true;
            }
        }

        ++y;
    }

    if (!start_tile_found)
    {
        std::cerr << "Start position not found." << std::endl;
        return 1;
    }

    Point current_direction = StartingDirections(starting_position, input_data);
    Point current_coordinates = starting_position + current_direction;
    std::unordered_set<Point, PointHash> visited_points;
    visited_points.insert(starting_position);

    while (true)
    {
        visited_points.insert(current_coordinates);
        const auto& [first_direction, second_direction] = graph[current_coordinates];
        Point alternative_direction = current_coordinates + first_direction;
        Point second_alternative_direction = current_coordinates + second_direction;

        if (visited_points.contains(alternative_direction) && visited_points.contains(second_alternative_direction))
        {
            break;
        }
        
        current_coordinates = visited_points.contains(alternative_direction) ? second_alternative_direction : alternative_direction;
    }

    std::cout << "Steps needed: " << visited_points.size() / 2 << std::endl;

    return 0;
}
