// Write a C++ function named `sortPointersAndDisplay` that takes an array of integers, its size, and an output stream reference as parameters. The function must create an array of pointers to the original integers, sort the pointer array in ascending order based on the pointed-to integer values, then write two lines to the output stream: first the sorted values (in ascending order) separated by spaces, then the original values in their original order separated by spaces. The original integer array must remain unmodified. The function should handle arrays with duplicate values correctly and must work for any positive size. Do not include a `main` function in your solution.

#include <iostream>
#include <sstream>
#include <cassert>

// Forward declaration of the function to test.
void sortPointersAndDisplay(const int arr[], int size, std::ostream& out);

int main() {
    // Test 1: Typical case with duplicates.
    {
        int arr[] = {5, 100, 5, 25, 10};
        std::ostringstream oss;
        sortPointersAndDisplay(arr, 5, oss);
        assert(oss.str() == "5 5 10 25 100\n5 100 5 25 10\n");
    }

    // Test 2: Single element.
    {
        int arr[] = {42};
        std::ostringstream oss;
        sortPointersAndDisplay(arr, 1, oss);
        assert(oss.str() == "42\n42\n");
    }

    // Test 3: Already sorted.
    {
        int arr[] = {1, 2, 3, 4};
        std::ostringstream oss;
        sortPointersAndDisplay(arr, 4, oss);
        assert(oss.str() == "1 2 3 4\n1 2 3 4\n");
    }

    // Test 4: Reverse sorted.
    {
        int arr[] = {9, 7, 5, 3, 1};
        std::ostringstream oss;
        sortPointersAndDisplay(arr, 4, oss); // size=4, only first 4 used
        assert(oss.str() == "3 5 7 9\n9 7 5 3\n");
    }

    // Test 5: Negative values and large size.
    {
        int arr[] = {-5, -1, -10, 0, 3, -2};
        std::ostringstream oss;
        sortPointersAndDisplay(arr, 6, oss);
        assert(oss.str() == "-10 -5 -2 -1 0 3\n-5 -1 -10 0 3 -2\n");
    }

    // Test 6: Ensure original array is not modified.
    {
        int arr[] = {10, 20, 30};
        std::ostringstream oss;
        sortPointersAndDisplay(arr, 3, oss);
        assert(arr[0] == 10 && arr[1] == 20 && arr[2] == 30);
        assert(oss.str() == "10 20 30\n10 20 30\n");
    }

    std::cout << "All tests passed!" << std::endl;
    return 0;
}

#include <iostream>

// Sorts an array of integers using an auxiliary pointer array,
// prints sorted then original order to the given output stream.
void sortPointersAndDisplay(const int arr[], int size, std::ostream& out) {
    // Create an array of pointers to the original elements.
    int* ptrArr[size];
    for (int i = 0; i < size; ++i) {
        ptrArr[i] = const_cast<int*>(&arr[i]);
    }

    // Selection sort on the pointer array based on pointed-to values.
    for (int startScan = 0; startScan < size - 1; ++startScan) {
        int minIndex = startScan;
        int* minElem = ptrArr[startScan];
        for (int index = startScan + 1; index < size; ++index) {
            if (*ptrArr[index] < *minElem) {
                minElem = ptrArr[index];
                minIndex = index;
            }
        }
        // Swap pointers (not the underlying integers).
        ptrArr[minIndex] = ptrArr[startScan];
        ptrArr[startScan] = minElem;
    }

    // Output sorted order.
    for (int i = 0; i < size; ++i) {
        out << *ptrArr[i];
        if (i < size - 1) out << " ";
    }
    out << "\n";

    // Output original order.
    for (int i = 0; i < size; ++i) {
        out << arr[i];
        if (i < size - 1) out << " ";
    }
    out << "\n";
}

// The core idea is to use an auxiliary array of pointers that we sort, leaving the original data untouched. We first populate the pointer array so each element points to the corresponding integer in the original array. Then we perform a selection sort on the pointer array, comparing the values pointed to by each pointer (dereferenced values) to determine order. During sorting, we swap pointers, not the underlying integers. After sorting, we iterate through the pointer array, dereference each pointer, and write the value to the output stream. For the original order, we iterate through the original integer array directly. Edge cases include duplicate values (they appear multiple times in both outputs, sorted adjacent) and a single-element array (the sorted and original orders are identical). Time complexity is \(O(n^2)\) due to selection sort (where \(n\) is the array size), and space complexity is \(O(n)\) for the pointer array. The original array is safe because we only read from it and modify pointers.
