#include <iostream>

int main()
{
    size_t total_time_given = 44806572;         // Total time for the race in milliseconds
    size_t record_distance = 208158110501102;   // Record distance for the race in millimeters

    size_t ways = 0;
    for (size_t hold_time = 0; hold_time < total_time_given; ++hold_time)
    {
        size_t speed = hold_time;
        size_t travel_time = total_time_given - hold_time;
        size_t traveled_distance = speed * travel_time;
        if (traveled_distance > record_distance)
        {
            ways++;
        }
    }

    std::cout << "Number of ways to win the race: " << ways << std::endl;
    
    return 0;
}
