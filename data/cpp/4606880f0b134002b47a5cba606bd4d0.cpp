// Given a sorted array of distinct positive integers and a series of queries, write a C++ function `int lowerBoundPosition(const std::vector<int>& arr, int target)` that returns the index of the first element strictly greater than `target`. If no such element exists, return the size of the array (i.e., the position just past the end). However, if `target` is exactly equal to some element in the array, return `-1` instead. The array is guaranteed to be sorted in non-decreasing order and contain no duplicates. Your function must use binary search to achieve logarithmic performance per query. The queries will ask for a combined value `target = x + y` for two positive integers `x` and `y`, and the caller will directly invoke the function. The function should handle empty arrays and targets smaller than all elements or larger than all elements correctly. Time complexity per query must be O(log n) and space O(1).
// The problem is a classic binary search variant: we need to find the insertion point for `target` in a sorted array, but with a special rule: if `target` exists in the array, we return `-1` (since the original code uses that as a signal for "found"). Otherwise, we return the lower bound index (first element greater than `target`), which is exactly `low` after a standard binary search terminates. The binary search maintains `low` and `high` such that after the loop, `low` is the position where `target` would be inserted to keep the array sorted. We check equality inside the loop: if `arr[mid] == target`, we return `-1` immediately. Otherwise, we adjust `low` or `high` according to comparison. Edge cases: empty array → low=0, high=-1, loop never runs, returns 0 (but also no equality check). Target smaller than first element → low stays 0. Target larger than last element → low ends at n (size). Since array is sorted and distinct, no duplicates cause ambiguity. Complexity: O(log n) time per query, O(1) extra space.
#include <vector>

// Returns -1 if target is found in arr, otherwise returns the index of the
// first element strictly greater than target. If all elements are <= target,
// returns arr.size().
int lowerBoundPosition(const std::vector<int>& arr, int target) {
    int low = 0;
    int high = static_cast<int>(arr.size()) - 1;

    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (arr[mid] == target) {
            return -1;
        } else if (arr[mid] < target) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return low;
}
#include <cassert>
#include <vector>

// Declaration for testing (assume function defined above or included)
int lowerBoundPosition(const std::vector<int>& arr, int target);

int main() {
    std::vector<int> arr = {1, 3, 5, 7, 9};
    assert(lowerBoundPosition(arr, 4) == 2);  // first > 4 is 5 at index 2
    assert(lowerBoundPosition(arr, 5) == -1); // exact match
    assert(lowerBoundPosition(arr, 0) == 0);  // smaller than all
    assert(lowerBoundPosition(arr, 10) == 5); // larger than all
    assert(lowerBoundPosition(arr, 1) == -1); // exact first element
    assert(lowerBoundPosition(arr, 9) == -1); // exact last element

    std::vector<int> empty = {};
    assert(lowerBoundPosition(empty, 5) == 0);

    std::vector<int> single = {7};
    assert(lowerBoundPosition(single, 7) == -1);
    assert(lowerBoundPosition(single, 6) == 0);
    assert(lowerBoundPosition(single, 8) == 1);

    std::vector<int> arr2 = {2, 4, 6};
    assert(lowerBoundPosition(arr2, 3) == 1);

    return 0;
}
