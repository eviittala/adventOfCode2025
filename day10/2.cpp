#include <algorithm>
#include <bitset>
#include <cassert>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <ctime>
#include <fstream>
#include <iostream>
#include <numeric>
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

std::map<uint64_t, std::vector<std::string>>
generateButtonCombinations(const std::vector<std::string>& buttons) {
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

uint64_t getMatches(const std::vector<size_t>& first, const std::vector<size_t>& second) {
    uint64_t ret{};
    for (const auto nbr : first) {
        ret += std::ranges::count(second, nbr);
    }
    return ret;
}

std::map<uint64_t, std::pair<std::vector<size_t>, uint64_t>>
generateButtonCombinationsIdx(const std::vector<std::string>& buttons,
                              const std::vector<uint64_t>& joltages) {
    std::map<uint64_t, std::pair<std::vector<size_t>, uint64_t>> combinations;

    for (size_t idx{}; idx < buttons.size(); ++idx) {
        for (const auto nbr : buttons.at(idx)) {
            if (nbr < '0' || nbr > '9') {
                continue;
            }
            combinations[nbr - '0'].first.push_back(idx);
            combinations[nbr - '0'].second = joltages.at(nbr - '0');
        }
    }
    /* for some reason did not work
    std::vector<std::pair<size_t, uint64_t>> matches;
    for (size_t i{0}; i < combinations.size(); ++i) {
        uint64_t count{};
        for (size_t j{0}; j < combinations.size(); ++j) {
            if (i != j) {
                count += getMatches(combinations.at(i).first, combinations.at(j).first);
            }
        }
        matches.emplace_back(i, count);
    }
    std::map<uint64_t, std::pair<std::vector<size_t>, uint64_t>> combinations_t;
    std::ranges::sort(matches, [](const auto a, const auto b) { return a.second < b.second; });
    size_t i{};
    for (const auto& [idx, count] : matches) {
        combinations_t[i++] = combinations.at(idx);
    }
    combinations = std::move(combinations_t);
    */

    bool run{true};
    while (run) {
        run = false;
        for (size_t i{0}; i < combinations.size() - 1; ++i) {
            if (combinations.at(i).first.size() > combinations.at(i + 1).first.size()) {
                std::swap(combinations[i], combinations[i + 1]);
                run = true;
            }
        }
    }
    run = true;
    while (run) {
        run = false;
        for (size_t i{0}; i < combinations.size() - 2; ++i) {
            if (combinations.at(i + 1).first.size() == combinations.at(i + 2).first.size()) {
                const auto first =
                    getMatches(combinations.at(i).first, combinations.at(i + 1).first);
                const auto second =
                    getMatches(combinations.at(i).first, combinations.at(i + 2).first);
                // printf("%zu. first: %lu, second: %lu\n", i, first, second);
                if (second > first) {
                    std::swap(combinations[i + 1], combinations[i + 2]);
                    run = true;
                }
            }
        }
    }
    return combinations;
}

uint64_t getNbrOfIdx(const std::map<uint64_t, std::pair<std::vector<size_t>, uint64_t>>& combs) {
    std::set<uint64_t> ret{};
    for (const auto& [idx, comb] : combs) {
        for (const auto& nbr : comb.first) {
            ret.insert(nbr);
        }
    }
    return ret.size();
}

void printNumbers(const std::vector<uint64_t>& numbers) {
    printf("Numbers(%zu): ", numbers.size());
    for (size_t i{}; i < numbers.size(); ++i) {
        printf("(%zu) = %lu", i, numbers.at(i));
        if (i < numbers.size() - 1) {
            printf(", ");
        }
    }
    printf("\n");
}

uint64_t getNumbers(const std::vector<uint64_t>& numbers) {
    uint64_t ret{};
    for (const auto i : numbers) {
        ret += i;
    }
    return ret;
}

bool isCombValid(const std::vector<size_t>& combs, const std::vector<uint64_t>& numbers,
                 const uint64_t joltage) {
    uint64_t val{};
    for (const auto idx : combs) {
        val += numbers.at(idx);
    }
    return val == joltage;
}

uint64_t accumulateComb(const std::vector<size_t>& combs, const std::vector<uint64_t>& numbers) {
    uint64_t ret{};
    for (const auto idx : combs) {
        ret += numbers.at(idx);
    }
    return ret;
}

bool update(const std::vector<uint64_t*>& ptrs, const std::vector<uint64_t>& numbers,
            const std::vector<size_t>& combs, const uint64_t joltage) {
    if (ptrs.empty()) {
        return false;
    }
    const uint64_t sum = accumulateComb(combs, numbers);
    if (sum < joltage) {
        *ptrs.at(0) = joltage - sum;
        return true;
    }

    // if (numbers.back() == joltage) {
    //     return false;
    // }

    *ptrs.front() += 1;
    while (joltage < accumulateComb(combs, numbers)) {
        if (ptrs.size() == 1) {
            return false;
        }
        for (size_t i{1}; i < ptrs.size(); ++i) {
            if (0 < *ptrs.at(i - 1)) {
                *ptrs.at(i) += 1;
                *ptrs.at(i - 1) = 0;
                break;
            } else if (i == (ptrs.size() - 1)) {
                return false;
            }
        }
    }

    *ptrs.at(0) = joltage - accumulateComb(combs, numbers);
    return true;
}

void setZeroToNumbers(const std::vector<uint64_t*>& ptrsToNbr) {
    for (const auto ptr : ptrsToNbr) {
        *ptr = 0;
    }
}

uint64_t getFewestPressesFast(
    const std::map<uint64_t, std::pair<std::vector<size_t>, uint64_t>>& combinations,
    std::vector<uint64_t>& numbers, const std::map<size_t, std::vector<uint64_t*>>& ptrsToNbr,
    const uint64_t idx) {

    if (idx == combinations.size()) {
        const auto presses = getNumbers(numbers);
        printf("Found! idx: %lu presses: %lu\n", idx, presses);
        // printNumbers(numbers);
        return presses;
    }

    const auto& combs = combinations.at(idx);
    const auto joltage = combinations.at(idx).second;
    if (ptrsToNbr.contains(idx)) {
        setZeroToNumbers(ptrsToNbr.at(idx));
    }
    if (!isCombValid(combs.first, numbers, joltage)) {
        // printf("Comb not valid idx: %zu constains: %d\n", idx, ptrsToNbr.contains(idx));
        if (ptrsToNbr.contains(idx)) {
            if (!update(ptrsToNbr.at(idx), numbers, combs.first, joltage)) {
                // printf("cannot update values\n");
                return UINT64_MAX;
            }
        } else {
            return UINT64_MAX;
        }
    }
    // if (idx > combinations.size() - 6) {
    // printf("Scanning idx: %lu : ", idx);
    // printNumbers(numbers);
    // }

    uint64_t ret{UINT64_MAX};
    do {
        const auto presses = getFewestPressesFast(combinations, numbers, ptrsToNbr, idx + 1);
        if (presses < ret) {
            ret = presses;
        }
    } while (ptrsToNbr.contains(idx) && update(ptrsToNbr.at(idx), numbers, combs.first, joltage));

    return ret;
}

std::map<size_t, std::vector<uint64_t*>>
makePtrsToUse(const std::map<uint64_t, std::pair<std::vector<size_t>, uint64_t>>& combinations,
              std::vector<uint64_t>& numbers) {
    std::map<size_t, std::vector<uint64_t*>> retPtrs;
    for (size_t i{}; i < combinations.size(); ++i) {
        // printf("IDX: %lu -  ", i);
        for (const auto nbrIdx : combinations.at(i).first) {
            if (numbers.at(nbrIdx) == UINT64_MAX) {
                // printf("%lu, ", nbrIdx);
                retPtrs[i].push_back(&numbers.at(nbrIdx));
                numbers.at(nbrIdx) = 0;
            }
        }
        // printf("\n");
    }
    return retPtrs;
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
    printf("Joltages: ");
    for (auto joltage : joltages) {
        std::cout << joltage << ", ";
    }
    std::cout << std::endl;
    const auto combinations = generateButtonCombinationsIdx(buttons, joltages);
    for (const auto& [i, combination] : combinations) {
        printf("Combinations(%zu) size: %ld: ", i, combination.first.size());
        for (const auto& combo : combination.first) {
            std::cout << "(" << combo << "), ";
        }
        printf(" = %lu\n", combination.second);
    }

    std::vector<uint64_t> numbers(getNbrOfIdx(combinations));
    std::ranges::fill(numbers, UINT64_MAX);
    const auto ptrsToUse = makePtrsToUse(combinations, numbers);
    //    for (const auto& [idx, ptrs] : ptrsToUse) {
    //        printf("%zu. ptrs size: %zu - ", idx, ptrs.size());
    //        for (const auto nbrPtr : ptrs) {
    //            printf("%lu, ", *nbrPtr);
    //        }
    //        printf("\n");
    //    }
    return getFewestPressesFast(combinations, numbers, ptrsToUse, 0);
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
// 15835 too high
