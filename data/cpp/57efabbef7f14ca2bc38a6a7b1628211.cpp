// Write a C++ function named `findDuplicatesInPlace` that takes a reference to a `std::vector<int>` where every integer appears either once or twice, all integers are in the range `[1, n]` (where `n = nums.size()`), and the vector is mutable. The function must return a `std::vector<int>` containing all integers that appear exactly twice in the input, in any order. The solution must run in \(O(n)\) time and use \(O(1)\) extra space (excluding the output vector). Do not use sorting, hash maps, or sets. The input vector may be modified during the process.
#include <cassert>
#include <vector>
#include <algorithm>

int main() {
    // Case 1: standard example
    {
        std::vector<int> nums = {4,3,2,7,8,2,3,1};
        std::vector<int> expect = {2,3};
        std::vector<int> ans = findDuplicatesInPlace(nums);
        std::sort(ans.begin(), ans.end());
        std::sort(expect.begin(), expect.end());
        assert(ans == expect);
    }

    // Case 2: single duplicate
    {
        std::vector<int> nums = {2,1,1};
        std::vector<int> ans = findDuplicatesInPlace(nums);
        std::sort(ans.begin(), ans.end());
        std::vector<int> expect = {1};
        assert(ans == expect);
    }

    // Case 3: no duplicates
    {
        std::vector<int> nums = {1,2,3};
        std::vector<int> ans = findDuplicatesInPlace(nums);
        std::vector<int> expect = {};
        assert(ans == expect);
    }

    // Case 4: all duplicates
    {
        std::vector<int> nums = {1,1,2,2,3,3};
        std::vector<int> ans = findDuplicatesInPlace(nums);
        std::sort(ans.begin(), ans.end());
        std::vector<int> expect = {1,2,3};
        assert(ans == expect);
    }

    // Case 5: single element
    {
        std::vector<int> nums = {1};
        std::vector<int> ans = findDuplicatesInPlace(nums);
        std::vector<int> expect = {};
        assert(ans == expect);
    }

    // Case 6: maximum value at the end
    {
        std::vector<int> nums = {2,2,1,1,3,3,4,4};
        std::vector<int> ans = findDuplicatesInPlace(nums);
        std::sort(ans.begin(), ans.end());
        std::vector<int> expect = {1,2,3,4};
        assert(ans == expect);
    }

    // Case 7: unsorted duplicates
    {
        std::vector<int> nums = {5,5,2,2,1,1};
        std::vector<int> ans = findDuplicatesInPlace(nums);
        std::sort(ans.begin(), ans.end());
        std::vector<int> expect = {1,2,5};
        assert(ans == expect);
    }

    return 0;
}
#include <vector>
#include <cstdlib> // for std::abs

// Given a vector nums where each integer appears once or twice, all in [1, n],
// return a vector of all integers that appear exactly twice.
// Uses the input array as an in-place hash map, O(n) time, O(1) extra space.
std::vector<int> findDuplicatesInPlace(std::vector<int>& nums) {
    std::vector<int> result;
    const int n = static_cast<int>(nums.size());

    for (int i = 0; i < n; ++i) {
        int index = std::abs(nums[i]) - 1;  // value is in [1, n], so index is valid
        if (nums[index] < 0) {
            // Already marked, so this value has been seen before.
            result.push_back(std::abs(nums[i]));
        }
        // Mark as visited by negating.
        nums[index] *= -1;
    }

    return result;
}
// The key insight is to use the array itself as a hash table by exploiting the fact that every value is between `1` and `n`. For each element, we compute an index `j = abs(nums[i]) - 1` (since values can become negative during processing, we take the absolute value). If `nums[j]` is already negative, it means we have seen the value `abs(nums[i])` before, so it is a duplicate and we append it to the result. Otherwise, we mark it as visited by negating `nums[j]`. This works because the original positive value can be recovered via `abs`. Important edge cases: duplicate values appear exactly twice, so each duplicate is detected exactly once; the input values are always in range so `j` is always a valid index; the order of results does not matter as long as the set of duplicates is correct. Time complexity is \(O(n)\) because we iterate through the array once with constant-time operations. Space complexity is \(O(1)\) auxiliary (ignoring the output vector), since we only use a result vector and a few variables.
