Given an array of integers `a` of length `n` and a positive integer `k`, write a C++ function that computes the maximum subarray sum when the array is repeated `k` times consecutively. The repeated array has length `n * k`. The subarray must be contiguous within this repeated sequence, and it may wrap around from the end of one copy to the beginning of the next. The array may contain negative numbers, and the maximum subarray sum could be negative if all elements are negative. Return the maximum possible sum as an `int`. You may assume `n >= 1` and `k >= 1`. The function should be robust for large `k` values, but the straightforward O(n*k) simulation is acceptable for this task.
The core problem is an extension of Kadane's algorithm. The naive approach is to simulate the repeated array by iterating `i` from 0 to `n*k - 1` and accessing `a[i % n]`. Maintain `max_ending_here` (the maximum sum of a subarray ending at the current position) and `max_so_far` (the global maximum). For each element, add it to `max_ending_here`. If `max_ending_here` exceeds `max_so_far`, update `max_so_far`. If `max_ending_here` becomes negative, reset it to 0, because starting a new subarray from the next element is always better than continuing a negative prefix. This is exactly Kadane's algorithm on the repeated sequence. Edge cases include: all negative numbers (then `max_so_far` will be the largest negative number, because the reset to 0 means we never take a negative `max_ending_here`, but we still capture each single element as a candidate when we add it before the reset); `n=1` or `k=1`; and possibly large `k` causing overflow. While the direct O(n*k) loop is correct, note that for very large `k` (e.g., 10^9) it would be too slow, so a more advanced approach using prefix sums or observing that the answer is either max subarray in one copy, total sum times something plus borders, etc., could be discussed. However, the provided snippet uses the direct loop, so the task specification accepts that. Time complexity is O(n*k), space is O(1) auxiliary.
#include <vector>
#include <algorithm>
#include <limits>

// Returns the maximum contiguous subarray sum of the array formed by
// repeating the input array 'a' of length 'n' exactly 'k' times.
int maxSubarraySumInRepeatedArray(const std::vector<int>& a, int n, int k) {
    int max_so_far = std::numeric_limits<int>::min();
    int max_ending_here = 0;

    for (int i = 0; i < n * k; ++i) {
        max_ending_here += a[i % n];
        if (max_so_far < max_ending_here) {
            max_so_far = max_ending_here;
        }
        if (max_ending_here < 0) {
            max_ending_here = 0;
        }
    }
    return max_so_far;
}
#include <cassert>
#include <vector>
#include <iostream>

// Assume the solution function is declared above.
int maxSubarraySumInRepeatedArray(const std::vector<int>& a, int n, int k);

int main() {
    // Basic case: array [1, 2, 3], repeated 2 times -> [1,2,3,1,2,3], max sum is 12 (entire array)
    assert(maxSubarraySumInRepeatedArray({1,2,3}, 3, 2) == 12);

    // All negative numbers: [-2, -3], repeated 3 times -> [-2,-3,-2,-3,-2,-3], max is -2
    assert(maxSubarraySumInRepeatedArray({-2,-3}, 2, 3) == -2);

    // Single element repeated: [5], repeated 4 times -> [5,5,5,5], max sum is 20
    assert(maxSubarraySumInRepeatedArray({5}, 1, 4) == 20);

    // Wrap-around benefit: [5, -2, 5], repeated 2 times -> [5,-2,5,5,-2,5], max is 13 (subarray indices 2..5)
    assert(maxSubarraySumInRepeatedArray({5,-2,5}, 3, 2) == 13);

    // k=1 reduces to standard Kadane: [1,-2,3,4], max is 7
    assert(maxSubarraySumInRepeatedArray({1,-2,3,4}, 4, 1) == 7);

    // Mixed with negatives and positives, repeated: [2, -1, 2], k=3, max sum is 7 (from index 2 to 8? Actually [2, -1, 2, 2, -1, 2, 2, -1, 2] -> best subarray is whole array? Let's compute: 2-1+2+2-1+2+2-1+2 = 9, but is that allowed? Yes contiguous, sum=9. But simpler: choose [2,2,2]? But those are not contiguous. The correct max is 9.
    assert(maxSubarraySumInRepeatedArray({2,-1,2}, 3, 3) == 9);

    // Large k with positive total sum: [1,2], k=5 -> [1,2,1,2,1,2,1,2,1,2], max is 15 (all)
    assert(maxSubarraySumInRepeatedArray({1,2}, 2, 5) == 15);

    // Zero and negative mix: [0, -1, 0], k=2 -> max is 0 (either single 0 or empty? But subarray must be non-empty, so max is 0)
    assert(maxSubarraySumInRepeatedArray({0,-1,0}, 3, 2) == 0);

    // Edge: n=1, k=1, negative: [-7] -> -7
    assert(maxSubarraySumInRepeatedArray({-7}, 1, 1) == -7);

    // Edge: n=1, k=large (but keep small for test): [0], k=5 -> 0
    assert(maxSubarraySumInRepeatedArray({0}, 1, 5) == 0);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
