#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <optional>
#include <regex>
#include <string>
#include <vector>

struct Rule
{
    char left;
    char operation;
    int right;
    std::string result;
};

struct Workflow
{
    std::string name;
    std::vector<Rule> rules;
    std::string end;
};

struct WorkflowParts
{
    std::map<std::string, Workflow> workflows;
    std::vector<std::map<char, int>> ratings;
};

struct Intersection
{
    size_t range() const { return b - a + 1; }
    void intersect(const Intersection& latter) { a = std::max(a, latter.a); b = std::min(b, latter.b); };
    int64_t a, b;
};

static auto
Analyse(const std::string& line, const std::regex& reg) -> std::vector<std::string>
{
    return { std::sregex_token_iterator(line.begin(), line.end(), reg), std::sregex_token_iterator() };
}

static auto 
ProcessInput(std::ifstream& input_file) -> WorkflowParts
{
    WorkflowParts workflow_parts;
    std::string line;
    bool analyse_parts = false;

    while (std::getline(input_file, line))
    {
        if (line.empty())
        {
            analyse_parts = true;
            continue;
        }

        auto parsed_strings = Analyse(line, std::regex(analyse_parts ? "[\\w]+|[=]" : "[\\w]+|[<>]"));
        if (analyse_parts)
        {
            std::map<char, int> parts;
            for (size_t i = 0; i < parsed_strings.size(); i += 3)
            {

                parts[parsed_strings[i][0]] = std::stoi(parsed_strings[i + 2]);
            }

            workflow_parts.ratings.push_back(std::move(parts));
        }
        else
        {
            Workflow work_flow{ .name = parsed_strings[0] };
            for (size_t i = 1; i < parsed_strings.size() - 1; i += 4)
            {
                work_flow.rules.push_back(Rule {
                        .left = parsed_strings[i][0],
                        .operation = parsed_strings[i + 1][0],
                        .right = std::stoi(parsed_strings[i + 2]),
                        .result = parsed_strings[i + 3]
                    });
            }

            work_flow.end = parsed_strings.back();
            workflow_parts.workflows[work_flow.name] = std::move(work_flow);
        }
    }

    return workflow_parts;
}

static auto 
DataSorting(const std::map<std::string, Workflow>& workflows, const std::map<char, int>& parts,
            const std::string& name, int position_index, size_t& total_sum) -> std::optional<size_t>
{
    if (name == "A")
    {
        total_sum += parts.at('x') + parts.at('m') + parts.at('a') + parts.at('s');
        return std::nullopt;
    }
    else if (name == "R")
    {
        return std::nullopt;
    }

    if (position_index == workflows.at(name).rules.size())
    {
        DataSorting(workflows, parts, workflows.at(name).end, 0, total_sum);
    }
    else
    {
        auto operation = [](int left, char operation, int right) -> bool {
            return operation == '<' ? left < right : left > right;
            };

        auto& rule = workflows.at(name).rules[position_index];
        if (operation(parts.at(rule.left), rule.operation, rule.right))
        {
            DataSorting(workflows, parts, rule.result, 0, total_sum);
        }
        else
        {
            DataSorting(workflows, parts, name, position_index + 1, total_sum);
        }
    }
}

static auto
DataSelected(const std::map<char, Intersection>& xmas_map, const std::map<std::string, Workflow>& workflows,
             const std::string& name, int position_index, size_t& total_sum) -> std::optional<size_t>
{
    if (name == "A")
    {
        total_sum += xmas_map.at('x').range() * xmas_map.at('m').range() *
                     xmas_map.at('a').range() * xmas_map.at('s').range();

        return std::nullopt;
    }
    else if (name == "R")
    {
        return std::nullopt;
    }

    if (position_index == workflows.at(name).rules.size())
    {
        DataSelected(xmas_map, workflows, workflows.at(name).end, 0, total_sum);
    }
    else
    {
        auto& rule = workflows.at(name).rules[position_index];
        bool less_than = rule.operation == '<';

        std::map<char, Intersection> another_map = xmas_map;
        another_map[rule.left].intersect(less_than ? Intersection{ 1, rule.right - 1 } : Intersection{ rule.right + 1, 4000 });
        DataSelected(another_map, workflows, rule.result, 0, total_sum);

        std::map<char, Intersection> yet_another_map = xmas_map;
        yet_another_map[rule.left].intersect(less_than ? Intersection{ rule.right, 4000 } : Intersection{ 1, rule.right });
        DataSelected(yet_another_map, workflows, name, position_index + 1, total_sum);
    }

    return std::nullopt;
}

static auto 
DistinctCombinations(const WorkflowParts& analysed_data) -> size_t
{
    size_t total_sum = 0;
    std::map<char, Intersection> xmas_map{ {'x', {1, 4000}}, {'m', {1, 4000}}, {'a', {1, 4000}}, {'s', {1, 4000}} };
    DataSelected(xmas_map, analysed_data.workflows, "in", 0, total_sum);

    return total_sum;
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::ifstream input_file("part_ratings.txt");

    if (!input_file.is_open())
    {
        std::cerr << "File cannot be opened!\n";
        return 1;
    }

    auto analysed_data = ProcessInput(input_file);
    std::cout << "Number of distinct combinations of ratings: " << DistinctCombinations(analysed_data) << '\n';

    return 0;
}
