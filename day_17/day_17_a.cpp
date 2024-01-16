#include <iostream>
#include <fstream>
#include <queue>
#include <string>
#include <unordered_set>
#include <vector>

struct State
{
    int heat_loss, row, column, row_change, column_change, steps;
};

struct CompareState
{
    bool operator () (const State& a, const State& b) { return a.heat_loss > b.heat_loss; }
};

namespace std
{
    template <>
    struct hash<std::tuple<int, int, int, int, int>>
    {
        size_t operator () (const std::tuple<int, int, int, int, int>& t) const
        {
            auto hash1 = std::hash<int>{}(std::get<0>(t));
            auto hash2 = std::hash<int>{}(std::get<1>(t));
            auto hash3 = std::hash<int>{}(std::get<2>(t));
            auto hash4 = std::hash<int>{}(std::get<3>(t));
            auto hash5 = std::hash<int>{}(std::get<4>(t));

            return hash1 ^ hash2 ^ hash3 ^ hash4 ^ hash5;
        }
    };
}

static void
AddToPQ(std::priority_queue<State, std::vector<State>, CompareState>& queue, const std::vector<std::vector<int>>& grid,
    int heat_loss, int row, int column, int row_change, int column_change, int steps = 1)
{
    int new_row = row + row_change;
    int new_column = column + column_change;

    if (!(0 <= new_row && new_row < grid.size() && 0 <= new_column && new_column < grid[new_row].size())) { return; }

    queue.push({ heat_loss + grid[new_row][new_column], new_row, new_column, row_change, column_change, steps });
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

    std::vector<std::vector<int>> grid;

    std::string line;
    while (std::getline(input_file, line))
    {
        grid.push_back({});
        for (char c : line)
        {
            grid.back().push_back(c - '0');
        }
    }

    std::priority_queue<State, std::vector<State>, CompareState> pq;
    pq.push({ 0, 0, 0, 0, 0, 0 });

    std::unordered_set<std::tuple<int, int, int, int, int>> visited;

    int heat_loss_sum = 0;

    while (!pq.empty())
    {
        State current = pq.top();
        pq.pop();

        int heat_loss = current.heat_loss;
        int row = current.row;
        int column = current.column;
        int row_change = current.row_change;
        int column_change = current.column_change;
        int steps = current.steps;

        if (row == grid.size() - 1 && column == grid[row].size() - 1)
        {
            heat_loss_sum = heat_loss;
            break;
        }

        if (visited.count({ row, column, row_change, column_change, steps }) > 0)
        {
            continue;
        }

        visited.insert({ row, column, row_change, column_change, steps });

        if (steps < 3 && (row_change != 0 || column_change != 0))
        {
            AddToPQ(pq, grid, heat_loss, row, column, row_change, column_change, steps + 1);
        }

        for (auto [new_row_change, new_column_change] : { std::make_pair(1, 0), std::make_pair(-1, 0), std::make_pair(0, 1), std::make_pair(0, -1) })
        {
            if ((new_row_change != row_change || new_column_change != column_change) && (new_row_change != -row_change || new_column_change != -column_change))
            {
                AddToPQ(pq, grid, heat_loss, row, column, new_row_change, new_column_change);
            }
        }
    }

    std::cout << "The least heat loss that can occur is " << heat_loss_sum << std::endl;

    return 0;
}
