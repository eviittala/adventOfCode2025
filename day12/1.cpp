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

std::map<uint64_t, std::array<uint8_t, 3>> makePatternsMap(const std::string& input) {
    std::map<uint64_t, std::array<uint8_t, 3>> ret;
    // std::istringstream is{input};
    std::string is{input};
    std::regex re(R"((\d+):\n)");
    std::smatch sm;

    while (std::regex_search(is, sm, re)) {
        const uint64_t nbr = *sm[1].str().c_str() - '0';
        // printf("nbr: %d\n", *nbr - '0');
        is = sm.suffix();
        auto& arr = ret[nbr];
        for (size_t i{}, j{}; i < (4 * 3); ++i) {
            // std::cout << is.at(i);
            if (is.at(i) == '#' or is.at(i) == '.') {
                arr[j] <<= 1;
                arr[j] |= is.at(i) == '#' ? 1 : 0;
            } else if (is.at(i) == '\n') {
                ++j;
            }
        }
        // is = is.at(4 * 3);
        // printf("rest : %s\n", is.c_str());
    }
    return ret;
}

void printMap(const std::map<uint64_t, std::array<uint8_t, 3>>& map) {
    for (const auto& [key, arr] : map) {
        std::cout << key << ": " << std::endl;
        for (const auto& p : arr) {
            std::bitset<3> bits{p};
            std::cout << bits << std::endl;
        }
        std::cout << std::endl;
    }
}

void printTask(const std::vector<std::string>& task) {
    for (const auto& line : task) {
        std::cout << line << std::endl;
    }
}

std::vector<std::string> getTask(const std::string& input) {
    std::vector<std::string> ret;
    std::string is{input};
    std::regex re(R"((\d+x\d+:.*)\n?)");
    std::smatch sm;
    while (std::regex_search(is, sm, re)) {
        ret.push_back(sm[1].str());
        is = sm.suffix();
    }
    return ret;
}

std::tuple<uint64_t, uint64_t> getGridSize(const std::string& str) {
    std::regex re(R"((\d+)x(\d+):.*)");
    std::smatch sm;
    if (std::regex_search(str, sm, re)) {
        return {std::stoull(sm[1]), std::stoull(sm[2])};
    }
    return {0, 0};
}

std::vector<uint64_t> getShapes(const std::string& str) {
    std::vector<uint64_t> ret;
    std::regex re(R"(\s(\d+))");
    std::smatch sm;
    std::string is(str);
    while (std::regex_search(is, sm, re)) {
        ret.push_back(std::stoull(sm[1]));
        is = sm.suffix();
    }
    return ret;
}

uint64_t solution(const std::string& input) {
    const auto map = makePatternsMap(input);
    // printMap(map);
    const auto task = getTask(input);
    // printTask(task);
    uint64_t ret{};
    for (const auto& line : task) {
        const auto [x, y] = getGridSize(line);
        const auto shapes = getShapes(line);
        // std::cout << x << ", " << y << std::endl;
        const auto sum = std::accumulate(std::begin(shapes), std::end(shapes), 0);
        // std::cout << std::endl;
        // printf("std::flooor: %f\n", std::floor((double)x / 3));
        // printf("std::flooor2: %f\n", std::floor((double)y / 3));

        // printf("%d : %f\n", sum, (std::floor((double)x / 3) * std::floor((double)y / 3)));
        if (sum <= (std::floor((double)x / 3) * std::floor((double)y / 3))) {
            ++ret;
        }
    }
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

