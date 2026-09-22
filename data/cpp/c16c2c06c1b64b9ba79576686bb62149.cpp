// Write a C++ function `std::string inorderCheck(int n, const std::vector<std::tuple<int,int,bool>>& constraints)` that determines whether there exists a binary search tree (BST) containing exactly the integers `1` through `n` (in labels, not values) such that for every given constraint `(a, b, side)`, the node with label `a` must have the node with label `b` in its subtree in the specified direction: if `side == 0` (left), then `b` must be in the left subtree of `a`; if `side == 1` (right), then `b` must be in the right subtree of `a`. The function returns `"POSSIBLE"` if such a BST exists, or `"IMPOSSIBLE"` otherwise. The BST is defined by its shape only (the parent-child relationships among the labels), ignoring actual key values; the in-order traversal of the tree must be exactly `1,2,...,n`. You may assume `1 ≤ a < b ≤ n` (the smaller label is always the ancestor) and all constraints are consistent internally (i.e., no direct contradiction like a node having two left children).

#include <cassert>
#include <vector>
#include <tuple>
#include <string>
using namespace std;

// Declare the solution function (include the above implementation here in a real test)
std::string possible_bst(int n, const std::vector<std::tuple<int,int,bool>>& constraints);

int main() {
    // No constraints: always possible
    assert(possible_bst(3, {}) == "POSSIBLE");

    // 1 must have 2 as left child
    assert(possible_bst(3, {{1,2,0}}) == "POSSIBLE");

    // 1 must have both 2 (left) and 3 (right): valid
    assert(possible_bst(3, {{1,2,0}, {1,3,1}}) == "POSSIBLE");

    // 1 must have 3 as left and 2 as right: impossible because left must be smaller labels
    assert(possible_bst(3, {{1,3,0}, {1,2,1}}) == "IMPOSSIBLE");

    // 2 requires left 3, but 3 > 2 and cannot be left of 2 in in-order 1..3 (since 3 is after 2)
    assert(possible_bst(3, {{2,3,0}}) == "IMPOSSIBLE");

    // 2 requires right 3: possible (2 is root, right child 3)
    assert(possible_bst(3, {{2,3,1}}) == "POSSIBLE");

    // Chain: 1 right 2, 2 right 3
    assert(possible_bst(3, {{1,2,1}, {2,3,1}}) == "POSSIBLE");

    // Conflict: 1 right 2 and 1 left 2
    assert(possible_bst(3, {{1,2,0}, {1,2,1}}) == "IMPOSSIBLE");

    // Larger: 1 left all others, 2 left 3, etc. valid
    assert(possible_bst(5, {{1,2,0}, {1,3,0}, {1,4,0}, {1,5,0}}) == "POSSIBLE");

    // Larger with cross constraint: 1 left 2 and 1 right 5, but 2 right 3 and 3 left 4 -> possible
    assert(possible_bst(5, {{1,2,0}, {1,5,1}, {2,3,1}, {3,4,0}}) == "POSSIBLE");

    return 0;
}

#include <bits/stdc++.h>

