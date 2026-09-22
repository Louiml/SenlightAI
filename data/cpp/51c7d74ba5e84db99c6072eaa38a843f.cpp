// You are given two positive integers `n` and `m` describing a rooted binary tree that has `n` vertices (numbered `1` through `n`) in total. The tree is constructed level by level as follows: the root is vertex `1` and belongs to level `0`; every node at level `d` has exactly two children at level `d+1`, except that if adding another full level would exceed `n` vertices, the last level is filled left to right (so every level except possibly the last is full). For example, for `n=6`, levels are: level 0 has 1 node, level 1 has 2 nodes, level 2 has 3 nodes (only the first 3 of a possible 4). For each node, define its "child-depth" as the number of edges from that node to the deepest leaf in its subtree. Let `S` be the sum of child-depths over all vertices. Write a C++ function `bool isPossibleChildDepthSum(int n, long long m)` that returns `true` if there exists a labeling of the vertices with distinct labels `1..n` such that the sum of the labels of the children of each vertex (only for vertices that have children) equals exactly `m`. If no such labeling exists, return `false`. The output must be correct for all `1 <= n <= 10^5` and `m` fitting in a 64-bit signed integer. Note: This is a special variant where the sum is over the labels of children, not depths, and you must decide feasibility based solely on structural properties of the perfect-then-full binary tree.

#include <cassert>

int main() {
    // n=1: no children, sum=0
    assert(isPossibleChildDepthSum(1, 0) == true);
    assert(isPossibleChildDepthSum(1, 5) == false);

    // n=2: root has one child (label 2), sum must be 2
    assert(isPossibleChildDepthSum(2, 2) == true);
    assert(isPossibleChildDepthSum(2, 3) == false);

    // n=3: root has two children (2 and 3), sum=5 always
    assert(isPossibleChildDepthSum(3, 5) == true);
    assert(isPossibleChildDepthSum(3, 4) == false);

    // n=4: children labels are 2,3,4 (root has two children, one child has one child?)
    // Actually tree: root (1) children 2,3; node 2 has child 4. Sum = 2+3+4=9.
    assert(isPossibleChildDepthSum(4, 9) == true);
    assert(isPossibleChildDepthSum(4, 8) == false);

    // n=5: children 2..5 sum=2+3+4+5=14
    assert(isPossibleChildDepthSum(5, 14) == true);
    assert(isPossibleChildDepthSum(5, 15) == false);

    // Large n: sum = (n+2)*(n-1)/2
    int n = 100000;
    long long expected = static_cast<long long>(n + 2) * (n - 1) / 2;
    assert(isPossibleChildDepthSum(n, expected) == true);
    assert(isPossibleChildDepthSum(n, expected + 1) == false);

    // n=6: sum = (8*5)/2 = 20
    assert(isPossibleChildDepthSum(6, 20) == true);

    // n=10: sum = (12*9)/2 = 54
    assert(isPossibleChildDepthSum(10, 54) == true);
}

#include <algorithm>
#include <cstdint>

