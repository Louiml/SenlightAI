// Write a C++ function named `stableRadixSort` that takes a non-empty array of non-negative integers and its size, sorts the array in ascending order using the radix sort algorithm (least significant digit first), and returns nothing (modifies the array in place). The function must be stable, meaning that equal elements retain their relative order from the original array. The implementation must use counting sort as a subroutine for each digit position. You may assume that all input values are non-negative and fit within a standard `int`. The function should not use any external sorting libraries.

#include <cassert>
#include <vector>
#include <algorithm>

// Declaration of the solution function (provided in the solution section)
void stableRadixSort(int arr[], int n);

int main() {
    // Test case 1: Basic sorting
    int arr1[] = {170, 45, 75, 90, 802, 24, 2, 66};
    int n1 = sizeof(arr1) / sizeof(arr1[0]);
    int expected1[] = {2, 24, 45, 66, 75, 90, 170, 802};
    stableRadixSort(arr1, n1);
    for (int i = 0; i < n1; ++i) {
        assert(arr1[i] == expected1[i]);
    }

    // Test case 2: Single element
    int arr2[] = {42};
    stableRadixSort(arr2, 1);
    assert(arr2[0] == 42);

    // Test case 3: Duplicate values (stability check)
    int arr3[] = {5, 3, 5, 1, 3, 5};
    int n3 = sizeof(arr3) / sizeof(arr3[0]);
    int expected3[] = {1, 3, 3, 5, 5, 5};
    stableRadixSort(arr3, n3);
    for (int i = 0; i < n3; ++i) {
        assert(arr3[i] == expected3[i]);
    }

    // Test case 4: Already sorted array
    int arr4[] = {1, 2, 3, 4, 5};
    int n4 = sizeof(arr4) / sizeof(arr4[0]);
    int expected4[] = {1, 2, 3, 4, 5};
    stableRadixSort(arr4, n4);
    for (int i = 0; i < n4; ++i) {
        assert(arr4[i] == expected4[i]);
    }

    // Test case 5: Reverse sorted array
    int arr5[] = {9, 7, 5, 3, 1};
    int n5 = sizeof(arr5) / sizeof(arr5[0]);
    int expected5[] = {1, 3, 5, 7, 9};
    stableRadixSort(arr5, n5);
    for (int i = 0; i < n5; ++i) {
        assert(arr5[i] == expected5[i]);
    }

    // Test case 6: Large numbers with different digit lengths
    int arr6[] = {1000, 99, 100, 9, 10, 1};
    int n6 = sizeof(arr6) / sizeof(arr6[0]);
    int expected6[] = {1, 9, 10, 99, 100, 1000};
    stableRadixSort(arr6, n6);
    for (int i = 0; i < n6; ++i) {
        assert(arr6[i] == expected6[i]);
    }

    // Test case 7: All zeros
    int arr7[] = {0, 0, 0, 0};
    int n7 = sizeof(arr7) / sizeof(arr7[0]);
    int expected7[] = {0, 0, 0, 0};
    stableRadixSort(arr7, n7);
    for (int i = 0; i < n7; ++i) {
        assert(arr7[i] == expected7[i]);
    }

    // Test case 8: Comparability with std::sort on random data
    int arr8[] = {123, 45, 678, 90, 12, 345, 678, 901};
    int n8 = sizeof(arr8) / sizeof(arr8[0]);
    std::vector<int> vec(arr8, arr8 + n8);
    std::sort(vec.begin(), vec.end());
    stableRadixSort(arr8, n8);
    for (int i = 0; i < n8; ++i) {
        assert(arr8[i] == vec[i]);
    }

    // Test case 9: Larger array with duplicates and zeros
    int arr9[] = {0, 5, 2, 0, 9, 5, 1, 2, 0, 9};
    int n9 = sizeof(arr9) / sizeof(arr9[0]);
    int expected9[] = {0, 0, 0, 1, 2, 2, 5, 5, 9, 9};
    stableRadixSort(arr9, n9);
    for (int i = 0; i < n9; ++i) {
        assert(arr9[i] == expected9[i]);
    }

    return 0;
}

#include <vector>
#include <algorithm>

// A utility function to get the maximum value in the array
static int getMax(const int arr[], int n) {
    int mx = arr[0];
    for (int i = 1; i < n; ++i) {
        if (arr[i] > mx) {
            mx = arr[i];
        }
    }
    return mx;
}

// A function to perform counting sort on arr[] according to the digit represented by exp
static void countingSort(int arr[], int n, int exp) {
    std::vector<int> output(n); // output array
    int count[10] = {0};

    // Store count of occurrences in count[]
    for (int i = 0; i < n; ++i) {
        count[(arr[i] / exp) % 10]++;
    }

    // Change count[i] so that count[i] now contains actual position of this digit in output[]
    for (int i = 1; i < 10; ++i) {
        count[i] += count[i - 1];
    }

    // Build the output array (from right to left for stability)
    for (int i = n - 1; i >= 0; --i) {
        int digit = (arr[i] / exp) % 10;
        output[count[digit] - 1] = arr[i];
        count[digit]--;
    }

    // Copy the output array to arr[], so that arr[] now contains sorted numbers according to current digit
    for (int i = 0; i < n; ++i) {
        arr[i] = output[i];
    }
}

// The main function that sorts arr[] of size n using Radix Sort
void stableRadixSort(int arr[], int n) {
    // Find the maximum number to know number of digits
    int m = getMax(arr, n);

    // Do counting sort for every digit. Note that instead of passing digit number, exp is passed.
    // exp is 10^i where i is the current digit number
    for (int exp = 1; m / exp > 0; exp *= 10) {
        countingSort(arr, n, exp);
    }
}

// The solution implements radix sort by first finding the maximum element in the array to determine the number of digits needed. For each digit position (starting from the least significant digit, i.e., exponent = 1, then 10, 100, etc.), we perform a stable counting sort on that digit. The counting sort works by counting occurrences of each digit (0–9) in an auxiliary array, then converting counts to cumulative positions, and finally building an output array by iterating from the end of the input to preserve stability. After each counting sort, the array is updated with the partially sorted values. The process continues until the exponent exceeds the maximum value. Edge cases include a single element (already sorted) and duplicate values (stability is maintained by iterating the input from right to left during the output build). Time complexity is O(d·(n + k)) where d is the number of digits (max value magnitude) and k=10 (constant digit range), so effectively O(d·n). Space complexity is O(n) for the output array plus O(1) for the count array (since k=10), so O(n) auxiliary space. The function must handle any non-negative `int` values, but be mindful that very large values increase the number of passes.
