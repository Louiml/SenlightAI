Write a C++ function `bool isPowerOfTwo(long long n)` that returns `true` if the given non-negative integer `n` is an exact power of two (i.e., \( n = 2^k \) for some non-negative integer \( k \)), and `false` otherwise. The function must handle the full range of `long long` including `0` (which is not a power of two). Use only bitwise operations in your implementation—do not use loops, floating-point math, or library functions like `log2`. The function should be efficient and avoid any division or multiplication.
The core idea is that any power of two has exactly one bit set in its binary representation. For example, \( 1 = 0001 \), \( 2 = 0010 \), \( 4 = 0100 \), etc. A common bitwise trick is to use `(n & (n - 1))`. For a power of two, `n-1` flips all lower bits to 1 and the one set bit to 0, so `n & (n-1)` equals zero. For example, \( 4 (100) \) and \( 3 (011) \) have no bits in common. For non-powers, the AND will be non-zero. However, this trick fails for `n = 0` because `0 & (-1)` is zero, which would wrongly suggest it is a power of two. Therefore, we must explicitly check that `n > 0`. Edge cases include `n = 1` (which is \(2^0\)) → must return true; `n = 0` → false; large values like `1LL << 62` → true; and values like `(1LL << 62) + 1` → false. Time complexity is O(1) (constant time), and space complexity is O(1). The solution uses only bitwise AND and subtraction.
#include <cstdint>

// Returns true if n is an exact power of two (n = 2^k for k >= 0).
// Returns false for n = 0 and for non-powers of two.
bool isPowerOfTwo(long long n) {
    if (n <= 0) {
        return false;
    }
    // A power of two has exactly one bit set, so n & (n-1) == 0.
    return (n & (n - 1)) == 0;
}
#include <cassert>
#include <cstdint>

// Declaration of the solution function
bool isPowerOfTwo(long long n);

int main() {
    // Basic powers of two
    assert(isPowerOfTwo(1LL) == true);
    assert(isPowerOfTwo(2LL) == true);
    assert(isPowerOfTwo(4LL) == true);
    assert(isPowerOfTwo(1024LL) == true);
    assert(isPowerOfTwo(1LL << 62) == true); // Largest representable power of two in long long

    // Non-powers of two
    assert(isPowerOfTwo(0LL) == false);
    assert(isPowerOfTwo(3LL) == false);
    assert(isPowerOfTwo(5LL) == false);
    assert(isPowerOfTwo(1000LL) == false);
    assert(isPowerOfTwo((1LL << 62) + 1LL) == false);

    // Negative numbers (should be false)
    assert(isPowerOfTwo(-1LL) == false);
    assert(isPowerOfTwo(-1024LL) == false);

    return 0;
}
