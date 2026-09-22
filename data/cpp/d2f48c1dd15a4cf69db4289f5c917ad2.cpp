/*
Given an array of `n` positive integers and `q` queries, for each query with a target value `a`, determine the minimum number of elements (from the array) that must be selected such that their sum is at least `a`. You may select any elements, but each element can be used at most once, and the goal is to minimize the count. If it is impossible to reach the target sum (even using all elements), return `-1`. Write a C++ function `vector<long long> minElementsForTargets(vector<long long> arr, vector<long long> queries)` that takes the array of values and a list of targets, and returns a vector of results in the same order as the queries. The function should handle up to `n = 1,500,000` and `q` up to the same magnitude, with each value and target up to `10^18`. Optimize for speed and memory.
*/
#include <vector>
#include <algorithm>
#include <cstdint>

// Precondition: arr contains positive integers, queries are non-negative.
// Returns: for each query, the minimum number of largest elements needed
// to reach at least the target sum, or -1 if impossible.
std::vector<long long> minElementsForTargets(std::vector<long long> arr, std::vector<long long> queries) {
    // Sort in descending order to prefer larger elements.
    std::sort(arr.begin(), arr.end(), std::greater<long long>());
    
    // Build prefix sums: pref[i] = sum of largest i elements (1-indexed).
    std::vector<long long> pref(arr.size() + 1, 0);
    for (std::size_t i = 0; i < arr.size(); ++i) {
        pref[i + 1] = pref[i] + arr[i];
    }
    
    std::vector<long long> result;
    result.reserve(queries.size());
    
    for (long long target : queries) {
        // Find first index with prefix sum >= target.
        // pref is non-decreasing because all arr elements are positive.
        auto it = std::lower_bound(pref.begin(), pref.end(), target);
        if (it == pref.end()) {
            result.push_back(-1);
        } else {
            result.push_back(static_cast<long long>(it - pref.begin()));
        }
    }
    return result;
}
#include <cassert>
#include <vector>

// Solution function declaration (provided above)
std::vector<long long> minElementsForTargets(std::vector<long long> arr, std::vector<long long> queries);

int main() {
    // Basic test
    std::vector<long long> arr1 = {5, 1, 3, 2};
    std::vector<long long> q1 = {8, 6, 11, 0, 12};
    std::vector<long long> res1 = minElementsForTargets(arr1, q1);
    assert(res1 == (std::vector<long long>{2, 2, 3, 0, 4}));
    // Explanation: sorted [5,3,2,1], prefix [0,5,8,10,11].
    // 8->2, 6->2, 11->4? Actually 11 is exactly sum of all, so 4. Wait prefix[4]=11, so 4.
    // 0->0, 12->-1.

    // All equal values
    std::vector<long long> arr2 = {2, 2, 2};
    std::vector<long long> q2 = {2, 4, 6, 7};
    std::vector<long long> res2 = minElementsForTargets(arr2, q2);
    assert(res2 == (std::vector<long long>{1, 2, 3, -1}));

    // Single element
    std::vector<long long> arr3 = {10};
    std::vector<long long> q3 = {10, 9, 11};
    std::vector<long long> res3 = minElementsForTargets(arr3, q3);
    assert(res3 == (std::vector<long long>{1, 1, -1}));

    // Empty array (edge case, though constraints say positive n, test anyway)
    std::vector<long long> arr4 = {};
    std::vector<long long> q4 = {0, 1};
    std::vector<long long> res4 = minElementsForTargets(arr4, q4);
    assert(res4 == (std::vector<long long>{0, -1}));

    // Large numbers
    std::vector<long long> arr5 = {1000000000000000000LL, 1};
    std::vector<long long> q5 = {1000000000000000000LL, 1000000000000000001LL, 2};
    std::vector<long long> res5 = minElementsForTargets(arr5, q5);
    assert(res5 == (std::vector<long long>{1, 2, 2}));

    // Duplicate targets and values
    std::vector<long long> arr6 = {4, 4, 4, 4};
    std::vector<long long> q6 = {8, 8, 16, 17};
    std::vector<long long> res6 = minElementsForTargets(arr6, q6);
    assert(res6 == (std::vector<long long>{2, 2, 4, -1}));

    return 0;
}
// The key observation is that to minimize the number of elements selected to reach a given sum, we should always choose the largest elements first, because larger elements contribute more to the sum per item. Thus, sort the array in descending order and compute prefix sums: `pref[i]` = sum of the largest `i` elements (1-indexed). For a target `a`, find the smallest index `i` such that `pref[i] >= a`. This is a lower_bound search on the prefix sum array. If no such index exists (i.e., total sum < a), answer is `-1`; otherwise answer is `i`. Since the prefix sums are non-decreasing (all values are positive), binary search works. Edge cases: duplicate values do not affect the logic; `a` could be `0` (answer is `0`), and the total sum might exactly equal a target. Time complexity: sorting takes `O(n log n)`, each query is `O(log n)` via `std::lower_bound`. Space complexity: `O(n)` for the prefix sum array.
