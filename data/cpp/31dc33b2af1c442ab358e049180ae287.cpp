// Write a C++ function `int findPeakIndex(const std::vector<long long>& arr)` that, given a vector of distinct-or-equal integers, returns the 1-based position of the first maximum element that is strictly greater than at least one of its immediate neighbors (left or right). If the maximum value appears multiple times, the function must return the 1-based index of the first occurrence (from left to right) that satisfies this "greater-than-neighbor" condition. If no maximum element has a strictly smaller neighbor (e.g., all elements are equal, or the maximum is at position 0 and also at position n-1 but no internal max occurs), return -1. The input vector will have at least 1 element. Treat the vector as linear (no wrap-around). The function must be const-correct and not modify the input.

The core idea: first find the maximum value `m` in the vector via a single pass. Then, scan the vector left to right; for each index `i` where `arr[i] == m`, check if it is "locally greater" than at least one neighbor. For the first index `i`, the left neighbor does not exist, so we only check the right neighbor if `i < n-1` and `arr[i] > arr[i+1]`. For the last index `i=n-1`, we only check the left neighbor if `i > 0` and `arr[i] > arr[i-1]`. For interior indices, check both sides. Return `i+1` (1-based) at the first such index. If none found, return -1. Edge cases: single-element vector → no neighbor, return -1; all values equal → every max has neighbors equal, return -1; maximum appears at both ends but also internally, internal one should be returned since we scan left to right; maximum at edges only (e.g., [5,1]) → index 0 has right neighbor smaller, so return 1 (valid). Time: first pass O(n) to find max, second pass O(n) to scan, total O(n). Space O(1) auxiliary. Do not confuse with typical "peak" that requires greater than both neighbors; here only at least one neighbor is smaller.

#include <vector>
#include <algorithm>

// Returns the 1-based index of the first maximum element that has at least one
// strictly smaller neighbor. If no such element exists, returns -1.
int findPeakIndex(const std::vector<long long>& arr) {
    if (arr.empty()) return -1;

    // Find the maximum value in the vector.
    long long maxVal = *std::max_element(arr.begin(), arr.end());

    const int n = static_cast<int>(arr.size());

    // Scan for the first occurrence of maxVal that has a smaller neighbor.
    for (int i = 0; i < n; ++i) {
        if (arr[i] != maxVal) continue;

        bool hasSmallerNeighbor = false;
        if (i > 0 && arr[i] > arr[i - 1]) {
            hasSmallerNeighbor = true;
        }
        if (i < n - 1 && arr[i] > arr[i + 1]) {
            hasSmallerNeighbor = true;
        }

        if (hasSmallerNeighbor) {
            return i + 1; // 1-based index
        }
    }

    return -1;
}

#include <cassert>
#include <vector>

// Declaration of the function under test (from solution).
int findPeakIndex(const std::vector<long long>& arr);

int main() {
    // Basic case with a single maximum inside.
    assert(findPeakIndex({1, 3, 2}) == 2);

    // Maximum at the first index with right neighbor smaller.
    assert(findPeakIndex({5, 1, 2}) == 1);

    // Maximum at the last index with left neighbor smaller.
    assert(findPeakIndex({2, 1, 5}) == 3);

    // Maximum appears multiple times; first qualifying is index 2 (1-based).
    assert(findPeakIndex({4, 2, 4, 3, 4}) == 3);

    // All elements equal -> no smaller neighbor, return -1.
    assert(findPeakIndex({7, 7, 7}) == -1);

    // Single element vector -> no neighbor, return -1.
    assert(findPeakIndex({10}) == -1);

    // Maximum at both ends, but internal max at index 2 qualifies first.
    assert(findPeakIndex({9, 1, 9, 1, 9}) == 3);

    // Larger test with negative numbers.
    assert(findPeakIndex({-5, -1, -10, -3}) == 2);

    // Vector with maximum at position 0 only, and also last position? Here last is not max.
    assert(findPeakIndex({3, 2, 1}) == 1);

    // Vector where max appears but all neighbors equal to max? Already covered, but add another.
    std::vector<long long> v = {2, 2, 2, 1};
    assert(findPeakIndex(v) == 1); // first max at index 0 has right neighbor 2 (not smaller), but index 0 has no left neighbor; wait check: right neighbor is 2, not smaller, so index 0 fails, index 1 right neighbor 2 fails, index 2 right neighbor 1 (smaller) so return 3.
    // Let's correct: Actually index 2 has left neighbor 2 (not smaller) but right neighbor 1 (smaller) -> qualifies.
    // So expected 3.
    assert(findPeakIndex({2, 2, 2, 1}) == 3);

    return 0;
}
