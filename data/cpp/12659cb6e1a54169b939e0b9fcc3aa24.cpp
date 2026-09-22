/*
Write a C++ function `findMinimumMarks` that takes an array of integers representing marks obtained by students (index 1 through `tot`, where index 0 is unused) and the total number of students, and returns the minimum mark by building a min-heap using the standard heap insertion algorithm (bubble-up). The function must handle the case where `tot` is 0 or negative by returning 0, and must not modify the original array (use a copy if needed). Your implementation should not use any standard library heap functions; implement the heap manually as shown in the snippet, but the function should be standalone and return the minimum value.
*/

#include <vector>
#include <algorithm>

// Returns the minimum mark from a list of student marks using a min-heap.
// The input array is 1-indexed; marks[0] is unused and ignored.
// Returns 0 if total students is non-positive.
int findMinimumMarks(const std::vector<int>& marks, int tot) {
    if (tot <= 0) {
        return 0;
    }

    // Create a local copy of marks (1-indexed) to build the heap.
    std::vector<int> heap(tot + 1, 0);
    for (int i = 1; i <= tot; ++i) {
        heap[i] = marks[i];
    }

    // Build min-heap using bubble-up (insertion) for each element.
    for (int i = 1; i <= tot; ++i) {
        int j = i;
        int parent = j / 2;
        // Bubble up while current node is smaller than its parent.
        while (j != 0 && heap[j] <= heap[parent]) {
            std::swap(heap[j], heap[parent]);
            j = parent;
            parent = j / 2;
        }
    }

    // Root contains the minimum mark.
    return heap[1];
}

#include <cassert>
#include <vector>

// Declaration from solution
int findMinimumMarks(const std::vector<int>& marks, int tot);

int main() {
    // Test 1: Basic positive marks
    std::vector<int> marks1 = {0, 85, 92, 67, 78};
    assert(findMinimumMarks(marks1, 4) == 67);

    // Test 2: Duplicate minimum at various positions
    std::vector<int> marks2 = {0, 50, 50, 50, 50};
    assert(findMinimumMarks(marks2, 4) == 50);

    // Test 3: Single student
    std::vector<int> marks3 = {0, 42};
    assert(findMinimumMarks(marks3, 1) == 42);

    // Test 4: Already sorted descending (heap must still work)
    std::vector<int> marks4 = {0, 100, 90, 80, 70};
    assert(findMinimumMarks(marks4, 4) == 70);

    // Test 5: Already sorted ascending
    std::vector<int> marks5 = {0, 10, 20, 30, 40};
    assert(findMinimumMarks(marks5, 4) == 10);

    // Test 6: Edge case: zero students
    std::vector<int> marks6 = {0, 5, 9};
    assert(findMinimumMarks(marks6, 0) == 0);

    // Test 7: Negative total
    std::vector<int> marks7 = {0, 5, 9};
    assert(findMinimumMarks(marks7, -2) == 0);

    // Test 8: Only one element but total says many? We trust total as authoritative
    std::vector<int> marks8 = {0, 77, 88};
    assert(findMinimumMarks(marks8, 2) == 77);

    // Test 9: Large random-ish sequence
    std::vector<int> marks9 = {0, 33, 5, 99, 12, 7, 88, 2};
    assert(findMinimumMarks(marks9, 7) == 2);

    return 0;
}

// The core approach is to maintain a min-heap in an array where the root at index 1 always holds the minimum mark. Starting from an empty heap, each mark is inserted at the next available position (index `i` from 1 to `tot`), then the bubble-up operation compares the new node with its parent (at index `j/2`) and swaps if the new node is smaller, repeating until the heap property holds. After all insertions, the minimum mark is simply the element at index 1. Edge cases: if `tot` is 0 or negative, return 0 (no marks). Since the input array is passed by const reference or pointer, we make a local copy to build the heap without altering the original. Time complexity is O(tot * log tot) due to each insertion bubbling up at most O(log tot) steps, and space complexity is O(tot) for the heap array copy.
