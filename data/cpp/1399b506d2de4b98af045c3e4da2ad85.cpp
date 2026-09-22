/*
Write a C++ function `int rangeBitwiseAnd(int m, int n)` that computes the bitwise AND of all integers in the inclusive range `[m, n]`, where `0 <= m <= n <= 2^31 - 1`. The function must handle large ranges efficiently without iterating through every number. For example, `rangeBitwiseAnd(5, 7)` should return `4` because `5 & 6 & 7 = 4`, and `rangeBitwiseAnd(0, 0)` should return `0`. The solution must run in constant time and constant space relative to the input size.
*/

#include <cstdint>

// Compute bitwise AND of all integers in the inclusive range [m, n].
// Precondition: 0 <= m <= n <= 2^31 - 1.
int rangeBitwiseAnd(int m, int n) {
    int result = 0;
    for (int bit = 0; bit < 31; ++bit) {
        const int divisor = 1 << bit;               // 2^bit
        const int a = m / divisor;
        const int b = n / divisor;
        if (a == b && (a & 1) != 0) {
            result |= (1 << bit);
        }
    }
    return result;
}

#include <cassert>

int main() {
    assert(rangeBitwiseAnd(0, 0) == 0);
    assert(rangeBitwiseAnd(1, 1) == 1);
    assert(rangeBitwiseAnd(5, 7) == 4);
    assert(rangeBitwiseAnd(0, 1) == 0);
    assert(rangeBitwiseAnd(10, 10) == 10);
    assert(rangeBitwiseAnd(10, 12) == 8);
    assert(rangeBitwiseAnd(17, 31) == 16);
    assert(rangeBitwiseAnd(0, 2147483647) == 0);
    assert(rangeBitwiseAnd(2147483647, 2147483647) == 2147483647);
    assert(rangeBitwiseAnd(12, 15) == 12);
    return 0;
}

// The key observation is that the bitwise AND of a range `[m, n]` is determined by the common prefix of the binary representations of `m` and `n`. Any bit position where `m` and `n` differ will become `0` in the final result because that bit will toggle somewhere within the range. For each bit position `i` (from 0 to 30, since the maximum value fits in 31 bits), we can check whether the `i`-th bit is the same for all numbers in the range. This is true if and only if `floor(m / 2^i) == floor(n / 2^i)`, meaning that dividing by `2^i` groups numbers into blocks of size `2^i` where the lower `i` bits cycle; if `m` and `n` fall into the same block, then the `i`-th bit is constant across the range. Furthermore, that constant bit must be `1` for it to contribute to the result, which occurs when `floor(m / 2^i)` is odd. Therefore, for each bit `i`, set that bit in the answer if `(m >> i) == (n >> i)` and `((m >> i) & 1) == 1`. This matches the code's logic using `k = 2^i`, `a = m / k`, `b = n / k`, and checking `a == b && a % 2`. Edge cases include when `m == n` (the result is just `m`), when `m == 0` (result is 0 because bit 0 will differ if `n > 0`), and very large ranges where many high bits become `0`. The time complexity is O(31) = O(1), and space complexity is O(1).
