Write a C++ function named `compactVector` that takes a `std::vector<int>` and an integer `value` as parameters. The function should remove all occurrences of `value` from the vector in-place, preserving the relative order of the remaining elements, and return the new length of the vector after removal. The function must operate directly on the input vector, not on a copy, and should not rely on external libraries beyond the standard library. The relative order of the remaining elements must be maintained, and the function should handle edge cases such as an empty vector, a vector with no occurrences of `value`, or a vector where all elements equal `value`. The function should return the count of elements that remain, and the first `count` positions of the vector must contain the remaining elements in their original order; the contents of positions beyond `count` are irrelevant.

// The task is a classic in-place array compaction problem. The main algorithm uses a two-pointer technique: one pointer (`readIndex`) iterates through the entire vector, and another pointer (`writeIndex`) tracks where the next valid element should be placed. For each element at `readIndex`, if it does not equal the target `value`, we copy it to position `writeIndex` and increment `writeIndex`. If it does equal `value`, we simply skip it (advance `readIndex` without writing). This ensures that all elements not equal to `value` are shifted to the front in their original relative order, and `writeIndex` ends up as the new logical size of the vector. 
//
// Edge cases to consider: an empty vector (loop never runs, returns 0), a vector with no occurrences of `value` (every element is copied to its own position, `writeIndex` equals original size), and a vector where all elements equal `value` (nothing is written, `writeIndex` remains 0). The algorithm runs in O(n) time because we traverse the vector once, and uses O(1) extra space because we modify the vector in-place without allocating additional data structures.

#include <vector>

// Removes all occurrences of 'value' from 'nums' in-place,
// preserves relative order of remaining elements,
// and returns the new length of the vector.
int compactVector(std::vector<int>& nums, int value) {
    int writeIndex = 0;
    for (int readIndex = 0; readIndex < static_cast<int>(nums.size()); ++readIndex) {
        if (nums[readIndex] != value) {
            nums[writeIndex] = nums[readIndex];
            ++writeIndex;
        }
    }
    return writeIndex;
}

#include <cassert>
#include <vector>

int main() {
    // Basic case: remove a value that appears multiple times.
    {
        std::vector<int> nums = {3, 2, 2, 3};
        int newLen = compactVector(nums, 3);
        assert(newLen == 2);
        assert(nums[0] == 2 && nums[1] == 2);
    }

    // Empty vector.
    {
        std::vector<int> nums;
        int newLen = compactVector(nums, 5);
        assert(newLen == 0);
    }

    // No occurrences of value.
    {
        std::vector<int> nums = {1, 2, 3, 4};
        int newLen = compactVector(nums, 9);
        assert(newLen == 4);
        assert(nums[0] == 1 && nums[1] == 2 && nums[2] == 3 && nums[3] == 4);
    }

    // All elements equal value.
    {
        std::vector<int> nums = {7, 7, 7};
        int newLen = compactVector(nums, 7);
        assert(newLen == 0);
    }

    // Single element matching.
    {
        std::vector<int> nums = {5};
        int newLen = compactVector(nums, 5);
        assert(newLen == 0);
    }

    // Single element not matching.
    {
        std::vector<int> nums = {1};
        int newLen = compactVector(nums, 2);
        assert(newLen == 1);
        assert(nums[0] == 1);
    }

    // Value appears at beginning, middle, end, and consecutively.
    {
        std::vector<int> nums = {0, 1, 0, 2, 0, 3, 0};
        int newLen = compactVector(nums, 0);
        assert(newLen == 3);
        assert(nums[0] == 1 && nums[1] == 2 && nums[2] == 3);
    }

    // Large vector with mixed values.
    {
        std::vector<int> nums = {4, 5, 4, 6, 4, 7, 4};
        int newLen = compactVector(nums, 4);
        assert(newLen == 3);
        assert(nums[0] == 5 && nums[1] == 6 && nums[2] == 7);
    }
}
