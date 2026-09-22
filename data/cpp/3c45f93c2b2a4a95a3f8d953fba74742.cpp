Given a sorted (non-decreasing) array of integers that may contain negative values, write a C++ function named `squaredSortedArray` that takes the input array, an output array of the same size, and its length, and fills the output array with the squares of all input elements in non-decreasing order. You may not use sorting algorithms or extra memory beyond the given arrays; you must compute the result in linear time by comparing squares from both ends of the original array. The function should not return anything, but modify the `res` array in place. Assume the input array is already sorted and the output array has enough space. Handle edge cases such as arrays with all negative numbers, all positive numbers, zeros, and repeated values.

The key observation is that because the input array is sorted, the square of its elements is the largest at the two extremes: either the smallest (most negative) element or the largest (most positive) element will produce the largest square. Therefore, we fill the output array from the back (index `n-1` down to `0`) by comparing the squares of the current leftmost and rightmost elements. If the left square is greater or equal, we place it in the current output position and move the left pointer inward; otherwise, we place the right square and move the right pointer inward. We continue until both pointers cross. This works for all cases: all negatives (squares decrease from left to right), all positives (squares increase), mixed (squares form a "V" shape), and duplicates (equal squares are handled naturally). No sorting is needed because the two-pointer approach inherently produces a non-decreasing sequence. Time complexity is O(n) since each pointer moves at most n steps. Space complexity is O(1) besides the input/output arrays themselves, as we only use a few integer variables.

#include <vector>

// Fills res[0..n-1] with squares of arr[0..n-1] in non-decreasing order.
// Precondition: arr is sorted in non-decreasing order, res has at least n elements.
void squaredSortedArray(const int arr[], int res[], int n) {
    int left = 0;
    int right = n - 1;
    int pos = n - 1; // Fill from the highest index downwards.

    while (left <= right) {
        int left_square = arr[left] * arr[left];
        int right_square = arr[right] * arr[right];
        if (left_square >= right_square) {
            res[pos--] = left_square;
            ++left;
        } else {
            res[pos--] = right_square;
            --right;
        }
    }
}

#include <cassert>

int main() {
    // Test 1: Mixed negative and positive
    int arr1[] = {-4, -1, 0, 3, 10};
    int res1[5];
    squaredSortedArray(arr1, res1, 5);
    assert(res1[0] == 0 && res1[1] == 1 && res1[2] == 9 && res1[3] == 16 && res1[4] == 100);

    // Test 2: All negative
    int arr2[] = {-7, -3, -2};
    int res2[3];
    squaredSortedArray(arr2, res2, 3);
    assert(res2[0] == 4 && res2[1] == 9 && res2[2] == 49);

    // Test 3: All positive
    int arr3[] = {1, 2, 3};
    int res3[3];
    squaredSortedArray(arr3, res3, 3);
    assert(res3[0] == 1 && res3[1] == 4 && res3[2] == 9);

    // Test 4: Single element zero
    int arr4[] = {0};
    int res4[1];
    squaredSortedArray(arr4, res4, 1);
    assert(res4[0] == 0);

    // Test 5: Duplicate values
    int arr5[] = {-2, -2, 2, 2};
    int res5[4];
    squaredSortedArray(arr5, res5, 4);
    assert(res5[0] == 4 && res5[1] == 4 && res5[2] == 4 && res5[3] == 4);

    // Test 6: Large negative and large positive
    int arr6[] = {-100, 1, 50};
    int res6[3];
    squaredSortedArray(arr6, res6, 3);
    assert(res6[0] == 1 && res6[1] == 2500 && res6[2] == 10000);
}
