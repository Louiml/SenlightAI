// Write a C++ function `findPairWithSum` that takes a non-empty vector of distinct integers and a target integer, and returns a `std::pair<int, int>` containing the indices (in any order) of the two numbers that add up to the target. If no such pair exists, return `{-1, -1}`. The function must handle vectors with negative numbers, positive numbers, and a target that may be negative, zero, or positive. The vector is guaranteed to have at least two elements, but may not contain a valid pair, and each number appears at most once.

The most efficient approach is a single-pass hash map. Iterate through the vector, and for each element `nums[i]`, compute the complement `target - nums[i]`. If that complement is already in the hash map (with its index), we have found the pair and return the stored index and `i`. Otherwise, insert the current element and its index into the map. This works for any integer values because the complement check is exact arithmetic. Edge cases include: the target being zero when two opposite numbers exist (e.g., 3 and -3), negative targets, and the case where no pair exists (return `{-1, -1}`). The algorithm runs in O(n) time and O(n) auxiliary space for the hash map. Since all numbers are distinct, there is no ambiguity about duplicate keys.

#include <vector>
#include <unordered_map>
#include <utility>

std::pair<int, int> findPairWithSum(const std::vector<int>& nums, int target) {
    std::unordered_map<int, int> seen; // value -> index
    for (int i = 0; i < static_cast<int>(nums.size()); ++i) {
        int complement = target - nums[i];
        auto it = seen.find(complement);
        if (it != seen.end()) {
            return {it->second, i};
        }
        seen.emplace(nums[i], i);
    }
    return {-1, -1};
}

#include <cassert>
#include <vector>
#include <utility>

int main() {
    // Basic positive case
    std::vector<int> v1 = {2, 7, 11, 15};
    auto r1 = findPairWithSum(v1, 13);
    assert((r1 == std::pair<int,int>{0, 1}) || (r1 == std::pair<int,int>{1, 0}));

    // Negative numbers and target
    std::vector<int> v2 = {-3, 4, 5, -1};
    auto r2 = findPairWithSum(v2, 1);
    assert((r2 == std::pair<int,int>{0, 1}) || (r2 == std::pair<int,int>{1, 0}));

    // Target zero with opposite numbers
    std::vector<int> v3 = {10, -10, 3, 8};
    auto r3 = findPairWithSum(v3, 0);
    assert((r3 == std::pair<int,int>{0, 1}) || (r3 == std::pair<int,int>{1, 0}));

    // No pair exists
    std::vector<int> v4 = {1, 2, 3, 4};
    auto r4 = findPairWithSum(v4, 20);
    assert(r4 == std::pair<int,int>{-1, -1});

    // Duplicate values not allowed, but distinct values with same sum
    std::vector<int> v5 = {5, 6, 7, 8};
    auto r5 = findPairWithSum(v5, 13);
    assert((r5 == std::pair<int,int>{0, 2}) || (r5 == std::pair<int,int>{2, 0}) ||
           (r5 == std::pair<int,int>{1, 3}) || (r5 == std::pair<int,int>{3, 1}));

    // Large negative target
    std::vector<int> v6 = {-100, 50, 2, -50};
    auto r6 = findPairWithSum(v6, -150);
    assert((r6 == std::pair<int,int>{0, 2}) || (r6 == std::pair<int,int>{2, 0}));

    // Only two elements that don't match
    std::vector<int> v7 = {5, 10};
    auto r7 = findPairWithSum(v7, 14);
    assert(r7 == std::pair<int,int>{-1, -1});

    // Only two elements that match
    std::vector<int> v8 = {5, 10};
    auto r8 = findPairWithSum(v8, 5);
    assert(r8 == std::pair<int,int>{1, 0} || r8 == std::pair<int,int>{0, 1});

    return 0;
}
