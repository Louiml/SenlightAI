Write a C++ function that takes a `std::vector<int>` by reference, removes all duplicate elements while preserving the relative order of the first occurrences of each distinct value, and returns the new number of unique elements. The function must modify the vector in place so that the first `k` elements contain the unique values in their original order, and the remaining elements (from index `k` to `size()-1`) may have any values. The vector can be empty, contain negative numbers, or contain all identical values. Do not use `std::unique` or `std::erase`; implement the deduplication logic manually using a hash set or a sorted approach.

The core challenge is to remove duplicates without reordering the remaining elements. The most straightforward approach is to use a hash set (`std::unordered_set<int>`) to track seen values. Iterate through the vector with a slow pointer (`write` index) and a fast pointer (`read` index). For each element at the read position, if it has not been seen before, copy it to the write position and increment the write index; also insert the value into the set. If it has been seen, skip it. After the loop, resize the vector to `write` size and return `write`. This runs in O(n) average time and O(n) extra space for the set. Edge cases: empty vector returns 0; all duplicates (e.g., [5,5,5]) returns 1 with vector becoming [5]; negative numbers are handled fine. Alternative: sort the vector and then compress, but that would violate order preservation, so the set approach is preferred. Space complexity is O(n) due to the set; this can be reduced to O(1) if the input is sorted, but that's not acceptable here.

#include <vector>
#include <unordered_set>

// Removes duplicates from a vector while preserving the order of first occurrences.
// Modifies the vector in place and returns the number of unique elements.
int removeDuplicatesPreservingOrder(std::vector<int>& nums) {
    std::unordered_set<int> seen;
    int writeIndex = 0;

    for (int readIndex = 0; readIndex < static_cast<int>(nums.size()); ++readIndex) {
        int value = nums[readIndex];
        if (seen.find(value) == seen.end()) {
            seen.insert(value);
            nums[writeIndex] = value;
            ++writeIndex;
        }
    }

    nums.resize(writeIndex);
    return writeIndex;
}

#include <cassert>
#include <vector>

int main() {
    // Test case 1: mixed duplicates
    std::vector<int> nums1 = {1, 2, 2, 3, 1, 4, 3};
    int k1 = removeDuplicatesPreservingOrder(nums1);
    assert(k1 == 4);
    assert((nums1 == std::vector<int>{1, 2, 3, 4}));

    // Test case 2: all identical
    std::vector<int> nums2 = {7, 7, 7};
    int k2 = removeDuplicatesPreservingOrder(nums2);
    assert(k2 == 1);
    assert((nums2 == std::vector<int>{7}));

    // Test case 3: empty vector
    std::vector<int> nums3;
    int k3 = removeDuplicatesPreservingOrder(nums3);
    assert(k3 == 0);
    assert(nums3.empty());

    // Test case 4: already unique
    std::vector<int> nums4 = {5, -1, 3, 9};
    int k4 = removeDuplicatesPreservingOrder(nums4);
    assert(k4 == 4);
    assert((nums4 == std::vector<int>{5, -1, 3, 9}));

    // Test case 5: negative numbers and duplicates at boundaries
    std::vector<int> nums5 = {-2, -2, 0, 3, -2, 4, 0};
    int k5 = removeDuplicatesPreservingOrder(nums5);
    assert(k5 == 4);
    assert((nums5 == std::vector<int>{-2, 0, 3, 4}));

    // Test case 6: single element
    std::vector<int> nums6 = {42};
    int k6 = removeDuplicatesPreservingOrder(nums6);
    assert(k6 == 1);
    assert((nums6 == std::vector<int>{42}));

    // Test case 7: duplicates not adjacent
    std::vector<int> nums7 = {1, 1, 2, 1, 2, 3};
    int k7 = removeDuplicatesPreservingOrder(nums7);
    assert(k7 == 3);
    assert((nums7 == std::vector<int>{1, 2, 3}));

    // Test case 8: large values with many duplicates
    std::vector<int> nums8 = {1000, 1000, 1000, 2000, 1000};
    int k8 = removeDuplicatesPreservingOrder(nums8);
    assert(k8 == 2);
    assert((nums8 == std::vector<int>{1000, 2000}));

    return 0;
}
