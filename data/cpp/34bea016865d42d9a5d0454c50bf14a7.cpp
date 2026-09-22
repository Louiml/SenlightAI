// Write a C++ function template `bubbleSortDescending` that takes a pointer to an array of elements of a generic type `T` and its length as parameters, and sorts the array in descending order using the bubble sort algorithm. The function should work for any type that supports the `>` and `<` operators (e.g., `int`, `double`, `char`, or custom classes with suitable comparison operators). The function must modify the array in place and return nothing. You may not use any standard library sorting functions or containers—implement the sorting logic manually. Also provide a helper `swapElements` function template (similar to `std::swap`) to exchange two elements of type `T`. Your solution should be self-contained and not require a `main` function in the solution section; only the function templates are needed.

#include <cassert>
#include <iostream>

// Include the solution header or paste the function template here.
// For demonstration, we include the same implementation inline (or via #include).
// (For brevity, the solution code is assumed to be included above.)

#include <string>
#include <vector>

// Simple test type that uses '<' and '>'.
struct Point {
    int x;
    int y;
    bool operator<(const Point& other) const { return x < other.x; }
    bool operator>(const Point& other) const { return x > other.x; }
    bool operator==(const Point& other) const { return x == other.x && y == other.y; }
};

int main() {
    // Test 1: Empty array (length 0) - no crash.
    int* empty = nullptr;
    bubbleSortDescending(empty, 0);

    // Test 2: Single element.
    int single[1] = {42};
    bubbleSortDescending(single, 1);
    assert(single[0] == 42);

    // Test 3: Already descending sorted array.
    int sortedDesc[5] = {9, 7, 5, 3, 1};
    bubbleSortDescending(sortedDesc, 5);
    int expected1[5] = {9, 7, 5, 3, 1};
    for (int i = 0; i < 5; ++i) assert(sortedDesc[i] == expected1[i]);

    // Test 4: Unsorted array with duplicates and negatives.
    int arr1[7] = {3, -1, 4, -1, 2, 0, 5};
    bubbleSortDescending(arr1, 7);
    int expected2[7] = {5, 4, 3, 2, 0, -1, -1};
    for (int i = 0; i < 7; ++i) assert(arr1[i] == expected2[i]);

    // Test 5: All equal elements.
    double arr2[4] = {2.5, 2.5, 2.5, 2.5};
    bubbleSortDescending(arr2, 4);
    for (int i = 0; i < 4; ++i) assert(arr2[i] == 2.5);

    // Test 6: Large random-like array (simple check for descending order).
    int arr3[10] = {10, 1, 9, 2, 8, 3, 7, 4, 6, 5};
    bubbleSortDescending(arr3, 10);
    for (int i = 0; i < 9; ++i) {
        assert(arr3[i] >= arr3[i+1]); // Descending order check.
    }

    // Test 7: Character array (char supports '<').
    char chars[5] = {'b', 'a', 'e', 'c', 'd'};
    bubbleSortDescending(chars, 5);
    assert(chars[0] == 'e' && chars[1] == 'd' && chars[2] == 'c' && chars[3] == 'b' && chars[4] == 'a');

    // Test 8: Custom type with '<' operator.
    Point points[3] = {{1, 5}, {3, 2}, {2, 8}};
    bubbleSortDescending(points, 3);
    assert(points[0] == Point{3, 2});
    assert(points[1] == Point{2, 8});
    assert(points[2] == Point{1, 5});

    std::cout << "All tests passed!" << std::endl;
    return 0;
}

#ifndef BUBBLE_SORT_DESCENDING_H
#define BUBBLE_SORT_DESCENDING_H

// Helper function to swap two elements of generic type T.
template <typename T>
void swapElements(T& a, T& b) {
    T temp = a;
    a = b;
    b = temp;
}

// Sorts an array of type T in descending order using bubble sort.
// Works for any type T that supports the '<' operator.
// Takes a pointer to the array and its length as parameters.
// Modifies the array in place.
template <typename T>
void bubbleSortDescending(T* arr, int length) {
    if (length <= 1) {
        return; // Already sorted for 0 or 1 elements.
    }

    for (int i = 0; i < length - 1; ++i) {
        bool swapped = false; // Optimization: track if any swap occurred.
        for (int j = 0; j < length - i - 1; ++j) {
            // For descending order, swap if current element is less than next.
            if (arr[j] < arr[j + 1]) {
                swapElements(arr[j], arr[j + 1]);
                swapped = true;
            }
        }
        // If no swaps occurred in a full pass, the array is sorted.
        if (!swapped) {
            break;
        }
    }
}

#endif // BUBBLE_SORT_DESCENDING_H

// The solution requires implementing a generic bubble sort that sorts in descending order. The bubble sort algorithm works by repeatedly stepping through the array, comparing adjacent elements and swapping them if they are in the wrong order (i.e., if `arr[j] < arr[j+1]` for descending order). Each pass places the smallest remaining element at the end, reducing the effective length of the unsorted portion by one. The algorithm terminates when no swaps occur in a full pass (or after `length-1` passes, whichever comes first). Edge cases include empty arrays (length 0) and arrays with a single element—both require no action. The function must handle all types that support comparison operators. Time complexity is `O(n^2)` in the worst and average cases (where `n` is the array length), and `O(n)` in the best case if we add an early-exit flag when the array is already sorted (though the requirement doesn't explicitly ask for optimization, adding it is good practice). Space complexity is `O(1)` because only a temporary variable for swapping is used.
