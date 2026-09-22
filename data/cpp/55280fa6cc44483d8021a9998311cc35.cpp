// Write a C++ function named `positionInArray` that takes an array of integers (with a fixed size of 100 elements, as defined by a constant `N`), its logical size (number of valid elements), and a target integer. The function returns the index (position) of the target value if it exists in the array, or `-1` if it does not. Additionally, write a helper function `initializeArray` that fills the first `n` elements of the array with values read sequentially from standard input (assuming the input provides exactly `n` integers), and a helper function `printArray` that prints the first `n` elements separated by spaces. The main logic should prompt the user to enter an integer to search, call `positionInArray`, and print either the position if found or a message that the value is not present. The array is zero-indexed, so positions start at 0. The function must be `const`-correct (accept a read-only array reference), handle duplicate values by returning the first occurrence, and work for edge cases like an empty logical size (`n=0`).
// The solution requires three functions:  
// 1. `initializeArray(int arr[], int n)` – reads `n` integers from `std::cin` into `arr[0..n-1]`.  
// 2. `printArray(const int arr[], int n)` – prints `arr[0..n-1]` with spaces.  
// 3. `positionInArray(const int arr[], int n, int target)` – performs a linear search: iterate from index 0 to `n-1`, and if `arr[i] == target`, return `i` immediately (ensuring first occurrence). If the loop completes without finding, return `-1`.  
//
// Edge cases:  
// - `n=0`: loop doesn’t execute, return `-1`.  
// - Duplicate values: the early return on first match gives the smallest index.  
// - `const` correctness: `positionInArray` and `printArray` take `const int arr[]` to prevent modification.  
//
// Time complexity: O(n) for search and initialization/printing. Space complexity: O(1) auxiliary (array storage is external). The main program uses a fixed-size array of 100, but only the first `n` elements are used.
#include <iostream>

// Constant for maximum array size
const int N = 100;

// Reads n integers from standard input into the array.
void initializeArray(int arr[], int n) {
    for (int i = 0; i < n; ++i) {
        std::cin >> arr[i];
    }
}

// Prints the first n elements of the array, separated by spaces.
void printArray(const int arr[], int n) {
    for (int i = 0; i < n; ++i) {
        if (i > 0) std::cout << " ";
        std::cout << arr[i];
    }
    std::cout << std::endl;
}

// Returns the index of the first occurrence of target in arr[0..n-1], or -1 if not found.
int positionInArray(const int arr[], int n, int target) {
    for (int i = 0; i < n; ++i) {
        if (arr[i] == target) {
            return i;
        }
    }
    return -1;
}
#include <cassert>
#include <iostream>

// Declare the solution function (already defined above in a complete program).
const int N = 100;
void initializeArray(int arr[], int n);
void printArray(const int arr[], int n);
int positionInArray(const int arr[], int n, int target);

int main() {
    // Test 1: basic search with multiple values
    int arr1[N] = {10, 20, 30, 40, 50};
    assert(positionInArray(arr1, 5, 30) == 2);
    assert(positionInArray(arr1, 5, 10) == 0);
    assert(positionInArray(arr1, 5, 50) == 4);

    // Test 2: value not present
    assert(positionInArray(arr1, 5, 99) == -1);

    // Test 3: duplicates return first occurrence
    int arr2[N] = {7, 7, 7, 7};
    assert(positionInArray(arr2, 4, 7) == 0);

    // Test 4: empty logical size
    int arr3[N] = {};
    assert(positionInArray(arr3, 0, 5) == -1);

    // Test 5: single element
    int arr4[N] = {42};
    assert(positionInArray(arr4, 1, 42) == 0);
    assert(positionInArray(arr4, 1, 41) == -1);

    // Test 6: negative numbers
    int arr5[N] = {-1, -2, -3};
    assert(positionInArray(arr5, 3, -2) == 1);
    assert(positionInArray(arr5, 3, 0) == -1);

    return 0;
}
