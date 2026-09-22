/*
Given a fixed-size array of integers (represented by a global array `arr` of length `N = 11`) and a 1-based index `position` (where `1 ≤ position ≤ N`), write a C++ function `void maxHeapify(int position)` that assumes the binary tree rooted at `position` (with array indices treated as 1-based heap nodes) satisfies the max-heap property at all descendants, but the value at `position` may be smaller than its children. The function should restore the max-heap property at `position` and all affected descendants by repeatedly swapping the node with its largest child until the node is larger than both children or becomes a leaf. The heap is stored in the global array `arr`, and node `i` has children at indices `2*i` and `2*i+1` (1-based). You may write helper functions for parent, left child, right child, and swap if needed. The function must operate in-place and modify the global array. It should also handle edge cases where the node has only a left child or no children. Do not include a `main` function in your solution—only provide the `arr` declaration, the helper functions, and `maxHeapify`. For the test, you will use the provided sample arrays and verify that after calling `maxHeapify` on a specific index, the resulting array satisfies the max-heap property locally and globally (if the root was used).
*/

#include <algorithm>

// Global array and size (exposed for testing, but the function modifies it directly)
int arr[11] = {11,4,55,6,77,8,9,0,7,1,-1};
const int N = 11;

// Swap two integer values via pointers
void swapValues(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Return 1-based left child index of a given 1-based parent index
int leftChild(int parentIdx) {
    return 2 * parentIdx;
}

// Return 1-based right child index of a given 1-based parent index
int rightChild(int parentIdx) {
    return 2 * parentIdx + 1;
}

// Restore max-heap property at a given 1-based position
void maxHeapify(int position) {
    int largest = position;
    int l = leftChild(position);
    int r = rightChild(position);

    // If left child exists and is larger than current largest
    if (l <= N && arr[l - 1] > arr[largest - 1]) {
        largest = l;
    }
    // If right child exists and is larger than current largest
    if (r <= N && arr[r - 1] > arr[largest - 1]) {
        largest = r;
    }

    // If the largest is a child, swap and recurse
    if (largest != position) {
        swapValues(&arr[largest - 1], &arr[position - 1]);
        maxHeapify(largest);
    }
}

#include <cassert>
#include <iostream>

// Global array and size (must match the solution's declaration)
extern int arr[11];
extern const int N;

// Forward declaration of the solution function
void maxHeapify(int position);

// Helper to check if the subtree rooted at 1-based index i satisfies max-heap property
bool isMaxHeap(int i) {
    if (i > N) return true;
    int l = 2 * i;
    int r = 2 * i + 1;
    bool leftOk = (l > N) || (arr[i - 1] >= arr[l - 1] && isMaxHeap(l));
    bool rightOk = (r > N) || (arr[i - 1] >= arr[r - 1] && isMaxHeap(r));
    return leftOk && rightOk;
}

int main() {
    // Test 1: heapify leaf (index 11) should do nothing
    int original[11] = {11,4,55,6,77,8,9,0,7,1,-1};
    for (int i = 0; i < N; ++i) arr[i] = original[i];
    maxHeapify(11);
    for (int i = 0; i < N; ++i) assert(arr[i] == original[i]);
    assert(isMaxHeap(11));

    // Test 2: heapify index 2 (value 4) should swap with child 77 at index 5
    int expected2[11] = {11,77,55,6,4,8,9,0,7,1,-1};
    maxHeapify(2);
    for (int i = 0; i < N; ++i) assert(arr[i] == expected2[i]);
    assert(isMaxHeap(2));

    // Test 3: heapify root (index 1) should produce max-heap with largest at root
    int expectedRoot[11] = {77,11,55,6,4,8,9,0,7,1,-1}; // after heapify root, 77 goes to root
    for (int i = 0; i < N; ++i) arr[i] = original[i];
    maxHeapify(1);
    for (int i = 0; i < N; ++i) assert(arr[i] == expectedRoot[i]);
    assert(isMaxHeap(1));

    // Test 4: heapify with a different input array (use a known array)
    int second[11] = {11,10,9,88,77,660,5,44,3,290,48};
    for (int i = 0; i < N; ++i) arr[i] = second[i];
    maxHeapify(1);
    // After heapify from root, the root must be the maximum of the entire array
    int maxVal = arr[0];
    for (int i = 1; i < N; ++i) assert(arr[i] <= maxVal);
    assert(isMaxHeap(1));

    // Test 5: heapify a node with only a left child (index 5, value 77, left child 10)
    // After heapify, 77 stays because it's larger than 10
    for (int i = 0; i < N; ++i) arr[i] = second[i];
    maxHeapify(5);
    assert(arr[4] == 77);
    assert(isMaxHeap(5));

    std::cout << "All tests passed.\n";
    return 0;
}

// The solution implements the standard bottom-up max-heapify algorithm. Starting at a given 1-based index, we compare the value at that node with its left and right children (if they exist within the array bounds). We find the index of the largest among the node, left child, and right child. If the largest is not the node itself, we swap the node's value with that child, then recursively call `maxHeapify` on that child's index to continue repairing downward. If the node is already the largest, the function stops. Key edge cases: (1) The right child may not exist if `2*pos+1 > N`, so we must guard against accessing out-of-bounds. (2) The left child may not exist if `2*pos > N`. (3) The array is 0-based in C++ but the heap logic uses 1-based indices, so we access `arr[index-1]`. Time complexity is O(log N) for a single heapify on a balanced heap, but in the worst case (e.g., heapify from the root of an arbitrary array) it is O(N) because the recursive depth can be N in an unbalanced tree; however, for a single call starting from any node in a complete binary tree of size N, the height is at most O(log N), so typical analysis gives O(log N) time and O(log N) stack space due to recursion. Space complexity is O(1) auxiliary besides the recursion stack.
