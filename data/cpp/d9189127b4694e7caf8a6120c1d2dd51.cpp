// Write a C++ function named `countInversions` that takes a vector of integers and returns the number of inversions in it. An inversion is a pair of indices `(i, j)` such that `i < j` and `arr[i] > arr[j]`. The function must handle arrays with duplicates, negative numbers, and sizes up to 100,000 efficiently. It should return the total inversion count as a `long long`. The function must not modify the input vector.
The solution uses a modified merge sort that counts inversions during the merge step. When merging two sorted halves, if an element from the right half is smaller than an element from the left half, then that right element forms inversions with all remaining elements in the left half (since they are all greater). We count these by adding `(leftSize - leftIndex)` to the total. The algorithm decomposes the problem into counting inversions within left half, right half, and cross-inversions between halves. Edge cases include empty arrays (return 0), arrays with all duplicates (0 inversions), and already sorted arrays (0). Negative numbers don’t affect the logic. Time complexity is O(n log n) due to merge sort, and space complexity is O(n) for the temporary arrays used during merging. The function is made `const`-correct by taking the vector by const reference and creating copies only during the merge process.
#include <vector>

// Count inversions in a vector using merge sort.
// An inversion is a pair (i, j) with i < j and arr[i] > arr[j].
long long mergeAndCount(std::vector<int>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    std::vector<int> leftArr(n1);
    std::vector<int> rightArr(n2);

    for (int i = 0; i < n1; ++i) leftArr[i] = arr[left + i];
    for (int i = 0; i < n2; ++i) rightArr[i] = arr[mid + 1 + i];

    int i = 0, j = 0, k = left;
    long long invCount = 0;

    while (i < n1 && j < n2) {
        if (leftArr[i] <= rightArr[j]) {
            arr[k++] = leftArr[i++];
        } else {
            arr[k++] = rightArr[j++];
            invCount += (n1 - i); // all remaining left elements are greater
        }
    }

    while (i < n1) arr[k++] = leftArr[i++];
    while (j < n2) arr[k++] = rightArr[j++];

    return invCount;
}

long long mergeSortCount(std::vector<int>& arr, int left, int right) {
    if (left >= right) return 0;
    int mid = left + (right - left) / 2;
    long long count = 0;
    count += mergeSortCount(arr, left, mid);
    count += mergeSortCount(arr, mid + 1, right);
    count += mergeAndCount(arr, left, mid, right);
    return count;
}

// Main function that returns inversion count without modifying input.
long long countInversions(const std::vector<int>& input) {
    if (input.size() <= 1) return 0;
    std::vector<int> arr = input; // copy to allow sorting
    return mergeSortCount(arr, 0, static_cast<int>(arr.size()) - 1);
}
#include <cassert>
#include <vector>

int main() {
    // Empty and single-element arrays have zero inversions.
    assert(countInversions({}) == 0);
    assert(countInversions({5}) == 0);

    // Already sorted array has zero inversions.
    assert(countInversions({1, 2, 3, 4}) == 0);

    // Reverse sorted array has n*(n-1)/2 inversions.
    assert(countInversions({4, 3, 2, 1}) == 6);

    // Array with duplicates.
    assert(countInversions({2, 2, 2}) == 0);

    // Mixed case with negative numbers.
    assert(countInversions({-1, -2, -3}) == 3);

    // Example from typical problems.
    assert(countInversions({5, 3, 2, 4, 1}) == 8);

    // Larger unsorted array.
    assert(countInversions({9, 8, 7, 6, 5, 4, 3, 2, 1}) == 36);

    // Array with two elements.
    assert(countInversions({2, 1}) == 1);

    // Array with two equal elements.
    assert(countInversions({3, 3}) == 0);

    return 0;
}
