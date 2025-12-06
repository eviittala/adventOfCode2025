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
    const std::regex re(R"(([*-+]))");
    std::smatch match;
    uint64_t i{};
    while (std::regex_search(line, match, re)) {
        if (i++ == idx) {
            return match[1].str()[0];
        }
        line = match.suffix();
    }
    assert(false);
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

bool isEmpty(const std::string& str) {
    for (auto c : str) {
        if (c != ' ') return false;
    }
    return true;
}

std::vector<std::vector<uint64_t>> getValues(
    const std::vector<std::string>& vec) {
    std::vector<std::vector<uint64_t>> ret;
    const auto nbrOfElems = getNbrOfElems(vec);
    ret.resize(nbrOfElems);
    size_t idx{nbrOfElems - 1};
    bool updateIdx = false;

    for (int64_t i = vec.at(0).size() - 1; i >= 0; --i) {
        std::string nbr;
        for (size_t j{}; j < vec.size() - 1; ++j) {
            nbr += vec.at(j).at(i);
        }
        if (!isEmpty(nbr)) {
            if (updateIdx) {
                --idx;
                updateIdx = false;
            }
            ret[idx].push_back(std::stoull(nbr));
        } else {
            updateIdx = true;
        }
    }

    return ret;
}

uint64_t getCalculation(const std::vector<uint64_t> vec, const uint64_t sign) {
    uint64_t ret{};

    for (const auto val : vec) {
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

uint64_t solution(const std::string& input) {
    uint64_t ret{};
    const auto vec = makeVec(input);
    auto values = getValues(vec);

    for (size_t i{}; i < values.size(); ++i) {
        ret += getCalculation(values.at(i), getSign(vec, i));
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

