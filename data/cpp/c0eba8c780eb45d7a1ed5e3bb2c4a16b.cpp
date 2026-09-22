Write a C++ function `long minimumArrayEnd(int n, int x)` that, given two integers `n` and `x` where `n >= 1`, constructs the lexicographically smallest array of length `n` such that every element is strictly increasing and the bitwise AND of all elements equals exactly `x`. Return the last element of that array. In other words, find the smallest possible value `ans` such that there exists an increasing sequence `a[0] < a[1] < ... < a[n-1]` with `a[0] & a[1] & ... & a[n-1] == x` and `a[n-1] == ans`. The function must handle cases where `n` is 1 (then `ans == x`) and where `x` may be large (up to `10^8`), and `n` up to `10^5`, so the result may exceed 32-bit range—use `long long` (or `long` which is at least 32-bit but in practice 64-bit on LeetCode platforms).
The key observation is that if the AND of all array elements equals `x`, then every element must have all bits set that are set in `x`. Any bit that is `0` in `x` can be either `0` or `1` in the elements, but to keep the sequence minimal and increasing, we want the smallest possible numbers subject to the AND constraint. The smallest first element is `x` itself. For the next element, we need a number greater than `x` that still has all bits of `x` set; the smallest such number is `x | 1` if bit 0 is not set in `x`, but we must also ensure it's > previous. In general, we treat the bits of `x` that are 1 as fixed (must be 1 in every element), and the remaining bits (where `x` has 0) can be freely toggled to create increasing numbers. Starting from `x`, to get the next numbers, we increment a "counter" in the free bits only. This is equivalent to: let `res = x`, and `rem = n - 1`. For each bit position `pos` from 0 upward, if `x` has that bit = 0, then that bit in `res` can be used to represent the binary digits of `rem` (the number of increments needed after the first element). Specifically, while `rem > 0`, we look for the next free bit positon in `x`, set that bit in `res` to the lowest bit of `rem`, then shift `rem` right. If `x` has the bit = 1, we skip it (it's fixed). This "merges" the binary representation of `n-1` into the zero-bit positions of `x`. The result is the smallest possible last element because we are effectively constructing the minimal sequence where each increment uses only the free bits. Edge case: if `n == 1`, `rem = 0`, the while loop doesn't run, and `res` stays `x`, which is correct. The algorithm runs in O(number of bits) ≈ O(32) since `x` and `n-1` fit in 64-bit anyway, but conceptually O(log(max(x, n))). Constant extra space.
#include <cstdint>

// Given n (>=1) and x, returns the last element of the smallest increasing
// array of length n whose bitwise AND equals exactly x.
long long minimumArrayEnd(int n, int x) {
    long long res = x;
    long long rem = n - 1;      // how many increments to distribute
    long long pos = 1;          // current bit position (1, 2, 4, ...)

    while (rem > 0) {
        // If bit `pos` is 0 in x, we can use it to encode part of rem.
        if ((x & pos) == 0) {
            // Set this bit in res if the lowest bit of rem is 1.
            res |= (rem & 1) * pos;
            rem >>= 1;
        }
        pos <<= 1;
    }

    return res;
}
#include <cassert>

int main() {
    // n = 1: only element is x.
    assert(minimumArrayEnd(1, 5) == 5);

    // Simple case: x = 1 (binary ...0001), n = 3.
    // Sequence: 1, 3 (1|2), 5? Actually and of [1,3,5] = 1, last=5.
    assert(minimumArrayEnd(3, 1) == 5);

    // x = 2 (binary 10), n = 2: sequence [2, 3? no, 2&3=2 but 3>2; last=3].
    assert(minimumArrayEnd(2, 2) == 3);

    // x = 7 (all low bits set), n = 4:
    // Sequence must keep bits 0,1,2 set; free bits start at 3.
    // n-1=3 binary 11 -> set bits 3 and 4: last = 7 | 8 | 16 = 31.
    assert(minimumArrayEnd(4, 7) == 31);

    // x = 0, n = 5: free bits from 0 upward, n-1=4 (100) -> last = 4.
    assert(minimumArrayEnd(5, 0) == 4);

    // Large n and x, ensure no overflow (result fits in long long).
    // x = 1<<30 (bit 30 set), n = 1<<20, last should be x | (n-1) shifted
    // to free bits above bit 30? Actually free bits start at 0, so last = x + (n-1).
    assert(minimumArrayEnd(1 << 20, 1 << 30) == (1LL << 30) + (1 << 20) - 1);

    // n=2, x=5 (101): free bit at position 1, n-1=1 -> set bit 1: last = 7.
    assert(minimumArrayEnd(2, 5) == 7);

    // n=3, x=5 (101): n-1=2 (10) -> free bits pos1 and pos2? Actually free bits: bit0? x has bit0=1, so free bits are 1,2,3,... We need to encode rem=2 (binary 10) into free bits starting from lowest: set bit1 (value 2) once? Wait: process bit0=1 skip, bit1=0 -> take rem LSB=0, rem=1; bit2=0 -> take rem LSB=1, rem=0; result = x | 4 = 9. Sequence: 5, 7? Check: 5&7=5, but need 3 numbers: 5,6? 6 has bit0=0 so 5&6=4. So 5,7,9 works? 5&7&9=5, and sequence 5<7<9. Last=9.
    assert(minimumArrayEnd(3, 5) == 9);

    // Edge case: n large, x has many bits set, e.g., x = 0x7FFFFFFFFFFFFFFF (all low 63 bits set) but that's > 10^8, but for test use smaller: x=0xFFFFFFFF (all 32 bits set), n=2: free bits start at 32, n-1=1 -> last = x | (1<<32) = 0x1FFFFFFFF.
    assert(minimumArrayEnd(2, 0xFFFFFFFF) == 0x1FFFFFFFFll);

    return 0;
}
