#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_set>
#include <vector>

static std::vector<std::string> 
Split(const std::string& str, char delimiter)
{
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream stream_of_tokens(str);
    
    while (std::getline(stream_of_tokens, token, delimiter))
    {
        tokens.push_back(token);
    }
    
    return tokens;
}

static int 
CardPoints(const std::unordered_set<int>& winning_numbers, const std::unordered_set<int>& scratchcard_numbers)
{
    int points = 0;
    for (int number : scratchcard_numbers)
    {
        if (winning_numbers.find(number) != winning_numbers.end())
        {
            points = points == 0 ? 1 : points * 2;
        }
    }

    return points;
}

static int 
TotalPoints(const std::string& input_file)
{
    std::ifstream file(input_file);
    if (!file.is_open())
    {
        std::cerr << "Failed to open file." << std::endl;
        return -1;
    }
    
    std::string line;
    int total_points = 0;

    while (std::getline(file, line))
    {
        auto parts = Split(line, '|');
        auto winning_numbers = Split(parts[0].substr(parts[0].find(':') + 1), ' ');
        auto scratchcard_numbers = Split(parts[1], ' ');

        std::unordered_set<int> the_winning_numbers;
        std::unordered_set<int> the_scratchcard_numbers;

        for (const auto& number : winning_numbers)
        {
            if (!number.empty() && std::all_of(number.begin(), number.end(), ::isdigit))
            {
                the_winning_numbers.insert(std::stoi(number));
            }
        }

        for (const auto& number : scratchcard_numbers)
        {
            if (!number.empty() && std::all_of(number.begin(), number.end(), ::isdigit))
            {
                the_scratchcard_numbers.insert(std::stoi(number));
            }
        }

        total_points += CardPoints(the_winning_numbers, the_scratchcard_numbers);
    }

    return total_points;
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(NULL);

    std::string input_file = "scratchcard_points.txt";
    int result = TotalPoints(input_file);

    std::cout << "Total points from scratchcards: " << result << std::endl;

    return 0;
}
