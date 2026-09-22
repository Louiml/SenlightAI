/*
Write a C++ function that takes a vector of integers and a target value, then removes all occurrences of that target value from the vector in-place, preserving the relative order of the remaining elements. The function should return the new length of the vector after removal. The function must modify the original vector and not rely on creating a separate copy. Optimize for efficiency, and handle edge cases such as an empty vector or a vector where all elements equal the target.
*/
#include <vector>

// Removes all occurrences of 'val' from 'nums' in-place.
// Returns the new length of the vector after removal.
// Preserves the relative order of remaining elements.
int removeAllOccurrences(std::vector<int>& nums, int val) {
    int slow = 0;
    for (int fast = 0; fast < static_cast<int>(nums.size()); ++fast) {
        if (nums[fast] != val) {
            nums[slow] = nums[fast];
            ++slow;
        }
    }
    // Optional: trim the vector to the new logical size.
    nums.resize(slow);
    return slow;
}
#include <cassert>
#include <vector>

int main() {
    // Basic case: remove a value that appears multiple times.
    {
        std::vector<int> nums = {3, 2, 2, 3};
        int len = removeAllOccurrences(nums, 3);
        assert(len == 2);
        assert(nums == std::vector<int>({2, 2}));
    }

    // Remove a value that does not appear.
    {
        std::vector<int> nums = {1, 2, 3, 4};
        int len = removeAllOccurrences(nums, 5);
        assert(len == 4);
        assert(nums == std::vector<int>({1, 2, 3, 4}));
    }

    // Remove a value that appears everywhere.
    {
        std::vector<int> nums = {7, 7, 7};
        int len = removeAllOccurrences(nums, 7);
        assert(len == 0);
        assert(nums.empty());
    }

    // Empty vector.
    {
        std::vector<int> nums;
        int len = removeAllOccurrences(nums, 1);
        assert(len == 0);
        assert(nums.empty());
    }

    // Vector with one element, not equal.
    {
        std::vector<int> nums = {9};
        int len = removeAllOccurrences(nums, 2);
        assert(len == 1);
        assert(nums == std::vector<int>({9}));
    }

    // Vector with one element, equal.
    {
        std::vector<int> nums = {4};
        int len = removeAllOccurrences(nums, 4);
        assert(len == 0);
        assert(nums.empty());
    }

    // Mixed values, check order preserved.
    {
        std::vector<int> nums = {5, 1, 5, 2, 5, 3, 5};
        int len = removeAllOccurrences(nums, 5);
        assert(len == 3);
        assert(nums == std::vector<int>({1, 2, 3}));
    }

    // Values appear consecutively.
    {
        std::vector<int> nums = {1, 2, 2, 2, 3};
        int len = removeAllOccurrences(nums, 2);
        assert(len == 2);
        assert(nums == std::vector<int>({1, 3}));
    }

    // Remove negative values.
    {
        std::vector<int> nums = {-1, -2, -1, -3, -1};
        int len = removeAllOccurrences(nums, -1);
        assert(len == 2);
        assert(nums == std::vector<int>({-2, -3}));
    }

    // Larger sequence with many removals.
    {
        std::vector<int> nums = {10, 0, 0, 10, 0, 10};
        int len = removeAllOccurrences(nums, 0);
        assert(len == 3);
        assert(nums == std::vector<int>({10, 10, 10}));
    }
}
// The standard efficient approach uses a two-pointer technique: one pointer (`slow`) tracks the position where the next non-target element should be placed, and another (`fast`) scans through the vector. When the fast pointer encounters a value not equal to the target, it copies that value to the slow pointer’s position and increments slow. This effectively overwrites the target elements and compacts the non-target ones to the front. After the loop, the new logical size is `slow`, and we can optionally resize the vector to that length. This approach avoids the O(n) erase operations from the given snippet (which can be O(n²) in the worst case due to shifting). Edge cases: an empty vector yields 0; a vector with no target elements returns the original length; a vector with all target elements returns 0. Time complexity is O(n) for a single pass, and space complexity is O(1) because we modify in-place. The solution function should be `const`-correct by taking the vector by reference and not marking it const because it mutates the input.
