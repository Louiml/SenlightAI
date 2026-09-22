// Write a standalone C++ function named `countStrictlySmallerPairs` that takes two integer vectors, `A` and `B`, and returns the maximum number of disjoint index pairs `(i, j)` such that `A[i] < B[j]`. Each element from `A` and each element from `B` can be used at most once. The function must compute this maximum count efficiently. The vectors may have different sizes (including empty), and values can be negative, zero, or positive, possibly with duplicates. The order of elements in the vectors is irrelevant, but the function should return the largest possible number of such pairings.

#include <cassert>
#include <vector>

// Function declaration (for completeness, normally included from header)
int countStrictlySmallerPairs(std::vector<int> A, std::vector<int> B);

int main() {
    // Basic cases
    assert(countStrictlySmallerPairs({1, 2, 3}, {2, 3, 4}) == 3);
    assert(countStrictlySmallerPairs({1, 1, 1}, {2, 2, 2}) == 3);
    assert(countStrictlySmallerPairs({3, 3, 3}, {1, 2, 3}) == 0);
    
    // Different sizes
    assert(countStrictlySmallerPairs({1, 5, 9}, {2, 3}) == 2);
    assert(countStrictlySmallerPairs({1, 2, 3}, {4}) == 1);
    
    // Empty vectors
    assert(countStrictlySmallerPairs({}, {}) == 0);
    assert(countStrictlySmallerPairs({1}, {}) == 0);
    assert(countStrictlySmallerPairs({}, {1}) == 0);
    
    // Negative and duplicate values
    assert(countStrictlySmallerPairs({-5, -3, -1}, {-4, -2, 0}) == 3);
    assert(countStrictlySmallerPairs({-1, -1}, {0, -1}) == 0);
    assert(countStrictlySmallerPairs({0, 5, 5}, {1, 6, 6}) == 3);
    
    // Unsorted input is handled
    assert(countStrictlySmallerPairs({4, 1, 2}, {3, 5, 2}) == 2);
    
    // Larger B than A
    assert(countStrictlySmallerPairs({1, 2}, {0, 1, 2, 3, 4}) == 2);
    
    return 0;
}

#include <vector>
#include <algorithm>

// Counts the maximum number of disjoint pairs (i, j) such that A[i] < B[j].
int countStrictlySmallerPairs(std::vector<int> A, std::vector<int> B) {
    std::sort(A.begin(), A.end());
    std::sort(B.begin(), B.end());

    int answer = 0;
    std::size_t start = 0;

    for (std::size_t i = 0; i < A.size(); ++i) {
        bool found = false;
        for (std::size_t j = start; j < B.size(); ++j) {
            if (A[i] < B[j]) {
                ++answer;
                start = j + 1;  // This element B[j] is used, skip it next time.
                found = true;
                break;
            }
        }
        if (!found) {
            // Remaining A[i] are even larger, so no further matches possible.
            break;
        }
    }
    return answer;
}

// The goal is to maximize the number of pairs, which is a classic greedy matching problem on two sorted sequences. The key observation: if both vectors are sorted in non-decreasing order, the optimal strategy is to iterate through the smaller or equal candidate elements from `A` and try to match each with the smallest possible element in `B` that is strictly greater. This leaves larger elements in `B` for potential matches with later (larger) elements of `A`. Using two pointers, we can avoid revisiting elements in `B`. Specifically, sort both vectors, then use a pointer `start` into `B`. For each element in `A` (in ascending order), scan forward from `start` in `B` to find the first element greater than `A[i]`. If found, increment the answer and move `start` past that matched element (i.e., `start = j + 1`). If not found, we can break because all remaining elements in `A` are even larger, so no later element will match. Edge cases: empty vectors yield 0; duplicate values in `A` or `B` are handled naturally because we skip matched elements and require strict inequality; if `B` has fewer elements than `A`, the answer is at most `B.size()`. Time complexity is O(n log n + m log m) due to sorting, and the scanning is O(n + m) after sorting, so overall O(n log n + m log m). Space complexity is O(1) auxiliary (ignoring input storage and sort overhead).
