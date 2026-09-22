Write a standalone C++ function named `isPowerOfTwo` that takes a single integer argument `num` and returns a `bool` indicating whether `num` is a positive power of two (i.e., 1, 2, 4, 8, 16, …). The function must return `false` for zero, negative numbers, and any integer that is not an exact power of two. The function should not rely on floating-point math (like `log2` or `ceil`), as these can produce precision errors for very large integers. Instead, use an integer bitwise or iterative shifting approach. The function must be `const`-correct (though no member variables exist, so this mainly means not modifying the input) and should be accompanied by a brief comment describing its behavior. The task is to provide only the function definition (no `main`), plus a separate test block that verifies correctness with `assert` statements.
// The simplest reliable method to check if an integer is a power of two is to observe that a power of two has exactly one bit set in its binary representation. A classic bitwise trick is: for any positive integer `n > 0`, `n` is a power of two if and only if `(n & (n - 1)) == 0`. This works because subtracting 1 from a power of two flips all lower bits and clears the highest set bit, so the bitwise AND with the original becomes zero. For `n <= 0`, we immediately return `false` (especially `0` and negative numbers). Edge cases include `1` (which is 2^0 and should return `true`), very large integers (e.g., `1<<30`, but avoiding overflow), and numbers like `0` and `-8`. The time complexity is O(1) (single bitwise operation), and space complexity is O(1). An alternative iterative approach (dividing by 2 repeatedly) would be O(log n), but the bitwise method is cleaner and more efficient.
#include <cstdint>

// Returns true if the given integer is a positive power of two (1, 2, 4, 8, ...).
// Returns false for zero, negative numbers, and non-powers of two.
bool isPowerOfTwo(int num) {
    if (num <= 0) {
        return false;
    }
    // A positive power of two has exactly one bit set.
    // (num & (num - 1)) is zero only for powers of two.
    return (num & (num - 1)) == 0;
}
#include <cassert>

int main() {
    assert(isPowerOfTwo(1) == true);
    assert(isPowerOfTwo(2) == true);
    assert(isPowerOfTwo(4) == true);
    assert(isPowerOfTwo(1024) == true);
    assert(isPowerOfTwo(0) == false);
    assert(isPowerOfTwo(-4) == false);
    assert(isPowerOfTwo(3) == false);
    assert(isPowerOfTwo(6) == false);
    assert(isPowerOfTwo(100) == false);
    // Large power of two within int range (2^30)
    assert(isPowerOfTwo(1073741824) == true);
    // Just above a power of two
    assert(isPowerOfTwo(1073741825) == false);
    return 0;
}
