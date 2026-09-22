/*
Write a C++ function named `trappedWater` that takes a non-const array of integers (heights of bars) and its size `n` as parameters, and returns the total volume of water trapped between the bars after a rain, using the classic "Trapping Rain Water" algorithm. The function should handle arrays of any size (including size 0 and size 1, returning 0 in those cases), and the heights can be zero or positive integers. The function must be efficient and avoid modifying the input array. For an array `arr` of size `n`, the trapped water at index `i` is `min(maxLeft[i], maxRight[i]) - arr[i]`, where `maxLeft[i]` is the maximum height to the left of or at `i`, and `maxRight[i]` is the maximum height to the right of or at `i`. Sum only if the difference is positive.
*/

#include <vector>
#include <algorithm>

// Return the total amount of water trapped between bars of given heights.
// `arr` is a pointer to the first element of a non-owning array of `n` integers.
long long trappedWater(const int arr[], int n) {
    if (n < 3) {
        return 0; // Need at least 3 bars to trap water.
    }

    std::vector<int> leftMax(n);
    std::vector<int> rightMax(n);

    // Precompute left maximums: the maximum height from index 0 to i.
    int maxSoFar = 0;
    for (int i = 0; i < n; ++i) {
        maxSoFar = std::max(maxSoFar, arr[i]);
        leftMax[i] = maxSoFar;
    }

    // Precompute right maximums: the maximum height from index i to n-1.
    maxSoFar = 0;
    for (int i = n - 1; i >= 0; --i) {
        maxSoFar = std::max(maxSoFar, arr[i]);
        rightMax[i] = maxSoFar;
    }

    long long totalWater = 0;
    for (int i = 0; i < n; ++i) {
        int waterAtBar = std::min(leftMax[i], rightMax[i]) - arr[i];
        if (waterAtBar > 0) {
            totalWater += waterAtBar;
        }
    }

    return totalWater;
}

#include <cassert>

int main() {
    // Basic case from the classic problem
    int arr1[] = {0, 1, 0, 2, 1, 0, 1, 3, 2, 1, 2, 1};
    assert(trappedWater(arr1, 12) == 6);

    // Flat terrain, no water
    int arr2[] = {5, 5, 5, 5};
    assert(trappedWater(arr2, 4) == 0);

    // Strictly increasing, no water
    int arr3[] = {1, 2, 3, 4, 5};
    assert(trappedWater(arr3, 5) == 0);

    // Strictly decreasing, no water
    int arr4[] = {5, 4, 3, 2, 1};
    assert(trappedWater(arr4, 5) == 0);

    // Single element
    int arr5[] = {7};
    assert(trappedWater(arr5, 1) == 0);

    // Two elements
    int arr6[] = {2, 1};
    assert(trappedWater(arr6, 2) == 0);

    // Some water trapped with a peak in middle
    int arr7[] = {3, 0, 0, 2, 0, 4};
    assert(trappedWater(arr7, 6) == 10);

    // All zero heights, no water
    int arr8[] = {0, 0, 0, 0, 0};
    assert(trappedWater(arr8, 5) == 0);

    // Large values, ensure it returns long long properly
    int arr9[] = {100000, 0, 100000};
    assert(trappedWater(arr9, 3) == 100000);

    // Edge with a single high bar surrounded by lower ones
    int arr10[] = {1, 0, 1};
    assert(trappedWater(arr10, 3) == 1);

    return 0;
}

// The solution uses two precomputed arrays (or vectors) to store the left maximum and right maximum for each position. First, traverse the array from left to right, maintaining the running maximum and storing it in `leftMax`. Then, traverse from right to left, maintaining the running maximum and storing it in `rightMax`. For each index, the water trapped is the minimum of the two maxima minus the current height, and if this value is positive, add it to the total. The main edge cases include: an empty array or array with one element (returns 0 because no trapping possible), and the case where no water can be trapped (e.g., strictly increasing or decreasing heights) where the formula naturally yields zero. Time complexity is O(n) because we do three linear passes (left, right, compute). Space complexity is O(n) for the two auxiliary vectors, which is acceptable. The function should be `const`-correct by taking the array as `const int*` to guarantee the input is not modified, which is good practice even though the original snippet didn't.
