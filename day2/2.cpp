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

bool isInvalid(const std::string& str) {
    const size_t half = str.size() / 2;
    for (uint64_t i{0}; i < half; ++i) {
        const std::string first = str.substr(0, i + 1);
        uint64_t match{};
        for (uint64_t j{0}; j < str.size(); j += first.size()) {
            const std::string second = str.substr(j, first.size());
            if (first == second) {
                ++match;
            } else {
                break;
            }
        }
        if ((match * first.size()) == str.size()) return true;
    }
    return false;
}

uint64_t checkRange(const uint64_t min, const uint64_t max) {
    uint64_t ret{};
    for (uint64_t i{min}; i <= max; ++i) {
        if (isInvalid(std::to_string(i))) {
            ret += i;
        }
    }
    return ret;
}

uint64_t solution(const std::string& input) {
    uint64_t ret{};
    std::regex re(R"((\d+)-(\d+),?)");
    std::string temp{input};
    for (std::smatch sm; std::regex_search(temp, sm, re);) {
        const uint64_t val1 = std::stoull(sm[1].str());
        const uint64_t val2 = std::stoull(sm[2].str());
        ret += checkRange(val1, val2);
        temp = sm.suffix();
    }
    return ret;
}

int main(int argc, char* argv[]) {
    printf("Answer: %lu\n", solution(getInput()));
    return 0;
}

