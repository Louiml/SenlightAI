/*
Write a C++ function `bool hasDiamond(const std::vector<std::vector<int>>& adjacencyList)` that determines whether a directed graph contains a diamond pattern: two distinct paths from the same starting node to the same ending node where the paths diverge and then converge again at the end. Input is a graph with nodes numbered 0 to N-1, and each node's list contains the outgoing neighbors (indices). The function returns `true` if there exists a node (call it source) such that there are two different paths from source to some other node (call it target) that share no intermediate nodes except source and target. Equivalently, it means the graph has a node with two distinct children that eventually reach a common descendant, where that common descendant is reachable from both children via paths that do not cross each other except at the end. The graph is given as adjacency lists, and may contain cycles. The function must handle up to 1000 nodes and 10000 edges efficiently.
*/

#include <vector>
#include <stack>

// Returns true if the directed graph contains a diamond pattern:
// there exists a node s such that two distinct paths from s reach the same node.
// The graph is given as an adjacency list, nodes are 0..n-1.
bool hasDiamond(const std::vector<std::vector<int>>& adj) {
    const int n = static_cast<int>(adj.size());
    
    // Try every node as a potential source of the diamond.
    for (int u = 0; u < n; ++u) {
        const int deg = static_cast<int>(adj[u].size());
        if (deg < 2) continue;  // Need at least two outgoing edges to diverge

        // branchId[v] = the label of the child of u through which v was first reached.
        // -1 means v has not been reached in this DFS from u.
        std::vector<int> branchId(n, -1);
        std::stack<std::pair<int,int>> st;  // (node, branchLabel)

        // Initialize with each neighbor of u as a distinct branch.
        for (int i = 0; i < deg; ++i) {
            int v = adj[u][i];
            // If v already has a different label, two edges from u reach v directly.
            if (branchId[v] != -1 && branchId[v] != i) return true;
            if (branchId[v] == -1) {
                branchId[v] = i;
                st.push({v, i});
            }
        }

        // Explore the graph reachable from u through each distinct child.
        while (!st.empty()) {
            auto [x, lab] = st.top();
            st.pop();
            for (int y : adj[x]) {
                if (branchId[y] == -1) {
                    branchId[y] = lab;
                    st.push({y, lab});
                } else if (branchId[y] != lab) {
                    // y is reachable from two different children of u.
                    return true;
                }
                // If branchId[y] == lab, same branch revisits y (cycle) -> no diamond.
            }
        }
    }
    return false;
}

#include <cassert>
#include <vector>

// Function under test is declared above (or included from the solution).
// We'll repeat the declaration here for clarity.
bool hasDiamond(const std::vector<std::vector<int>>& adj);

int main() {
    // No edges -> no diamond.
    assert(hasDiamond({{}}) == false);
    assert(hasDiamond({{}, {}, {}}) == false);

    // Simple path 0->1->2 -> no diamond.
    assert(hasDiamond({{1}, {2}, {}}) == false);

    // Diamond: 0 points to 1 and 2; both 1 and 2 point to 3.
    assert(hasDiamond({{1,2}, {3}, {3}, {}}) == true);

    // Self-loop only -> no diamond (single path).
    assert(hasDiamond({{0}}) == false);

    // Two parallel edges from 0 to 1 (duplicate entries) -> diamond.
    assert(hasDiamond({{1,1}, {}}) == true);

    // A cycle 0<->1 but no diamond.
    // 0->1, 1->0
    assert(hasDiamond({{1}, {0}}) == false);

    // Diamond with source not at beginning: 2 points to 3 and 4, both to 5.
    std::vector<std::vector<int>> g = {{}, {}, {3,4}, {5}, {5}, {}};
    assert(hasDiamond(g) == true);

    // Graph with multiple levels but no diamond.
    // 0->1, 0->2, 1->3, 2->4, 3 and 4 have no outgoing.
    assert(hasDiamond({{1,2}, {3}, {4}, {}, {}}) == false);

    // Graph where two different paths converge through a shared intermediate.
    // 0->1, 0->2, 1->3, 2->3, 3->4  -> diamond at 3.
    assert(hasDiamond({{1,2}, {3}, {3}, {4}, {}}) == true);
}

