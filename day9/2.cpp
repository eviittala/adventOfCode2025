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

const double epsilon = std::numeric_limits<float>().epsilon();
const std::numeric_limits<double> DOUBLE;
const double MIN = DOUBLE.min();
const double MAX = DOUBLE.max();
using Point = std::tuple<double, double>;

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

struct PointXY {
    const double x, y;
};

struct Edge {
    const PointXY a, b;

    bool operator()(const PointXY& p) const {
        if (a.y > b.y)
            return Edge{b, a}(p);
        if (p.y == a.y || p.y == b.y)
            return operator()({p.x, p.y + epsilon});
        if (p.y > b.y || p.y < a.y || p.x > std::max(a.x, b.x))
            return false;
        if (p.x < std::min(a.x, b.x))
            return true;
        const auto blue = abs(a.x - p.x) > MIN ? (p.y - a.y) / (p.x - a.x) : MAX;
        const auto red = abs(a.x - b.x) > MIN ? (b.y - a.y) / (b.x - a.x) : MAX;
        return blue >= red;
    }
};

std::tuple<Point, Point> shrinkArea(Point first, Point second) {
    auto& [x1, y1] = first;
    auto& [x2, y2] = second;
    double& xmin = x1 < x2 ? x1 : x2;
    xmin += epsilon;
    double& xmax = x1 < x2 ? x2 : x1;
    xmax -= epsilon;
    double& ymin = y1 < y2 ? y1 : y2;
    ymin += epsilon;
    double& ymax = y1 < y2 ? y2 : y1;
    ymax -= epsilon;
    return {{x1, y1}, {x2, y2}};
}
// bool pnpoly(int nvert, float *vertx, float *verty, float testx, float testy)
bool isPointInsidePolygon(const std::vector<Point>& points, const Point& test) {

    size_t c{};
    for (size_t i = 0, j = points.size() - 1; i < points.size(); j = i++) {
        const auto [vertx_j, verty_j] = points[j];
        const auto [vertx_i, verty_i] = points[i];
        const auto [testx, testy] = test;
        Edge edge{{vertx_j, verty_j}, {vertx_i, verty_i}};
        if (edge({testx, testy}))
            ++c;
    }

    // const auto [testx, testy] = test;
    //  printf("Point {%lf, %lf} is %sinside polygon\n", testx, testy, c % 2 != 0 ? "" : "not ");
    return c % 2 != 0;
    /*
    // constexpr float EPSILON = 0.001f;
    const auto [testx, testy] = point;
    const size_t nvert = points.size();
    size_t c{};
    size_t counter{};
    for (size_t i = 0, j = nvert - 1; i < nvert; j = i++) {
        const auto [vertx_i, verty_i] = points[i];
        const auto [vertx_j, verty_j] = points[j];
        if (vertx_i == testx or verty_i == testy)
            continue;
        if (vertx_j == testx or verty_j == testy)
            continue;

        if ((((float)verty_i > (float)testy) != ((float)verty_j > (float)testy)) &&
            ((float)testx <
             ((float)vertx_j - (float)vertx_i) * ((float)testy - (float)verty_i) / ((float)verty_j - (float)verty_i) +
                 (float)vertx_i)) {
            // if (std::abs((float)verty_j - (float)testy) < EPSILON ||
            //     std::abs((float)verty_i - (float)testy) < EPSILON) {
            //  printf("Skipping horizontal edge from {%f, %f} to {%f,
            //  %f}\n", vertx_i, verty_i, vertx_j, verty_j);:w

            //     continue;
            // }
            //            if (std::abs((float)vertx_j - (float)testx) < EPSILON
            //            && std::abs((float)vertx_i - (float)testx) < EPSILON)
            //            {
            //                //printf("Skipping vertical edge from {%f, %f} to
            //                {%f, %f}\n", vertx_i, verty_i, vertx_j, verty_j);
            //                continue;
            //            }
            c = !c;
            ++counter;

            // printf(
            //     "Edge from {%ld, %ld} to {%ld, %ld} crosses ray from {%ld, "
            //     "%ld} \n",
            //     vertx_i, verty_i, vertx_j, verty_j, testx, testy);
        }
    }
    // const auto [x, y] = point;
    // printf("Point {%ld, %ld} is %sinside polygon counter = %zu\n", testx,
    // testy,
    //       c == 1 ? "" : "not ", counter);
    return c == 1;
    */
}

uint64_t calcArea(const Point p1, const Point p2) {
    const auto [x1, y1] = p1;
    const auto [x2, y2] = p2;
    return (std::abs(x2 - x1) + 1) * (std::abs(y2 - y1) + 1);
}

std::vector<Point> getAllPolygonPoints(const Point P1, const Point P2) {
    std::vector<Point> ret;
    const auto [p1, p2] = shrinkArea(P1, P2);
    const auto [x1, y1] = p1;
    const auto [x2, y2] = p2;
    for (double x = std::min(x1, x2); x <= std::max(x1, x2); ++x) {
        ret.push_back({x, y1});
        ret.push_back({x, y2});
    }
    for (double y = std::min(y1, y2) + 1; y < std::max(y1, y2); ++y) {
        ret.push_back({x1, y});
        ret.push_back({x2, y});
    }
    return ret;
}

uint64_t solution(const std::string& input) {
    const auto vec = makeVec(input);
    // printVec(vec);
    std::vector<std::tuple<uint64_t, Point, Point>> areas;
    for (size_t i{}; i < vec.size(); ++i) {
        for (size_t j{i + 1}; j < vec.size(); ++j) {
            areas.push_back({calcArea(vec.at(i), vec.at(j)), vec.at(i), vec.at(j)});
        }
    }
    std::ranges::sort(areas, [](const auto& first, const auto& second) { return get<0>(first) > get<0>(second); });
    // for (size_t i{69000}; i < areas.size(); ++i) {
    for (size_t i{45000}; i < areas.size(); ++i) {
        // for (size_t i{0}; i < areas.size(); ++i) {
        const auto& area = areas.at(i);
        const auto [p1, p2] = shrinkArea(get<1>(area), get<2>(area));
        const auto [x1, y1] = p1;
        const auto [x2, y2] = p2;
        // const auto [x2, y2] = get<2>(area);
        if (isPointInsidePolygon(vec, {x1, y1}) && isPointInsidePolygon(vec, {x2, y2}) &&
            isPointInsidePolygon(vec, {x1, y2}) && isPointInsidePolygon(vec, {x2, y1})) {
            bool valid = true;
            for (const auto& point : getAllPolygonPoints(get<1>(area), get<2>(area))) {
                if (!isPointInsidePolygon(vec, point)) {
                    valid = false;
                    break;
                }
            }

            if (valid) {
                const uint64_t size = get<0>(area);
                printf("Biggest area: %lu, {%lf, %lf}, {%lf, %lf}\n", size, x1, y1, x2, y2);
                return size;
            }
            printf("Area %lu is invalid (%zu/%zu)\n", get<0>(area), i, areas.size());
        }
    }
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

// 4755064176 too high
// 3046667767 too high
// 1929330  too low
// 1613305596 Right answer
