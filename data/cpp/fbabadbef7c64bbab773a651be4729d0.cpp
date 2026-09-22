Write a C++ function named `splitIntoTwoAlmostEqualParts` that takes a single positive integer `n` and returns a `std::pair<int, int>` representing two non-negative integers `x` and `y` such that `x + y = n`, and the absolute difference `|x - y|` is as small as possible. If there are two possible solutions with the same minimal difference (which happens when `n` is even), return the one where `x >= y`. For example, for `n = 10`, both `(5,5)` and `(6,4)` have difference 0 and 2 respectively, so the minimal difference is 0, resulting in `(5,5)`. For `n = 11`, the minimal difference is 1, and the result should be `(6,5)`, not `(5,6)`. The function should handle both small and large positive integers (up to `10^18`), and must not use division by zero or negative inputs. The problem is inspired by the code snippet that computes `x = ceil(n/3)` and `y = floor(n/3)`, but here the goal is simply to split `n` into two parts as evenly as possible.
// The main task is to find two integers `x` and `y` such that `x + y = n` and `|x - y|` is minimized. The most balanced split occurs when both parts are as close to `n/2` as possible. For any integer `n`, the optimal x and y are given by:
// - `x = ceil(n / 2)` and `y = floor(n / 2)`.
//
// Since `x >= y` is required, we set `x = (n + 1) / 2` using integer division (which automatically rounds up), and `y = n / 2` (floor division). For even `n`, `x = y = n/2`. For odd `n`, `x = (n+1)/2` and `y = (n-1)/2`, giving a difference of exactly 1, which is minimal. Edge cases: `n = 1` results in `(1, 0)`, `n = 2` results in `(1, 1)`, and very large `n` up to `10^18` fits safely in a 64-bit signed integer. The algorithm is constant time, `O(1)`, using only a few integer operations and no auxiliary space.
#include <cstdint>
#include <utility>

// Returns a pair (x, y) such that x + y = n and |x - y| is minimal,
// with x >= y when both are equally close.
std::pair<std::int64_t, std::int64_t> splitIntoTwoAlmostEqualParts(std::int64_t n) {
    const std::int64_t x = (n + 1) / 2;  // ceil(n/2)
    const std::int64_t y = n / 2;        // floor(n/2)
    return {x, y};
}
#include <cassert>
#include <cstdint>

// Include the solution function declaration here (or include the header).
std::pair<std::int64_t, std::int64_t> splitIntoTwoAlmostEqualParts(std::int64_t n);

int main() {
    assert(splitIntoTwoAlmostEqualParts(1) == std::make_pair<std::int64_t, std::int64_t>(1, 0));
    assert(splitIntoTwoAlmostEqualParts(2) == std::make_pair<std::int64_t, std::int64_t>(1, 1));
    assert(splitIntoTwoAlmostEqualParts(3) == std::make_pair<std::int64_t, std::int64_t>(2, 1));
    assert(splitIntoTwoAlmostEqualParts(10) == std::make_pair<std::int64_t, std::int64_t>(5, 5));
    assert(splitIntoTwoAlmostEqualParts(11) == std::make_pair<std::int64_t, std::int64_t>(6, 5));
    assert(splitIntoTwoAlmostEqualParts(1000000000000000000LL) == std::make_pair<std::int64_t, std::int64_t>(500000000000000000LL, 500000000000000000LL));
    assert(splitIntoTwoAlmostEqualParts(999999999999999999LL) == std::make_pair<std::int64_t, std::int64_t>(500000000000000000LL, 499999999999999999LL));
    // Ensure sum is always n and difference is minimal
    for (std::int64_t n = 0; n <= 100; ++n) {
        auto [x, y] = splitIntoTwoAlmostEqualParts(n);
        assert(x + y == n);
        assert(x >= y);
        assert(x - y <= 1);
    }
    return 0;
}
