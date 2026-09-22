// Write a C++ function that, given a non-empty array of long long integers, its size `n`, and an integer `k`, returns a long long integer according to the following rules: if `k == 1`, return the minimum element of the array; if `k == 2`, return the larger of the first and second elements (if `arr[0] > arr[n-1]`, return `arr[0]`, otherwise return `arr[1]`—note this uses the last element only for comparison, not for output); if `k >= 3`, return the maximum element of the array. The function must handle arrays of any size, including when `n == 1` (in which case `k == 2` must return the only element), and must be robust to duplicate values and negative numbers. Do not assume any particular ordering of the input array. The function should be named `selectByRule` and must be defined in a header-only style with proper `const` correctness.

int main() {
    // Example from the snippet style
    long long arr1[] = {5, 3, 9, 1};
    assert(selectByRule(arr1, 4, 1) == 1);
    assert(selectByRule(arr1, 4, 2) == 5); // arr[0]=5 > arr[3]=1 -> return arr[0]=5
    assert(selectByRule(arr1, 4, 3) == 9);

    // Single-element array
    long long arr2[] = {7};
    assert(selectByRule(arr2, 1, 1) == 7);
    assert(selectByRule(arr2, 1, 2) == 7);
    assert(selectByRule(arr2, 1, 5) == 7);

    // k==2 case where arr[0] <= arr[n-1] -> returns arr[1]
    long long arr3[] = {10, -3, 20, 15};
    assert(selectByRule(arr3, 4, 2) == -3); // arr[0]=10 <= arr[3]=15 -> return arr[1]=-3

    // Duplicate values and negatives
    long long arr4[] = {-4, -4, -2, -2, 0};
    assert(selectByRule(arr4, 5, 1) == -4);
    assert(selectByRule(arr4, 5, 2) == -4); // arr[0]=-4 > arr[4]=0? no -> return arr[1]=-4
    assert(selectByRule(arr4, 5, 3) == 0);

    // Large values
    long long arr5[] = {1000000000000LL, -1000000000000LL, 500000000000LL};
    assert(selectByRule(arr5, 3, 1) == -1000000000000LL);
    assert(selectByRule(arr5, 3, 2) == 1000000000000LL); // arr[0] > arr[2]? yes -> arr[0]
    assert(selectByRule(arr5, 3, 42) == 1000000000000LL);

    return 0;
}

#include <vector>
#include <algorithm>
#include <cstdint>

// Select an array element based on the given rule k.
// k == 1: return minimum element.
// k == 2: return larger of first and second elements (if n == 1, return only element).
// k >= 3: return maximum element.
long long selectByRule(const long long* arr, int n, int k) {
    if (n <= 0) return 0; // defensively return 0 for invalid size, though problem guarantees non-empty.
    if (k == 1) {
        long long minVal = arr[0];
        for (int i = 1; i < n; ++i) {
            if (arr[i] < minVal) minVal = arr[i];
        }
        return minVal;
    }
    if (k == 2) {
        if (n == 1) return arr[0];
        // Compare first and last, then choose between first and second.
        return (arr[0] > arr[n - 1]) ? arr[0] : arr[1];
    }
    // k >= 3 (or any other value)
    long long maxVal = arr[0];
    for (int i = 1; i < n; ++i) {
        if (arr[i] > maxVal) maxVal = arr[i];
    }
    return maxVal;
}

// The solution involves a single pass over the array to compute the minimum and maximum values, initializing both from the first element. Then, based on the value of `k`, we return either the stored minimum, the stored maximum, or for `k == 2` we compare the first and last elements directly. The tricky part for `k == 2` is that the rule uses `arr[0]` and `arr[n-1]` to decide which index (0 or 1) to output, but the output itself is either `arr[0]` or `arr[1]`—never `arr[n-1]`. This means for `n == 1`, `arr[0]` and `arr[n-1]` are the same, so the comparison `arr[0] > arr[n-1]` is false (since not greater), and we return `arr[1]`—but that would be out of bounds! The original snippet would crash for `n == 1` when `k == 2`. To make the function safe, we must handle this edge case by returning `arr[0]` when `n == 1`. Also, for `n >= 2`, `arr[1]` is valid. Time complexity is O(n) for the single pass over the array (we also do O(1) extra work for the comparisons). Space complexity is O(1) beyond the input array. Edge cases include: negative numbers, duplicate minimum/maximum, `k` values less than 1 (we can treat as `k == 1` fallback? The original only handles 1,2,else, so we'll follow that: `k <= 0` treated as `k == 1`? Better to use the original logic: if `k == 1` return min; else if `k == 2` return the rule; else return max).
