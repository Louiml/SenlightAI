Write a C++ function named `singleNonDuplicate` that takes a non-empty `std::vector<int>` where every element appears exactly twice except for one element that appears exactly once, and returns the value of that unique element. The vector is guaranteed to be sorted in strictly ascending order, and the length is always odd. The function must be `const`-correct (i.e., it should not modify the input vector) and should work for vectors of any size from 1 upward, including large sizes. You may assume the input is always well‑formed per the above guarantees, so no error handling for invalid input is required.

The problem is a classic "single element in a sorted array where all others appear twice." Since the input is sorted and each duplicate appears exactly twice, the pairs occupy adjacent positions. Starting from index 0, if we step by 2 (checking positions 0–1, 2–3, 4–5, …), we can compare each pair: if `nums[i]` and `nums[i+1]` are equal, that pair is complete and the unique element must lie to the right; if they differ, then `nums[i]` is the unique element (because the left side would have formed perfect pairs). The loop naturally stops when `i` reaches the last index (odd-length guarantee), at which point the last element is the single. This approach is `O(n)` time and `O(1)` extra space. Edge cases: single-element vector (returns that element immediately), unique element at the beginning (first comparison fails), unique element at the end (loop reaches last index), and unique element in the middle (first mismatch at a pair boundary). The algorithm handles all these correctly.

#include <vector>

// Given a sorted vector where every element appears exactly twice except one,
// return the element that appears exactly once.
int singleNonDuplicate(const std::vector<int>& nums) {
    // Step through pairs: (0,1), (2,3), ...
    for (std::size_t i = 0; i < nums.size(); i += 2) {
        // If we are at the last index, this is the single element.
        if (i == nums.size() - 1) {
            return nums[i];
        }
        // If the pair doesn't match, the first element of this pair is unique.
        if (nums[i] != nums[i + 1]) {
            return nums[i];
        }
    }
    // Should never reach here given valid input, but return a sentinel.
    return -1;
}

#include <cassert>
#include <vector>

int main() {
    // Basic case with unique in the middle.
    assert(singleNonDuplicate({1, 1, 2, 3, 3, 4, 4, 8, 8}) == 2);
    // Unique at the beginning.
    assert(singleNonDuplicate({0, 1, 1, 2, 2, 3, 3}) == 0);
    // Unique at the end.
    assert(singleNonDuplicate({1, 1, 2, 2, 3, 3, 4}) == 4);
    // Single-element vector.
    assert(singleNonDuplicate({5}) == 5);
    // Larger vector with unique in middle.
    std::vector<int> v = {1, 1, 2, 2, 3, 3, 4, 4, 5, 6, 6, 7, 7, 8, 8, 9, 9};
    assert(singleNonDuplicate(v) == 5);
    // All duplicates except last.
    std::vector<int> w = {10, 10, 20, 20, 30, 30, 40};
    assert(singleNonDuplicate(w) == 40);
    // All duplicates except first.
    std::vector<int> x = {100, 200, 200, 300, 300, 400, 400};
    assert(singleNonDuplicate(x) == 100);
    return 0;
}
