Given a sorted integer array `nums` in non-decreasing order, write a C++ function `sortedSquares` that returns a new array containing the squares of each element, also sorted in non-decreasing order. The input array may contain negative numbers, zeros, and positive numbers. The original array remains unchanged, and the function must handle arrays of any length from 1 to 10,000. The solution must run in linear time and use only constant extra space, without using the standard `sort` function.
The array is already sorted, so the largest absolute-values are at the two ends: the most negative value (leftmost) and the largest positive value (rightmost). Use two pointers: one starting at the beginning and one at the end. Compare the absolute values of the elements at these pointers. The one with the larger absolute value will produce the largest square among the remaining elements, so store its square at the current end position of the result array, then move the corresponding pointer inward. Continue until the two pointers meet (inclusive). This is a classic two-pointer merge in reverse. Edge cases: single element (works naturally), all negative numbers (the pointer logic still yields squares in ascending order because the leftmost element has the largest absolute value), all positive (right pointer dominates), and zeros (they will be placed in the middle appropriately). Time complexity is O(n) because each element is processed exactly once. Space complexity is O(n) for the result array, plus O(1) auxiliary space for the pointers and index.
#include <vector>
#include <cstdlib>

// Return a new vector containing the squares of each element in sorted order.
// The input vector is assumed to be sorted in non-decreasing order.
std::vector<int> sortedSquares(const std::vector<int>& nums) {
    int n = nums.size();
    std::vector<int> result(n);
    int left = 0;
    int right = n - 1;
    int pos = n - 1;
    
    while (left <= right) {
        if (std::abs(nums[left]) < std::abs(nums[right])) {
            result[pos--] = nums[right] * nums[right];
            --right;
        } else {
            result[pos--] = nums[left] * nums[left];
            ++left;
        }
    }
    return result;
}
#include <cassert>
#include <vector>

// Test helper to compare vectors
void assertVectorEqual(const std::vector<int>& a, const std::vector<int>& b) {
    assert(a.size() == b.size());
    for (size_t i = 0; i < a.size(); ++i) {
        assert(a[i] == b[i]);
    }
}

int main() {
    // General mixed negatives and positives
    assertVectorEqual(sortedSquares({-4, -1, 0, 3, 10}), {0, 1, 9, 16, 100});
    // All negatives
    assertVectorEqual(sortedSquares({-7, -3, -1}), {1, 9, 49});
    // All positives
    assertVectorEqual(sortedSquares({1, 2, 3}), {1, 4, 9});
    // Single element zero
    assertVectorEqual(sortedSquares({0}), {0});
    // Single element negative
    assertVectorEqual(sortedSquares({-5}), {25});
    // Duplicates and zeros
    assertVectorEqual(sortedSquares({-3, -3, 0, 2, 2}), {0, 4, 4, 9, 9});
    // Large numbers
    assertVectorEqual(sortedSquares({-10000, 0, 10000}), {0, 100000000, 100000000});
    // Adjacent negatives and positives
    assertVectorEqual(sortedSquares({-2, -1, 1, 2}), {1, 1, 4, 4});
    return 0;
}
