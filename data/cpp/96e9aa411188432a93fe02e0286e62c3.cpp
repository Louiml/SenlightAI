// Write a C++ function `doubleArrayMedian` that takes two sorted integer arrays `a` and `b` (both strictly increasing, no duplicates within each array) and their sizes `n` and `m`, and returns the median (as a `double`) of the combined sorted array of all `n + m` elements. The median is defined as the middle element when the total count is odd (average of the two middle elements when even). You must not merge the arrays into a new vector; instead, implement an efficient algorithm that uses binary search on the smaller array to find the correct partition point. Assume both arrays are non-empty and may have different sizes. The function should handle cases where one array is entirely smaller than the other, and also when one array is much larger than the other.

The core idea is to partition both arrays into a left half and right half such that all elements in the left half are less than or equal to all elements in the right half, and the left half contains exactly `(n+m+1)/2` elements (the first half). Since the arrays are sorted, we perform binary search on the smaller array (for efficiency) to select how many elements from that array go to the left half. For each trial partition index `i` (number of elements taken from `a`), we compute `j = (n+m+1)/2 - i` as the number of elements from `b` in the left half. We then check the four boundary conditions: `a[i-1] <= b[j]` and `b[j-1] <= a[i]` (with proper handling of indices out of range). If the condition fails, we adjust the binary search range: if `a[i-1] > b[j]`, we need to take fewer from `a` (move left); otherwise take more. Once a valid partition is found, the median is:
- If total count is odd: `max(a[i-1], b[j-1])`
- If even: `(max(a[i-1], b[j-1]) + min(a[i], b[j])) / 2.0`
Edge cases include when one array is empty (but problem says non-empty), and when the partition index hits 0 or the full length of an array (use `-inf` and `+inf` as sentinels). Time complexity is `O(log(min(n,m)))` and space `O(1)`. Must handle integer overflow by using `long long` for the median sum when even.

#include <vector>
#include <algorithm>
#include <cstdint>

// Compute the median of two sorted arrays without merging.
// a, b: sorted arrays (strictly increasing), n, m: their sizes.
// Returns double median.
double arrayMedian(const std::vector<int>& a, const std::vector<int>& b) {
    const int n = static_cast<int>(a.size());
    const int m = static_cast<int>(b.size());
    
    // Ensure a is the smaller array for binary search.
    if (n > m) {
        return arrayMedian(b, a);
    }
    
    const int totalLeft = (n + m + 1) / 2;
    int low = 0, high = n;
    
    while (low <= high) {
        int i = low + (high - low) / 2; // elements taken from a
        int j = totalLeft - i;          // elements taken from b
        
        // Boundary values: default to -inf / +inf when out of range.
        int aLeft = (i > 0) ? a[i - 1] : INT32_MIN;
        int aRight = (i < n) ? a[i] : INT32_MAX;
        int bLeft = (j > 0) ? b[j - 1] : INT32_MIN;
        int bRight = (j < m) ? b[j] : INT32_MAX;
        
        if (aLeft <= bRight && bLeft <= aRight) {
            // Valid partition found.
            if ((n + m) % 2 == 1) {
                return static_cast<double>(std::max(aLeft, bLeft));
            } else {
                double leftMax = static_cast<double>(std::max(aLeft, bLeft));
                double rightMin = static_cast<double>(std::min(aRight, bRight));
                return (leftMax + rightMin) / 2.0;
            }
        } else if (aLeft > bRight) {
            // Too many from a, move left.
            high = i - 1;
        } else {
            // Too few from a, move right.
            low = i + 1;
        }
    }
    
    // Should never reach here for valid input.
    return 0.0;
}

#include <cassert>
#include <vector>

// (The solution function is assumed to be included above.)

int main() {
    // Basic odd total
    std::vector<int> a1 = {1, 3};
    std::vector<int> b1 = {2};
    assert(arrayMedian(a1, b1) == 2.0);

    // Basic even total
    std::vector<int> a2 = {1, 2};
    std::vector<int> b2 = {3, 4};
    assert(arrayMedian(a2, b2) == 2.5);

    // One array entirely smaller
    std::vector<int> a3 = {1, 2};
    std::vector<int> b3 = {3, 4, 5, 6};
    assert(arrayMedian(a3, b3) == 3.5);

    // Duplicate values across arrays (allowed, but each array internally unique)
    std::vector<int> a4 = {1, 2, 2};
    std::vector<int> b4 = {2, 3};
    // Combined: 1,2,2,2,3 -> median 2
    assert(arrayMedian(a4, b4) == 2.0);

    // Single element each
    std::vector<int> a5 = {5};
    std::vector<int> b5 = {1};
    assert(arrayMedian(a5, b5) == 3.0);

    // Larger test, odd count
    std::vector<int> a6 = {1, 4, 7, 10};
    std::vector<int> b6 = {2, 5, 8, 11, 13};
    // Combined: 1,2,4,5,7,8,10,11,13 -> median 7
    assert(arrayMedian(a6, b6) == 7.0);

    // Larger test, even count
    std::vector<int> a7 = {1, 3, 5};
    std::vector<int> b7 = {2, 4, 6, 8};
    // Combined: 1,2,3,4,5,6,8 -> count 7? Actually 3+4=7, odd, median 4
    // Let's recompute: 1,2,3,4,5,6,8 -> median 4.0
    assert(arrayMedian(a7, b7) == 4.0);

    // Edge: one array much larger, all elements less
    std::vector<int> a8 = {1, 2};
    std::vector<int> b8 = {10, 20, 30, 40, 50};
    // Combined: 1,2,10,20,30,40,50 -> median 20
    assert(arrayMedian(a8, b8) == 20.0);

    // Edge: all equal
    std::vector<int> a9 = {7, 7};
    std::vector<int> b9 = {7, 7, 7};
    // Combined: 7,7,7,7,7,7,7? Actually 2+3=5, all 7s -> median 7
    assert(arrayMedian(a9, b9) == 7.0);

    // Large values to ensure double precision
    std::vector<int> a10 = {1000000000};
    std::vector<int> b10 = {1000000000, 1000000001};
    // Combined: 1e9,1e9,1e9+1 -> median 1e9
    assert(arrayMedian(a10, b10) == 1000000000.0);

    return 0;
}
