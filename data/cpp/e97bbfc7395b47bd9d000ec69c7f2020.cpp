Given an array of distinct integers, write a C++ function `longestSpecialSubsequence` that takes a `std::vector<int64_t>` (which may contain duplicates) and returns a `std::vector<int64_t>` containing the indices (0-based) of the longest strictly increasing subsequence in the order they appear (i.e., an increasing sequence of values such that each next value is strictly greater than the previous, and indices are increasing). If multiple longest subsequences exist, return the one with the lexicographically smallest sequence of indices (compare index sequences element‑wise). The function must not modify the input, must handle empty input (return empty vector), and must run in O(n log n) time. The indices in the output must be in increasing order of position. Example: input `{3, 1, 2, 4}` → output `{1, 2, 3}` (values 1,2,4) or `{0,2,3}` (3,4? but 3,4 is length 2 only; longest length is 3 and unique? Actually multiple: {1,2,4} at indices {1,2,3}; {1,2,4} only one; but also {3,?} no. So output `{1,2,3}`). For duplicates like `{2,2,2}` → longest strictly increasing length 1, so return the smallest index among all occurrences, i.e., `{0}`.

#include <cassert>
#include <vector>
#include <cstdint>

// Declaration from solution.
std::vector<int64_t> longestSpecialSubsequence(const std::vector<int64_t>& numbers);

int main() {
    // Basic increasing sequence
    assert(longestSpecialSubsequence({3, 1, 2, 4}) == std::vector<int64_t>({1, 2, 3}));
    // Decreasing sequence: any single element is longest
    assert(longestSpecialSubsequence({5, 4, 3, 2, 1}) == std::vector<int64_t>({0}));
    // Duplicates: strict increase, so length 1
    assert(longestSpecialSubsequence({2, 2, 2}) == std::vector<int64_t>({0}));
    // Mixed with duplicates, lexicographically smallest index sequence
    std::vector<int64_t> input = {1, 4, 2, 3, 4};
    // Options: length 3 sequences: {1,2,3} indices {0,2,3} or {0,2,4}? But 4 duplicate, cannot use both. Actually {1,2,3} values: indices {0,2,3} length3; {1,2,4} cannot because 4 at index1 and 4 at index4 equal, cannot use both; {1,3,4}? 1,3,4 indices {0,3,4} also length3. Lexicographically {0,2,3} vs {0,3,4}? Compare first 0 equal, second 2<3, so {0,2,3} wins.
    assert(longestSpecialSubsequence(input) == std::vector<int64_t>({0, 2, 3}));
    // Empty input
    assert(longestSpecialSubsequence({}) == std::vector<int64_t>({}));
    // Single element
    assert(longestSpecialSubsequence({42}) == std::vector<int64_t>({0}));
    // All negative with a tie: {-5, -5, -4} -> only -5,-4 length2 at indices {0,2} or {1,2} -> lexicographically {0,2}
    assert(longestSpecialSubsequence({-5, -5, -4}) == std::vector<int64_t>({0, 2}));
    // Sequence where the longest is not the first start: {1, 3, 2, 4} -> {0,1,3} values 1,3,4 length3, also {0,2,3} length3; lexicographically {0,1,3} (since second 1<2)
    assert(longestSpecialSubsequence({1, 3, 2, 4}) == std::vector<int64_t>({0, 1, 3}));
    // Long increasing sequence, typical
    assert(longestSpecialSubsequence({10, 20, 30, 5, 6, 7}) == std::vector<int64_t>({3, 4, 5}));
    // Duplicate at end forces length 3 with smallest indices
    assert(longestSpecialSubsequence({1, 2, 3, 2}) == std::vector<int64_t>({0, 1, 2}));
    // Large test to verify O(n log n) doesn't break, but here small
    assert(longestSpecialSubsequence({5, 1, 4, 2, 3}) == std::vector<int64_t>({1, 3, 4}));
    return 0;
}

#include <vector>
#include <algorithm>
#include <cstdint>

