// Write a C++ function `firstNegativeInWindows` that takes an array of long long integers, its length `N`, and a window size `K` (where `1 <= K <= N`). For each contiguous subarray (window) of size `K` starting at index `0, 1, ..., N-K`, the function must return a `vector<long long>` where each element is the first (leftmost) negative integer in that window. If a window contains no negative integer, output `0` for that window. The function must be efficient, avoiding a naive re-scan of each window, and must handle edge cases such as all-positive arrays, all-negative arrays, and a single-element array.
// The solution uses a sliding window approach with a deque (double-ended queue) to track indices of negative numbers within the current window. First, initialize the deque by scanning the first `K` elements; if an element is negative, push its index to the back of the deque. The first window’s result is simply the value at the front of the deque if it is non-empty, otherwise `0`. Then slide the window one position at a time from `i = K` to `N-1`. At each step: remove indices from the front of the deque if they are no longer in the current window (i.e., `i - dq.front() >= K`). Then, if the new element `A[i]` is negative, push its index to the back. The current window’s first negative is the value at the front of the deque if non-empty, else `0`. This maintains order because the deque stores indices in increasing order, and the front always corresponds to the earliest (leftmost) negative in the window. Edge cases: if no negatives exist at all, all results are `0`; if `K == N`, only one result is produced; if `K == 1`, each result is the element itself if negative, otherwise `0`. Time complexity is \(O(N)\) because each index is pushed and popped at most once. Space complexity is \(O(K)\) for the deque (worst case all indices in window are negative).
#include <vector>
#include <deque>

// Given an array of long long integers, return the first negative integer in each
// contiguous window of size K. If a window has no negative integer, use 0.
std::vector<long long> firstNegativeInWindows(const long long arr[], long long N, long long K) {
    std::deque<long long> negativeIndices;
    std::vector<long long> result;

    // Process the first window
    for (long long i = 0; i < K; ++i) {
        if (arr[i] < 0) {
            negativeIndices.push_back(i);
        }
    }
    result.push_back(negativeIndices.empty() ? 0 : arr[negativeIndices.front()]);

    // Slide the window
    for (long long i = K; i < N; ++i) {
        // Remove indices that are out of the current window
        if (!negativeIndices.empty() && (i - negativeIndices.front() >= K)) {
            negativeIndices.pop_front();
        }
        // Add the new element if negative
        if (arr[i] < 0) {
            negativeIndices.push_back(i);
        }
        // First negative in the current window
        result.push_back(negativeIndices.empty() ? 0 : arr[negativeIndices.front()]);
    }

    return result;
}
#include <cassert>
#include <vector>

// Declaration of the function under test (assume it is in the same file)
std::vector<long long> firstNegativeInWindows(const long long arr[], long long N, long long K);

int main() {
    // Test 1: Basic mixed negatives and positives
    long long arr1[] = {-1, 2, -3, 4, -5};
    std::vector<long long> res1 = firstNegativeInWindows(arr1, 5, 2);
    assert(res1 == std::vector<long long>({-1, -3, -3, -5}));

    // Test 2: No negatives anywhere
    long long arr2[] = {1, 2, 3, 4};
    std::vector<long long> res2 = firstNegativeInWindows(arr2, 4, 3);
    assert(res2 == std::vector<long long>({0, 0}));

    // Test 3: All negatives
    long long arr3[] = {-1, -2, -3, -4};
    std::vector<long long> res3 = firstNegativeInWindows(arr3, 4, 2);
    assert(res3 == std::vector<long long>({-1, -2, -3}));

    // Test 4: Single-element array with negative (K = 1)
    long long arr4[] = {-7};
    std::vector<long long> res4 = firstNegativeInWindows(arr4, 1, 1);
    assert(res4 == std::vector<long long>({-7}));

    // Test 5: Single-element array with positive (K = 1)
    long long arr5[] = {9};
    std::vector<long long> res5 = firstNegativeInWindows(arr5, 1, 1);
    assert(res5 == std::vector<long long>({0}));

    // Test 6: Window size equals array size
    long long arr6[] = {5, -1, 2, -3};
    std::vector<long long> res6 = firstNegativeInWindows(arr6, 4, 4);
    assert(res6 == std::vector<long long>({-1}));

    // Test 7: Large values and long long precision
    long long arr7[] = {1000000000000LL, -999999999999LL, 1};
    std::vector<long long> res7 = firstNegativeInWindows(arr7, 3, 2);
    assert(res7 == std::vector<long long>({-999999999999LL, -999999999999LL}));

    // Test 8: Negative appears only later in windows
    long long arr8[] = {1, 2, -1, 3};
    std::vector<long long> res8 = firstNegativeInWindows(arr8, 4, 3);
    assert(res8 == std::vector<long long>({0, -1}));

    return 0;
}
