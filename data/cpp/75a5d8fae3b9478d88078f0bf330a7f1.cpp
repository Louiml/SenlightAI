// Write a C++ function `findPairWithSum` that takes a vector of integers `nums` (1-indexed positions are used for output) and a target sum `x`, and returns a pair of 1-based indices (in any order) of two distinct elements from the vector whose values add up to exactly `x`. If no such pair exists, return a pair `{-1, -1}`. The function must handle duplicate values correctly; you may assume the input has at least two elements and that at most one valid pair exists. The solution must not modify the original vector (do not sort the input in place; instead, work on a sorted copy of values paired with their original indices). The indices in the returned pair must be the original 1-based positions from the input vector.
The classic two-pointer technique on a sorted array is used, but since we need the original indices, we first create a vector of pairs `(value, original_index)` and sort it by the value. If multiple values are equal, the sort order does not matter because we only need two distinct positions. We then place a left pointer at the beginning and a right pointer at the end of this sorted vector. At each step, we compute the sum `left_value + right_value`. If it is less than `x`, we move the left pointer right to increase the sum; if it is greater, we move the right pointer left to decrease the sum. If it equals `x`, we return the original indices (1-based) from the pair. If the pointers cross without finding a sum, we return `{-1, -1}`. The algorithm correctly handles duplicates because the sorted copy retains both positions, and the two-pointer logic ensures we try all possible distinct pairs in sorted order. An important edge case is when the two equal values sum to `x` (e.g., `nums = [3, 3]`, `x = 6`); the two-pointer approach will find them because both pointers will eventually land on the same value but different indices. Time complexity is \(O(n \log n)\) for sorting, and the pointer scan is \(O(n)\), so overall \(O(n \log n)\). Space complexity is \(O(n)\) for the copy of pairs.
#include <vector>
#include <utility>
#include <algorithm>

// Returns a pair of 1-based indices (a, b) with a != b such that
// nums[a-1] + nums[b-1] == target. If no such pair exists, returns {-1, -1}.
std::pair<int, int> findPairWithSum(const std::vector<int>& nums, int target) {
    const int n = static_cast<int>(nums.size());
    
    // Create a vector of (value, original_index) pairs and sort by value.
    std::vector<std::pair<int, int>> indexed;
    indexed.reserve(n);
    for (int i = 0; i < n; ++i) {
        indexed.emplace_back(nums[i], i);
    }
    std::sort(indexed.begin(), indexed.end());
    
    int left = 0;
    int right = n - 1;
    
    while (left < right) {
        const int sum = indexed[left].first + indexed[right].first;
        if (sum < target) {
            ++left;
        } else if (sum > target) {
            --right;
        } else {
            // Found a pair; return 1-based original indices (order not important).
            return {indexed[left].second + 1, indexed[right].second + 1};
        }
    }
    
    return {-1, -1};
}
#include <cassert>
#include <vector>
#include <utility>

// The solution function is assumed to be included above.

int main() {
    // Basic case
    auto res = findPairWithSum(std::vector<int>{1, 3, 5, 7}, 8);
    assert((res.first == 1 && res.second == 3) || (res.first == 3 && res.second == 1));
    
    // Pair with duplicate values
    res = findPairWithSum(std::vector<int>{3, 3}, 6);
    assert((res.first == 1 && res.second == 2) || (res.first == 2 && res.second == 1));
    
    // Negative and positive numbers
    res = findPairWithSum(std::vector<int>{-2, 4, -1, 3}, 3);
    assert((res.first == 2 && res.second == 3) || (res.first == 3 && res.second == 2));
    
    // No valid pair
    res = findPairWithSum(std::vector<int>{1, 2, 3}, 10);
    assert(res.first == -1 && res.second == -1);
    
    // Large target with distinct elements at ends
    res = findPairWithSum(std::vector<int>{10, 20, 30, 40, 50}, 60);
    assert((res.first == 1 && res.second == 5) || (res.first == 5 && res.second == 1));
    
    // Pair where target is zero
    res = findPairWithSum(std::vector<int>{-5, 5, 7}, 0);
    assert((res.first == 1 && res.second == 2) || (res.first == 2 && res.second == 1));
    
    // Duplicate values that do not sum to target
    res = findPairWithSum(std::vector<int>{2, 2, 2}, 4);
    assert((res.first == 1 && res.second == 2) || (res.first == 2 && res.second == 1) ||
           (res.first == 1 && res.second == 3) || (res.first == 3 && res.second == 1) ||
           (res.first == 2 && res.second == 3) || (res.first == 3 && res.second == 2));
    
    // Smallest input size
    res = findPairWithSum(std::vector<int>{4, 4}, 8);
    assert((res.first == 1 && res.second == 2) || (res.first == 2 && res.second == 1));
    
    return 0;
}