// Segment tree that stores pairs (dp_length, index) and supports:
// - Build: initialize all leaves to (0, -1)
// - Update: set a leaf to (len, idx)
// - Query: get the pair with maximum length (if tie, smallest index) over a prefix [0, r]
struct SegTree {
    struct Node {
        int64_t len;
        int64_t idx;
        bool operator>(const Node& other) const {
            if (len != other.len) return len > other.len;
            return idx < other.idx;
        }
    };

    std::vector<Node> tree;

    SegTree(int n) : tree(4 * n, {0, -1}) {}

    void update(int node, int l, int r, int pos, Node val) {
        if (l == r) {
            tree[node] = val;
            return;
        }
        int mid = l + (r - l) / 2;
        if (pos <= mid) update(2 * node + 1, l, mid, pos, val);
        else update(2 * node + 2, mid + 1, r, pos, val);
        if (tree[2 * node + 1] > tree[2 * node + 2])
            tree[node] = tree[2 * node + 1];
        else
            tree[node] = tree[2 * node + 2];
    }

    Node query(int node, int l, int r, int ql, int qr) {
        if (ql > r || qr < l) return {0, -1}; // neutral element
        if (ql <= l && r <= qr) return tree[node];
        int mid = l + (r - l) / 2;
        Node left = query(2 * node + 1, l, mid, ql, qr);
        Node right = query(2 * node + 2, mid + 1, r, ql, qr);
        return (left > right) ? left : right;
    }
};

std::vector<int64_t> longestSpecialSubsequence(const std::vector<int64_t>& numbers) {
    int n = static_cast<int>(numbers.size());
    if (n == 0) return {};

    // Create pairs (value, original index), sort by value ascending,
    // and for equal values, by index descending (to avoid extending equal values).
    std::vector<std::pair<int64_t, int64_t>> pairs(n);
    for (int i = 0; i < n; ++i) pairs[i] = {numbers[i], i};
    std::sort(pairs.begin(), pairs.end(), [](const auto& a, const auto& b) {
        if (a.first != b.first) return a.first < b.first;
        return a.second > b.second;
    });

    // dp[i] = length of the best strictly increasing subsequence ending at index i.
    // prev[i] = predecessor index in that subsequence, or -1 if none.
    std::vector<int64_t> dp(n, 1);
    std::vector<int64_t> prev(n, -1);

    SegTree seg(n);
    // Build initial tree with all leaves (0, -1). The constructor already does this.

    for (auto& p : pairs) {
        int64_t val = p.first;
        int64_t pos = p.second;
        // Query for the best subsequence among indices < pos.
        SegTree::Node best = (pos > 0) ? seg.query(0, 0, n - 1, 0, static_cast<int>(pos - 1)) : SegTree::Node{0, -1};
        dp[pos] = best.len + 1;
        prev[pos] = best.idx;
        // Update the leaf at pos with the new dp and its index.
        seg.update(0, 0, n - 1, static_cast<int>(pos), {dp[pos], pos});
    }

    // Find the position with maximum dp, breaking ties by smallest index.
    int64_t best_len = 0;
    int64_t best_end = -1;
    for (int i = 0; i < n; ++i) {
        if (dp[i] > best_len || (dp[i] == best_len && i < best_end)) {
            best_len = dp[i];
            best_end = i;
        }
    }

    // Reconstruct the sequence of indices.
    std::vector<int64_t> result;
    for (int64_t cur = best_end; cur != -1; cur = prev[cur]) {
        result.push_back(cur);
    }
    std::reverse(result.begin(), result.end());
    return result;
}

