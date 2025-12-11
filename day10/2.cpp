#include <algorithm>
#include <bitset>
#include <cassert>
#include <chrono>
#include <cmath>
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
    // std::regex re(R"((\d+),(\d+))");
    std::smatch sm;
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

std::string getOutput(const std::string& line) {
    std::regex re(R"(\[(.*)\])");
    std::string ret;
    std::smatch sm;
    if (std::regex_search(line, sm, re)) {
        ret = sm[1];
    }
    return ret;
}

std::vector<uint64_t> getJoltage(const std::string& line) {
    std::regex re(R"(\{(.*)\})");
    std::vector<uint64_t> ret;
    std::smatch sm;
    if (std::regex_search(line, sm, re)) {
        std::string temp = sm[1];
        while (std::regex_search(temp, sm, std::regex(R"((\d+))"))) {
            ret.push_back(std::stoull(sm[1]));
            temp = sm.suffix();
        }
    }
    return ret;
}

std::vector<std::string> getButtons(const std::string& line) {
    std::regex re(R"(\((.*?)\))");
    std::vector<std::string> ret;
    std::string temp(line);
    std::smatch sm;
    while (std::regex_search(temp, sm, re)) {
        ret.push_back(sm[1]);
        temp = sm.suffix();
    }
    return ret;
}

void toggleCounterWithButtons(std::vector<uint64_t>& counter, const std::string& button) {
    for (const auto nbr : button) {
        if (nbr < '0' || nbr > '9') {
            continue;
        }
        counter.at(nbr - '0') += 1;
    }
}

uint64_t makeUint32t(const std::string& display) {
    uint64_t key{};
    for (auto it = display.rbegin(); it != display.rend(); ++it) {
        key <<= 1;
        if (*it == '#') {
            key |= 1;
        }
    }
    return key;
}

int compare(const std::vector<uint64_t>& expected, const std::vector<uint64_t>& counter) {
    int ret{0};
    for (size_t i{}; i < expected.size(); ++i) {
        if (counter.at(i) < expected.at(i)) {
            ret = -1;
        } else if (counter.at(i) > expected.at(i)) {
            return 1;
        }
    }
    return ret;
}

uint64_t findFewestButtonsPressed(const std::vector<uint64_t>& expected,
                                  std::vector<uint64_t> counter,
                                  const std::vector<std::string>& buttons, const uint64_t presses) {
    const auto result = compare(expected, counter);
    if (result == 0) {
        return presses;
    }
    if (result == 1) {
        return UINT64_MAX;
    }
    if (presses > 10) {
        return UINT64_MAX;
    }

    uint64_t minPresses{UINT64_MAX};

    for (const auto& button : buttons) {
        auto newCounter = counter;
        toggleCounterWithButtons(newCounter, button);
        const auto nbrOfPresses =
            findFewestButtonsPressed(expected, newCounter, buttons, presses + 1);
        if (nbrOfPresses < minPresses) {
            minPresses = nbrOfPresses;
        }
    }

    return minPresses;
}

uint64_t getNbrOfButtonPresses(const std::string& line) {
    const auto joltages = getJoltage(line);
    const auto buttons = getButtons(line);
    std::vector<uint64_t> counter(joltages.size());
    printf("Buttons:\n");
    printVec(buttons);
    printf("Joltage: \n");
    for (auto joltage : joltages) {
        std::cout << joltage << ", ";
    }
    std::cout << std::endl;
    // uint64_t nbrOfPresses{UINT64_MAX};
    const auto nbrOfPresses = findFewestButtonsPressed(joltages, counter, buttons, {});
    // uint64_t maxPresses = 1;
    //  do {
    //      nbrOfPresses = findFewestButtonsPressed(makeUint32t(output), {}, buttons, 0,
    //      maxPresses++);
    //  } while (nbrOfPresses == UINT64_MAX);

    return nbrOfPresses;
}

uint64_t solution(const std::string& input) {
    const auto vec = makeVec(input);
    // printVec(vec);
    uint64_t ret{};
    for (const auto& line : vec) {
        const auto nbrOfPresses = getNbrOfButtonPresses(line);
        printf("%s nbrOfPresses: %lu\n", line.c_str(), nbrOfPresses);
        ret += nbrOfPresses;
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

// 15000 too high
