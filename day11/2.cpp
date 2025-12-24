#include <algorithm>
#include <bitset>
#include <cassert>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <ctime>
#include <fstream>
#include <iostream>
#include <numeric>
#include <regex>
#include <set>
#include <sstream>
#include <unordered_map>
#include <vector>

std::string getInput() {
    const std::string file = "input.txt";
    if (std::ifstream is{file, std::ios::ate}) {
        auto size = is.tellg();
        std::string str(size, '\0');
        is.seekg(0);
        is.read(&str[0], size);
        return str;
    }
    printf("Cannot open file: %s\n", file.c_str());
    return {};
}

std::map<std::string, std::vector<std::string>> makeMap(const std::string& input) {
    std::map<std::string, std::vector<std::string>> ret;
    std::istringstream is{input};
    // std::regex re(R"((\d+),(\d+))");
    // std::smatch sm;
    for (std::string line; std::getline(is, line);) {
        const auto idx = line.find(':');
        const auto key = line.substr(0, idx);
        std::istringstream rest(line.substr(idx + 2));
        for (std::string r; std::getline(rest, r, ' ');) {
            ret[key].push_back(r);
        }
    }
    return ret;
}

void printMap(const std::map<std::string, std::vector<std::string>>& map) {
    for (const auto& [key, line] : map) {
        std::cout << key << ": ";
        for (const auto& p : line) {
            std::cout << p << " ";
        }
        std::cout << std::endl;
    }
}

std::map<std::string, uint64_t> mem;

uint64_t getPaths(const std::map<std::string, std::vector<std::string>>& map,
                  const std::string& start, const std::string& end) {
    if (start == end) {
        return 1;
    }

    if (!map.contains(start)) {
        return 0;
    }

    if (mem.contains(start))
        return mem.at(start);

    uint64_t ret{};

    for (const auto& path : map.at(start)) {
        ret += getPaths(map, path, end);
    }
    if (!mem.contains(start)) {
        mem[start] = ret;
    }
    return ret;
}

uint64_t solution(const std::string& input) {
    const auto map = makeMap(input);
    // printMap(map);
    const auto first = getPaths(map, "svr", "dac");
    mem.clear();
    const auto second = getPaths(map, "dac", "fft");
    mem.clear();
    const auto third = getPaths(map, "fft", "out");
    mem.clear();
    const auto fourth = getPaths(map, "svr", "fft");
    mem.clear();
    const auto fifth = getPaths(map, "fft", "dac");
    mem.clear();
    const auto sixth = getPaths(map, "dac", "out");
    mem.clear();
    uint64_t ret = first * second * third + fourth * fifth * sixth;
    return ret;
}

int main(int argc, char* argv[]) {
    const auto start = std::chrono::system_clock::now();
    printf("Answer: %lu\n", solution(getInput()));
    const auto end = std::chrono::system_clock::now();
    const std::chrono::duration<double> diff = end - start;
    std::cout << "Test elapsed: " << diff << std::endl;
    return 0;
}

