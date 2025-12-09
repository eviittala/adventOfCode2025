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

using Point = std::tuple<int64_t, int64_t>;

std::vector<Point> makeVec(const std::string& input) {
    std::vector<Point> ret;
    std::istringstream is{input};
    std::regex re(R"((\d+),(\d+))");
    std::smatch sm;
    for (std::string line; std::getline(is, line);) {
        if (std::regex_search(line, sm, re)) {
            ret.push_back({std::stoll(sm[1].str()), std::stoll(sm[2].str())});
        }
    }
    return ret;
}

void printVec(const std::vector<Point>& vec) {
    for (const auto& line : vec) {
        const auto [x, y] = line;
        std::cout << x << ", " << y << std::endl;
    }
}

uint64_t calcArea(const Point p1, const Point p2) {
    const auto [x1, y1] = p1;
    const auto [x2, y2] = p2;
    return (std::abs(x2 - x1) + 1) * (std::abs(y2 - y1) + 1);
}

uint64_t solution(const std::string& input) {
    auto vec = makeVec(input);
    // printVec(vec);
    std::vector<std::tuple<uint64_t, Point, Point>> areas;
    for (size_t i{}; i < vec.size(); ++i) {
        for (size_t j{i + 1}; j < vec.size(); ++j) {
            areas.push_back(
                {calcArea(vec.at(i), vec.at(j)), vec.at(i), vec.at(j)});
        }
    }
    std::ranges::sort(areas, [](const auto& first, const auto& second) {
        return get<0>(first) > get<0>(second);
    });
    const auto [area, p1, p2] = areas.front();
    const auto [x1, y1] = p1;
    const auto [x2, y2] = p2;
    printf("Biggest area: %lu, {%ld, %ld}, {%ld, %ld}\n", area, x1, y1, x2, y2);
    return get<0>(areas.front());
}

int main(int argc, char* argv[]) {
    const auto start = std::chrono::system_clock::now();
    printf("Answer: %lu\n", solution(getInput()));
    const auto end = std::chrono::system_clock::now();
    const std::chrono::duration<double> diff = end - start;
    std::cout << "Test elapsed: " << diff << std::endl;
    return 0;
}

