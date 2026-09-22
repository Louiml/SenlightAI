// Write a C++ function named `mergeSortIntegers` that takes a reference to a `std::vector<int>` and sorts it in ascending order using the merge sort algorithm. The function must implement the divide-and-conquer strategy with explicit recursive splitting and merging, and must not use any built-in sorting functions. The input vector may be empty, contain duplicate values, or contain negative numbers. The function should modify the vector in place and return `void`. Ensure the implementation handles edge cases such as vectors of size 0 or 1 correctly.

The solution recursively divides the array into two halves until each subarray has at most one element, then merges the halves back in sorted order. The merge step uses two temporary arrays (or vectors) to hold the left and right halves, then compares elements from both and writes the smaller to the original array, followed by copying any remaining elements from either half. Edge cases include empty or single-element arrays (base case returns immediately) and duplicate values (comparison uses `<` so equal elements remain stable). Time complexity is \(O(n \log n)\) for all cases because the array is always divided in half and each merge takes \(O(n)\). Space complexity is \(O(n)\) due to temporary arrays created during each merge call, though the recursion depth contributes \(O(\log n)\) additional stack space.

#include <vector>

// Sorts a vector of integers in ascending order using merge sort.
void mergeSortIntegers(std::vector<int>& arr) {
    // Helper lambda for recursive merge sort
    void mergeSortHelper(std::vector<int>& a, int left, int right);
    mergeSortHelper(arr, 0, static_cast<int>(arr.size()) - 1);
}

// Recursive helper function
void mergeSortHelper(std::vector<int>& a, int left, int right) {
    // Base case: zero or one element
    if (left >= right) return;

    int mid = left + (right - left) / 2;

    // Sort left and right halves
    mergeSortHelper(a, left, mid);
    mergeSortHelper(a, mid + 1, right);

    // Merge two sorted halves
    std::vector<int> leftPart(a.begin() + left, a.begin() + mid + 1);
    std::vector<int> rightPart(a.begin() + mid + 1, a.begin() + right + 1);

    int i = 0, j = 0, k = left;
    int leftSize = static_cast<int>(leftPart.size());
    int rightSize = static_cast<int>(rightPart.size());

    while (i < leftSize && j < rightSize) {
        if (leftPart[i] <= rightPart[j]) {
            a[k++] = leftPart[i++];
        } else {
            a[k++] = rightPart[j++];
        }
    }

    while (i < leftSize) a[k++] = leftPart[i++];
    while (j < rightSize) a[k++] = rightPart[j++];
}

#include <cassert>
#include <vector>
#include <algorithm>

// Forward declaration of the solution function (since no main in solution)
void mergeSortIntegers(std::vector<int>& arr);

int main() {
    // Empty vector
    std::vector<int> v1;
    mergeSortIntegers(v1);
    assert(v1.empty());

    // Single element
    std::vector<int> v2 = {5};
    mergeSortIntegers(v2);
    assert(v2.size() == 1 && v2[0] == 5);

    // Already sorted
    std::vector<int> v3 = {1, 2, 3, 4};
    mergeSortIntegers(v3);
    assert((v3 == std::vector<int>{1, 2, 3, 4}));

    // Reverse sorted
    std::vector<int> v4 = {9, 5, 1, 0, -3};
    mergeSortIntegers(v4);
    assert((v4 == std::vector<int>{-3, 0, 1, 5, 9}));

    // Duplicates and negatives
    std::vector<int> v5 = {3, -1, 2, -1, 3, 0, -5};
    mergeSortIntegers(v5);
    assert((v5 == std::vector<int>{-5, -1, -1, 0, 2, 3, 3}));

    // Large random test comparing with std::sort
    std::vector<int> v6 = {10, 7, 8, 9, 1, 5, 0, -2, 100, -100};
    std::vector<int> expected = v6;
    std::sort(expected.begin(), expected.end());
    mergeSortIntegers(v6);
    assert(v6 == expected);

    return 0;
}
