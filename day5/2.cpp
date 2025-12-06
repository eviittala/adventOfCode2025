#include <cassert>
#include <chrono>
#include <cstdint>
#include <ctime>
#include <fstream>
#include <iostream>
#include <regex>
#include <set>
#include <sstream>
#include <unordered_map>
#include <vector>

std::string getInput()
{
    const std::string file = "input.txt";
    if (std::ifstream is{file, std::ios::ate})
    {
        auto        size = is.tellg();
        std::string str(size, '\0');
        is.seekg(0);
        is.read(&str[0], size);
        return str;
    }
    printf("Cannot open file: %s\n", file.c_str());
    return {};
}

std::vector<uint64_t> makeIdVec(const std::string& input)
{
    std::vector<uint64_t> ret;
    std::regex            re(R"(^\d+$)");
    std::istringstream    is{input};
    for (std::string line; std::getline(is, line);)
    {
        if (std::regex_search(line, re))
        {
            ret.push_back(std::stoull(line));
        }
    }
    return ret;
}

std::vector<std::pair<uint64_t, uint64_t>> makeRangesVec(const std::string& input)
{
    std::vector<std::pair<uint64_t, uint64_t>> ret;
    std::regex                                 re(R"(^(\d+)-(\d+)$)");
    std::istringstream                         is{input};
    std::smatch                                match;
    for (std::string line; std::getline(is, line);)
    {
        if (std::regex_search(line, match, re))
        {
            ret.push_back({std::stoull(match[1]), std::stoull(match[2])});
        }
    }
    return ret;
}

uint64_t countRange(const std::vector<std::pair<uint64_t, uint64_t>>& ranges)
{
    uint64_t ret{};
    for (auto [from, to] : ranges)
    {
        ret += (to - from + 1);
    }
    return ret;
}

void removeOverlappingRanges(std::vector<std::pair<uint64_t, uint64_t>>& ranges)
{
    std::sort(ranges.begin(), ranges.end(), [](const auto& a, const auto& b) { return a.first < b.first; });
    std::vector<std::pair<uint64_t, uint64_t>> result;
    auto                                       current = ranges[0];

    for (size_t i = 1; i < ranges.size(); ++i)
    {
        if (ranges[i].first <= current.second)
        {
            current.second = std::max(current.second, ranges[i].second);
        }
        else
        {
            result.push_back(current);
            current = ranges[i];
        }
    }
    result.push_back(current);
    ranges = std::move(result);
}

uint64_t solution(const std::string& input)
{
    auto ids    = makeIdVec(input);
    auto ranges = makeRangesVec(input);
    removeOverlappingRanges(ranges);
    return countRange(ranges);
}

int main(int argc, char* argv[])
{
    const auto start = std::chrono::system_clock::now();
    printf("Answer: %lu\n", solution(getInput()));
    const auto                          end  = std::chrono::system_clock::now();
    const std::chrono::duration<double> diff = end - start;
    std::cout << "Test elapsed: " << diff << std::endl;
    return 0;
}
