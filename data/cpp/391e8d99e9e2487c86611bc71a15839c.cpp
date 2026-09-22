/*
Given a rooted tree with `n` nodes (numbered 1..n) and a specified root node, where each node has a processing cost `a[i]` and an initial delay `b[i]` (initially zero for all nodes), write a C++ function `computeTotalCost` that takes as parameters the number of nodes `n`, the root index `root`, a vector of `a` values (size n+1, 1-indexed), a vector of `parent` values (size n+1, 1-indexed, where parent[i]=0 for root, and parent for others is a valid parent index), and returns the minimized total completion cost when all nodes are processed sequentially. Each node’s processing takes 1 unit of time but can be merged into its parent (if the parent is not already processed) to combine costs: when merging node `v` into parent `p`, the parent’s `a[p]` increases by `a[v]`, the parent’s `b[p]` becomes `b[p] + b[v] + a[v] * t[p]` where `t[p]` is the number of nodes currently in the merged group of `p`, and `t[p]` increases by `t[v]`. All children of `v` become children of `p`. When a node or group has no parent (i.e., its parent is 0), it is processed immediately: its cost contributes `a[group] * current_time + b[group]` to the total, and `current_time` is incremented by the group size `t[group]`. The order of merging is greedy: at each step, among all unprocessed nodes, select the node with the highest cost rate `a[i] / t[i]` (where `t[i]` is the size of the group the node currently represents, initially 1 for each node). If that node has a parent, merge it into the parent; otherwise, process it. Stop when all nodes are processed. The input may have multiple test cases; the function should be called separately for each. The output for each test case is the total cost (an integer).
*/
#include <vector>
#include <algorithm>
#include <limits>

// Compute the minimized total completion cost for a rooted tree.
// Parameters are 1-indexed: a[1..n], parent[1..n] with parent[root]=0.
long long computeTotalCost(int n, int root,
                           const std::vector<int>& a,
                           const std::vector<int>& parent_in) {
    // Work with mutable copies.
    std::vector<long long> A(n + 1);
    std::vector<long long> B(n + 1, 0);
    std::vector<int> T(n + 1, 1);
    std::vector<int> parent = parent_in;
    std::vector<std::vector<int>> children(n + 1);
    std::vector<int> used(n + 1, 0);

    for (int i = 1; i <= n; ++i) {
        A[i] = a[i];
    }
    for (int i = 1; i <= n; ++i) {
        if (parent[i] != 0) {
            children[parent[i]].push_back(i);
        }
    }

    int nodeCount = n;
    long long totalCost = 0;
    int time = 1;

    while (nodeCount > 0) {
        int idx = -1;
        double bestRate = -1.0;
        for (int i = 1; i <= n; ++i) {
            if (used[i]) continue;
            double rate = (double)A[i] / T[i];
            if (rate > bestRate) {
                bestRate = rate;
                idx = i;
            }
        }

        if (parent[idx] == 0) {
            // This group is a root; process it.
            for (int child : children[idx]) {
                parent[child] = 0;
            }
            totalCost += A[idx] * time + B[idx];
            time += T[idx];
        } else {
            int p = parent[idx];
            A[p] += A[idx];
            B[p] += B[idx] + A[idx] * T[p];
            T[p] += T[idx];
            for (int child : children[idx]) {
                parent[child] = p;
                children[p].push_back(child);
            }
        }

        used[idx] = 1;
        --nodeCount;
    }

    return totalCost;
}
#include <cassert>
#include <vector>

// The function under test is assumed to be declared before main.
// We copy it here for completeness of the test file.
long long computeTotalCost(int n, int root,
                           const std::vector<int>& a,
                           const std::vector<int>& parent_in);

