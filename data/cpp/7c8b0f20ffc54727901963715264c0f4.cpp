Write a standalone C++ function named `analyzePairRange` that takes a `std::pair<int, int>` and returns a `std::tuple<int, int, int>`. The tuple must contain, in order: the absolute difference between the two pair elements, the smaller value, and the larger value. If the two values are equal, the difference must be zero and both the smaller and larger elements must be that same value. The function must be `const`-correct, handle negative numbers correctly for the absolute difference (use `std::abs`), and must not modify the input pair. A single free function is required; do not include a `main` function in the solution section.

// The solution extracts the two integers from the `std::pair`, computes their absolute difference using `std::abs` (which works for negative values via integer arithmetic), and determines the minimum and maximum using `std::min` and `std::max`. For equal values, both min and max are that same value, and the difference is zero—no special case is needed. Edge cases include negative numbers (e.g., `{-5, 3}` gives difference `8`, min `-5`, max `3`) and zeros. The algorithm uses only constant-time operations, so time complexity is \(O(1)\) and auxiliary space is \(O(1)\) (the returned tuple is a fixed-size object). The input is passed by `const` reference to avoid copying and ensure the original pair is unchanged.

#include <tuple>
#include <utility>
#include <algorithm>
#include <cstdlib>

// Given a pair of integers, return a tuple containing:
//   first  - absolute difference between the two values
//   second - the smaller of the two values
//   third  - the larger of the two values
// For equal values, difference is zero and both min and max are that value.
std::tuple<int, int, int> analyzePairRange(const std::pair<int, int>& p) {
    int diff = std::abs(p.first - p.second);
    int smaller = std::min(p.first, p.second);
    int larger = std::max(p.first, p.second);
    return std::make_tuple(diff, smaller, larger);
}

#include <cassert>
#include <tuple>
#include <utility>

// Declare the solution function (assumed to be in the same translation unit).
std::tuple<int, int, int> analyzePairRange(const std::pair<int, int>& p);

int main() {
    // Basic positive values
    auto r1 = analyzePairRange({3, 7});
    assert(std::get<0>(r1) == 4);
    assert(std::get<1>(r1) == 3);
    assert(std::get<2>(r1) == 7);

    // Negative and positive mix
    auto r2 = analyzePairRange({-5, 3});
    assert(std::get<0>(r2) == 8);
    assert(std::get<1>(r2) == -5);
    assert(std::get<2>(r2) == 3);

    // Both negative
    auto r3 = analyzePairRange({-10, -3});
    assert(std::get<0>(r3) == 7);
    assert(std::get<1>(r3) == -10);
    assert(std::get<2>(r3) == -3);

    // Equal values
    auto r4 = analyzePairRange({4, 4});
    assert(std::get<0>(r4) == 0);
    assert(std::get<1>(r4) == 4);
    assert(std::get<2>(r4) == 4);

    // Reversed order input
    auto r5 = analyzePairRange({9, 2});
    assert(std::get<0>(r5) == 7);
    assert(std::get<1>(r5) == 2);
    assert(std::get<2>(r5) == 9);

    // Zero and negative
    auto r6 = analyzePairRange({0, -2});
    assert(std::get<0>(r6) == 2);
    assert(std::get<1>(r6) == -2);
    assert(std::get<2>(r6) == 0);

    // Large magnitude numbers (difference may overflow int? here it's safe)
    auto r7 = analyzePairRange({1000000, -1000000});
    assert(std::get<0>(r7) == 2000000);
    assert(std::get<1>(r7) == -1000000);
    assert(std::get<2>(r7) == 1000000);
}
