#include <cstdint>
#include <fstream>
#include <iostream>
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

uint64_t move(int64_t& val, const int64_t rot, const uint8_t dir) {
    uint64_t ret{};
    if (dir == 0)
        for (int i{}; i < rot; ++i) {
            val = (val + 1) % 100;
            if (val == 0) ++ret;
        }
    else {
        for (int i{}; i < rot; ++i) {
            val = (val - 1);
            if (val == 0) ++ret;
            if (val < 0) val = 100 - std::abs(val);
        }
    }
    return ret;
}

uint64_t solution(const std::string& input) {
    int64_t val{50};
    uint64_t ret{};
    std::istringstream is{input};
    for (std::string line; std::getline(is, line);) {
        const int64_t rot = std::atoi(line.substr(1).c_str());
        ret += move(val, rot, line.at(0) == 'L' ? 1 : 0);
    }

    return ret;
}

int main(int argc, char* argv[]) {
    printf("Answer: %lu\n", solution(getInput()));
    return 0;
}
