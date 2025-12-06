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

std::vector<std::string> makeVec(const std::string& input) {
    std::vector<std::string> ret;
    std::istringstream is{input};
    for (std::string line; std::getline(is, line);) {
        ret.push_back(line);
    }
    return ret;
}

char getSign(const std::vector<std::string>& vec, const uint64_t idx) {
    std::string line = vec.back();
    // std::cout << line << std::endl;
    const std::regex re(R"(([*-+]))");
    std::smatch match;
    uint64_t i{};
    while (std::regex_search(line, match, re)) {
        // printf("%lu %lu %s\n", i, idx, match[1].str().c_str());
        if (i++ == idx) {
            return match[1].str()[0];
        }
        line = match.suffix();
    }
    assert(false);
}

uint64_t getValue(const std::string& line, const uint64_t idx) {
    std::string line_t = line;
    const std::regex re(R"((\d+))");
    std::smatch match;
    uint64_t i{};
    while (std::regex_search(line_t, match, re)) {
        if (i++ == idx) {
            // std::cout << match[1].str() << std::endl;
            return std::stoull(match[1].str());
        }
        line_t = match.suffix();
    }
    assert(false);
}

uint64_t getCalculation(const std::vector<std::string> vec,
                        const uint64_t idx) {
    const char sign = getSign(vec, idx);
    uint64_t ret{};

    for (size_t i{}; i < vec.size() - 1; ++i) {
        const auto val = getValue(vec.at(i), idx);
        // printf("VAL: %lu sign: %c\n", val, sign);
        switch (sign) {
            case '+':
                ret += val;
                break;
            case '*':
                if (ret == 0) {
                    ret = val;
                } else {
                    ret *= val;
                }
                break;
            case '-':
                ret -= val;
                break;
            default:
                assert(false);
                break;
        }
    }
    return ret;
}

size_t getNbrOfElems(const std::vector<std::string>& vec) {
    std::string line = vec.at(0);
    const std::regex re(R"(\d+)");
    std::smatch match;
    size_t ret{};
    while (std::regex_search(line, match, re)) {
        ++ret;
        line = match.suffix();
    }
    return ret;
}

uint64_t solution(const std::string& input) {
    uint64_t ret{};
    const auto vec = makeVec(input);
    for (size_t i{}; i < getNbrOfElems(vec); ++i) {
        const uint64_t res = getCalculation(vec, i);
        // printf("%zu Res: %lu\n", i, res);
        ret += res;
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

