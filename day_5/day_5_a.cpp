#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <vector>

struct MapStructure { long long destination, source, range; };

static std::vector<long long> 
Seeds(std::ifstream& input_file)
{
    std::string line;
    std::getline(input_file, line);
    std::stringstream ss(line);

    std::string useless;
    ss >> useless;

    std::vector<long long> seeds;

    long long seed_number;
    while (ss >> seed_number)
    {
        seeds.push_back(seed_number);
    }

    std::getline(input_file, useless);

    return seeds;
}

static std::vector<MapStructure>
ParseMap(std::ifstream& input_file)
{
    std::vector<MapStructure> map_structure;
    std::string line;
    while (std::getline(input_file, line) && !line.empty())
    {
        std::stringstream ss(line);

        long long destination, source, range;
        while (ss >> destination >> source >> range)
        {
            map_structure.push_back({ destination, source, range });
        }
    }

    return map_structure;
}

inline static long long
Search(long long key, const std::vector<MapStructure>& mp)
{
    for (const auto& m : mp)
    {
        if (key > m.source && key <= m.source + m.range)
        {
            return m.destination + (key - m.source);
        }
    }

    return key;
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(NULL);

    std::ifstream input_file("seed.txt");
    if (!input_file)
    {
        std::cerr << "File cannot be opened!" << std::endl;
        return 1;
    }

    auto seeds = Seeds(input_file);
    auto seed_2_soil = ParseMap(input_file);
    auto soil_2_fertilizer = ParseMap(input_file);
    auto fertilizer_2_water = ParseMap(input_file);
    auto water_2_light = ParseMap(input_file);
    auto light_2_temperature = ParseMap(input_file);
    auto temperature_2_humidity = ParseMap(input_file);
    auto humidity_2_location = ParseMap(input_file);

    long long lowest_location = std::numeric_limits<long long>::max();
    for (const auto& seed : seeds)
    {
        auto soil = Search(seed, seed_2_soil);
        auto fertilizer = Search(soil, soil_2_fertilizer);
        auto water = Search(fertilizer, fertilizer_2_water);
        auto light = Search(water, water_2_light);
        auto temperature = Search(light, light_2_temperature);
        auto humidity = Search(temperature, temperature_2_humidity);
        auto location = Search(humidity, humidity_2_location);

        lowest_location = std::min(lowest_location, location);
    }

    std::cout << "The lowest location number is  " << lowest_location << '\n';

    return 0;
}
