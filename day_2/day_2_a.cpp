#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

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
    int sum = 0;
    int index = 1;

    while (std::getline(input_file, line))
    {
        std::istringstream record_line(line);
        std::string section;
        std::getline(record_line, section, ':');
        std::getline(record_line, section);

        std::istringstream record_section(section);
        std::string game_set;
        bool is_game_possible = true;

        while (std::getline(record_section, game_set, ';'))
        {
            std::istringstream game_set_record(game_set);
            std::string game;

            while (std::getline(game_set_record, game, ','))
            {
                std::istringstream game_record(game);
                int occurence_frequency;
                std::string colour;

                game_record >> occurence_frequency >> colour;

                if ((colour == "red" && occurence_frequency > 12) ||
                    (colour == "green" && occurence_frequency > 13) ||
                    (colour == "blue" && occurence_frequency > 14))
                {
                    is_game_possible = false;
                }
            }
        }

        if (is_game_possible)
        {
            sum += index;
        }

        index++;
    }

    std::cout << "Sum of IDs of possible games: " << sum << std::endl;

    return 0;
}
