Write a C++ function `int findPeakIndex(const std::vector<int>& arr)` that takes a non-empty vector of integers (indices considered 1-based for clarity) and returns the 1-based index of any element that is equal to the maximum value in the array AND is strictly greater than at least one of its adjacent neighbors (i.e., it is a "peak" among the maxima). If no such element exists (e.g., all elements are equal, or the maximum appears only in a flat region where all equal values have equal neighbors), return -1. For boundary elements, only consider the single existing neighbor. The function must handle vectors of size up to 300,000, and the input values can be any integers fitting in `int`.
// The core idea is to first find the maximum value in the array in a single pass. Then, iterate through the array again, checking each position where the current element equals that global maximum. For such positions, test whether it is strictly greater than its left neighbor (if i > 1) or strictly greater than its right neighbor (if i < n). If either condition holds, that index is a valid answer; we can return the first such index found, or more specifically, the last one found if we want to match typical output patterns, but the task says "any", so returning the first is acceptable. Important edge cases: (1) If the array has only one element, there are no neighbors, so no peak exists, return -1. (2) If all elements are equal, no element is strictly greater than any neighbor, return -1. (3) The maximum might appear multiple times; we only need to find one position that satisfies the peak condition. Time complexity is O(n) with two passes (or one pass if we track max and candidate simultaneously, but simpler is two passes). Space complexity is O(1) extra aside from input storage.
#include <vector>
#include <algorithm>

// Returns the 1-based index of an element that equals the array's maximum
// and is strictly greater than at least one adjacent neighbor.
// Returns -1 if no such element exists.
int findPeakIndex(const std::vector<int>& arr) {
    int n = static_cast<int>(arr.size());
    if (n <= 1) {
        return -1;
    }

    int max_val = *std::max_element(arr.begin(), arr.end());

    for (int i = 0; i < n; ++i) {
        if (arr[i] == max_val) {
            bool is_peak = false;
            if (i > 0 && arr[i] > arr[i - 1]) {
                is_peak = true;
            }
            if (i < n - 1 && arr[i] > arr[i + 1]) {
                is_peak = true;
            }
            if (is_peak) {
                return i + 1; // convert to 1-based
            }
        }
    }
    return -1;
}
#include <cassert>
#include <vector>

// Function declaration (already defined above)
int findPeakIndex(const std::vector<int>& arr);

int main() {
    // Basic case: max appears at position 2 and is greater than left neighbor
    std::vector<int> a1 = {1, 5, 3};
    assert(findPeakIndex(a1) == 2);

    // Max appears at edge, greater than only neighbor
    std::vector<int> a2 = {9, 1, 2};
    assert(findPeakIndex(a2) == 1);

    // All equal -> no peak
    std::vector<int> a3 = {4, 4, 4};
    assert(findPeakIndex(a3) == -1);

    // Single element -> no neighbors
    std::vector<int> a4 = {7};
    assert(findPeakIndex(a4) == -1);

    // Multiple maxima, but only one is a peak
    std::vector<int> a5 = {2, 6, 6, 1};
    // index 3 (1-based) is 6 greater than right neighbor 1, also index 2 has left neighbor 2 and right neighbor 6 not greater, but index 2 is not greater than left (2<6) nor right (6==6), so index 3 is the only peak
    assert(findPeakIndex(a5) == 3);

    // Max at both ends and both are peaks, returns first encountered
    std::vector<int> a6 = {8, 1, 8};
    // index 1 is greater than right 1 -> returns 1
    assert(findPeakIndex(a6) == 1);

    // Negative numbers
    std::vector<int> a7 = {-5, -1, -2};
    assert(findPeakIndex(a7) == 2);

    // Large vector test (size 300000) with single peak at end
    std::vector<int> a8(300000, 0);
    a8.back() = 5; // last element is max, only neighbor is left (0), so peak
    assert(findPeakIndex(a8) == 300000);

    // Max appears but only in flat region with no greater-than-neighbor
    std::vector<int> a9 = {3, 5, 5, 5, 1};
    // middle element at position 3 is 5, left neighbor 5, right neighbor 5 -> not greater, but position 2 (1-based) has left 3 -> peak, position 4 has right 1 -> peak, our function returns first (position 2)
    assert(findPeakIndex(a9) == 2);

    return 0;
}
