#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

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

    std::unordered_map<size_t, size_t> number_visited;
    int total_ratio_sum = 0;

    for (size_t row = 0; row < engine_schematic.size(); ++row)
    {
        for (size_t column = 0; column < engine_schematic[row].size(); ++column)
        {
            if (engine_schematic[row][column] == '*')
            {
                std::unordered_set<size_t> part_numbers;

                for (int adjacent_row_cell = -1; adjacent_row_cell <= 1; ++adjacent_row_cell)
                {
                    for (int adjacent_column_cell = -1; adjacent_column_cell <= 1; ++adjacent_column_cell)
                    {
                        if (adjacent_row_cell == 0 && adjacent_column_cell == 0)
                        {
                            continue;
                        }
                        
                        size_t adjacent_row = row + adjacent_row_cell;
                        size_t adjacent_column = column + adjacent_column_cell;
                        if (adjacent_row >= 0 && adjacent_row < engine_schematic.size() && 
                            adjacent_column >= 0 &&
                            adjacent_column < engine_schematic[0].size() &&
                            std::isdigit(engine_schematic[adjacent_row][adjacent_column]))
                        {
                            size_t key = adjacent_row * 1000 + adjacent_column;
                            if (number_visited.find(key) == number_visited.end())
                            {
                                size_t multi_digit_number_left = adjacent_column;
                                while (multi_digit_number_left > 0 && 
                                       std::isdigit(engine_schematic[adjacent_row][multi_digit_number_left - 1]))
                                {
                                    multi_digit_number_left--;
                                }

                                size_t multi_digit_number_right = adjacent_column;
                                while (multi_digit_number_right < engine_schematic[adjacent_row].size() - 1 &&
                                    std::isdigit(engine_schematic[adjacent_row][multi_digit_number_right + 1]))
                                {
                                    multi_digit_number_right++;
                                }
                                
                                int number = std::stoi(engine_schematic[adjacent_row]
                                            .substr(multi_digit_number_left, 
                                                    multi_digit_number_right - 
                                                    multi_digit_number_left + 1));
                                
                                for (size_t i = multi_digit_number_left; i <= multi_digit_number_right; ++i)
                                {
                                    number_visited[adjacent_row * 1000 + i] = number;
                                }
                            }

                            part_numbers.insert(number_visited[key]);
                        }
                    }
                }

                if (part_numbers.size() == 2)
                {
                    auto it = part_numbers.begin();
                    int gear_ratio = *it * *(++it);
                    total_ratio_sum += gear_ratio;
                }
            }
        }
    }

    std::cout << "The sum of all gear ratios is: " << total_ratio_sum << std::endl;

    return 0;
}
