#include <ctre.hpp>
#include <fstream>
#include <iostream>
#include <unordered_map>

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::ifstream input_file("desert_map.txt");
    if (!input_file)
    {
        std::cerr << "File cannot be opened!" << std::endl;
        return 1;
    }

    std::string instructions;
    std::getline(input_file, instructions);

    std::unordered_map<std::string, std::pair<std::string, std::string>> network;

    std::string line;
    while (std::getline(input_file, line))
    {
        auto matches = ctre::match<R"((\w{3}) = \((\w{3}), (\w{3})\))">(line);
        if (matches)
        {
            network[matches.get<1>().to_string()] = { matches.get<2>().to_string(), matches.get<3>().to_string() };
        }
    }

    std::string current_node = "AAA";
    auto counter = 0;
    size_t instruction_index = 0;

    while (current_node != "ZZZ")
    {
        char direction = instructions[instruction_index];
        current_node = (direction == 'L') ? network[current_node].first : network[current_node].second;
        instruction_index = (instruction_index + 1) % instructions.size();
        counter++;
    }

    std::cout << "Steps required to reach ZZZ: " << counter << std::endl;

    return 0;
}
