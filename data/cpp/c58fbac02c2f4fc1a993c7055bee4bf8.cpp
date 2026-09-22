Write a C++ function named `bitwiseComplement` that takes a non-negative integer `n` and returns its bitwise complement considering only its most significant bit (MSB) up to the bit position just before the highest set bit of `n`. In other words, flip all bits of `n` from its highest set bit (inclusive) down to bit 0, but ignore any leading zeros above the MSB. For example, `5` (binary `101`) becomes `2` (binary `010`), and `0` is a special case where the result is `1`. The function must handle `n` in the range `0` to `10^9` (inclusive) and return the result as an `int`. Do not use built-in bitwise complement operators like `~`; implement the algorithm manually using bit masks and loops.

// The key insight is to identify the position of the most significant set bit (MSB) in `n`. For any positive `n`, we need to flip all bits from the MSB down to bit 0. This is equivalent to computing `(2^(k+1) - 1) ^ n` where `k` is the bit position of the MSB (since a mask of all ones up to that bit XORed with `n` flips those bits). The approach in the snippet scans from bit 30 down to 0, skipping leading zeros until the first set bit is found (`find` flag). For every bit position from the MSB down, if the bit in `n` is `0`, we set that bit in the answer. If it is `1`, we leave the answer bit as `0`. This builds the complement. Edge cases: `n == 0` returns `1` (since no MSB, but by convention the complement of `0` over a single bit is `1`). For positive numbers, the algorithm works correctly. Time complexity is `O(31)` since we iterate over a fixed number of bits (30 down to 0, which is enough for `10^9` since `2^30` is about 1.07e9). Space complexity is `O(1)`.

#include <cstdint>

// Return the bitwise complement of n, considering only bits up to its most significant set bit.
// For n == 0, returns 1. For positive n, flips all bits from the MSB down to bit 0.
int bitwiseComplement(int n) {
    if (n == 0) {
        return 1;
    }

    int result = 0;
    bool found_msb = false;

    // Iterate from bit 30 down to 0 (enough for int values up to 2^31-1).
    for (int i = 30; i >= 0; --i) {
        int bit = n & (1 << i);
        if (!found_msb && bit == 0) {
            continue; // Skip leading zeros above the MSB.
        }
        found_msb = true;

        // If the current bit in n is 0, set it in the result; otherwise leave it as 0.
        if (bit == 0) {
            result |= (1 << i);
        }
    }

    return result;
}

#include <cassert>

int main() {
    assert(bitwiseComplement(0) == 1);
    assert(bitwiseComplement(5) == 2);   // 101 -> 010
    assert(bitwiseComplement(1) == 0);   // 1 -> 0
    assert(bitwiseComplement(7) == 0);   // 111 -> 000
    assert(bitwiseComplement(10) == 5);  // 1010 -> 0101
    assert(bitwiseComplement(8) == 7);   // 1000 -> 0111
    assert(bitwiseComplement(2) == 1);   // 10 -> 01
    assert(bitwiseComplement(15) == 0);  // 1111 -> 0000
    assert(bitwiseComplement(16) == 15); // 10000 -> 01111
    assert(bitwiseComplement(1000000000) == 73632255); // 10^9 -> complement up to MSB
    return 0;
}