// Returns "POSSIBLE" if a BST with in-order 1..n exists satisfying all constraints.
// Each constraint is (a, b, side) where a < b and side=0 means left, side=1 means right.
std::string possible_bst(int n, const std::vector<std::tuple<int,int,bool>>& constraints) {
    std::vector<std::vector<int>> left(n+1), right(n+1);
    for (const auto& [a, b, side] : constraints) {
        if (side == 0) left[a].push_back(b);
        else right[a].push_back(b);
    }
    for (int i = 1; i <= n; ++i) {
        std::sort(left[i].begin(), left[i].end());
        std::sort(right[i].begin(), right[i].end());
    }

    std::vector<int> fa(n+1), fin(n+1);
    std::iota(fa.begin(), fa.end(), 0);
    std::iota(fin.begin(), fin.end(), 0);

    std::function<int(int)> find = [&](int x) {
        return x == fa[x] ? x : fa[x] = find(fa[x]);
    };

    auto connect = [&](int x, int y) {
        // x is the node immediately after the current block ending at y
        int fy = find(y);
        fa[x] = fy;
        fin[fy] = x;
    };

    for (int i = n; i >= 1; --i) {
        bool has_left = !left[i].empty();
        bool has_right = !right[i].empty();
        if (has_left && has_right) {
            int lmax = left[i].back();
            int rmin = right[i][0];
            if (find(lmax) >= find(rmin)) return "IMPOSSIBLE";
        }

        if (has_left) {
            // left subtree must be the block starting at i+1 and containing all left[i]
            int start = i+1;
            if (start > n) return "IMPOSSIBLE";
            // Ensure i+1 is the root of the left subtree
            fa[start] = i;
            fin[i] = fin[start];
            while (find(left[i].back()) != i) {
                int child = fin[i] + 1;
                connect(child, fin[i]);
            }
        }

        if (has_right) {
            // right subtree must start at the block after left subtree or at i+1 if no left
            int rt_start;
            if (has_left) {
                rt_start = fin[i] + 1;
                if (rt_start > n) return "IMPOSSIBLE";
            } else {
                rt_start = i+1;
                if (rt_start > n) return "IMPOSSIBLE";
            }
            // Ensure the right subtree root is the first node of the right block
            // If we have no left, we need to attach as right child
            if (!has_left) {
                // make i's right child the root of the block starting at i+1
                if (find(rt_start) == i) return "IMPOSSIBLE";
                fa[rt_start] = i;
                fin[i] = fin[rt_start];
            }
            while (find(right[i].back()) != i) {
                int child = fin[i] + 1;
                connect(child, fin[i]);
            }
            // Now connect the right block to i
            int root_r = find(right[i][0]);
            if (root_r != i) {
                fa[root_r] = i;
                fin[i] = fin[root_r];
            }
        }
    }

    // After processing all, merge any leftover to form one tree
    while (find(n) != 1 && fin[1] != n) {
        int child = fin[1] + 1;
        connect(child, fin[1]);
    }

    // Final check: the root should be 1 and fin[1] == n
    if (fin[1] != n || find(1) != 1) return "IMPOSSIBLE";
    return "POSSIBLE";
}

// The problem reduces to constructing a BST whose in-order sequence is `1..n` (so the root is some integer, left subtree contains smaller labels, right subtree contains larger). Since the in-order is fixed, the shape is determined by choosing parent relationships. We process labels from `n` down to `1` (i.e., from largest to smallest). For each label `i`, we must handle constraints that require `i` to have a left or right child subtree containing certain larger labels. Key insight: For a label `i`, all constraints `(i, y, side)` require `y` to be in the left or right subtree of `i`. Since all such `y > i`, they must occupy a contiguous segment of the labels immediately following `i` in the in-order sequence (because the left subtree of `i` must consist of labels `i+1..k` for some `k`, and the right subtree must be `k+1..m`). If both left and right constraints exist for the same `i`, they must be separable: all left-subtree labels must be strictly smaller than all right-subtree labels, and the maximum left label must be less than the minimum right label. We use a disjoint-set (union-find) structure to merge nodes into connected components that must form a contiguous block in the in-order sequence. We process from `n` down to `1`, maintaining for each `i` the current `fa[i]` as the representative of the contiguous block starting at `i`. For each `i`, we check the constraints: if both left and right constraints exist and the blocks overlap or are in the wrong order, it's impossible. Then we assign children based on the constraints, merging blocks as needed. A key invariant: after processing `i`, the block containing `i` is exactly the set of nodes `i..fin[i]` that are already connected, and all constraints involving nodes inside this block are satisfied. If at any point we cannot merge without violating the in-order property, we return `"IMPOSSIBLE"`. After processing all labels, we merge any remaining disconnected blocks (to form the final tree) and verify that a valid tree exists. The algorithm runs in O((n+q) α(n)) time due to union-find and sorting, with O(n+q) space.
