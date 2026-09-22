Write a standalone C++ function named `quickSortDescending` that performs an in-place descending-order quicksort on a fixed-size array of integers, given the array, its size, and the inclusive range `[first, last]` to sort. The function must use the leftmost element of the current subarray as the pivot and employ the classic two-pointer (left/right) partitioning scheme, but with the comparison logic reversed so that the final sorted order is strictly non-increasing (largest to smallest). The function must directly modify the array and must not use any external libraries beyond the C++ standard library. It must handle edge cases such as empty ranges (`first >= last`), duplicate values, and negative numbers correctly. The implementation should be self-contained, follow `const` correctness where possible (though the array itself is mutable), and be suitable for testing via assertions in a separate driver program.
The core algorithm is a recursive quicksort with a modified partitioning step to achieve descending order. The pivot is chosen as the element at index `first`. Two indices, `left` and `right`, are initialized to `first` and `last` respectively. The `left` index moves rightward while `array[left] >= array[pivot]` (since we want larger elements on the left), and the `right` index moves leftward while `array[right] < array[pivot]` (smaller elements on the right). The `left` movement must also be bounded by `left < last` to prevent index out-of-bounds when all elements are already in descending order. When both indices stop and `left < right`, their values are swapped, and the process repeats until `left >= right`. After the loop, the pivot is swapped with the element at index `left - 1` (or equivalently, `right` after adjustments) to place it in its final sorted position. Then recursion continues on the subarray `[first, left-1]` and `[left, last]`. Edge cases: if the range has zero or one element, the function returns immediately. Duplicate values are handled naturally because comparisons use `>=` and `<`; duplicates may cross the pivot but the partition remains valid. Negative numbers are treated the same as positives since only relative ordering matters. Time complexity is \(O(n \log n)\) on average and \(O(n^2)\) in the worst case (e.g., already sorted arrays due to poor pivot choice), with \(O(\log n)\) recursion stack space on average. Space complexity is \(O(\log n)\) for the call stack.
#include <vector>
#include <algorithm>  // for std::swap (or use custom swap)

// Helper to swap two integers.
void swapInts(int& a, int& b) {
    int temp = a;
    a = b;
    b = temp;
}

// Recursive descending quicksort on array[first..last] inclusive.
// The pivot is the leftmost element; partition so that larger values
// end up on the left, smaller on the right.
void quickSortDescending(int* array, int first, int last) {
    if (first >= last) return;  // empty or single-element range

    int pivot = first;
    int left = first;
    int right = last;

    while (left < right) {
        // Move left pointer right while element is >= pivot (larger/equal go left).
        while (array[left] >= array[pivot] && left < last) {
            left++;
        }
        // Move right pointer left while element is < pivot (smaller go right).
        while (array[right] < array[pivot]) {
            right--;
        }
        if (left < right) {
            swapInts(array[left], array[right]);
        }
    }

    // Place pivot at its correct position: after the loop, 'left' is one past
    // the last element that is >= pivot. So pivot belongs at index left-1.
    swapInts(array[pivot], array[left - 1]);

    // Recursively sort left and right partitions (excluding the pivot).
    quickSortDescending(array, first, left - 2);  // left-1 is pivot's final index
    quickSortDescending(array, left, last);
}
#include <cassert>
#include <iostream>

// Declaration of the function under test
void quickSortDescending(int* array, int first, int last);

// Utility to check if a range is sorted in non-increasing order
bool isDescending(const int* array, int size) {
    for (int i = 1; i < size; ++i) {
        if (array[i] > array[i - 1]) return false;
    }
    return true;
}

int main() {
    // Test 1: Simple unsorted array
    int arr1[5] = {5, 2, 8, 1, 9};
    quickSortDescending(arr1, 0, 4);
    assert(isDescending(arr1, 5));
    assert(arr1[0] == 9 && arr1[4] == 1);

    // Test 2: Already descending (should remain unchanged)
    int arr2[4] = {10, 7, 3, 1};
    quickSortDescending(arr2, 0, 3);
    assert(isDescending(arr2, 4));

    // Test 3: Duplicate values
    int arr3[6] = {3, 1, 3, 2, 3, 1};
    quickSortDescending(arr3, 0, 5);
    assert(isDescending(arr3, 6));
    assert(arr3[0] == 3 && arr3[3] == 2 && arr3[5] == 1);

    // Test 4: Negative numbers
    int arr4[5] = {-5, -1, -9, -3, -7};
    quickSortDescending(arr4, 0, 4);
    assert(isDescending(arr4, 5));
    assert(arr4[0] == -1 && arr4[4] == -9);

    // Test 5: Single element
    int arr5[1] = {42};
    quickSortDescending(arr5, 0, 0);
    assert(isDescending(arr5, 1));

    // Test 6: Already ascending (worst case), should still sort correctly
    int arr6[4] = {1, 2, 3, 4};
    quickSortDescending(arr6, 0, 3);
    assert(isDescending(arr6, 4));
    assert(arr6[0] == 4 && arr6[3] == 1);

    // Test 7: Two elements
    int arr7[2] = {100, 50};
    quickSortDescending(arr7, 0, 1);
    assert(isDescending(arr7, 2));

    // Test 8: Empty range (first > last) should not crash
    int arr8[3] = {9, 8, 7};
    quickSortDescending(arr8, 1, 0);  // invalid range, nothing should happen
    assert(arr8[0] == 9 && arr8[1] == 8 && arr8[2] == 7);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
