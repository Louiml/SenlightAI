Write a C++ function named `absoluteDifference` that takes two integer parameters `a` and `b` and returns the absolute difference between them as a non-negative integer. The function must handle all possible integer values (including negative numbers, zero, and extreme values like `INT_MAX` and `INT_MIN`) without causing overflow or undefined behavior. The result should always be mathematically correct (the absolute value of `a - b`). The function must be `const`-correct (though parameters are passed by value) and include appropriate documentation comments. Do not use the standard library’s `std::abs` or any built-in absolute value function; implement the logic manually using conditional checks to avoid pitfalls with `INT_MIN` (where `-INT_MIN` overflows). The task is to demonstrate robust integer arithmetic handling and edge-case awareness.

#include <cassert>
#include <climits>

// Global main function for testing the solution
int main() {
    // Basic cases
    assert(absoluteDifference(5, 3) == 2);
    assert(absoluteDifference(3, 5) == 2);
    assert(absoluteDifference(-4, 4) == 8);
    assert(absoluteDifference(0, 0) == 0);

    // Edge cases with extreme values
    assert(absoluteDifference(INT_MAX, -1) == 2147483648LL); // 2^31
    assert(absoluteDifference(INT_MIN, 0) == 2147483648LL);  // 2^31
    assert(absoluteDifference(INT_MAX, INT_MIN) == 4294967295LL); // 2^32 - 1

    // Negative numbers
    assert(absoluteDifference(-10, -5) == 5);
    assert(absoluteDifference(-5, -10) == 5);

    // Mixed signs
    assert(absoluteDifference(100, -200) == 300);

    return 0;
}

#include <cstddef> // for long long? not needed

// Return the absolute difference between two integers as a 64-bit value.
// This avoids overflow by casting to long long before subtraction.
long long absoluteDifference(int a, int b) {
    long long diff = static_cast<long long>(a) - static_cast<long long>(b);
    if (diff < 0) {
        diff = -diff;
    }
    return diff;
}

// The core challenge is computing `|a - b|` safely. The naive approach `std::abs(a - b)` can overflow when `a` and `b` have opposite signs and large magnitudes (e.g., `a = INT_MAX`, `b = -1` gives `a - b = INT_MAX + 1` which overflows). A safer method is to compute the difference without overflow by comparing the two values first. If `a >= b`, then the difference `a - b` is non-negative and safe (since `a - b <= INT_MAX` when `a >= b` because the maximum possible value is when `a=INT_MAX` and `b=INT_MIN`, but `a-b` only overflows if `a` and `b` have opposite signs and the absolute sum exceeds `INT_MAX`; however for `a >= b`, if `a` is positive and `b` is negative, `a - b` can be large but still ≤ `INT_MAX`? Actually `a - b` can exceed `INT_MAX` if `a` is positive and `b` is negative, e.g., `a=INT_MAX`, `b=-1` gives `INT_MAX+1` overflow. So careful: even when `a >= b`, if signs differ, `a - b` can overflow. The safe approach is to compare the two numbers and then compute the difference in the direction that is guaranteed not to overflow: if `a >= b`, compute `a - b` only if both are same sign? Better: use `long long` to compute the difference and then cast to int after taking absolute value. But the task prohibits using `std::abs`, but using `long long` for arithmetic is fine. The simplest robust solution is to cast parameters to `long long`, compute the difference, take absolute value using a conditional (if negative, multiply by -1), and then cast back to `int` (knowing the result will fit because the absolute difference of two `int`s is at most `2^31`? Actually `|INT_MAX - INT_MIN|` = `4294967295` which does not fit in `int`. The problem statement says "return the absolute difference as a non-negative integer" — for a typical `int` (32-bit), the absolute difference between `INT_MAX` and `INT_MIN` is 4294967295, which is not representable in signed 32-bit int. So we must decide: either assume that inputs are such that the absolute difference fits in `int`, or change the return type to `long long`. The task specification says "returns the absolute difference between them as a non-negative integer" — to be safe and correct for all possible inputs, we should return `long long`. Let’s modify: the function should return `long long` to accommodate the full range. The task statement originally said "non-negative integer" but not explicit bit width. I’ll make the solution return `long long`. The algorithm: compute `long long diff = (long long)a - (long long)b; if (diff < 0) diff = -diff; return diff;` This uses `long long` (which is at least 64-bit) to avoid overflow. Complexity: O(1) time and O(1) space.
