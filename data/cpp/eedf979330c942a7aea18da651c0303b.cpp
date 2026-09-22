Write a C++ function named `maxWaterContainer` that takes a non-empty array of non-negative integers representing the heights of vertical lines at each index and returns the maximum amount of water (an integer) that can be contained between any two lines, where the container's width is the distance between indices and its height is the shorter of the two lines. The function must accept a pointer to the first element of the array and its size, and must work for sizes from 1 to any large value. Handle edge cases where all heights are equal, where heights increase or decrease monotonically, and where the maximum area is obtained by lines that are not at the extremes. The function should not modify the input array and should use only constant extra space.
#include <cassert>

// The solution function is assumed to be defined above this main.
int main() {
    int arr1[] = {1, 8, 6, 2, 5, 4, 8, 3, 7};
    assert(maxWaterContainer(arr1, 9) == 49);

    int arr2[] = {1, 1};
    assert(maxWaterContainer(arr2, 2) == 1);

    int arr3[] = {5};
    assert(maxWaterContainer(arr3, 1) == 0);

    int arr4[] = {4, 3, 2, 1, 4};
    assert(maxWaterContainer(arr4, 5) == 16);

    int arr5[] = {1, 2, 1};
    assert(maxWaterContainer(arr5, 3) == 2);

    int arr6[] = {3, 3, 3, 3};
    assert(maxWaterContainer(arr6, 4) == 9);

    int arr7[] = {0, 0, 0};
    assert(maxWaterContainer(arr7, 3) == 0);

    int arr8[] = {2, 3, 10, 5, 7, 8, 9};
    assert(maxWaterContainer(arr8, 7) == 36);

    int arr9[] = {10, 9, 8, 7, 6, 5};
    assert(maxWaterContainer(arr9, 6) == 25);

    int arr10[] = {6, 5, 4, 3, 2, 1};
    assert(maxWaterContainer(arr10, 6) == 9);

    return 0;
}
#include <algorithm> // for std::min, std::max

// Returns the maximum water area between two vertical lines.
// height: pointer to array of non-negative integers.
// size: number of elements in the array (must be >= 0).
int maxWaterContainer(const int* height, int size) {
    if (size < 2) {
        return 0;
    }

    int left = 0;
    int right = size - 1;
    int maxArea = 0;

    while (left < right) {
        int currentHeight = std::min(height[left], height[right]);
        int area = (right - left) * currentHeight;
        maxArea = std::max(maxArea, area);

        // Move the pointer pointing to the shorter line.
        if (height[left] < height[right]) {
            ++left;
        } else {
            --right;
        }
    }

    return maxArea;
}
// The optimal solution uses a two-pointer technique. Place one pointer at the start (`left = 0`) and the other at the end (`right = size - 1`). The area formed by the two lines is `(right - left) * min(height[left], height[right])`. Track the maximum area seen. The key insight is that the shorter line limits the height, so to possibly find a larger area, we should move the pointer pointing to the shorter line inward, because moving the taller line inward would only reduce the width while keeping the height the same or lower. Continue until the pointers meet. Edge cases: when there are only two lines, the answer is simply their product; when all heights are equal, the maximum area is between the first and last lines (largest width); when a single element exists (size=1), the answer is 0 because no pair exists (though the problem states non-empty, we can safely return 0 for size<2). Time complexity is O(n) and space complexity is O(1).
