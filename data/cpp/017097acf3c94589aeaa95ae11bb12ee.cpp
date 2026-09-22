/*
Given an integer `n` followed by two permutations `p` and `q` of the numbers `1` through `n` (each on its own line, space-separated), write a C++ function `findConsistentOrdering(int n, const std::vector<int>& p, const std::vector<int>& q)` that returns a vector containing the numbers from `1` to `n` in an order such that for any two elements `i` and `j` in that order, if `i` appears before `j`, then it must NOT be the case that both `i` appears after `j` in the permutation `p` AND `i` appears after `j` in the permutation `q`. In other words, the output order must respect the partial order where `i` must come before `j` if `i` occurs before `j` in both `p` and `q`. The returned vector should be any valid topological sort consistent with that partial order. If the input permutations are the same (i.e., `p == q`), the returned vector should be exactly that common order. The function should return a vector of size `n` containing each integer from `1` to `n` exactly once. The input `p` and `q` are guaranteed to be valid permutations of `1..n` (with no duplicates or missing numbers).
*/

#include <vector>
#include <queue>
#include <algorithm>

// Given two permutations p and q of 1..n, return a topological ordering
// of the poset where i < j iff i appears before j in both p and q.
std::vector<int> findConsistentOrdering(int n, const std::vector<int>& p, const std::vector<int>& q) {
    // Build position arrays for both permutations (1-indexed).
    std::vector<int> pos_p(n + 1), pos_q(n + 1);
    for (int i = 0; i < n; ++i) {
        pos_p[p[i]] = i;
        pos_q[q[i]] = i;
    }

    // Build adjacency matrix: edge i->j if i must come before j.
    std::vector<std::vector<bool>> adj(n, std::vector<bool>(n, false));
    std::vector<int> indeg(n, 0);

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i != j && pos_p[i+1] < pos_p[j+1] && pos_q[i+1] < pos_q[j+1]) {
                adj[i][j] = true;
                indeg[j]++;
            }
        }
    }

    // Kahn's algorithm with a min-heap to produce a deterministic ordering.
    std::priority_queue<int, std::vector<int>, std::greater<int>> pq;
    for (int i = 0; i < n; ++i) {
        if (indeg[i] == 0) {
            pq.push(i);
        }
    }

    std::vector<int> result;
    result.reserve(n);
    while (!pq.empty()) {
        int u = pq.top();
        pq.pop();
        result.push_back(u + 1); // convert back to 1-indexed
        for (int v = 0; v < n; ++v) {
            if (adj[u][v]) {
                --indeg[v];
                if (indeg[v] == 0) {
                    pq.push(v);
                }
            }
        }
    }

    // The problem guarantees a valid topological order exists, but if not,
    // fill remaining numbers (shouldn't happen). For safety, if result size < n,
    // append missing numbers in any order.
    if ((int)result.size() < n) {
        std::vector<bool> used(n + 1, false);
        for (int x : result) used[x] = true;
        for (int i = 1; i <= n; ++i) {
            if (!used[i]) result.push_back(i);
        }
    }
    return result;
}

#include <cassert>
#include <vector>

// Include the solution function here (or include the header).

int main() {
    // Case 1: identical permutations -> the same order.
    std::vector<int> p1 = {3, 1, 2};
    std::vector<int> q1 = {3, 1, 2};
    std::vector<int> r1 = findConsistentOrdering(3, p1, q1);
    assert(r1 == p1);

    // Case 2: p and q are opposite orders -> no constraints, any order valid.
    std::vector<int> p2 = {1, 2, 3};
    std::vector<int> q2 = {3, 2, 1};
    std::vector<int> r2 = findConsistentOrdering(3, p2, q2);
    // Since any order is valid, check it's a permutation of 1..3.
    std::vector<int> sorted_r2 = r2;
    std::sort(sorted_r2.begin(), sorted_r2.end());
    assert(sorted_r2 == std::vector<int>({1, 2, 3}));

    // Case 3: n=1.
    std::vector<int> p3 = {1};
    std::vector<int> q3 = {1};
    std::vector<int> r3 = findConsistentOrdering(1, p3, q3);
    assert(r3 == std::vector<int>({1}));

    // Case 4: n=4, some partial order.
    // p: 2 1 4 3, q: 2 3 1 4
    // Check constraints: 2 must come before all others because 2 first in both.
    // Between 1,3,4: 1 before 4? p:1 before4, q:1 before4 -> yes. 3 before 1? p:3 before1? p:3 is pos2,1 pos1 -> no. q:3 pos2,1 pos2 -> no. So 3 and 1 incomparable. 4 before3? p:4 pos3,3 pos2 -> no; q:4 pos3,3 pos1 -> no. So 4 and 3 incomparable.
    // Edges: 2->1,2->4,2->3,1->4. So valid topo orders: 2,1,3,4 or 2,3,1,4 or 2,1,4,3 etc.
    std::vector<int> p4 = {2, 1, 4, 3};
    std::vector<int> q4 = {2, 3, 1, 4};
    std::vector<int> r4 = findConsistentOrdering(4, p4, q4);
    // Check that 2 comes first.
    assert(r4[0] == 2);
    // Check that 1 comes before 4.
    auto pos1 = std::find(r4.begin(), r4.end(), 1);
    auto pos4 = std::find(r4.begin(), r4.end(), 4);
    assert(pos1 < pos4);

    // Case 5: p and q are same but reversed indices, n=5.
    std::vector<int> p5 = {5, 4, 3, 2, 1};
    std::vector<int> q5 = {5, 4, 3, 2, 1};
    std::vector<int> r5 = findConsistentOrdering(5, p5, q5);
    assert(r5 == p5);

    return 0;
}

// The problem asks us to produce a topological sort of a directed graph where an edge exists from `i` to `j` if both `i` appears before `j` in permutation `p` and also before `j` in permutation `q`. This graph is a partial order (transitive and acyclic) because it’s the intersection of two total orders, and the intersection of two partial orders is a partial order. A key observation: the partial order is exactly the comparability relation given by the product order on the plane: each element `i` has coordinates `(pos_p[i], pos_q[i])`. Then `i < j` in the partial order iff both coordinates of `i` are less than both coordinates of `j`. The problem statement wants us to output any linear extension (topological order) of this poset. Since the intersection of two total orders is a partial order, a topological sort always exists. We can perform a topological sort using Kahn’s algorithm: compute in-degree counts for each element (number of other elements that must come before it), maintain a queue (or priority queue if we want a deterministic order, e.g., smallest indices first) of elements with zero in-degree, pop them, append to result, and decrement in-degrees of their successors. To efficiently find successors, for each element `i` we need all `j` such that `pos_p[i] < pos_p[j]` and `pos_q[i] < pos_q[j]`. We can brute-force: for each `i`, iterate over all `j` and add an edge if both conditions hold. For `n` up to say 5000, this is O(n^2) edges, which is acceptable for this task. Edge cases: when `p == q`, the graph becomes a total order, and topological sort gives exactly that order. When `n=1`, the result is just `[1]`. Another edge case: if there are no edges (e.g., `p` and `q` are opposite orders), then any topological order is valid, and our algorithm will output the elements in the order they become zero in-degree, which might be the order `1,2,...,n` if we use a simple queue. Time complexity: O(n^2) to build edges and O(n^2) for Kahn's algorithm if we use adjacency matrix or list; space O(n^2) for adjacency. For typical constraints (n <= 1000), it's fine. We'll implement it with O(n^2) time and O(n) extra space using in-degree array and a boolean adjacency matrix stored as vector of char for brevity.
