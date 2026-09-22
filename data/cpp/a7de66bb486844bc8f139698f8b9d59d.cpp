/*
Write a C++ function `int maxDiamondsInWindow(const std::vector<int>& diamondSizes, int k)` that takes a list of diamond sizes (each between 1 and 10000) and an integer `k` (≥ 0), and returns the maximum number of diamonds that can be collected if you choose a window of size `k+1` (i.e., a contiguous range of sizes from `start` to `start+k`) and count all diamonds whose size falls inside that range. The input vector may contain duplicate sizes, and sizes are not necessarily sorted. For example, if the sizes are `{3, 5, 1, 2, 5, 4}` and `k = 2`, a window covering sizes `2..4` would capture diamonds with sizes 2, 3, and 4 (namely 2, 3, 4 → 3 diamonds), and a window covering `3..5` captures 3, 5, 5, 4 (4 diamonds), so the answer is 4. If `k` is 0, each window is exactly one size, and the answer is the maximum frequency of any size. The function should handle empty input (return 0) and very large `k` (the window may exceed the maximum size present; in that case, the window effectively covers all present sizes, so the answer is the total number of diamonds). The sizes are positive integers; you may assume the maximum size is at most 10000 for efficiency, but your solution should be correct regardless.
*/

#include <vector>
#include <algorithm>

// Returns the maximum number of diamonds that fit in any contiguous size
// window of length (k+1). Diamond sizes are positive integers.
int maxDiamondsInWindow(const std::vector<int>& diamondSizes, int k) {
    if (diamondSizes.empty()) {
        return 0;
    }

    // Determine the maximum size present to limit the frequency array.
    int maxSize = 0;
    for (int size : diamondSizes) {
        maxSize = std::max(maxSize, size);
    }

    // Build frequency array; index 0 unused (sizes are positive).
    std::vector<int> freq(maxSize + 1, 0);
    for (int size : diamondSizes) {
        ++freq[size];
    }

    // Window length is k+1. If the window covers all sizes, answer is total.
    long long windowLen = static_cast<long long>(k) + 1;
    if (windowLen >= maxSize) {
        return static_cast<int>(diamondSizes.size());
    }

    // Initialize the first window: sizes 1 to windowLen (or up to maxSize).
    int currentSum = 0;
    for (int size = 1; size <= static_cast<int>(windowLen) && size <= maxSize; ++size) {
        currentSum += freq[size];
    }

    int maxSum = currentSum;

    // Slide the window one size at a time.
    for (int start = 2; start <= maxSize; ++start) {
        int leavingSize = start - 1;
        int enteringSize = start + static_cast<int>(windowLen) - 1; // start + k
        currentSum -= freq[leavingSize];
        if (enteringSize <= maxSize) {
            currentSum += freq[enteringSize];
        }
        maxSum = std::max(maxSum, currentSum);
    }

    return maxSum;
}

#include <cassert>
#include <vector>

int main() {
    // Basic case from the snippet: sizes 3,5,1,2,5,4, k=2 -> max 4
    std::vector<int> sizes1 = {3, 5, 1, 2, 5, 4};
    assert(maxDiamondsInWindow(sizes1, 2) == 4);

    // k=0: each window is a single size, answer is max frequency (5 appears twice)
    assert(maxDiamondsInWindow(sizes1, 0) == 2);

    // Empty input
    assert(maxDiamondsInWindow({}, 5) == 0);

    // All same size, any k
    std::vector<int> sizes2 = {7, 7, 7, 7};
    assert(maxDiamondsInWindow(sizes2, 0) == 4);
    assert(maxDiamondsInWindow(sizes2, 10) == 4);

    // Large k covers all sizes: total diamonds
    std::vector<int> sizes3 = {1, 10, 5, 3, 8};
    assert(maxDiamondsInWindow(sizes3, 100) == 5);

    // Window of size 1 (k=0) with unique sizes: max frequency 1
    std::vector<int> sizes4 = {2, 4, 6, 8};
    assert(maxDiamondsInWindow(sizes4, 0) == 1);

    // Window spanning exactly over a dense region
    std::vector<int> sizes5 = {2, 2, 3, 3, 3, 4, 4, 4, 4, 10};
    // sizes 2..4 (k=2) counts: 2 appears 2, 3 appears 3, 4 appears 4 -> total 9
    assert(maxDiamondsInWindow(sizes5, 2) == 9);
    // sizes 2..3 (k=1) counts 2+3=5; 3..4 counts 3+4=7; 4..5 counts 4; but 3..4 gives 7
    assert(maxDiamondsInWindow(sizes5, 1) == 7);

    // Single diamond
    std::vector<int> sizes6 = {42};
    assert(maxDiamondsInWindow(sizes6, 0) == 1);
    assert(maxDiamondsInWindow(sizes6, 3) == 1);

    return 0;
}

// The given code uses a frequency array indexed by diamond size, then slides a window of size `k+1` over the possible size values from 1 upward, summing frequencies in each window and tracking the maximum sum. The key observation is that the problem is equivalent to finding the maximum sum of any contiguous subarray of length `k+1` in the frequency array, where the array is indexed from 1 to `max_size`. To generalize the snippet into a reusable function, we first build a frequency count of each size using a `std::vector<int>` of size `maxSize+1` (or up to 10000). Then we compute a sliding-window sum: initialize the sum of the first window (sizes 1 to `min(k+1, maxSize)`), then move the window one size at a time, subtracting the frequency that leaves the window and adding the frequency that enters, all while tracking the maximum sum. Edge cases: (1) empty input → return 0. (2) `k` is very large → the window covers all sizes, so the sum is the total count. (3) `k=0` → window length 1, so we return the maximum frequency. (4) The window may start beyond `maxSize`? Actually we only iterate starting positions from 1 up to `maxSize` because a window starting beyond the largest size yields zero sum. We also handle the case where the right boundary exceeds `maxSize` by simply not adding beyond the array (the window sum naturally stops including absent sizes). Time complexity is O(n + maxSize) for building the frequency array and O(maxSize) for the sliding window, so overall O(n + maxSize) where n is the number of diamonds. Space complexity is O(maxSize) for the frequency array, which we can limit to 10001 as given.
