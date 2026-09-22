// Given a sequence of \(n\) integers (where \(n \ge 1\)), write a C++ function `minimumWindowToSort` that takes a `std::vector<long long>` and returns an `int` representing the length of the shortest contiguous subarray that, if sorted alone, would make the entire array sorted in non-decreasing order. If the array is already sorted, return 0. The input array can contain duplicate values, and the required subarray is defined as the segment from the first index (0-based) where the element differs from its sorted position to the last such index; the length is `last_index - first_index + 1`. Your function should operate directly on the given vector without modifying it, and it must handle large values and up to \(10^5\) elements efficiently.
#include <cassert>
#include <vector>

int main() {
    // Already sorted array
    assert(minimumWindowToSort({1, 2, 3, 4, 5}) == 0);
    // Single element
    assert(minimumWindowToSort({42}) == 0);
    // Already sorted with duplicates
    assert(minimumWindowToSort({1, 1, 2, 2, 3}) == 0);
    // Entire array unsorted (reverse order)
    assert(minimumWindowToSort({5, 4, 3, 2, 1}) == 5);
    // Middle segment unsorted
    assert(minimumWindowToSort({1, 3, 2, 4, 5}) == 2);
    // Unsorted segment at start
    assert(minimumWindowToSort({3, 1, 2, 4, 5}) == 3);
    // Unsorted segment at end
    assert(minimumWindowToSort({1, 2, 4, 3, 5}) == 2);
    // Duplicates in unsorted segment
    assert(minimumWindowToSort({1, 5, 3, 3, 7}) == 3);
    // Large values
    assert(minimumWindowToSort({1000000000000LL, 1LL, 2LL}) == 3);
    // Already sorted with duplicates at boundaries
    assert(minimumWindowToSort({2, 2, 1, 1, 3}) == 4);
    return 0;
}
#include <vector>
#include <algorithm>

// Returns the length of the shortest contiguous subarray that, if sorted,
// would make the entire vector sorted in non-decreasing order.
// Returns 0 if the vector is already sorted.
int minimumWindowToSort(const std::vector<long long>& arr) {
    if (arr.size() <= 1) {
        return 0;
    }

    std::vector<long long> sorted = arr;
    std::sort(sorted.begin(), sorted.end());

    size_t left = 0;
    size_t right = arr.size() - 1;

    // Find first index where original differs from sorted
    while (left < arr.size() && arr[left] == sorted[left]) {
        ++left;
    }

    // If left reached end, array is already sorted
    if (left == arr.size()) {
        return 0;
    }

    // Find last index where original differs from sorted
    while (right > left && arr[right] == sorted[right]) {
        --right;
    }

    return static_cast<int>(right - left + 1);
}
// The problem reduces to identifying the smallest interval that contains all "misplaced" elements relative to the sorted version of the array. First, create a sorted copy of the input vector. Then, iterate through both the original and sorted vectors simultaneously, tracking the first index (`left`) where they differ and the last index (`right`) where they differ. If no difference is found, the array is already sorted and the answer is 0; otherwise, the answer is `right - left + 1`. This works because any element that is out of place must lie inside this interval, and sorting that entire interval will place all its elements correctly while preserving the order of elements outside it. Edge cases include an array that is already sorted (answer 0), a single element (answer 0), duplicates (they are handled naturally since equality checks compare values), and arrays where only a small middle segment is unsorted (the interval captures exactly that segment). Time complexity is \(O(n \log n)\) due to sorting, and space complexity is \(O(n)\) for the sorted copy. The function is `const`-correct, taking the input by const reference and returning an `int`.
