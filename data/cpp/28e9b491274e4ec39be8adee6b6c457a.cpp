// Given a list of N positive integers representing the rank a person expects to receive in a competition (where ranks are 1 through N, but the list may contain duplicates or out-of-range values), write a C++ function that returns the minimum possible sum of absolute differences between the actual assigned ranks (a permutation of 1..N) and the expected ranks, when the expected ranks are optimally matched to the actual ranks. Specifically, sort the expected ranks and compute the sum of |(i+1) - expected[i]| for i from 0 to N-1. The input is a vector of integers (size N, N≥1), and the function should return a `long long` because the sum can exceed 32-bit int for large N.
#include <cassert>
#include <vector>

// Free function declared in the solution section (included here for completeness)
long long minimumRankSum(std::vector<int> expected);

int main() {
    // Example from typical expected ranks: 1,2,3 -> sorted same, differences 0.
    assert(minimumRankSum({1, 2, 3}) == 0);
    // Reverse order: 3,2,1 -> sorted 1,2,3 -> sum 0.
    assert(minimumRankSum({3, 2, 1}) == 0);
    // All same: 1,1,1 -> sorted 1,1,1 -> diffs 0,1,2 sum=3.
    assert(minimumRankSum({1, 1, 1}) == 3);
    // Single element case.
    assert(minimumRankSum({5}) == 4);
    // Out-of-range values: 10,1 -> sorted 1,10 -> diffs |1-1|+|2-10|=0+8=8.
    assert(minimumRankSum({10, 1}) == 8);
    // Duplicates with larger N: 2,2,2,2 -> sorted -> diffs 1,0,1,2 sum=4.
    assert(minimumRankSum({2, 2, 2, 2}) == 4);
    // Large case to ensure long long: 100000 values all 1.
    std::vector<int> big(100000, 1);
    // Sum of |(i+1)-1| for i=0..99999 = sum of i from 0 to 99999 = 99999*100000/2 = 4999950000
    assert(minimumRankSum(big) == 4999950000LL);
    // Mixed: 4,1,3 -> sorted 1,3,4 -> diffs 0,1,1 sum=2.
    assert(minimumRankSum({4, 1, 3}) == 2);
    // N=1 with expected=1 -> 0.
    assert(minimumRankSum({1}) == 0);
    return 0;
}
#include <vector>
#include <algorithm>
#include <cstdlib>

// Given a vector of expected ranks, return the minimal sum of absolute
// differences when matched to the actual ranks 1..N after sorting.
long long minimumRankSum(std::vector<int> expected) {
    std::sort(expected.begin(), expected.end());
    long long sum = 0;
    const int n = static_cast<int>(expected.size());
    for (int i = 0; i < n; ++i) {
        sum += std::llabs(static_cast<long long>(i + 1) - expected[i]);
    }
    return sum;
}
// The problem is a classic greedy assignment: to minimize the sum of absolute differences between two sets of values (here, the fixed ranks 1..N and the given expected ranks), we sort both sets and pair them in order (the i-th smallest expected with i-th smallest actual). This is optimal because the absolute difference is a convex function, and the rearrangement inequality ensures that sorting both sequences and pairing corresponding elements minimizes the sum of absolute differences. For the input, we sort the expected ranks in non-decreasing order, then for each index i (0-based), compute abs((i+1) - sorted[i]) and accumulate. Edge cases include N=1 (simple difference), duplicate expected ranks (e.g., all equal) — still correct, and values that might be outside 1..N (the formula still works, it's just a sum of differences). Use `long long` to avoid overflow for large N (e.g., N up to 10^5 or more, differences up to ~N, sum ~N^2 which can exceed 2^31). Time complexity is O(N log N) due to sorting, space O(1) extra aside from input vector (or O(N) if a copy is needed). No special handling is required for negative numbers because the problem statement implies positive integers, but the absolute value works regardless.
