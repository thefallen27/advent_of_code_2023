#include <fstream>
#include <iostream>
#include <numeric>
#include <unordered_map>
#include <vector>
#include "ctre.hpp"

class Network
{
public:
    Network(const std::string& seq) : sequence(seq) {}

    char next()
    {
        char direction = sequence[index];
        index = (index + 1) % sequence.size();
        return direction;
    }

private:
    const std::string& sequence;
    size_t index = 0;
};

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::vector<std::string> lines;
    std::string line;

    std::ifstream input_file("desert_map.txt");
    if (!input_file)
    {
        std::cerr << "File cannot be opened!" << std::endl;
        return 1;
    }

    while (std::getline(input_file, line))
    {
        lines.push_back(std::move(line));
    }
        
    std::unordered_map<std::string, std::pair<std::string, std::string>> network;

    for (const auto& line : lines)
    {
        auto match = ctre::match<R"((\w{3}) = \((\w{3}), (\w{3})\))">(line);
        if (match)
        {
            network[match.get<1>().to_string()] = { match.get<2>().to_string(), match.get<3>().to_string() };
        }
    }

    std::vector<unsigned long long> counters;
    Network instruction(lines[0]);

    for (const auto& [node, _] : network)
    {
        if (node.ends_with('A'))
        {
            std::string current_node = node;
            unsigned long long counter = 0;
            while (!current_node.ends_with('Z'))
            {
                const auto& [next_left, next_right] = network[current_node];
                current_node = (instruction.next() == 'L') ? next_left : next_right;
                ++counter;
            }
            
            counters.push_back(counter);
        }
    }

    unsigned long long lcm = std::accumulate(counters.begin(), counters.end(), 1ULL,
                                [](unsigned long long a, unsigned long long b) {
                                return std::lcm(a, b);
                                });

    std::cout << "Least common multiple of steps: " << lcm << std::endl;

    return 0;
 }
