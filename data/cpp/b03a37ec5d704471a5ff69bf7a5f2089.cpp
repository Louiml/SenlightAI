/*
Write a C++ function `rangeMaxSubarray` that takes an integer array (via a `std::vector<int>`), a count of operations `m` (each operation is either a point update or a range maximum-subarray-sum query), and a list of operations encoded as a vector of triples `(opt, x, y)`. The function must process these operations in order and return a vector of integers containing the results of all type-1 queries (where `opt == 1`), where each query asks for the maximum subarray sum (contiguous, non-empty) within the subarray `a[x..y]` (1-indexed). For type-0 operations (`opt == 0`), the function must update the element at position `x` to the value `y`. The function should support at most 50,000 elements and handle both positive and negative values, including all-negative arrays (where the maximum subarray is the largest single element).
*/
#include <vector>
#include <algorithm>
#include <stdexcept>

// Segment tree node: sum, best prefix, best suffix, best subarray
struct Node {
    int sum, pre, suf, best;
};

// Merge two child nodes into a parent
Node mergeNode(const Node& left, const Node& right) {
    Node res;
    res.sum = left.sum + right.sum;
    res.pre = std::max(left.pre, left.sum + right.pre);
    res.suf = std::max(right.suf, right.sum + left.suf);
    res.best = std::max({left.best, right.best, left.suf + right.pre});
    return res;
}

class SegmentTree {
private:
    std::vector<Node> tree;
    int n;

    void build(const std::vector<int>& arr, int node, int l, int r) {
        if (l == r) {
            tree[node] = {arr[l], arr[l], arr[l], arr[l]};
            return;
        }
        int mid = (l + r) >> 1;
        build(arr, node << 1, l, mid);
        build(arr, node << 1 | 1, mid + 1, r);
        tree[node] = mergeNode(tree[node << 1], tree[node << 1 | 1]);
    }

    void update(int pos, int val, int node, int l, int r) {
        if (l == r) {
            tree[node] = {val, val, val, val};
            return;
        }
        int mid = (l + r) >> 1;
        if (pos <= mid) update(pos, val, node << 1, l, mid);
        else update(pos, val, node << 1 | 1, mid + 1, r);
        tree[node] = mergeNode(tree[node << 1], tree[node << 1 | 1]);
    }

    Node query(int ql, int qr, int node, int l, int r) {
        if (ql <= l && r <= qr) return tree[node];
        int mid = (l + r) >> 1;
        if (qr <= mid) return query(ql, qr, node << 1, l, mid);
        if (ql > mid) return query(ql, qr, node << 1 | 1, mid + 1, r);
        Node left = query(ql, qr, node << 1, l, mid);
        Node right = query(ql, qr, node << 1 | 1, mid + 1, r);
        return mergeNode(left, right);
    }

public:
    SegmentTree(const std::vector<int>& arr) {
        n = static_cast<int>(arr.size());
        if (n == 0) throw std::invalid_argument("Array must be non-empty");
        tree.resize(4 * n);
        build(arr, 1, 0, n - 1);
    }

    void pointUpdate(int pos, int val) {
        if (pos < 0 || pos >= n) throw std::out_of_range("Position out of bounds");
        update(pos, val, 1, 0, n - 1);
    }

    int rangeMaxSubarray(int l, int r) {
        if (l < 0 || r >= n || l > r) throw std::out_of_range("Range invalid");
        return query(l, r, 1, 0, n - 1).best;
    }
};

// Main function: process operations and return results of type-1 queries
std::vector<int> rangeMaxSubarray(const std::vector<int>& initialArray,
                                  const std::vector<std::vector<int>>& operations) {
    SegmentTree seg(initialArray);
    std::vector<int> answers;
    for (const auto& op : operations) {
        if (op.size() != 3) throw std::invalid_argument("Each operation must have 3 elements");
        int opt = op[0], x = op[1], y = op[2];
        if (opt == 0) {
            seg.pointUpdate(x - 1, y); // convert to 0-indexed
        } else if (opt == 1) {
            answers.push_back(seg.rangeMaxSubarray(x - 1, y - 1));
        } else {
            throw std::invalid_argument("Unknown operation type");
        }
    }
    return answers;
}
#include <cassert>
#include <vector>

