Write a C++ function `int countDistinctWaysToClimb(int n)` that returns the number of distinct ways to climb a staircase with `n` steps, where on each move you can take either 1 or 2 steps. The function must handle input values `n` from 1 to 90 inclusive. For `n=1`, there is exactly 1 way (1 step); for `n=2`, there are 2 ways (1+1, 2). For larger `n`, the result grows quickly, so use a `long long` or `unsigned long long` to avoid overflow, and compute iteratively with constant space (no recursion or memoization arrays). The function should be `const`-correct and not modify any global state.
#include <cassert>

int main() {
    assert(countDistinctWaysToClimb(1) == 1);
    assert(countDistinctWaysToClimb(2) == 2);
    assert(countDistinctWaysToClimb(3) == 3);
    assert(countDistinctWaysToClimb(4) == 5);
    assert(countDistinctWaysToClimb(5) == 8);
    assert(countDistinctWaysToClimb(10) == 89);
    assert(countDistinctWaysToClimb(20) == 10946);
    assert(countDistinctWaysToClimb(50) == 20365011074ULL);
    assert(countDistinctWaysToClimb(70) == 190392490709135ULL);
    assert(countDistinctWaysToClimb(90) == 2880067194370816120ULL);
}
#include <cstdint>

// Returns the number of distinct ways to climb n steps, taking 1 or 2 steps at a time.
// n must be between 1 and 90 inclusive.
unsigned long long countDistinctWaysToClimb(const int n) {
    if (n == 1) {
        return 1;
    }
    if (n == 2) {
        return 2;
    }

    unsigned long long prev2 = 1; // ways for n=1
    unsigned long long prev1 = 2; // ways for n=2
    unsigned long long current = 0;

    for (int step = 3; step <= n; ++step) {
        current = prev1 + prev2;
        prev2 = prev1;
        prev1 = current;
    }

    return current;
}
// This is a classic Fibonacci-like problem: the number of ways to reach step `n` equals the sum of ways to reach step `n-1` (then take 1 step) and step `n-2` (then take 2 steps). Base cases: `f(1)=1`, `f(2)=2`. For `n>=3`, we compute iteratively using three variables: two previous values and the current result. The loop runs from 3 to `n`, updating the previous values with each iteration. Edge cases: `n=1` and `n=2` must be returned directly without entering the loop. Since `n` can be up to 90, the result fits in `unsigned long long` (the 90th Fibonacci-like number is about 7.9e18, which is below the max of ~1.8e19 for `unsigned long long`). Time complexity is O(n) with O(1) auxiliary space. No recursion is needed, avoiding stack overflow for large `n`. The implementation uses `const` parameters and local variables, and does not modify any external state.
