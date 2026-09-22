// Write a C++ function `int kthSmallestElement(const std::vector<int>& nums, int k)` that returns the k-th smallest element (1-indexed) from an unsorted vector of integers. The vector may contain duplicates, negative numbers, and is guaranteed non-empty with `k` between 1 and the vector size. Do not modify the input vector, and do not use sorting, `std::nth_element`, or any heap-based algorithms. Your solution must use binary search on the value range.
#include <cassert>
#include <vector>
#include <iostream>

// Function declaration (or include the header)
int kthSmallestElement(const std::vector<int>& nums, int k);

int main() {
    // Basic test with duplicates
    std::vector<int> v1 = {1, 4, 5, 3, 19, 3};
    assert(kthSmallestElement(v1, 1) == 1);
    assert(kthSmallestElement(v1, 2) == 3);
    assert(kthSmallestElement(v1, 3) == 3);
    assert(kthSmallestElement(v1, 4) == 4);
    assert(kthSmallestElement(v1, 5) == 5);
    assert(kthSmallestElement(v1, 6) == 19);

    // Negative numbers
    std::vector<int> v2 = {-5, -1, -10, 3, 0};
    assert(kthSmallestElement(v2, 1) == -10);
    assert(kthSmallestElement(v2, 2) == -5);
    assert(kthSmallestElement(v2, 3) == -1);
    assert(kthSmallestElement(v2, 4) == 0);
    assert(kthSmallestElement(v2, 5) == 3);

    // Single element
    std::vector<int> v3 = {42};
    assert(kthSmallestElement(v3, 1) == 42);

    // All duplicates
    std::vector<int> v4 = {7, 7, 7, 7};
    assert(kthSmallestElement(v4, 1) == 7);
    assert(kthSmallestElement(v4, 4) == 7);

    // Large range and perturbed order
    std::vector<int> v5 = {1000, 0, -1000, 500, -250, 750};
    assert(kthSmallestElement(v5, 1) == -1000);
    assert(kthSmallestElement(v5, 2) == -250);
    assert(kthSmallestElement(v5, 3) == 0);
    assert(kthSmallestElement(v5, 4) == 500);
    assert(kthSmallestElement(v5, 5) == 750);
    assert(kthSmallestElement(v5, 6) == 1000);

    // k at boundaries for a sorted array
    std::vector<int> v6 = {2, 4, 6, 8, 10};
    assert(kthSmallestElement(v6, 1) == 2);
    assert(kthSmallestElement(v6, 5) == 10);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
#include <vector>
#include <algorithm>
#include <climits>

// Helper: count how many elements in nums are <= threshold
int countLeq(const std::vector<int>& nums, int threshold) {
    int cnt = 0;
    for (int val : nums) {
        if (val <= threshold) ++cnt;
    }
    return cnt;
}

// Return the k-th smallest element (1-indexed) from nums without sorting.
int kthSmallestElement(const std::vector<int>& nums, int k) {
    // Find min and max in O(n)
    int low = *std::min_element(nums.begin(), nums.end());
    int high = *std::max_element(nums.begin(), nums.end());

    // Binary search on value range
    while (low < high) {
        int mid = low + (high - low) / 2;
        if (countLeq(nums, mid) < k) {
            low = mid + 1;
        } else {
            high = mid;
        }
    }
    return low;
}
// The approach uses binary search on the integer value range, from the minimum to the maximum element of the array. At each mid value, we count how many array elements are less than or equal to mid using a helper `countLeq`. If this count is less than `k`, then the k-th smallest element must be greater than mid, so we move the lower bound to `mid + 1`. Otherwise (count ≥ k), the k-th smallest element is either mid or smaller, so we set the upper bound to `mid`. This works because the predicate "count of elements ≤ x is at least k" is monotonic in x. The binary search converges to the smallest x for which this predicate holds, which is exactly the k-th smallest value. Edge cases: duplicates are naturally handled since the count includes them; negative numbers and large values work because we initialize low and high from the actual array. Complexity: O(n log(range)) time where `range = max - min` and O(1) extra space, not counting input storage.
