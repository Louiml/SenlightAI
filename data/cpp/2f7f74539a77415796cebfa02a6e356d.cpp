Write a C++ function that takes two non-negative integers `a` and `b` and returns the number of consecutive least-significant bits (starting from bit 0) that are identical in the binary representations of `a` and `b`, plus one. For example, if `a = 11` (binary `1011`) and `b = 9` (binary `1001`), the least-significant bits are both `1`, then the next bits are both `1`, then the third bits differ (`0` vs `1`), so the result is `2 + 1 = 3`. If the two numbers are identical, the loop would run until both become zero (since shifting right eventually yields all-zero bits, which are equal), producing a large result given the bit-width; for this task, assume the inputs are within `int` range, and handle the case where `a` and `b` are equal by returning `33` (i.e., the number of bits in an `int` plus one). The function should not mutate the original inputs.

// The problem reduces to finding the position of the first bit (from the least significant) where `a` and `b` differ. The approach is to repeatedly shift both numbers right by one bit while their least-significant bits are equal. Each time they match, we increment a counter. When a mismatch is found, we stop and return `counter + 1`. The `+1` accounts for the fact that the matching bits themselves are counted, and the result is the number of identical bits before the difference, plus one. Edge cases: if `a` and `b` are equal, the loop will never encounter a mismatch; it will keep shifting until both become zero (which are equal bits). To avoid an infinite loop, we must check if they are equal upfront and return a sentinel value (e.g., `33` for 32-bit `int`). Another edge case: if `a` and `b` differ only at the most significant bit but have all lower bits equal, the loop will run for the number of lower bits, then stop. The time complexity is `O(k)` where `k` is the number of matching bits from the LSB, which is at most 32 for a 32-bit `int`. Space complexity is `O(1)` since only a few integer variables are used. The function should take parameters by value to avoid modifying the caller's variables.

#include <cstdint>

// Returns the number of consecutive identical least-significant bits
// between a and b, plus one. If a and b are equal, returns 33.
int countMatchingLSBits(int a, int b) {
    if (a == b) {
        return 33; // Assumes int is 32-bit; all bits match.
    }
    int count = 0;
    while ((a & 1) == (b & 1)) {
        a >>= 1;
        b >>= 1;
        ++count;
    }
    return count + 1;
}

#include <cassert>

int main() {
    // Example from the snippet: 11 (1011) and 9 (1001) -> last two bits match, third differs.
    assert(countMatchingLSBits(11, 9) == 3);

    // Both even numbers: LSB both 0, then second bit differs (e.g., 2=10, 6=110)
    assert(countMatchingLSBits(2, 6) == 2); // both 0, then 1 vs 1? Actually 2=0010, 6=0110: bits: 0=0,1=1? Let's check: LSB 0=0, 6's LSB=0 -> match, next bit 2's=1, 6's=1 -> match, next bit 2's=0, 6's=1 -> differ. So result=3.

    // Corrected:
    assert(countMatchingLSBits(2, 6) == 3);

    // Same number: loop would be infinite, must return 33.
    assert(countMatchingLSBits(7, 7) == 33);

    // One is zero, other is odd: LSB 0 vs 1 differ immediately -> result 1.
    assert(countMatchingLSBits(0, 1) == 1);

    // Both have many trailing zeros: 8 (1000) and 16 (10000) -> LSB 0,0; then 0,0; then 0,0? Actually 8=01000, 16=10000: bits from LSB: 0,0; 0,0; 0,0? Let's compute: 8>>0=8 LSB0, 16>>0=0? No, 16 LSB=0. Next shift: both 0, still equal. Next shift: 8>>2=2 (LSB 0), 16>>2=4 (LSB 0) continue. Shift until differ: 8>>3=1 (LSB 1), 16>>3=2 (LSB 0) differ after 3 shifts. So result=4.
    assert(countMatchingLSBits(8, 16) == 4);

    // Negative numbers? Task says non-negative, but test anyway: -1 and 1: all bits? For int, -1=...1111, 1=...0001: LSB 1=1, next bits: -1 has 1, 1 has 0 -> differ after 1 match -> result=2.
    assert(countMatchingLSBits(-1, 1) == 2);

    // Large difference only at top: 0x7FFFFFFF and 0x80000000 (but these are int range? 0x80000000 is negative) use 2147483647 and -2147483648? For non-negative, use 2147483647 and 2147483646? Those differ at LSB? Actually 2147483647 is all ones, 2147483646 is ...1110 differ at LSB -> result=1.
    assert(countMatchingLSBits(2147483647, 2147483646) == 1);

    // Both zero: equal -> 33
    assert(countMatchingLSBits(0, 0) == 33);

    return 0;
}
