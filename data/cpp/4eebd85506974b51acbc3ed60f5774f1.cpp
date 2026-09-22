/*
Write a standalone C++ function `bitonicSort` that takes a `std::vector<int>` and a boolean `ascending` and returns a new sorted vector using the bitonic sort algorithm. The function should work for vectors of any size (including non-power-of-two sizes by padding with `std::numeric_limits<int>::max()` for ascending or `std::numeric_limits<int>::min()` for descending, then trimming the padding from the result). For simplicity, you may implement the classic CPU-based bitonic sort using a clear, educational approach rather than GPU shaders. The input vector must not be modified; return a new sorted vector. Handle empty inputs gracefully by returning an empty vector. The sort must be stable in the sense that equal elements do not need to preserve original order (standard bitonic sort is not stable), but the algorithm must produce a fully sorted result in the correct direction.
*/
#include <vector>
#include <algorithm>
#include <limits>
#include <cstdint>

// Bitonic sort: returns a new vector sorted in the specified direction.
// The original input is not modified. Works for any size vector by padding
// with sentinel values, sorting, then removing padding.
std::vector<int> bitonicSort(const std::vector<int>& input, bool ascending) {
    if (input.empty()) return {};

    // Find next power of two >= input.size()
    size_t n = 1;
    while (n < input.size()) n <<= 1;

    // Pad with sentinel values that sort to the end
    int sentinel = ascending ? std::numeric_limits<int>::max() : std::numeric_limits<int>::min();
    std::vector<int> arr(input.begin(), input.end());
    arr.insert(arr.end(), n - input.size(), sentinel);

    // Perform bitonic sort on the padded array
    for (size_t k = 2; k <= n; k <<= 1) {
        for (size_t j = k >> 1; j > 0; j >>= 1) {
            for (size_t i = 0; i < n; ++i) {
                size_t l = i ^ j; // partner index
                if (l > i) {
                    // Determine direction for this block
                    // The block is [i/(2*j) * 2*j, (i/(2*j)+1) * 2*j)
                    bool asc = ((i / (2 * j)) % 2 == 0) == ascending;
                    if ((arr[i] > arr[l]) == asc) {
                        std::swap(arr[i], arr[l]);
                    }
                }
            }
        }
    }

    // Remove padding: trim sentinel values from the end for ascending,
    // from the beginning for descending (since sentinels sort to the correct end)
    if (ascending) {
        // Sentinels are at the end; find the first sentinel from the right
        size_t end = arr.size();
        while (end > 0 && arr[end - 1] == sentinel) --end;
        // But only trim as many as we added (input.size()..n)
        size_t keep = input.size();
        arr.resize(keep);
    } else {
        // Sentinels (INT_MIN) sort to the beginning; trim them
        size_t start = 0;
        while (start < arr.size() && arr[start] == sentinel) ++start;
        // But only trim as many as we added
        std::vector<int> result(arr.begin() + (n - input.size()), arr.end());
        return result;
    }

    return arr;
}
#include <cassert>
#include <vector>
#include <iostream>

int main() {
    // Basic ascending sort
    std::vector<int> a = {5, 2, 9, 1, 7};
    std::vector<int> a_sorted = bitonicSort(a, true);
    std::vector<int> a_expected = {1, 2, 5, 7, 9};
    assert(a_sorted == a_expected);
    assert(a == std::vector<int>({5, 2, 9, 1, 7})); // original unchanged

    // Basic descending sort
    std::vector<int> b = {5, 2, 9, 1, 7};
    std::vector<int> b_sorted = bitonicSort(b, false);
    std::vector<int> b_expected = {9, 7, 5, 2, 1};
    assert(b_sorted == b_expected);

    // Empty input
    std::vector<int> empty;
    assert(bitonicSort(empty, true).empty());
    assert(bitonicSort(empty, false).empty());

    // Single element
    std::vector<int> one = {42};
    assert(bitonicSort(one, true) == std::vector<int>({42}));
    assert(bitonicSort(one, false) == std::vector<int>({42}));

    // Power-of-two size
    std::vector<int> power = {3, 1, 4, 2};
    assert(bitonicSort(power, true) == std::vector<int>({1, 2, 3, 4}));
    assert(bitonicSort(power, false) == std::vector<int>({4, 3, 2, 1}));

    // Size 3 (non-power-of-two)
    std::vector<int> three = {3, 1, 2};
    assert(bitonicSort(three, true) == std::vector<int>({1, 2, 3}));
    assert(bitonicSort(three, false) == std::vector<int>({3, 2, 1}));

    // Duplicate values
    std::vector<int> dup = {2, 2, 1, 3};
    assert(bitonicSort(dup, true) == std::vector<int>({1, 2, 2, 3}));

    // Already sorted
    std::vector<int> sorted = {1, 2, 3, 4};
    assert(bitonicSort(sorted, true) == sorted);

    // Reverse sorted
    std::vector<int> rev = {4, 3, 2, 1};
    assert(bitonicSort(rev, true) == std::vector<int>({1, 2, 3, 4}));

    std::cout << "All tests passed!\n";
    return 0;
}
// Bitonic sort works by repeatedly building bitonic sequences (sequences that increase then decrease) and then merging them into sorted sequences. The classic implementation assumes the input length is a power of two. For arbitrary sizes, we pad the vector to the next power of two with sentinel values that will sort to the end (for ascending: `INT_MAX`; for descending: `INT_MIN`), perform the bitonic sort on the padded array, then remove the padding from the result. The core algorithm has two phases per `k` step: first, for each `j` from `k/2` down to 1, we perform compare-and-swap operations to create bitonic sequences; second, we merge them. The compare-and-swap depends on whether the element index is in the upper half of the current `j` block and on the direction (ascending or descending) determined by the bit pattern. Specifically, for each `j`, we compare elements at indices `i` and `i+j`; we swap them if they are out of order for the current sub-sequence direction. The direction for each block is determined by whether the block index (i divided by `2*j`) is even (ascending) or odd (descending). After processing all `k` values from 2 up to the padded length, the entire sequence is sorted in the desired direction. Time complexity is O(n log² n) due to log n merge steps each requiring log n comparison rounds, and space complexity is O(n) for the padded copy (plus O(1) auxiliary).
