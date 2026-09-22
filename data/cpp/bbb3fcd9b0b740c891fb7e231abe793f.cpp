// Write a C++ function named `removeAllOccurrences` that takes a `std::vector<int>& nums` (by reference, so the original vector is modified) and an integer `val`, removes all occurrences of `val` from the vector, and returns the new logical length of the vector after removal. The function must maintain the relative order of the remaining elements (the ones not equal to `val`) without using any extra storage (i.e., in-place, O(1) auxiliary space). Elements beyond the returned length in the vector can contain any values; they are not considered part of the result. The function must work correctly for empty input, when all elements equal `val` (returning 0), when no element equals `val` (returning the original size), and for arbitrary positive and negative integers. Note: You are not allowed to use `std::remove` or its variants; you must implement the logic manually.
The core idea is a two-pointer traversal from the front: one pointer (`slow`) tracks the position where the next valid element should be placed, and the other pointer (`fast`) scans each element. When `nums[fast]` is not equal to `val`, we copy it to `nums[slow]` and increment `slow`. If it equals `val`, we simply skip it. After the loop, `slow` equals the count of elements not equal to `val`, which is the new logical size. This works in-place because we never need the old values at indices `>= slow` after we write to them, and each element is processed once. Edge cases: empty input (loop doesn't run, returns 0), all elements equal `val` (slow stays 0, returns 0), no element equals `val` (slow increments for every element, returns original size). The algorithm runs in O(n) time and O(1) auxiliary space, with `n` being the original size of the vector.
#include <vector>

// Removes all occurrences of val from nums in-place.
// Returns the new logical length of nums after removal.
// The relative order of remaining elements is preserved.
int removeAllOccurrences(std::vector<int>& nums, int val) {
    int slow = 0;  // position to place the next valid element
    for (int fast = 0; fast < static_cast<int>(nums.size()); ++fast) {
        if (nums[fast] != val) {
            nums[slow] = nums[fast];
            ++slow;
        }
    }
    return slow;  // new logical size
}
#include <cassert>
#include <vector>

int main() {
    std::vector<int> v1 = {3, 2, 2, 3};
    assert(removeAllOccurrences(v1, 3) == 2);
    assert(v1[0] == 2 && v1[1] == 2);

    std::vector<int> v2 = {0, 1, 2, 2, 3, 0, 4, 2};
    assert(removeAllOccurrences(v2, 2) == 5);
    assert(v2[0] == 0 && v2[1] == 1 && v2[2] == 3 && v2[3] == 0 && v2[4] == 4);

    std::vector<int> v3 = {};
    assert(removeAllOccurrences(v3, 5) == 0);

    std::vector<int> v4 = {7, 7, 7};
    assert(removeAllOccurrences(v4, 7) == 0);

    std::vector<int> v5 = {1, 2, 3};
    assert(removeAllOccurrences(v5, 9) == 3);
    assert(v5[0] == 1 && v5[1] == 2 && v5[2] == 3);

    std::vector<int> v6 = {-1, -1, 0, 1, -1};
    assert(removeAllOccurrences(v6, -1) == 2);
    assert(v6[0] == 0 && v6[1] == 1);

    std::vector<int> v7 = {5};
    assert(removeAllOccurrences(v7, 5) == 0);

    std::vector<int> v8 = {5};
    assert(removeAllOccurrences(v8, 6) == 1);
    assert(v8[0] == 5);

    std::vector<int> v9 = {2, 2, 1, 2, 3, 2, 4};
    assert(removeAllOccurrences(v9, 2) == 3);
    assert(v9[0] == 1 && v9[1] == 3 && v9[2] == 4);

    return 0;
}
