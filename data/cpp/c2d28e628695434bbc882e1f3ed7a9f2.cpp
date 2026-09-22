/*
Write a C++ function named `isPowerOfTwo` that takes an integer `n` and returns `true` if `n` is a positive power of two (i.e., `n == 2^x` for some non-negative integer `x`), and `false` otherwise. The function must handle all valid `int` values, including negative numbers and zero, which are never powers of two. The implementation should avoid loops and recursion, using only bitwise or mathematical operations, and must be efficient for large inputs. Ensure the function is `const`-correct and properly documented.
*/
#include <cstdint>

// Returns true if n is a positive power of two, false otherwise.
bool isPowerOfTwo(int n) {
    // Negative numbers and zero are never powers of two.
    if (n <= 0) {
        return false;
    }
    // A power of two has exactly one bit set, so n & (n-1) is zero.
    return (n & (n - 1)) == 0;
}
#include <cassert>

int main() {
    assert(isPowerOfTwo(1) == true);   // 2^0
    assert(isPowerOfTwo(2) == true);   // 2^1
    assert(isPowerOfTwo(16) == true);  // 2^4
    assert(isPowerOfTwo(1024) == true); // 2^10
    assert(isPowerOfTwo(0) == false);
    assert(isPowerOfTwo(-4) == false);
    assert(isPowerOfTwo(3) == false);
    assert(isPowerOfTwo(15) == false);
    assert(isPowerOfTwo(1073741824) == true); // 2^30 (max power of two in int)
    assert(isPowerOfTwo(2147483647) == false); // INT_MAX
    return 0;
}
// A number is a power of two if it is positive and has exactly one bit set in its binary representation. The classic bitwise trick is that for any power of two `n > 0`, `n & (n - 1)` equals zero, because subtracting 1 flips all lower bits and the single set bit becomes zero. For `n <= 0`, return `false` immediately. Edge cases: `n = 0` (`0 & -1` is not zero, but we exclude non-positive), `n = 1` (binary `1` has one set bit, so true), `n = INT_MIN` (negative, false), and `n = 2^30` (positive with one set bit, true). Time complexity is O(1) and space complexity is O(1).
