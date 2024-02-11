#include <algorithm>
#include <deque>
#include <fstream>
#include <iostream>
#include <numeric>
#include <ranges>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

enum ModuleType : char
{
	Conjunction,
	FlipFlop,
	Neither
};

struct Module
{
	std::unordered_map<std::string, bool> input_data;
	std::vector<std::string> destination;
	ModuleType module_type = ModuleType::Neither;
	bool flip_flop_on = false;
};

struct Pulse
{
	std::string source;
	std::string sink;
	bool high = false;
};

static void
ReadInputFile(const std::string& filename, std::unordered_map<std::string, Module>& modules,
					std::vector<std::string>& conjunctions, std::string& destination_rx)
{
	std::ifstream input_file(filename);
	if (!input_file.is_open())
	{
		std::cerr << "File cannot be opened!" << std::endl;
		exit(1);
	}

	std::string line;
	while (input_file >> line)
	{
		std::string module_name = line;
		input_file >> line;
		std::getline(input_file, line);
		std::stringstream ss(line);
		std::string output_data;
		Module module_;

		while (std::getline(ss, output_data, ','))
		{
			module_.destination.push_back(output_data.substr(1));
		}

		if (module_name.starts_with('%'))
		{
			module_name.erase(0, 1);
			module_.module_type = ModuleType::FlipFlop;
		}
		else if (module_name.starts_with('&'))
		{
			module_name.erase(0, 1);
			module_.module_type = ModuleType::Conjunction;
			conjunctions.push_back(module_name);
		}

		if (std::ranges::find(module_.destination, "rx") != module_.destination.end())
		{
			destination_rx = module_name;
		}

		modules[module_name] = std::move(module_);
	}
}

static void
ConfigureModules(std::unordered_map<std::string, Module>& modules,
					const std::vector<std::string>& conjunctions)
{
	for (auto& [module_name, module_] : modules)
	{
		for (const auto& iterator : module_.destination)
		{
			if (std::ranges::find(conjunctions, iterator) != conjunctions.end())
			{
				modules[iterator].input_data[module_name] = false;
			}
		}
	}
}

static void
TransmitPulse(std::unordered_map<std::string, Module>& modules, const std::string& destination_rx)
{
	int64_t total_number_of_pulses = 0;
	int64_t fewest_number_of_presses = 1;
	int64_t number_of_pulses = 0;
	int64_t number_of_lows = 0, number_of_highs = 0;
	std::deque<Pulse> pulse_queue;
	std::unordered_map<std::string, std::int64_t> search;
	
	for (auto& [input, _] : modules[destination_rx].input_data)
	{
		search[input] = 0;
	}

	auto transmit_pulse = [&]()
		{
			pulse_queue.emplace_back("button", "broadcaster", false);
			while (!pulse_queue.empty())
			{
				auto& [source, sink, high] = pulse_queue.front();
				high ? ++number_of_highs : ++number_of_lows;

				if (auto it = modules.find(sink); it != modules.end())
				{
					auto& module_ = it->second;
					switch (module_.module_type)
					{
					case ModuleType::Neither:
						for (const auto& d : module_.destination)
						{
							pulse_queue.emplace_back(sink, d, high);
						}
						break;
					case ModuleType::FlipFlop:
						if (!high)
						{
							module_.flip_flop_on = !module_.flip_flop_on;
							for (const auto& dest : module_.destination)
							{
								pulse_queue.emplace_back(sink, dest, module_.flip_flop_on);
							}
						}
						break;
					default:
						if (auto iter = module_.input_data.find(source); iter != module_.input_data.end())
						{
							iter->second = high;
						}

						bool allHigh = std::ranges::all_of(module_.input_data, [](const auto& input) { return input.second; });

						if (search.contains(sink) && search[sink] == 0 && !allHigh)
						{
							search[sink] = number_of_pulses;
						}

						for (const auto& dest : module_.destination)
						{
							pulse_queue.emplace_back(sink, dest, !allHigh);
						}
					}
				}

				pulse_queue.pop_front();
			}
		};

	bool contains_zero;
	do
	{
		++number_of_pulses;
		transmit_pulse();
		if (number_of_pulses == 1000)
		{
			total_number_of_pulses = number_of_lows * number_of_highs;
		}

		contains_zero = std::ranges::any_of(search, [](const auto& s) { return s.second == 0; });

	} while (contains_zero);

	for (const auto& [_, cycle] : search)
	{
		fewest_number_of_presses = std::lcm(fewest_number_of_presses, cycle);
	}

	std::cout << "Total number of pulses sent (multiplied): " << total_number_of_pulses << std::endl;
	std::cout << "Fewest number of presses required for single low pulse: "
			  << fewest_number_of_presses << std::endl;
}

int main()
{
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(nullptr);

	std::unordered_map<std::string, Module> modules;
	std::vector<std::string> conjunctions;
	std::string destination_rx;

	ReadInputFile("modular_configuration.txt", modules, conjunctions, destination_rx);
	ConfigureModules(modules, conjunctions);
	TransmitPulse(modules, destination_rx);

	return 0;
}
