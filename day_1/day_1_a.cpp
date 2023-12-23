#include <fstream>
#include <iostream>
#include <string>

int main()
{
	std::ios::sync_with_stdio(false);
    std::cin.tie(NULL);
    
	std::ifstream file("calibration.txt");
    if (!file)
    {
        std::cerr << "File cannot be opened!" << std::endl;
        return 1;
    }

    std::string line;
    int total_sum = 0;

    while (getline(file, line))
    {
        if (line.empty())
        {
            continue;
        }

        int first_digit_position = 0;
        while (first_digit_position < line.length() 
			&& !isdigit(line[first_digit_position]))
        {
            ++first_digit_position;
        }

        size_t last_digit_position = line.length() - 1;
        while (last_digit_position >= 0 
		    && !isdigit(line[last_digit_position]))
        {
            --last_digit_position;
        }

        if (first_digit_position < line.length() && last_digit_position >= 0)
        {
            int calibration_value = (line[first_digit_position] - '0') * 10 
									+ (line[last_digit_position] - '0');
            total_sum += calibration_value;
        }
    }

    std::cout << "Total sum of calibration values: " << total_sum << std::endl;

    return 0;
}
