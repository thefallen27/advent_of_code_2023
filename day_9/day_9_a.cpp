#include <execution>
#include <fstream>
#include <iostream>
#include <vector>
#include "ctre.hpp"

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

    std::vector<int> extrapolated_values;
    extrapolated_values.reserve(1000);

    for (std::string line; std::getline(input_file, line);)
    {
        std::vector<int> history;

        for (auto& match : ctre::search_all<R"(-?\d+)">(line))
        {
            history.push_back(std::stoi(match.to_string()));
        }

        std::vector<std::vector<int>> sequences;
        sequences.push_back(std::move(history));

        while (std::ranges::any_of(sequences.back(), [](int value) { return value != 0; }))
        {
            std::vector<int> differences;
            differences.reserve(sequences.back().size());

            for (size_t i = 1; i < sequences.back().size(); ++i) {
                differences.push_back(sequences.back()[i] - sequences.back()[i - 1]);
            }
            sequences.push_back(std::move(differences));
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
