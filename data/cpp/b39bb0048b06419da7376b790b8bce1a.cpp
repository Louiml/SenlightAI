Write a C++ function that takes a vector of non-negative integers, a left index `l` and a right index `r` (both 0-based, with `l <= r`), and returns the bitwise AND of all elements in the inclusive range `[l, r]`. The function should support repeated range queries on a static array, and must be efficient for up to 200,000 elements and up to 200,000 queries. The array values are stored in a 1-indexed segment tree for efficient querying (the tree is zero-indexed internally but values are placed starting at index `n`). The query must return the correct bitwise AND even when the range spans multiple segments, and must handle single-element ranges correctly.
#include <cassert>
#include <vector>
#include <climits>

// (declaration of rangeAnd is assumed to be included)

int main() {
    // Single element
    std::vector<int> a = {7};
    assert(rangeAnd(a, 0, 0) == 7);
    assert(rangeAnd(a, 0, 0) == 7);

    // Small array
    std::vector<int> b = {5, 3, 8, 6};
    // 5&3 = 1, 3&8 = 0, 8&6 = 0, whole = 0
    assert(rangeAnd(b, 0, 1) == (5 & 3));
    assert(rangeAnd(b, 1, 2) == (3 & 8));
    assert(rangeAnd(b, 2, 3) == (8 & 6));
    assert(rangeAnd(b, 0, 3) == 0);
    assert(rangeAnd(b, 1, 1) == 3);

    // All identical values
    std::vector<int> c = {12, 12, 12};
    assert(rangeAnd(c, 0, 2) == 12);

    // Large values
    std::vector<int> d = {INT_MAX, INT_MAX, 0, INT_MAX};
    assert(rangeAnd(d, 0, 1) == INT_MAX);
    assert(rangeAnd(d, 0, 2) == 0);
    assert(rangeAnd(d, 2, 3) == 0);
    assert(rangeAnd(d, 1, 3) == 0);

    // Many elements (power of two)
    std::vector<int> e(8, 15);
    e[3] = 0;
    e[5] = 7;
    // query [1,6] (inclusive) -> indices 1..6
    int expected = 15 & 15 & 0 & 15 & 7 & 15;
    assert(rangeAnd(e, 1, 6) == expected);

    return 0;
}
#include <vector>
#include <cstdint>
#include <climits>

// Returns bitwise AND of arr[l..r] inclusive (0-based indices).
// The input vector is considered static; the segment tree is built here.
int rangeAnd(const std::vector<int>& input, int l, int r) {
    const int n = static_cast<int>(input.size());
    // Allocate segment tree of size 2*n (1-indexed internally)
    std::vector<int> tree(2 * n);
    // Copy input to leaves at indices n..2n-1
    for (int i = 0; i < n; ++i) {
        tree[i + n] = input[i];
    }
    // Build internal nodes
    for (int i = n - 1; i > 0; --i) {
        tree[i] = tree[2 * i] & tree[2 * i + 1];
    }

    // Convert inclusive [l, r] to half-open [l, r+1)
    int left = l + n;
    int right = r + 1 + n;  // r + 1 is the exclusive end

    int answer = INT_MAX;  // bitwise AND identity: all bits set
    bool first = true;

    while (left < right) {
        if (left & 1) {
            if (first) {
                answer = tree[left++];
                first = false;
            } else {
                answer &= tree[left++];
            }
        }
        if (right & 1) {
            if (first) {
                answer = tree[--right];
                first = false;
            } else {
                answer &= tree[--right];
            }
        }
        left >>= 1;
        right >>= 1;
    }
    return answer;
}
// The solution uses a segment tree where each node stores the bitwise AND of its segment. The array is stored at leaves `tree[n+i]` for `i = 0..n-1`. The internal nodes are built bottom-up using `tree[i] = tree[2*i] & tree[2*i+1]`. For a query `[l, r]` (0-based), convert to half-open `[l, r+1)` and iterate over the segment tree using the standard iterative approach: `l += n, r += n` and while `l < r`, if `l` is odd, include `tree[l]` and increment `l`; if `r` is odd, decrement `r` and include `tree[r]`. The result accumulates using bitwise AND, initializing the answer with the first included value. Edge cases: `l == r` returns `tree[l+n]`; identical values; range covering entire array; large values up to 2^31-1. Time complexity: build O(n), each query O(log n). Space O(n).
