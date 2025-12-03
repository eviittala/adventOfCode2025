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

uint64_t getJoltage(const std::string& str) {
    uint64_t ret{};
    const size_t size = str.size();

    for (size_t i{}; i < size; ++i) {
        for (size_t j{i + 1}; j < size; ++j) {
            char valStr[2]{};
            valStr[0] = str[i];
            valStr[1] = str[j];
            const uint64_t val = std::stoull(valStr);
            if (ret < val) {
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
        // printf("%s\n", line.c_str());
        ret += getJoltage(line);
    }
    return ret;
}

int main(int argc, char* argv[]) {
    printf("Answer: %lu\n", solution(getInput()));
    return 0;
}
