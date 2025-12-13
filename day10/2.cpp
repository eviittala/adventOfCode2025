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

/*
void addCounterWithButtons(std::vector<uint64_t>& counter,
                           const std::string& button) {
    printf("Adding button (%s) to counter\n", button.c_str());
    for (const auto nbr : button) {
        if (nbr < '0' || nbr > '9') {
            continue;
        }
        counter.at(nbr - '0') += 1;
    }
}*/

void addCounterWithButtons(std::vector<uint64_t>& counter, const size_t idx) {
    counter.at(idx) += 1;
}

int compare(const std::vector<uint64_t>& expected,
            const std::vector<uint64_t>& counter) {
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

std::map<uint64_t, std::vector<std::string>> generateButtonCombinations(
    const std::vector<std::string>& buttons) {
    std::map<uint64_t, std::vector<std::string>> combinations;

    for (const auto& button : buttons) {
        for (const auto nbr : button) {
            if (nbr < '0' || nbr > '9') {
                continue;
            }
            combinations[nbr - '0'].push_back(button);
        }
    }
    return combinations;
}

std::map<uint64_t, std::vector<size_t>> generateButtonCombinationsIdx(
    const std::vector<std::string>& buttons) {
    std::map<uint64_t, std::vector<size_t>> combinations;

    for (size_t idx{}; idx < buttons.size(); ++idx) {
        for (const auto nbr : buttons.at(idx)) {
            if (nbr < '0' || nbr > '9') {
                continue;
            }
            combinations[nbr - '0'].push_back(idx);
        }
    }
    return combinations;
}

bool hasExceeded(const std::vector<uint64_t>& joltages,
                 const std::vector<uint64_t>& pressed,
                 const std::map<uint64_t, std::vector<size_t>>& combinations) {
    for (const auto& [i, combs] : combinations) {
        const auto joltage = joltages.at(i);
        uint64_t totalPresses{};
        for (const auto& comb : combs) {
            totalPresses += pressed.at(comb);
        }
        if (totalPresses > joltage) {
            return true;
        }
    }
    return false;
}

uint64_t getPresses(const std::vector<uint64_t>& presses) {
    uint64_t ret{};
    for (const auto press : presses) {
        ret += press;
    }
    return ret;
}

std::vector<uint64_t> getJoltagesFromPressed(
    const std::vector<std::string>& buttons,
    const std::vector<uint64_t>& presses, const size_t size) {
    std::vector<uint64_t> ret(size);  // TODO Eero
    // printf("Calculating joltages from presses: ");
    // for (const auto press : presses) {
    //     std::cout << press << ", ";
    // }
    // std::cout << std::endl;
    for (size_t i{}; i < presses.size(); ++i) {
        const auto button = buttons.at(i);
        for (const auto nbr : button) {
            if (nbr < '0' || nbr > '9') {
                continue;
            }
            ret.at(nbr - '0') += presses.at(i);
        }
    }
    /*
    printf("Joltages from pressed: ");
    for (const auto joltage : ret) {
        std::cout << joltage << ", ";
    }
    std::cout << std::endl;
    */

    return ret;
}

uint64_t getFewestButtonsPressed(
    const std::vector<uint64_t>& joltages,
    const std::vector<std::string>& buttons, std::vector<uint64_t> presses,
    const std::map<uint64_t, std::vector<size_t>>& combinations) {
    if (hasExceeded(joltages, presses, combinations)) {
        return UINT64_MAX;
    }

    if (joltages == getJoltagesFromPressed(buttons, presses, joltages.size())) {
        // printf("Found valid presses: \n");
        return getPresses(presses);
    }
    uint64_t ret{UINT64_MAX};

    for (size_t idx{}; idx < buttons.size(); ++idx) {
        auto newPresses = presses;
        addCounterWithButtons(newPresses, idx);
        const auto nbrOfPresses = getFewestButtonsPressed(
            joltages, buttons, newPresses, combinations);
        if (nbrOfPresses < ret) {
            ret = nbrOfPresses;
        }
    }

    return ret;
}

uint64_t getNbrOfButtonPresses(const std::string& line) {
    const auto joltages = getJoltage(line);
    const auto buttons = getButtons(line);
    std::vector<uint64_t> pressed(buttons.size());
    printf("Buttons: ");
    for (auto button : buttons) {
        std::cout << "(" << button << "), ";
    }
    std::cout << std::endl;
    // printVec(buttons);
    printf("Joltage: ");
    for (auto joltage : joltages) {
        std::cout << joltage << ", ";
    }
    std::cout << std::endl;
    const auto combinations = generateButtonCombinationsIdx(buttons);
    for (const auto& [i, combination] : combinations) {
        printf("Combinations for %ld: ", i);
        for (const auto& combo : combination) {
            std::cout << "(" << combo << "), ";
        }
        printf(" = %lu\n", joltages.at(i));
    }
    /*
    const auto combinations = generateButtonCombinations(buttons);
    for (const auto& [i, combination] : combinations) {
        printf("Combinations for %ld: ", i);
        for (const auto& combo : combination) {
            std::cout << "(" << combo << "), ";
        }
        printf(" = %lu\n", joltages.at(i));
        // std::cout << std::endl;
    }*/
    const auto nbrOfPresses =
        getFewestButtonsPressed(joltages, buttons, pressed, combinations);

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
