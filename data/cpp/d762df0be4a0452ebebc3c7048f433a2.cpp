// Write a C++ function that takes a reference to a sorted vector of integers and removes all duplicate elements in-place, so that each element appears only once. The function should return the new length of the vector after removing duplicates. The relative order of the remaining elements must be preserved, and the function must not use extra space for another array; it must operate only on the input vector using constant extra memory (O(1) additional space). The input vector is guaranteed to be sorted in non-decreasing order, but it may be empty.

#include <cassert>
#include <vector>

int main() {
    {
        std::vector<int> nums = {1, 1, 2};
        int len = removeDuplicatesFromSorted(nums);
        assert(len == 2);
        assert(nums[0] == 1 && nums[1] == 2);
    }
    {
        std::vector<int> nums = {0, 0, 1, 1, 2, 2, 3, 3, 4};
        int len = removeDuplicatesFromSorted(nums);
        assert(len == 5);
        assert(nums[0] == 0 && nums[1] == 1 && nums[2] == 2 && nums[3] == 3 && nums[4] == 4);
    }
    {
        std::vector<int> nums = {5};
        int len = removeDuplicatesFromSorted(nums);
        assert(len == 1);
        assert(nums[0] == 5);
    }
    {
        std::vector<int> nums = {};
        int len = removeDuplicatesFromSorted(nums);
        assert(len == 0);
    }
    {
        std::vector<int> nums = {7, 7, 7, 7};
        int len = removeDuplicatesFromSorted(nums);
        assert(len == 1);
        assert(nums[0] == 7);
    }
    {
        std::vector<int> nums = {-3, -3, -1, 0, 2, 2, 4};
        int len = removeDuplicatesFromSorted(nums);
        assert(len == 5);
        assert(nums[0] == -3 && nums[1] == -1 && nums[2] == 0 && nums[3] == 2 && nums[4] == 4);
    }
    return 0;
}

#include <vector>

// Removes duplicates from a sorted vector in-place.
// Returns the number of unique elements.
int removeDuplicatesFromSorted(std::vector<int>& nums) {
    if (nums.empty()) {
        return 0;
    }

    int uniqueIndex = 0;  // position of the last unique element

    for (size_t i = 1; i < nums.size(); ++i) {
        if (nums[i] != nums[uniqueIndex]) {
            ++uniqueIndex;
            nums[uniqueIndex] = nums[i];
        }
    }

    return uniqueIndex + 1;  // count of unique elements
}

// The solution uses a two-pointer in-place technique. Maintain an index `j` that points to the position where the next unique element should be placed. Start with `j = 0` because the first element (if any) is always unique. Then iterate through the vector with a read pointer `i` from index 1 onward. Whenever the value at `i` differs from the value at `j`, it means we've encountered a new unique element; increment `j` and copy `nums[i]` to `nums[j]`. If the vector is empty, return 0 immediately. After the loop, the number of unique elements is `j + 1`, which is returned. Edge cases: empty vector returns 0; vector with one element returns 1; vectors with all identical elements return 1. Time complexity is O(n) for n elements because we traverse the vector once. Space complexity is O(1) because we only use a few integer variables and modify the vector in-place.
