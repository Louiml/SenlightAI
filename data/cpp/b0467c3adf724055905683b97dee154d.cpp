Write a C++ function that takes an MPL-style placeholder expression or simply two integer arguments and returns an integer representing the result of subtracting the second from the first, but ignoring the actual Boost.MPL library and instead implementing a simple compile-time or runtime subtraction. Specifically, implement a free function `subtractIntegers(int a, int b)` that returns `a - b`. Additionally, the function must handle potential integer overflow gracefully by clamping the result to the range of `int` (i.e., if the mathematical result is less than `INT_MIN`, return `INT_MIN`; if greater than `INT_MAX`, return `INT_MAX`). The function should be `const`-correct and use only standard C++ headers.

#include <cassert>
#include <climits>

int main() {
    // Normal cases
    assert(subtractIntegers(10, 3) == 7);
    assert(subtractIntegers(3, 10) == -7);
    assert(subtractIntegers(0, 0) == 0);
    assert(subtractIntegers( -5, 2) == -7);

    // Overflow cases (clamping)
    assert(subtractIntegers(INT_MAX, -1) == INT_MAX);       // would be INT_MAX+1
    assert(subtractIntegers(INT_MIN, 1) == INT_MIN);        // would be INT_MIN-1
    assert(subtractIntegers(INT_MAX, INT_MIN) == INT_MAX);  // 4294967295 clamps
    assert(subtractIntegers(INT_MIN, INT_MAX) == INT_MIN);  // -4294967295 clamps

    // Boundary values but no overflow
    assert(subtractIntegers(INT_MAX, 0) == INT_MAX);
    assert(subtractIntegers(INT_MIN, 0) == INT_MIN);
    assert(subtractIntegers(INT_MAX, INT_MAX) == 0);
    assert(subtractIntegers(INT_MIN, INT_MIN) == 0);

    return 0;
}

#include <climits>

// Return a - b, clamping the result to the range [INT_MIN, INT_MAX].
int subtractIntegers(int a, int b) {
    // Use a wider type to safely compute the difference.
    long long result = static_cast<long long>(a) - static_cast<long long>(b);

    // Clamp to int range.
    if (result < INT_MIN) {
        return INT_MIN;
    }
    if (result > INT_MAX) {
        return INT_MAX;
    }
    return static_cast<int>(result);
}

// The core algorithm is straightforward: compute `a - b`. However, because subtracting two `int` values can overflow (e.g., `INT_MAX - (-1)` yields `INT_MAX + 1` which is out of range), we must detect potential overflow before performing the subtraction. The safe approach is to use a wider integer type, such as `long long`, to compute the difference, then clamp the result to the `int` range. If `long long` is 64-bit, it can safely represent the difference of any two 32-bit `int` values (since `INT_MIN` to `INT_MAX` difference fits in 64 bits). We then compare the result with `INT_MIN` and `INT_MAX` (from `<climits>`) and return the clamped value. Edge cases: both `a` and `b` can be `INT_MIN` or `INT_MAX`, including `INT_MIN - INT_MAX` (which is `-4294967295` — fits in 64-bit) and `INT_MAX - INT_MIN` (which is `4294967295` — fits in 64-bit). The time complexity is O(1), space complexity O(1). No loops or data structures are needed.
