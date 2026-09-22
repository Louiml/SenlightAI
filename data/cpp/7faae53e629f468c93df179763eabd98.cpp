Write a C++ function named `positionOfFirstSetBit` that takes a non-negative integer `n` as input and returns the 1-based position of the least significant set bit (i.e., the rightmost bit that is 1) in its binary representation. If `n` is zero, the function should return 0. For example, given `n = 12` (binary `1100`), the first set bit is at position 3; given `n = 1` (binary `1`), the position is 1; and given `n = 0`, the return value is 0. The function must handle all possible values of an `unsigned int` on the platform (typically 0 to 4,294,967,295) and must not use any bit-manipulation built-ins like `__builtin_ctz`. Implement the solution in a self-contained manner, including all needed headers and using appropriate `const` correctness where applicable.

The algorithm scans bits from the least significant position (position 0 in 0-indexed terms) upward. We use a loop with a counter `k` starting at 0. At each step, we check whether the bit at position `k` is set by computing `(n & (1 << k))`. If the result is non-zero, that bit is set, and we return `k + 1` because the problem asks for a 1-based position. If the bit is zero, we increment `k` and continue. The loop must terminate because `n` is finite and has a limited bit length (at most 32 for `unsigned int` on common platforms); if `n` is zero, we handle it as a special case and return 0 immediately. Edge cases include `n = 0`, `n` being a power of two (only one set bit), and large values like `UINT_MAX` (all 32 bits set, so the first set bit is position 1). The time complexity is O(k) where `k` is the number of zero bits before the first set bit, which is at most 32 for typical `unsigned int`; thus, it is O(1) in practice. The auxiliary space complexity is O(1) because we only use a few integer variables.

#include <cstdint>

// Return the 1-based position of the least significant set bit in n.
// Return 0 if n == 0.
unsigned int positionOfFirstSetBit(unsigned int n) {
    if (n == 0) {
        return 0;
    }

    unsigned int position = 0;
    while ((n & (1u << position)) == 0) {
        ++position;
    }
    return position + 1;
}

#include <cassert>
#include <climits>

int main() {
    // Basic cases
    assert(positionOfFirstSetBit(0) == 0);
    assert(positionOfFirstSetBit(1) == 1);
    assert(positionOfFirstSetBit(2) == 2);   // binary 10
    assert(positionOfFirstSetBit(3) == 1);   // binary 11
    assert(positionOfFirstSetBit(4) == 3);   // binary 100
    assert(positionOfFirstSetBit(12) == 3);  // binary 1100
    assert(positionOfFirstSetBit(16) == 5);  // binary 10000
    // Even numbers
    assert(positionOfFirstSetBit(6) == 2);   // binary 110
    // Large value with all bits set
    assert(positionOfFirstSetBit(UINT_MAX) == 1);
    // Large value with first set bit in the highest position
    assert(positionOfFirstSetBit(1u << 31) == 32); // if unsigned int has 32 bits
    return 0;
}
