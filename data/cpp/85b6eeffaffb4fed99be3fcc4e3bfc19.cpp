// Write a C++ function that takes an integer array `a` of size `n = 2^p` (where `p` is a non-negative integer) and returns a vector of length `2n - 1` representing the results of a segment-tree-like reduction. The function should recursively divide the array into two halves until reaching single elements, then for each node (segment) compute the maximum of that segment and store it in the output vector at the position corresponding to that node's index in a 1-based heap layout: the root is at index 1, its left child at 2, right child at 3, and so on. The output vector `b` should be 0-indexed, so that the node at heap index `i` is stored at `b[i-1]`. The leaves (single-element segments) are stored in order in the second half of `b`. The function must handle the edge case where `n = 1`, in which case the output vector has length 1 and contains only the single element. The original array must not be modified, and the function should be `const`-correct with respect to its input. Do not use global variables; return the result. Test with various `p` values including `p=0`.

#include <cassert>
#include <vector>
#include <functional>

int main() {
    // p = 0 -> n = 1
    std::vector<int> a1 = {42};
    auto r1 = segmentTreeMax(a1);
    assert(r1.size() == 1 && r1[0] == 42);

    // p = 1 -> n = 2
    std::vector<int> a2 = {3, 7};
    auto r2 = segmentTreeMax(a2);
    // tree: root max(3,7)=7, leaves: 3,7
    assert(r2.size() == 3 && r2[0] == 7 && r2[1] == 3 && r2[2] == 7);

    // p = 2 -> n = 4
    std::vector<int> a3 = {5, 1, 9, 2};
    auto r3 = segmentTreeMax(a3);
    // tree: root max=9, left child max(5,1)=5, right child max(9,2)=9, leaves: 5,1,9,2
    assert(r3.size() == 7 && r3[0] == 9 && r3[1] == 5 && r3[2] == 9 && r3[3] == 5 && r3[4] == 1 && r3[5] == 9 && r3[6] == 2);

    // p = 3 -> n = 8, test with all same values
    std::vector<int> a4(8, 4);
    auto r4 = segmentTreeMax(a4);
    assert(r4.size() == 15);
    for (int val : r4) assert(val == 4);

    // p = 3 -> n = 8, test descending
    std::vector<int> a5 = {8,7,6,5,4,3,2,1};
    auto r5 = segmentTreeMax(a5);
    // root is 8, left subtree root is 8 (since left half has 8,7,6,5), right subtree root is 4
    assert(r5[0] == 8 && r5[1] == 8 && r5[2] == 4);
    // leaves are the original values
    for (int i = 0; i < 8; ++i) {
        assert(r5[7 + i] == a5[i]);
    }

    // p = 3 -> n = 8, test ascending
    std::vector<int> a6 = {1,2,3,4,5,6,7,8};
    auto r6 = segmentTreeMax(a6);
    assert(r6[0] == 8 && r6[1] == 4 && r6[2] == 8);
    // all internal nodes should be the max of their segment, check a few
    assert(r6[3] == 2 && r6[4] == 4 && r6[5] == 6 && r6[6] == 8);
    // leaves
    for (int i = 0; i < 8; ++i) {
        assert(r6[7 + i] == a6[i]);
    }

    // Edge: p = 0 with negative numbers
    std::vector<int> a7 = {-5};
    auto r7 = segmentTreeMax(a7);
    assert(r7.size() == 1 && r7[0] == -5);

    // p = 3 with mixed negative/positive
    std::vector<int> a8 = {-3, 0, -1, 4, 5, -2, 7, -8};
    auto r8 = segmentTreeMax(a8);
    assert(r8[0] == 7); // overall max
    assert(r8[1] == 4); // max of first half {-3,0,-1,4}
    assert(r8[2] == 7); // max of second half {5,-2,7,-8}
    // leaves
    for (int i = 0; i < 8; ++i) {
        assert(r8[7 + i] == a8[i]);
    }

    return 0;
}

#include <vector>
#include <algorithm>

// Build a segment tree that stores the maximum of each segment.
// a: input array, size must be a power of two.
// Returns a vector of length 2*n - 1 representing the segment tree in heap layout (1-based index stored at 0-based position).
std::vector<int> segmentTreeMax(const std::vector<int>& a) {
    int n = static_cast<int>(a.size());
    std::vector<int> tree(2 * n - 1);
    
    // Helper lambda for recursion; captures tree and a by reference.
    // l, r: segment bounds (inclusive), i: heap index (1-based)
    std::function<void(int, int, int)> build = [&](int l, int r, int i) {
        int m = (l + r) / 2;
        if (l != r) {
            build(l, m, 2 * i);
            build(m + 1, r, 2 * i + 1);
        }
        int maxVal = a[l];
        for (int idx = l + 1; idx <= r; ++idx) {
            if (a[idx] > maxVal) {
                maxVal = a[idx];
            }
        }
        tree[i - 1] = maxVal;
    };
    
    build(0, n - 1, 1);
    return tree;
}

// The solution recursively partitions the array `[l, r]` into `[l, m]` and `[m+1, r]` where `m = (l + r) / 2`, and for each partition, it computes the maximum of that segment by a linear scan. The recursion continues until `l == r` (leaf). The output vector `b` has size `2n - 1` (for `n` leaves, a full binary tree has `n` internal nodes plus `n` leaves = `2n - 1` nodes). The root corresponds to heap index 1, stored at `b[0]`. The left child of heap index `i` is `2i`, right child `2i+1`. The recursion visits nodes in pre-order, but because we compute the maximum after recursing into both children, the result for each node is placed correctly. Important edge cases: when `n=1`, the recursion stops immediately and the root is the only node; when `p` is large, recursion depth is `p` (logarithmic), so it's safe. Time complexity: For each node we do a linear scan over its segment. The total work is the sum of segment lengths across all nodes. In a segment tree, the sum of segment lengths at each level is `n`, and there are `p+1` levels, so total work is `O(n log n)`. Space complexity: The recursion stack depth is `O(log n)`, and the output vector is `O(n)`. The function returns the vector by value, which is fine for typical sizes, but could be moved for efficiency.
