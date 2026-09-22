// Write a C++ function that takes a positive integer `n` and returns a `std::vector<int>` containing two positive integers `a` and `b` such that `a + b == n`, and neither `a` nor `b` contains the digit `0` in its decimal representation. If multiple valid pairs exist, return the pair with the smallest possible `a`. The input is guaranteed to be at least 2 and at most 10,000. If no such pair exists (which will not happen for the given constraints), return an empty vector.

The solution uses a brute-force linear scan. Starting from `a = 1` up to `n - 1`, for each candidate `a`, check whether `a` and `b = n - a` both have no zero digits. The helper function `no_zero(int x)` repeatedly extracts the last digit using modulo 10 and checks if it equals 0, then divides the number by 10 to remove that digit. The loop continues until the number becomes 0. Since the problem guarantees at least one valid pair for all inputs, the first found pair (with smallest `a`) is returned. The algorithm runs in O(n * d) time, where `d` is the number of digits in `n`, and in the worst case `d` is about 5 (since n ≤ 10,000), so the complexity is effectively O(n). Space complexity is O(1) besides the output vector. Edge cases include `n = 2`, where the only pair is `1 + 1`; and values like `n = 10`, where `a = 1` works because `9` has no zero, but `a = 2` would fail because `8` is valid, but `1` is smaller so it returns first. Also note that numbers like 10, 20, 100, etc. contain zero and are rejected by the helper.

#include <vector>

// Helper: returns true if the decimal representation of x contains no digit '0'.
bool no_zero(int x) {
    while (x > 0) {
        if (x % 10 == 0) {
            return false;
        }
        x /= 10;
    }
    return true;
}

// Returns a vector {a, b} such that a + b = n and both a and b have no zero digits.
// Returns the pair with the smallest possible a.
std::vector<int> getNoZeroIntegers(int n) {
    for (int a = 1; a < n; ++a) {
        int b = n - a;
        if (no_zero(a) && no_zero(b)) {
            return {a, b};
        }
    }
    return {}; // Should never happen for valid constraints.
}

#include <cassert>
#include <vector>

int main() {
    assert(getNoZeroIntegers(2) == std::vector<int>({1, 1}));
    assert(getNoZeroIntegers(3) == std::vector<int>({1, 2}));
    assert(getNoZeroIntegers(10) == std::vector<int>({1, 9}));
    assert(getNoZeroIntegers(11) == std::vector<int>({2, 9}));
    assert(getNoZeroIntegers(19) == std::vector<int>({1, 18}));
    assert(getNoZeroIntegers(99) == std::vector<int>({1, 98}));
    assert(getNoZeroIntegers(100) == std::vector<int>({11, 89}));
    assert(getNoZeroIntegers(101) == std::vector<int>({12, 89}));
    assert(getNoZeroIntegers(1000) == std::vector<int>({111, 889}));
    assert(getNoZeroIntegers(9999) == std::vector<int>({1, 9998}));
}
