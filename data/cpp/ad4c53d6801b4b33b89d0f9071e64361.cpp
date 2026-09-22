// Given a non-empty vector of 32-bit signed integers, write a C++ function that returns the minimum total number of moves required to make all elements equal, where each move increments or decrements any one element by exactly 1. The input vector may contain negative numbers, duplicates, and be of any size from 1 to 100,000. The result is guaranteed to fit in a 64-bit signed integer. Do not modify the input vector.
#include <cassert>

int main() {
    // Single element: no moves needed
    assert(minMovesToEqualize({5}) == 0);

    // Already equal
    assert(minMovesToEqualize({7, 7, 7}) == 0);

    // Simple case: median is 2, distances |1-2|+|2-2|+|3-2| = 1+0+1 = 2
    assert(minMovesToEqualize({1, 2, 3}) == 2);

    // Even length: median upper at index 2 (value 3), distances |1-3|+|3-3|+|3-3|+|5-3| = 2+0+0+2 = 4
    assert(minMovesToEqualize({1, 3, 3, 5}) == 4);

    // Negative numbers and duplicates: median is 0, distances | -5-0|+|0-0|+|0-0|+|5-0| = 5+0+0+5 = 10
    assert(minMovesToEqualize({-5, 0, 0, 5}) == 10);

    // Large numbers: median 1000000, sum = 999999+0+999999 = 1999998
    assert(minMovesToEqualize({1, 1000000, 1999999}) == 1999998);

    // Unsorted input with negatives
    assert(minMovesToEqualize({3, -2, 0, 5, 1}) == 10); // sorted: -2,0,1,3,5 median=1, sum=3+1+0+2+4=10

    // All same negative
    assert(minMovesToEqualize({-4, -4, -4}) == 0);

    // Two elements: any midpoint equal? For {0,10} median upper=10, sum=10+0=10; For lower median=0 also gives 10.
    assert(minMovesToEqualize({0, 10}) == 10);

    return 0;
}
#include <vector>
#include <algorithm>
#include <cstdlib>  // for std::llabs

// Return the minimum number of increment/decrement moves to make all elements equal.
long long minMovesToEqualize(const std::vector<int>& nums) {
    if (nums.size() <= 1) return 0;

    // Work on a copy so the input vector is not modified.
    std::vector<int> sorted = nums;
    std::sort(sorted.begin(), sorted.end());

    // The optimal target is a median (upper median for even size).
    int median = sorted[sorted.size() / 2];

    long long totalMoves = 0;
    for (int value : sorted) {
        totalMoves += std::llabs(static_cast<long long>(value) - static_cast<long long>(median));
    }
    return totalMoves;
}
// The optimal equal value for minimizing the sum of absolute deviations is the median of the array, not the mean. This is because the absolute-value loss function is convex and minimized at a median. To find the median, sort a copy of the vector (or sort in-place if permitted, but here we must not modify input, so copy first). For an even-length array, either of the two middle elements works (both yield the same minimal sum); here we choose the element at index `n/2` (which is the upper median). Then sum the absolute differences between each element and that median using a 64-bit accumulator to avoid overflow. Edge cases: single element (sum is 0), all elements already equal (sum is 0), negative numbers (absolute difference works fine), large magnitudes (use `long long`). Time complexity is O(n log n) due to sorting, and O(n) auxiliary space for the copy (or O(1) if we were allowed to sort in place, but here we copy, so O(n)). The summation is O(n).
