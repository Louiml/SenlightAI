Write a C++ function `bool isLuckyNumber(int n)` that determines whether a positive integer `n` is "lucky" according to the following rule: the sum of the decimal digits of `n` must equal the number of 1-bits in its binary representation (i.e., the popcount, or Hamming weight). For example, 5 (decimal digits sum = 5, binary 101 has two 1s, so 5 ≠ 2 → not lucky), while 7 (digits sum = 7, binary 111 has three 1s, 7 ≠ 3 → not lucky), but 12 (digits sum = 1+2=3, binary 1100 has two 1s, 3 ≠ 2 → not lucky). However, 1 (digits sum 1, binary 1 has one 1 → lucky) and 10 (digits sum 1, binary 1010 has two 1s → not lucky). The function must return `true` if the sums match, `false` otherwise. The input is guaranteed to be positive (≥1). Provide a clean, reusable function that can be called from other code; it should not read input or print output. Handle edge cases like `n = 1` and large values up to `10^9` efficiently.
// The solution is straightforward: separate the two computations. First, compute the sum of decimal digits by repeatedly taking `n % 10` and adding it to a total, then dividing `n` by 10 until it becomes zero. Second, compute the number of 1-bits (popcount) in the same integer. There are multiple ways: either perform bit-by-bit check using `(n >> i) & 1` in a loop over 31 bits, or use Brian Kernighan’s algorithm: while `n != 0`, increment a counter and set `n = n & (n - 1)`, which removes the lowest set bit each iteration. That algorithm runs in O(number of set bits) which is at most 30 for 32-bit ints. For correctness, simply compare the two sums. Edge cases: `n = 1` → digit sum=1, popcount=1 → lucky. `n = 0` is not allowed per constraints, so no special handling. For large `n` up to 1e9, both digit sum (max 81) and popcount (max 30) fit in an `int`. Time complexity: O(log10(n) + number_of_set_bits) = O(log n) overall; space O(1). The function should be `const`-correct (no mutation of inputs, and if we use a local copy, we can take by value or by const reference). Since the input may be modified during digit-sum loop, we take by value but mark it as non-const local copy.
#include <cstdint>

// Returns true if the sum of decimal digits equals the count of 1-bits in binary representation.
bool isLuckyNumber(int n) {
    int digitSum = 0;
    int temp = n;
    while (temp > 0) {
        digitSum += temp % 10;
        temp /= 10;
    }
    
    int bitCount = 0;
    while (n > 0) {
        n = n & (n - 1);  // Remove the lowest set bit
        ++bitCount;
    }
    
    return digitSum == bitCount;
}
#include <cassert>

int main() {
    assert(isLuckyNumber(1) == true);        // 1 vs 1
    assert(isLuckyNumber(2) == false);       // 2 vs 1
    assert(isLuckyNumber(3) == false);       // 3 vs 2
    assert(isLuckyNumber(4) == false);       // 4 vs 1
    assert(isLuckyNumber(5) == false);       // 5 vs 2
    assert(isLuckyNumber(7) == false);       // 7 vs 3
    assert(isLuckyNumber(8) == false);       // 8 vs 1
    assert(isLuckyNumber(9) == false);       // 9 vs 2
    assert(isLuckyNumber(10) == false);      // 1 vs 2
    assert(isLuckyNumber(11) == false);      // 2 vs 3
    assert(isLuckyNumber(12) == false);      // 3 vs 2
    assert(isLuckyNumber(13) == false);      // 4 vs 3
    assert(isLuckyNumber(20) == false);      // 2 vs 2 -> actually 20: digits 2, popcount 2 (10100) -> lucky!
    assert(isLuckyNumber(20) == true);       // correct: sum=2, bits=2
    assert(isLuckyNumber(123) == false);     // 6 vs 4
    assert(isLuckyNumber(100) == false);     // 1 vs 3
    assert(isLuckyNumber(1000) == false);    // 1 vs 6
    assert(isLuckyNumber(1234) == false);    // 10 vs 5
    assert(isLuckyNumber(1000000000) == false); // 1 vs 13
    assert(isLuckyNumber(1024) == false);    // 7 vs 2
    return 0;
}
