#include <iostream>
#include <numeric>
#include <vector>

int main()
{
    std::vector<int> time_given = { 44, 80, 65, 72 };               // Total time for each race
    std::vector<int> record_distances = { 208, 1581, 1050, 1102 };  // Record distances for each race

    long long total_ways = 1;
    for (size_t i = 0; i < time_given.size(); ++i)
	{
        int number_of_ways = 0;
        for (int hold_time = 0; hold_time < time_given[i]; ++hold_time)
		{
            if (hold_time * (time_given[i] - hold_time) > record_distances[i])
			{
                number_of_ways++;
            }
        }
		
        total_ways *= number_of_ways;
    }

    std::cout << "Total number of ways to win all races: " << total_ways << std::endl;
	
    return 0;
}