int main() {
    // Test 1: Single node tree, root=1. Cost = a[1]*1 + 0 = 5.
    {
        int n = 1, root = 1;
        std::vector<int> a = {0, 5};
        std::vector<int> parent = {0, 0};
        assert(computeTotalCost(n, root, a, parent) == 5);
    }
    // Test 2: Chain 1 -> 2, root=1. 
    // a1=1, a2=10. Greedy selects node 2 (rate 10) first, merges into 1:
    // A1 becomes 11, B1=0+0+10*1=10, T1=2. Then process root at time=1:
    // cost = 11*1 + 10 = 21.
    {
        int n = 2, root = 1;
        std::vector<int> a = {0, 1, 10};
        std::vector<int> parent = {0, 0, 1};
        assert(computeTotalCost(n, root, a, parent) == 21);
    }
    // Test 3: Star: root 1 with children 2,3. a1=1, a2=2, a3=3.
    // Greedy picks child 3 (rate 3) first, merges into root: A1=4, B1=0+0+3*1=3, T1=2.
    // Then picks child 2 (rate 2) merges into root: A1=6, B1=3+0+2*2=7, T1=3.
    // Process root at time=1: cost=6*1+7=13.
    {
        int n = 3, root = 1;
        std::vector<int> a = {0, 1, 2, 3};
        std::vector<int> parent = {0, 0, 1, 1};
        assert(computeTotalCost(n, root, a, parent) == 13);
    }
    // Test 4: Tree with two roots (not possible in single-root, but parent[2]=0 for a separate tree).
    // Actually enforce root=1 but parent[2]=0 would make node 2 also root; process them in order of rates.
    // a1=2, a2=3. Both are roots. rates: 2 and 3. Pick 2 (rate 3) first: time=1 cost=3*1=3, time=2.
    // Then pick 1: cost=2*2=4. Total=7.
    {
        int n = 2, root = 1;
        std::vector<int> a = {0, 2, 3};
        std::vector<int> parent = {0, 0, 0};
        assert(computeTotalCost(n, root, a, parent) == 7);
    }
    // Test 5: Deeper tree: 1->2, 2->3. root=1.
    // a1=1, a2=1, a3=100. Greedy picks 3 (rate 100) merges into 2: A2=101, B2=0+0+100*1=100, T2=2.
    // Now rates: node1=1, node2=50.5. Pick node2 merges into 1: A1=102, B1=0+100+101*1=201, T1=3.
    // Process root at time=1: cost=102*1+201=303.
    {
        int n = 3, root = 1;
        std::vector<int> a = {0, 1, 1, 100};
        std::vector<int> parent = {0, 0, 1, 2};
        assert(computeTotalCost(n, root, a, parent) == 303);
    }
    // Test 6: Empty? Not applicable; n>=1.
    // Test 7: Node with a child and root has another child, weights cause ordering.
    // root=1, children 2 and 3. a1=0, a2=5, a3=6. 
    // Greedy: rates 5 and 6. Pick 3 merges into 1: A1=6, B1=0+0+6*1=6, T1=2.
    // Then pick 2 (rate 5) merges into 1: A1=11, B1=6+0+5*2=16, T1=3.
    // Process root at time=1: cost=11*1+16=27.
    {
        int n = 3, root = 1;
        std::vector<int> a = {0, 0, 5, 6};
        std::vector<int> parent = {0, 0, 1, 1};
        assert(computeTotalCost(n, root, a, parent) == 27);
    }
    return 0;
}
// The core idea is a greedy algorithm that repeatedly selects the node with the maximum average cost per unit time within its current merged group (`a[i]/t[i]`). Since merging a node into its parent effectively postpones that group’s processing until the parent is processed, the algorithm always processes the group with the highest “density” first, either by merging it upward (if it has a parent) or by actually paying its cost when it becomes a root. This is analogous to the classic problem of minimizing weighted completion time on a tree with preemption by merging. The algorithm maintains for each node the aggregate `a`, `b`, and size `t` of the group rooted at that node. Initially `t[i]=1`, `b[i]=0`. The selection step scans all nodes not yet used, computing `a[i]*1.0/t[i]` and picking the maximum. If the chosen node is a root (parent=0), its children become new roots (parent=0), and we add `a[idx]*time + b[idx]` to the total, then increment `time` by `t[idx]`. If it has a parent `p`, we merge: update `a[p] += a[idx]`, `b[p] += b[idx] + a[idx]*t[p]`, `t[p] += t[idx]`, and reassign all children of `idx` to be children of `p`, also updating their parent pointers. The chosen node is marked used. Repeat until all nodes are used. Edge cases: the tree is given as parent array; the root has parent 0. If a node is a root and has no children, it is just processed. If multiple nodes have the same maximum ratio, any tie-breaking works; the original code picks the first encountered. The algorithm runs in O(n^2) per test case due to the linear scan for each of the n nodes, and O(n) additional space for arrays. Time complexity O(n^2) and space O(n).
