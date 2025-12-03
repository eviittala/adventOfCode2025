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

uint64_t makeValue(const uint64_t val1, const uint64_t val2) {
    return (val1 << 32) | val2;
}

std::unordered_map<uint64_t, std::string> mem;

std::string getJoltage(const std::string& str, const uint64_t idx,
                       const uint64_t num) {
    if (num >= 11) {
        return str.substr(idx, 1);
    }
    const auto hash = makeValue(idx, num);
    if (mem.contains(hash)) {
        return mem[hash];
    }
    std::string ret{};
    const size_t size = str.size();

    for (size_t i{idx}; i < size; ++i) {
        for (size_t j{i + 1}; j < size; ++j) {
            const auto joltage = getJoltage(str, j, num + 1);
            if (joltage.size() == 0) break;
            std::string val = str.substr(i, 1);
            val += joltage;
            // printf("ret: %s - val: %s\n", ret.c_str(), val.c_str());
            if (ret.size() == 0 || (std::stoull(ret) < std::stoull(val))) {
                ret = val;
            }
        }
    }

    if (!mem.contains(hash)) {
        mem[hash] = ret;
    }
    return ret;
}

uint64_t solution(const std::string& input) {
    uint64_t ret{};
    std::istringstream is{input};
    for (std::string line; std::getline(is, line);) {
        mem.clear();
        const auto joltage = getJoltage(line, 0, 0);
        // printf("%s: %s\n", line.c_str(), joltage.c_str());
        ret += std::stoull(joltage);
    }
    return ret;
}

int main(int argc, char* argv[]) {
    // 171435596092638
    const auto start = std::chrono::system_clock::now();
    printf("Answer: %lu\n", solution(getInput()));
    const auto end = std::chrono::system_clock::now();
    const std::chrono::duration<double> diff = end - start;
    std::cout << "Test elapsed: " << diff << std::endl;
    return 0;
}

