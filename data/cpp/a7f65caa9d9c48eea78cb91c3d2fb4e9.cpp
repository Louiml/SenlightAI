/*
Write a C++ function that takes a vector of integers `nums` and a vector of queries, where each query is a pair `[l, r]` (0-indexed, inclusive on both ends). A subarray `nums[l..r]` is called "special" if every pair of adjacent elements in that subarray have different parities (one even, one odd). For each query, return `true` if the corresponding subarray is special, otherwise `false`. Optimize for many queries on the same `nums` array.
*/
#include <vector>
#include <set>
#include <cstddef>

// Returns a vector of booleans, one per query, indicating whether each subarray is special.
std::vector<bool> isArraySpecial(const std::vector<int>& nums, const std::vector<std::vector<int>>& queries) {
    std::set<int> badIndices;
    for (std::size_t i = 1; i < nums.size(); ++i) {
        if ((nums[i] % 2) == (nums[i - 1] % 2)) {
            badIndices.insert(static_cast<int>(i));
        }
    }

    std::vector<bool> result;
    result.reserve(queries.size());
    for (const auto& query : queries) {
        int left = query[0];
        int right = query[1];
        auto it = badIndices.upper_bound(left);
        bool special = (it == badIndices.end()) || (*it > right);
        result.push_back(special);
    }
    return result;
}
#include <cassert>
#include <vector>

int main() {
    // Basic case: alternating parities, all subarrays special
    std::vector<int> nums1 = {1, 2, 3, 4};
    std::vector<std::vector<int>> q1 = {{0, 3}, {0, 1}, {1, 2}, {2, 3}, {1, 3}};
    std::vector<bool> r1 = isArraySpecial(nums1, q1);
    assert(r1 == std::vector<bool>({true, true, true, true, true}));

    // Case with a bad pair at index 2 (nums[1]=2, nums[2]=4 both even)
    std::vector<int> nums2 = {1, 2, 4, 5};
    std::vector<std::vector<int>> q2 = {{0, 2}, {0, 3}, {1, 2}, {2, 3}, {0, 1}};
    std::vector<bool> r2 = isArraySpecial(nums2, q2);
    assert(r2 == std::vector<bool>({false, false, false, true, true}));

    // Single element queries are always special
    std::vector<int> nums3 = {2, 4, 6};
    std::vector<std::vector<int>> q3 = {{0, 0}, {1, 1}, {2, 2}, {0, 2}};
    std::vector<bool> r3 = isArraySpecial(nums3, q3);
    assert(r3 == std::vector<bool>({true, true, true, false}));

    // Query range that starts right after a bad index but before the next
    std::vector<int> nums4 = {1, 3, 2, 2};
    // Bad indices: 1 (1 and 3 same odd), 3 (2 and 2 same even)
    std::vector<std::vector<int>> q4 = {{1, 3}, {0, 0}, {0, 1}, {2, 3}, {0, 2}};
    std::vector<bool> r4 = isArraySpecial(nums4, q4);
    assert(r4 == std::vector<bool>({false, true, false, false, false}));

    // Empty nums and empty queries
    std::vector<int> nums5 = {};
    std::vector<std::vector<int>> q5 = {};
    std::vector<bool> r5 = isArraySpecial(nums5, q5);
    assert(r5.empty());

    // Large single query with one bad index at the start
    std::vector<int> nums6 = {2, 2, 3, 4};
    std::vector<std::vector<int>> q6 = {{0, 3}, {1, 3}, {0, 1}};
    std::vector<bool> r6 = isArraySpecial(nums6, q6);
    assert(r6 == std::vector<bool>({false, true, false}));

    return 0;
}
// The naive approach checks every adjacent pair inside each query's range, leading to O(n * q) time worst-case, which is too slow for large inputs. Instead, precompute the positions where adjacent elements have the same parity—these are "bad" indices (specifically, the right index `i` of a bad pair `(i-1, i)`). A subarray `[l, r]` is special if and only if there is no bad index in the range `(l, r]` (since `l` itself is not compared with anything on its left). Store bad indices in a sorted container like `std::set`. For each query, use `upper_bound(l)` to find the first bad index greater than `l`. If that bad index exists and is ≤ `r`, then the subarray is not special; otherwise it is special. Edge cases: `l == r` (single element) is always special, even if `l` is bad relative to `l-1`—our condition handles this because `upper_bound(l)` will skip a bad index at exactly `l`. Also, empty `nums` or empty queries should return an empty result. Time complexity: preprocessing O(n log n) for set insertions, each query O(log n) for `upper_bound`. Space: O(n) for the set and O(q) for the result.
