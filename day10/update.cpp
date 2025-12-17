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
    std::vector<uint64_t> numbers{0, 1, 1, 3, 2};
    std::vector<uint64_t*> ptrs(1);
    ptrs.at(0) = &numbers.at(0);
    //    ptrs.at(1) = &numbers.at(3);
    //    ptrs.at(2) = &numbers.at(4);
    const uint64_t joltage = 10;

    while (update(ptrs, numbers, joltage)) {
        printf("%lu %lu %lu %lu %lu = %d\n", numbers.at(0), numbers.at(1), numbers.at(2),
               numbers.at(3), numbers.at(4),
               std::accumulate(std::begin(numbers), std::end(numbers), 0));
    }
    return 0;
}
