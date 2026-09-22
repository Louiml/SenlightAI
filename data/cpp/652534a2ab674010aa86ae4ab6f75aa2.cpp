Write a C++ function that takes two non-negative integers `left` and `right` (where `left <= right`) and returns the bitwise AND of all integers in the inclusive range `[left, right]`. For example, given `left = 5` and `right = 7`, the result should be `4` because `5 & 6 & 7 = 4`. The function should handle edge cases such as `left == right`, where the answer is simply `left`, and large values up to `2^31 - 1`. Do not use brute-force iteration over the range, as that would be too slow for large ranges. Instead, implement an efficient bit-manipulation algorithm.

The key observation is that the bitwise AND of a range `[left, right]` is determined by the common prefix of the binary representations of `left` and `right`. Any bit position where `left` and `right` differ will become `0` in the AND result because there will be at least one number in the range that flips that bit. The algorithm repeatedly right-shifts both `left` and `right` while `left < right`, counting the number of shifts. This effectively removes all differing lower bits. After this loop, `left` contains the common prefix (shifted right), and we shift it back left by the count to restore its original position, giving the final AND result. For example, with `left = 5 (101)` and `right = 7 (111)`, the loop shifts twice until `left == right`, then returns `1 << 2 = 4`. Edge cases: if `left == right`, the loop runs zero times and returns `left`; if `left = 0`, the result is always `0` because `0` is in the range. Time complexity is `O(1)` (at most 31 iterations for 32-bit integers), and space complexity is `O(1)`.

#include <cstdint>

// Returns the bitwise AND of all integers in the inclusive range [left, right].
// Assumes left and right are non-negative and left <= right.
int rangeBitwiseAnd(int left, int right) {
    int shift_count = 0;
    while (left < right) {
        left >>= 1;
        right >>= 1;
        ++shift_count;
    }
    return left << shift_count;
}

int main() {
    // Basic cases
    assert(rangeBitwiseAnd(5, 7) == 4);
    assert(rangeBitwiseAnd(0, 0) == 0);
    assert(rangeBitwiseAnd(1, 1) == 1);
    
    // Single element range
    assert(rangeBitwiseAnd(10, 10) == 10);
    
    // Range where result is 0
    assert(rangeBitwiseAnd(0, 1) == 0);
    assert(rangeBitwiseAnd(1, 2) == 0);
    
    // Larger range
    assert(rangeBitwiseAnd(8, 15) == 8); // 1000 & 1001 & ... & 1111 = 1000
    assert(rangeBitwiseAnd(16, 31) == 16);
    
    // Maximum value edge
    assert(rangeBitwiseAnd(2147483647, 2147483647) == 2147483647);
    assert(rangeBitwiseAnd(2147483646, 2147483647) == 2147483646);
    
    // Range spanning many powers of two
    assert(rangeBitwiseAnd(13, 15) == 12); // 1101 & 1110 & 1111 = 1100
    assert(rangeBitwiseAnd(6, 7) == 6);
}
