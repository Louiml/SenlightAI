// Write a C++ function that takes a permutation of integers from `0` to `n-1` (inclusive) and a special index `x`, and returns a vector containing the permutation sorted in a specific way: the elements `0, 1, ..., n-1` must appear exactly once, but the element at index `x` must be moved to the very end of the sequence, while all other elements remain in their natural increasing order. If `x == n-1`, the permutation is already in natural order, so no change is needed. The function should handle the case where `n` is a positive integer and `x` is guaranteed to be a valid index in `[0, n-1]`. The output must be a vector of integers, and the function should be efficient for large `n`.

The task is straightforward: we need to construct a sequence of numbers from `0` to `n-1` in increasing order, but with the special element `x` removed from its natural position and appended at the end. The main algorithm is:  
1. If `x == n-1`, then the sequence is already `0, 1, ..., n-1` and we can return that directly.  
2. Otherwise, iterate through `i` from `0` to `n-1`. For each `i`, if `i != x`, append `i` to the result. Finally, append `x` at the end.  
Important edge cases:  
- When `x == 0`, the sequence becomes `1, 2, ..., n-1, 0`.  
- When `n == 1`, the only valid index is `x == 0`, and since `x == n-1`, we return `[0]`.  
Time complexity is `O(n)` because we iterate over all `n` elements once. Space complexity is `O(n)` for the output vector (not counting input parameters). No additional auxiliary space is used except for a few loop variables.

#include <vector>

// Return a permutation of [0, n) where element x is moved to the end,
// keeping all other elements in increasing order.
std::vector<int> moveElementToEnd(int n, int x) {
    std::vector<int> result;
    result.reserve(n);

    // If x is already at the last position, the natural order is correct.
    if (x == n - 1) {
        for (int i = 0; i < n; ++i) {
            result.push_back(i);
        }
        return result;
    }

    // Append all elements except x in natural order.
    for (int i = 0; i < n; ++i) {
        if (i != x) {
            result.push_back(i);
        }
    }
    // Append x at the end.
    result.push_back(x);

    return result;
}

#include <cassert>
#include <vector>

int main() {
    // Basic cases
    assert(moveElementToEnd(5, 3) == std::vector<int>({0, 1, 2, 4, 3}));
    assert(moveElementToEnd(5, 0) == std::vector<int>({1, 2, 3, 4, 0}));
    assert(moveElementToEnd(1, 0) == std::vector<int>({0}));

    // Case where x is already at the end
    assert(moveElementToEnd(5, 4) == std::vector<int>({0, 1, 2, 3, 4}));
    assert(moveElementToEnd(2, 1) == std::vector<int>({0, 1}));

    // Larger n, arbitrary x
    assert(moveElementToEnd(10, 7) == std::vector<int>({0, 1, 2, 3, 4, 5, 6, 8, 9, 7}));
    assert(moveElementToEnd(10, 9) == std::vector<int>({0, 1, 2, 3, 4, 5, 6, 7, 8, 9}));

    // Edge: x = n-1 for n=3
    assert(moveElementToEnd(3, 2) == std::vector<int>({0, 1, 2}));

    // Edge: n=2, x=0
    assert(moveElementToEnd(2, 0) == std::vector<int>({1, 0}));

    return 0;
}
