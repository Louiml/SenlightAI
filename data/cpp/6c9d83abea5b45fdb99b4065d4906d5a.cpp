Write a C++ function named `compactZeros` that takes a reference to a `std::vector<int>` and rearranges its elements in-place so that all non-zero integers appear first (preserving their original relative order) and all zeros are moved to the end of the vector. The function must not allocate a new vector or use additional storage beyond a constant number of variables; it must modify the input vector directly. The function should handle vectors of any size, including empty vectors and vectors containing only zeros or only non-zero integers. After the function returns, the vector should have the same total length, with all zeros at the tail. The function must be declared with appropriate `const` correctness on parameters where applicable (though the vector itself is mutable). The function does not return a value; it only modifies the input.

#include <cassert>
#include <vector>

// Declaration of the tested function (from the solution).
void compactZeros(std::vector<int>& nums);

int main() {
    {
        std::vector<int> input = {0, 1, 0, 3, 12};
        compactZeros(input);
        assert(input == std::vector<int>({1, 3, 12, 0, 0}));
    }
    {
        std::vector<int> input = {1, 0, 0, 0, 0, 0, 0, 0, 0};
        compactZeros(input);
        assert(input == std::vector<int>({1, 0, 0, 0, 0, 0, 0, 0, 0}));
    }
    {
        std::vector<int> input = {0, 0, 0, 0, 0, 0, 0, 0, 1};
        compactZeros(input);
        assert(input == std::vector<int>({1, 0, 0, 0, 0, 0, 0, 0, 0}));
    }
    {
        std::vector<int> input = {0};
        compactZeros(input);
        assert(input == std::vector<int>({0}));
    }
    {
        std::vector<int> input = {5};
        compactZeros(input);
        assert(input == std::vector<int>({5}));
    }
    {
        std::vector<int> input = {};
        compactZeros(input);
        assert(input.empty());
    }
    {
        std::vector<int> input = {0, 0, 0};
        compactZeros(input);
        assert(input == std::vector<int>({0, 0, 0}));
    }
    {
        std::vector<int> input = {-1, 0, 2, 0, 0, 3};
        compactZeros(input);
        assert(input == std::vector<int>({-1, 2, 3, 0, 0, 0}));
    }
    {
        std::vector<int> input = {0, 0, 1, 0, 2};
        compactZeros(input);
        assert(input == std::vector<int>({1, 2, 0, 0, 0}));
    }
    {
        std::vector<int> input = {4, 0, 0, 0, 0};
        compactZeros(input);
        assert(input == std::vector<int>({4, 0, 0, 0, 0}));
    }
    return 0;
}

#include <vector>

// Rearrange the vector so that all non-zero elements come first (in original order)
// and all zeros are moved to the end. Performs the operation in-place with O(1) extra space.
void compactZeros(std::vector<int>& nums) {
    if (nums.empty()) return;

    std::size_t writeIndex = 0;

    // First pass: copy all non-zero elements to the front.
    for (std::size_t i = 0; i < nums.size(); ++i) {
        if (nums[i] != 0) {
            nums[writeIndex] = nums[i];
            ++writeIndex;
        }
    }

    // Second pass: fill the remaining tail positions with zeros.
    for (std::size_t i = writeIndex; i < nums.size(); ++i) {
        nums[i] = 0;
    }
}

// The classic approach is to use a two-pointer or "slow-fast" technique without erasing elements (which is O(n) per erase). We iterate through the vector with a fast pointer `i`. Maintain a `writeIndex` (or `lastNonZeroFoundAt`) that points to the position where the next non-zero element should be placed. For each element `nums[i]`, if it is non-zero, we assign `nums[writeIndex] = nums[i]`, increment `writeIndex`, and if `i != writeIndex` (to avoid self-assignment, which is harmless but wasteful), we can set `nums[i] = 0` to clear the old position. Alternatively, a simpler correct approach: first pass copies non-zero elements to the front, then second pass fills remaining positions with zeros. This is O(n) time and O(1) extra space. Edge cases: empty vector (do nothing), vector of all zeros (writeIndex stays 0, then fill with zeros), vector of all non-zeros (writeIndex equals size, second pass writes nothing). The order of non-zero elements is preserved because we copy them in sequence. Time complexity: O(n) with a single pass plus a fill pass (or two passes). Space complexity: O(1).
