// Given an array of 0s and 1s (initially provided by the user), implement a C++ function that supports two types of queries: (1) flip the value at a given index (0 becomes 1, 1 becomes 0), and (2) find the position (0-based index) of the k-th 1 in the array, where k is 0-based (i.e., k=0 means the first 1). The function should take the initial array, a sequence of operations, and return the results of all find queries in order. The array size and number of operations are given as input; operations are encoded as pairs (type, argument) where type=1 means flip at index argument, and type=2 means find the k-th 1 (argument is k). The solution must be efficient for large inputs (up to 10^5 elements and 10^5 operations).
#include <cassert>
#include <vector>
#include <utility>

// The solution function is declared above; here we test it.
int main() {
    // Test 1: simple array, flip and find
    {
        std::vector<int> arr = {1, 0, 1, 0, 1};
        std::vector<std::pair<int, size_t>> ops = {{2, 0}}; // find first 1 -> index 0
        std::vector<size_t> res = processOperations(arr, ops);
        assert(res.size() == 1 && res[0] == 0);
    }
    // Test 2: multiple flips then find
    {
        std::vector<int> arr = {0, 0, 0, 0};
        std::vector<std::pair<int, size_t>> ops = {{1, 0}, {1, 2}, {2, 0}, {2, 1}};
        std::vector<size_t> res = processOperations(arr, ops);
        // After flips: [1,0,1,0]; first 1 at 0, second 1 at 2
        assert(res.size() == 2 && res[0] == 0 && res[1] == 2);
    }
    // Test 3: flipping back and forth
    {
        std::vector<int> arr = {1, 1, 1};
        std::vector<std::pair<int, size_t>> ops = {{1, 1}, {2, 1}, {1, 1}, {2, 1}};
        std::vector<size_t> res = processOperations(arr, ops);
        // Initial [1,1,1] -> flip index1 -> [1,0,1], k=1 -> index2; flip back -> [1,1,1], k=1 -> index1
        assert(res.size() == 2 && res[0] == 2 && res[1] == 1);
    }
    // Test 4: larger array, all zeros then set one
    {
        std::vector<int> arr(10, 0);
        std::vector<std::pair<int, size_t>> ops = {{1, 5}, {2, 0}};
        std::vector<size_t> res = processOperations(arr, ops);
        assert(res.size() == 1 && res[0] == 5);
    }
    // Test 5: edge: find when only one 1 exists
    {
        std::vector<int> arr = {0, 1, 0};
        std::vector<std::pair<int, size_t>> ops = {{2, 0}};
        std::vector<size_t> res = processOperations(arr, ops);
        assert(res.size() == 1 && res[0] == 1);
    }
    // Test 6: empty array not allowed by constraints but if n=0, just no queries
    {
        std::vector<int> arr = {};
        std::vector<std::pair<int, size_t>> ops = {};
        std::vector<size_t> res = processOperations(arr, ops);
        assert(res.empty());
    }
    return 0;
}
#include <vector>
#include <cstddef>

// Segment tree for a binary array supporting point flips and k-th 1 queries.
class BinarySegmentTree {
private:
    std::vector<int> tree;  // store counts of 1s
    size_t n;               // size of the array

    void build(const std::vector<int>& arr, size_t node, size_t l, size_t r) {
        if (l + 1 == r) {
            tree[node] = arr[l];
            return;
        }
        size_t mid = (l + r) / 2;
        build(arr, node * 2 + 1, l, mid);
        build(arr, node * 2 + 2, mid, r);
        tree[node] = tree[node * 2 + 1] + tree[node * 2 + 2];
    }

    void update(size_t node, size_t l, size_t r, size_t pos, int new_val) {
        if (l + 1 == r) {
            tree[node] = new_val;
            return;
        }
        size_t mid = (l + r) / 2;
        if (pos < mid) {
            update(node * 2 + 1, l, mid, pos, new_val);
        } else {
            update(node * 2 + 2, mid, r, pos, new_val);
        }
        tree[node] = tree[node * 2 + 1] + tree[node * 2 + 2];
    }

    size_t find_kth(size_t node, size_t l, size_t r, size_t k) const {
        if (l + 1 == r) return l;
        size_t mid = (l + r) / 2;
        size_t left_count = tree[node * 2 + 1];
        if (k < left_count) {
            return find_kth(node * 2 + 1, l, mid, k);
        } else {
            return find_kth(node * 2 + 2, mid, r, k - left_count);
        }
    }

public:
    BinarySegmentTree(const std::vector<int>& arr) : n(arr.size()) {
        tree.resize(4 * n);
        if (n > 0) build(arr, 0, 0, n);
    }

    // Flip the value at index pos (0↔1)
    void flip(size_t pos) {
        // get current value at leaf: we can query tree, but simpler: just read current leaf value
        size_t node = 0, l = 0, r = n;
        int current;
        // find leaf
        while (l + 1 < r) {
            size_t mid = (l + r) / 2;
            if (pos < mid) { node = node * 2 + 1; r = mid; }
            else { node = node * 2 + 2; l = mid; }
        }
        current = tree[node];
        update(0, 0, n, pos, 1 - current);
    }

    // Return the index of the k-th 1 (0-based k)
    size_t findKthOne(size_t k) const {
        return find_kth(0, 0, n, k);
    }
};

// Solution function: processes operations and returns results of find queries.
std::vector<size_t> processOperations(const std::vector<int>& initialArray, 
                                      const std::vector<std::pair<int, size_t>>& operations) {
    BinarySegmentTree seg(initialArray);
    std::vector<size_t> results;
    for (const auto& op : operations) {
        if (op.first == 1) {
            seg.flip(op.second);
        } else { // op.first == 2
            results.push_back(seg.findKthOne(op.second));
        }
    }
    return results;
}
// The core problem is maintaining a dynamic binary array with point updates and order-statistic queries (finding the k-th set bit). A segment tree is ideal: each leaf stores the value (0 or 1), and each internal node stores the sum of its children's counts (total number of 1s in that segment). For a flip operation, we update the leaf (toggling 0↔1) and recompute sums up the tree—this takes O(log n) time. For a find operation, we traverse the tree from the root: at each internal node, compare the query index k to the left child's sum. If k < left_sum, the desired position is in the left subtree; otherwise, subtract left_sum from k and go right. This descends to a leaf in O(log n) time. Edge cases: if k is out of range (≥ total number of 1s), the behavior is undefined—we can assume valid input as per problem constraints. The solution uses O(n) space for the segment tree (typically 4*n). Time complexity per query is O(log n), so total O((n+m) log n) for m operations.
