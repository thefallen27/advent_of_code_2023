#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

class MapStructure
{
public:
    MapStructure(std::array<long long, 2> from, std::array<long long, 2> to)
        : from_(from)
        , to_(to)
          {}

    long long apply_map(long long n) const { return to_[0] + (n - from_[0]); }
    const std::array<long long, 2>& get_source_part() const { return from_; }
    const std::array<long long, 2>& get_destination_part() const { return to_; }

private:
    std::array<long long, 2> from_;
    std::array<long long, 2> to_;
};

class Map
{
public:
    Map(std::string source, std::string destination)
        : source_(std::move(source))
        , destination_(std::move(destination))
          {}

    void AddMap(const MapStructure& mapping) { mappings_.push_back(mapping); }
    const std::vector<MapStructure>& GetMap() const { return mappings_; }

private:
    std::string source_;
    std::string destination_;
    std::vector<MapStructure> mappings_;
};

static std::vector<long long>
ParseForNumbers(const std::string& s)
{
    std::vector<long long> numbers;
    size_t start = 0;
    size_t end;
    while ((end = s.find(' ', start)) != std::string::npos)
    {
        numbers.push_back(std::stoll(s.substr(start, end - start)));
        start = end + 1;
    }
    
    numbers.push_back(std::stoll(s.substr(start)));
    
    return numbers;
}

static std::vector<std::pair<long long, long long>>
TransformMappings(const std::vector<std::pair<long long, long long>>& original_range,
                  const std::vector<MapStructure>& mapping_rules)
{
    std::vector<std::pair<long long, long long>> updated_values;
    updated_values.reserve(original_range.size() * mapping_rules.size());

    for (const auto& [range_start, range_end] : original_range)
    {
        for (const auto& mapping : mapping_rules)
        {
            const auto& from_range = mapping.get_source_part();
            const auto& to_range = mapping.get_destination_part();

            if (range_end < from_range[0] || range_start > from_range[1])
            {
                continue;
            }

            auto mapped_start = std::max(range_start, from_range[0]);
            auto mapped_end = std::min(range_end, from_range[1]);
            updated_values.emplace_back(mapping.apply_map(mapped_start), mapping.apply_map(mapped_end));
        }
    }

    std::ranges::sort(updated_values, [](const auto& a, const auto& b) { return a.first < b.first; });

    return updated_values;
}


int main()
{
    std::string input = "seed.txt";
    std::fstream file(input);
    if (!file)
    {
        std::cerr << "Error opening file: " << input << std::endl;
        return 1;
    }

    std::string line;
    std::getline(file, line);
    auto input_ranges = ParseForNumbers(line.substr(7));
    std::vector<std::pair<long long, long long>> seeds;
    for (size_t i = 0; i < input_ranges.size(); i += 2)
    {
        seeds.emplace_back(input_ranges[i], input_ranges[i] + input_ranges[i + 1] - 1);
    }

    std::vector<Map> maps;
    while (std::getline(file, line))
    {
        if (line.empty())
        {
            std::getline(file, line);
            size_t end = line.find('-');
            std::string map_from = line.substr(0, end);
            size_t start = end + 4;
            end = line.find(' ', start);
            std::string map_to = line.substr(start, end - start);

            maps.emplace_back(map_from, map_to);
            std::getline(file, line);
        }

        const auto numbers = ParseForNumbers(line);
        MapStructure mapping({ numbers[1], numbers[1] + numbers[2] - 1 },
                             { numbers[0], numbers[0] + numbers[2] - 1 });
        maps.back().AddMap(mapping);
    }

    std::vector<std::pair<long long, long long>> values = seeds;
    values.reserve(maps.size() * seeds.size());

    for (const auto& map : maps)
    {
        values = TransformMappings(values, map.GetMap());
    }

    auto minimum_location = std::ranges::min_element(values, {}, &std::pair<long long, long long>::first);
    std::cout << "The lowest location number is " << minimum_location->first << std::endl;

    return 0;
}
