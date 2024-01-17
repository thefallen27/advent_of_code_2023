#include <iostream>
#include <fstream>
#include <queue>
#include <set>
#include <string>
#include <tuple>
#include <vector>

static void
AddToPQ(std::priority_queue<std::tuple<int, int, int, int, int, int>,
    std::vector<std::tuple<int, int, int, int, int, int>>,
    std::greater<std::tuple<int, int, int, int, int, int>>>& queue,
    const std::vector<std::vector<int>>& traffic_map,
    int heat_loss, int row, int column, int row_change, int column_change, int steps = 1)
{
    int new_row = row + row_change;
    int new_column = column + column_change;

    if (!(0 <= new_row && new_row < traffic_map.size() &&
        0 <= new_column && new_column < traffic_map[new_row].size()))
    {
        return;
    }

    queue.emplace(heat_loss + traffic_map[new_row][new_column], new_row, new_column,
        row_change, column_change, steps);
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::ifstream input_file("traffic_map.txt");

    if (!input_file.is_open())
    {
        std::cerr << "File cannot be opened!" << std::endl;
        return 1;
    }

    std::string line;
    std::vector<std::vector<int>> traffic_map;

    while (getline(input_file, line))
    {
        std::vector<int> row;
        for (char ch : line)
        {
            row.push_back(ch - '0');
        }

        traffic_map.push_back(row);
    }

    std::set<std::tuple<int, int, int, int, int>> visited;
    std::priority_queue<std::tuple<int, int, int, int, int, int>,
        std::vector<std::tuple<int, int, int, int, int, int>>,
        std::greater<std::tuple<int, int, int, int, int, int>>> pq;
    pq.emplace(0, 0, 0, 0, 0, 0);

    bool final_heat_loss_found = false;
    int final_heat_loss = 0;

    while (!pq.empty() && !final_heat_loss_found)
    {
        auto [heat_loss, row, column, row_change, column_change, steps] = pq.top();
        pq.pop();

        if (steps >= 4 && row == traffic_map.size() - 1 && column == traffic_map[row].size() - 1)
        {
            final_heat_loss_found = true;
            final_heat_loss = heat_loss;
            continue;
        }

        if (visited.find(std::make_tuple(row, column, row_change, column_change, steps)) != visited.end())
        {
            continue;
        }

        visited.insert(std::make_tuple(row, column, row_change, column_change, steps));

        if (steps < 10 && std::make_pair(row_change, column_change) != std::make_pair(0, 0))
        {
            AddToPQ(pq, traffic_map, heat_loss, row, column, row_change, column_change, steps + 1);
        }

        if (steps >= 4 || std::make_pair(row_change, column_change) == std::make_pair(0, 0))
        {
            for (auto& [new_row_change, new_column_change] : std::vector<std::pair<int, int>>{ {1, 0}, {-1, 0}, {0, 1}, {0, -1} })
            {
                if (std::make_pair(new_row_change, new_column_change) !=
                    std::make_pair(row_change, column_change) &&
                    std::make_pair(new_row_change, new_column_change) !=
                    std::make_pair(-row_change, -column_change))
                {
                    AddToPQ(pq, traffic_map, heat_loss, row, column, new_row_change, new_column_change);
                }
            }
        }
    }

    std::cout << "The least heat loss that can occur is " << final_heat_loss << std::endl;

    return 0;
}