#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

int main()
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::ifstream input_file("space.txt");
    std::vector<std::string> input_scheme;

    if (!input_file.is_open())
    {
        std::cerr << "File cannot be opened!" << std::endl;
        return 1;
    }

    std::string line;
    std::vector<std::string> space;
  
    while (std::getline(input_file, line))
    {
        space.push_back(line);
        if (line.find('#') == std::string::npos)
        {
            space.push_back(std::string(line.length(), '.'));
        }
    }

    size_t i = 0;
    while (i < space[0].size())
    {
        bool galaxy_found = false;
        for (auto& c : space)
        {
            if (c[i] == '#')
            {
                galaxy_found = true;
                break;
            }
        }

        if (!galaxy_found)
        {
            for (auto& x : space)
            {
                x.insert(i, 1, '.');
            }
            
            i++;
        }
        
        i++;
    }

    std::vector<std::pair<int64_t, int64_t>> galaxies;
    for (int x = 0; x < space[0].size(); ++x)
    {
        for (int y = 0; y < space.size(); ++y)
        {
            if (space[y][x] == '#')
            {
                galaxies.emplace_back(x, y);
            }
        }
    }

    long long path_length = 0;
    for (size_t i = 0; i < galaxies.size(); ++i)
    {
        for (size_t j = i + 1; j < galaxies.size(); ++j)
        {
            path_length += std::abs(galaxies[i].first - galaxies[j].first) + 
                           std::abs(galaxies[i].second - galaxies[j].second);
        }
    }

    std::cout << "The sum of all path lengths is " << path_length << std::endl;

    return 0;
}
