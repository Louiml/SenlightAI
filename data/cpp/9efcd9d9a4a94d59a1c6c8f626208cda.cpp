// Write a C++ function named `printReverseRecursive` that takes an array of integers, a starting index (which will be the last valid index of the array), and the size of the array. The function should recursively print all elements of the array from the given starting index down to index 0, each on a separate line. If the starting index is negative or the array size is zero, the function should do nothing (return without printing). The function must not use any loops or iterative constructs; it must be implemented purely with recursion. The task is to fill in the body of the function in the provided skeleton, ensuring it works correctly for edge cases like an empty array, a single-element array, and negative starting index.

#include <cassert>
#include <iostream>
#include <sstream>

// A helper to capture cout output for testing
std::string captureOutput(void (*func)(const int[], int, int), const int arr[], int idx, int n) {
    std::streambuf* old = std::cout.rdbuf();
    std::ostringstream oss;
    std::cout.rdbuf(oss.rdbuf());
    
    func(arr, idx, n);
    
    std::cout.rdbuf(old);
    return oss.str();
}

int main() {
    // Test 1: Normal array
    int arr1[] = {1, 2, 3, 4, 5};
    assert(captureOutput(printReverseRecursive, arr1, 4, 5) == "5\n4\n3\n2\n1\n");
    
    // Test 2: Single element
    int arr2[] = {42};
    assert(captureOutput(printReverseRecursive, arr2, 0, 1) == "42\n");
    
    // Test 3: Empty array (n = 0)
    int arr3[] = {};
    assert(captureOutput(printReverseRecursive, arr3, -1, 0) == "");
    
    // Test 4: Negative starting index (invalid call, should do nothing)
    int arr4[] = {10, 20};
    assert(captureOutput(printReverseRecursive, arr4, -1, 2) == "");
    
    // Test 5: Starting from middle index (should print from that index down)
    int arr5[] = {7, 8, 9, 10};
    assert(captureOutput(printReverseRecursive, arr5, 2, 4) == "9\n8\n7\n");
    
    // Test 6: Array with duplicates
    int arr6[] = {1, 1, 1};
    assert(captureOutput(printReverseRecursive, arr6, 2, 3) == "1\n1\n1\n");
    
    // Test 7: Negative numbers
    int arr7[] = {-3, -2, -1};
    assert(captureOutput(printReverseRecursive, arr7, 2, 3) == "-1\n-2\n-3\n");
    
    std::cout << "All tests passed!" << std::endl;
    return 0;
}

#include <iostream>

// Recursively print array elements from index `idx` down to 0, each on a new line.
// If idx < 0 or the array is empty, do nothing.
void printReverseRecursive(const int arr[], int idx, int n) {
    // Base case: stop if we've passed the beginning or the array is empty
    if (idx < 0 || n <= 0) {
        return;
    }
    
    // Print the current element
    std::cout << arr[idx] << std::endl;
    
    // Recurse to the previous index
    printReverseRecursive(arr, idx - 1, n);
}

// The solution approach is based on recursive descent: starting from the provided index (which represents the position of the last element to print in this call), we first check for the base case — if the index is less than 0, we stop. Otherwise, we print the element at the current index, then call the function recursively with `idx-1`. This prints elements from the end of the array to the beginning. Important edge cases include: (1) an empty array (`n == 0` or `idx < 0`), where the function should not access any array element and simply return; (2) a negative starting index, which is also handled by the base case; (3) a single-element array, where the recursion prints that element and then stops. Time complexity is O(n) because each recursive call processes one element, and space complexity is O(n) due to the call stack depth, which is proportional to the number of elements printed.
