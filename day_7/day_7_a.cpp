#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

constexpr std::array<char, 13> labels{ 'A', 'K', 'Q', 'J', 'T', '9', '8', '7', '6', '5', '4', '3', '2' };

static int
TypeRanks(const std::string& hand)
{
    std::unordered_map<char, int> label_counts;
    for (char card : hand)
    {
        label_counts[card]++;
    }

    std::vector<int> cardinal_number;
    for (const auto& [_, count] : label_counts)
    {
        cardinal_number.push_back(count);
    }

    std::sort(cardinal_number.rbegin(), cardinal_number.rend());

    if (cardinal_number == std::vector<int>{5}) { return 0; }
    if (cardinal_number == std::vector<int>{4, 1}) { return 1; }
    if (cardinal_number == std::vector<int>{3, 2}) { return 2; }
    if (cardinal_number == std::vector<int>{3, 1, 1}) { return 3; }
    if (cardinal_number == std::vector<int>{2, 2, 1}) { return 4; }
    if (cardinal_number == std::vector<int>{2, 1, 1, 1}) { return 5; }
    return 6;
}

static std::array<int, 5>
LabelsAsIntegers(const std::string& hand)
{
    std::array<int, 5> label_values{};
    std::transform(hand.begin(), hand.end(), label_values.begin(),
        [](char card) {
            return std::distance(labels.begin(), std::find(labels.begin(), labels.end(), card));
        });

    return label_values;
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(NULL);

    std::ifstream input_file("camel.txt");
    if (!input_file)
    {
        std::cerr << "File cannot be opened!" << std::endl;
        return 1;
    }

    std::vector<std::tuple<int, std::array<int, 5>, int>> hands;
    std::string line;
    while (std::getline(input_file, line))
    {
        auto space_pos = line.find(' ');
        std::string hand = line.substr(0, space_pos);
        int bid = std::stoi(line.substr(space_pos + 1));
        hands.emplace_back(TypeRanks(hand), LabelsAsIntegers(hand), bid);
    }

    std::sort(hands.rbegin(), hands.rend());

    long long winnings = 0;
    for (size_t i = 0; i < hands.size(); ++i)
    {
        winnings += (i + 1) * std::get<2>(hands[i]);
    }

    std::cout << "The total winnings are " << winnings << std::endl;
    
    return 0;
}
