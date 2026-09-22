// Write a C++ function that, given a vector of integers forming a "mountain array" (first strictly increasing to a peak value, then strictly decreasing), returns the index of the peak element. The vector will have at least 3 elements, and the peak is guaranteed to exist and be unique (no equal adjacent elements). The function must run in O(log n) time using a binary search, and must not modify the input vector.
// The key observation is that the mountain array has a well-defined "slope": for indices before the peak, `arr[i] < arr[i+1]` (upward slope), and after the peak, `arr[i] > arr[i+1]` (downward slope). We use binary search on the index range [0, n-1]. At each step, compute the middle index `mid`. If `arr[mid] < arr[mid+1]`, then the peak must be to the right of `mid` (because the slope is still upward), so set the lower bound to `mid+1`. Otherwise (i.e., `arr[mid] > arr[mid+1]`), the peak is at `mid` or to its left, so set the upper bound to `mid`. When the loop ends (lower bound equals upper bound), that index is the peak. This works because the array is strictly decreasing after the peak, so the condition `arr[mid] > arr[mid+1]` first becomes true exactly at the peak. Edge cases: the peak can be at index 0 or n-1? No, because the problem guarantees a mountain (at least one up and one down), so the peak is strictly between 0 and n-1. The loop will correctly handle any n >= 3. Time complexity: O(log n) because we halve the search range each iteration. Space complexity: O(1) extra space (only a few integer variables). The function should take `const vector<int>&` to avoid copying and respect const-correctness.
#include <vector>

// Returns the index of the peak element in a mountain array.
// Precondition: arr is a mountain array (strictly increasing, then strictly decreasing).
// The function runs in O(log n) time and O(1) extra space.
int peakIndexInMountainArray(const std::vector<int>& arr) {
    int low = 0;
    int high = static_cast<int>(arr.size()) - 1;

    while (low < high) {
        int mid = low + (high - low) / 2;
        // If the slope is upward, the peak is to the right of mid.
        if (arr[mid] < arr[mid + 1]) {
            low = mid + 1;
        } else {
            // Otherwise, the peak is at mid or to its left.
            high = mid;
        }
    }
    return low; // low == high is the peak index.
}
#include <cassert>
#include <vector>

// Declaration of the solution function (assume it is defined elsewhere in this file).
int peakIndexInMountainArray(const std::vector<int>& arr);

int main() {
    // Standard mountain: 0,1,0 -> peak at index 1
    assert(peakIndexInMountainArray({0, 1, 0}) == 1);

    // Larger mountain: [0,2,1,0] -> peak at index 1 (2)
    assert(peakIndexInMountainArray({0, 2, 1, 0}) == 1);

    // Peak at the rightmost allowed position (not the last element because must descend)
    // e.g., [1,3,5,4,2] -> peak at index 2 (5)
    assert(peakIndexInMountainArray({1, 3, 5, 4, 2}) == 2);

    // Peak near the left: [1,0,-1] -> peak at index 0 (but this is not valid since must increase first)
    // So use [1,2,1] -> peak at index 1
    assert(peakIndexInMountainArray({1, 2, 1}) == 1);

    // Longer mountain with clear peak: [0,2,4,6,5,3,1] -> peak at index 3 (6)
    assert(peakIndexInMountainArray({0, 2, 4, 6, 5, 3, 1}) == 3);

    // Even-length mountain: [1,4,3,2] -> peak at index 1 (4)
    assert(peakIndexInMountainArray({1, 4, 3, 2}) == 1);

    // Peak with non-consecutive values: [0,10,9,8,7] -> peak at index 1 (10)
    assert(peakIndexInMountainArray({0, 10, 9, 8, 7}) == 1);

    // Minimal mountain with 3 elements: [-2, 5, -3] -> peak at index 1
    assert(peakIndexInMountainArray({-2, 5, -3}) == 1);

    // All positive: [1,3,2] -> peak at index 1
    assert(peakIndexInMountainArray({1, 3, 2}) == 1);

    // Large values: [100,200,150,50] -> peak at index 1
    assert(peakIndexInMountainArray({100, 200, 150, 50}) == 1);

    return 0;
}
