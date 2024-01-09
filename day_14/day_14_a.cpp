#include <fstream>
#include <iostream>
#include <string>
#include <vector>

int main()
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::ifstream input_file("parabolic.txt");

    if (!input_file.is_open())
    {
        std::cerr << "File cannot be opened!" << std::endl;
        return 1;
    }

    std::string line;
    std::vector<std::string> platform;
    while (std::getline(input_file, line))
    {
        platform.push_back(line);
    }

    int total_load = 0;
    for (int y = 0; y < platform.size(); ++y)
    {
        for (int x = 0; x < platform[y].size(); ++x)
        {
            if (platform[y][x] == 'O')
            {
                platform[y][x] = '.';
                int64_t i = y - 1;
                while (i >= 0 && platform[i][x] == '.')
                {
                    --i;
                }
                
                platform[i + 1][x] = 'O';
                total_load += platform.size() - i - 1;
            }
        }
    }

    std::cout << "Total load on the north support beams is " << total_load << std::endl;

    return 0;
}
