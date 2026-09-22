Write a C++ function that, given a vector of integers that first increases strictly and then decreases strictly (forming a single peak, with no duplicates and at least three elements), returns the index of the peak element. The peak is the only element that is greater than both its immediate neighbors. The function must handle the case where the vector is already sorted in increasing order (then the peak is the last element) or decreasing order (then the peak is the first element), but the input is guaranteed to be a valid mountain array in the sense that it strictly increases then strictly decreases (the two slopes intersect at one peak). Use binary search for efficiency. The function should be named `peakIndex` and accept a `const std::vector<int>&`.

The array is strictly increasing up to some index `p` and strictly decreasing after `p`. The peak is the only index where both neighbors are smaller. Binary search works by comparing the middle element with its next neighbor:
- If `a[mid] < a[mid+1]`, we are on the increasing slope, so the peak must be to the right, set `start = mid + 1`.
- Else (i.e., `a[mid] > a[mid+1]`), we are on the decreasing slope (or at the peak), so the peak is at `mid` or to the left, set `end = mid`.
This avoids out-of-bounds access by never checking `a[mid-1]` when `mid` is 0. The loop continues while `start < end`, and when they meet, `start` is the peak index. This works for any mountain array, including edge cases where the peak is at index 0 (strictly decreasing) or at the last index (strictly increasing), because the binary search condition correctly reduces the range to that boundary. Time complexity is O(log n), space O(1).

#include <vector>

// Given a strictly increasing then strictly decreasing vector (mountain array),
// return the index of the peak element. The peak is the only element greater
// than both its neighbors.
int peakIndex(const std::vector<int>& a) {
    int start = 0;
    int end = static_cast<int>(a.size()) - 1;
    
    while (start < end) {
        int mid = start + (end - start) / 2;
        if (a[mid] < a[mid + 1]) {
            // On increasing slope, peak is to the right
            start = mid + 1;
        } else {
            // On decreasing slope or at peak, peak is at mid or left
            end = mid;
        }
    }
    return start;
}

#include <cassert>
#include <vector>

int peakIndex(const std::vector<int>& a); // declaration from solution

int main() {
    // Standard mountain array
    assert(peakIndex({10, 20, 50, 40, 30}) == 2);
    // Longer mountain
    assert(peakIndex({10, 20, 30, 90, 70, 60, 50, 40}) == 3);
    // Strictly increasing (peak at last index)
    assert(peakIndex({1, 2, 3, 4, 5}) == 4);
    // Strictly decreasing (peak at first index)
    assert(peakIndex({5, 4, 3, 2, 1}) == 0);
    // Minimal length 3, peak in middle
    assert(peakIndex({1, 3, 2}) == 1);
    // Peak at index 1 with large values
    assert(peakIndex({-5, 100, -6}) == 1);
    // Peak at index 2
    assert(peakIndex({1, 2, 10, 9, 8}) == 2);
    // Negative values mixed
    assert(peakIndex({-10, -5, -1, -2, -3}) == 2);
    // Large vector with peak near start
    assert(peakIndex({0, 100, 99, 98, 97, 96, 95}) == 1);
    // Peak near end
    assert(peakIndex({1, 2, 3, 4, 100, 99}) == 4);
    return 0;
}
