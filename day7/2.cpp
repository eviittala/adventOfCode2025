#include <cassert>
#include <chrono>
#include <cstdint>
#include <ctime>
#include <fstream>
#include <iostream>
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

std::vector<std::string> makeVec(const std::string& input) {
    std::vector<std::string> ret;
    std::istringstream is{input};
    for (std::string line; std::getline(is, line);) {
        ret.push_back(line);
    }
    return ret;
}

void printVec(const std::vector<std::string>& vec) {
    for (const auto& line : vec) {
        std::cout << line << std::endl;
    }
}

size_t getStartingPoint(const std::vector<std::string>& vec) {
    for (size_t i{}; i < vec.at(0).size(); ++i) {
        if (vec.at(0).at(i) == 'S') {
            return i;
        }
    }
    assert(false);
}

uint64_t solution(const std::string& input) {
    uint64_t ret{};
    const auto vec = makeVec(input);
    printVec(vec);

    std::set<size_t> beams{getStartingPoint(vec)};
    // ret += beams.size();

    for (size_t l{1}; l < vec.size(); ++l) {
        bool beamsAdded = false;
        for (size_t i{}; i < vec.at(l).size(); ++i) {
            const char c = vec.at(l).at(i);
            if (c == '^') {
                if (const auto beam = beams.find(i); beam != beams.end()) {
                    beams.erase(beam);
                    ++ret;
                    if (0 < i) {
                        if (beams.insert(i - 1).second) /* ++ret*/
                            ;
                    }
                    if (i < vec.at(l).size()) {
                        if (beams.insert(i + 1).second) /*++ret*/
                            ;
                    }
                    beamsAdded = true;
                }
            }
        }
        if (beamsAdded) {
            printf("line: %zu size: %zu ret: %lu\n", l, beams.size(), ret);
            // ret += beams.size();
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

