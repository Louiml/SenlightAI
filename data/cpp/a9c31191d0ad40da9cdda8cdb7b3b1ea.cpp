// Given an array of `n` integers, write a C++ function `countBadElements(const std::vector<int>& arr)` that returns the number of "bad" elements, where an element is considered bad if it is strictly greater than the minimum of all elements to its right (i.e., elements at strictly higher indices). The array is non-empty. For example, in `[3, 1, 2, 4]`, scanning from right to left: 4 is good (no right elements), 2 is greater than min of right = 4? No, 2 ≤ 4 so good; 1 is ≤ min right = 2 so good; 3 is > min right = 1 so bad → answer = 1. Handle edge cases: array of size 1 (answer 0), all equal (answer 0), strictly decreasing (answer n-1), and negative numbers.

The problem is solved by scanning the array from right to left while maintaining the minimum value seen so far among elements to the right. Initialize the minimum as the last element. For each element from index n-2 down to 0, if the current element is strictly greater than the current minimum, it is bad and we increment a counter; otherwise, we update the minimum to the current element (since it's smaller or equal). This works because at each step we know the smallest element among all indices greater than the current one. Edge cases: empty array is not allowed; array of one element always returns 0 because there are no right neighbors. All equal elements produce no bad elements because no element is strictly greater than the minimum. Strictly decreasing array: every element except the last is greater than the minimum to its right (which is the next element), so counter = n-1. Negative numbers work naturally with integer comparison. Time complexity: O(n) single pass. Space complexity: O(1) extra (excluding input vector).

#include <vector>
#include <algorithm>

// Counts how many elements are strictly greater than the minimum of all elements to their right.
// The input vector must be non-empty.
int countBadElements(const std::vector<int>& arr) {
    int n = static_cast<int>(arr.size());
    if (n == 0) return 0; // defensive, though problem guarantees non-empty

    int badCount = 0;
    int minRight = arr[n - 1]; // minimum among elements strictly to the right of current index

    // Iterate from second-last element down to first
    for (int i = n - 2; i >= 0; --i) {
        if (arr[i] > minRight) {
            ++badCount;
        } else {
            minRight = arr[i]; // update min with current element (it's ≤ current minRight)
        }
    }
    return badCount;
}

#include <cassert>
#include <vector>

int main() {
    // Basic cases
    assert(countBadElements({3, 1, 2, 4}) == 1);
    assert(countBadElements({5, 4, 3, 2, 1}) == 4); // strictly decreasing
    assert(countBadElements({1, 2, 3, 4}) == 0);    // strictly increasing
    assert(countBadElements({7}) == 0);             // single element

    // All equal
    assert(countBadElements({2, 2, 2, 2}) == 0);

    // Negative numbers
    assert(countBadElements({-1, -5, -3, -10}) == 2); // -1 > -5? no, -5 ≤ -5? no, -5 > -10? yes, -3 > -10? yes → 2

    // Mixed with duplicates
    assert(countBadElements({5, 1, 5, 1}) == 2); // from right: 1 good, 5 > 1 bad, 1 ≤ 1 good, 5 > 1 bad → 2

    // Larger array
    std::vector<int> big = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
    assert(countBadElements(big) == 9);

    // Const correctness: function works with const vector
    const std::vector<int> constVec = {4, 3, 2, 1};
    assert(countBadElements(constVec) == 3);

    return 0;
}
