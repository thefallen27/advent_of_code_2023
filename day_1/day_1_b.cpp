#include <algorithm>
#include <fstream>
#include <iostream>
#include <limits>
#include <string>
#include <unordered_map>
#include <vector>

std::vector<size_t>
substring_index(const std::string& str, const std::string& sub_str)
{
    std::vector<size_t> indexes;
    size_t position = str.find(sub_str, 0);
    while (position != std::string::npos)
    {
        indexes.push_back(position);
        position = str.find(sub_str, position + sub_str.length());
    }

    return indexes;
}

int
WordToDigit(std::ifstream& input_file)
{
    std::unordered_map<std::string, std::string> word_to_digit_map =
    {
        {"one", "1"},
        {"two", "2"},
        {"three", "3"},
        {"four", "4"},
        {"five", "5"},
        {"six", "6"},
        {"seven", "7"},
        {"eight", "8"},
        {"nine", "9"}
    };

    for (int i = 1; i <= 9; ++i)
    {
        word_to_digit_map[std::to_string(i)] = std::to_string(i);
    }

    int total_sum = 0;
    std::string line;
    while (std::getline(input_file, line))
    {
        std::unordered_map<size_t, std::string> results;
        for (const auto& [word, digit] : word_to_digit_map)
        {
            for (size_t index_position : substring_index(line, word))
            {
                results[index_position] = digit;
            }
        }

        if (!results.empty())
        {
            auto first_index = std::min_element(results.begin(), results.end(),
                [](const auto& a, const auto& b)
                { return a.first < b.first; })->first;

            auto last_index = std::max_element(results.begin(), results.end(),
                [](const auto& a, const auto& b)
                { return a.first < b.first; })->first;

            total_sum += std::stoi(results[first_index] + results[last_index]);
        }
    }

    return total_sum;
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(NULL);

    std::ifstream input_file("calibration.txt");
    if (!input_file)
    {
        std::cerr << "File cannot be opened!" << std::endl;
        return 1;
    }

    int total_sum = WordToDigit(input_file);
    std::cout << "Total sum of calibration values: " << total_sum << std::endl;

    return 0;
}
