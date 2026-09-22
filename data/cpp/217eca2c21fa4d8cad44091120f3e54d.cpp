// Write a C++ function that takes a vector of integers (which may be negative, zero, or positive) and two parameters `n` (number of elements) and `k` (a positive integer, 1 ≤ k ≤ n). The function must compute the maximum possible sum that can be obtained by selecting exactly `k` consecutive elements from the array, where before the selected block and after the selected block you are also allowed to optionally take any prefix of the array and any suffix of the array, but you must pick at most one contiguous block of length `k` and you cannot overlap the prefix/suffix with the selected block. In other words: choose an index `i` (0 ≤ i ≤ n-k) as the start of the mandatory block of length `k`; then you may optionally add the maximum possible sum of any prefix ending at index i-1 (if i>0) and any suffix starting at index i+k (if i+k < n). However, if the prefix sum or suffix sum is negative, you should take 0 (i.e., skip it). The goal is to maximize the total sum = (optional best prefix) + (sum of block of k starting at i) + (optional best suffix). Return the maximum total sum. The input vector `brojevi` is given, and `n` is its size, `k` is the block length. Note: you cannot take the entire array more than once; the prefix and suffix are disjoint from the selected block. The function should handle edge cases where i=0 (no prefix) or i+k=n (no suffix). The function signature: `long long maxSumWithBlock(const vector<int>& brojevi, int k);` where n = size of vector, and you assume 1 ≤ k ≤ n. Return the maximum total sum as a long long. Also, in your solution, you must precompute arrays for prefix maximum sums and prefix cumulative sums to achieve O(n) time.

The problem is a variant of the maximum subarray with a fixed-length mandatory middle segment. The approach:  
1. Precompute `prefixSum[i]` = sum of elements from index 0 to i (inclusive). Use long long to avoid overflow.  
2. Precompute `bestPrefix[i]` = maximum sum of any prefix ending at or before index i, but if that maximum is negative, store 0 (i.e., best we can get is to take nothing). This can be done in one pass: `bestPrefix[i] = max(0, bestPrefix[i-1] + brojevi[i])`? Wait, careful: Actually we need the maximum sum of a contiguous prefix that ends exactly at i? No, we need "any prefix ending at i-1" – that is the sum of elements from 0 to some index ≤ i-1. To maximize the gain from prefix, we want the maximum subarray sum that starts at index 0 and ends anywhere ≤ i-1. That is simply the maximum prefix sum among all prefixes ending before i. So we maintain `maxPrefixSumUpTo[i]` = max(0, prefixSum[0..i]) but also we want the max over all prefixes up to i-1. So we can compute an array `bestPrefixBefore[i]` = maximum value among prefixSum[0..i] and 0, but only for indices ≤ i. Actually we need for a given i, the maximum of {0, prefixSum[0], prefixSum[1], ..., prefixSum[i-1]}. So we can compute `maxPref[i]` = max(0, prefixSum[0..i]) and then for each i we want maxPref[i-1].  

But also we need the best suffix after the block: the sum of a contiguous suffix starting at index j ≥ i+k and ending at n-1, with possibility to skip (0). That is equivalent to the maximum subarray sum that is a suffix of the array (starting at some index ≥ j). We can precompute suffix sums from the right: `suffixSum[i]` = sum from i to n-1. Then `bestSuffixFrom[i]` = max(0, max(suffixSum[j] for j from i to n-1)). We can compute this by scanning from right: maintain current best suffix sum starting at or after current index.  

Then for each i from 0 to n-k, compute:  
`total = (i>0 ? bestSuffixFrom[i+k] : 0) + (i>0 ? maxPref[i-1] : 0) + (prefixSum[i+k-1] - (i>0 ? prefixSum[i-1] : 0))`.  
But note that `bestSuffixFrom[i+k]` already includes the option to take 0. Similarly `maxPref[i-1]` includes 0. However, we must ensure that prefix and suffix do not overlap with the block – they are disjoint by construction.  

The algorithm is O(n) time and O(n) space for prefix sums, max prefix, and best suffix arrays. Actually we only need two arrays: prefixSum and maxPrefixBeforeIndex (or we can compute on the fly). But simplicity: store `pref` (cumulative sums) and `maxPref` (max of 0 and pref[0..i]). Also store `suf` (suffix sums) and `bestSuf` (best suffix sum from index i). Then iterate. Edge cases: n=1, k=1, negative numbers. If all numbers negative and k is fixed, you must take the block anyway (you can’t skip it), so the sum of that block is negative, but prefix and suffix may be 0. So max total could be negative. The function should return long long, so negative values are fine.  

Complexity: O(n) time, O(n) extra space.

#include <vector>
#include <algorithm>
#include <cstddef>

