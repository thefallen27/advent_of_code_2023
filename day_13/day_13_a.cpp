#include <fstream>
#include <iostream>
#include <string>
#include <vector>

static bool
IsHorizontallySymmetrical(const std::vector<std::string>& pattern, int start, int end)
{
    for (; start >= 0 && end < pattern.size(); --start, ++end)
    {
        if (pattern[start] != pattern[end])
        {
            return false;
        }
    }

    return true;
}

static bool
IsVerticallySymmetrical(const std::vector<std::string>& pattern, int start, int end)
{
    for (; start >= 0 && end < pattern[0].size(); --start, ++end)
    {
        for (const auto& row : pattern)
        {
            if (row[start] != row[end])
            {
                return false;
            }
        }
    }

    return true;
}

static int
SummarizeNotes(const std::vector<std::string>& pattern)
{
    for (int i = 0; i < pattern.size() - 1; ++i)
    {
        if (IsHorizontallySymmetrical(pattern, i, i + 1))
        {
            return (i + 1) * 100;
        }
    }

    for (int i = 0; i < pattern[0].size() - 1; ++i)
    {
        if (IsVerticallySymmetrical(pattern, i, i + 1))
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
