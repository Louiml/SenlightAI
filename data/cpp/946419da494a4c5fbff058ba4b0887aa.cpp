Write a standalone C++ function `double medianOfSortedArrays(const std::vector<int>& nums1, const std::vector<int>& nums2)` that takes two sorted integer arrays (in non-decreasing order) and returns the median of the combined sorted sequence formed by merging them. The arrays may be empty, may have different lengths, and may contain duplicate values. The median is defined as the middle element if the total number of elements is odd, or the average of the two middle elements if even. Your solution must not rely on sorting a merged copy of the arrays (that would be trivial); instead, you must implement an efficient algorithm based on binary search or a divide-and-conquer approach that runs in \(O(\log(\min(n,m)))\) time, where \(n\) and \(m\) are the lengths of the two arrays. Handle edge cases such as one array being empty, both arrays being empty (return 0.0 or throw, but specify your choice), and arrays with negative numbers. Provide a clean, const-correct, self-contained implementation with only the function (no `main` or test code in the solution section).
// The optimal solution uses binary search on the smaller array to partition both arrays into two halves such that the left half contains exactly the first `k` smallest elements (where `k = (n+m+1)/2` for the median). We treat the partition as a cut in each array: `partitionA` elements from `nums1` and `partitionB = k - partitionA` from `nums2` are taken for the left half. A valid partition satisfies: `maxLeftA <= minRightB` and `maxLeftB <= minRightA`. Because both arrays are sorted, we only need to check the boundary elements. If not, we adjust `partitionA` via binary search. If one array is empty, we directly return the median of the other. If both are empty, we return `0.0` (specified in the task). For the even/odd total length, after finding a correct partition, the median is: if odd, `max(maxLeftA, maxLeftB)`; if even, the average of that max and `min(minRightA, minRightB)`. Time complexity is \(O(\log(\min(n,m)))\), space \(O(1)\). Edge cases: when one array is empty, the partition on that side has no elements; when a cut is at index 0 or at the end, we must treat the missing boundary as `-INF` or `+INF` using `INT_MIN`/`INT_MAX` (or `LONG_MIN`/`LONG_MAX` to avoid overflow with negative values). Since all elements are integers, using `long long` for comparisons is safe.
#include <vector>
#include <algorithm>
#include <climits>

/**
 * @brief Returns the median of two sorted integer arrays.
 * 
 * The function efficiently finds the median without merging the arrays,
 * using binary search on the smaller array. It handles empty arrays and
 * returns 0.0 if both are empty.
 * 
 * @param nums1 First sorted vector (non-decreasing).
 * @param nums2 Second sorted vector (non-decreasing).
 * @return double The median of the combined sorted sequence.
 */
double medianOfSortedArrays(const std::vector<int>& nums1, const std::vector<int>& nums2) {
    // Ensure nums1 is the smaller (or equal) array for binary search efficiency.
    const std::vector<int>& A = (nums1.size() <= nums2.size()) ? nums1 : nums2;
    const std::vector<int>& B = (nums1.size() <= nums2.size()) ? nums2 : nums1;

    const int n = static_cast<int>(A.size());
    const int m = static_cast<int>(B.size());
    const int total = n + m;

    if (total == 0) {
        return 0.0;  // Both arrays are empty, as per task specification.
    }

    const int half = (total + 1) / 2;  // Number of elements in left half.

    int low = 0;
    int high = n;  // partitionA can range from 0 to n.

    while (low <= high) {
        const int partitionA = low + (high - low) / 2;
        const int partitionB = half - partitionA;

        // Guard against out-of-bound accesses by clamping indices.
        const long long maxLeftA = (partitionA == 0) ? LLONG_MIN : A[partitionA - 1];
        const long long minRightA = (partitionA == n) ? LLONG_MAX : A[partitionA];

        const long long maxLeftB = (partitionB == 0) ? LLONG_MIN : B[partitionB - 1];
        const long long minRightB = (partitionB == m) ? LLONG_MAX : B[partitionB];

        if (maxLeftA <= minRightB && maxLeftB <= minRightA) {
            // Correct partition found.
            if (total % 2 == 0) {
                const long long leftMax = std::max(maxLeftA, maxLeftB);
                const long long rightMin = std::min(minRightA, minRightB);
                return (leftMax + rightMin) / 2.0;
            } else {
                return static_cast<double>(std::max(maxLeftA, maxLeftB));
            }
        } else if (maxLeftA > minRightB) {
            // Too many elements from A in left half, move partitionA left.
            high = partitionA - 1;
        } else {
            // Too few elements from A in left half, move partitionA right.
            low = partitionA + 1;
        }
    }

    // Should never reach here if inputs are properly sorted.
    return 0.0;
}
#include <cassert>
#include <vector>
#include <cmath>

// Forward declaration (already defined in solution, but for clarity)
double medianOfSortedArrays(const std::vector<int>& nums1, const std::vector<int>& nums2);

int main() {
    // Both arrays empty
    assert(medianOfSortedArrays({}, {}) == 0.0);

    // One array empty
    assert(medianOfSortedArrays({1,2,3}, {}) == 2.0);
    assert(medianOfSortedArrays({}, {4,5,6,7}) == 5.5);

    // Basic odd total length
    assert(medianOfSortedArrays({1,3}, {2}) == 2.0);
    assert(medianOfSortedArrays({0,0}, {0,0}) == 0.0);

    // Basic even total length
    assert(medianOfSortedArrays({1,2}, {3,4}) == 2.5);
    assert(medianOfSortedArrays({1,3,5}, {2,4,6}) == 3.5);

    // Negative numbers and duplicates
    assert(medianOfSortedArrays({-5,-1,3}, {-2,0,7}) == 0.0);
    assert(medianOfSortedArrays({-10,-10}, {-9,-8}) == -9.5);

    // Large one array, small other
    assert(medianOfSortedArrays({1,2,3,4,5,6,7,8,9}, {5}) == 5.0);
    assert(medianOfSortedArrays({1,100}, {50,60}) == 55.0);

    // Equal length arrays
    assert(medianOfSortedArrays({1,2}, {3,4}) == 2.5);
    assert(medianOfSortedArrays({-3,-2}, {-1,0}) == -1.5);

    // Asymmetric sizes
    assert(medianOfSortedArrays({1,2,3,4,5}, {6,7,8}) == 4.0);
    assert(medianOfSortedArrays({10,20}, {30,40,50,60}) == 35.0);

    // Test with floating-point precision
    std::vector<int> a = {1, 3, 8, 9, 15};
    std::vector<int> b = {7, 11, 18, 19, 21, 25};
    double med = medianOfSortedArrays(a, b);
    assert(std::abs(med - 11.0) < 1e-9);

    return 0;
}
