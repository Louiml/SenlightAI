// Given an array of `n` integers (where `n` is odd and at least 1) on a single line, write a C++ function that rearranges the array so that all elements at odd indices (1-based) are in non-decreasing order and are all less than or equal to every element at even indices (1-based), while also ensuring the elements at even indices are in non-decreasing order. More precisely, after sorting the entire array in ascending order, place the smaller half (first `(n+1)/2` elements) into the odd positions in order, and the larger half (remaining `(n-1)/2` elements) into the even positions in order. The function should return a `std::vector<int>` containing the rearranged sequence. The input array may contain duplicates, and the output must preserve the sorted order within each parity group. For example, with input `{5, 2, 1, 4, 3}`, the sorted array is `{1,2,3,4,5}`, odd positions get `{1,3,5}` and even positions get `{2,4}`, producing output `{1,2,3,4,5}`.
The solution first sorts the input array in ascending order using `std::sort`. Since the array is sorted, the first `(n+1)/2` elements are the smaller half and the remaining `(n-1)/2` are the larger half. We then create an output vector of the same size. We use two loops: one iterating over odd indices (1-based: 1, 3, 5, ...) and assigning the next smallest elements from the sorted array in order, and another over even indices (2, 4, 6, ...) assigning the remaining larger elements in order. This guarantees that odd-indexed elements are all ≤ even-indexed elements because the entire smaller half is placed in odd positions and the larger half in even positions, and within each group the order is preserved due to the sorted input. Edge cases: when `n` is 1, there are no even positions and the output is simply the single element. Duplicates are handled naturally because sorting groups equal values together, and the assignment works regardless. Time complexity is O(n log n) due to sorting, and space complexity is O(n) for the output vector (excluding input storage). The approach is straightforward and robust for any odd `n`.
#include <vector>
#include <algorithm>

// Rearrange the array so that odd-indexed (1-based) elements are the smaller half
// sorted ascending, and even-indexed elements are the larger half sorted ascending.
std::vector<int> rearrangeOddEven(const std::vector<int>& input) {
    std::vector<int> sorted = input;
    std::sort(sorted.begin(), sorted.end());

    const int n = static_cast<int>(sorted.size());
    std::vector<int> result(n);

    int index = 0;
    // Odd positions (1-based): 1, 3, 5, ... -> 0-based indices 0, 2, 4, ...
    for (int i = 0; i < n; i += 2) {
        result[i] = sorted[index];
        ++index;
    }
    // Even positions (1-based): 2, 4, 6, ... -> 0-based indices 1, 3, 5, ...
    for (int i = 1; i < n; i += 2) {
        result[i] = sorted[index];
        ++index;
    }

    return result;
}
#include <cassert>
#include <vector>

// Declaration of the function to test (assuming it's defined above or included)
std::vector<int> rearrangeOddEven(const std::vector<int>& input);

int main() {
    // Basic odd-length input
    assert(rearrangeOddEven({5, 2, 1, 4, 3}) == std::vector<int>({1, 2, 3, 4, 5}));
    // Unsorted input with duplicates
    assert(rearrangeOddEven({3, 1, 2, 1, 3}) == std::vector<int>({1, 1, 2, 3, 3}));
    // Already sorted
    assert(rearrangeOddEven({1, 2, 3}) == std::vector<int>({1, 2, 3}));
    // Single element
    assert(rearrangeOddEven({42}) == std::vector<int>({42}));
    // Larger odd-sized array with negative numbers
    assert(rearrangeOddEven({-3, 0, 5, -1, 2, 4, 1}) == std::vector<int>({-3, -1, 0, 1, 2, 4, 5}));
    // All equal values
    assert(rearrangeOddEven({7, 7, 7, 7, 7}) == std::vector<int>({7, 7, 7, 7, 7}));
    // Random order with duplicates
    assert(rearrangeOddEven({10, 1, 9, 2, 8, 3, 7}) == std::vector<int>({1, 2, 3, 7, 8, 9, 10}));
    // Already correct arrangement
    assert(rearrangeOddEven({1, 2, 3, 4, 5, 6, 7}) == std::vector<int>({1, 2, 3, 4, 5, 6, 7}));
    // Only two elements (n=2 is even, but we test that it still works for even sizes)
    assert(rearrangeOddEven({2, 1}) == std::vector<int>({1, 2}));
    return 0;
}
