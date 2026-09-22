/*
Write a C++ function `std::pair<int, int> findFirstAndLast(const std::vector<int>& arr, int x)` that takes a sorted vector of integers (which may contain duplicates) and a target value `x`, and returns a pair containing the first and last indices (0-based) where `x` appears in the array. If `x` is not present, return `{-1, -1}`. The function must be efficient for large arrays, using a binary-search-based approach rather than a linear scan. The input vector is guaranteed to be sorted in non-decreasing order. Your implementation should handle edge cases such as an empty vector, a single-element vector, and duplicate values of `x` appearing at the beginning or end of the array. You must not modify the input vector and should apply `const` correctness.
*/

#include <vector>
#include <utility>

// Returns the first and last occurrence indices of x in a sorted vector.
// If x is absent, returns {-1, -1}.
std::pair<int, int> findFirstAndLast(const std::vector<int>& arr, int x) {
    int n = static_cast<int>(arr.size());
    int first = -1, last = -1;

    // Binary search for first occurrence
    int low = 0, high = n - 1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (arr[mid] == x) {
            first = mid;
            high = mid - 1; // continue left
        } else if (arr[mid] < x) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    // Binary search for last occurrence
    low = 0;
    high = n - 1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (arr[mid] == x) {
            last = mid;
            low = mid + 1; // continue right
        } else if (arr[mid] < x) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    return {first, last};
}

#include <cassert>
#include <vector>
#include <utility>

int main() {
    // Basic case with duplicates in middle
    std::vector<int> arr1 = {1, 2, 2, 2, 2, 3, 4, 7, 8, 8};
    auto res1 = findFirstAndLast(arr1, 2);
    assert(res1.first == 1 && res1.last == 4);

    // Target at end
    auto res2 = findFirstAndLast(arr1, 8);
    assert(res2.first == 8 && res2.last == 9);

    // Target at beginning
    auto res3 = findFirstAndLast(arr1, 1);
    assert(res3.first == 0 && res3.last == 0);

    // Target not present
    auto res4 = findFirstAndLast(arr1, 5);
    assert(res4.first == -1 && res4.last == -1);

    // Empty vector
    std::vector<int> arr2;
    auto res5 = findFirstAndLast(arr2, 10);
    assert(res5.first == -1 && res5.last == -1);

    // Single element present
    std::vector<int> arr3 = {42};
    auto res6 = findFirstAndLast(arr3, 42);
    assert(res6.first == 0 && res6.last == 0);

    // Single element absent
    auto res7 = findFirstAndLast(arr3, 7);
    assert(res7.first == -1 && res7.last == -1);

    // All duplicates
    std::vector<int> arr4 = {5, 5, 5, 5};
    auto res8 = findFirstAndLast(arr4, 5);
    assert(res8.first == 0 && res8.last == 3);

    // Negative numbers
    std::vector<int> arr5 = {-10, -5, -5, 0, 3};
    auto res9 = findFirstAndLast(arr5, -5);
    assert(res9.first == 1 && res9.last == 2);

    return 0;
}

// The solution uses two separate binary searches to locate the first and last occurrences independently. For the first occurrence: perform a standard binary search, but when `arr[mid] == x`, record `mid` as a candidate answer and continue searching in the left half (`end = mid - 1`) to see if an earlier occurrence exists. For the last occurrence: when `arr[mid] == x`, record `mid` and continue searching in the right half (`start = mid + 1`). If the target is not found, the initial `res = -1` remains unchanged. Edge cases: empty array (immediately return `{-1, -1}`), single element (binary search works), and duplicates at boundaries (handled by continuing search after recording). Time complexity is \(O(\log n)\) for each search, so \(O(\log n)\) total; space complexity is \(O(1)\) auxiliary, not counting the input vector. The algorithm is robust and does not rely on linear scanning.