// Assuming the solution function is declared as above

int main() {
    // Test 1: Basic query on simple array
    {
        std::vector<int> arr = {1, 2, 3, 4};
        std::vector<std::vector<int>> ops = {{1, 1, 4}};
        auto res = rangeMaxSubarray(arr, ops);
        assert(res.size() == 1);
        assert(res[0] == 10);
    }

    // Test 2: Query with negative numbers
    {
        std::vector<int> arr = {-2, 1, -3, 4, -1, 2, 1, -5, 4};
        std::vector<std::vector<int>> ops = {{1, 1, 9}};
        auto res = rangeMaxSubarray(arr, ops);
        assert(res.size() == 1);
        assert(res[0] == 6); // subarray 4,-1,2,1
    }

    // Test 3: All negative array
    {
        std::vector<int> arr = {-5, -2, -9};
        std::vector<std::vector<int>> ops = {{1, 1, 3}};
        auto res = rangeMaxSubarray(arr, ops);
        assert(res.size() == 1);
        assert(res[0] == -2); // largest single element
    }

    // Test 4: Point update and then query
    {
        std::vector<int> arr = {1, 2, 3, 4};
        std::vector<std::vector<int>> ops = {{0, 2, 10}, {1, 1, 4}};
        auto res = rangeMaxSubarray(arr, ops);
        assert(res.size() == 1);
        assert(res[0] == 18); // 1+10+3+4
    }

    // Test 5: Query on subrange not covering whole array
    {
        std::vector<int> arr = {-1, 5, -2, 3, -4, 8};
        std::vector<std::vector<int>> ops = {{1, 2, 5}}; // indices 2..5 → 5,-2,3,-4
        auto res = rangeMaxSubarray(arr, ops);
        assert(res.size() == 1);
        assert(res[0] == 6); // 5+-2+3
    }

    // Test 6: Mixed updates and queries
    {
        std::vector<int> arr = {10, -5, 6, -2, 7};
        std::vector<std::vector<int>> ops = {
            {1, 1, 5}, // 10-5+6-2+7=16
            {0, 3, -100}, // change index3 (value6) to -100
            {1, 2, 5}, // -5,-100,-2,7 → best = 7
            {1, 1, 2} // 10,-5 → best = 10
        };
        auto res = rangeMaxSubarray(arr, ops);
        assert(res.size() == 3);
        assert(res[0] == 16);
        assert(res[1] == 7);
        assert(res[2] == 10);
    }

    // Test 7: Single element array
    {
        std::vector<int> arr = {42};
        std::vector<std::vector<int>> ops = {{1, 1, 1}, {0, 1, -7}, {1, 1, 1}};
        auto res = rangeMaxSubarray(arr, ops);
        assert(res.size() == 2);
        assert(res[0] == 42);
        assert(res[1] == -7);
    }

    // Test 8: Large array with alternating signs (spot check)
    {
        std::vector<int> arr(50000, 1);
        for (int i = 0; i < 50000; ++i) arr[i] = (i % 2 == 0 ? 1 : -1);
        std::vector<std::vector<int>> ops = {{1, 1, 50000}};
        auto res = rangeMaxSubarray(arr, ops);
        assert(res.size() == 1);
        assert(res[0] == 1); // any single positive element is best
    }

    return 0;
}
// The core algorithm is a segment tree where each node stores for its segment: total sum (`sum`), maximum prefix sum (`prel`), maximum suffix sum (`prer`), and maximum subarray sum (`res`). This structure allows merging two child segments in O(1): the merged sum is the sum of both, the merged prefix is the maximum of the left prefix or the left sum plus right prefix, the merged suffix is the maximum of the right suffix or the right sum plus left suffix, and the merged best is the maximum of left best, right best, and left suffix plus right prefix. For building, updating, and querying, we recursively traverse the tree. The query function must handle three cases: if the query range entirely lies in the left child, entirely in the right child, or spans both; in the spanning case, it merges the results from both children. Edge cases include a single-element segment (all four fields are that element), and all-negative arrays (the prefix/suffix/best should correctly pick the largest single element due to the max operations). Time complexity is O(n) for building, O(log n) per update, and O(log n) per query; space is O(n). The function should return a vector of query answers.
