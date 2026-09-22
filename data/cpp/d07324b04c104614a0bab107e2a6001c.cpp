Write a C++ function that takes two non-negative integers `num1` and `num2` (both in the range [0, 10^9]) and returns the integer `x` that satisfies all three conditions: (1) `x` has exactly the same number of set bits (1s in binary) as `num2`, (2) the value of `x XOR num1` is as small as possible (i.e., `x` is as close to `num1` as possible in terms of bitwise difference), and (3) if multiple candidates tie, return the one with the smallest numeric value. The function must be standalone, not rely on global state, and operate correctly for all valid inputs. Note: The original snippet uses a greedy bit-manipulation approach; your task is to replicate that logic exactly, preserving bit order and handling edge cases like when `num1` has fewer set bits than `num2` or vice versa.

// The core idea is to match `num1`'s bits as closely as possible while forcing the total count of set bits to equal the popcount of `num2`. First, compute the number of set bits in `num1` (call it `a`) and in `num2` (call it `b`). If `a == b`, then `num1` itself already satisfies the condition, so return it unchanged. If `a > b`, then we must remove `a - b` bits from `num1`. To minimize `x XOR num1`, we want to clear the least significant set bits first, because clearing a lower-order bit changes the numeric value by the smallest amount, thereby keeping `x` as close to `num1` as possible. So iterate from bit position 0 upward; whenever a bit is set in `num1`, clear it (using XOR with 1<<i) and decrement a counter until we have removed exactly `a - b` bits. Conversely, if `a < b`, we must add `b - a` bits. To keep `x` close to `num1`, set the least significant zero bits (starting from bit 0) to 1, because adding a low-order bit changes the value the least. Iterate from bit 0 upward; whenever a bit is zero in `num1`, set it (using XOR) and decrement a counter until exactly `b - a` bits are added. Since the integers are at most 10^9 (~2^30), checking bits from 0 to 31 is safe and covers all cases. Edge cases: if `num1` is 0 and `num2` has some set bits, we set the lowest bits of 0; if `num2` has zero set bits, we clear all bits of `num1`. Time complexity is O(32) per call, effectively O(1), and space complexity is O(1). The algorithm is deterministic and always produces a valid `x` because we are counting bits and adjusting exactly the required amount.

#include <cstdint>

// Returns an integer x with the same number of set bits as num2,
// minimizing x XOR num1 (with smallest numeric value on ties).
int minimizeXor(int num1, int num2) {
    // Helper: count set bits (popcount) for a non-negative int.
    auto popcount = [](int n) {
        int count = 0;
        while (n > 0) {
            count += (n & 1);
            n >>= 1;
        }
        return count;
    };

    int a = popcount(num1);  // number of set bits in num1
    int b = popcount(num2);  // number of set bits in num2

    if (a == b) {
        return num1;
    }

    if (a > b) {
        // Need to clear (a - b) set bits, starting from LSB.
        int toClear = a - b;
        for (int i = 0; i <= 31 && toClear > 0; ++i) {
            if ((num1 & (1 << i)) != 0) {
                num1 ^= (1 << i);
                --toClear;
            }
        }
    } else {
        // Need to set (b - a) zero bits, starting from LSB.
        int toSet = b - a;
        for (int i = 0; i <= 31 && toSet > 0; ++i) {
            if ((num1 & (1 << i)) == 0) {
                num1 ^= (1 << i);
                --toSet;
            }
        }
    }

    return num1;
}

#include <cassert>

int main() {
    // Basic equal popcounts: return num1 unchanged.
    assert(minimizeXor(3, 5) == 3);  // 3 (011), 5 (101), both have 2 bits

    // num1 has more bits than num2: clear lowest set bits.
    assert(minimizeXor(7, 1) == 4);  // 7 (111) -> clear bits 0,1 -> 4 (100), popcount 1

    // num1 has fewer bits than num2: set lowest zero bits.
    assert(minimizeXor(1, 7) == 7);  // 1 (001) -> set bits 1,2 -> 7 (111), popcount 3

    // num2 has zero set bits: clear all bits.
    assert(minimizeXor(28, 0) == 0); // 28 (11100) -> clear all -> 0

    // num1 is zero, num2 has set bits: build lowest possible number.
    assert(minimizeXor(0, 15) == 7); // 15 has 4 bits, smallest with 4 bits is 7 (0111)

    // Larger test: num1 = 10 (1010, 2 bits), num2 = 12 (1100, 2 bits) -> same count, return 10.
    assert(minimizeXor(10, 12) == 10);

    // Edge: num1 = 1024 (1 bit), num2 = 3 (2 bits) -> set lowest zero bit: 1024 becomes 1025? 
    // 1024 binary 10000000000, set bit 0 -> 1025 (10000000001) has 2 bits, XOR diff = 1.
    assert(minimizeXor(1024, 3) == 1025);

    // Edge: num1 = 15 (4 bits), num2 = 1 (1 bit) -> clear 3 lowest bits: 15=1111 -> clear bits 0,1,2 -> 8 (1000)
    assert(minimizeXor(15, 1) == 8);

    // Verify that result has exactly the popcount of num2 in all cases above? 
    // This is implicitly checked by the comparisons.

    return 0;
}
