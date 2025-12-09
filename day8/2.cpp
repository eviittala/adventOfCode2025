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

using Point = std::tuple<int64_t, int64_t, int64_t>;

std::vector<Point> makeVec(const std::string& input) {
    std::vector<Point> ret;
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

float calcDistance(const Point& p1, const Point& p2) {
    const auto [x1, y1, z1] = p1;
    const auto [x2, y2, z2] = p2;

    auto ret = std::sqrt(std::pow((x2 - x1), 2) + std::pow((y2 - y1), 2) +
                         std::pow((z2 - z1), 2));
    return ret;
}

void printVec(const std::vector<Point>& vec) {
    for (const auto& line : vec) {
        auto [x, y, z] = line;
        std::cout << x << ", " << y << ", " << z << std::endl;
    }
}

size_t getIndex(const std::vector<std::vector<Point>>& vec, const Point val) {
    for (size_t i{}; i < vec.size(); ++i) {
        if (auto it = std::ranges::find(vec.at(i), val);
            it != std::end(vec.at(i))) {
            return i;
        }
    }
    return vec.size();
}

std::vector<std::pair<Point, Point>> connections;

void addToCircuit(std::vector<std::vector<Point>>& vecs, const Point p1,
                  const Point p2) {
    if (vecs.empty()) {
        std::vector<Point> temp;
        temp.push_back(p1);
        temp.push_back(p2);
        vecs.push_back(std::move(temp));
        return;
    }
    const auto idx1 = getIndex(vecs, p1);
    const auto idx2 = getIndex(vecs, p2);
    if (idx1 != vecs.size() and idx2 != vecs.size() and idx1 != idx2) {
        auto& vecFrom = vecs.at(idx2);
        auto& vecTo = vecs.at(idx1);
        connections.emplace_back(p1, p2);
        std::copy(std::begin(vecFrom), std::end(vecFrom),
                  std::back_inserter(vecTo));
        vecs.erase(vecs.begin() + idx2);

        return;
    } else if (idx1 != vecs.size() and idx2 == vecs.size()) {
        connections.emplace_back(p1, p2);
        vecs.at(idx1).push_back(p2);
        return;
    } else if (idx1 == vecs.size() and idx2 != vecs.size()) {
        connections.emplace_back(p1, p2);
        vecs.at(idx2).push_back(p1);
        return;
    } else if (idx1 != vecs.size() and idx2 != vecs.size() and idx1 == idx2) {
        return;
    }
    connections.emplace_back(p1, p2);
    std::vector<Point> temp;
    temp.push_back(p1);
    temp.push_back(p2);
    vecs.push_back(std::move(temp));
}

void printCircuits(const std::vector<std::vector<Point>>& vecs) {
    for (const auto& vec : vecs) {
        for (const auto& val : vec) {
            std::cout << "{" << std::get<0>(val) << ", " << std::get<1>(val)
                      << ", " << std::get<2>(val) << "}, ";
        }
        std::cout << std::endl;
    }
}

std::vector<std::vector<Point>> makeCircuits(
    const std::vector<std::tuple<float, Point, Point>> junctions,
    const size_t nbr) {
    std::vector<std::vector<Point>> ret;
    for (size_t i{}; i < nbr; ++i) {
        const auto [dist, p1, p2] = junctions.at(i);
        addToCircuit(ret, p1, p2);
    }
    return ret;
}

uint64_t getLastConnectionResult() {
    const auto [p1, p2] = connections.back();
    const auto [x1, y1, z1] = p1;
    const auto [x2, y2, z2] = p2;
    return x1 * x2;
}

uint64_t solution(const std::string& input) {
    auto vec = makeVec(input);
    // printVec(vec);
    std::vector<std::tuple<float, Point, Point>> junctions;

    for (size_t i{0}; i < vec.size(); ++i) {
        for (size_t j{i + 1}; j < vec.size(); ++j) {
            junctions.emplace_back(calcDistance(vec.at(i), vec.at(j)),
                                   vec.at(i), vec.at(j));
        }
    }

    std::ranges::sort(junctions, [](const auto& first, const auto& second) {
        return get<0>(first) < get<0>(second);
    });

    auto circuits = makeCircuits(junctions, junctions.size());
    std::ranges::sort(circuits, [](const auto& first, const auto& second) {
        return first.size() > second.size();
    });

    return getLastConnectionResult();
}

int main(int argc, char* argv[]) {
    const auto start = std::chrono::system_clock::now();
    printf("Answer: %lu\n", solution(getInput()));
    const auto end = std::chrono::system_clock::now();
    const std::chrono::duration<double> diff = end - start;
    std::cout << "Test elapsed: " << diff << std::endl;
    return 0;
}

