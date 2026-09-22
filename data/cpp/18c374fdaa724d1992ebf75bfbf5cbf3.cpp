// Write a C++ function `binary_string_operations` that takes a binary string (only '0' and '1' characters) and an integer `num_queries`. The function must simulate the following operations on the binary string: each query is given as a pair `(type, index)` where `type == 1` means to flip the bit at the given index (change '0' to '1' or '1' to '0'), and `type == 2` means to find the position (0-based index) of the `index`-th '1' in the current string, where `index` is 0-based and guaranteed to be valid (i.e., there are at least `index+1` ones). The function should return a vector of integers containing the answers to all type-2 queries in the order they appear.
#include <cassert>
#include <string>
#include <vector>

// Assume the solution function is already included above.

int main() {
    // Example 1: basic flips and queries
    std::string s = "10100";
    std::vector<int> types = {2, 1, 2, 1, 2};
    std::vector<int> idx = {0, 2, 1, 0, 0};
    std::vector<int> res = binary_string_operations(s, 5, types, idx);
    std::vector<int> expected = {0, 1, 2};
    assert(res == expected);
    
    // Example 2: all ones, queries without flips
    s = "1111";
    types = {2, 2, 2, 2};
    idx = {0, 1, 2, 3};
    res = binary_string_operations(s, 4, types, idx);
    expected = {0, 1, 2, 3};
    assert(res == expected);
    
    // Example 3: all zeros, then flip and query
    s = "0000";
    types = {1, 1, 2, 2};
    idx = {1, 3, 0, 1};
    res = binary_string_operations(s, 4, types, idx);
    expected = {1, 3};
    assert(res == expected);
    
    // Example 4: flips that toggle the same bit multiple times
    s = "10";
    types = {1, 1, 2, 2};
    idx = {0, 0, 0, 0};
    res = binary_string_operations(s, 4, types, idx);
    // After flip 0: "00", then flip 0: "10" (original), so first query returns 0, second query also returns 0
    expected = {0, 0};
    assert(res == expected);
    
    // Example 5: larger string, mixed operations
    s = "0101010101";
    types = {2, 1, 2, 2, 1, 2};
    idx = {2, 5, 2, 0, 5, 1};
    res = binary_string_operations(s, 6, types, idx);
    // Initially ones at positions 1,3,5,7,9. k=2 -> pos 5. Flip pos5 -> ones at 1,3,7,9. k=2 -> pos7. k=0 -> pos1. Flip pos5 (now 0) -> ones at 1,3,7,9 stay. k=1 -> pos3.
    expected = {5, 7, 1, 3};
    assert(res == expected);
    
    return 0;
}
#include <string>
#include <vector>

class BinarySegmentTree {
private:
    int n;
    std::vector<int> tree;

    void build(const std::string& s, int node, int l, int r) {
        if (l == r) {
            tree[node] = (s[l] == '1') ? 1 : 0;
            return;
        }
        int mid = (l + r) / 2;
        build(s, 2 * node + 1, l, mid);
        build(s, 2 * node + 2, mid + 1, r);
        tree[node] = tree[2 * node + 1] + tree[2 * node + 2];
    }

    void update(int node, int l, int r, int pos) {
        if (l == r) {
            tree[node] ^= 1; // flip 0↔1
            return;
        }
        int mid = (l + r) / 2;
        if (pos <= mid) {
            update(2 * node + 1, l, mid, pos);
        } else {
            update(2 * node + 2, mid + 1, r, pos);
        }
        tree[node] = tree[2 * node + 1] + tree[2 * node + 2];
    }

    int query(int node, int l, int r, int k) {
        if (l == r) {
            return l;
        }
        int mid = (l + r) / 2;
        int left_count = tree[2 * node + 1];
        if (k < left_count) {
            return query(2 * node + 1, l, mid, k);
        } else {
            return query(2 * node + 2, mid + 1, r, k - left_count);
        }
    }

public:
    explicit BinarySegmentTree(const std::string& s) : n(s.size()), tree(4 * s.size()) {
        build(s, 0, 0, n - 1);
    }

    void flip(int pos) {
        update(0, 0, n - 1, pos);
    }

    int find_kth_one(int k) {
        return query(0, 0, n - 1, k);
    }
};

// Main function: processes queries on a binary string.
// For each type-1 query, flips bit at 'index'.
// For each type-2 query, returns the position of the 'index'-th one (0-based).
std::vector<int> binary_string_operations(const std::string& initial, int num_queries,
                                          const std::vector<int>& types, const std::vector<int>& indices) {
    BinarySegmentTree seg(initial);
    std::vector<int> result;
    for (int i = 0; i < num_queries; ++i) {
        if (types[i] == 1) {
            seg.flip(indices[i]);
        } else {
            result.push_back(seg.find_kth_one(indices[i]));
        }
    }
    return result;
}
// The core challenge is supporting both point updates (flips) and order-statistic queries (find the k-th one). A segment tree that stores the count of ones in each segment is ideal. Build the tree from the initial binary string, where each leaf stores either 0 or 1. For a flip, update the leaf and propagate the sum upward. For a k-th one query (0-based), traverse the tree: at each node, look at the left child's count; if k is less than that count, go left; otherwise, subtract the left count from k and go right. When reaching a leaf, return its index. This works because the tree maintains the prefix sums. Edge cases: the query index for type 2 is always valid, so no out-of-range checks needed. The initial string must be at least length 1. Time complexity is O(log n) per update and O(log n) per query, where n is the length of the string; building the tree takes O(n). Memory is O(n) for the tree array. Note: the problem is inspired by the provided snippet which uses a segment tree for sum and k-th one queries, but here we adapt it to a string rather than a vector of integers.