// Compute the maximum sum achievable by selecting exactly one block of length k,
// optionally adding the best non-overlapping prefix and suffix sums (but skipping if negative).
// The block is mandatory, so its sum is always included even if negative.
long long maxSumWithBlock(const std::vector<int>& brojevi, int k) {
    const size_t n = brojevi.size();
    if (n == 0) return 0;
    
    // prefixSum[i] = sum of elements from 0 to i (inclusive)
    std::vector<long long> prefixSum(n);
    prefixSum[0] = brojevi[0];
    for (size_t i = 1; i < n; ++i) {
        prefixSum[i] = prefixSum[i-1] + brojevi[i];
    }
    
    // maxPrefixBefore[i] = maximum of 0 and prefixSum[0..i-1] for i>0, else 0.
    // This represents the best sum we can get from a prefix ending before index i.
    std::vector<long long> maxPrefixBefore(n, 0);
    long long bestSoFar = 0; // includes the possibility of taking empty prefix (sum 0)
    for (size_t i = 1; i < n; ++i) {
        bestSoFar = std::max(bestSoFar, prefixSum[i-1]);
        maxPrefixBefore[i] = bestSoFar;
    }
    // Note: maxPrefixBefore[0] stays 0, which is correct (no prefix before index 0).
    
    // bestSuffixFrom[i] = maximum of 0 and the sum of any suffix starting at or after index i.
    // We compute suffix sums from the right.
    std::vector<long long> suffixSum(n);
    suffixSum[n-1] = brojevi[n-1];
    for (size_t i = n-1; i > 0; --i) {
        suffixSum[i-1] = suffixSum[i] + brojevi[i-1];
    }
    
    std::vector<long long> bestSuffixFrom(n, 0);
    long long bestSuffix = 0; // best suffix sum among indices >= current position
    for (size_t i = n; i-- > 0; ) {
        bestSuffix = std::max(bestSuffix, suffixSum[i]);
        bestSuffixFrom[i] = bestSuffix;
    }
    // For i = n (beyond the end), we can treat bestSuffixFrom[n] as 0, but we handle with condition.
    
    long long answer = 0;
    bool first = true;
    for (size_t i = 0; i + k <= n; ++i) {
        // Sum of the mandatory block [i, i+k-1]
        long long blockSum = prefixSum[i+k-1] - (i > 0 ? prefixSum[i-1] : 0);
        long long prefixGain = (i > 0) ? maxPrefixBefore[i] : 0;
        long long suffixGain = (i+k < n) ? bestSuffixFrom[i+k] : 0;
        long long total = prefixGain + blockSum + suffixGain;
        if (first) {
            answer = total;
            first = false;
        } else {
            answer = std::max(answer, total);
        }
    }
    return answer;
}

#include <cassert>
#include <vector>

// Free function declaration (already defined elsewhere)
long long maxSumWithBlock(const std::vector<int>& brojevi, int k);

int main() {
    // Case 1: Simple positive numbers
    std::vector<int> v1 = {1, 2, 3, 4, 5};
    assert(maxSumWithBlock(v1, 2) == 14); // block 4+5=9, prefix 1+2+3=6, total 15? Actually let's compute: best is i=1 (block 2+3=5, prefix=1, suffix=4+5=9 total=15). Check: i=0 block 1+2=3, suffix=3+4+5=12 total=15. i=1 block 2+3=5, prefix=1, suffix 4+5=9 total=15. i=2 block 3+4=7, prefix 1+2=3, suffix 5=5 total=15. i=3 block 4+5=9, prefix 1+2+3=6, no suffix total=15. So all 15. So answer should be 15.
    assert(maxSumWithBlock(v1, 2) == 15);

    // Case 2: All negative numbers, block mandatory
    std::vector<int> v2 = {-5, -1, -3};
    // k=1: best block is -1 (i=1), prefix 0 (skip), suffix 0 (skip) total -1.
    assert(maxSumWithBlock(v2, 1) == -1);
    // k=2: block -5-1=-6 or -1-3=-4 => best -4, prefix/suffix 0 -> -4
    assert(maxSumWithBlock(v2, 2) == -4);

    // Case 3: Mixed with negative prefix but negative block
    std::vector<int> v3 = {10, -20, 5, -1, 6};
    // k=2: possible blocks:
    // i=0: block 10-20=-10, suffix best from 2.. = max(0, 5-1+6=10, -1+6=5,6)=10 -> total 0
    // i=1: block -20+5=-15, prefix 10, suffix from 3.. = max(0, -1+6=5,6)=6 -> total -15+10+6=1
    // i=2: block 5-1=4, prefix from 0..1: max(0,10,-10)=10, suffix 6 -> total 20
    // i=3: block -1+6=5, prefix from 0..2: max(0,10,-10,-5)=10, no suffix -> 15
    // Max = 20 at i=2.
    assert(maxSumWithBlock(v3, 2) == 20);

    // Case 4: Single element
    std::vector<int> v4 = {7};
    assert(maxSumWithBlock(v4, 1) == 7);
    std::vector<int> v5 = {-4};
    assert(maxSumWithBlock(v5, 1) == -4);

    // Case 5: k equals n (whole array as block, no prefix/suffix)
    std::vector<int> v6 = {3, -1, 2};
    assert(maxSumWithBlock(v6, 3) == 4);
    std::vector<int> v7 = {-2, -3};
    assert(maxSumWithBlock(v7, 2) == -5);

    // Case 6: Prefix and suffix both contribute, block sum positive
    std::vector<int> v8 = {5, -10, 4, -2, 8};
    // k=1: try each block:
    // i=0 block 5, suffix best from 1.. = max(0, -10+4-2+8=0, 4-2+8=10, -2+8=6,8)=10 => total 15
    // i=1 block -10, prefix 5, suffix from 2.. = max(0,4-2+8=10, -2+8=6,8)=10 => 5-10+10=5
    // i=2 block 4, prefix max 5 (or 5-10=-5 but best is 5), suffix from 3.. = max(0,-2+8=6,8)=8 => 5+4+8=17
    // i=3 block -2, prefix best 5, suffix 8 => 11
    // i=4 block 8, prefix best 5 (from 0..3 max is 5) => 13
    // Max = 17 at i=2.
    assert(maxSumWithBlock(v8, 1) == 17);

    // Case 7: Zero values
    std::vector<int> v9 = {0, 0, 0};
    assert(maxSumWithBlock(v9, 2) == 0);

    return 0;
}
