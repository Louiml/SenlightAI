Write a C++ function `int rangeMinAfterReversals(std::vector<int>& array, const std::vector<std::tuple<char, size_t, size_t>>& operations)` that processes a sequence of operations on an initial array. Operations are of two types: `'R'` (reverse the subarray from 1-indexed position `l` to `r`, inclusive) and `'Q'` (query the minimum value in the subarray from 1-indexed position `l` to `r`, inclusive). After each query, immediately output (e.g., by `std::cout`) the minimum value for that query. The function should return the sum of all query answers (the sum of minima returned). The array and all operations are valid (`1 <= l <= r <= array.size()`). The function must not modify the original array content persistently—it should apply the reversals only to an internal copy. Handle edge cases like empty operations or arrays of size 1. The implementation must be efficient for up to 100,000 elements and 100,000 operations, using a treap with lazy reversal. The provided code snippet already implements a treap with `Reverse` and `Min` methods; your solution should rewrite these as a clean standalone function with proper memory management and without using global variables.
#include <cassert>
#include <vector>
#include <tuple>

int rangeMinAfterReversals(std::vector<int>& array, const std::vector<std::tuple<char, size_t, size_t>>& operations);

int main() {
    // Test 1: Simple reversal then query
    std::vector<int> arr1 = {1, 2, 3, 4, 5};
    std::vector<std::tuple<char, size_t, size_t>> ops1 = {
        {'R', 1, 5},   // reverse whole array -> {5,4,3,2,1}
        {'Q', 1, 3}    // min of first 3 elements -> 3
    };
    assert(rangeMinAfterReversals(arr1, ops1) == 3);

    // Test 2: Queries without reversals
    std::vector<int> arr2 = {10, -2, 7, 4};
    std::vector<std::tuple<char, size_t, size_t>> ops2 = {
        {'Q', 1, 4},   // min = -2
        {'Q', 2, 3}    // min = -2
    };
    assert(rangeMinAfterReversals(arr2, ops2) == -4);

    // Test 3: Multiple reversals and queries
    std::vector<int> arr3 = {1, 5, 3, 9, 2};
    std::vector<std::tuple<char, size_t, size_t>> ops3 = {
        {'R', 2, 4},   // {1,9,3,5,2}
        {'Q', 2, 4},   // min of {9,3,5} = 3
        {'R', 3, 5},   // {1,9,2,5,3}
        {'Q', 1, 5}    // min = 1
    };
    assert(rangeMinAfterReversals(arr3, ops3) == 4);

    // Test 4: Single element array
    std::vector<int> arr4 = {42};
    std::vector<std::tuple<char, size_t, size_t>> ops4 = {
        {'R', 1, 1},
        {'Q', 1, 1}
    };
    assert(rangeMinAfterReversals(arr4, ops4) == 42);

    // Test 5: Empty operations
    std::vector<int> arr5 = {3, 1, 2};
    std::vector<std::tuple<char, size_t, size_t>> ops5 = {};
    assert(rangeMinAfterReversals(arr5, ops5) == 0);

    // Test 6: Reversals of subsegments and overlapping queries
    std::vector<int> arr6 = {4, 8, 1, 6, 3, 9};
    std::vector<std::tuple<char, size_t, size_t>> ops6 = {
        {'R', 2, 5},   // {4,3,6,1,8,9}
        {'Q', 1, 3},   // min of {4,3,6} = 3
        {'R', 1, 6},   // {9,8,1,6,3,4}
        {'Q', 3, 5}    // min of {1,6,3} = 1
    };
    assert(rangeMinAfterReversals(arr6, ops6) == 4);

    // Test 7: Large number of operations (optional stress with small array)
    std::vector<int> arr7 = {7, 5, 3, 1};
    std::vector<std::tuple<char, size_t, size_t>> ops7;
    for (int i = 0; i < 100; ++i) {
        ops7.push_back({'R', 1, 4});
        ops7.push_back({'Q', 1, 1});
    }
    // After each full reversal, first element alternates between 7 and 1, so 50*7 + 50*1 = 400
    assert(rangeMinAfterReversals(arr7, ops7) == 400);

    return 0;
}
#include <bits/stdc++.h>

const int kInf = 2 * 1e9;

template <typename T>
struct Node {
    T value;
    int64_t prior;
    int min;
    size_t size;
    Node* left;
    Node* right;
    bool reversed;

    Node(int64_t p, const T& v) : value(v), prior(p), min(v), size(1), left(nullptr), right(nullptr), reversed(false) {}