// The problem is the classic Longest Increasing Subsequence (LIS) but with two twists: (1) the values are distinct? Actually not distinct—the problem says array may contain duplicates, but the subsequence must be strictly increasing, so equal values cannot be used. (2) We need to output the indices of one LIS, preferring the lexicographically smallest index sequence among all LIS of maximum length. The given code snippet uses a segment tree to compute LIS in O(n log n) by processing elements in decreasing value order while tracking DP values. However, for a standalone task, a simpler and more robust approach is to use a standard O(n log n) LIS with binary search on the "tails" array, and simultaneously track predecessor indices to reconstruct the subsequence. But the standard method returns the lexicographically smallest sequence of values, not necessarily indices. To get lexicographically smallest indices, we need to ensure that when multiple candidates give the same length, we pick the one with smaller indices. A clean way: process elements from left to right, but for the same tail length, we want the smallest index? Actually to obtain the lexicographically smallest index sequence, we can compute LIS from right to left (longest decreasing?) Alternatively, we can use a segment tree exactly as in the snippet: sort pairs (value, index) by decreasing value, then for each element in that order, query the segment tree for the maximum DP among positions left of its index, set dp = that + 1, and record predecessor. This gives a valid LIS, but does it guarantee lexicographically smallest indices? Not automatically. To guarantee lexicographically smallest indices, we can process elements in increasing value order (not decreasing) but then we need to query the segment tree for positions to the left that have already been processed (since increasing value ensures strictly greater values are processed later? Actually for strictly increasing values, we want to extend from a previous element with smaller value and smaller index. So if we process in increasing value order, when we encounter a new value, all elements with strictly smaller values have been processed, so we can query the segment tree for the best DP among positions with index less than current. This yields the correct DP because we only ever extend from smaller values. To break ties for lexicographically smallest indices, when the DP values are equal, we prefer the predecessor with the smallest index? Actually, the DP value is the length of the best subsequence ending at this position. If multiple predecessors give the same length, we want the one that eventually leads to the lexicographically smallest index sequence. A standard trick: process elements in increasing value order, and among equal values, process them in increasing index order? But we need strictly increasing, so equal values cannot extend each other. Process increasing value, and for equal values, we should process them in increasing index order? Actually if we process equal values in increasing index order, earlier equal values might incorrectly extend later equal values? No, because the segment tree query only looks at positions strictly less than the current index, and we update the DP after query. However, if we process equal values in increasing index order, the later equal value will not see the earlier equal value's update because the earlier equal value has a larger index? Wait, if we process equal values in increasing index order, the first equal value (smaller index) updates, then the second equal value (larger index) queries the segment tree that includes the first equal value's position? But the first equal value has a smaller index, so the second (larger index) can query positions <= index-1, which includes the first equal value's index, but the value is equal, so it would incorrectly extend to equal value, violating strictly increasing. To avoid this, we must process equal values in **decreasing index order** so that a later index (in value order) with same value does not see an earlier index with same value. Since we process equal values in decreasing index order, when we process the larger index first, it queries the segment tree that hasn't been updated by the smaller index yet (since smaller index has not been processed). Then after processing all equal values, the segment tree contains updates from all of them, but no equal value extends another. This is a standard technique: sort by value ascending, and for equal values, sort by index descending. Then for each element, query the segment tree for the best DP among indices < current index, update dp = best + 1, and record predecessor as the best index. To achieve lexicographically smallest index sequence, we need to break ties: when the best DP value is the same from multiple predecessors, we should choose the one with the smallest index. Because the index sequence is output in increasing order, a smaller predecessor at an earlier position leads to a smaller sequence lexicographically. So in the segment tree, we store pairs (dp_value, index) and compare first by dp_value descending, then by index ascending. This ensures that when querying for the best predecessor, we get the smallest index among those with maximal dp. Also, when updating the tree at position i with value (dp[i], i), for future queries, a larger dp is better; among equal dp, smaller index is better because it leads to lexicographically smaller sequences. This works because the sequence output is in index order; choosing a smaller predecessor yields a smaller first differing index. After computing dp and predecessors, we find the position with maximum dp, and among those, the smallest index (since lexicographically smallest sequence starts with the smallest index). Then reconstruct the sequence by following predecessors backward, collecting indices, and reversing. Edge cases: empty input returns empty; single element returns {0}. The algorithm is O(n log n) time and O(n) space.
