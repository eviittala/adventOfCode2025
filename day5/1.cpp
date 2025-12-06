#include <cassert>
#include <chrono>
#include <cstdint>
#include <ctime>
#include <fstream>
#include <iostream>
#include <regex>
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

bool isFresh(uint64_t id, const std::vector<std::pair<uint64_t, uint64_t>>& ranges)
{
    for (const auto& [from, to] : ranges)
    {
        if (id >= from && id <= to)
        {
            return true;
        }
    }
    return false;
}

uint64_t solution(const std::string& input)
{
    uint64_t ret{};
    auto     ids    = makeIdVec(input);
    auto     ranges = makeRangesVec(input);

    for (auto id : ids)
    {
        if (isFresh(id, ranges))
        {
            ++ret;
        }
    }

    return ret;
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
