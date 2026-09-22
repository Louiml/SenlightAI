// Given two 32-bit signed integers `a` and `b` where `a <= b`, write a C++ function `xorRange` that returns the bitwise XOR of all integers from `a` to `b` inclusive. The function must handle the full range of `int` values, including negative numbers and cases where `a` or `b` are close to `INT_MIN` or `INT_MAX`. Your implementation should avoid a naive O(b-a) loop for large ranges and instead use a mathematical observation about XOR patterns to compute the result in O(1) time. Ensure your function is well-documented and correct for all edge cases, such as `a == b`, `b < 0`, and ranges that cross zero.

#include <cassert>
#include <climits>

int main() {
    // Basic positive range
    assert(xorRange(1, 4) == (1 ^ 2 ^ 3 ^ 4)); // 4
    // Single element
    assert(xorRange(7, 7) == 7);
    // Zero included
    assert(xorRange(0, 3) == (0 ^ 1 ^ 2 ^ 3) == 0);
    // Negative range
    assert(xorRange(-3, -1) == (-3 ^ -2 ^ -1));
    // Cross zero
    assert(xorRange(-2, 2) == (-2 ^ -1 ^ 0 ^ 1 ^ 2) == 0);
    // Large range near INT_MAX
    assert(xorRange(INT_MAX - 2, INT_MAX) == (INT_MAX - 2 ^ INT_MAX - 1 ^ INT_MAX));
    // Edge with INT_MIN
    assert(xorRange(INT_MIN, INT_MIN + 1) == (INT_MIN ^ INT_MIN + 1));
    assert(xorRange(INT_MIN, INT_MAX) == 0); // XOR of all ints is 0
    // a == b with negative
    assert(xorRange(-10, -10) == -10);
    // Full range check via direct comparison for small range
    int expected = 0;
    for (int i = -5; i <= 5; ++i) expected ^= i;
    assert(xorRange(-5, 5) == expected);
    // a=0,b=0
    assert(xorRange(0, 0) == 0);
    
    return 0;
}

#include <cstdint>
#include <stdexcept>

// Returns XOR of all integers from 0 to n (inclusive).
// Works for negative n as well due to two's-complement properties.
static int xorFromZero(long long n) {
    switch (n & 3LL) { // n % 4 using bitwise AND
        case 0: return static_cast<int>(n);
        case 1: return 1;
        case 2: return static_cast<int>(n + 1);
        case 3: return 0;
        default: return 0; // unreachable
    }
}

// Returns the bitwise XOR of all integers from a to b inclusive.
// Requires a <= b. Throws std::invalid_argument if a > b.
int xorRange(int a, int b) {
    if (a > b) {
        throw std::invalid_argument("a must be <= b");
    }
    // Since xorRange(a,b) = xorFromZero(b) ^ xorFromZero(a-1),
    // but a-1 may overflow for INT_MIN, handle it specially.
    if (a == INT32_MIN) {
        return xorFromZero(static_cast<long long>(b));
    }
    return xorFromZero(static_cast<long long>(b)) ^ xorFromZero(static_cast<long long>(a - 1));
}

// The naive loop XORing every integer from `a` to `b` is correct but too slow for large ranges (up to billions of iterations). Instead, we can exploit the pattern of XOR of consecutive integers from 0 to n. Define a helper function `xorFromZero(n)` that returns the XOR of all integers from 0 to n. The pattern repeats every 4 numbers:
// - If `n % 4 == 0`, result = n
// - If `n % 4 == 1`, result = 1
// - If `n % 4 == 2`, result = n+1
// - If `n % 4 == 3`, result = 0
//
// For negative `n`, this helper still works because two's-complement arithmetic ensures the XOR pattern holds for negative numbers as well (e.g., xorFromZero(-1) is 0 because there are no positive integers in the range; xorFromZero(-2) is the XOR of -2 and -1 = 1, etc.). The XOR of a range `[a,b]` is then `xorFromZero(b) ^ xorFromZero(a-1)`. This works for all `a,b`, including negative values, because XOR is its own inverse and the property `xor(a..b) = xor(0..b) ^ xor(0..a-1)` holds for any integers. The only careful part is computing `a-1` safely when `a == INT_MIN`; we can handle this by using `long long` for the helper’s argument or by special-casing `a == INT_MIN` to return `xorFromZero(b)` (since `xorFromZero(INT_MIN-1)` would overflow). Time complexity is O(1) and space O(1).
