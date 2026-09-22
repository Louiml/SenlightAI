// Write a C++ function named `findTwoSumIndices` that takes a constant reference to a vector of integers and a target integer, and returns a vector containing the two indices of the numbers that add up to the target. Exactly one valid solution exists, each element can be used at most once, and indices can be returned in any order. The function must run in better than O(n²) time, so implement it using an unordered map to store previously seen numbers and their indices. The vector length is between 2 and 10,000, and values and target fit in a 32-bit signed integer range.
// The optimal approach uses a single pass with a hash map. For each element at index `i`, compute the complement `target - nums[i]`. If the complement already exists in the map, we’ve found the pair and return `{map[complement], i}`. Otherwise, store the current number with its index in the map. This works because the problem guarantees exactly one solution, and we never reuse the same element (since the complement is always a previously seen value from a different index). Edge cases include negative numbers, duplicates, and large magnitudes; the map handles all naturally. Time complexity is O(n) average, space complexity O(n) for the map.
#include <vector>
#include <unordered_map>

// Return the two indices whose values sum to target.
// Assumes exactly one solution exists and each element is used at most once.
std::vector<int> findTwoSumIndices(const std::vector<int>& nums, int target) {
    std::unordered_map<int, int> seen; // value -> index
    for (int i = 0; i < static_cast<int>(nums.size()); ++i) {
        int complement = target - nums[i];
        if (seen.find(complement) != seen.end()) {
            return {seen[complement], i};
        }
        seen[nums[i]] = i;
    }
    return {}; // Should never reach here per constraints
}
#include <cassert>
#include <vector>

int main() {
    assert(findTwoSumIndices({2, 7, 11, 15}, 9) == std::vector<int>({0, 1}));
    assert(findTwoSumIndices({3, 2, 4}, 6) == std::vector<int>({1, 2}));
    assert(findTwoSumIndices({3, 3}, 6) == std::vector<int>({0, 1}));
    assert(findTwoSumIndices({-1, -2, -3, -4}, -7) == std::vector<int>({2, 3}));
    assert(findTwoSumIndices({0, 4, 3, 0}, 0) == std::vector<int>({0, 3}));
    assert(findTwoSumIndices({1, 5, 9, 2}, 10) == std::vector<int>({1, 2}));
    assert(findTwoSumIndices({10, 20, 30}, 50) == std::vector<int>({1, 2}));
    assert(findTwoSumIndices({-1000000000, 1000000000}, 0) == std::vector<int>({0, 1}));
    assert(findTwoSumIndices({1, 2, 3, 4, 5}, 7) == std::vector<int>({1, 4}));
    assert(findTwoSumIndices({-5, 5, 10}, 0) == std::vector<int>({0, 1}));
    return 0;
}
