#include <execution>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "ctre.hpp"

static std::vector<int>
sequence_differences(const std::vector<int>& sequence)
{
    std::vector<int> differences;
    for (size_t i = 1; i < sequence.size(); ++i)
    {
        differences.push_back(sequence[i] - sequence[i - 1]);
    }

    return differences;
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::ifstream input_file("oasis.txt");
    if (!input_file)
    {
        std::cerr << "File cannot be opened!" << std::endl;
        return 1;
    }

    std::string line;
    std::vector<int> extrapolated_values;
    extrapolated_values.reserve(1000);

    while (std::getline(input_file, line))
    {
        std::vector<int> history;
        history.reserve(line.size() / 2);

        for (auto& match : ctre::search_all<R"(-?\d+)">(line))
        {
            history.push_back(std::stoi(match.to_string()));
        }

        std::reverse(history.begin(), history.end());

        std::vector<std::vector<int>> sequences = { std::move(history) };
        sequences.reserve(10);

        while (std::any_of(sequences.back().begin(), sequences.back().end(), [](int value) { return value != 0; }))
        {
            sequences.push_back(sequence_differences(sequences.back()));
        }

        int extrapolated_value = std::transform_reduce(
            std::execution::par,
            sequences.rbegin(), sequences.rend(), 0,
            std::plus<>(),
            [](const auto& seq) { return seq.back(); });

        extrapolated_values.push_back(extrapolated_value);
    }

    int total = std::reduce(std::execution::par, extrapolated_values.begin(), extrapolated_values.end());
    std::cout << "Sum of extrapolated values is " << total << std::endl;

    return 0;
}
