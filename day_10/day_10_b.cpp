#include <fstream>
#include <functional>
#include <iostream>
#include <optional>
#include <string>
#include <utility>
#include <vector>

class MapLayout
{
public:
    explicit MapLayout(const std::vector<std::string>& layout_data)
    {
        plan = layout_data;
        path.assign(plan.size(), std::vector<char>(plan[0].size(), '.'));
    }

    int MapDirections()
    {
        auto start = StartingPoint();
        path[start.first][start.second] = 'X';
        auto path_finder_position = std::make_pair(start.first - 1, start.second);
        path[path_finder_position.first][path_finder_position.second] = 'X';
        auto previous_position = start;

        while (path_finder_position != start)
        {
            auto previous_step = path_finder_position;
            path_finder_position = FollowingStep(previous_position, path_finder_position, false);
            path[path_finder_position.first][path_finder_position.second] = 'X';
            previous_position = previous_step;
        }

        path_finder_position = std::make_pair(start.first - 1, start.second);
        previous_position = start;
        while (path_finder_position != start)
        {
            auto previous_step = path_finder_position;
            path_finder_position = FollowingStep(previous_position, path_finder_position, true);
            previous_position = previous_step;
        }

        while (Boundary())
        {
            for (size_t i = 0; i < path.size(); ++i)
            {
                for (size_t j = 0; j < path[i].size(); ++j)
                {
                    if (path[i][j] == 'L' || path[i][j] == 'R')
                    {
                        MoveForward(i, j, path[i][j]);
                    }
                }
            }
        }

        auto sum = 0;
        for (auto& row : path)
        {
            for (char c : row)
            {
                if (c == 'R')
                {
                    sum++;
                }
            }
        }

        return sum;
    }

private:
    std::vector<std::string> plan;
    std::vector<std::vector<char>> path;

    std::pair<int, int> StartingPoint()
    {
        for (size_t row = 0; row < plan.size(); ++row)
        {
            for (size_t column = 0; column < plan[row].size(); ++column)
            {
                if (plan[row][column] == 'S')
                {
                    return { row, column };
                }
            }
        }
    }

    bool Boundary()
    {
        for (auto& row : path)
        {
            for (char c : row)
            {
                if (c == '.')
                {
                    return true;
                }
            }
        }

        return false;
    }

    std::pair<int, int> FollowingStep(const std::pair<int, int>& previous_step, const std::pair<int, int>& current_step, bool boundary_found)
    {
        size_t current_row = current_step.first;
        size_t current_column = current_step.second;
        size_t previous_row = previous_step.first;
        size_t previous_column = previous_step.second;

        char edge;
        std::pair<int, int> aftermath;

        switch (plan[current_row][current_column])
        {
        case 'L':
            if (previous_row == current_row - 1 && previous_column == current_column)
            {
                aftermath = { current_row, current_column + 1 };
                edge = 'R';
            }
            else
            {
                aftermath = { current_row - 1, current_column };
                edge = 'L';
            }

            if (boundary_found)
            {
                if (current_column != 0 && path[current_row][current_column - 1] != 'X')
                {
                    path[current_row][current_column - 1] = edge;
                }

                if (current_row != path.size() - 1 && path[current_row + 1][current_column] != 'X')
                {
                    path[current_row + 1][current_column] = edge;
                }
            }
            break;
        case 'F':
            if (previous_row == current_row + 1 && previous_column == current_column)
            {
                aftermath = { current_row, current_column + 1 };
                edge = 'L';
            }
            else
            {
                aftermath = { current_row + 1, current_column };
                edge = 'R';
            }

            if (boundary_found)
            {
                if (current_column != 0 && path[current_row][current_column - 1] != 'X')
                {
                    path[current_row][current_column - 1] = edge;
                }

                if (current_row != 0 && path[current_row - 1][current_column] != 'X')
                {
                    path[current_row - 1][current_column] = edge;
                }
            }
            break;
        case '7':
            if (previous_row == current_row + 1 && previous_column == current_column)
            {
                aftermath = { current_row, current_column - 1 };
                edge = 'R';
            }
            else
            {
                aftermath = { current_row + 1, current_column };
                edge = 'L';
            }

            if (boundary_found)
            {
                if (current_column != path.size() - 1 && path[current_row][current_column + 1] != 'X')
                {
                    path[current_row][current_column + 1] = edge;
                }

                if (current_row != 0 && path[current_row - 1][current_column] != 'X')
                {
                    path[current_row - 1][current_column] = edge;
                }
            }
            break;
        case 'J':
            if (previous_row == current_row - 1 && previous_column == current_column)
            {
                aftermath = { current_row, current_column - 1 };
                edge = 'L';
            }
            else
            {
                aftermath = { current_row - 1, current_column };
                edge = 'R';
            }

            if (boundary_found)
            {
                if (current_column != path.size() - 1 && path[current_row][current_column + 1] != 'X')
                {
                    path[current_row][current_column + 1] = edge;
                }

                if (current_row != path.size() - 1 && path[current_row + 1][current_column] != 'X')
                {
                    path[current_row + 1][current_column] = edge;
                }
            }
            break;
        case '|':
            if (previous_row == current_row - 1 && previous_column == current_column)
            {
                return { current_row + 1, current_column };
            }
            else
            {
                return { current_row - 1, current_column };
            }
            break;
        case '-':
            if (previous_row == current_row && previous_column == current_column - 1)
            {
                return { current_row, current_column + 1 };
            }
            else if (previous_row == current_row && previous_column == current_column + 1)
            {
                return { current_row, current_column - 1 };
            }
            break;
        }

        return aftermath;
    }

    void MoveForward(size_t row, size_t column, char c)
    {
       if (row != 0 && path[row - 1][column] == '.')
       {
           path[row - 1][column] = c;
       }

       if (column != 0 && path[row][column - 1] == '.')
       {
           path[row][column - 1] = c;
       }

       if (column != path.size() - 1 && path[row][column + 1] == '.')
       {
           path[row][column + 1] = c;
       }

       if (row != path.size() - 1 && path[row + 1][column] == '.')
       {
           path[row + 1][column] = c;
       }
    }
};

int main()
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::ifstream input_file("pipes.txt");
    std::vector<std::string> input_scheme;

    if (!input_file.is_open())
    {
        std::cerr << "File cannot be opened!" << std::endl;
        return 1;
    }

    std::string line;
    while (std::getline(input_file, line))
    {
        input_scheme.push_back(line);
    }

    MapLayout find_tiles(input_scheme);
    std::cout << "There are " << find_tiles.MapDirections() 
              << " tiles enclosed by the loop!" << std::endl;

    return 0;

}
