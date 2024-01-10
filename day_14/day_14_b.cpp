#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

static void
VerticalShift(std::vector<std::string>& rows, char direction)
{
    std::size_t row_number = rows.size();
    std::vector<std::size_t> row_order(row_number);
    std::iota(row_order.begin(), row_order.end(), 0);

    if (direction == 'S')
    {
        std::reverse(row_order.begin(), row_order.end());
    }

    for (std::size_t column = 0; column < rows[0].size(); ++column)
    {
        std::vector<std::size_t> o_positions;
        for (auto row : row_order)
        {
            switch (rows[row][column])
            {
            case '.':
                o_positions.push_back(row);
                break;
            case '#':
                o_positions.clear();
                break;
            case 'O':
                if (!o_positions.empty())
                {
                    rows[o_positions.front()][column] = 'O';
                    rows[row][column] = '.';
                    o_positions.push_back(row);
                    o_positions.erase(o_positions.begin());
                }
                break;
            }
        }
    }
}

static void
HorizontalShift(std::vector<std::string>& rows, char direction)
{
    std::size_t column_count = rows[0].size();
    std::vector<std::size_t> column_order(column_count);
    std::iota(column_order.begin(), column_order.end(), 0);

    if (direction == 'E')
    {
        std::reverse(column_order.begin(), column_order.end());
    }

    for (std::size_t row = 0; row < rows.size(); ++row)
    {
        std::vector<std::size_t> o_positions;
        for (auto column : column_order)
        {
            switch (rows[row][column])
            {
            case '.':
                o_positions.push_back(column);
                break;
            case '#':
                o_positions.clear();
                break;
            case 'O':
                if (!o_positions.empty())
                {
                    rows[row][o_positions.front()] = 'O';
                    rows[row][column] = '.';
                    o_positions.push_back(column);
                    o_positions.erase(o_positions.begin());
                }
                break;
            }
        }
    }
}

static size_t
TotalLoad(const std::vector<std::string>& rows)
{
    size_t total_load = 0;
    for (std::size_t i = 0; i < rows.size(); ++i)
    {
        total_load += std::count(rows[i].begin(), rows[i].end(), 'O') * (rows.size() - i);
    }
    
    return total_load;
}

static std::string
ConvertGridToString(const std::vector<std::string>& rows)
{
    std::string resulting_string;
    for (const auto& row : rows)
    {
        resulting_string += row;
    }

    return resulting_string;
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::ifstream input_file("parabolic.txt");

    if (!input_file.is_open())
    {
        std::cerr << "File cannot be opened!" << std::endl;
        return 1;
    }

    std::vector<std::string> rows;
    std::string line;
    while (std::getline(input_file, line))
    {
        rows.push_back(line);
    }

    auto north = [&]() { VerticalShift(rows, 'N'); };
    auto south = [&]() { VerticalShift(rows, 'S'); };
    auto east = [&]() { HorizontalShift(rows, 'E'); };
    auto west = [&]() { HorizontalShift(rows, 'W'); };

    auto spinning_cycle = [&]() { north(); west(); south(); east(); };

    std::vector<std::string> cycles;

    spinning_cycle();
    while (std::find(cycles.begin(), cycles.end(), ConvertGridToString(rows)) == cycles.end())
    {
        cycles.push_back(ConvertGridToString(rows));
        spinning_cycle();
    }

    std::size_t steps_before_repeating = std::distance(cycles.begin(), std::find(cycles.begin(), cycles.end(), ConvertGridToString(rows)));
    std::size_t repeating_cycle_length = cycles.size() - steps_before_repeating;
    
    std::size_t cycles_left = (1000000000 - steps_before_repeating) % repeating_cycle_length;

    for (std::size_t i = 0; i < cycles_left - 1; ++i)
    {
        spinning_cycle();
    }

    std::cout << "Total load on the north support beams is " << TotalLoad(rows) << std::endl;

    return 0;
}
