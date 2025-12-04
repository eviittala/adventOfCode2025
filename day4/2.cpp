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

bool isRollAccessable(const std::vector<std::string>& lines, const int64_t x,
                      const int64_t y) {
    uint64_t hits{};
    for (auto XY : std::vector<std::pair<uint64_t, uint64_t>>{{-1, 1},
                                                              {0, 1},
                                                              {1, 1},
                                                              {-1, 0},
                                                              {1, 0},
                                                              {-1, -1},
                                                              {0, -1},
                                                              {1, -1}}) {
        try {
            if (lines.at(y + XY.second).at(x + XY.first) == '@') {
                ++hits;
            }
        } catch (const std::out_of_range&) {
        }
    }

    return hits < 4;
}

uint64_t solution(const std::string& input) {
    uint64_t ret{};
    auto lines = makeVec(input);
    const int64_t lineSize = lines.at(0).size();
    const int64_t size = lines.size();

    while (true) {
        std::vector<std::pair<int64_t, int64_t>> delXY;
        for (int64_t y{}; y < size; ++y) {
            for (int64_t x{}; x < lineSize; ++x) {
                if (lines.at(y).at(x) == '@' && isRollAccessable(lines, x, y)) {
                    ++ret;
                    delXY.push_back({x, y});
                }
            }
        }

        if (delXY.empty()) break;

        for (auto XY : delXY) lines.at(XY.second).at(XY.first) = '.';
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

