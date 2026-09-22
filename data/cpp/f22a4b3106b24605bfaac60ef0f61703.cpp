Write a C++ function `maxIndexDiff` that takes a non-empty C-style integer array and its size, and returns the maximum value of `j - i` such that `i < j` and `arr[i] < arr[j]`. If no such pair exists (i.e., the array is strictly non-increasing), return `-1`. The function should handle duplicate values correctly (strict inequality required) and work for arrays of any length. You may assume the array size is at least 1.

// The classic approach for this problem uses two auxiliary arrays: `LMin` and `RMax`. `LMin[i]` stores the minimum value from index 0 to i, and `RMax[j]` stores the maximum value from index j to n-1. Then we traverse both arrays with two pointers `i` (starting at 0) and `j` (starting at 0). While `i < n` and `j < n`, if `LMin[i] < RMax[j]`, then we have a valid pair (i, j), and we update the answer with `j - i` and increment `j` (to try a larger gap). Otherwise, we increment `i` (since the current minimum is not less than the current maximum, moving i forward might find a smaller minimum). This runs in O(n) time and O(n) extra space. Edge cases: strictly decreasing array returns -1; all elements equal returns -1 (because strict inequality is required); a single element returns -1 (since no j > i possible). The logic is correct because if there exists any pair with `arr[i] < arr[j]`, the maximum gap will be captured by comparing the smallest prefix minimum with the largest suffix maximum at appropriate indices.

#include <vector>
#include <algorithm>

// Returns the maximum j - i such that i < j and arr[i] < arr[j].
// Returns -1 if no such pair exists.
int maxIndexDiff(const int arr[], int num) {
    if (num < 2) return -1;

    std::vector<int> LMin(num);
    std::vector<int> RMax(num);

    // Build LMin: minimum from left up to index i
    LMin[0] = arr[0];
    for (int i = 1; i < num; ++i) {
        LMin[i] = std::min(LMin[i - 1], arr[i]);
    }

    // Build RMax: maximum from index j to right
    RMax[num - 1] = arr[num - 1];
    for (int j = num - 2; j >= 0; --j) {
        RMax[j] = std::max(RMax[j + 1], arr[j]);
    }

    int i = 0, j = 0;
    int maxDiff = -1;
    while (i < num && j < num) {
        if (LMin[i] < RMax[j]) {
            maxDiff = std::max(maxDiff, j - i);
            ++j;
        } else {
            ++i;
        }
    }
    return maxDiff;
}

#include <cassert>

int main() {
    int arr1[] = {34, 8, 10, 3, 2, 80, 30, 33, 1};
    assert(maxIndexDiff(arr1, sizeof(arr1)/sizeof(int)) == 6);

    int arr2[] = {9, 2, 3, 4, 5, 6, 7, 8, 18, 0};
    assert(maxIndexDiff(arr2, sizeof(arr2)/sizeof(int)) == 8);

    int arr3[] = {1, 2, 3, 4, 5, 6};
    assert(maxIndexDiff(arr3, sizeof(arr3)/sizeof(int)) == 5);

    int arr4[] = {6, 5, 4, 3, 2, 1};
    assert(maxIndexDiff(arr4, sizeof(arr4)/sizeof(int)) == -1);

    int arr5[] = {3, 3, 3, 3};
    assert(maxIndexDiff(arr5, sizeof(arr5)/sizeof(int)) == -1);

    int arr6[] = {5};
    assert(maxIndexDiff(arr6, sizeof(arr6)/sizeof(int)) == -1);

    int arr7[] = {1, 1, 2, 2, 3, 3};
    assert(maxIndexDiff(arr7, sizeof(arr7)/sizeof(int)) == 4);

    int arr8[] = {2, 1, 3, 0, 4};
    assert(maxIndexDiff(arr8, sizeof(arr8)/sizeof(int)) == 4);

    return 0;
}
