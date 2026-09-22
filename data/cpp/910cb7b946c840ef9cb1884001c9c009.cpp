// Write a C++ function named `rangeBitwiseAnd` that accepts two non-negative integers, `left` and `right`, with `left <= right`, and returns the bitwise AND of all integers in the inclusive range `[left, right]`. The function must handle edge cases such as zero inputs and large ranges efficiently. Do not use loops that iterate over every number in the range. Instead, implement an efficient algorithm based on common prefix bits of `left` and `right`. The function should be `const`-correct (though it has no member variables, it should be a free function) and return an `int` (the bitwise AND result). The solution must work for all `int` values up to `INT_MAX`. Provide a single free function with appropriate standard library includes.
int main() {
    assert(rangeBitwiseAnd(5, 7) == 4);        // 5&6&7 = 4
    assert(rangeBitwiseAnd(0, 0) == 0);        // edge case zero
    assert(rangeBitwiseAnd(0, 2147483647) == 0);
    assert(rangeBitwiseAnd(1, 1) == 1);        // single element
    assert(rangeBitwiseAnd(1, 2) == 0);        // different lengths
    assert(rangeBitwiseAnd(10, 10) == 10);     // left == right
    assert(rangeBitwiseAnd(12, 15) == 12);     // 1100 & 1101 & 1110 & 1111 = 1100
    assert(rangeBitwiseAnd(2147483647, 2147483647) == 2147483647);
    assert(rangeBitwiseAnd(1073741824, 2147483647) == 0); // crosses power-of-two
    return 0;
}
#include <cmath>

// Returns the bitwise AND of all integers in [left, right].
int rangeBitwiseAnd(int left, int right) {
    if (left == 0 || right == 0) return 0;
    if (static_cast<int>(log2(left)) != static_cast<int>(log2(right))) return 0;

    int shift = 0;
    while (left != right) {
        left >>= 1;
        right >>= 1;
        ++shift;
    }
    return left << shift;
}
// The bitwise AND of a range `[left, right]` can be computed by finding the common prefix of the binary representations of `left` and `right`. Any bit position where `left` and `right` differ will become `0` in the final AND, because as you iterate through the range, at some point that bit will toggle from `0` to `1` (or vice versa), and the AND of both values will clear it. Therefore, we only need to keep the most significant bits where `left` and `right` agree. The algorithm:
// 1. If either `left` or `right` is `0`, return `0` (since any range containing `0` has AND result `0`).
// 2. Compare the bit-lengths of `left` and `right` using `log2`. If their integer lengths differ, then there is at least one power-of-two boundary between them, meaning the most significant differing bit exists, so the result is `0`. This is because the range will include both a number with that high bit set and a number without it.
// 3. Otherwise, repeatedly right-shift both `left` and `right` until they become equal. Count the number of shifts. After the loop, `left` (equal to `right`) is the common prefix. Left-shift it back by the number of shifts to restore the original bit positions, which gives the final AND result.
// Time complexity: O(number of bits) = O(1) since `int` has at most 32 bits. Space complexity: O(1). Edge cases: `left == 0`, `right == 0`, `left == right` (returns the value itself), and cases where lengths differ.
