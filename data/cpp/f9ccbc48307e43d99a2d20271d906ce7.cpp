// Write a C++ function named `maximumGap` that takes a non-empty vector of integers and returns the maximum difference between two adjacent elements after sorting the array in ascending order. If the vector has fewer than 2 elements, or if all elements are equal, the function must return 0. The function must not modify the input vector, and it must achieve this in linear time complexity (no full sort), handling negative numbers, duplicates, and very large positive/negative values safely (avoid integer overflow in intermediate calculations). The result is the largest gap between consecutive values in the sorted order.
The classic linear-time solution uses the concept of bucketization (also known as the pigeonhole principle). Given `n` numbers with minimum `min` and maximum `max`, the maximum possible gap is at least `ceil((max - min) / (n - 1))`, but we do not need that bound directly. Instead, we create `n - 1` buckets covering the range `(min, max]`, with each bucket width `len = ceil((max - min) / (n - 1))`. Because there are `n` numbers and `n - 1` buckets, at least one bucket will be empty, so the maximum gap cannot occur between two numbers inside the same bucket; it must occur between the maximum of one bucket and the minimum of the next non-empty bucket. For each bucket, we track only the minimum and maximum values that fall into it, plus a flag indicating whether that bucket is used. We iterate through the numbers once to fill buckets (skipping the minimum itself, since it is the left boundary and not in any bucket), then iterate through buckets to compute the maximum difference between the previous bucket’s maximum and the current bucket’s minimum. Edge cases: if `n < 2` or all elements are equal, return 0. The bucket index calculation uses the formula `(x - min - 1) / len` to avoid including the minimum in a bucket. To prevent overflow, compute `len` using a 64-bit intermediate or using the expression `(max - min + n - 2) / (n - 1)` carefully, ensuring the subtraction doesn’t overflow by using `long long` where needed. Time complexity is O(n), space complexity is O(n) for the bucket array.
#include <vector>
#include <limits>
#include <algorithm>

// Returns the maximum difference between adjacent elements in sorted order.
// Requires: nums is non-empty.
int maximumGap(const std::vector<int>& nums) {
    const int n = static_cast<int>(nums.size());
    if (n < 2) return 0;

    int minVal = std::numeric_limits<int>::max();
    int maxVal = std::numeric_limits<int>::min();
    for (int x : nums) {
        minVal = std::min(minVal, x);
        maxVal = std::max(maxVal, x);
    }
    if (minVal == maxVal) return 0;  // all equal

    // Number of buckets = n-1, each representing an interval (min + (k)*len, min + (k+1)*len]
    // Use long long to avoid overflow when computing the range.
    long long range = static_cast<long long>(maxVal) - minVal;
    long long len = (range + n - 2) / (n - 1);  // ceil division, len > 0

    struct Bucket {
        int bucketMin;
        int bucketMax;
        bool used;
        Bucket() : bucketMin(std::numeric_limits<int>::max()),
                   bucketMax(std::numeric_limits<int>::min()),
                   used(false) {}
    };

    std::vector<Bucket> buckets(n - 1);
    for (int x : nums) {
        if (x == minVal) continue;  // min is excluded from all buckets (left-open intervals)
        // Compute bucket index: floor((x - minVal - 1) / len)
        int idx = static_cast<int>((static_cast<long long>(x) - minVal - 1) / len);
        buckets[idx].used = true;
        buckets[idx].bucketMin = std::min(buckets[idx].bucketMin, x);
        buckets[idx].bucketMax = std::max(buckets[idx].bucketMax, x);
    }

    int maxGap = 0;
    int previousMax = minVal;  // start with the overall minimum as the left boundary
    for (int i = 0; i < n - 1; ++i) {
        if (buckets[i].used) {
            maxGap = std::max(maxGap, buckets[i].bucketMin - previousMax);
            previousMax = buckets[i].bucketMax;
        }
    }
    return maxGap;
}
#include <cassert>
#include <vector>

// Assume maximumGap is defined above.

int main() {
    // Basic test from the original problem
    assert(maximumGap({3, 6, 9, 1}) == 3);

    // Single element
    assert(maximumGap({5}) == 0);

    // Two elements
    assert(maximumGap({1, 10}) == 9);

    // All equal
    assert(maximumGap({4, 4, 4}) == 0);

    // Negative numbers
    assert(maximumGap({-3, -10, -1, -5}) == 5);  // sorted: -10,-5,-3,-1, gaps:5,2,2 => max=5

    // Duplicate values
    assert(maximumGap({1, 3, 3, 7}) == 4);  // sorted: 1,3,3,7 gaps:2,0,4 => max=4

    // Large range and n=5: 1, 2, 100, 101, 1000 → gaps:1,98,1,899 → max=899
    assert(maximumGap({1, 2, 100, 101, 1000}) == 899);

    // Random order with negatives and zero
    assert(maximumGap({-5, -2, 0, 8, 12}) == 8);  // sorted: -5,-2,0,8,12 gaps:3,2,8,4 => max=8

    // Large values close together, no overflow
    assert(maximumGap({2147483647, 2147483646, 2147483645}) == 1);

    // Two distinct values only
    assert(maximumGap({0, 100}) == 100);

    // Already sorted with max gap at the end
    assert(maximumGap({1, 5, 6, 20}) == 14);

    return 0;
}
