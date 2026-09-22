Write a C++ function `bool isMaxHeap(const int arr[], int size)` that determines whether a given array of integers, interpreted as a complete binary tree stored level by level (root at index 0, left child at `2*i+1`, right child at `2*i+2`), satisfies the max-heap property. The max-heap property requires that for every node `i` that has at least one child, the node’s value is greater than or equal to both of its children. The function must handle edge cases such as empty arrays (return `true`), single-element arrays (return `true`), and arrays where nodes have only a left child (the last internal node may lack a right child). Do not modify the input array; the function must be `const`-correct and use only the array and its size as parameters.
// The solution simulates the traversal of a complete binary tree stored in an array. For each index `i` from `0` to `size/2 - 1` (these are exactly the internal nodes—nodes that could have at least one child), we check whether the current value is at least as large as its children. The left child is at `2*i+1`. The right child exists only if `2*i+2 < size`; otherwise, it does not. For the last internal node when `size` is even, only the left child exists. For every internal node, we compare the node's value against both children (if present) and immediately return `false` if the node is smaller than any existing child. If all checks pass, return `true`. Edge cases: empty array (`size == 0`) has no nodes, so it vacuously satisfies the property; single-element array has no children, so it also satisfies. The algorithm runs in O(n) time because each array element is examined at most once as a child (and each internal node is examined once), and O(1) auxiliary space since no extra data structures are used beyond loop variables.
#include <algorithm> // for std::max

// Check if an array represents a max-heap when viewed as a complete binary tree.
// Returns true for empty or single-element arrays (vacuously true).
bool isMaxHeap(const int arr[], int size) {
    if (size <= 1) return true;
    
    // Only internal nodes (indices 0 to size/2 - 1) can have children.
    for (int i = 0; i < size / 2; ++i) {
        int leftChild = 2 * i + 1;
        int rightChild = 2 * i + 2;
        
        // Check left child (always exists because leftChild < size for i < size/2)
        if (arr[i] < arr[leftChild]) {
            return false;
        }
        
        // Check right child if it exists (rightChild < size)
        if (rightChild < size && arr[i] < arr[rightChild]) {
            return false;
        }
    }
    return true;
}
#include <cassert>

int main() {
    // Empty array
    int empty[] = {};
    assert(isMaxHeap(empty, 0) == true);
    
    // Single element
    int single[] = {42};
    assert(isMaxHeap(single, 1) == true);
    
    // Valid max-heap with two children per internal node
    int heap1[] = {10, 9, 8, 7, 6, 5, 4};
    assert(isMaxHeap(heap1, 7) == true);
    
    // Invalid max-heap (left child bigger than parent)
    int notHeap1[] = {10, 11, 8};
    assert(isMaxHeap(notHeap1, 3) == false);
    
    // Invalid max-heap (right child bigger than parent)
    int notHeap2[] = {10, 9, 12};
    assert(isMaxHeap(notHeap2, 3) == false);
    
    // Valid max-heap with only left child for last internal node (size even)
    int heap2[] = {5, 4, 3, 2, 1, 0}; // size=6, last internal index=2, only left child
    assert(isMaxHeap(heap2, 6) == true);
    
    // Invalid max-heap where last internal node (index 2) left child violates
    int notHeap3[] = {5, 4, 1, 2, 3, 0}; // arr[2]=1 < arr[5]=0? no, but arr[2]=1 < arr[4]=3 (child of index1?) Actually check: index2 left child=5, value 0, okay; index1 left=3 value 3, arr[1]=4>=3 ok; index0 left=1 value 4, arr[0]=5>=4 ok; but index1 right=4 value 3, arr[1]=4>=3 ok; so this actually is valid? Let's instead use a clear violation
    // Replace notHeap3 with a clear invalid case involving last internal node's left child
    int notHeap3[] = {5, 4, 1, 2, 3, 10}; // arr[2]=1 < arr[5]=10 (its left child) -> false
    assert(isMaxHeap(notHeap3, 6) == false);
    
    // All equal values are a valid max-heap
    int allEqual[] = {7, 7, 7, 7};
    assert(isMaxHeap(allEqual, 4) == true);
    
    return 0;
}
