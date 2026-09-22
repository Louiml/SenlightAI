/*
You are given a network of power stations where each station can transmit power to at most two other stations, forming a binary tree. Each station has an integer power demand that must be satisfied (negative values indicate demand, positive values indicate surplus). Write a standalone C++ function `long long computeMaxPower(const vector<int>& demands, int rootIndex)` that takes a vector of demands and the index of the root station, and returns the maximum total power that can be delivered from the root to all other stations, assuming power can only flow outward from the root along tree edges, and each edge can carry power in one direction only (from parent to child). The tree is implicitly defined by indexing: for a node at index `i`, its left child is at index `2*i + 1` and its right child is at index `2*i + 2`, if those indices are within the vector bounds. A station’s demand is satisfied if it receives exactly that amount of power (positive means it consumes that much, negative means it can donate that much to its parent). The root must be able to satisfy the total net demand of its subtree; if impossible, return the maximum total power that can be delivered to the root from its descendants (which may be negative if the root itself has demand).
*/
#include <vector>
#include <climits>

// Compute maximum net power contribution from a subtree rooted at index i.
long long computeSubtree(const std::vector<int>& demands, int i, long long& globalMax) {
    if (i >= (int)demands.size()) return 0; // no child

    long long left = computeSubtree(demands, 2*i + 1, globalMax);
    long long right = computeSubtree(demands, 2*i + 2, globalMax);

    // If either child is impossible, this node is impossible.
    if (left == LLONG_MIN || right == LLONG_MIN) return LLONG_MIN;

    long long contribution = left + right - demands[i];
    // Track the maximum possible net power that could be delivered to this node
    // from its descendants (i.e., the maximum positive value we can push up).
    // This is used only for reporting, but not needed for final answer.
    globalMax = std::max(globalMax, contribution);
    return contribution;
}

// Return the maximum total power that can be delivered to the root from all descendants,
// satisfying all demands. If impossible, return LLONG_MIN.
long long computeMaxPower(const std::vector<int>& demands, int rootIndex) {
    long long dummy = LLONG_MIN;
    long long result = computeSubtree(demands, rootIndex, dummy);
    return result;
}
#include <cassert>
#include <vector>
#include <climits>

// The solution function is declared above (not repeated here for brevity in test).

int main() {
    // Single node with demand 5 (needs 5) -> cannot satisfy, contribution = -5
    std::vector<int> d1 = {5};
    assert(computeMaxPower(d1, 0) == -5);

    // Single node with supply -3 (can donate 3) -> contribution = 3
    std::vector<int> d2 = {-3};
    assert(computeMaxPower(d2, 0) == 3);

    // Root with demand 4, children supplies -1 and -2 -> total contribution = -1 + -2 - 4 = -7
    std::vector<int> d3 = {4, -1, -2};
    assert(computeMaxPower(d3, 0) == -7);

    // Root with supply -5, children demands 2 and 3 -> contribution = 2 + 3 - (-5) = 10
    std::vector<int> d4 = {-5, 2, 3};
    assert(computeMaxPower(d4, 0) == 10);

    // Leaf child missing: root demand 1, left child supply -4, right child absent
    // Morph: vector {1, -4} (right child at index 2 absent)
    std::vector<int> d5 = {1, -4};
    assert(computeMaxPower(d5, 0) == -4 - 1); // -5

    // Impossible subtree: child demand huge, parent cannot satisfy
    std::vector<int> d6 = {0, 1000000000};
    // Contribution = 1000000000 - 0 = 1000000000 (impossible? actually it is possible because child can demand from parent? Wait, demand positive means needs that much; parent must supply. But parent has no external source, so contribution is negative of total net? Re-evaluate: For leaf at index 1 with demand 1000000000, contribution = -1000000000. Root contribution = -1000000000 + 0 = -1000000000, not impossible. So this is fine.
    assert(computeMaxPower(d6, 0) == -1000000000LL);

    // More complex tree: indices: 0 root demand 2, children 1 and 2 with supplies -1 each,
    // grandchild of node 1 (index 3) demand 100 (impossible to satisfy because not enough supply)
    std::vector<int> d7 = {2, -1, -1, 100};
    // left subtree (1): contribution = (contribution of 3 = -100) + (-1) - (-1)?? Wait correct: node 3 leaf demand 100 -> -100. Node 1: left=-100, right absent, demand -1 -> contribution = -100 - (-1) = -99. Node 2: leaf -1 -> 1. Root: -99 + 1 - 2 = -100. So not impossible? Actually -100 is possible because net contribution negative. To make impossible, child demand exceeds what parent can supply? But since root has no external source, any negative contribution is possible as long as we allow borrowing? Actually demand must be satisfied, so if a node has positive demand, it must receive that power from parent. That is possible because parent can get from elsewhere. So no impossibility ever? Unless we impose constraint that a node cannot receive from both children and parent? The problem says power flows outward from root only, so children send surplus up, parent sends down. So any node can meet demand by receiving from parent. The only constraint is the root has no parent, so if root demand is positive, it needs external source, which is not available, so root contribution becomes negative meaning deficit. But we can still compute. So the function always returns a finite value; LLONG_MIN never used. Let's just test that.
    long long res7 = computeMaxPower(d7, 0);
    assert(res7 == -100);

    return 0;
}
// This is a tree DP problem where each node must decide how much power to send to its parent, given that its own demand must be met and it may receive power from its children. For each node, we compute the maximum net power that can be contributed upward (positive if surplus, negative if deficit) while satisfying all demands in its subtree. The recurrence: for a leaf, contribution = -demand (because if demand is positive it needs that much incoming, so contribution negative; if demand is negative it can donate that much). For an internal node, sum the contributions from its left and right children (if they exist), then subtract its own demand. If the child’s subtree is impossible to satisfy internally (contribution = -infinity), then the whole subtree is impossible. The answer for the root is the contribution computed at the root (since root has no parent). Edge cases: single node, unbalanced tree (some nodes have only one child), and cases where children are absent (leaf). Time complexity is O(n) with n being the number of nodes, and space complexity O(depth) for recursion, worst-case O(n). Use `LLONG_MIN` to represent impossibility, and treat it as a sentinel.
