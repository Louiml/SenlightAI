Write a C++ function named `insertionSort` that takes an array of integers and its size as parameters, sorts the array in ascending order using the insertion sort algorithm, and returns void. The function must modify the array in place. You may assume the input array is non-empty and that all elements are valid integers. The function should be declared as a free function (not a member of a class) with appropriate `const` correctness for read-only parameters, and it should not print anything. The signature should be: `void insertionSort(int arr[], int n)`. The sorting must be stable, meaning equal elements retain their relative order.
// The insertion sort algorithm works by building a sorted portion of the array from left to right. For each element at index `i` (starting from index 1), we compare it backward with the previous elements, shifting larger elements one position to the right until we find the correct position for the current element. A naive implementation that uses swaps instead of shifts is also correct as long as the algorithm terminates and preserves order of equal elements. The typical implementation iterates `i` from 1 to `n-1`, and for each `i`, compares `arr[j]` with `arr[j-1]` for `j` decreasing from `i` to 1, swapping if `arr[j] < arr[j-1]` and breaking otherwise. Edge cases include an array of size 1 (no work needed), already sorted array (inner loop breaks immediately, O(n) time), reverse-sorted array (worst case, O(n^2) time), and duplicate elements (stability requires using `<` rather than `<=` to swap, so equal elements do not swap). Time complexity is O(n^2) in the worst and average cases, O(n) in the best case (already sorted). Space complexity is O(1) auxiliary, as it sorts in place with only a single temporary variable for swapping.
#include <vector> // not required, but included for clarity of available types; the function uses raw arrays

// Sorts an array of integers in ascending order using the insertion sort algorithm.
// The array is modified in place. The parameter n is the number of elements.
void insertionSort(int arr[], int n) {
    // Start from the second element; the first element is trivially sorted.
    for (int i = 1; i < n; ++i) {
        // Move element at index i leftward into its correct position among arr[0..i-1].
        for (int j = i; j > 0; --j) {
            // Use strict less-than to maintain stability (equal elements do not swap).
            if (arr[j] < arr[j - 1]) {
                // Swap the two elements.
                int temp = arr[j];
                arr[j] = arr[j - 1];
                arr[j - 1] = temp;
            } else {
                // The element is in the correct place; no need to continue leftward.
                break;
            }
        }
    }
}
#include <cassert>

// The solution function is declared above; this test file includes it by assuming it is in scope.
int main() {
    // Test case 1: Unsorted array with distinct elements.
    int arr1[] = {34, 22, 45, 12, 67, 4, 31};
    int n1 = 7;
    insertionSort(arr1, n1);
    assert(arr1[0] == 4);
    assert(arr1[1] == 12);
    assert(arr1[2] == 22);
    assert(arr1[3] == 31);
    assert(arr1[4] == 34);
    assert(arr1[5] == 45);
    assert(arr1[6] == 67);

    // Test case 2: Already sorted array (best case).
    int arr2[] = {1, 2, 3, 4, 5};
    insertionSort(arr2, 5);
    assert(arr2[0] == 1 && arr2[4] == 5);

    // Test case 3: Reverse-sorted array (worst case).
    int arr3[] = {5, 4, 3, 2, 1};
    insertionSort(arr3, 5);
    assert(arr3[0] == 1 && arr3[4] == 5);

    // Test case 4: Array with duplicates (stability check: order of equal elements preserved).
    int arr4[] = {3, 1, 2, 3, 1};
    insertionSort(arr4, 5);
    assert(arr4[0] == 1);
    assert(arr4[1] == 1);
    assert(arr4[2] == 2);
    assert(arr4[3] == 3);
    assert(arr4[4] == 3);

    // Test case 5: Single element.
    int arr5[] = {42};
    insertionSort(arr5, 1);
    assert(arr5[0] == 42);

    // Test case 6: Negative and zero values.
    int arr6[] = {-5, 0, -1, 4, -10};
    insertionSort(arr6, 5);
    assert(arr6[0] == -10);
    assert(arr6[1] == -5);
    assert(arr6[2] == -1);
    assert(arr6[3] == 0);
    assert(arr6[4] == 4);

    // Test case 7: Two elements already in order.
    int arr7[] = {1, 2};
    insertionSort(arr7, 2);
    assert(arr7[0] == 1 && arr7[1] == 2);

    // Test case 8: Two elements in reverse order.
    int arr8[] = {2, 1};
    insertionSort(arr8, 2);
    assert(arr8[0] == 1 && arr8[1] == 2);
}
