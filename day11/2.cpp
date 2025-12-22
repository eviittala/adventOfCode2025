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

uint64_t getPaths(const std::map<std::string, std::vector<std::string>>& map,
                  const std::string& key) {
    if (key == "out") {
        return 1;
    }

    if (!map.contains(key)) {
        return 0;
    }

    uint64_t ret{};

    for (const auto& path : map.at(key)) {
        ret += getPaths(map, path);
    }
    return ret;
}

uint64_t solution(const std::string& input) {
    const auto map = makeMap(input);
    // printMap(map);
    const std::string start = "you";
    uint64_t ret = getPaths(map, start);
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

