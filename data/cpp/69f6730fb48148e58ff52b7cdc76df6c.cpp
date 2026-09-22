// Given a vector of integers `nums` and an integer `target`, write a C++ function that returns a vector containing the indices of the two numbers such that they add up to `target`. You may assume that each input has exactly one solution, and you may not use the same element twice. Return the indices in any order. The function must handle vectors with negative numbers and duplicates, and it should work efficiently for large inputs. Do not modify the input vector; mark it `const` in the function signature.
#include <cassert>
#include <vector>

// The solution function is expected to be defined above this test.
int main() {
    std::vector<int> nums1 = {2, 7, 11, 15};
    auto res1 = twoSum(nums1, 9);
    assert((res1 == std::vector<int>{0, 1}) || (res1 == std::vector<int>{1, 0}));

    std::vector<int> nums2 = {3, 2, 4};
    auto res2 = twoSum(nums2, 6);
    assert((res2 == std::vector<int>{1, 2}) || (res2 == std::vector<int>{2, 1}));

    std::vector<int> nums3 = {3, 3};
    auto res3 = twoSum(nums3, 6);
    assert((res3 == std::vector<int>{0, 1}) || (res3 == std::vector<int>{1, 0}));

    std::vector<int> nums4 = {-3, 4, 3, 90};
    auto res4 = twoSum(nums4, 0);
    assert((res4 == std::vector<int>{0, 2}) || (res4 == std::vector<int>{2, 0}));

    std::vector<int> nums5 = {1, 5, 8, -2, 9};
    auto res5 = twoSum(nums5, 7);
    assert((res5 == std::vector<int>{1, 3}) || (res5 == std::vector<int>{3, 1}));

    std::vector<int> nums6 = {0, 4, 3, 0};
    auto res6 = twoSum(nums6, 0);
    assert((res6 == std::vector<int>{0, 3}) || (res6 == std::vector<int>{3, 0}));

    std::vector<int> nums7 = {10, 20, 30};
    auto res7 = twoSum(nums7, 50);
    assert((res7 == std::vector<int>{1, 2}) || (res7 == std::vector<int>{2, 1}));

    std::vector<int> nums8 = {1, 1, 1, 1};
    auto res8 = twoSum(nums8, 2);
    assert((res8 == std::vector<int>{0, 1}) || (res8 == std::vector<int>{1, 0}) ||
           (res8 == std::vector<int>{0, 2}) || (res8 == std::vector<int>{2, 0}) ||
           (res8 == std::vector<int>{0, 3}) || (res8 == std::vector<int>{3, 0}) ||
           (res8 == std::vector<int>{1, 2}) || (res8 == std::vector<int>{2, 1}) ||
           (res8 == std::vector<int>{1, 3}) || (res8 == std::vector<int>{3, 1}) ||
           (res8 == std::vector<int>{2, 3}) || (res8 == std::vector<int>{3, 2}));
}
#include <vector>
#include <unordered_map>

// Return the indices of two numbers in nums that add to target.
// Assumes exactly one solution exists and indices are distinct.
std::vector<int> twoSum(const std::vector<int>& nums, int target) {
    std::unordered_map<int, int> seen; // value -> index
    for (int i = 0; i < static_cast<int>(nums.size()); ++i) {
        int complement = target - nums[i];
        auto it = seen.find(complement);
        if (it != seen.end()) {
            return {it->second, i};
        }
        seen[nums[i]] = i;
    }
    return {}; // Should never be reached given problem constraints
}
// The naive brute-force approach checks every pair `(i, j)` with `j > i` and returns the first pair whose sum equals the target. This runs in O(n²) time and O(1) extra space, which is acceptable only for small inputs. For a more efficient solution, use a hash map (unordered_map) that stores each element's value as the key and its index as the value. Iterate through the array once. For each element `nums[i]`, compute `complement = target - nums[i]`. If the complement already exists in the map, we have found the pair and return `{map[complement], i}`. Otherwise, insert `nums[i]` with index `i`. This guarantees O(n) time and O(n) space. Edge cases: the input always has exactly one valid pair, so no need to handle "no solution". Negative numbers and duplicates are naturally handled because the map stores the latest index of a value, but since each pair uses distinct indices, this does not cause issues — the first time we find a matching complement, we return immediately. If the same value appears multiple times and is needed as a pair (e.g., `[3,3]` target `6`), we insert the first 3 at index 0, then when we see the second 3, the complement 3 exists in the map, and we return `{0,1}`.
