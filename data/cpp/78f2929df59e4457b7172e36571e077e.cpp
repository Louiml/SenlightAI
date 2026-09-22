// Write a C++ function named `selection_sort` that takes a `std::vector<int>&` by non-const reference and sorts its elements in ascending order using the selection sort algorithm. The function must not return anything, must not use any standard library sorting functions (e.g., `std::sort`), and must operate in-place. Your implementation should correctly handle vectors of any size, including empty vectors and vectors with duplicate values. Additionally, provide a separate `is_sorted_ascending` helper function (non-const, taking a `const std::vector<int>&`) that returns `true` if the vector is sorted in non-decreasing order, and `false` otherwise; this is useful for testing but must be implemented as part of the solution. The main focus is on the sorting function; ensure the selection sort algorithm is implemented exactly: for each position from the beginning to the second-to-last, find the index of the minimum element in the unsorted suffix and swap it with the current position.
The solution uses selection sort, which divides the array into a sorted prefix and an unsorted suffix. For each index `i` from `0` to `n-2`, we scan the subarray from `i` to `n-1` to find the index of the smallest element. After finding it, we swap the element at that index with the element at `i`. This builds the sorted prefix one element at a time. Edge cases: an empty vector or a vector of size 1 requires no swaps; the algorithm naturally handles duplicates because we swap only when a strictly smaller element is found (or we can swap even on equal, but it is safe to compare `<`). The time complexity is O(n²) in all cases because the inner scan always runs for the remaining suffix, regardless of input order. Space complexity is O(1) auxiliary because only a few temporary variables are used. The helper `is_sorted_ascending` simply iterates through the vector and checks that each adjacent pair is in non-decreasing order; it returns `true` for empty or single-element vectors.
#include <vector>

// Perform selection sort in ascending order on the input vector.
void selection_sort(std::vector<int>& arr) {
    int n = static_cast<int>(arr.size());
    for (int i = 0; i < n - 1; ++i) {
        int min_index = i;
        // Find the index of the smallest element in the unsorted suffix.
        for (int j = i + 1; j < n; ++j) {
            if (arr[j] < arr[min_index]) {
                min_index = j;
            }
        }
        // Swap the found minimum with the first element of the suffix.
        if (min_index != i) {
            std::swap(arr[i], arr[min_index]);
        }
    }
}

// Check if the given vector is sorted in non-decreasing order.
bool is_sorted_ascending(const std::vector<int>& arr) {
    for (size_t i = 1; i < arr.size(); ++i) {
        if (arr[i - 1] > arr[i]) {
            return false;
        }
    }
    return true;
}
#include <cassert>
#include <vector>

// Forward declarations (already defined in the solution).
void selection_sort(std::vector<int>& arr);
bool is_sorted_ascending(const std::vector<int>& arr);

int main() {
    // Test 1: empty vector
    std::vector<int> v1;
    selection_sort(v1);
    assert(v1.empty());
    assert(is_sorted_ascending(v1));

    // Test 2: single element
    std::vector<int> v2 = {42};
    selection_sort(v2);
    assert(v2 == std::vector<int>{42});
    assert(is_sorted_ascending(v2));

    // Test 3: already sorted
    std::vector<int> v3 = {1, 2, 3, 4, 5};
    selection_sort(v3);
    assert(v3 == std::vector<int>({1, 2, 3, 4, 5}));
    assert(is_sorted_ascending(v3));

    // Test 4: reverse sorted
    std::vector<int> v4 = {5, 4, 3, 2, 1};
    selection_sort(v4);
    assert(v4 == std::vector<int>({1, 2, 3, 4, 5}));
    assert(is_sorted_ascending(v4));

    // Test 5: unsorted with duplicates
    std::vector<int> v5 = {3, 1, 2, 1, 3, 0};
    selection_sort(v5);
    assert(v5 == std::vector<int>({0, 1, 1, 2, 3, 3}));
    assert(is_sorted_ascending(v5));

    // Test 6: all equal
    std::vector<int> v6 = {7, 7, 7, 7};
    selection_sort(v6);
    assert(v6 == std::vector<int>({7, 7, 7, 7}));
    assert(is_sorted_ascending(v6));

    // Test 7: negative numbers
    std::vector<int> v7 = {-10, -5, -20, 0, 15};
    selection_sort(v7);
    assert(v7 == std::vector<int>({-20, -10, -5, 0, 15}));
    assert(is_sorted_ascending(v7));

    // Test 8: larger vector with random order
    std::vector<int> v8 = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0, -1};
    selection_sort(v8);
    assert(v8 == std::vector<int>({-1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9}));
    assert(is_sorted_ascending(v8));

    return 0;
}