// Returns whether the sum of child labels can equal exactly targetSum for a
// complete binary tree with n vertices, numbered 1..n.
bool isPossibleChildDepthSum(int n, long long targetSum) {
    if (n <= 0) return false;
    if (n == 1) return targetSum == 0; // no children

    // Compute minimum and maximum possible sum of child labels.
    // The tree is built level by level: root at level 0, then 2,4,8,... full
    // levels, except the last level may be partially filled left to right.

    // We will simulate the tree level order using contiguous ranges of vertex numbers.
    // For each parent level, we know the range of parent vertex labels, and the
    // range of child vertex labels (the next block of size 2 * numParents).

    long long minSum = 0;
    long long maxSum = 0;

    int parentsStart = 1;       // first vertex label of current parent level
    int parentsCount = 1;       // number of parents on this level
    int nextLabel = 2;          // next vertex label available for children

    while (parentsCount > 0 && nextLabel <= n) {
        int availableChildren = std::min(2 * parentsCount, n - nextLabel + 1);
        if (availableChildren <= 0) break;

        // For minimum sum: assign the smallest `availableChildren` labels as children.
        // Each parent gets exactly 2 children (except possibly the last parent on the
        // final partial level, which may get 1 child). But since we process level by
        // level, each parent on this level gets exactly 2 children if the last level
        // is full, otherwise some parents may have fewer. In a complete tree, the
        // number of children per parent is 2 for all but possibly the last parent
        // on the final level, which may have 1 child. To keep this simple, we notice
        // that the sum of children labels is just the sum of all labels in the child
        // range, because every child appears exactly once as a child of some parent.
        // Therefore minSum and maxSum are the same! Because the set of children
        // labels is exactly {nextLabel, nextLabel+1, ..., nextLabel+availableChildren-1}.
        // Ordering among parents does not change the sum. So minSum == maxSum.

        // Indeed, every vertex except the root is a child of exactly one parent.
        // Thus the sum of all child labels is simply the sum of labels 2..n.
        // The total sum is independent of ordering. So the only possible value is
        // sum_{i=2}^n i = (n+2)*(n-1)/2.

        // So we can directly compute it.
        break;
    }

    // The sum of child labels equals the sum of all vertex labels except the root.
    // Because each non-root vertex is a child of exactly one parent.
    long long totalSumAll = static_cast<long long>(n) * (n + 1) / 2;
    long long possibleSum = totalSumAll - 1; // subtract root label 1

    return targetSum == possibleSum;
}

// The problem reduces to determining whether the desired sum `m` lies within the achievable range of sums of child labels for a given binary tree shape. The tree shape is fixed once `n` is given: it is a complete binary tree where all levels are full except possibly the last. Let `totalVertices = n`. The root has 2 children (since we always have at least `n >= 3`). For any node, its children (if any) are exactly the next available vertex numbers in the level-order assignment. To minimize the sum of child labels, we always put the smallest available labels as children of the earliest nodes (left to right). This yields a deterministic minimum possible sum `minSum`. To maximize, we put the largest labels as children (i.e., reverse the order on each level), giving `maxSum`. Any integer sum between `minSum` and `maxSum` (inclusive) is achievable because we can swap labels among children at the same parent without changing the sum for other parents, and we can adjust by swapping labels between different parents to fine-tune the sum by 1, as long as the tree has at least two nodes on some level. For `n=1`, there are no children, so only `m=0` is possible; for `n=2`, the root has one child? Actually for `n=2`, the tree shape: root level 0 with 1 node; level 1 has 2 children? But we only have 2 vertices total, so the root has 1 child (the last level is partially filled left to right: first child exists). So we need to handle small `n` carefully. More generally, compute `minSum` and `maxSum` recursively: For a subtree of size `k` (number of vertices in that subtree), the minimum sum of child labels in that subtree is achievable by assigning the smallest labels to the deepest subtrees first, but an easier closed form exists: For a full binary tree with `k` nodes, the minimum sum of child labels is `sum_{v} (smallest label among children)`. Since the tree is built level by level, we can simulate level order and assign labels greedily. A simpler approach: The minimum sum is achieved by assigning labels in level-order (i.e., the tree's natural numbering), and the maximum by reversing the order at each parent level. Then check if `m` is between these extremes. Complexity: `O(log n)` to compute sum of an arithmetic‑progression‑like series; but we can do a recursive computation in `O(log n)` depth. The total time is `O(log n)` per call, space `O(log n)` stack. Edge cases: `n=1`, `n=2`, and when `minSum == maxSum` (e.g., `n=3`: root has children 2 and 3, sum=5 regardless of ordering, so only `m=5` possible). Also note that `m` can be up to roughly `n^2` (since each child label is ≤n and there are about n/2 parent-child edges), but fits in 64-bit.
