// Write a C++ function named `insertionSortOneStep` that simulates one backward pass of insertion sort on the last element of an integer vector. The function takes a non-empty vector of integers and prints, each on its own line, the array state after each shift operation (when a larger element is moved right to make room for the last element), and finally the array state with the last element inserted into its correct sorted position. The array is passed by value (so the original is unchanged), and the function returns nothing. All array elements are distinct and the array is guaranteed to be sorted except possibly for the last element, which may be smaller than some preceding elements. The output format uses spaces between numbers and a newline after each printed line.
#include <cassert>
#include <sstream>
#include <iostream>
#include <vector>

// The solution function is declared above; here we capture its output.
static std::string captureOutput(const std::vector<int>& arr) {
    std::ostringstream oss;
    std::streambuf* oldCout = std::cout.rdbuf(oss.rdbuf());
    insertionSortOneStep(arr);
    std::cout.rdbuf(oldCout);
    return oss.str();
}

int main() {
    // Test 1: last element needs to move to front
    assert(captureOutput({2, 3, 4, 5, 1}) == "2 3 4 5 5\n2 3 4 4 5\n2 3 3 4 5\n2 2 3 4 5\n1 2 3 4 5\n");
    
    // Test 2: last element needs to move one position
    assert(captureOutput({1, 3, 4, 2}) == "1 3 4 4\n1 3 3 4\n1 2 3 4\n");
    
    // Test 3: last element is already in place
    assert(captureOutput({1, 2, 3}) == "1 2 3\n");
    
    // Test 4: single element, no shifts
    assert(captureOutput({5}) == "5\n");
    
    // Test 5: last element is smallest, two elements
    assert(captureOutput({4, 1}) == "4 4\n1 4\n");
    
    // Test 6: last element fits in middle, larger test
    assert(captureOutput({2, 5, 7, 9, 3}) == "2 5 7 9 9\n2 5 7 7 9\n2 5 5 7 9\n2 3 5 7 9\n");
    
    // Test 7: last element is second smallest
    assert(captureOutput({3, 4, 5, 2}) == "3 4 5 5\n3 4 4 5\n3 3 4 5\n2 3 4 5\n");
    
    std::cout << "All tests passed!\n";
    return 0;
}
#include <vector>
#include <iostream>

// Simulate one backward pass of insertion sort for the last element.
// Prints the array after each shift and after final insertion.
void insertionSortOneStep(const std::vector<int>& arr) {
    int n = static_cast<int>(arr.size());
    // Make a local copy to modify
    std::vector<int> a = arr;
    int key = a[n-1];
    int j = n - 2;
    
    while (j >= 0 && key < a[j]) {
        a[j+1] = a[j];
        // Print current state after shift
        for (int i = 0; i < n; ++i) {
            std::cout << a[i];
            if (i < n - 1) std::cout << " ";
        }
        std::cout << "\n";
        --j;
    }
    
    a[j+1] = key;
    
    // Print final state after insertion
    for (int i = 0; i < n; ++i) {
        std::cout << a[i];
        if (i < n - 1) std::cout << " ";
    }
    std::cout << "\n";
}
// The core idea is to store the last element of the array (the "key") in a temporary variable, then iterate from the second-last position down to the first. For each index `j`, if the key is smaller than the current element `arr[j]`, we shift `arr[j]` one position to the right into `arr[j+1]`, and immediately print the entire current array (this is the partial state after a shift). If the key is not smaller than `arr[j]`, we break out of the loop because the correct insertion position is to the right of this element. After the loop ends, we place the key into position `j+1` and print the final array. Edge cases: if the key is larger than or equal to all preceding elements, the loop breaks immediately at `j = n-2` with no intermediate prints, and the final print is the original array. If the key is smaller than all preceding elements, the loop runs to `j = -1`, then we place the key at index 0. The algorithm performs at most `n-1` iterations and `n-1` prints for intermediate states plus one final print, so time complexity is O(n²) in the worst case due to printing O(n) elements per shift (up to n shifts), and space complexity is O(1) auxiliary (ignoring the input copy passed by value).
