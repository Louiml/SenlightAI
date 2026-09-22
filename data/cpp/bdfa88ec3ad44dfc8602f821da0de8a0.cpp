/*
Write a C++ function `int kthSmallest(int arr[], int n, int k)` that returns the k-th smallest element (1-indexed) from an unsorted array of distinct integers. The function must implement a divide-and-conquer selection algorithm based on partitioning around a pivot, similar to QuickSelect but with a clear contract: if `k` is outside `[1, n]`, return `-1`. The input array will be modified during partitioning, so make a copy inside the function if you need to preserve the original. The solution must handle arrays of length 1, wide value ranges, and arbitrary pivot selection (you may use the last element as pivot, but must correctly recurse into the side that contains the k-th smallest element). You may assume all values are distinct and fit in `int`. The function should have `O(n)` average time and `O(log n)` average auxiliary stack space.
*/
#include <vector>
#include <algorithm>

// Return the k-th smallest element (1-indexed) from arr[0..n-1].
// Returns -1 if k is outside [1, n].
int kthSmallest(int arr[], int n, int k) {
    if (k < 1 || k > n) return -1;

    // Work on a copy to avoid modifying the caller's array.
    std::vector<int> a(arr, arr + n);

    int low = 0, high = n - 1;
    while (low <= high) {
        int pivot = a[high];
        int i = low, j = low;
        for (; i <= high; ++i) {
            if (a[i] <= pivot) {
                std::swap(a[i], a[j]);
                ++j;
            }
        }
        int pi = j - 1; // final index of pivot
        if (pi == k - 1) return a[pi];
        if (pi > k - 1) {
            high = pi - 1;
        } else {
            low = pi + 1;
        }
    }
    return -1; // Should never reach here for valid k
}
#include <cassert>

int kthSmallest(int[], int, int); // forward declaration

int main() {
    int arr1[] = {4, 3, 1, 2, 6, 7, 5};
    assert(kthSmallest(arr1, 7, 1) == 1);
    assert(kthSmallest(arr1, 7, 3) == 3);
    assert(kthSmallest(arr1, 7, 7) == 7);
    assert(kthSmallest(arr1, 7, 4) == 4);

    int arr2[] = {10};
    assert(kthSmallest(arr2, 1, 1) == 10);
    assert(kthSmallest(arr2, 1, 0) == -1);
    assert(kthSmallest(arr2, 1, 2) == -1);

    int arr3[] = {5, -3, 100, 42, 0};
    assert(kthSmallest(arr3, 5, 2) == 0);
    assert(kthSmallest(arr3, 5, 5) == 100);

    int arr4[] = {9, 8, 7, 6, 5, 4, 3, 2, 1};
    assert(kthSmallest(arr4, 9, 5) == 5);
    assert(kthSmallest(arr4, 9, 9) == 9);

    // Original array should not be modified because we copy
    int original[] = {4, 3, 1, 2, 6, 7, 5};
    int copy1[7]; for(int i=0;i<7;++i) copy1[i]=original[i];
    kthSmallest(copy1, 7, 3);
    for(int i=0;i<7;++i) assert(copy1[i] == original[i]);

    return 0;
}
// The typical approach is a randomized or deterministic QuickSelect: choose a pivot (here the last element for simplicity), partition the array so that elements ≤ pivot appear before those > pivot, and the pivot ends up at its final sorted position `pi`. If `pi + 1 == k`, return the pivot. Because the array is 0-indexed, if `k-1 < pi` recurse on the left subarray `[low, pi-1]`; if `k-1 > pi` recurse on the right subarray `[pi+1, high]`. The partition step in the snippet uses two indices `i` and `j` to place all elements ≤ pivot before larger ones, but note that the snippet’s implementation returns `j-1` which is the pivot index after swapping, and it correctly handles the pivot itself (initial pivot is the last element, but after partition the pivot might be moved). Edge cases: `k=1` returns the minimum, `k=n` returns the maximum, and invalid `k` returns -1. Since values are distinct, no ties need handling. The recursion depth is logarithmic on average if pivots are good, but can degrade to O(n) with worst-case pivots; still average time is O(n) per level and sum of sizes is O(n) overall, so expected O(n) time. Space is O(log n) for recursion stack in average case.
