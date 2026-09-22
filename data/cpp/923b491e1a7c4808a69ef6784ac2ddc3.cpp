Write a C++ function named `hIndex` that takes a vector of non-negative integers representing the number of citations each researcher's paper has received, and returns the researcher's h-index. The h-index is defined as the maximum value `h` such that the researcher has at least `h` papers with at least `h` citations each. For example, given citations `[3, 0, 6, 1, 5]`, the h-index is 3 because there are 3 papers with at least 3 citations, but not 4 papers with at least 4 citations. The input vector may be empty (in which case the h-index is 0), may contain duplicates, and may contain zeros. No sorting is allowed; you must implement a simple, brute-force approach.
// The intuitive definition of the h-index suggests scanning possible h values from the maximum citation count down to 0. For each candidate h, count how many papers have at least `h` citations. If that count is ≥ `h`, then `h` is a valid h-index; since we start from the highest possible value and go downward, the first valid h we find is the maximum valid h, i.e., the answer. The maximum possible h cannot exceed the number of papers, nor the largest citation count, but starting from the largest citation count is sufficient. Edge cases: empty vector → loop from 0 down to 0, count is 0, check `cnt >= i` → `0 >= 0` true, answer = 0. All zeros → max_c=0, loop i=0, count papers with ≥0 citations = size, which is ≥0, answer=0. If all citations are huge, the loop runs many times but still correct. The algorithm is O(max_c * n) time and O(1) auxiliary space.
#include <vector>
#include <algorithm>

// Return the h-index given a vector of non-negative citation counts.
int hIndex(const std::vector<int>& citations) {
    int max_c = 0;
    for (int c : citations) {
        max_c = std::max(max_c, c);
    }

    for (int h = max_c; h >= 0; --h) {
        int count = 0;
        for (int c : citations) {
            if (c >= h) {
                ++count;
            }
        }
        if (count >= h) {
            return h;
        }
    }
    return 0; // Should never reach here, but for safety.
}
#include <cassert>
#include <vector>

int main() {
    // Example from the problem
    assert(hIndex({3, 0, 6, 1, 5}) == 3);
    // Empty input
    assert(hIndex({}) == 0);
    // All zeros
    assert(hIndex({0, 0, 0}) == 0);
    // Single paper with high citation
    assert(hIndex({10}) == 1);
    // Duplicates and zeros
    assert(hIndex({0, 1, 1, 1}) == 1);
    // All citations equal
    assert(hIndex({5, 5, 5, 5}) == 4);
    // Large numbers
    assert(hIndex({100, 100, 100}) == 3);
    // Mixed values including max_c == n
    assert(hIndex({1, 2, 3, 4, 5}) == 3);
    // Reverse order
    assert(hIndex({5, 4, 3, 2, 1}) == 3);
}
