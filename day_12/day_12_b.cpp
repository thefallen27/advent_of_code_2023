#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

int main()
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::ifstream input_file("springs.txt");
    std::vector<std::string> input_scheme;

    if (!input_file.is_open())
    {
        std::cerr << "File cannot be opened!" << std::endl;
        return 1;
    }

    int64_t possible_permutations = 0;
    std::string line;
    while (std::getline(input_file, line))
    {
        std::istringstream iss(line);
        std::string condition_record, arrangements;
        iss >> condition_record >> arrangements;

        std::istringstream arrangements_stream(arrangements);
        std::vector<int> cardinal_numbers;
        std::string numbers;
        while (std::getline(arrangements_stream, numbers, ','))
        {
            cardinal_numbers.push_back(std::stoi(numbers));
        }

        std::string tmp_cond_rec = condition_record;
        for (int i = 1; i < 5; ++i)
        {
            condition_record += "?" + tmp_cond_rec;
        }

        std::vector<int> additional_numbers = cardinal_numbers;
        for (int i = 1; i < 5; ++i)
        {
            cardinal_numbers.insert(cardinal_numbers.end(), additional_numbers.begin(), additional_numbers.end());
        }

        std::unordered_map<int64_t, int64_t> permutations = { {0, 1} };
        for (size_t i = 0; i < cardinal_numbers.size(); ++i)
        {
            std::unordered_map<int64_t, int64_t> new_permutations;
            for (auto& [k, v] : permutations)
            {
                for (size_t n = k; n < condition_record.length() -
                    std::accumulate(cardinal_numbers.begin() + i + 1, cardinal_numbers.end(), 0) + cardinal_numbers.size() - i - 1;
                    ++n)
                {
                    if (n + cardinal_numbers[i] - 1 < condition_record.length() &&
                        condition_record.substr(n, cardinal_numbers[i]).find('.') == std::string::npos)
                    {
                        if ((i == cardinal_numbers.size() - 1 && condition_record.substr(n + cardinal_numbers[i]).find('#') ==
                            std::string::npos) || (i < cardinal_numbers.size() - 1 &&
                                n + cardinal_numbers[i] < condition_record.length() && condition_record[n + cardinal_numbers[i]] != '#'))
                        {
                            new_permutations[n + cardinal_numbers[i] + 1] += v;
                        }
                    }

                    if (condition_record[n] == '#')
                    {
                        break;
                    }
                }
            }

            permutations = std::move(new_permutations);
        }

        for (auto& [_, v] : permutations)
        {
            possible_permutations += v;
        }
    }

    std::cout << "The sum of all permutations is " << possible_permutations << std::endl;

    return 0;
}
