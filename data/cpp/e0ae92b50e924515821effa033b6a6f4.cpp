Given a positive integer `n` and an array of `n` integers where each integer is between 1 and `n` inclusive, write a C++ function that returns the maximum number of disjoint pairs that can be formed such that each pair sums to the same value. A pair consists of two distinct elements from the array (they can have equal values if duplicates exist, but each element can be used at most once). The function should take `int n` and a `std::vector<int>&` (or by value) and return an `int` representing the maximum count of pairs sharing a common sum. The array is guaranteed to have at least 1 element and at most 50 elements. For example, if the array is `[1, 2, 3, 4]`, the maximum occurs for sum 5, forming pairs (1,4) and (2,3), so output 2. If the array is `[1, 1, 1, 1]`, the maximum is 2 for any sum equal to 2 (two pairs of (1,1)). You must consider all possible sums from 2 to 2n.
// The key observation is that since the maximum value is `n`, any possible pair sum lies between `2` (two ones) and `2n` (two n's). For each candidate sum `s` from 2 to 2n, we count how many disjoint pairs with sum `s` can be formed. To do this efficiently, we create a frequency map of each value in the array. For each distinct value `v`, its required partner is `other = s - v`. If `other` is within `[1, n]` and appears in the map, then the number of pairs we can form using `v` and `other` is `min(freq[v], freq[other])`. Summing this over all distinct `v` counts each pair twice (once when `v` is considered, once when `other` is considered), so we divide the total by 2. This gives the maximum number of disjoint pairs for that sum. The answer is the maximum over all sums. Edge cases include duplicate values (handled by the min), the case where `other == v` (then the count is `freq[v] / 2` which is naturally produced because `min(f,v)` summed twice and divided by 2 gives `freq[v]`/2 when needed), and arrays with only one element (result 0). Time complexity is O(n * t) where t is the number of distinct values (≤ n), so O(n^2) overall, which is fine for n ≤ 50. Space complexity is O(n) for the frequency map.
#include <vector>
#include <map>
#include <algorithm>

// Returns the maximum number of disjoint pairs with a common sum.
int maxPairsWithCommonSum(int n, const std::vector<int>& arr) {
    std::map<int, int> freq;
    for (int val : arr) {
        ++freq[val];
    }

    int best = 0;
    for (int sum = 2; sum <= 2 * n; ++sum) {
        int totalPairs = 0;
        for (const auto& [val, count] : freq) {
            int other = sum - val;
            if (other >= 1 && other <= n && freq.count(other)) {
                totalPairs += std::min(count, freq.at(other));
            }
        }
        totalPairs /= 2;
        best = std::max(best, totalPairs);
    }
    return best;
}
#include <cassert>
#include <vector>

int main() {
    // Example 1: [1,2,3,4], best sum = 5, two pairs.
    assert(maxPairsWithCommonSum(4, {1,2,3,4}) == 2);

    // Example 2: All ones, can form two pairs of (1,1).
    assert(maxPairsWithCommonSum(4, {1,1,1,1}) == 2);

    // Single element, no pairs possible.
    assert(maxPairsWithCommonSum(1, {1}) == 0);

    // Duplicate values with different pairs: [2,2,3,3], sum=5 gives 2 pairs.
    assert(maxPairsWithCommonSum(3, {2,2,3,3}) == 2);

    // Unbalanced: [1,2,2,3], sum=4 gives pairs (1,3) and (2,2) → 2.
    assert(maxPairsWithCommonSum(3, {1,2,2,3}) == 2);

    // All distinct values from 1 to 5: best is sum=6 with pairs (1,5),(2,4) → 2 (3 is left).
    assert(maxPairsWithCommonSum(5, {1,2,3,4,5}) == 2);

    // Large duplicates: [1,1,2,2,3,3], sum=4 gives pairs (1,3) twice and (2,2) once → 3.
    assert(maxPairsWithCommonSum(3, {1,1,2,2,3,3}) == 3);

    // All identical value 2 with n=2, arr=[2,2,2,2], sum=4 gives two pairs.
    assert(maxPairsWithCommonSum(2, {2,2,2,2}) == 2);

    // Value 1 and n=1, arr=[1], no pairs.
    assert(maxPairsWithCommonSum(1, {1}) == 0);

    // Empty? Not allowed per spec but test small extreme: arr=[1,1,1] sum=2 gives one pair (min count 1 after divide by 2? freq[1]=3, total=3, /2=1).
    assert(maxPairsWithCommonSum(1, {1,1,1}) == 1);
}
