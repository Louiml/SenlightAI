// Write a C++ function that takes a vector of integers and a positive integer `windowSize`, and returns the maximum sum among all contiguous subarrays (windows) of exactly `windowSize` consecutive elements. The function must handle cases where `windowSize` is larger than the array size by returning `0`, and must work correctly with negative numbers. The input vector may be empty, and `windowSize` is guaranteed to be at least 1.
// The problem is solved using a sliding window technique to avoid recomputing sums from scratch. First, compute the sum of the first `windowSize` elements (if they exist). This sum becomes the initial candidate for the maximum. Then iterate through the array from index `windowSize` onward: for each step, add the new element entering the window and subtract the element leaving the window, updating the maximum if the new sum is larger. Edge cases: if the array size is 0 or `windowSize` > array size, no valid window exists, so return 0. If `windowSize` equals the array size, the only window is the entire array, so its sum is the answer. Negative numbers are handled naturally because the initial sum and updates simply compare numeric values. Time complexity is \(O(n)\) with one pass for the initial sum and one pass for the slide, using \(O(1)\) extra space beyond the input vector.
#include <vector>
#include <algorithm>

// Returns the maximum sum of any contiguous subarray of exactly windowSize elements.
// Returns 0 if no such subarray exists (empty array or windowSize > array size).
int maxWindowSum(const std::vector<int>& arr, int windowSize) {
    const int n = static_cast<int>(arr.size());
    if (n == 0 || windowSize > n) {
        return 0;
    }

    int currentSum = 0;
    for (int i = 0; i < windowSize; ++i) {
        currentSum += arr[i];
    }

    int maxSum = currentSum;
    for (int i = windowSize; i < n; ++i) {
        currentSum = currentSum - arr[i - windowSize] + arr[i];
        maxSum = std::max(maxSum, currentSum);
    }
    return maxSum;
}
#include <cassert>
#include <vector>

int maxWindowSum(const std::vector<int>& arr, int windowSize);

int main() {
    std::vector<int> v1 = {1, 2, 3, 4};
    assert(maxWindowSum(v1, 2) == 7);   // 3+4
    assert(maxWindowSum(v1, 3) == 9);   // 2+3+4
    assert(maxWindowSum(v1, 4) == 10);  // whole array

    std::vector<int> v2 = {-2, -1, -3, -4};
    assert(maxWindowSum(v2, 2) == -3);  // -2 + -1
    assert(maxWindowSum(v2, 1) == -1);  // largest single

    std::vector<int> v3 = {5};
    assert(maxWindowSum(v3, 1) == 5);
    assert(maxWindowSum(v3, 2) == 0);   // window too large

    std::vector<int> v4 = {};
    assert(maxWindowSum(v4, 1) == 0);   // empty

    std::vector<int> v5 = {1, -2, 3, 4, -5};
    assert(maxWindowSum(v5, 3) == 5);   // 1-2+3=2, -2+3+4=5, 3+4-5=2
    assert(maxWindowSum(v5, 5) == 1);   // whole array sum

    return 0;
}
