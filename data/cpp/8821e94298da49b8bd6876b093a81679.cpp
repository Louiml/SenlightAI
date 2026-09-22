Write a C++ function `findPairWithSum` that takes a `std::vector<int>` and a target integer, and returns a `std::vector<int>` containing the 0‑based indices of the two distinct elements whose sum equals the target. If no such pair exists, return `{-1, -1}`. The input vector may contain duplicates, negative numbers, and is guaranteed to be non‑empty. The function must be efficient even for large inputs, and there is exactly one valid solution when one exists (i.e., the pair is unique). Do not use the same element twice, and do not modify the input vector.
#include <cassert>
#include <vector>

int main() {
    // Basic case with negative numbers
    std::vector<int> v1 = {2, 7, 11, 15};
    assert(findPairWithSum(v1, 9) == std::vector<int>({0, 1}));

    // Duplicate values, pair uses two different indices
    std::vector<int> v2 = {3, 3, 4, 5};
    assert(findPairWithSum(v2, 6) == std::vector<int>({0, 1}));

    // Negative numbers
    std::vector<int> v3 = {-1, -2, -3, -4};
    assert(findPairWithSum(v3, -6) == std::vector<int>({0, 3}));

    // No solution returns {-1, -1}
    std::vector<int> v4 = {1, 2, 3};
    assert(findPairWithSum(v4, 10) == std::vector<int>({-1, -1}));

    // Single element, no pair
    std::vector<int> v5 = {5};
    assert(findPairWithSum(v5, 5) == std::vector<int>({-1, -1}));

    // Large target, pair at the end
    std::vector<int> v6 = {10, 1, 2, 3, 7};
    assert(findPairWithSum(v6, 17) == std::vector<int>({0, 4}));

    // Multiple pairs but unique solution guaranteed, here only one
    std::vector<int> v7 = {1, 2, 3, 4, 5};
    assert(findPairWithSum(v7, 5) == std::vector<int>({0, 3}));

    // All negative numbers and zero
    std::vector<int> v8 = {-5, -3, 0, 2};
    assert(findPairWithSum(v8, -3) == std::vector<int>({1, 2}));

    // Target is zero with a pair summing to zero
    std::vector<int> v9 = {-1, 1, 2};
    assert(findPairWithSum(v9, 0) == std::vector<int>({0, 1}));

    // Duplicates with complement not equal to itself
    std::vector<int> v10 = {4, 6, 4, 6};
    assert(findPairWithSum(v10, 10) == std::vector<int>({0, 1}));

    return 0;
}
#include <vector>
#include <unordered_map>

// Find indices of two distinct elements in nums that sum to target.
// Return {i, j} with i < j, or {-1, -1} if no such pair exists.
std::vector<int> findPairWithSum(const std::vector<int>& nums, int target) {
    std::unordered_map<int, int> valueToIndex;

    for (int i = 0; i < static_cast<int>(nums.size()); ++i) {
        int complement = target - nums[i];
        auto it = valueToIndex.find(complement);
        if (it != valueToIndex.end()) {
            return {it->second, i};
        }
        valueToIndex[nums[i]] = i;
    }

    return {-1, -1};
}
// The optimal approach uses a hash map to record each element’s index as we scan the array. For every element `nums[i]`, we compute the complement `target - nums[i]`. If that complement has already been seen, we immediately return `{map[complement], i}` because the earlier occurrence has a smaller index. If not, we store the current element and its index in the map. This works for negative numbers and duplicates: for duplicates, we only store the latest index, but since the problem guarantees a unique pair, the first time we encounter a complement we have the correct earlier index. Edge cases include no solution (return `{-1,-1}`), a single element (no pair), and the same value appearing twice (allowed because indices differ). Time complexity is O(n) on average, with O(n) auxiliary space for the hash map, where n is the number of elements.
