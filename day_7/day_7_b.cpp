#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

constexpr std::array<char, 14> labels{ 'A', 'K', 'Q', 'T', '9', '8', '7', '6', '5', '4', '3', '2', 'J' };

static int
TypeRanks(const std::string& hand)
{
    std::unordered_map<char, int> label_counts;
    for (char card : hand)
    {
        if (card != 'J')
        {
            label_counts[card]++;
        }
    }

    std::vector<int> cardinal_number;
    for (const auto& [_, count] : label_counts)
    {
        cardinal_number.push_back(count);
    }

    if (cardinal_number.empty()) { cardinal_number.push_back(0); }

    std::sort(cardinal_number.rbegin(), cardinal_number.rend());
    cardinal_number[0] += std::count(hand.begin(), hand.end(), 'J');

    static const std::vector<std::vector<int>> ranking = 
            { {5}, {4, 1}, {3, 2}, {3, 1, 1}, {2, 2, 1}, {2, 1, 1, 1}, {1, 1, 1, 1, 1} };

    return std::distance(ranking.begin(), std::ranges::find(ranking, cardinal_number));
}

static std::array<int, 5>
LabelsAsIntegers(const std::string& hand)
{
    std::array<int, 5> card_values{};
    std::ranges::transform(hand, card_values.begin(),
        [](char card) {
            return std::distance(labels.begin(), std::ranges::find(labels, card));
        });

    return card_values;
}

int main() {
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