    size_t LSize() const { return left ? left->size : 0; }
    size_t RSize() const { return right ? right->size : 0; }
    int LMin() const { return left ? left->min : kInf; }
    int RMin() const { return right ? right->min : kInf; }

    void Upd() {
        size = 1 + LSize() + RSize();
        min = std::min({value, LMin(), RMin()});
    }
};

template <typename T>
void Clear(Node<T>* node) {
    if (!node) return;
    Clear(node->left);
    Clear(node->right);
    delete node;
}

// Process reversals and range-min queries; return sum of all query answers.
int rangeMinAfterReversals(std::vector<int>& array, const std::vector<std::tuple<char, size_t, size_t>>& operations) {
    struct Treap {
        Node<int>* root = nullptr;

        ~Treap() { Clear(root); }

        void Push(Node<int>* node) {
            if (!node || !node->reversed) return;
            std::swap(node->left, node->right);
            if (node->left) node->left->reversed ^= 1;
            if (node->right) node->right->reversed ^= 1;
            node->reversed = false;
        }

        void Upd(Node<int>* node) {
            if (node) node->Upd();
        }

        std::pair<Node<int>*, Node<int>*> Split(Node<int>* node, int cnt) {
            Push(node);
            if (!node) return {nullptr, nullptr};
            if (cnt <= static_cast<int>(node->LSize())) {
                auto [leftPart, rightPart] = Split(node->left, cnt);
                node->left = rightPart;
                Upd(node);
                return {leftPart, node};
            }
            auto [leftPart, rightPart] = Split(node->right, cnt - static_cast<int>(node->LSize()) - 1);
            node->right = leftPart;
            Upd(node);
            return {node, rightPart};
        }

        Node<int>* Merge(Node<int>* a, Node<int>* b) {
            Push(a);
            Push(b);
            if (!a) return b;
            if (!b) return a;
            if (a->prior > b->prior) {
                a->right = Merge(a->right, b);
                Upd(a);
                return a;
            }
            b->left = Merge(a, b->left);
            Upd(b);
            return b;
        }

        void Build(const std::vector<int>& arr) {
            for (int v : arr) {
                auto node = new Node<int>(rand(), v);
                root = Merge(root, node);
            }
        }

        void Reverse(size_t l, size_t r) {
            auto [a, bc] = Split(root, static_cast<int>(l));
            auto [b, c] = Split(bc, static_cast<int>(r - l));
            if (b) b->reversed ^= 1;
            root = Merge(Merge(a, b), c);
        }

        int QueryMin(size_t l, size_t r) {
            auto [a, bc] = Split(root, static_cast<int>(l));
            auto [b, c] = Split(bc, static_cast<int>(r - l));
            int ans = (b ? b->min : kInf);
            root = Merge(Merge(a, b), c);
            return ans;
        }
    };

    srand(12345); // deterministic for reproducibility (optional)
    Treap treap;
    treap.Build(array);

    int total = 0;
    for (const auto& [type, l, r] : operations) {
        // Convert to 0-indexed for internal treap; operation l,r are 1-indexed inclusive
        size_t left = l - 1;
        size_t right = r; // exclusive for split
        if (type == 'R') {
            treap.Reverse(left, right);
        } else if (type == 'Q') {
            total += treap.QueryMin(left, right);
        }
    }
    return total;
}
// The core challenge is supporting range reversal and range minimum queries on a mutable sequence efficiently. A treap (randomized binary search tree) with subtree sizes and minima can handle both operations in `O(log n)` expected time. The treap stores each element as a node with a random priority, a value, subtree size, and subtree minimum. Lazy reversal is implemented by storing a `reversed` flag; when reversing a segment, we split the treap into three parts (left, middle, right) using `Split` by size, toggle the flag on the middle root, then merge back. For a query, we split out the segment, read the root's minimum, then merge back. `Split` recursively divides the tree by size: if the split count is less than or equal to the left subtree size, split the left subtree; otherwise, split the right subtree with an adjusted count. `Merge` combines two treaps while maintaining heap order by priority. Before any recursive operation, we must push down lazy reversals to ensure child sizes and minima are correct. Edge cases: null nodes must be handled (`min` = infinity, `size` = 0), and reversals of a single-element or empty segment are no-ops. The initial construction can insert elements one by one from the end to preserve order, or use a linear-time build. Time complexity for `n` elements and `m` operations is `O((n+m) log n)` expected, with `O(n)` space for nodes. The function returns the sum of all query minima; if there are no queries, it returns 0.
