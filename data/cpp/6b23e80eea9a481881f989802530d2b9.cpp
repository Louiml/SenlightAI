Write a C++ function `double medianOfTwoSortedArrays(const std::vector<int>& nums1, const std::vector<int>& nums2)` that takes two sorted integer arrays (possibly empty, possibly of different sizes) and returns the median of the combined sorted sequence. The median is the middle element if the total number of elements is odd, or the average of the two middle elements if even. Your solution must work in O(log(min(n,m))) time and O(1) extra space (excluding input), without fully merging the arrays or modifying inputs. Handle empty arrays gracefully (e.g., if one array is empty, the median is simply the median of the other array). You may assume all integers are distinct for simplicity, but your solution should still work correctly with duplicates. Do not use built-in median functions; implement the binary search partitioning approach.

The optimal solution uses binary search on the smaller array to find a correct partition point such that the left half of the combined array has exactly `(totalSize+1)/2` elements (the left half includes the smaller of the two middle elements when total is even). For a chosen partition index `i` in the smaller array (between 0 and its size), we derive the corresponding partition index `j` in the larger array as `(totalSize+1)/2 - i`. We then check if the partition is valid: every element in the left half must be ≤ every element in the right half. This requires `maxLeftX <= minRightY` and `maxLeftY <= minRightX`, where `maxLeftX` is the largest element on the left side of the partition in the smaller array (or `-infinity` if `i==0`), `minRightX` is the smallest on the right side (or `+infinity` if `i==size`), and similarly for the larger array with `j`. If `maxLeftX > minRightY`, we move `hi` to `i-1` (we need fewer elements from the smaller array). If `maxLeftY > minRightX`, we move `lo` to `i+1`. Once a valid partition is found, the median is `max(maxLeftX, maxLeftY)` when total size is odd, or `(max(maxLeftX, maxLeftY) + min(minRightX, minRightY))/2.0` when even. Edge cases include empty arrays (return median of the non-empty one) and when a partition boundary is at the very start or end of an array, use `INT_MIN`/`INT_MAX` as sentinels. Time complexity is O(log(min(n,m))) because binary search runs on the smaller array. Space complexity is O(1) aside from a few variables.

#include <vector>
#include <algorithm>
#include <climits>

// Return the median of two sorted arrays without fully merging them.
// Uses binary search on the smaller array for O(log(min(n,m))) time.
double medianOfTwoSortedArrays(const std::vector<int>& nums1, const std::vector<int>& nums2) {
    // Ensure nums1 is the smaller array (or equal size)
    const std::vector<int>& A = (nums1.size() <= nums2.size()) ? nums1 : nums2;
    const std::vector<int>& B = (nums1.size() <= nums2.size()) ? nums2 : nums1;

    int m = static_cast<int>(A.size());
    int n = static_cast<int>(B.size());

    // Handle individual empty arrays
    if (m == 0) {
        int mid = n / 2;
        if (n % 2 == 1) return static_cast<double>(B[mid]);
        else return (static_cast<double>(B[mid-1]) + static_cast<double>(B[mid])) / 2.0;
    }
    if (n == 0) {
        int mid = m / 2;
        if (m % 2 == 1) return static_cast<double>(A[mid]);
        else return (static_cast<double>(A[mid-1]) + static_cast<double>(A[mid])) / 2.0;
    }

    int totalLeft = (m + n + 1) / 2; // number of elements on left side
    int lo = 0, hi = m;

    while (lo <= hi) {
        int i = (lo + hi) / 2;       // partition in A
        int j = totalLeft - i;       // partition in B

        int leftA  = (i == 0) ? INT_MIN : A[i-1];
        int rightA = (i == m) ? INT_MAX : A[i];
        int leftB  = (j == 0) ? INT_MIN : B[j-1];
        int rightB = (j == n) ? INT_MAX : B[j];

        if (leftA <= rightB && leftB <= rightA) {
            // Correct partition
            if ((m + n) % 2 == 1) {
                return static_cast<double>(std::max(leftA, leftB));
            } else {
                double leftMax = static_cast<double>(std::max(leftA, leftB));
                double rightMin = static_cast<double>(std::min(rightA, rightB));
                return (leftMax + rightMin) / 2.0;
            }
        } else if (leftA > rightB) {
            hi = i - 1; // too many from A, move left
        } else {
            lo = i + 1; // too few from A, move right
        }
    }
    // Should never reach here if inputs are sorted and sizes are valid
    return 0.0;
}

#include <cassert>
#include <vector>

// Assume the solution function is declared above

int main() {
    // Basic odd total
    assert(medianOfTwoSortedArrays({1,3}, {2}) == 2.0);
    // Basic even total
    assert(medianOfTwoSortedArrays({1,2}, {3,4}) == 2.5);
    // One empty array (odd)
    assert(medianOfTwoSortedArrays({}, {1,2,3}) == 2.0);
    // One empty array (even)
    assert(medianOfTwoSortedArrays({}, {1,2,3,4}) == 2.5);
    // Both empty (should not happen per spec but handle gracefully? Not required, but test with non-empty)
    // Duplicates
    assert(medianOfTwoSortedArrays({1,1}, {1,1}) == 1.0);
    // Different sizes, all on one side
    assert(medianOfTwoSortedArrays({1,2}, {3}) == 2.0);
    // Larger gap
    assert(medianOfTwoSortedArrays({1,5,9}, {2,3,4,6,7,8}) == 5.0);
    // Negative numbers
    assert(medianOfTwoSortedArrays({-5,-3}, {-2,-1}) == -2.5);
    // Single element each
    assert(medianOfTwoSortedArrays({1}, {2}) == 1.5);
    // First array larger than second
    assert(medianOfTwoSortedArrays({1,4,7,10}, {2,3}) == 3.5);
}
