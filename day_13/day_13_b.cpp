#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

static int 
HammingDistance(const std::string& list1, const std::string& list2)
{
    int differences = 0;
    for (size_t i = 0; i < list1.size(); ++i)
    {
        if (list1[i] != list2[i])
        {
            ++differences;
        }
    }

    return differences;
}

static bool 
IsHorizontallySymmetrical(const std::vector<std::string>& pattern, int start, int end, bool is_there_a_smudge)
{
    while (start >= 0 && end < pattern.size())
    {
        int hamming_number = HammingDistance(pattern[start], pattern[end]);
        if (hamming_number > 1)
        {
            return false;
        }
        else if (hamming_number == 1 && !is_there_a_smudge)
        {
            return false;
        }
        
        is_there_a_smudge = is_there_a_smudge && hamming_number == 0;
        --start; ++end;
    }
    
    return !is_there_a_smudge;
}

static bool
IsVerticallySymmetrical(const std::vector<std::string>& pattern, int start, int end, bool is_there_a_smudge)
{
    while (start >= 0 && end < pattern[0].size())
    {
        std::string column_a, column_b;
        for (const auto& row : pattern)
        {
            column_a.push_back(row[start]);
            column_b.push_back(row[end]);
        }
        
        int hamming_number = HammingDistance(column_a, column_b);
        if (hamming_number > 1)
        {
            return false;
        }
        else if (hamming_number == 1 && !is_there_a_smudge)
        {
            return false;
        }
        
        is_there_a_smudge = is_there_a_smudge && hamming_number == 0;
        --start; ++end;
    }

    return !is_there_a_smudge;
}

static int
SummarizeNotes(const std::vector<std::string>& pattern)
{
    for (int i = 0; i < pattern.size() - 1; ++i)
    {
        if (IsHorizontallySymmetrical(pattern, i, i + 1, true))
        {
            return (i + 1) * 100;
        }
    }

    for (int i = 0; i < pattern[0].size() - 1; ++i)
    {
        if (IsVerticallySymmetrical(pattern, i, i + 1, true))
        {
            return i + 1;
        }
    }

    return 0;
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::ifstream input_file("mirrors.txt");

    if (!input_file.is_open())
    {
        std::cerr << "File cannot be opened!" << std::endl;
        return 1;
    }

    std::vector<std::vector<std::string>> patterns;
    std::string line;
    std::vector<std::string> current_pattern;

    while (std::getline(input_file, line))
    {
        if (line.empty())
        {
            patterns.push_back(std::move(current_pattern));
            current_pattern.clear();
        }
        else
        {
            current_pattern.push_back(line);
        }
    }

    if (!current_pattern.empty())
    {
        patterns.push_back(std::move(current_pattern));
    }

    int sum = 0;
    for (const auto& pattern : patterns)
    {
        sum += SummarizeNotes(pattern);
    }

    std::cout << "The sum is " << sum << std::endl;

    return 0;
}
