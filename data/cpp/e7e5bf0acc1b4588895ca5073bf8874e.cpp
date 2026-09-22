// Write a C++ function named `mergeSortArray` that takes a pointer to an integer array and its size as parameters, and sorts the array in ascending order using the merge sort algorithm. The function must be self-contained, use only standard library utilities, and modify the original array in-place. The array may contain duplicate values and any integers (positive, negative, or zero). You must implement both the recursive divide step and the merge step, but ensure your function is exported as a free function with no global state. Return `void`; the caller expects the array to be sorted after the call.

The solution follows the classic merge sort divide-and-conquer paradigm. The function recursively splits the array into two halves until each subarray has size 0 or 1 (already sorted). Then it merges the two sorted halves into a temporary vector using two pointers to compare elements from the left and right subarrays, selecting the smaller element each time. After merging, the temporary result is copied back into the original array segment. Key edge cases include: an empty array or size 0 (no operation), a single element (already sorted), and arrays with duplicate values (handled by using `<=` in the merge comparison to maintain stability and ensure correct ordering). Time complexity is O(n log n) for all cases (best, average, worst) because the recursion always halves the array and each merge takes linear time. Space complexity is O(n) for the temporary vector used during each merge, and O(log n) for the recursion stack, so total auxiliary space is O(n) in the worst case.

#include <vector>

// Sorts the given integer array in ascending order using merge sort.
void mergeSortArray(int* arr, int size) {
    if (size <= 1) {
        return;
    }

    // Recursive helper lambda to avoid exposing extra parameters.
    auto mergeSortHelper = [&](auto&& self, int* a, int start, int end) -> void {
        if (start >= end) {
            return;
        }

        int mid = start + (end - start) / 2;

        // Recursively sort left and right halves.
        self(self, a, start, mid);
        self(self, a, mid + 1, end);

        // Merge the two sorted halves into a temporary vector.
        std::vector<int> temp(end - start + 1);
        int first = start;
        int second = mid + 1;
        int index = 0;

        while (first <= mid && second <= end) {
            if (a[first] <= a[second]) {
                temp[index++] = a[first++];
            } else {
                temp[index++] = a[second++];
            }
        }

        // Copy remaining elements from left half.
        while (first <= mid) {
            temp[index++] = a[first++];
        }

        // Copy remaining elements from right half.
        while (second <= end) {
            temp[index++] = a[second++];
        }

        // Copy merged result back into the original array.
        for (int i = 0; i < (int)temp.size(); ++i) {
            a[start + i] = temp[i];
        }
    };

    // Start the recursion on the entire array.
    mergeSortHelper(mergeSortHelper, arr, 0, size - 1);
}

#include <cassert>
#include <vector>

// Declare the function under test.
void mergeSortArray(int* arr, int size);

int main() {
    // Test 1: Empty array (size 0) - should not crash.
    {
        int* ptr = nullptr;
        mergeSortArray(ptr, 0);
        // No assertion needed, just ensure no crash.
    }

    // Test 2: Single element array.
    {
        int arr[] = {42};
        mergeSortArray(arr, 1);
        assert(arr[0] == 42);
    }

    // Test 3: Already sorted array.
    {
        int arr[] = {1, 2, 3, 4, 5};
        mergeSortArray(arr, 5);
        int expected[] = {1, 2, 3, 4, 5};
        for (int i = 0; i < 5; ++i) assert(arr[i] == expected[i]);
    }

    // Test 4: Reverse sorted array.
    {
        int arr[] = {9, 7, 5, 3, 1};
        mergeSortArray(arr, 5);
        int expected[] = {1, 3, 5, 7, 9};
        for (int i = 0; i < 5; ++i) assert(arr[i] == expected[i]);
    }

    // Test 5: Array with negative and positive numbers.
    {
        int arr[] = {3, -1, 4, -5, 2};
        mergeSortArray(arr, 5);
        int expected[] = {-5, -1, 2, 3, 4};
        for (int i = 0; i < 5; ++i) assert(arr[i] == expected[i]);
    }

    // Test 6: Duplicate values.
    {
        int arr[] = {4, 2, 4, 2, 1};
        mergeSortArray(arr, 5);
        int expected[] = {1, 2, 2, 4, 4};
        for (int i = 0; i < 5; ++i) assert(arr[i] == expected[i]);
    }

    // Test 7: Larger mixed array.
    {
        std::vector<int> arr = {23, 11, 45, 34, 77, 66, 88, 99, 1, 445, 2};
        int n = (int)arr.size();
        mergeSortArray(arr.data(), n);
        std::vector<int> expected = {1, 2, 11, 23, 34, 45, 66, 77, 88, 99, 445};
        for (int i = 0; i < n; ++i) assert(arr[i] == expected[i]);
    }

    // Test 8: All identical values.
    {
        int arr[] = {-7, -7, -7, -7};
        mergeSortArray(arr, 4);
        int expected[] = {-7, -7, -7, -7};
        for (int i = 0; i < 4; ++i) assert(arr[i] == expected[i]);
    }

    return 0;
}
