/*
Write a C++ function named `isSortedRotated` that takes a `const std::vector<int>&` and returns a `bool` indicating whether the array is a sorted (in non-decreasing order) array that has been rotated by some number of positions (including zero, i.e., a perfectly sorted array is also considered valid). The array must have at least 2 elements. The function should handle duplicates and negative numbers. A rotated sorted array is one where, starting from some index, the elements are in non-decreasing order and then wrap around to the beginning. For example, `[3,4,5,1,2]` is valid (rotation of `[1,2,3,4,5]`), `[1,2,3,4,5]` is valid (rotation by 0), `[5,5,1,2,3]` is valid, but `[2,1,3,4,5]` is not. The function should not print anything; it should only return the boolean result.
*/
#include <vector>

// Returns true if the given vector is a sorted (non-decreasing) array rotated by any amount.
bool isSortedRotated(const std::vector<int>& arr) {
    const std::size_t n = arr.size();
    if (n < 2) return false; // Task requires at least 2 elements.

    int dropCount = 0;
    for (std::size_t i = 0; i < n; ++i) {
        if (arr[i] > arr[(i + 1) % n]) {
            ++dropCount;
            if (dropCount > 1) return false;
        }
    }
    return true;
}
#include <cassert>
#include <vector>

// Declaration of the function under test
bool isSortedRotated(const std::vector<int>& arr);

int main() {
    // Perfectly sorted, no rotation
    assert(isSortedRotated({1, 2, 3, 4, 5}) == true);
    // Rotated once
    assert(isSortedRotated({3, 4, 5, 1, 2}) == true);
    // Rotation with duplicates
    assert(isSortedRotated({5, 5, 1, 2, 3}) == true);
    // All equal elements (no drops)
    assert(isSortedRotated({7, 7, 7}) == true);
    // Negative numbers rotated
    assert(isSortedRotated({-3, -2, -5, -4}) == true); // rotation of [-5,-4,-3,-2]
    // Invalid: more than one drop
    assert(isSortedRotated({2, 1, 3, 4, 5}) == false);
    // Invalid: random unsorted
    assert(isSortedRotated({1, 3, 2, 4}) == false);
    // Invalid: only one element (size < 2)
    assert(isSortedRotated({1}) == false);
    // Edge: drop at wrap-around for sorted array? Actually sorted non-rotated has no drop, but test boundary
    assert(isSortedRotated({1, 2, 3}) == true);
    return 0;
}
// The problem is to detect whether a given vector is a sorted array that has been rotated. A standard approach is to count the number of "drops" where an element is greater than the next element (considering wrap-around from the last element to the first). In a perfectly sorted (non-decreasing) array, there is exactly zero drops because the last element is ≤ the first element. In a rotated sorted array (rotation > 0), there is exactly one drop, because the sequence is increasing up to the rotation point, then drops once, then continues increasing. If there is more than one drop, the array is not a rotated sorted array. Also, we must be careful when the array has duplicates, because a drop might be at a location where the next element is equal (no drop), and a valid rotated array could have the drop at the last-to-first transition (e.g., `[2,3,4,5,1]` has a drop from 5 to 1). Additionally, for an array that is fully sorted without rotation, the last element is ≤ the first element, so there is zero drops. So the algorithm: iterate over all consecutive pairs (including the pair `(last, first)`), count how many times `arr[i] > arr[(i+1)%n]`. If count is 0 or 1, return true; else return false. Edge cases: arrays with all equal elements (e.g., `[5,5,5]`) have zero drops and are valid. Also, for a valid rotation with duplicates, e.g., `[2,2,3,1,2]` – let's check: pairs: 2>2? no; 2>3? no; 3>1? yes (drop1); 1>2? no; last-to-first: 2>2? no → count=1 → valid. But is that actually a rotated sorted array? The original sorted array could be `[1,2,2,2,3]`? Rotating by 2 gives `[2,3,1,2,2]` – not our example. However, our algorithm would accept some arrays that may not be strictly formed by rotating a sorted array with duplicates? Let's think: For a rotated sorted array, there can be at most one point where the sequence decreases (the "break"). Our counting method correctly identifies that condition. It's a known problem "Check if array is sorted and rotated" on LeetCode (problem 1752). The solution counts the number of times `nums[i] > nums[(i+1) % n]` and returns `count <= 1`. Time complexity: O(n), space O(1). This handles all cases correctly.
