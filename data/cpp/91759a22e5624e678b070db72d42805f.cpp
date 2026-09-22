// Write a C++ function named `insertionSort` that takes a non-empty vector of integers and sorts it in ascending order using the insertion sort algorithm. The function should modify the vector in place and return nothing. The input vector may contain duplicate values, negative numbers, and any number of elements (at least one). The function must be const-correct where applicable (i.e., it should not modify any parameter other than the target vector). Do not use any standard sorting functions; implement the algorithm manually. Ensure the function works correctly for edge cases like a single element or an already-sorted vector.
// The insertion sort algorithm builds the sorted portion of the array one element at a time. Starting from index 1, each element is compared with the elements before it (which are already sorted) and inserted into its correct position by shifting larger elements one position to the right. The outer loop runs from the second element to the end, and for each element we store its value in a temporary variable. The inner while loop shifts elements that are greater than the temporary value to the right, until we find the correct insertion point. Edge cases include an empty vector (though the task specifies non-empty), a single-element vector (which is already sorted and the loop does nothing), duplicates (the comparison uses `<` so equal elements do not shift, maintaining stability), and negative numbers (comparisons work normally). Time complexity is \(O(n^2)\) in the worst and average cases (reverse-sorted or random), and \(O(n)\) in the best case (already sorted), where \(n\) is the number of elements. Space complexity is \(O(1)\) since it sorts in place using only a constant number of variables.
#include <vector>

// Sorts a vector of integers in ascending order using insertion sort.
void insertionSort(std::vector<int>& arr) {
    for (std::size_t i = 1; i < arr.size(); ++i) {
        int key = arr[i];
        std::size_t j = i;
        // Shift elements greater than key to the right
        while (j > 0 && arr[j - 1] > key) {
            arr[j] = arr[j - 1];
            --j;
        }
        arr[j] = key;
    }
}
#include <cassert>
#include <vector>

// (The insertionSort function is assumed to be defined above or included here.)

int main() {
    // Already sorted
    std::vector<int> v1 = {1, 2, 3, 4, 5};
    insertionSort(v1);
    assert(v1 == (std::vector<int>{1, 2, 3, 4, 5}));

    // Reverse sorted
    std::vector<int> v2 = {5, 4, 3, 2, 1};
    insertionSort(v2);
    assert(v2 == (std::vector<int>{1, 2, 3, 4, 5}));

    // Contains duplicates and negatives
    std::vector<int> v3 = {3, -1, 3, 0, -2, 3};
    insertionSort(v3);
    assert(v3 == (std::vector<int>{-2, -1, 0, 3, 3, 3}));

    // Single element
    std::vector<int> v4 = {42};
    insertionSort(v4);
    assert(v4 == (std::vector<int>{42}));

    // Two elements, unsorted
    std::vector<int> v5 = {2, 1};
    insertionSort(v5);
    assert(v5 == (std::vector<int>{1, 2}));

    // Large random-ish case with all same values
    std::vector<int> v6 = {7, 7, 7, 7, 7};
    insertionSort(v6);
    assert(v6 == (std::vector<int>{7, 7, 7, 7, 7}));

    // Mixed large values
    std::vector<int> v7 = {10, -5, 3, 0, 8, -5, -5};
    insertionSort(v7);
    assert(v7 == (std::vector<int>{-5, -5, -5, 0, 3, 8, 10}));

    // Empty vector (though task says non-empty, we test robustness)
    std::vector<int> v8;
    insertionSort(v8);
    assert(v8.empty());

    // Multiple duplicates with unsorted order
    std::vector<int> v9 = {2, 2, 1, 2, 1};
    insertionSort(v9);
    assert(v9 == (std::vector<int>{1, 1, 2, 2, 2}));

    return 0;
}
