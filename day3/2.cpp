#include <cassert>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
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

std::string getJoltage(const std::string& str, const uint64_t idx,
                       const uint64_t num) {
    if (num >= 11) {
        return str.substr(idx, 1);
    }
    std::string ret{"0"};
    const size_t size = str.size();

    for (size_t i{idx}; i < size; ++i) {
        for (size_t j{i + 1}; j < size; ++j) {
            std::string val = str.substr(i, 1);
            const auto joltage = getJoltage(str, j, num + 1);
            if (joltage.size() == 1 && joltage[0] == '0') break;
            val += joltage;
            // printf("ret: %s - val: %s\n", ret.c_str(), val.c_str());
            if (std::stoull(ret) < std::stoull(val)) {
                ret = val;
            }
        }
    }
    return ret;
}

uint64_t solution(const std::string& input) {
    uint64_t ret{};
    std::istringstream is{input};
    for (std::string line; std::getline(is, line);) {
        const auto joltage = getJoltage(line, 0, 0);
        printf("%s: %s\n", line.c_str(), joltage.c_str());
        ret += std::stoull(joltage);
    }
    return ret;
}

int main(int argc, char* argv[]) {
    printf("Answer: %lu\n", solution(getInput()));
    return 0;
}

