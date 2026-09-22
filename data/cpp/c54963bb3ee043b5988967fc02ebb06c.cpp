// Write a C++ function that takes three integers and returns a `std::array<int, 3>` containing the three numbers sorted in ascending order. The input integers can be negative, zero, or positive, and may include duplicate values. The function must not modify the original parameters, must use only constant extra space, and must handle all possible integer ranges within the `int` data type.
The simplest approach is to compute the minimum, maximum, and middle value directly. The minimum is obtained by comparing all three numbers, the maximum similarly, and the middle value is the total sum minus the minimum and maximum. This works because the sum of the three numbers minus the smallest and largest leaves exactly the middle one. Edge cases include all numbers equal (then min = max = middle), only two distinct values (the duplicate occupies either the min or max position), and negative numbers (no special handling needed, comparisons work naturally). There is no risk of integer overflow because `A + B + C` stays within the range of `int` (three `int` values sum to at most 3 × 2^31 − 1, which fits in a 64-bit integer, but to be safe we could use `long long` for the sum; however, since the problem guarantees valid `int` inputs, the sum will not overflow a standard 32-bit `int`? Actually, it can overflow: e.g., INT_MAX + INT_MAX + INT_MAX = 6,442,450,941 which exceeds 2,147,483,647. So to be safe, we compute the middle without summing all three using a comparison-based method instead, or use `long long`. We'll use `long long` for the sum to avoid overflow, or we can implement the middle as `max(min(A,B), min(max(A,B), C))` but that's more complex. Simplest: sort via `std::min` and `std::max` and compute middle with `std::min(std::max(A,B), std::max(A, std::min(B,C)))` but that's messy. We'll use `long long` sum for safety. Time complexity is O(1), space O(1). Alternative: use `std::array` and `std::sort`, but that would require copying, still O(1) space. We'll implement a free function that returns `std::array<int, 3>`.
#include <array>
#include <algorithm> // for std::min, std::max

// Returns the three input integers sorted in ascending order.
// Uses constant extra space and does not modify the inputs.
std::array<int, 3> sortThree(int a, int b, int c) {
    int smallest = std::min({a, b, c});
    int largest = std::max({a, b, c});
    // Use long long to safely compute the middle without overflow.
    long long sum = static_cast<long long>(a) + b + c;
    int middle = static_cast<int>(sum - smallest - largest);
    return {smallest, middle, largest};
}
#include <cassert>
#include <array>
#include <iostream>

// The solution function is declared above (omitted here for brevity, but must be included).

int main() {
    // Basic ordering
    std::array<int, 3> r1 = sortThree(3, 1, 2);
    assert(r1 == std::array<int, 3>{1, 2, 3});

    // Already sorted
    std::array<int, 3> r2 = sortThree(-5, 0, 5);
    assert(r2 == std::array<int, 3>{-5, 0, 5});

    // Reverse order
    std::array<int, 3> r3 = sortThree(10, 0, -10);
    assert(r3 == std::array<int, 3>{-10, 0, 10});

    // All equal
    std::array<int, 3> r4 = sortThree(7, 7, 7);
    assert(r4 == std::array<int, 3>{7, 7, 7});

    // Two equal smaller, one larger
    std::array<int, 3> r5 = sortThree(2, 2, 5);
    assert(r5 == std::array<int, 3>{2, 2, 5});

    // Two equal larger, one smaller
    std::array<int, 3> r6 = sortThree(1, 9, 9);
    assert(r6 == std::array<int, 3>{1, 9, 9});

    // Negative numbers and duplicates
    std::array<int, 3> r7 = sortThree(-3, -1, -1);
    assert(r7 == std::array<int, 3>{-3, -1, -1});

    // Large values to test overflow safety (INT_MAX, INT_MIN)
    std::array<int, 3> r8 = sortThree(INT_MAX, INT_MIN, 0);
    assert(r8 == std::array<int, 3>{INT_MIN, 0, INT_MAX});

    // Large positive and negative mix
    std::array<int, 3> r9 = sortThree(INT_MAX, INT_MAX, INT_MIN);
    assert(r9 == std::array<int, 3>{INT_MIN, INT_MAX, INT_MAX});

    // Another large mix
    std::array<int, 3> r10 = sortThree(INT_MAX, INT_MIN, INT_MIN);
    assert(r10 == std::array<int, 3>{INT_MIN, INT_MIN, INT_MAX});

    std::cout << "All tests passed!\n";
    return 0;
}
