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

    std::unordered_map<std::string, std::pair<const std::string*, const std::string*>> network;
    std::unordered_map<std::string, std::string> nodes;
    std::string line;

    while (std::getline(input_file, line))
    {
        auto matches = ctre::match<R"((\w{3}) = \((\w{3}), (\w{3})\))">(line);
        if (matches)
        {
            std::string node = matches.get<1>().to_string();
            std::string left = matches.get<2>().to_string();
            std::string right = matches.get<3>().to_string();

            nodes.insert({ node, node });
            nodes.insert({ left, left });
            nodes.insert({ right, right });

            network[node] = { &nodes[left], &nodes[right] };
        }
    }

    const std::string* current_node = &nodes["AAA"];
    auto counter = 0;
    size_t instruction_index = 0;
    size_t instructions_size = instructions.size();

    while (*current_node != "ZZZ")
    {
        char direction = instructions[instruction_index];
        current_node = (direction == 'L') ? network[*current_node].first : network[*current_node].second;
        instruction_index = (instruction_index + 1) % instructions_size;
        counter++;
    }

    std::cout << "Steps required to reach ZZZ: " << counter << std::endl;

    return 0;
}
