// Write a C++ function that takes a vector of integers and returns the minimum sum of squared differences between each element and a common integer value `p`, where `p` is any integer between 1 and 100 (inclusive). The function should compute the sum as Σ(X[i] - p)² for each possible p from 1 to 100, and return the smallest such sum. The input vector may contain any integers (including negatives, zeros, and values outside the 1–100 range), and its size is at least 1. The result must be returned as a `long long` to avoid overflow, and the function must be efficient for large vectors (up to 10^6 elements). You may assume the input vector is non-empty and that all arithmetic fits within a 64-bit signed integer.
The problem is a simple brute-force enumeration over the candidate values of `p` (from 1 to 100). For each candidate `p`, compute the sum of squared differences with every element in the vector. Track the minimum sum across all candidates. Since the number of candidates is fixed at 100, the time complexity is O(100 * N) = O(N), where N is the vector size. Space complexity is O(1) extra space beyond the input vector. Edge cases include: vector containing only one element (the sum is (X[0]-p)² for each p — the minimum occurs at the p closest to X[0] within [1,100]); vector with all elements equal (the minimum occurs when p equals that value, if within range, otherwise at the nearest bound); values outside the 1–100 range don't affect correctness because we never choose p outside that range. The initial minimum should be set to a very large value (e.g., `LLONG_MAX`). Use `long long` for sums because the squared differences can be large (e.g., X[i] up to 10^6, squared up to 10^12, times 10^6 elements could overflow 32-bit int). The function should be `const`-correct, taking the vector by `const std::vector<long long>&` and returning `long long`.
#include <vector>
#include <algorithm>
#include <climits>

// Given a vector of integers, return the minimum sum over p in [1,100] of
// sum_i (X[i] - p)^2. Assumes vector is non-empty.
long long minSumSquaredDifferences(const std::vector<long long>& X) {
    long long best = LLONG_MAX; // or a large constant like 1LL << 60

    for (long long p = 1; p <= 100; ++p) {
        long long sum = 0;
        for (long long x : X) {
            long long diff = x - p;
            sum += diff * diff;
        }
        best = std::min(best, sum);
    }
    return best;
}
#include <cassert>
#include <vector>

// Declare the function (if not in same translation unit)
long long minSumSquaredDifferences(const std::vector<long long>& X);

int main() {
    // Single element: p=1 gives (5-1)^2=16, p=2 gives 9, p=3 gives 4, p=4 gives1, p=5 gives0, p=6 gives1. Minimum=0
    assert(minSumSquaredDifferences({5}) == 0);

    // Two elements [1, 101]: p=1 gives 0+10000=10000, p=100 gives 99^2+1^2=9802, p=51 gives 2500+2500=5000. Actual min at p=51 gives 5000.
    assert(minSumSquaredDifferences({1, 101}) == 5000);

    // All equal 100: p=100 gives 0 each, sum=0
    assert(minSumSquaredDifferences({100, 100, 100}) == 0);

    // Negative values: X = {-10, 10}. p=1: (-11)^2+9^2=121+81=202; p=100: (-110)^2+(-90)^2=12100+8100=20200; p=? Minimum likely near p=0? But p range only 1..100, so p=1 gives 202. p=2: (-12)^2+8^2=144+64=208, etc. Minimum = 202
    assert(minSumSquaredDifferences({-10, 10}) == 202);

    // Mixed values: {1, 2, 3, 4, 5}. p=3 gives ( -2)^2+(-1)^2+0+1^2+2^2=10, which is min.
    assert(minSumSquaredDifferences({1, 2, 3, 4, 5}) == 10);

    // Large values: {1000000, 1000000}. p=100 gives (999900)^2*2 huge; p=1 gives (999999)^2*2. But p=100 is closer to 1M than p=1, so min at p=100. diff=999900, squared=999800010000, times 2 = 1999600020000. That fits in long long.
    assert(minSumSquaredDifferences({1000000, 1000000}) == 2LL * (1000000 - 100) * (1000000 - 100));

    // Mixed with out-of-range values: {0, 200}. p=1 gives 1+199^2=39602; p=100 gives 10000+10000=20000; p=100 gives min=20000.
    assert(minSumSquaredDifferences({0, 200}) == 20000);

    // Only one element outside range: {1000}. p=100 gives (900)^2=810000, p=1 gives 999^2=998001, min=810000.
    assert(minSumSquaredDifferences({1000}) == 810000);

    // Empty? Not allowed but not tested.

    return 0;
}
