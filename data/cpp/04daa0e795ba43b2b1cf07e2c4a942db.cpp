// Write a C++ function `long long minimumTotalDistance(const std::vector<int>& values)` that, given a non-empty vector of integers, returns the minimum possible sum of absolute differences between all elements of the vector and some single integer value `x`. The integer `x` does not need to be present in the vector and can be any integer (positive, negative, or zero). The function must compute the optimal choice of `x` that minimizes the sum of `|value[i] - x|` for all elements, and return that minimal sum. Assume the input vector size may be large (up to 10^5 elements) and each element may be an integer in the range `[-10^9, 10^9]`.

#include <cassert>
#include <vector>

// The solution function is declared above; include it here.

int main() {
    // Single element: distance to itself is 0.
    assert(minimumTotalDistance({5}) == 0);

    // Two elements: any x between them gives sum = difference.
    assert(minimumTotalDistance({1, 5}) == 4);
    assert(minimumTotalDistance({-3, 7}) == 10);

    // Three elements: median is middle after sorting.
    assert(minimumTotalDistance({1, 2, 3}) == 2);  // x=2 gives 1+0+1=2
    assert(minimumTotalDistance({10, -5, 0}) == 15); // sorted: -5,0,10, median 0, sum=5+0+10=15

    // Duplicates and negatives.
    assert(minimumTotalDistance({-2, -2, 4}) == 6); // median -2: 0+0+6=6
    assert(minimumTotalDistance({3, 3, 3, 3}) == 0);

    // Even size with negatives.
    assert(minimumTotalDistance({-10, -1, 2, 5}) == 18); // sorted: -10,-1,2,5; lower median -1: 9+0+3+6=18

    // Large values to ensure long long handling.
    std::vector<int> big = {1000000000, -1000000000, 0};
    assert(minimumTotalDistance(big) == 2000000000LL);
}

#include <vector>
#include <algorithm>
#include <cstdlib>

// Given a non-empty vector of integers, returns the minimum possible sum of
// absolute differences between every element and some single integer x.
// The optimal x is the median of the data (lower median for even sizes).
long long minimumTotalDistance(const std::vector<int>& values) {
    // Copy to allow sorting without modifying the input.
    std::vector<int> sorted = values;
    std::sort(sorted.begin(), sorted.end());

    // Choose the lower median.
    int median = sorted[sorted.size() / 2];

    long long total = 0;
    for (int v : sorted) {
        total += std::llabs(static_cast<long long>(v) - median);
    }
    return total;
}

// The classic result for minimizing the sum of absolute deviations is that the optimal `x` is any median of the dataset. For an even number of elements, any value between the two middle elements (inclusive) yields the same minimal sum; using the lower median (the element at index `n/2` after sorting) is simplest and valid. First, sort the input vector. Then, choose the median `m = sorted[n/2]`. Compute the sum `∑ |sorted[i] - m|` for all `i`. This sum is the minimal total absolute difference. Edge cases include a single-element vector (sum is 0), vectors with duplicated values, and negative numbers—sorting and median selection handle all correctly. Time complexity is `O(n log n)` due to sorting, and space complexity is `O(1)` auxiliary (if we mutate a copy) or `O(n)` if we copy the input before sorting. Since the problem statement expects a function that does not modify the original, we make a local copy and sort it.
