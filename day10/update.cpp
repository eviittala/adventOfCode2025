#include <algorithm>
#include <cstdint>
#include <iostream>
#include <numeric>
#include <vector>

bool update(std::vector<uint64_t*>& ptrs, const std::vector<uint64_t>& numbers,
            const uint64_t joltage) {
    if (ptrs.empty()) {
        return false;
    }
    const uint64_t sum = std::accumulate(std::begin(numbers), std::end(numbers), 0);
    if (sum < joltage) {
        *ptrs.at(0) = joltage - sum;
        return true;
    }

    if (numbers.back() == joltage) {
        return false;
    }

    *ptrs.front() += 1;
    while (joltage < std::accumulate(std::begin(numbers), std::end(numbers), 0)) {
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

    *ptrs.at(0) = joltage - std::accumulate(std::begin(numbers), std::end(numbers), 0);
    return true;
}

int main(void) {
    std::vector<uint64_t> numbers{0, 0, 0, 0, 0, 0};
    std::vector<uint64_t*> ptrs(6);
    ptrs.at(0) = &numbers.at(0);
    ptrs.at(1) = &numbers.at(1);
    ptrs.at(2) = &numbers.at(2);
    ptrs.at(3) = &numbers.at(3);
    ptrs.at(4) = &numbers.at(4);
    ptrs.at(5) = &numbers.at(5);
    //    ptrs.at(2) = &numbers.at(4);
    const uint64_t joltage = 55;

    while (update(ptrs, numbers, joltage)) {
        for (const auto nbr : numbers) {
            printf("%lu ", nbr);
        }
        printf("= %d\n", std::accumulate(std::begin(numbers), std::end(numbers), 0));
    }
    return 0;
}
