Write a C++ function named `findMinMax` that takes three integer numbers as input and returns a `std::pair<int, int>` where the first element is the minimum and the second element is the maximum. The function must use only conditional logic (if-else or switch-case) without any loops or library help for min/max. It should handle cases where two or all three numbers are equal. Also, ensure the function is `const`-correct and does not modify its arguments.
The solution compares the three numbers using a series of if-else conditions. The simplest approach: first assume `a` is both min and max, then compare with `b` and `c` and adjust accordingly. But to strictly follow the "if-else" requirement and avoid min/max libraries, we can perform explicit comparisons:
- Initialize `min = a`, `max = a`.
- If `b < min`, set `min = b`; else if `b > max`, set `max = b`.
- Similarly for `c`.
This is O(1) time and O(1) space. Edge cases: equal numbers — the conditions handle them correctly because we use strict `<` and `>` so equal values do not trigger updates, leaving the first encountered as both. Negative numbers are handled naturally. The function returns a pair, which is a clean way to bundle two results.
#include <utility> // for std::pair

// Returns {minimum, maximum} of three integers.
// Uses only if-else comparisons; no loops or min/max helpers.
std::pair<int, int> findMinMax(int a, int b, int c) {
    int min_val = a;
    int max_val = a;

    if (b < min_val) {
        min_val = b;
    } else if (b > max_val) {
        max_val = b;
    }

    if (c < min_val) {
        min_val = c;
    } else if (c > max_val) {
        max_val = c;
    }

    return {min_val, max_val};
}
#include <cassert>
#include <utility>

// Free function declared here for testing
std::pair<int, int> findMinMax(int a, int b, int c);

int main() {
    // Basic order
    assert(findMinMax(1, 2, 3) == std::make_pair(1, 3));
    assert(findMinMax(3, 2, 1) == std::make_pair(1, 3));
    assert(findMinMax(2, 3, 1) == std::make_pair(1, 3));

    // Negative numbers
    assert(findMinMax(-5, -1, -10) == std::make_pair(-10, -1));
    assert(findMinMax(0, -3, 2) == std::make_pair(-3, 2));

    // Equal values
    assert(findMinMax(7, 7, 7) == std::make_pair(7, 7));
    assert(findMinMax(5, 5, 3) == std::make_pair(3, 5));
    assert(findMinMax(5, 3, 5) == std::make_pair(3, 5));
    assert(findMinMax(3, 5, 5) == std::make_pair(3, 5));

    // Mixed signs and zeros
    assert(findMinMax(-2, 0, 2) == std::make_pair(-2, 2));
    assert(findMinMax(0, 0, -1) == std::make_pair(-1, 0));

    // Larger magnitude values
    assert(findMinMax(1000000, -1000000, 0) == std::make_pair(-1000000, 1000000));

    return 0;
}
