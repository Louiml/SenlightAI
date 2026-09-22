Write a C++ function named `majorityElement` that accepts a non-empty vector of integers and returns the majority element, which is defined as the element that appears more than `n / 2` times in the vector (where `n` is the vector's size). The input is guaranteed to always contain a valid majority element. The function must be implemented using a sorting-based approach and must be `const`-correct (i.e., it should not modify the input vector). The function should return an `int` and must handle vectors of any size from 1 upward, including cases where elements are negative or repeated. You may use the standard library sorting function.
#include <cassert>
#include <vector>

// Function declaration (from solution above)
int majorityElement(const std::vector<int>& nums);

int main() {
    // Single element
    assert(majorityElement({5}) == 5);
    // Simple majority
    assert(majorityElement({3, 2, 3}) == 3);
    assert(majorityElement({2, 2, 1, 1, 1, 2, 2}) == 2);
    // Larger vector with negative numbers
    assert(majorityElement({-5, -5, -5, 2, 3}) == -5);
    // All identical
    assert(majorityElement({7, 7, 7, 7}) == 7);
    // Majority at ends
    assert(majorityElement({1, 1, 2, 2, 1, 1, 1}) == 1);
    // Majority exactly n/2+1
    assert(majorityElement({4, 4, 4, 4, 9, 8, 7}) == 4);
    // Mixed with duplicates and negatives
    assert(majorityElement({-1, -1, -1, -1, 10, 10}) == -1);
    // Even size with majority
    assert(majorityElement({6, 6, 6, 6, 2, 2}) == 6);
    // Majority element is the largest
    assert(majorityElement({1, 2, 3, 9, 9, 9, 9}) == 9);
    return 0;
}
#include <vector>
#include <algorithm>

// Return the majority element in a non-empty vector of integers.
// The majority element appears more than n/2 times, so after sorting
// it always occupies the middle index n/2.
int majorityElement(const std::vector<int>& nums) {
    // Create a local copy because we must not modify the input.
    std::vector<int> sorted = nums;
    std::sort(sorted.begin(), sorted.end());
    return sorted[sorted.size() / 2];
}
// The simplest robust approach is to sort the vector in ascending order. Since the majority element occurs more than `n / 2` times, after sorting, it must occupy the middle position of the (0-indexed) array, i.e., index `n / 2`. For example, with `n = 5`, the majority element appears at least 3 times, so after sorting it will be present at indices `2` (even if all occurrences are consecutive at the beginning, middle, or end). This property holds because the majority element's count is greater than half the length, meaning it cannot all be confined to only the left or right half without overlapping the center. The algorithm sorts the vector in `O(n log n)` time and uses `O(1)` auxiliary space (ignoring the space used by the sorting algorithm's internal stack or heap). Edge cases include `n = 1`, where the single element trivially is the majority, and duplicate values of the same element. Since the problem guarantees a majority element exists, we do not need to validate or check counts. The function should take the vector by `const` reference to avoid copying and preserve immutability.
