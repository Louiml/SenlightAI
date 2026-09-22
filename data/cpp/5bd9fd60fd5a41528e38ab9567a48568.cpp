Write a C++ function `int selectionRank(const std::vector<int>& values, int rank)` that takes a vector of integers and a rank `k` (1-indexed) and returns the k-th smallest element in the vector. The function must use a partial insertion sort approach similar to the one in the provided Stockfish code: it should keep a sorted prefix of the vector (initially empty) and, for each element, if the element is smaller than the current largest in the sorted prefix, insert it into the correct position and discard the largest if the prefix exceeds size `k`. The function should not modify the input vector and should work for any `1 ≤ k ≤ values.size()`. Handle edge cases such as duplicate values and very large inputs, and ensure the time complexity is O(n * k) in the worst case and O(n) when k is small, with O(k) auxiliary space.

// The main idea mirrors the `partial_insertion_sort` in the provided code: maintain a sorted list (using `std::vector<int>`) that holds the smallest `k` elements seen so far. Iterate through the input. For each value `x`, if the current sorted list has fewer than `k` elements, insert `x` in sorted order. Otherwise, if `x` is less than the largest element (the back of the sorted list), remove that largest element and insert `x` in sorted order. This ensures the list always contains the smallest `min(k, processed)` elements. At the end, the k-th smallest is the back of the list. Since the list size is at most `k`, inserting into it via linear scan (or using binary search with vector insertion) takes O(k) per operation, leading to O(n * k) time in the worst case. For small `k`, this is efficient. The auxiliary space is O(k). Edge cases include duplicate values—they are handled naturally because duplicates will be inserted and possibly removed; the k-th smallest still correctly reflects duplicates. If `k` equals the size, the sorted list will contain all elements and the back is the maximum. The function must not modify the input, so it copies values into the sorted list. We also consider `k` being 1: the function returns the minimum. For `k` equal to `n`, it returns the maximum. Time complexity: O(n * k) worst-case, O(n) when k is constant (since insertion is O(k) but k is constant). Space: O(k) for the sorted list.

#include <vector>
#include <algorithm>

// Returns the k-th smallest element (1-indexed) from the given vector.
// Uses a partial insertion sort approach: maintains a sorted list of the
// smallest k elements seen so far. Does not modify the input.
int selectionRank(const std::vector<int>& values, int rank) {
    std::vector<int> sorted; // sorted ascending, size at most rank
    sorted.reserve(rank);    // avoid reallocations

    for (int x : values) {
        if (static_cast<int>(sorted.size()) < rank) {
            // Insert x in sorted position (linear insertion)
            auto it = std::upper_bound(sorted.begin(), sorted.end(), x);
            sorted.insert(it, x);
        } else if (x < sorted.back()) {
            // Remove the largest (which is at back) and insert x
            sorted.pop_back();
            auto it = std::upper_bound(sorted.begin(), sorted.end(), x);
            sorted.insert(it, x);
        }
        // If x >= sorted.back() and we are full, do nothing
    }
    // The k-th smallest is the last element of the sorted list
    return sorted.back();
}

#include <cassert>
#include <vector>

int selectionRank(const std::vector<int>& values, int rank); // declaration

int main() {
    // Basic test
    assert(selectionRank({5, 3, 1, 4, 2}, 3) == 3);
    // Duplicate values
    assert(selectionRank({2, 2, 2, 2}, 2) == 2);
    assert(selectionRank({1, 2, 2, 3}, 3) == 2);
    // Single element
    assert(selectionRank({42}, 1) == 42);
    // k equals size (returns max)
    assert(selectionRank({1, 5, 3, 9, 2}, 5) == 9);
    // k=1 returns min
    assert(selectionRank({-5, 10, 0, 3, -2}, 1) == -5);
    // Negative and large
    assert(selectionRank({-3, -7, -1, -9}, 4) == -1);
    // Larger input
    std::vector<int> big = {7, 8, 1, 3, 10, 4, 2, 9, 6, 5};
    assert(selectionRank(big, 6) == 6);
    // Verify sorted list actually contains correct values (indirectly)
    // The second smallest of {8, 6, 7, 5} is 6
    assert(selectionRank({8, 6, 7, 5}, 2) == 6);
    return 0;
}
