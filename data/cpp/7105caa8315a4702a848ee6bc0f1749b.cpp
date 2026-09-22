/*
Write a C++ function `widestBalancedPair` that takes two vectors of integers, `nums1` and `nums2`, of equal non-zero length, and returns the width (i.e., `j - i`) of the widest pair of indices `(i, j)` with `i <= j` such that the sum of `(nums1[k] - nums2[k])` from `k = i` to `k = j` inclusive equals zero. If no such pair exists, return 0. The pair width is defined as `j - i`, where a single-element pair `(i, i)` has width 0 (which is always valid because the sum over one element is zero only if `nums1[i] == nums2[i]`). Handle cases with negative numbers, zeros, and duplicates. The function should be `const`-correct with respect to its inputs and must not modify them.
*/

#include <vector>
#include <unordered_map>
#include <algorithm>

// Returns the width of the widest pair of indices (i, j) with i <= j
// such that the sum of (nums1[k] - nums2[k]) for k in [i, j] equals zero.
// If no such pair exists, returns 0.
int widestBalancedPair(const std::vector<int>& nums1, const std::vector<int>& nums2) {
    std::unordered_map<int, int> earliestPrefix; // prefix sum -> earliest index where it appears
    earliestPrefix[0] = -1; // prefix before the first element
    int currentSum = 0;
    int maxWidth = 0;
    const int n = static_cast<int>(nums1.size());
    
    for (int i = 0; i < n; ++i) {
        currentSum += nums1[i] - nums2[i];
        auto it = earliestPrefix.find(currentSum);
        if (it != earliestPrefix.end()) {
            maxWidth = std::max(maxWidth, i - it->second);
        } else {
            earliestPrefix[currentSum] = i;
        }
    }
    return maxWidth;
}

#include <cassert>
#include <vector>
#include "solution.h"

int main() {
    // Case 1: Example from snippet – widest pair spans entire array
    std::vector<int> a1 = {1, 2, 3};
    std::vector<int> b1 = {3, 2, 1};
    assert(widestBalancedPair(a1, b1) == 3); // pref = [0,-2,-2,0] => distance 3

    // Case 2: Single element with equal values gives distance 1
    std::vector<int> a2 = {5};
    std::vector<int> b2 = {5};
    assert(widestBalancedPair(a2, b2) == 1); // pref = [0,0]

    // Case 3: No duplicate prefix sums except the first element has no partner
    std::vector<int> a3 = {1, 2};
    std::vector<int> b3 = {3, 4}; // diffs: -2, -2 => pref = [0,-2,-4] no duplicates -> 0
    assert(widestBalancedPair(a3, b3) == 0);

    // Case 4: Zeros and negative numbers with multiple repeats
    std::vector<int> a4 = {0, -1, 2, 3};
    std::vector<int> b4 = {0, 1, 2, 3}; // diffs: 0,-2,0,0 => pref = [0,0,-2,-2,-2] => max distance for 0 is 1? Actually (0,1) distance1; for -2: indices 2,3,4 => distance 2 (2 to 4? actually earliest 2, latest 4 => 2), also (2,4) distance2. So ans=2.
    assert(widestBalancedPair(a4, b4) == 2);

    // Case 5: Alternating differences create multiple equal prefixes
    std::vector<int> a5 = {1, -1, 1, -1};
    std::vector<int> b5 = {0, 0, 0, 0}; // diffs: 1,-1,1,-1 => pref = [0,1,0,1,0] => distance for 0: indices 0,2,4 => max 4; for 1: 1,3 => 2; ans=4
    assert(widestBalancedPair(a5, b5) == 4);

    // Case 6: All differences non-zero and no repeat except first
    std::vector<int> a6 = {1, 1};
    std::vector<int> b6 = {2, 3}; // diffs: -1, -2 => pref = [0,-1,-3] no duplicates -> 0
    assert(widestBalancedPair(a6, b6) == 0);

    return 0;
}

// The key observation is that the condition "sum of differences from `i` to `j` equals zero" is equivalent to `prefix[j] - prefix[i-1] == 0` where `prefix[t] = sum of (nums1[0]-nums2[0]) through (nums1[t]-nums2[t])`. Thus, we need to find two indices `a < b` such that `prefix[a] == prefix[b]` and maximize `b - a`. Define `prefix[-1] = 0`. We scan left to right, maintaining a hash map from the current prefix sum to the earliest index where that prefix sum was seen. We initialize the map with `{0: -1}` because the prefix sum before any element is 0 at index -1. For each index `i` from 0 to n-1, we add `nums1[i] - nums2[i]` to a running sum `s`. If `s` exists in the map at earliest index `e`, then the width of the pair from `e+1` to `i` is `i - e`, which we compare against the current answer. If `s` is not in the map, we store it with current index `i` (so that future occurrences will yield the longest width). Edge cases: two empty vectors are not allowed; if all differences are non-zero and never repeat a prefix sum, the answer remains 0; a single-element pair with zero difference gives width 0, and that is handled because if `s == 0` at `i`, then `i - (-1) = i+1` width may be positive, but also the pair `(i,i)` gives 0, but the map approach already captures the widest. Time complexity is O(n), space O(n) in the worst case for the map.
