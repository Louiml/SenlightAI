/*
Write a C++ function named `sumOfCubes` that takes a single positive integer `n` and returns the sum of the cubes of all integers from 1 to `n` inclusive. For example, if `n = 3`, the result should be \(1^3 + 2^3 + 3^3 = 36\). The input `n` is guaranteed to be between 1 and 10,000 (inclusive). The function must use recursion (not a loop or a direct formula) to compute the result. The return type should be `long long` to avoid overflow since the sum grows as \(O(n^4)\). Ensure the function is `const`-correct where appropriate (e.g., parameters passed by value are fine here since `n` is a primitive integer).
*/
#include <cstdint>

// Recursively computes the sum of cubes from 1 to n inclusive.
// n must be a positive integer.
long long sumOfCubes(long long n) {
    // Base case: sum of cubes from 1 to 1 is 1^3 = 1.
    if (n == 1) {
        return 1;
    }
    // Recursive step: n^3 + sum of cubes from 1 to n-1.
    return n * n * n + sumOfCubes(n - 1);
}
#include <cassert>

int main() {
    // Base case: n = 1
    assert(sumOfCubes(1) == 1);

    // Small values
    assert(sumOfCubes(2) == 9);          // 1 + 8
    assert(sumOfCubes(3) == 36);         // 1 + 8 + 27
    assert(sumOfCubes(4) == 100);        // 1 + 8 + 27 + 64

    // Larger value: check against known result
    assert(sumOfCubes(5) == 225);        // 1 + 8 + 27 + 64 + 125

    // Check that the result matches the closed-form formula for a moderate n
    long long n = 100;
    long long expected = (n * n * (n + 1) * (n + 1)) / 4; // (sum of integers)^2
    assert(sumOfCubes(n) == expected);

    // Check a large but safe n to ensure no overflow and correct recursion
    n = 1000;
    expected = (n * n * (n + 1) * (n + 1)) / 4;
    assert(sumOfCubes(n) == expected);

    // Additional check for n = 10
    assert(sumOfCubes(10) == 3025);      // (10*11/2)^2 = 55^2 = 3025
}
// The problem is a classic recursive summation: the sum of cubes from 1 to `n` equals `n^3` plus the sum of cubes from 1 to `n-1`. The base case is when `n == 1`, at which point the sum is simply `1^3 = 1`. For any `n > 1`, we compute `n*n*n` (which is `n^3`) and add it to the recursive call `sumOfCubes(n-1)`. Because `n` is at most 10,000, the recursion depth is at most 10,000, which is safe for typical stack sizes. The time complexity is \(O(n)\) due to one recursive call per decrement from `n` down to 1, and each call does constant-time arithmetic. The space complexity is \(O(n)\) due to the recursion stack, which holds one frame per value from `n` down to 1. Edge cases: `n = 1` returns immediately; `n = 2` computes `8 + 1 = 9`; large `n` up to 10,000 is fine; overflow is avoided by using `long long` because the maximum sum is about \((10000^4)/4 \approx 2.5 \times 10^{15}\), which fits in `long long`. The function must be recursive; no loops or closed-form formula is allowed.
