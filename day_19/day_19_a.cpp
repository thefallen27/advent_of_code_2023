#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <tuple>
#include <vector>

struct Rule
{
    std::string category;
    std::string operation;
    int value;
    std::string workflow;
};

static Rule
ConvertToRule(const std::string& rule_workflow)
{
    Rule rule;
    if (rule_workflow.find(":") == std::string::npos)
    {
        rule.category = "";
        rule.operation = "";
        rule.value = 0;
        rule.workflow = rule_workflow;
    
        return rule;
    }

    size_t position_index = rule_workflow.find(":");
    std::string before_index = rule_workflow.substr(0, position_index);
    rule.workflow = rule_workflow.substr(position_index + 1);

    for (const std::string& angle : { "<", ">" })
    {
        size_t angle_position = before_index.find(angle);
        if (angle_position != std::string::npos)
        {
            rule.category = before_index.substr(0, angle_position);
            rule.value = std::stoi(before_index.substr(angle_position + 1));
            rule.operation = angle;
        
            return rule;
        }
    }

    return rule;
}


int main()
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::ifstream input_file("part_ratings.txt");

    if (!input_file.is_open())
    {
        std::cerr << "File cannot be opened!" << std::endl;
        return 1;
    }

    std::map<std::string, std::vector<Rule>> rules;
    std::string line;
    while (getline(input_file, line) && !line.empty())
    {
        std::stringstream ss(line);
        std::string name, rule_data;
        getline(ss, name, '{');
        getline(ss, rule_data, '}');

        std::vector<Rule> rule_vector;
        std::stringstream rule_stream(rule_data);
        std::string actual_rule;
        while (getline(rule_stream, actual_rule, ','))
        {
            rule_vector.push_back(ConvertToRule(actual_rule));
        }

        rules[name] = rule_vector;
    }

    long long sum_of_ratings = 0;
    while (getline(input_file, line) && !line.empty())
    {
        std::stringstream value_data(line.substr(1, line.size() - 2));
        std::map<std::string, int> current_value;
        long total = 0;
        std::string current_item;
        while (getline(value_data, current_item, ','))
        {
            size_t equal_index = current_item.find("=");
            std::string name = current_item.substr(0, equal_index);
            int value = stoi(current_item.substr(equal_index + 1));
            current_value[name] = value;
            total += value;
        }

        std::string workflow = "in";
        while (workflow != "A" && workflow != "R")
        {
            for (const auto& rule : rules[workflow])
            {
                if (rule.operation.empty() ||
                    (rule.operation == "<" && current_value[rule.category] < rule.value) ||
                    (rule.operation == ">" && current_value[rule.category] > rule.value))
                {
                    workflow = rule.workflow;
                    break;
                }
            }
        }

        if (workflow == "A")
        {
            sum_of_ratings += total;
        }
    }

    std::cout << "The sum of all the rating numbers for all parts is " << sum_of_ratings << std::endl;

    return 0;
}
