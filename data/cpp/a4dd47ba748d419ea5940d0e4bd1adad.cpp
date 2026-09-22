/*
Write a C++ function named `removeAdjacentDuplicates` that takes a `std::vector<int>&` (non-const, since it must modify the vector in place) and returns an `int` representing the number of unique elements after removing consecutive duplicate values. The function must preserve the relative order of the first occurrence of each distinct value, and it may leave the trailing elements beyond the new logical size in an unspecified state. The input vector can be empty, contain all identical values, or contain values in any order (including non-sorted). The function must work without using extra containers like `std::set` or copies of the vector, and it should only require `#include <vector>`.
*/

#include <vector>

// Removes consecutive duplicates in-place.
// Returns the new logical size of the vector.
int removeAdjacentDuplicates(std::vector<int>& nums) {
    if (nums.empty()) {
        return 0;
    }

    int writeIndex = 1;

    for (int readIndex = 1; readIndex < static_cast<int>(nums.size()); ++readIndex) {
        if (nums[readIndex] != nums[readIndex - 1]) {
            nums[writeIndex] = nums[readIndex];
            ++writeIndex;
        }
    }

    return writeIndex;
}

#include <cassert>
#include <vector>

int main() {
    {
        std::vector<int> nums = {1, 1, 2, 2, 3, 3, 3, 4};
        int newSize = removeAdjacentDuplicates(nums);
        assert(newSize == 4);
        assert(nums[0] == 1 && nums[1] == 2 && nums[2] == 3 && nums[3] == 4);
    }
    {
        std::vector<int> nums = {};
        assert(removeAdjacentDuplicates(nums) == 0);
    }
    {
        std::vector<int> nums = {5};
        assert(removeAdjacentDuplicates(nums) == 1);
        assert(nums[0] == 5);
    }
    {
        std::vector<int> nums = {7, 7, 7, 7};
        int newSize = removeAdjacentDuplicates(nums);
        assert(newSize == 1);
        assert(nums[0] == 7);
    }
    {
        std::vector<int> nums = {1, 2, 3, 4, 5};
        int newSize = removeAdjacentDuplicates(nums);
        assert(newSize == 5);
        for (int i = 0; i < newSize; ++i) {
            assert(nums[i] == i + 1);
        }
    }
    {
        std::vector<int> nums = {2, 2, 1, 1, 3, 3, 2, 2};
        int newSize = removeAdjacentDuplicates(nums);
        assert(newSize == 3);
        assert(nums[0] == 2 && nums[1] == 1 && nums[2] == 3);
    }
    {
        std::vector<int> nums = {0, 0, 0, 1, 1, 2, 2, 2, 3};
        int newSize = removeAdjacentDuplicates(nums);
        assert(newSize == 4);
        assert(nums[0] == 0 && nums[1] == 1 && nums[2] == 2 && nums[3] == 3);
    }
    return 0;
}

// The solution uses the **two-pointer technique** with a single pass over the array. We maintain a `writeIndex` (starting at 1 for non-empty arrays) that points to where the next unique element should be placed, and a `readIndex` (starting at 1) that scans forward. At each step, we compare `nums[readIndex]` with `nums[readIndex-1]`. If they are different, it means we have encountered a new unique value; we copy it to `nums[writeIndex]` and increment `writeIndex`. This works because after the first element, every time a value changes from its immediate predecessor, that value is distinct from everything before it. For an empty vector, we return 0. For a single element, we return 1 and do not enter the loop. The algorithm runs in **O(n)** time and **O(1)** auxiliary space. Edge cases include: empty input (return 0), all identical elements (writeIndex stays 1, so only the first element is kept), and already unique arrays (every step copies the same element to itself or to the next position). The trailing elements after writeIndex are ignored because the function returns the new logical size.
