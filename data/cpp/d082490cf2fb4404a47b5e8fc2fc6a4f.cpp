// Write a C++ function `std::vector<int> quickSortVector(const std::vector<int>& input)` that takes a vector of integers (which may be empty, contain duplicates, or contain negative values) and returns a new vector containing the same elements sorted in non-decreasing order. The function must not modify the input vector and must implement the quicksort algorithm recursively using the Lomuto partition scheme with the last element as the pivot. You are not allowed to use any standard sorting functions (like `std::sort`) or any additional containers beyond the input/output vectors. Provide a robust implementation that handles edge cases such as empty input, single-element input, already-sorted input, and reverse-sorted input.

#include <cassert>
#include <vector>

// The solution function is assumed to be defined above or in an included header.
// For completeness, include the declaration here if needed.
std::vector<int> quickSortVector(const std::vector<int>& input);

int main() {
    // Basic case with negatives and duplicates
    std::vector<int> v1 = {5, -2, 9, 1, 0, -2, 7};
    std::vector<int> sorted1 = quickSortVector(v1);
    assert((sorted1 == std::vector<int>{-2, -2, 0, 1, 5, 7, 9}));

    // Empty input
    std::vector<int> v2;
    assert(quickSortVector(v2).empty());

    // Single element
    std::vector<int> v3 = {42};
    assert((quickSortVector(v3) == std::vector<int>{42}));

    // Already sorted ascending
    std::vector<int> v4 = {1, 2, 3, 4, 5};
    assert((quickSortVector(v4) == std::vector<int>{1, 2, 3, 4, 5}));

    // Reverse sorted (worst-case for Lomuto with last pivot)
    std::vector<int> v5 = {5, 4, 3, 2, 1};
    assert((quickSortVector(v5) == std::vector<int>{1, 2, 3, 4, 5}));

    // All equal elements
    std::vector<int> v6 = {7, 7, 7};
    assert((quickSortVector(v6) == std::vector<int>{7, 7, 7}));

    // Larger random-like sequence
    std::vector<int> v7 = {100, -50, 0, 23, 0, -1, 99, 50};
    std::vector<int> sorted7 = quickSortVector(v7);
    assert((sorted7 == std::vector<int>{-50, -1, 0, 0, 23, 50, 99, 100}));

    // Ensure original input is not modified
    std::vector<int> original = {3, 1, 2};
    std::vector<int> copy = original;
    quickSortVector(original);
    assert(original == copy);

    return 0;
}

#include <vector>

// Helper: recursively sorts a subarray of a vector using quickSort (Lomuto partition)
void quickSortHelper(std::vector<int>& arr, int low, int high) {
    if (low >= high) {
        return; // Base case: 0 or 1 element
    }

    // Partition with last element as pivot
    int pivot = arr[high];
    int i = low - 1; // index of the last element smaller than pivot

    for (int j = low; j < high; ++j) {
        if (arr[j] < pivot) {
            ++i;
            std::swap(arr[i], arr[j]);
        }
    }
    int pivotIndex = i + 1;
    std::swap(arr[pivotIndex], arr[high]);

    // Recursively sort left and right partitions
    quickSortHelper(arr, low, pivotIndex - 1);
    quickSortHelper(arr, pivotIndex + 1, high);
}

// Public function: returns a new sorted vector (does not modify input)
std::vector<int> quickSortVector(const std::vector<int>& input) {
    std::vector<int> result = input; // copy input to preserve const correctness
    if (!result.empty()) {
        quickSortHelper(result, 0, static_cast<int>(result.size()) - 1);
    }
    return result;
}

// The solution uses the classic quicksort algorithm with a recursive helper function. The main public function `quickSortVector` creates a copy of the input vector (to maintain `const` correctness and avoid modifying the original) and then invokes the recursive helper `quickSortHelper` on the full range of indices. The helper takes a reference to a mutable vector and two indices `low` and `high` representing the inclusive range to sort. The base case occurs when `low >= high` (i.e., the range has zero or one element), which is already sorted. The partition step selects the last element as the pivot, initializes an index `i` to `low - 1`, and iterates `j` from `low` to `high - 1`. Whenever an element at `j` is less than the pivot, `i` is incremented and the elements at positions `i` and `j` are swapped. After the loop, the pivot is placed at position `i + 1` by swapping with the element at `high`. The partition index `pivotIndex = i + 1` is returned, and the function recursively sorts the left subarray `[low, pivotIndex - 1]` and the right subarray `[pivotIndex + 1, high]`. Key edge cases include empty input (returns an empty vector), single element (immediately returns), arrays with all equal elements (the partition will continuously split into one-element segments, but still works correctly), and pre-sorted arrays (the Lomuto scheme with the last pivot will lead to worst-case O(n²) but functionally correct result). The time complexity averages O(n log n) and worst case O(n²) for poorly chosen pivots (e.g., sorted input), with space complexity O(log n) for recursion stack in average case and O(n) in worst case. The auxiliary space for the returned vector is O(n).