// The problem is to detect if there exists any pair of distinct paths from a common source to a common target where the paths are internally disjoint (only share source and target). A simpler sufficient and necessary condition is: there exists a node `u` with two distinct outgoing neighbors `a` and `b` such that `a` and `b` have a common descendant (including possibly themselves) reachable from both. We can detect this by performing a DFS from each node that has out-degree ≥ 2. For each such node, we perform a DFS from each of its outgoing neighbors independently, marking reachable nodes in a set. If any node is reachable from two different neighbors of the same source, then a diamond exists. However, to avoid false positives due to shared descendants that are actually the same path? Actually if two different neighbors of the same source both reach a common node, then there are two paths from source to that common node: source->neighbor1->...->common and source->neighbor2->...->common. These paths are distinct because they diverge at source. They may share intermediate nodes later, but that is still a diamond (the definition in the code snippet allows shared intermediate nodes? The original code snippet marks "Yes" if during DFS from a source, it encounters a node already visited from the same source, regardless of whether the paths share later nodes. Actually the snippet uses a global visited set per source and does a stack DFS; when it pops a node and iterates its outgoing edges, if a neighbor is already in visited, it claims a diamond. This detects if two different paths from the same source reach the same node, even if they share some suffix. That is exactly the standard diamond detection: there exists a node reachable from the source via two distinct paths. Because if two paths reach the same node, then that node is a convergence. The snippet runs DFS from each root (nodes with no parents) and uses a global visited set for that root. But it only starts from nodes with zero in-degree, which is not correct for graphs with cycles or where the source is not a root. The problem statement likely intends to detect a "diamond" anywhere in the graph, not just from roots. So the solution should check every possible source node.
//
// Algorithm: For each node `s` in the graph, we want to know if there exist two distinct paths from `s` to some node `t`. Instead of checking all sources, we can do: For each node `u`, consider its outgoing neighbors. If there are at least two distinct outgoing neighbors `v1` and `v2`, perform a DFS from each `v` (without revisiting `u`), and collect reachable nodes. If any node is reachable from both `v1` and `v2` (including `v1` and `v2` themselves if they are equal? No, they are distinct neighbors, so they are different nodes). If the intersection of reachable sets is non-empty, then there is a diamond with source `u`. This works because if there are two paths from `u` to `t`, then the first edge on each path must go to two different neighbors (or the same neighbor, but then the paths would share the first edge and could still diverge later, but the snippet's detection would catch that later; however, if both paths start with the same edge, then they actually diverge later, so there exists a node on the path that has two different outgoing edges to reach the same target. So it suffices to check for each node `u` that has two different outgoing neighbors that can reach a common node. That is the condition. Complexity: For each node `u`, we may perform up to degree(u) DFS traversals, each O(V+E). In worst case, O(VE) which is too high. But we can optimize: For each `u`, do a single DFS from `u`, but maintain for each visited node the number of distinct "branches" from `u` that can reach it. Alternatively, we can use the following: For each node `u`, we perform a DFS from `u` and for each node we count how many distinct children of `u` can reach it. But that is complex.
//
// Simpler approach: For each source node `s`, we run a DFS that tracks the "parent edge" from `s`. But we need to know if there are two different paths. Actually the snippet does: for each root node (in-degree 0), start a stack DFS with a global visited set. When we pop a node `v`, for each neighbor `w`, if `w` is already in visited, then there is a diamond. This is exactly the condition that there exist two distinct paths from the source to `w`. Because the first time `w` is visited, it is added to visited; the second time we attempt to visit it via another path, we detect it. This works regardless of whether `w` is reachable from the source via two different first-edge choices. So we can simply run this for every node as potential source. However, doing a full DFS from every node would be O(V(V+E)). With V up to 1000 and E up to 10000, that's 10^7 which is fine. So we can implement: For each node `s` from 0 to N-1, run a DFS (using stack) with a local boolean array `visited` of size N initialized to false. Push `s` onto stack. While stack not empty, pop `v`. For each neighbor `w` of `v`: if `visited[w]` is true, return true (diamond found). Otherwise, set `visited[w]=true` and push `w`. Note: we must not mark `s` as visited at the start? Actually if we mark `s` as visited, then when we process a neighbor that points back to `s`, we would incorrectly detect a cycle as a diamond. But a cycle from `s` back to `s` is a self-cycle but not a diamond? The snippet's approach marks visited only when we encounter a node during the stack exploration, not the source itself. So we should not pre-mark `s` as visited. That way, if there is an edge from some node back to `s`, then `s` is not in visited, so it won't falsely trigger. But what about a path from `s` to itself via a cycle? That would give two distinct paths from `s` to `s`? Actually a self-loop is one path, but a cycle that returns to `s` gives two paths? For example, s->a->s and s->b->s? That would be a diamond with target `s`. But the snippet would detect it if `s` is not in visited initially. Let's test: start DFS from s, push s. Pop s, for neighbor a, visited[a]=true, push a. Pop a, neighbor s, is s in visited? No, because we never inserted s. So we insert s into visited and push s. Now we pop s, but note that s is now in visited. Then we process its neighbors again. If there is another neighbor b, we might eventually reach s again, and then s is already visited, so we detect diamond. So it works.
//
// But there is a subtlety: If the graph has a cycle that does not include s, but includes a node that is reachable from s via two different paths, the DFS will detect it when it tries to visit that node a second time. The algorithm is correct: starting from a source s, if during DFS we ever attempt to visit a node that is already in the visited set, then that node is reachable from s via at least two distinct paths (because the first time we added it to visited, we came via one path; the second time we come via a different path). Since we only add nodes when they are pushed onto the stack, and we never remove them, this detects any node with multiple reachable paths.
//
// Edge case: If there is a self-loop s->s, then starting from s, we pop s, neighbor s is not visited yet (since we didn't insert s initially), so we insert s and push s. Then later we pop s again, now s is visited, and we process neighbors again, including s again. At that point s is already visited, so we detect diamond. That is correct: there are two distinct paths from s to s: the empty path? Actually a self-loop gives path s->s (one edge) and also the trivial path of length 0? Usually we consider paths of length >=1. Two distinct paths: s->s (via the edge) and s->s (via length 0) – but length 0 is typically not considered. However, there is only one edge, so only one distinct path of length >=1. But the DFS will detect a diamond because when we pop s the first time, we process its neighbor s and push it, and then later we pop s again, and process its neighbor s again, which is already visited, triggering a diamond. That is a false positive. To avoid that, we should not process the same source node twice? Actually the issue is that we push s as a neighbor, so later s is visited, and then when we process s's outgoing edges again, we see s already visited. But that's just the same edge being traversed twice. So we need to be careful: the visited set should track nodes that have been reached via some path, but we should not consider revisiting a node as a diamond if the node is the source itself? Actually the source itself can be revisited via a cycle, but that would indeed give two distinct paths: one is the direct cycle back, and another would require another route. If there is only a self-loop, there is only one path s->s. So we should not count revisiting the source as a diamond unless there are at least two distinct paths to the source. The algorithm as described would falsely trigger on a self-loop. To fix, we can either not push the source as a neighbor? That doesn't work because we need to explore paths. Better: we can maintain visited as a set of nodes that have been encountered as *intermediate* nodes, but the source should be excluded from the check. In the snippet, they start with an empty visited set and do not insert the initial node `i` (the source). So when they pop `v` and look at neighbors, they check if neighbor is in visited. The source is never inserted, so accessing the source again via a cycle would insert it, and then later if we try to access it again, we'd detect a diamond. That is correct if there are two distinct paths to the source. For a self-loop, starting from s, we pop s, neighbor s is not in visited, so we insert s and push s. Then we pop s again, neighbor s is in visited, so we detect diamond. But is that a true diamond? Path1: s->s (via the self-loop). Path2: also s->s? No, there is only one edge. So it's a false positive. The reason is that the same edge is traversed twice because we are revisiting s after we already pushed it from itself. Actually the detection should be: when we are at node v, and we have an edge to w, and w is already in visited, that means w was reached earlier by a different path. In the self-loop case, w=s, and s was reached earlier via the same edge (v=s, w=s). So it's the same path, not different. The issue is that we are using the visited set that includes nodes that have been reached, but we are also processing nodes multiple times if they have multiple parents. To correctly detect, we need to ensure that we do not push the same node twice from the same parent? Actually the standard approach is: when you pop a node v, for each neighbor w, if w has not been visited yet, mark it and push. If w has been visited already, then there is a diamond only if w is not the ancestor of v along the current DFS path? But the snippet's approach is simpler: it uses a global visited set per source, and it marks a node as visited the first time it is pushed, and if we ever see a neighbor that is already visited, we claim diamond. This works if we never push the source initially. But it fails on self-loop as described. Let's test with self-loop: source s. visited = empty. stack push s. pop s, neighbor s, visited is empty, so insert s, push s. stack now [s]. pop s, neighbor s, visited contains s, so claim diamond. That is a false positive. So the snippet is buggy for self-loops? Actually maybe they consider self-loop as a diamond? Usually a diamond requires two distinct paths. A self-loop gives only one path. So the snippet would mistakenly say Yes for a graph with just a self-loop. But the problem likely expects No. So we need a better algorithm.
//
// Alternative correct approach: For each source `s`, run a DFS but track the "first parent" or "branch" that leads to each node. We want to know if any node is reachable from `s` via two distinct paths. This can be done by performing a modified BFS/DFS where we store for each node a set of "entry edges" from `s`? Actually simpler: For each node `u` with out-degree >= 2, we can run a DFS from each distinct child of `u` and see if any node is reachable from two different children. This is more robust and avoids false positives from self-loops. Because if there is a diamond from `s`, then at some point the two paths diverge, so there exists a node `u` on the paths (possibly the source) where the two paths take different outgoing edges. Specifically, consider the source of the diamond. If the two paths share the same first edge, then they diverge later, so there is an intermediate node with two different outgoing edges leading to the same target. So it suffices to check for every node `u` and every pair of distinct outgoing neighbors `a` and `b`, whether the set of nodes reachable from `a` (excluding `u`) intersects the set of nodes reachable from `b` (excluding `u`). This is O(V^2 * (V+E)) worst case, too high. But we can do better: For each node `u`, if it has degree `d`, we can compute reachable sets from each neighbor using DFS and check if any node appears in two sets. With V up to 1000 and E up to 10000, and total out-degree sum is E, so the sum of degrees is at most 10000. For each node `u`, we do at most `deg(u)` DFS traversals, but each DFS is O(V+E) worst case. So total O(E*(V+E)) = 10^4 * 11000 = 1.1e8 which might be okay in C++ with careful implementation. But we can optimize: For each node `u`, we can do a single DFS from `u` but track for each visited node how many distinct children of `u` can reach it. That is: For each node `u`, start a DFS that passes a "branch id" representing the child of `u` that first led to the node. We can maintain an array `fromBranch` of size N initialized to -1. We push `u` with branch id = -1 (since u itself is not a child). Then for each neighbor of a popped node, if the neighbor is not visited yet, we set its branch id to the branch id of the current node (except when the current node is `u`, then we assign a unique branch id for each neighbor? Actually we need to distinguish between different children of `u`. So we can do: For each neighbor `v` of `u`, we start a DFS from `v` with branch label `v` (or an index). But that would be multiple DFS. Instead, we can do a single DFS from `u` but maintain a "color" for each node indicating which child of `u` it was first reached from. We can do this: Initialize a map from node to branch id. Start with a queue/stack containing `u` with branch id = -1. Then when we pop node `x` with branch id `b`, for each neighbor `y`, if `y` is not visited, set visited[y] = true and assign branch id = b (or if x is u, we assign a new unique id for each neighbor). But if `y` is already visited and its branch id is different from the current branch id `b`, then we found two different branches reaching the same node, so diamond. However, we need to ensure that we don't false positive on the case where the same branch revisits a node (like a cycle) because then branch id is the same, and we don't count it. For self-loop from u to itself: start with u branch -1. Process neighbor u? That is a self-loop. But we need to treat: when we pop u, for each neighbor v, if v is u itself, and u is not visited (we haven't inserted u yet), then we insert u with branch id = new unique id (say 0). Then later if we pop u again with branch id 0, and its neighbor u is already visited with branch id 0, same branch, so no diamond. That avoids false positive. For a multi-edge? If there are two parallel edges from u to v (not distinct nodes), that is still one neighbor v? The input is adjacency list, so multiple edges to same node are not possible as separate neighbors? Usually they'd be duplicates, but we can ignore.
//
// So algorithm: For each node `u`:
// - Create an array `branchId` of size N initialized to -1 (meaning not visited).
// - Create a queue (or stack) of pairs (node, branchLabel) where branchLabel is an integer.
// - For each distinct neighbor `v` of `u`, we will start a DFS from `v` with a unique branch label. But we can combine: Start by pushing all neighbors of `u` with their own unique labels (e.g., the neighbor index itself as label, but two different neighbors may have same index? No, each neighbor is distinct node). So for each neighbor `v` of `u`, if `v` is not already visited (i.e., branchId[v] != -1) and it's from a different branch? Actually we need to process each neighbor separately. If we push all neighbors at once, the order doesn't matter, but we need to assign a unique label per neighbor. We can assign the label as an integer incrementing from 0 for each neighbor of `u`. Then for each neighbor, we set branchId[v] = label if not set, else if already set with a different label, then we have two different children of `u` reaching v (actually v is a direct child of u, but if there are two edges from u to v? That would be duplicated edges, but we assume no duplicates). So ignore that.
//
// Then we process a stack: for each neighbor v of u, we push (v, label). We'll set branchId[v] = label. Then while stack not empty, pop (x, lab). For each neighbor y of x (note: y is any node, could be u or others):
//     - If y == u, skip? Actually u is the source, and if we encounter u from a non-trivial path, that means there's a cycle back to u. That would be a diamond if there are two distinct paths from u to u. So we should treat u as a normal node. But we never set branchId[u] initially (it's -1). So when we see y == u, branchId[u] is -1, we would set it to lab and push (u, lab). That would allow cycles. But then later if we see u again from another branch, we'd detect. So it's fine.
//     - If branchId[y] == -1, set branchId[y] = lab and push (y, lab).
//     - Else if branchId[y] != lab, then we have found a node reachable from two different children of u, so return true.
//     - Else (branchId[y] == lab) means revisiting within same branch, which is fine (cycle), no diamond.
//
// But wait: what if two different branches reach the same node through a common intermediate that is not a child? That will be detected because branchId[y] will be different from lab. That is correct.
//
// Edge case: If u has only one neighbor, then no diamond can start at u because there is only one outgoing edge, so any path from u must start with that edge, so there can't be two distinct paths from u to somewhere unless they diverge later at a node that has two outgoing edges. But if they diverge later, then that later node `w` will be checked as a source when we consider `w` itself. So we only need to check nodes with out-degree >= 2. But the algorithm above works for any degree; if degree is 0 or 1, the loop over neighbors is trivial, and no diamond will be detected because there's no way to have two different labels.
//
// Complexity: For each node `u`, we run a BFS/DFS over the whole graph with a branchId array. That gives O(V+E) per node, total O(V(V+E)) = up to 1000*11000 = 11e6, which is fine. The memory per iteration is O(V).
//
// Thus, solution: Iterate each node `u`. For each `u`, if out-degree < 2, skip (optional). Initialize branchId array with -1. For each neighbor `v` (with index `i` from 0 to deg-1) of `u`, assign label = i+1 (or just i). If branchId[v] == -1, set it to label and push (v, label) to stack; else if branchId[v] != label, return true (since two different neighbors directly point to same v? That would be duplicate edges, but we treat as diamond). Actually if there are two parallel edges from u to v, that would be two distinct edges but same neighbor, but the adjacency list would have duplicate entries. We can treat as diamond? The original problem likely has distinct nodes, no duplicates. So we can ignore that case, or we should deduplicate neighbors. To be safe, we can sort or use a set to get unique neighbors. But we can just handle: if we see a neighbor v already visited with a different label, then return true (because two edges from u to v, which is a diamond Ni? Actually two parallel edges from u to v give two distinct paths of length 1 from u to v, so it's a diamond). So return true.
//
// After pushing all neighbors, run a stack DFS as described. If at any point we detect a node with branchId different from the current label, return true.
//
// At the end, return false.
//
// We can also optimize by breaking early.
//
// Now the solution function signature: `bool hasDiamond(const std::vector<std::vector<int>>& adj)`. The graph is directed.
//
// We'll implement with a vector of vector<int>. In the test, we'll provide examples.
//
// Time complexity: O(V*(V+E)) in worst case, but with V=1000 it's fine. Space: O(V) per call.
//
// Now write the solution code.
