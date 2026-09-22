/*
Write a standalone C++ function `int countPerfectSquares(int n)` that takes a non-negative integer `n` and returns the number of perfect squares (i.e., integers of the form `k*k` where `k` is a non-negative integer) that are less than or equal to `n`. For example, if `n = 10`, the perfect squares are 0, 1, 4, 9, so the count is 4. Handle the edge case `n = 0` (which should return 1, since 0 = 0*0 is a perfect square), and consider that the input may be as large as `2e9` (so use appropriate types). The function should not use any library functions for square root, and must compute the count by iterating from `k = 0` upward while `k*k <= n`.
*/
#include <cstdint>

// Counts the number of perfect squares (including 0) that are <= n.
// n must be non-negative.
int countPerfectSquares(int n) {
    int count = 0;
    int k = 0;
    while (static_cast<int64_t>(k) * k <= n) {
        ++count;
        ++k;
    }
    return count;
}
#include <cassert>

int main() {
    assert(countPerfectSquares(0) == 1);   // 0
    assert(countPerfectSquares(1) == 2);   // 0, 1
    assert(countPerfectSquares(2) == 2);   // 0, 1
    assert(countPerfectSquares(4) == 3);   // 0, 1, 4
    assert(countPerfectSquares(10) == 4);  // 0, 1, 4, 9
    assert(countPerfectSquares(25) == 6);  // 0,1,4,9,16,25
    assert(countPerfectSquares(100) == 11); // 0,1,4,9,16,25,36,49,64,81,100
    assert(countPerfectSquares(2000000000) == 44722); // sqrt(2e9) ≈ 44721.36, so k=0..44721, count=44722
    return 0;
}
// The solution is straightforward: start with a counter `count = 0` and a variable `k = 0`. While `k * k <= n`, increment the counter and then increment `k`. The loop terminates when `k*k > n`. The number of iterations is exactly the number of perfect squares (including 0) that are ≤ n. For `n = 0`, the condition `0*0 <= 0` is true, so the loop runs once, giving count = 1, which is correct because 0 is a perfect square. For `n = 1`, count = 2 (0 and 1). The main edge case is when `n` is large enough that `k*k` might overflow a 32-bit `int`; since `n` can be up to 2e9, `k` can go up to 44721 (since 44721^2 ≈ 2e9), and `k*k` fits in a 64-bit integer but not necessarily in a 32-bit `int` (44721^2 = 1,999,960,641, which is just under 2^31 = 2,147,483,648, so it fits in 32-bit signed, but for safety we can use `long long` for the multiplication). Time complexity: O(√n) because we iterate roughly up to √n. Space complexity: O(1) auxiliary.
