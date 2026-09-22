Create a C++ function that takes an array of integers, its size, and a target sum, and returns a dynamically allocated array of exactly two integers representing the indices (in the original array) of the two elements that add up to the target. If no such pair exists, the function should return `nullptr`. You must ensure the function returns the indices in ascending order (smaller index first), works correctly with negative numbers and duplicates, and does not count an element with itself (i.e., you cannot use the same index twice). The function signature must be `int* findPairWithSum(const int* nums, int size, int target)`.
The solution uses a brute-force double loop: for each index `i` from 0 to `size-1`, check every index `j` greater than `i` (so `j > i` ensures each pair is considered once and avoids using the same element twice). If `nums[i] + nums[j] == target`, allocate a two-element array on the heap, store `i` first and `j` second (which are already in ascending order), and return it. If no pair is found after all loops, return `nullptr`. Edge cases include: empty array or size less than 2 → return `nullptr`; negative numbers and duplicates are handled naturally because we compare actual integer values; target can be zero, positive, or negative. Time complexity is O(n²) because of the nested loops, and space complexity is O(1) for the loop variables plus O(1) for the returned allocation (or O(1) total if returning nullptr). The function must be `const`-correct by taking a pointer to `const int` for the input array.
#include <cstddef>  // for nullptr

// Find two indices whose values sum to target. Returns a heap-allocated
// array {smallerIndex, largerIndex}, or nullptr if no such pair exists.
int* findPairWithSum(const int* nums, int size, int target) {
    if (nums == nullptr || size < 2) {
        return nullptr;
    }
    for (int i = 0; i < size - 1; ++i) {
        for (int j = i + 1; j < size; ++j) {
            if (nums[i] + nums[j] == target) {
                int* result = new int[2];
                result[0] = i;
                result[1] = j;
                return result;
            }
        }
    }
    return nullptr;
}
#include <cassert>
#include <cstddef>

// Function to test
int* findPairWithSum(const int* nums, int size, int target);

int main() {
    // Basic case: pair exists
    int nums1[] = {1, 2, 3, 4};
    int* p1 = findPairWithSum(nums1, 4, 7);
    assert(p1 != nullptr);
    assert(p1[0] == 2 && p1[1] == 3);
    delete[] p1;

    // Pair at beginning
    int nums2[] = {5, 2, 9};
    int* p2 = findPairWithSum(nums2, 3, 7);
    assert(p2 != nullptr);
    assert(p2[0] == 0 && p2[1] == 1);
    delete[] p2;

    // No pair exists
    int nums3[] = {1, 2, 3};
    int* p3 = findPairWithSum(nums3, 3, 10);
    assert(p3 == nullptr);

    // Negative values
    int nums4[] = {-3, 4, 1, 8};
    int* p4 = findPairWithSum(nums4, 4, 5);
    assert(p4 != nullptr);
    assert(p4[0] == 1 && p4[1] == 3);
    delete[] p4;

    // Duplicates allowed (different indices)
    int nums5[] = {6, 2, 6, 3};
    int* p5 = findPairWithSum(nums5, 4, 12);
    assert(p5 != nullptr);
    assert(p5[0] == 0 && p5[1] == 2);
    delete[] p5;

    // Array too small
    int nums6[] = {1};
    assert(findPairWithSum(nums6, 1, 2) == nullptr);

    // Null pointer input
    assert(findPairWithSum(nullptr, 0, 0) == nullptr);
}
