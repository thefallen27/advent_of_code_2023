#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

static std::vector<std::pair<std::unordered_set<int>, std::vector<int>>>
ReadLinesFromFile(const std::string& input_file)
{
    std::ifstream file(input_file);
    if (!file.is_open())
    {
        std::cerr << "Failed to open file." << std::endl;
        exit(0);
    }

    std::vector<std::pair<std::unordered_set<int>, std::vector<int>>> cards;
    std::string line, token;

    while (std::getline(file, line))
    {
        std::istringstream input_ss(line);
        std::unordered_set<int> winning_numbers;
        std::vector<int> scratchcard_numbers;

        std::getline(input_ss, token, '|');
        std::istringstream winnin_numbers_stream(token.substr(token.find(':') + 1));
        while (winnin_numbers_stream >> token)
        {
            winning_numbers.insert(std::stoi(token));
        }

        while (input_ss >> token)
        {
            scratchcard_numbers.push_back(std::stoi(token));
        }

        cards.emplace_back(winning_numbers, scratchcard_numbers);
    }

    return cards;
}

static size_t
MatchingElements(const std::unordered_set<int>& winning_numbers, const std::vector<int>& scratchcard_numbers)
{
    return std::count_if(scratchcard_numbers.begin(), scratchcard_numbers.end(),
        [&winning_numbers](int num) { return winning_numbers.find(num) != winning_numbers.end(); });
}

static int 
ProcessScratchcards(std::vector<std::pair<std::unordered_set<int>, std::vector<int>>>& cards)
{
    int total_cards = 0;
    std::queue<int> card_queue;

    for (int i = 0; i < cards.size(); ++i)
    {
        card_queue.push(i);
    }

    while (!card_queue.empty())
    {
        auto card_pointer = card_queue.front();
        card_queue.pop();
        total_cards++;

        auto matches = MatchingElements(cards[card_pointer].first, cards[card_pointer].second);

        for (int i = 1; i <= matches && card_pointer + i < cards.size(); ++i)
        {
            card_queue.push(card_pointer + i);
        }
    }

    return total_cards;
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(NULL);

    std::string input_file = "scratchcard_points.txt";
    auto cards = ReadLinesFromFile(input_file);
    
    std::cout << "Grand total of scratchcards: " << ProcessScratchcards(cards) << std::endl;

    return 0;
}
