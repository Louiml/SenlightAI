Given a set of up to 20 positive integers, write a C++ function `findEqualSubsets` that returns a vector of two vectors of integers: the first vector contains a subset `A` and the second vector contains a disjoint subset `B` such that the sum of elements in `A` equals the sum of elements in `B`. If no such pair of disjoint non-empty subsets exists, return an empty vector (indicating "Impossible"). The function should accept a `const std::vector<int>&` input and return `std::vector<std::vector<int>>`. The solution must handle duplicate values and ensure that the two subsets are disjoint (i.e., they use different indices from the original set). If multiple solutions exist, any one valid pair is acceptable.
The problem is to find two disjoint non-empty subsets with equal sums. A direct subset-sum approach would be exponential in the worst case, but since the maximum set size is 20, we can use a meet-in-the-middle or a simpler brute-force search. However, the intended approach from the snippet is to iterate over all possible subsets for one side (using a bitmask), compute its sum `S`, then use dynamic programming (knapsack-style subset sum) on the remaining elements to see if a subset sum `S` can be formed. If found, we have subset A from the first mask and subset B from the DP reconstruction. The DP is `f(index, current_sum)` which returns whether a subset of the remaining elements can sum to `current_sum` starting from index `index`. We use memoization on `(index, current_sum)`. Important edge cases: the empty subset is not allowed for either A or B (since sum equal to 0 is skipped); we must ensure A and B are disjoint by construction (A from one mask, B from the complement bits). Also, duplicate values are fine; the indices distinguish them. Complexity: for each mask (2^n) we run a DP over at most 20 remaining elements and sum up to 20*10^6 (but with pruning), so worst-case time is O(2^n * n * maxSum). Space is O(n * maxSum) for the memo table. With n=20 this is feasible if sums are small, but in worst case it might be large; however, typical constraints keep sums moderate. We avoid recomputing DP from scratch for every mask by either clearing individually or using a global table with a timestamp, but for simplicity we allocate a fresh DP each time. The solution returns an empty vector if none found.
#include <vector>
#include <cstring>

// Finds two disjoint non-empty subsets with equal sums.
// Returns a vector of two vectors: {A, B}. If impossible, returns empty vector.
std::vector<std::vector<int>> findEqualSubsets(const std::vector<int>& input) {
    int n = (int)input.size();
    if (n == 0) return {};

    // DP memoization for subset sum on a list of remaining elements.
    // We'll use a separate function internally with global state.
    // Since we need to return vector, we'll implement as a lambda or helper.

    // Helper structure to simplify state.
    struct SubsetFinder {
        int m;
        std::vector<int> r; // remaining elements
        std::vector<std::vector<int>> dp;
        std::vector<int> chosen; // reconstruction

        // DP: can we make sum t starting from index x?
        bool f(int x, int t) {
            if (dp[x][t] != -1) return dp[x][t];
            int& ret = dp[x][t] = 0;
            if (t == 0) return ret = 1;
            if (x == m) return ret = 0;
            if (f(x+1, t)) return ret = 1;
            if (r[x] <= t && f(x+1, t - r[x])) {
                chosen.push_back(r[x]);
                return ret = 1;
            }
            return ret;
        }

        // Reset for a new subset of remaining elements.
        void reset(const std::vector<int>& remaining, int targetSum) {
            m = (int)remaining.size();
            r = remaining;
            // dp dimensions: (m+1) x (targetSum+1). Allocate with -1.
            dp.assign(m+1, std::vector<int>(targetSum+1, -1));
            chosen.clear();
        }
    };

    // Iterate over all non-empty subsets for A.
    for (int mask = 1; mask < (1 << n); ++mask) {
        int sumA = 0;
        std::vector<int> A;
        std::vector<int> remaining;
        for (int i = 0; i < n; ++i) {
            if (mask & (1 << i)) {
                sumA += input[i];
                A.push_back(input[i]);
            } else {
                remaining.push_back(input[i]);
            }
        }
        if (sumA == 0) continue; // A must be non-empty (mask not zero) and sum>0? Actually sum can be positive, skip zero.
        if (remaining.empty()) continue; // Need at least one element for B.

        SubsetFinder finder;
        finder.reset(remaining, sumA);
        if (finder.f(0, sumA)) {
            // B is finder.chosen (reversed order because push_back during backtracking)
            std::vector<int> B = finder.chosen;
            // Note: f returns true for sumA, so B sums to sumA.
            return {A, B};
        }
    }
    return {}; // Impossible
}
#include <cassert>
#include <iostream>
#include <vector>

// Declare the function (assuming header here, but for test we just copy)
// Since we cannot include the solution file directly, we'll paste the function above.
// For the test, we'll assume findEqualSubsets is available.

int main() {
    // Test 1: Simple case {1,2,3} -> {1,2} and {3} both sum to 3
    std::vector<int> t1 = {1,2,3};
    auto res1 = findEqualSubsets(t1);
    assert(res1.size() == 2);
    int sA = 0, sB = 0;
    for (int v : res1[0]) sA += v;
    for (int v : res1[1]) sB += v;
    assert(sA == sB && sA > 0);

    // Test 2: {1,1,1,1} -> {1} and {1} sum to 1
    std::vector<int> t2 = {1,1,1,1};
    auto res2 = findEqualSubsets(t2);
    assert(res2.size() == 2);
    assert(res2[0][0] == 1 && res2[1][0] == 1);

    // Test 3: {1,2,4,8} -> no equal subsets, impossible
    std::vector<int> t3 = {1,2,4,8};
    auto res3 = findEqualSubsets(t3);
    assert(res3.empty());

    // Test 4: {5,5} -> {5} and {5}
    std::vector<int> t4 = {5,5};
    auto res4 = findEqualSubsets(t4);
    assert(res4.size() == 2);
    assert(res4[0][0] == 5 && res4[1][0] == 5);

    // Test 5: {2,3,4,1} -> {2,3} and {4,1} both sum to 5
    std::vector<int> t5 = {2,3,4,1};
    auto res5 = findEqualSubsets(t5);
    assert(res5.size() == 2);
    int sum5a = 0, sum5b = 0;
    for (int v : res5[0]) sum5a += v;
    for (int v : res5[1]) sum5b += v;
    assert(sum5a == sum5b && sum5a == 5);

    // Test 6: {1,2,3,4,5} -> many solutions, check validity
    std::vector<int> t6 = {1,2,3,4,5};
    auto res6 = findEqualSubsets(t6);
    assert(res6.size() == 2);
    // Verify disjointness by checking indices? Since function returns values, we can't directly verify indices,
    // but we know disjointness is guaranteed by construction. We'll just check sums.
    int s6a = 0, s6b = 0;
    for (int v : res6[0]) s6a += v;
    for (int v : res6[1]) s6b += v;
    assert(s6a == s6b);

    // Test 7: Single element {10} -> impossible
    std::vector<int> t7 = {10};
    assert(findEqualSubsets(t7).empty());

    // Test 8: Three elements {2,2,4} -> {2,2} and {4}
    std::vector<int> t8 = {2,2,4};
    auto res8 = findEqualSubsets(t8);
    assert(res8.size() == 2);
    int s8a = 0, s8b = 0;
    for (int v : res8[0]) s8a += v;
    for (int v : res8[1]) s8b += v;
    assert(s8a == s8b && s8a == 4);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
