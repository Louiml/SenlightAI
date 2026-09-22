Write a C++ function named `compressSortedArray` that takes a reference to a `std::vector<int>` sorted in non-decreasing order and removes all duplicate elements in-place, keeping only the first occurrence of each unique value while preserving the original relative order. The function must return the number of unique elements remaining, and the first `k` elements of the vector (where `k` is the returned count) must contain the unique values in their original order. The contents of the vector beyond index `k-1` are irrelevant and may be left as-is. The input vector is guaranteed to be sorted; you must handle the case of an empty vector (return 0). The function should modify the vector directly and not rely on creating a new container for the final result. You may use additional variables but must achieve this in constant extra space (O(1) auxiliary space, excluding the input vector itself).
#include <cassert>
#include <vector>

int compressSortedArray(std::vector<int>& nums); // Declaration

int main() {
    // Test case 1: Basic duplicates
    std::vector<int> nums1 = {1, 1, 2};
    int k1 = compressSortedArray(nums1);
    assert(k1 == 2);
    assert(nums1[0] == 1 && nums1[1] == 2);

    // Test case 2: Longer example from problem statement
    std::vector<int> nums2 = {0, 0, 1, 1, 1, 2, 2, 3, 3, 4};
    int k2 = compressSortedArray(nums2);
    assert(k2 == 5);
    for (int i = 0; i < k2; ++i) {
        assert(nums2[i] == i); // Expected: 0,1,2,3,4
    }

    // Test case 3: Empty vector
    std::vector<int> nums3;
    assert(compressSortedArray(nums3) == 0);

    // Test case 4: All elements identical
    std::vector<int> nums4 = {7, 7, 7, 7};
    int k4 = compressSortedArray(nums4);
    assert(k4 == 1);
    assert(nums4[0] == 7);

    // Test case 5: No duplicates (already unique)
    std::vector<int> nums5 = {-3, -1, 0, 2, 5};
    int k5 = compressSortedArray(nums5);
    assert(k5 == 5);
    assert(nums5[0] == -3 && nums5[1] == -1 && nums5[2] == 0 && nums5[3] == 2 && nums5[4] == 5);

    // Test case 6: Duplicates at the end
    std::vector<int> nums6 = {1, 2, 2, 3, 3, 3};
    int k6 = compressSortedArray(nums6);
    assert(k6 == 3);
    assert(nums6[0] == 1 && nums6[1] == 2 && nums6[2] == 3);

    // Test case 7: Negative numbers with duplicates
    std::vector<int> nums7 = {-5, -5, -4, -4, -4, -1, -1};
    int k7 = compressSortedArray(nums7);
    assert(k7 == 3);
    assert(nums7[0] == -5 && nums7[1] == -4 && nums7[2] == -1);

    // Test case 8: Single element
    std::vector<int> nums8 = {42};
    int k8 = compressSortedArray(nums8);
    assert(k8 == 1);
    assert(nums8[0] == 42);

    return 0;
}
#include <vector>

// Removes duplicates in-place from a sorted vector and returns the count of unique elements.
// The first k elements of nums contain the unique values in original order.
int compressSortedArray(std::vector<int>& nums) {
    if (nums.empty()) {
        return 0;
    }
    
    int writePos = 0; // Last placed unique element index
    
    for (std::size_t readPos = 1; readPos < nums.size(); ++readPos) {
        if (nums[writePos] != nums[readPos]) {
            ++writePos;
            nums[writePos] = nums[readPos];
        }
    }
    
    return writePos + 1;
}
// The core insight is that since the input is already sorted, all duplicate values appear consecutively. We can use a two-pointer (or single-writer) technique: maintain an index `writePos` that points to the position where the next unique element should be placed, and a separate traversal index `readPos` to scan through the array. Initially, set `writePos = 0`; since the array is non-empty (we handle the empty case separately), the first element at index 0 is always unique, so we can start scanning from index 1. For each element at `readPos`, compare it with the element at `writePos` (which is the last placed unique element). If they differ, we have found a new unique value, so we increment `writePos` and copy `nums[readPos]` to `nums[writePos]`. If they are equal, we skip (do nothing) and move `readPos` forward. After the loop, `writePos + 1` gives the count of unique elements (since `writePos` is zero-based). Edge cases: empty array returns 0 immediately; array with one element returns 1 without entering the loop. Time complexity is O(n) because each element is visited once, and space complexity is O(1) additional space as we only use a few integer variables. The algorithm preserves the relative order because we always copy from a later index to an earlier one in the same sequence.
