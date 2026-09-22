Write a C++ function that takes a vector of integers by reference and rearranges its elements so that all non-zero values appear at the beginning (in their original relative order) and all zeros are moved to the end. The function must modify the input vector in-place, return void, and preserve the relative order of all non-zero elements. The input vector may be empty, contain only zeros, only non-zeros, or a mix, including negative numbers and duplicates. The function should not allocate any significant extra space (only a constant amount of auxiliary memory is allowed).
// The optimal solution uses a two-pointer (or two-index) technique. Maintain two indices: `nonZeroIndex` (the position where the next non-zero element should be placed) and `currentIndex` (iterating through the array). Initially both are 0. Iterate through the array with `currentIndex`. When `nums[currentIndex]` is non-zero, swap it with `nums[nonZeroIndex]`, then increment both indices. When `nums[currentIndex]` is zero, just increment `currentIndex`. This way, all non-zero elements encountered so far are placed contiguously at the front in their original order, and any zeros are pushed towards the back naturally through the swaps. Edge cases: an empty vector or a vector with no non-zero elements (all zeros) require no swaps; a vector with no zeros (all non-zero) simply results in every element being swapped with itself (harmless). The relative order of non-zeros is preserved because each non-zero is swapped to the next available front position without skipping over any earlier non-zero. Time complexity is O(n) with a single pass; space complexity is O(1) auxiliary.
#include <vector>
#include <utility>

// Rearrange the vector in-place so all non-zero elements appear first,
// preserving their original relative order, and all zeros are moved to the end.
void moveZeroesToEnd(std::vector<int>& nums) {
    int nonZeroIndex = 0;
    int currentIndex = 0;
    const int size = static_cast<int>(nums.size());

    while (currentIndex < size) {
        if (nums[currentIndex] != 0) {
            std::swap(nums[currentIndex], nums[nonZeroIndex]);
            ++nonZeroIndex;
        }
        ++currentIndex;
    }
}
#include <cassert>
#include <vector>

int main() {
    {
        std::vector<int> nums = {0, 1, 0, 3, 12};
        moveZeroesToEnd(nums);
        assert(nums == std::vector<int>({1, 3, 12, 0, 0}));
    }
    {
        std::vector<int> nums = {0};
        moveZeroesToEnd(nums);
        assert(nums == std::vector<int>({0}));
    }
    {
        std::vector<int> nums = {1, 2, 3};
        moveZeroesToEnd(nums);
        assert(nums == std::vector<int>({1, 2, 3}));
    }
    {
        std::vector<int> nums = {0, 0, 0};
        moveZeroesToEnd(nums);
        assert(nums == std::vector<int>({0, 0, 0}));
    }
    {
        std::vector<int> nums = {-1, 0, 5, -2, 0, 7};
        moveZeroesToEnd(nums);
        assert(nums == std::vector<int>({-1, 5, -2, 7, 0, 0}));
    }
    {
        std::vector<int> nums = {};
        moveZeroesToEnd(nums);
        assert(nums.empty());
    }
    {
        std::vector<int> nums = {0, 0, 1, 0, 2, 0, 3, 0};
        moveZeroesToEnd(nums);
        assert(nums == std::vector<int>({1, 2, 3, 0, 0, 0, 0, 0}));
    }
    {
        std::vector<int> nums = {5, 0, 0, 5, 0, 5};
        moveZeroesToEnd(nums);
        assert(nums == std::vector<int>({5, 5, 5, 0, 0, 0}));
    }
    return 0;
}
