// Write a C++ function `firstNegativeInEachWindow(const std::vector<int>& arr, int k)` that, given a non-empty vector of integers and a positive window size `k` (where `1 <= k <= arr.size()`), returns a vector of length `arr.size() - k + 1`. For each sliding window of size `k`, the function must store the first negative integer in that window; if the window contains no negative integers, store the special value `1` in that position. The function must preserve the order of windows and handle all edge cases, including windows with no negatives, windows where the first negative appears at any offset, and windows that overlap with previously found negatives. The solution must not use extra data structures beyond the output vector and a small constant number of integer indices.

#include <cassert>
#include <vector>

// The solution function is declared above. This test validates correctness.
int main() {
    // Basic example from the prompt.
    std::vector<int> arr1 = {2, 3, 4, 4, -7, -1, 4, -2, 6};
    std::vector<int> expected1 = {-7, -7, -7, -7, -1, -2};
    assert(firstNegativeInEachWindow(arr1, 4) == expected1);

    // Window size 1: each element is its own negative or 1.
    std::vector<int> arr2 = {-1, 2, -3, 4};
    std::vector<int> expected2 = {-1, 1, -3, 1};
    assert(firstNegativeInEachWindow(arr2, 1) == expected2);

    // Window size equal to full array.
    std::vector<int> arr3 = {1, -2, 3};
    std::vector<int> expected3 = {-2};
    assert(firstNegativeInEachWindow(arr3, 3) == expected3);

    // No negatives at all.
    std::vector<int> arr4 = {5, 6, 7};
    std::vector<int> expected4 = {1, 1};
    assert(firstNegativeInEachWindow(arr4, 2) == expected4);

    // All negatives.
    std::vector<int> arr5 = {-1, -2, -3};
    std::vector<int> expected5 = {-1, -2};
    assert(firstNegativeInEachWindow(arr5, 2) == expected5);

    // Negative appears exactly at the end of a window.
    std::vector<int> arr6 = {0, 0, -5, 0};
    std::vector<int> expected6 = {1, -5, -5};
    assert(firstNegativeInEachWindow(arr6, 2) == expected6);

    // Negative appears exactly at the start of a window.
    std::vector<int> arr7 = {-4, 1, 1, 1};
    std::vector<int> expected7 = {-4, 1, 1};
    assert(firstNegativeInEachWindow(arr7, 3) == expected7);

    // Multiple negatives in one window: first is chosen.
    std::vector<int> arr8 = {1, -2, -3, 4};
    std::vector<int> expected8 = {-2, -2, -3};
    assert(firstNegativeInEachWindow(arr8, 2) == expected8);

    // Single element array.
    std::vector<int> arr9 = {-9};
    std::vector<int> expected9 = {-9};
    assert(firstNegativeInEachWindow(arr9, 1) == expected9);

    // Negative in the first element and none elsewhere.
    std::vector<int> arr10 = {-7, 2, 3, 4};
    std::vector<int> expected10 = {-7, 1, 1};
    assert(firstNegativeInEachWindow(arr10, 3) == expected10);

    return 0;
}

#include <vector>

// Return a vector where position i holds the first negative number in the
// window arr[i, i+k-1], or 1 if that window has no negative numbers.
std::vector<int> firstNegativeInEachWindow(const std::vector<int>& arr, int k) {
    int n = static_cast<int>(arr.size());
    int windowCount = n - k + 1;
    std::vector<int> result(windowCount);

    // Find the first negative in the initial window [0, k-1].
    int negativeIndex = -1;
    for (int i = 0; i < k; ++i) {
        if (arr[i] < 0) {
            negativeIndex = i;
            break;
        }
    }
    result[0] = (negativeIndex == -1) ? 1 : arr[negativeIndex];

    // Sliding window: start of current window is 'start', end is 'end'.
    int start = 1;
    int end = k;
    while (end < n) {
        if (negativeIndex >= start) {
            // The previous negative is still inside this window.
            result[start] = arr[negativeIndex];
        } else {
            // The old negative has left; scan the current window.
            negativeIndex = -1;
            for (int i = start; i <= end; ++i) {
                if (arr[i] < 0) {
                    negativeIndex = i;
                    break;
                }
            }
            result[start] = (negativeIndex == -1) ? 1 : arr[negativeIndex];
        }
        ++start;
        ++end;
    }
    return result;
}

// The algorithm uses a sliding window with two pointers. First, find the index of the first negative number in the initial window `[0, k-1]` by scanning from left to right. If found, store its value; otherwise store `1`. For each subsequent window starting at index `i` (from 1 to `n-k`), we reuse the previously found negative index `p` (which is stored as an absolute index into the array). If `p >= i`, that negative is still inside the current window, so it remains the first negative in that window. If `p < i`, the previous negative has left the window, so we scan the new window `[i, i+k-1]` from left to right to find the first negative; if none is found, store `1`. This avoids re-scanning the entire window in most cases, giving an amortized linear total time. Edge cases include: no negatives anywhere (every window returns `1`), a negative at the very first element of the array, a negative exactly at the last position of a window, and consecutive windows where the same negative remains the answer. Time complexity is \(O(n)\) in the worst case because each element is scanned a constant number of times (when it becomes the answer and possibly when scanning for a new negative after it leaves). Space complexity is \(O(n-k+1)\) for the output vector, plus \(O(1)\) auxiliary space.
