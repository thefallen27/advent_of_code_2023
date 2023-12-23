#include <fstream>
#include <iostream>
#include <string>
#include <vector>

bool IsSymbol(char ch) { return !std::isdigit(ch) && ch != '.'; }

bool
IsAdjacentToSymbol(const std::vector<std::string>& engine_schematic, int row, int left_adjacent, int right_adjacent)
{
    for (int row_adjacent = -1; row_adjacent <= 1; ++row_adjacent)
    {
        for (int column_adjacent = -1; column_adjacent <= 1; ++column_adjacent)
        {
            for (int column = left_adjacent; column <= right_adjacent; ++column)
            {
                int adjacent_row = row + row_adjacent;
                int adjacent_column = column + column_adjacent;
                if (adjacent_row >= 0 && adjacent_row < engine_schematic.size() && 
                    adjacent_column >= 0 && adjacent_column < engine_schematic[0].size())
                {
                    if (IsSymbol(engine_schematic[adjacent_row][adjacent_column]))
                    {
                        return true;
                    }
                }
            }
        }
    }

    return false;
}

size_t
SumOfPartNumbers(const std::vector<std::string>& engine_schematic)
{
    size_t total_sum = 0;

    for (int row = 0; row < engine_schematic.size(); ++row)
    {
        for (int column = 0; column < engine_schematic[row].size(); )
        {
            if (std::isdigit(engine_schematic[row][column]))
            {
                size_t number_length = column;
                while (number_length < engine_schematic[row].size() &&
                       std::isdigit(engine_schematic[row][number_length]))
                {
                    number_length++;
                }
                
                if (IsAdjacentToSymbol(engine_schematic, row, column, number_length - 1))
                {
                    std::string numberStr = engine_schematic[row].substr(column, number_length - column);
                    total_sum += std::stoi(numberStr);
                }
                
                column = number_length;
            }
            else
            {
                column++;
            }
        }
    }

    return total_sum;
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(NULL);

    std::ifstream input_file("gear_ratios.txt");
    if (!input_file)
    {
        std::cerr << "File cannot be opened!" << std::endl;
        return 1;
    }

    std::vector<std::string> engine_schematic;
    std::string line;

    while (std::getline(input_file, line))
    {
        engine_schematic.push_back(line);
    }

    size_t result = SumOfPartNumbers(engine_schematic);
    std::cout << "Sum of all part numbers: " << result << std::endl;

    return 0;
}
