// Write a C++ function that takes a vector of integers by reference and moves all non-zero elements to the front of the vector while preserving their original relative order, and fills the remaining positions at the end with zeros. The function should modify the vector in-place and perform this operation in linear time using constant extra space. For example, input `[0,1,0,3,12]` should become `[1,3,12,0,0]`. The vector may contain any number of zeros including all zeros or no zeros, and may contain negative numbers. The function must handle an empty vector gracefully.
The solution uses a two-pointer technique with a single pass to compact non-zero elements, followed by a second pass to zero out the tail. The main idea is to maintain a write index `j` that tracks where the next non-zero element should be placed. We iterate through the array with a read index `i`. Whenever `nums[i]` is non-zero, we copy it to `nums[j]` and increment `j`. This preserves the relative order of non-zero elements because we always write them in the same order they appear. After the first pass, all non-zero elements are at the front (indices `0` to `j-1`), and the remaining positions from `j` to the end need to be set to zero. The second loop does exactly that. Edge cases: if the vector is empty, the loops simply do nothing. If there are no zeros, the write index `j` ends up equal to `nums.size()`, and the second loop runs zero iterations. If all elements are zero, the first loop never writes, `j` stays at 0, and the second loop zeros the entire vector (which it already is). Time complexity is O(n) because we traverse the array twice (two passes over at most n elements each). Space complexity is O(1) since we only use a single integer index and modify the vector in-place.
#include <vector>

// Move all non-zero elements to the front of the vector, preserving order, and fill the rest with zeros.
void moveNonZeroesToFront(std::vector<int>& nums) {
    int writeIndex = 0; // Position where the next non-zero element should be placed.

    // First pass: move all non-zero elements to the front.
    for (int readIndex = 0; readIndex < static_cast<int>(nums.size()); ++readIndex) {
        if (nums[readIndex] != 0) {
            nums[writeIndex] = nums[readIndex];
            ++writeIndex;
        }
    }

    // Second pass: fill the remaining positions with zeros.
    for (int i = writeIndex; i < static_cast<int>(nums.size()); ++i) {
        nums[i] = 0;
    }
}
#include <cassert>
#include <vector>

// The solution function is declared above; include it in the same translation unit.
int main() {
    std::vector<int> v1 = {0, 1, 0, 3, 12};
    moveNonZeroesToFront(v1);
    assert(v1 == std::vector<int>({1, 3, 12, 0, 0}));

    std::vector<int> v2 = {0, 0, 0};
    moveNonZeroesToFront(v2);
    assert(v2 == std::vector<int>({0, 0, 0}));

    std::vector<int> v3 = {5, 6, 7};
    moveNonZeroesToFront(v3);
    assert(v3 == std::vector<int>({5, 6, 7}));

    std::vector<int> v4 = {0, -1, 0, -2, 0};
    moveNonZeroesToFront(v4);
    assert(v4 == std::vector<int>({-1, -2, 0, 0, 0}));

    std::vector<int> v5 = {1, 0, 0, 0, 0, 2};
    moveNonZeroesToFront(v5);
    assert(v5 == std::vector<int>({1, 2, 0, 0, 0, 0}));

    std::vector<int> v6 = {0};
    moveNonZeroesToFront(v6);
    assert(v6 == std::vector<int>({0}));

    std::vector<int> v7 = {42};
    moveNonZeroesToFront(v7);
    assert(v7 == std::vector<int>({42}));

    std::vector<int> v8 = {};
    moveNonZeroesToFront(v8);
    assert(v8.empty());
}
