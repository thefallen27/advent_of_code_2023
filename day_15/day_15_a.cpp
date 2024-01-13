#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

int main()
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::ifstream input_file("init_seq.txt");

    if (!input_file.is_open())
    {
        std::cerr << "File cannot be opened!" << std::endl;
        return 1;
    }

    std::string line, input;
    while (std::getline(input_file, line))
    {
        input += line;
    }

    int results_sum = 0;
    std::stringstream ss(input);
    std::string step;

    while (std::getline(ss, step, ','))
    {
        int v = 0;
        for (char my_char : step)
        {
            v += static_cast<unsigned char>(my_char);
            v *= 17;
            v %= 256;
        }

        results_sum += v;
    }

    std::cout << "The sum of the results is " << results_sum << std::endl;
    
    return 0;
}
