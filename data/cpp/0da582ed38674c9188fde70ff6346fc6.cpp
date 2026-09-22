/*
Write a C++ function named `quickSortDescending` that sorts an array of integers in descending order using the QuickSort algorithm with the last element as the pivot. The function should take a pointer to the array, its starting index, and its ending index, and modify the array in place. It must also handle edge cases such as an empty array (where `low > high`) and arrays with duplicate values correctly. Additionally, implement a separate helper function `isSortedDescending` that takes a const array and its size, and returns `true` if the array is sorted in non-increasing (descending) order, and `false` otherwise. The solution must include both functions, but no `main` function in the solution code.
*/

#include <cstddef>

// Partition using the last element as pivot, placing elements greater than pivot to the left.
int partitionDescending(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = low - 1;  // index of the last element known to be greater than pivot

    for (int j = low; j <= high - 1; ++j) {
        if (arr[j] > pivot) {
            ++i;
            std::swap(arr[i], arr[j]);
        }
    }
    std::swap(arr[i + 1], arr[high]);
    return i + 1;
}

// Recursive QuickSort that sorts in descending order.
void quickSortDescending(int arr[], int low, int high) {
    if (low < high) {
        int pi = partitionDescending(arr, low, high);
        quickSortDescending(arr, low, pi - 1);
        quickSortDescending(arr, pi + 1, high);
    }
}

// Check if the array is sorted in non-increasing order.
bool isSortedDescending(const int arr[], std::size_t size) {
    for (std::size_t i = 1; i < size; ++i) {
        if (arr[i - 1] < arr[i]) {
            return false;
        }
    }
    return true;
}

#include <cassert>
#include <cstddef>

// Declare the solution functions (assume they are defined elsewhere).
void quickSortDescending(int arr[], int low, int high);
bool isSortedDescending(const int arr[], std::size_t size);

int main() {
    // Test 1: Basic unsorted array
    int arr1[] = {3, 1, 4, 1, 5, 9, 2, 6};
    quickSortDescending(arr1, 0, 7);
    assert(isSortedDescending(arr1, 8));
    assert(arr1[0] == 9 && arr1[1] == 6 && arr1[2] == 5 && arr1[3] == 4 &&
           arr1[4] == 3 && arr1[5] == 2 && arr1[6] == 1 && arr1[7] == 1);

    // Test 2: Already sorted in descending order
    int arr2[] = {10, 8, 5, 2, 1};
    quickSortDescending(arr2, 0, 4);
    assert(isSortedDescending(arr2, 5));
    assert(arr2[0] == 10 && arr2[4] == 1);

    // Test 3: Already sorted in ascending order (reverse needed)
    int arr3[] = {1, 2, 3, 4, 5};
    quickSortDescending(arr3, 0, 4);
    assert(isSortedDescending(arr3, 5));
    assert(arr3[0] == 5 && arr3[4] == 1);

    // Test 4: All elements equal
    int arr4[] = {7, 7, 7, 7};
    quickSortDescending(arr4, 0, 3);
    assert(isSortedDescending(arr4, 4));
    for (int i = 0; i < 4; ++i) assert(arr4[i] == 7);

    // Test 5: Single element
    int arr5[] = {-42};
    quickSortDescending(arr5, 0, 0);
    assert(isSortedDescending(arr5, 1));

    // Test 6: Empty array (low > high) - no crash, function returns
    int arr6[] = {0};
    quickSortDescending(arr6, 0, -1);  // low > high, should do nothing
    assert(isSortedDescending(arr6, 1));

    // Test 7: Negative numbers
    int arr7[] = {-5, -1, -10, 0, 3};
    quickSortDescending(arr7, 0, 4);
    assert(isSortedDescending(arr7, 5));
    int expected7[] = {3, 0, -1, -5, -10};
    for (std::size_t i = 0; i < 5; ++i) assert(arr7[i] == expected7[i]);

    // Test 8: Large array with duplicates
    int arr8[] = {4, 2, 4, 2, 4, 2};
    quickSortDescending(arr8, 0, 5);
    assert(isSortedDescending(arr8, 6));
    int expected8[] = {4, 4, 4, 2, 2, 2};
    for (std::size_t i = 0; i < 6; ++i) assert(arr8[i] == expected8[i]);
}

// The core algorithm is a standard recursive QuickSort, but the partition step must place elements greater than the pivot to the left and smaller elements to the right to achieve descending order. Choose the last element as the pivot. Maintain an index `i` that tracks the boundary of elements already known to be greater than the pivot. Iterate through the subarray from `low` to `high - 1`; whenever an element is greater than the pivot, increment `i` and swap that element with the element at index `i`. After the loop, swap the pivot (at `high`) with the element at `i + 1` and return `i + 1` as the partition index. Recursively sort the left segment (elements greater than pivot) and right segment (elements smaller than pivot). Edge cases: for an empty or single-element subarray (`low >= high`), do nothing. Duplicate values are simply compared using `>` (not `>=`), ensuring they are placed on the right side of the pivot but still end up adjacent after recursion, which is correct for descending order. Time complexity is average \(O(n \log n)\) and worst-case \(O(n^2)\) when the pivot is the smallest element (for descending sort, that happens on already-sorted descending input). Auxiliary space is \(O(\log n)\) due to recursion stack in average cases. The `isSortedDescending` helper iterates once through the array checking each adjacent pair, taking \(O(n)\) time and \(O(1)\) space.
