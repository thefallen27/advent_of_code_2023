#include <array>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(NULL);

    std::ifstream input_file("game_records.txt");
    if (!input_file)
    {
        std::cerr << "File cannot be opened!" << std::endl;
        return 1;
    }

    std::string line;
    size_t sum = 0;

    while (std::getline(input_file, line))
    {
        std::istringstream record_line(line);
        std::string section;
        std::getline(record_line, section, ':');
        std::getline(record_line, section);

        std::array<size_t, 3> red_green_blue = { 0, 0, 0 };

        std::istringstream record_section(section);
        std::string game_set;

        while (std::getline(record_section, game_set, ';'))
        {
            std::istringstream game_set_record(game_set);
            std::string game;

            while (std::getline(game_set_record, game, ','))
            {
                std::istringstream game_record(game);
                int count;
                std::string colour;

                game_record >> count >> colour;

                if (colour == "red" && count > red_green_blue[0])
                {
                    red_green_blue[0] = count;
                }
                else if (colour == "green" && count > red_green_blue[1])
                {
                    red_green_blue[1] = count;
                }
                else if (colour == "blue" && count > red_green_blue[2])
                {
                    red_green_blue[2] = count;
                }
            }
        }

        sum += red_green_blue[0] * red_green_blue[1] * red_green_blue[2];
    }

    std::cout << "The sum of the power of the minimum number sets is: " << sum << std::endl;

    return 0;
}
