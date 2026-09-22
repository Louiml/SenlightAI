Write a C++ function `medianOfTwoSortedArrays` that takes two sorted integer vectors `nums1` and `nums2` (both possibly empty, but not both empty) and returns the median of the combined sorted array as a `double`. The median is the middle value when the combined array is sorted; if the combined size is even, it is the average of the two middle elements. You must achieve this in `O(log(min(n, m)))` time, where `n` and `m` are the sizes of the input vectors. The function must handle edge cases such as one vector being empty, negative values, and duplicates.
// The solution uses a binary search on the smaller array to find a valid partition point. Since the two arrays are sorted, we aim to split both arrays at indices `a` (from `nums1`) and `b` (from `nums2`) such that the left part contains exactly `(n+m+1)/2` elements, and every element in the left part is ≤ every element in the right part. We set `low=0`, `high=n` (size of the smaller array) and compute `a = mid`, `b = (n+m+1)/2 - a`. Then we check the boundary elements: `minA` (last element of left part of `nums1`), `minB` (last element of left part of `nums2`), `maxA` (first element of right part of `nums1`), `maxB` (first element of right part of `nums2`). If `minA <= maxB` and `minB <= maxA`, the partition is correct; the median is `max(minA, minB)` if total length is odd, else `(max(minA, minB) + min(maxA, maxB)) / 2.0`. If `minA > maxB`, we need to move left (`high = mid - 1`); otherwise, move right. Edge cases are handled with `INT_MIN` and `INT_MAX` for out-of-bound indices. This runs in `O(log(min(n, m)))` time and `O(1)` space.
#include <vector>
#include <algorithm>
#include <climits>

// Returns the median of the combined sorted array from two sorted vectors.
double medianOfTwoSortedArrays(const std::vector<int>& nums1, const std::vector<int>& nums2) {
    // Ensure nums1 is the smaller or equal-sized vector.
    if (nums1.size() > nums2.size()) {
        return medianOfTwoSortedArrays(nums2, nums1);
    }

    const int n = static_cast<int>(nums1.size());
    const int m = static_cast<int>(nums2.size());

    int low = 0;
    int high = n;

    while (low <= high) {
        const int a = low + (high - low) / 2;  // partition index in nums1
        const int b = (n + m + 1) / 2 - a;     // partition index in nums2

        // Boundary elements: left side max and right side min.
        const int leftMax1 = (a > 0) ? nums1[a - 1] : INT_MIN;
        const int leftMax2 = (b > 0) ? nums2[b - 1] : INT_MIN;
        const int rightMin1 = (a < n) ? nums1[a] : INT_MAX;
        const int rightMin2 = (b < m) ? nums2[b] : INT_MAX;

        if (leftMax1 <= rightMin2 && leftMax2 <= rightMin1) {
            // Correct partition found.
            if ((n + m) % 2 == 1) {
                return static_cast<double>(std::max(leftMax1, leftMax2));
            } else {
                return (static_cast<double>(std::max(leftMax1, leftMax2)) +
                        static_cast<double>(std::min(rightMin1, rightMin2))) / 2.0;
            }
        } else if (leftMax1 > rightMin2) {
            // Move left in nums1.
            high = a - 1;
        } else {
            // Move right in nums1.
            low = a + 1;
        }
    }

    // Should never reach here for valid input.
    return 0.0;
}
#include <cassert>
#include <vector>

int main() {
    // Odd total length
    assert(medianOfTwoSortedArrays({1, 3}, {2}) == 2.0);
    // Even total length
    assert(medianOfTwoSortedArrays({1, 2}, {3, 4}) == 2.5);
    // One empty vector
    assert(medianOfTwoSortedArrays({}, {1, 2, 3}) == 2.0);
    assert(medianOfTwoSortedArrays({5}, {}) == 5.0);
    // Negative numbers
    assert(medianOfTwoSortedArrays({-5, -1}, {-3, 0}) == -2.0);
    // Duplicates
    assert(medianOfTwoSortedArrays({1, 1, 1}, {1, 1}) == 1.0);
    // Unequal sizes
    assert(medianOfTwoSortedArrays({1, 2, 3, 4, 5}, {6, 7, 8}) == 4.5);
    // Large values
    assert(medianOfTwoSortedArrays({1000000}, {1000001}) == 1000000.5);
    // Both arrays of size 1, even total
    assert(medianOfTwoSortedArrays({1}, {2}) == 1.5);
    // Both arrays of size 2, odd total with one empty
    assert(medianOfTwoSortedArrays({1, 2}, {}) == 1.5);
    // Mixed signs
    assert(medianOfTwoSortedArrays({-10, -5, 0}, {-3, -2}) == -2.5);
    // All same values
    assert(medianOfTwoSortedArrays({7, 7}, {7, 7, 7}) == 7.0);
    return 0;
}
