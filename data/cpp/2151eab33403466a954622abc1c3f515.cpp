You are given a rooted tree with `n` nodes (numbered from 1 to `n`), rooted at node 1. Each node has an associated weight `w[i]`. Define `sub[u]` as the number of nodes in the subtree rooted at `u` (including `u`), and `sum[u]` as the sum of weights of all nodes in that subtree. You must process `q` queries, each of type `1 x` or `2 x`. For type `1 x`, output the current `sum[x]`. For type `2 x`, perform a rotation operation: if node `x` has at least one child, let `s` be the child of `x` with the largest subtree size (ties broken by the smallest node index). Then perform a "rotation" as in an splay tree: the subtree rooted at `x` is re-rooted at `s`, meaning that `s` becomes the parent of `x`, and the previous parent `p` of `x` becomes the parent of `s`. As a result, all subtree sizes and sums must be updated appropriately for nodes `u`, `s`, and their parent `p` (note: `x`'s children other than `s` remain children of `x`, and `s`'s children remain children of `s`). Implement a function `int processQueries(...)`? Actually, the task is: Write a C++ function `int performRotation(int n, const vector<int>& weights, const vector<pair<int,int>>& edges, int x, vector<int>& sub, vector<long long>& sum)` that, given a rooted tree (root at 1) with initial `sub` and `sum` computed correctly, performs the rotation operation on node `x` as described, and returns 1. However, the simpler task is: implement the rotation logic given the tree, weights, and the current `sub` and `sum`, and update them in-place. The function should not read input or output; it should just update the given vectors. The tree adjacency is provided, and you may precompute parent and children. But note the rotation changes the parent relationships. So the function must update the parent pointers as well. Provide a standalone function `void rotateNode(int x, vector<int>& par, vector<vector<int>>& children, vector<int>& sub, vector<long long>& sum)` that performs the rotation as described, assuming `children` is a vector of sets or sorted vectors representing the current children of each node, and `par` is the current parent (with `par[1]=0`). In the rotation, the child `s` with largest `sub` (tie: smallest index) is selected, and the rotation is performed as in the snippet. The function should update `par`, `children`, `sub`, and `sum` accordingly. This is the task: implement exactly the `rot` function's logic from the snippet, generalized to a vector-based representation, and provide a test.
The core operation is a rotation in a tree that preserves the subtree sums and sizes. Given a node `u` with parent `p` and a chosen child `s` (the "heaviest" child), we re-link the tree so that `s` becomes the parent of `u`, and `p` becomes the parent of `s`. This is exactly the splay tree rotation but on a general rooted tree. The key observation is that the subtree of `u` originally consists of `u`, its other children (excluding `s`), and the subtree of `s` and its descendants. After rotation, the subtree rooted at `s` becomes the whole original subtree of `u` (since `u` and its other children become children of `s`), and the subtree rooted at `u` becomes the original subtree of `u` minus the subtree of `s`. Therefore, the new `sub[u] = old_sub[u] - old_sub[s]`, and new `sub[s] = old_sub[u]`. Similarly, `sum[u] = old_sum[u] - old_sum[s]`, and `sum[s] = old_sum[u]`. The parent of `u` changes from `p` to `s`, and the parent of `s` changes from `u` to `p`. The children sets: remove `s` from `u`'s children, add `u` to `s`'s children, remove `u` from `p`'s children, add `s` to `p`'s children. If `u` has no children, do nothing. Also, if `u` is the root (parent 0), then `p` is not defined; in that case, the rotation is still valid: `p` should be treated as 0 (no parent) and we just remove and add accordingly. Edge cases: when `u` is root, `p` is 0, but we still need to update the parent of `s` to 0 and remove `u` from `p`'s children (but `p` doesn't exist, so skip that part). The snippet assumes a dummy node 0 (since `par` initialized as -1 but in the snippet they use `sons[p]` even for the root? Actually in the snippet, `p` is always a valid node because the root's parent is -1 and they never call rot on the root? They do call rot on any node, but `sons[p]` where p = -1 would cause issues; but they likely never call rot on the root because the queries ensure x is not root? Actually the snippet does not avoid root; but `sons[-1]` would be invalid. So in our implementation, we must handle the root carefully: if `u` is the root, then `p` is 0 (a sentinel) and we simply skip updating `children[p]`. The function should check if `p != 0`. Also, we must select the heaviest child using a sorted set (by `{size, index}`) as in the snippet. We can store children in a `std::set` or a sorted vector, but since rotation changes children, we need to maintain them efficiently. For a standalone task, we can use `std::set<pair<int,int>>` per node, but simpler: we can store children as a vector and recompute the heaviest each time, but that's O(degree). For correctness, it's fine. Then time complexity per rotation is O(log deg) if using set, or O(deg) if linear search. For the task, we'll assume we have a `vector<set<pair<int,int>>>` childrenSet that stores (subsize, index) for each child, and we update it. We also maintain `par` vector. The function will take `u`, `par`, `childrenSet` (which is a reference to a vector of sets), `sub`, and `sum`. Implementation details: first find `s` from `childrenSet[u].begin()`; if empty, return. Then perform the updates as described. For `childrenSet`, we need to erase and insert accordingly: `childrenSet[u].erase({sub[s], s})`; `childrenSet[s].insert({newSubU, u})`; if `p != 0`, `childrenSet[p].erase({sub[u], u})`; `childrenSet[p].insert({newSubS, s})`. Also update `par`. Then update `sub` and `sum`. That's the entire logic. Complexity: O(log deg) per rotation due to set operations, but the overall solution for multiple queries would be O(q log n). For the test, we need to verify correctness.
#include <bits/stdc++.h>
using namespace std;

// Perform a rotation operation on node u.
// Updates parent pointers, children sets, subtree sizes, and subtree sums.
// childrenSet[u] stores pairs {subsize, childIndex} to easily pick the largest.
void rotateNode(
    int u,
    vector<int>& par,
    vector<set<pair<int,int>>>& childrenSet,
    vector<int>& sub,
    vector<long long>& sum
) {
    if (childrenSet[u].empty()) {
        return; // no children, nothing to rotate
    }

    // Select the heaviest child: largest size, tie by smallest index.
    auto it = childrenSet[u].begin();
    int s = it->second;

    int p = par[u];
    int old_sub_u = sub[u];
    int old_sub_s = sub[s];
    long long old_sum_u = sum[u];
    long long old_sum_s = sum[s];

    int new_sub_u = old_sub_u - old_sub_s;
    int new_sub_s = old_sub_u;
    long long new_sum_u = old_sum_u - old_sum_s;
    long long new_sum_s = old_sum_u;

    // Update child relationships
    childrenSet[u].erase({old_sub_s, s});
    childrenSet[s].insert({new_sub_u, u});
    if (p != 0) {
        childrenSet[p].erase({old_sub_u, u});
        childrenSet[p].insert({new_sub_s, s});
    }

    // Update parent pointers
    par[u] = s;
    par[s] = p;

    // Update sizes and sums
    sub[u] = new_sub_u;
    sub[s] = new_sub_s;
    sum[u] = new_sum_u;
    sum[s] = new_sum_s;
}
#include <bits/stdc++.h>
using namespace std;

// declare the function from solution (must be defined before main)
void rotateNode(int, vector<int>&, vector<set<pair<int,int>>>&, vector<int>&, vector<long long>&);

int main() {
    // Build a small tree: 1-2, 1-3, 2-4, 2-5, 3-6
    // Weights: 1..6
    int n = 6;
    vector<int> weight = {1, 2, 3, 4, 5, 6};
    vector<int> par(n + 1, 0);
    vector<vector<int>> adj(n + 1);
    vector<pair<int,int>> edges = {{1,2},{1,3},{2,4},{2,5},{3,6}};
    for (auto e : edges) {
        adj[e.first].push_back(e.second);
        adj[e.second].push_back(e.first);
    }

    // DFS to compute initial par, sub, sum
    vector<int> sub(n + 1, 1);
    vector<long long> sum(n + 1);
    function<void(int,int)> dfs = [&](int u, int p) {
        par[u] = p;
        sum[u] = weight[u];
        for (int v : adj[u]) {
            if (v == p) continue;
            dfs(v, u);
            sub[u] += sub[v];
            sum[u] += sum[v];
        }
    };
    dfs(1, 0);

    // Build childrenSet
    vector<set<pair<int,int>>> childrenSet(n + 1);
    for (int u = 1; u <= n; u++) {
        for (int v : adj[u]) {
            if (v != par[u]) {
                childrenSet[u].insert({sub[v], v});
            }
        }
    }

    // Initial sums: node 1 sum = 21, node 2 sum = 2+4+5 = 11, node 3 sum = 9, node 4=4, node5=5, node6=6
    assert(sum[1] == 21 && sum[2] == 11 && sum[3] == 9 && sum[4] == 4 && sum[5] == 5 && sum[6] == 6);

    // Rotate node 2 (its heaviest child is 4 or 5? both size1, tie smallest index -> 4)
    rotateNode(2, par, childrenSet, sub, sum);
    // After rotation: 2's parent becomes 4, 4's parent becomes 1.
    // sub[2] = old_sub[2] - old_sub[4] = 3 - 1 = 2, sum[2] = 11 - 4 = 7
    // sub[4] = old_sub[2] = 3, sum[4] = old_sum[2] = 11
    // Check: children of 4 now include 2 and it's original? 4 had no children originally, now 4 has child 2.
    // Also node 1's children: originally {2,3}, now remove 2 and add 4.
    assert(par[2] == 4 && par[4] == 1);
    assert(sub[2] == 2 && sub[4] == 3);
    assert(sum[2] == 7 && sum[4] == 11);
    // Check children sets
    assert(childrenSet[1].count({3,4}) == 1 && childrenSet[1].size() == 2); // {3,6} and {4,3}
    assert(childrenSet[4].count({2,2}) == 1 && childrenSet[4].size() == 1);
    assert(childrenSet[2].size() == 1); // child 5
    assert(childrenSet[2].count({1,5}) == 1);

    // Rotate node 4 (now children: only 2 with size2? Actually sub[2]=2, so heaviest is 2)
    rotateNode(4, par, childrenSet, sub, sum);
    // After rotation: 4's parent becomes 2, 2's parent becomes 1.
    // old_sub[4]=3, old_sub[2]=2 -> new_sub[4]=1, new_sub[2]=3
    // old_sum[4]=11, old_sum[2]=7 -> new_sum[4]=4, new_sum[2]=11
    assert(par[4] == 2 && par[2] == 1);
    assert(sub[4] == 1 && sub[2] == 3);
    assert(sum[4] == 4 && sum[2] == 11);
    // Check children sets: 2 now has children {5,4}? 5 size1, 4 size1 -> tie, but insertion order? Actually 5 was already there, now 4 added with size1 and index4, so both size1, tie smallest index 4, so 2's heaviest is 4.
    assert(childrenSet[2].size() == 2);
    assert(childrenSet[2].count({1,5}) == 1);
    assert(childrenSet[2].count({1,4}) == 1);
    assert(childrenSet[1].count({3,2}) == 1); // node 2 now has sub 3
    assert(childrenSet[1].size() == 2); // {2,3} and {3,6}
    // Node 4 has no children
    assert(childrenSet[4].empty());

    // Rotate on leaf: nothing changes
    rotateNode(6, par, childrenSet, sub, sum);
    assert(sub[6] == 1 && sum[6] == 6 && childrenSet[6].empty());

    // Rotate on root? Node 1 has children {2,3}, heaviest is 3 (size3 vs 2? Actually sub[2]=3, sub[3]=3? Wait sub[3] originally was 3, after rotations sub[3] unchanged? Let's compute: original sub[3]=2? Wait node3 originally has child6, so sub[3]=2 (3 and 6), sub[2]=3 originally. After the first rotation, sub[2] became 2, sub[3] still 2. After second rotation, sub[2] became 3. So now sub[2]=3, sub[3]=2, so heaviest child of 1 is 2 with size3, tie? Only one. Rotate root 1.
    rotateNode(1, par, childrenSet, sub, sum);
    // old_sub[1] = 6, old_sub[2] = 3 -> new_sub[1]=3, new_sub[2]=6
    // old_sum[1] = 21, old_sum[2] = 11 -> new_sum[1]=10, new_sum[2]=21
    assert(par[1] == 2 && par[2] == 0);
    assert(sub[1] == 3 && sub[2] == 6);
    assert(sum[1] == 10 && sum[2] == 21);
    // 2's children now include 1,4,5? Actually 2 had {4,5}, now add 1 with size3. So 2 has three children.
    assert(childrenSet[2].size() == 3);
    assert(childrenSet[1].size() == 1); // child 3? Actually 1's original children were {2,3}, after rotation remove 2 and add 1 to 2, but 1 still has child 3? Wait rotation on 1: children of 1 were {2,3}, we select s=2 (heaviest). Remove 2 from 1's children, so 1's children become {3}. Then add 1 to 2's children. So yes.
    assert(childrenSet[1].count({2,3}) == 1);
    assert(par[1] == 2);

    return 0;
}
