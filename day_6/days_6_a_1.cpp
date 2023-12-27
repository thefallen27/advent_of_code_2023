#include <iostream>
#include <numeric>
#include <vector>

static int 
FindNumberOfWays(int total_time, int record_distance)
{
    int number_of_ways = 0;
    for (int hold_time = 0; hold_time < total_time; ++hold_time)
    {
        int speed = hold_time;
        int travel_time = total_time - hold_time;
        int distance = speed * travel_time;
        if (distance > record_distance)
        {
            number_of_ways++;
        }
    }

    return number_of_ways;
}

int main()
{
    std::vector<int> time_given = { 44, 80, 65, 72 };               // Total time for each race
    std::vector<int> record_distances = { 208, 1581, 1050, 1102 };  // Record distances for each race

    std::vector<int> number_of_ways;
    number_of_ways.reserve(time_given.size());
    for (size_t i = 0; i < time_given.size(); ++i)
    {
        number_of_ways.push_back(FindNumberOfWays(time_given[i], record_distances[i]));
    }

    long long total_ways = std::accumulate(number_of_ways.begin(), number_of_ways.end(), 1LL, std::multiplies<>());

    std::cout << "Total number of ways to win all races: " << total_ways << std::endl;
    
    return 0;
}