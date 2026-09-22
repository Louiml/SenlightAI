/*
Write a C++ function that, given a vector of integers `nums` with at least one element, returns the median value of the first three elements of the vector (i.e., the element that is neither the minimum nor the maximum among those first three). If the vector contains fewer than three elements, the function should return `-1`. The function must not modify the original vector.
*/

#include <vector>
#include <algorithm>

// Given a non-empty vector of integers, return the median of the first three elements.
// If the vector has fewer than three elements, return -1.
int medianOfFirstThree(const std::vector<int>& nums) {
    if (nums.size() < 3) {
        return -1;
    }

    // Copy the first three elements into a local array to avoid modifying the input.
    int firstThree[3] = {nums[0], nums[1], nums[2]};
    
    // Sort the three-element array and return the middle element.
    std::sort(firstThree, firstThree + 3);
    return firstThree[1];
}

#include <cassert>
#include <vector>

int main() {
    // Basic cases
    assert(medianOfFirstThree({1, 2, 3}) == 2);
    assert(medianOfFirstThree({3, 2, 1}) == 2);
    assert(medianOfFirstThree({5, 5, 5}) == 5);

    // Vectors with fewer than three elements
    assert(medianOfFirstThree({}) == -1);
    assert(medianOfFirstThree({42}) == -1);
    assert(medianOfFirstThree({1, 2}) == -1);

    // Vectors with more than three elements; only first three matter
    assert(medianOfFirstThree({10, 20, 30, 40, 50}) == 20);
    assert(medianOfFirstThree({-1, -5, -3, 100}) == -3);

    // Duplicates among first three
    assert(medianOfFirstThree({7, 7, 8}) == 7);
    assert(medianOfFirstThree({7, 8, 7}) == 7);
    assert(medianOfFirstThree({2, 2, 1}) == 2);

    // Negative numbers
    assert(medianOfFirstThree({-3, 0, 3}) == 0);

    // Ensure original vector is not modified
    std::vector<int> original = {9, 1, 5, 4};
    medianOfFirstThree(original);
    assert(original[0] == 9 && original[1] == 1 && original[2] == 5 && original[3] == 4);

    return 0;
}

// The solution examines only the first three elements, regardless of the total vector length. If the vector has fewer than three elements, there is no possible "non-min-or-max" among the first three, so return `-1`. For three or more elements, we need the middle value among the first three. The simplest approach is to copy the first three elements into a small local array or vector, sort them, and return the second element (index 1). Sorting three elements is constant-time and trivial. Edge cases include vectors with exactly three elements, vectors with duplicates among the first three (e.g., `[2, 2, 3]` — the middle is `2`), and vectors with negative numbers. Since we copy the first three elements, the original input remains unchanged. Time complexity is `O(1)` because sorting three elements is a constant operation and we only read up to three values. Space complexity is `O(1)` because we only use a fixed-size local structure.
