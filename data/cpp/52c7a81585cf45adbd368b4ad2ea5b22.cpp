// Write a C++ function `void heapSortDescending(int arr[], int n)` that sorts an array of integers in descending order using the heap sort algorithm. The function should build a max-heap first, then repeatedly extract the maximum element (the root) by swapping it with the last element of the current heap, reducing the heap size, and reheapifying the root. After the loop, print the sorted array elements separated by spaces to the standard output. The function must modify the input array in-place and handle arrays of length 0 or 1 gracefully (printing nothing or the single element). Ensure the implementation uses the standard heapify technique with indices `left = 2*i + 1` and `right = 2*i + 2`, and that no extra data structures (other than a few local variables) are used.
The solution follows the classic heap sort algorithm but adapted for descending order. First, we build a max-heap from the input array using `buildHeap`, which calls `heapify` on every non-leaf node from `n/2 - 1` down to 0. The `heapify` function assumes that the left and right subtrees of node `i` are already valid heaps, and it propagates the largest element upward to the root. After building the max-heap, the largest element is at index 0. We then iterate from the last index down to 1, swapping the root with the current last element, reducing the heap size by one, and calling `heapify` on the new root to maintain the heap property for the reduced heap. This places the largest remaining element at the end of the array each iteration, producing a descending sorted order. Edge cases: an empty array (n=0) should produce no output, and a single-element array (n=1) should output that element. The time complexity is O(n log n) for both buildHeap (O(n)) and the extraction loop (O(n log n)), and the space complexity is O(1) auxiliary (excluding the recursive call stack of heapify, which is O(log n) in the worst case due to recursion depth).
#include <iostream>
#include <algorithm> // for std::swap

// Maintain the max-heap property for the subtree rooted at index i.
void heapifyMax(int arr[], int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left] > arr[largest])
        largest = left;

    if (right < n && arr[right] > arr[largest])
        largest = right;

    if (largest != i) {
        std::swap(arr[i], arr[largest]);
        heapifyMax(arr, n, largest);
    }
}

// Build a max-heap from an array.
void buildMaxHeap(int arr[], int n) {
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapifyMax(arr, n, i);
    }
}

// Sort array in descending order using heap sort and print the result.
void heapSortDescending(int arr[], int n) {
    if (n <= 1) {
        if (n == 1) {
            std::cout << arr[0] << " ";
        }
        return;
    }

    buildMaxHeap(arr, n);

    for (int i = n - 1; i >= 1; --i) {
        std::swap(arr[i], arr[0]);
        heapifyMax(arr, i, 0);
    }

    for (int i = 0; i < n; i++) {
        std::cout << arr[i] << " ";
    }
}
#include <iostream>
#include <cassert>
#include <sstream>

// Declare the function to test (include the solution code above in the same file)
// This test file assumes the solution function is already defined above.

int main() {
    // Test case 1: Basic descending sort
    int arr1[] = {4, 1, 3, 9, 7};
    int n1 = 5;
    std::ostringstream oss1;
    std::streambuf* old_cout = std::cout.rdbuf(oss1.rdbuf());
    heapSortDescending(arr1, n1);
    std::cout.rdbuf(old_cout);
    assert(oss1.str() == "9 7 4 3 1 ");

    // Test case 2: Already descending
    int arr2[] = {5, 4, 3, 2, 1};
    std::ostringstream oss2;
    old_cout = std::cout.rdbuf(oss2.rdbuf());
    heapSortDescending(arr2, 5);
    std::cout.rdbuf(old_cout);
    assert(oss2.str() == "5 4 3 2 1 ");

    // Test case 3: Already ascending
    int arr3[] = {1, 2, 3, 4, 5};
    std::ostringstream oss3;
    old_cout = std::cout.rdbuf(oss3.rdbuf());
    heapSortDescending(arr3, 5);
    std::cout.rdbuf(old_cout);
    assert(oss3.str() == "5 4 3 2 1 ");

    // Test case 4: All identical elements
    int arr4[] = {7, 7, 7, 7};
    std::ostringstream oss4;
    old_cout = std::cout.rdbuf(oss4.rdbuf());
    heapSortDescending(arr4, 4);
    std::cout.rdbuf(old_cout);
    assert(oss4.str() == "7 7 7 7 ");

    // Test case 5: Single element
    int arr5[] = {42};
    std::ostringstream oss5;
    old_cout = std::cout.rdbuf(oss5.rdbuf());
    heapSortDescending(arr5, 1);
    std::cout.rdbuf(old_cout);
    assert(oss5.str() == "42 ");

    // Test case 6: Empty array (no output)
    int arr6[] = {};
    std::ostringstream oss6;
    old_cout = std::cout.rdbuf(oss6.rdbuf());
    heapSortDescending(arr6, 0);
    std::cout.rdbuf(old_cout);
    assert(oss6.str() == "");

    // Test case 7: Negative numbers
    int arr7[] = {-1, -5, -3, -10};
    std::ostringstream oss7;
    old_cout = std::cout.rdbuf(oss7.rdbuf());
    heapSortDescending(arr7, 4);
    std::cout.rdbuf(old_cout);
    assert(oss7.str() == "-1 -3 -5 -10 ");

    std::cout << "All tests passed!\n";
    return 0;
}
