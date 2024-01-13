#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

static int
Hashing(const std::string& s)
{
    int v = 0;
    for (char ch : s)
    {
        v += static_cast<int>(ch);
        v *= 17;
        v %= 256;
    }

    return v;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::ifstream input_file("init_seq.txt");

    if (!input_file.is_open())
    {
        std::cerr << "File cannot be opened!" << std::endl;
        return 1;
    }

    std::string line;
    std::getline(input_file, line);
    std::istringstream iss(line);
    std::string step;
    std::vector<std::vector<std::pair<std::string, int>>> boxes(256);
    
    while (getline(iss, step, ','))
    {
        size_t sign_operator = step.find('-');
        if (step.back() == '-')
        {
            std::string label = step.substr(0, step.size() - 1);
            int box = Hashing(label);
            auto& box_vector = boxes[box];
            for (auto it = box_vector.begin(); it != box_vector.end(); ++it)
            {
                if (it->first == label)
                {
                    box_vector.erase(it);
                    break;
                }
            }
        }
        else
        {
            sign_operator = step.find('=');
            std::string label = step.substr(0, sign_operator);
            int focal_length = std::stoi(step.substr(sign_operator + 1));
            auto& box = boxes[Hashing(label)];
            auto it = std::find_if(box.begin(), box.end(), 
                [&](const std::pair<std::string, int>& p) {
                return p.first == label;
                });

            if (it != box.end())
            {
                it->second = focal_length;
            }
            else
            {
                box.emplace_back(label, focal_length);
            }
        }
    }

    size_t sum = 0;
    for (size_t n = 0; n < 256; ++n)
    {
        for (size_t m = 0; m < boxes[n].size(); ++m)
        {
            sum += (n + 1) * (m + 1) * boxes[n][m].second;
        }
    }

    std::cout << "The focusing power of the resulting lens configuration is " << sum << std::endl;
    
    return 0;
}
