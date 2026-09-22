// Write a C++ function `int minVal(int a, int b)` that, given two non-negative 32-bit integers `a` and `b`, returns an integer `x` such that the number of set bits (1s in binary representation) in `x` equals the number of set bits in `b`, and the XOR difference (sum of absolute bitwise differences) between `x` and `a` is minimized. If multiple such `x` values exist, any one that achieves the minimum difference is acceptable. The function should work for all `a` and `b` in the range `[0, 2^31 - 1]`, treating the binary representation as 31 bits (no sign bit; use standard 32-bit integers but ignore the sign). The output must be a valid integer that satisfies the constraints.
The goal is to transform `a` into a number `x` with exactly the same popcount as `b`, while minimizing the Hamming distance (number of differing bits) between `x` and `a`. The optimal strategy is to first try to preserve as many bits of `a` as possible. If `a` already has the same number of set bits as `b`, then `x = a` gives zero difference. If `a` has more set bits than `b`, we must clear some of `a`'s set bits — the best ones to clear are the least significant set bits, because changing them causes the smallest numerical difference but the same Hamming distance; however, since any set-bit removal is one differing bit, we can choose any set bits, but the standard approach is to clear from LSB to maintain minimal numeric value. Conversely, if `a` has fewer set bits than `b`, we must add set bits; the best ones to add are the least significant zero bits, again to minimize numeric value. The algorithm scans bits from LSB (position 0) to MSB (position 31). For each bit, if the corresponding bit in `a` is 0 and we still need more set bits (target > current count), we set it; if the bit in `a` is 1 and we have too many set bits (current count > target), we clear it (i.e., do not include it); otherwise, we keep the original bit. This greedy LSB-first approach yields an `x` with exactly `popcount(b)` set bits and minimizes the Hamming distance to `a` — in fact, it achieves the absolute minimum possible difference, which is `abs(popcount(a) - popcount(b))` (because each added or removed bit must differ). Important edge cases: `b = 0` (result must be 0), `a = 0` and `b` has many set bits (result is the smallest number with that popcount, e.g., all ones in LSBs), and when `popcount(a) == popcount(b)`, the result is exactly `a`. Time complexity is O(1) (always 32 iterations), space complexity O(1).
#include <bits/stdc++.h>

// Returns an integer x with the same number of set bits as b,
// minimizing the Hamming distance (bitwise difference) to a.
int minVal(int a, int b) {
    int targetCount = __builtin_popcount(static_cast<unsigned int>(b));
    int currentCount = __builtin_popcount(static_cast<unsigned int>(a));

    int result = 0;
    for (int bit = 0; bit < 31; ++bit) { // 31 bits, ignoring sign
        int mask = 1 << bit;
        bool isSet = (a & mask) != 0;

        if (!isSet && currentCount < targetCount) {
            // Need more set bits, add one at this LSB position
            result |= mask;
            ++currentCount;
        } else if (isSet && currentCount > targetCount) {
            // Too many set bits, skip this one by not including it
            --currentCount; // effectively clearing it
        } else {
            // Keep the original bit
            result |= (a & mask);
        }
    }

    // Handle the 32nd bit (bit 31) if necessary? Not needed for 31-bit valid range, but for safety:
    // For full 32-bit capability, loop to 32, but problem specifies 31-bit. We include this note.
    // We'll loop to 31 because a and b are non-negative and fit in 31 bits.
    // If both have bit 31 set, it would be part of a? But they're < 2^31, so bit 31 is 0.
    return result;
}
#include <cassert>
#include <bits/stdc++.h>

int minVal(int a, int b);

int main() {
    // Basic cases
    assert(minVal(0, 0) == 0);
    assert(minVal(5, 5) == 5);          // same popcount, return a exactly
    assert(minVal(7, 0) == 0);          // b has zero set bits, result must be 0
    assert(minVal(0, 7) == 7);          // a has zero set bits, need 3 set bits, smallest is 0b111
    // Popcount equality gives exact match
    assert(minVal(10, 3) == 10);        // a=1010 (2 bits), b=0011 (2 bits), same count, return a
    // Need to remove bits (a has more set bits than b)
    assert((__builtin_popcount(minVal(15, 1)) == 1)); // 15 has 4 bits, b has 1, result must have 1 bit
    assert(minVal(15, 1) == 8);         // choose highest bit to keep? Actually minimal difference: keep bit 3 (8) or bit 2? Both give Hamming distance 3, but 8 is largest. However, LSB-first clearing yields 8? Let's check: bits 0,1,2,3 set. Clear LSB first: remove bit0 -> count3, need1 -> remove bit1 -> count2, remove bit2 -> count1, keep bit3 -> result 8. So correct.
    // Need to add bits (a has fewer set bits than b)
    assert(minVal(1, 7) == 7);         // a=1, need 3 bits, add bits 1 and 2 -> 7
    // Larger numbers
    assert(minVal(1024, 3) == 1025);   // 1024=2^10, need 2 bits, add bit0 to get 1025 (bits 10 and 0)
    // Random-like check
    int a = 12345, b = 6789;
    int x = minVal(a, b);
    assert(__builtin_popcount(x) == __builtin_popcount(b));
    // Simple Hamming distance check: should be |count(a)-count(b)| because we can always achieve that
    int diff = 0;
    for (int i = 0; i < 31; ++i) {
        if (((a >> i) & 1) != ((x >> i) & 1)) ++diff;
    }
    assert(diff == abs(__builtin_popcount(a) - __builtin_popcount(b)));

    return 0;
}
