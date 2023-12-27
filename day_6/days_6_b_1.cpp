#include <iostream>

static size_t
FindNumberOfWays(size_t total_time, size_t record_distance)
{
    size_t ways = 0;
    for (size_t hold_time = 0; hold_time < total_time; ++hold_time)
    {
        size_t speed = hold_time;
        size_t travel_time = total_time - hold_time;
        size_t distance = speed * travel_time;
        if (distance > record_distance)
        {
            ways++;
        }
    }

    return ways;
}

int main()
{
    size_t total_time_given = 44806572;         // Total time for the race
    size_t record_distance = 208158110501102;   // Record distance for the race

    size_t total_ways = FindNumberOfWays(total_time_given, record_distance);
    std::cout << "Number of ways to win the race: " << total_ways << std::endl;

    return 0;
}
