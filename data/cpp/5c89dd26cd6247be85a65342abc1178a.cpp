Write a C++ function `insertion_sort_descending(int arr[], int n)` that sorts an array of integers in **descending** order using the **insertion sort** algorithm, where the array is modified in place. The function should not print anything. Additionally, write a helper function `is_sorted_descending(const int arr[], int n)` that returns `true` if the array is sorted in non-increasing order (i.e., each element is greater than or equal to the next) and `false` otherwise. The solution must use the classic insertion-sort mechanism exactly as shown in the snippet, but with the comparison reversed for descending order (i.e., swap when `arr[j-1] < arr[j]`). Handle edge cases such as an empty array (`n == 0`), a single-element array, and arrays already sorted in descending or ascending order. Your implementation must not use any STL sorting functions.

#include <cassert>

int main() {
    // Test 1: Basic random array
    int arr1[] = {13, 46, 24, 52, 20, 9};
    insertion_sort_descending(arr1, 6);
    assert(is_sorted_descending(arr1, 6));
    assert(arr1[0] == 52 && arr1[5] == 9);

    // Test 2: Already descending order
    int arr2[] = {10, 8, 5, 3, 1};
    insertion_sort_descending(arr2, 5);
    assert(is_sorted_descending(arr2, 5));
    assert(arr2[0] == 10 && arr2[4] == 1);

    // Test 3: Already ascending order (worst case for descending sort)
    int arr3[] = {1, 2, 3, 4, 5};
    insertion_sort_descending(arr3, 5);
    assert(is_sorted_descending(arr3, 5));
    assert(arr3[0] == 5 && arr3[4] == 1);

    // Test 4: Single element
    int arr4[] = {42};
    insertion_sort_descending(arr4, 1);
    assert(is_sorted_descending(arr4, 1));
    assert(arr4[0] == 42);

    // Test 5: Empty array (n = 0)
    int arr5[] = {};
    insertion_sort_descending(arr5, 0);
    assert(is_sorted_descending(arr5, 0));

    // Test 6: All equal elements
    int arr6[] = {7, 7, 7, 7};
    insertion_sort_descending(arr6, 4);
    assert(is_sorted_descending(arr6, 4));
    assert(arr6[0] == 7 && arr6[3] == 7);

    // Test 7: Negative numbers
    int arr7[] = {-3, -1, -5, -2};
    insertion_sort_descending(arr7, 4);
    assert(is_sorted_descending(arr7, 4));
    assert(arr7[0] == -1 && arr7[3] == -5);

    // Test 8: Duplicate values mixed
    int arr8[] = {5, 2, 5, 1, 2};
    insertion_sort_descending(arr8, 5);
    assert(is_sorted_descending(arr8, 5));
    assert(arr8[0] == 5 && arr8[4] == 1);

    return 0;
}

#include <cstddef>

// Sorts arr[0..n-1] in descending order using insertion sort.
void insertion_sort_descending(int arr[], std::size_t n) {
    for (std::size_t i = 0; i < n; ++i) {
        std::size_t j = i;
        // Swap while the previous element is smaller (to move larger ones left).
        while (j > 0 && arr[j - 1] < arr[j]) {
            int temp = arr[j - 1];
            arr[j - 1] = arr[j];
            arr[j] = temp;
            --j;
        }
    }
}

// Returns true if arr is sorted in non-increasing order (descending).
bool is_sorted_descending(const int arr[], std::size_t n) {
    for (std::size_t i = 1; i < n; ++i) {
        if (arr[i - 1] < arr[i]) {
            return false;
        }
    }
    return true;
}

// The insertion sort algorithm builds the sorted portion of the array from left to right. For each index `i` (starting at 0), we take the element at `arr[i]` and shift it leftward while the preceding element is **smaller** (for descending order) than the current element, swapping adjacent pairs until the correct position is found. This is identical to the original snippet except the comparison `arr[j-1] > arr[j]` becomes `arr[j-1] < arr[j]`. For the helper function, we iterate from 1 to n-1 and check that `arr[k-1] >= arr[k]` for all k; if any violation occurs, return `false`. The time complexity is \(O(n^2)\) in the worst and average cases (when the array is sorted in reverse order), and \(O(n)\) in the best case (when already sorted, because the while loop never runs). Space complexity is \(O(1)\) because only a single temporary integer is used. Edge cases: for `n == 0` or `n == 1`, the sort does nothing and the helper returns `true` since there are no adjacent pairs to violate.
