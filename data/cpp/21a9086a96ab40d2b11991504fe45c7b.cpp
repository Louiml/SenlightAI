Write a standalone C++ function that takes a positive integer `n` and returns the `n`-th ugly number, where ugly numbers are defined as positive numbers whose prime factors are limited to 2, 3, and 5. The sequence starts with 1 (which is ugly by convention), and you must compute the result efficiently for any `n` up to 1690 (the maximum guaranteed to fit in a 32-bit signed integer). The function should handle invalid input ( `n <= 0` ) by returning 0, and it must not generate numbers beyond the `n`-th ugly number. Do not use a brute-force check of every integer for ugliness; instead, generate the ugly numbers in increasing order using the classic three-pointer dynamic programming method. The function should be named `nthUglyNumber` and take `int n` as its argument, returning `int`.

// The solution leverages the property that every ugly number (except 1) is obtained by multiplying a smaller ugly number by 2, 3, or 5. Thus, we can generate ugly numbers in sorted order without checking each integer. Maintain an array `ugly` of size `n` where `ugly[0] = 1`. Use three indices `i2`, `i3`, `i5` pointing to the smallest already‑generated ugly number such that multiplying by 2, 3, and 5 respectively yields a value strictly greater than the last generated ugly number. At each step, compute `candidate2 = ugly[i2]*2`, `candidate3 = ugly[i3]*3`, `candidate5 = ugly[i5]*5`, and choose the smallest of these as the next ugly number. Then advance each index whose candidate equals the new ugly number (to avoid duplicates, e.g., 6 = 2*3 = 3*2). This ensures we always pick the next smallest ugly number in the sequence, and we only generate exactly `n` numbers. Edge cases: `n == 1` returns 1 directly by initialization; `n <= 0` returns 0; duplicates are handled by advancing all matching indices. Time complexity is O(n) because each step does constant work and each index only moves forward monotonically, at most `n` increments total. Space complexity is O(n) to store the sequence.

#include <vector>
#include <algorithm>

// Return the n-th ugly number (positive integers with prime factors only 2,3,5).
// Returns 0 if n<=0. Uses dynamic programming with three pointers.
int nthUglyNumber(int n) {
    if (n <= 0) return 0;
    std::vector<int> ugly(n);
    ugly[0] = 1;
    int i2 = 0, i3 = 0, i5 = 0;
    for (int i = 1; i < n; ++i) {
        int next2 = ugly[i2] * 2;
        int next3 = ugly[i3] * 3;
        int next5 = ugly[i5] * 5;
        int next = std::min({next2, next3, next5});
        ugly[i] = next;
        // Advance indices whose product equals the chosen next value
        if (next == next2) ++i2;
        if (next == next3) ++i3;
        if (next == next5) ++i5;
    }
    return ugly[n-1];
}

#include <cassert>

int main() {
    assert(nthUglyNumber(1) == 1);
    assert(nthUglyNumber(2) == 2);
    assert(nthUglyNumber(3) == 3);
    assert(nthUglyNumber(4) == 4);
    assert(nthUglyNumber(5) == 5);
    assert(nthUglyNumber(6) == 6);
    assert(nthUglyNumber(7) == 8);
    assert(nthUglyNumber(10) == 12);
    assert(nthUglyNumber(11) == 15);
    assert(nthUglyNumber(1690) == 2123366400); // largest valid input
    assert(nthUglyNumber(0) == 0);
    assert(nthUglyNumber(-5) == 0);
    return 0;
}
