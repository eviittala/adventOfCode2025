#include <algorithm>
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

std::vector<std::tuple<int64_t, int64_t, int64_t>> makeVec(
    const std::string& input) {
    std::vector<std::tuple<int64_t, int64_t, int64_t>> ret;
    std::istringstream is{input};
    std::regex re(R"((\d+),(\d+),(\d+))");
    std::smatch sm;
    for (std::string line; std::getline(is, line);) {
        if (std::regex_search(line, sm, re)) {
            ret.push_back({std::stoll(sm[1].str()), std::stoll(sm[2].str()),
                           std::stoll(sm[3].str())});
        }
    }
    return ret;
}

float calcDistance(const std::tuple<int64_t, int64_t, int64_t>& p1,
                   const std::tuple<int64_t, int64_t, int64_t>& p2) {
    const auto [x1, y1, z1] = p1;
    const auto [x2, y2, z2] = p2;

    auto ret = std::sqrt(std::pow(x2 - x1, 2) + std::pow(y2 - y1, 2) +
                         std::pow(z2 - z1, 2));
    return ret;
}

void printVec(const std::vector<std::tuple<int64_t, int64_t, int64_t>>& vec) {
    for (const auto& line : vec) {
        auto [x, y, z] = line;
        std::cout << x << ", " << y << ", " << z << std::endl;
    }
}

void addToCircuit(std::vector<std::vector<size_t>>& vecs, const size_t p1,
                  const size_t p2) {
    if (vecs.empty()) {
        std::vector<size_t> temp;
        temp.push_back(p1);
        temp.push_back(p2);
        vecs.push_back(std::move(temp));
        return;
    }
    for (size_t i{}; i < vecs.size(); ++i) {
        auto& vec = vecs.at(i);
        const auto it1 = std::ranges::find(vec, p1);
        const auto it2 = std::ranges::find(vec, p2);
        if (it1 != std::end(vec) and it2 == std::end(vec)) {
            vec.push_back(p2);
            return;
        } else if (it1 == std::end(vec) and it2 != std::end(vec)) {
            vec.push_back(p1);
            return;
        } else if (it1 != std::end(vec) and it2 != std::end(vec)) {
            return;
        }
    }
    std::vector<size_t> temp;
    temp.push_back(p1);
    temp.push_back(p2);
    vecs.push_back(std::move(temp));
}

std::vector<std::vector<size_t>> makeCircuits(
    const std::vector<std::tuple<float, size_t, size_t>> junctions,
    const size_t nbr) {
    std::vector<std::vector<size_t>> ret;
    for (size_t i{}; i < nbr; ++i) {
        const auto [dist, p1, p2] = junctions.at(i);
        // printf("makeCircuit dist: %f, %zu, %zu\n", dist, p1, p2);
        addToCircuit(ret, p1, p2);
    }
    bool combine{true};
    while (combine) {
        combine = false;
        for (size_t i{}; i < ret.size(); ++i) {
            for (size_t j{i + 1}; j < ret.size(); ++j) {
                for (const auto val : ret.at(i)) {
                    if (auto it = std::ranges::find(ret.at(j), val);
                        it != std::end(ret.at(j))) {
                        auto& vecFrom = ret.at(i);
                        auto& vecTo = ret.at(j);
                        std::copy(std::begin(vecFrom), std::end(vecFrom),
                                  std::back_inserter(vecTo));
                        vecFrom.clear();
                        std::ranges::sort(vecTo);
                        auto last = std::unique(vecTo.begin(), vecTo.end());
                        vecTo.erase(last, vecTo.end());
                        combine = true;
                        break;
                    }
                }
            }
        }
    }
    return ret;
}
uint64_t solution(const std::string& input) {
    auto vec = makeVec(input);
    // printVec(vec);
    std::vector<std::tuple<float, size_t, size_t>> junctions;
    for (size_t i{1}; i < vec.size(); ++i) {
        auto [x, y, z] = vec.at(i);
        std::cout << x << ", " << y << ", " << z
                  << " Dist: " << calcDistance(vec.at(0), vec.at(i))
                  << std::endl;
    }

    for (size_t i{0}; i < vec.size(); ++i) {
        for (size_t j{i + 1}; j < vec.size(); ++j) {
            junctions.emplace_back(calcDistance(vec.at(i), vec.at(j)), i, j);
        }
    }

    std::ranges::sort(junctions, [](const auto& first, const auto& second) {
        return get<0>(first) < get<0>(second);
    });

    std::cout << "PRINT first 10" << std::endl;

    // constexpr size_t index = 10;

    for (size_t i{}; i < junctions.size(); ++i) {
        auto [dist, p1, p2] = junctions.at(i);
        auto [x1, y1, z1] = vec.at(p1);
        auto [x2, y2, z2] = vec.at(p2);
        std::cout << i << ". " << x1 << ", " << y1 << ", " << z1 << " -> " << x2
                  << ", " << y2 << ", " << z2 << " Dist: " << dist << std::endl;
    }

    auto circuits = makeCircuits(junctions, junctions.size());
    std::ranges::sort(circuits, [](const auto& first, const auto& second) {
        return first.size() > second.size();
    });

    std::cout << "PRINT circuits" << std::endl;

    for (auto circuit : circuits) {
        std::cout << circuit.size() << ": ";
        for (auto& vec : circuit) {
            std::cout << vec << ", ";
        }
        std::cout << std::endl;
    }
    uint64_t ret{1};
    for (size_t i{}; i < 3; ++i) {
        ret *= circuits.at(i).size();
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

