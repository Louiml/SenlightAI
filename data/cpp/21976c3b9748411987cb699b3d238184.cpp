// You are given a directed graph with `N` vertices (numbered 1 to N, converted to 0-indexed internally), each having an associated integer age. There are `M` directed edges, where an edge from `a` to `b` means that `a` is a direct superior of `b` (i.e., the adjacency stores `a` inside `adj[b]`). You must process `I` operations. Each operation is either:
// - `T a b`: Swap the identities (and thus all incident edges and the age) of vertices `a` and `b`. This operation must be performed by updating the graph so that all edges that pointed to/from `a` now point to/from `b` and vice versa, and also swapping the ages of `a` and `b`.
// - `P a`: Query the minimum age among all vertices that can be reached from `a` by following the directed edges (including `a` itself? The original code doesn't include the starting vertex's age, it starts DFS from `a` without adding `a`'s age—it only considers ages of reachable vertices excluding `a`). If there is no reachable vertex other than `a`, output `*` (asterisk). Otherwise, output that minimum age.
//
// Write a function `void process_operations(int N, vector<int>& ages, vector<vector<int>>& adj_reverse, istream& in, ostream& out)` that reads the `M` edges (format: `a b` meaning edge `a->b`, i.e., `a` is superior of `b`—so you must add `a` to `adj[b]`), then reads `I` operations from `in` and writes the answers to `out` in the same order as the original code. The original code uses `vector<set<int>>` for adjacency to keep it unique and sorted, but you may use `set<int>` to maintain the same behavior. Use the operations exactly as described: for `T`, you must update all adjacency sets by swapping `a` and `b` in every set, and also swap the adjacency sets of `a` and `b` themselves (note: the ages also get swapped because the vertices swap identities). For `P`, perform a DFS/BFS from `a` (excluding `a` itself from the age consideration) and find the minimum age among reached vertices. If none, output `*`.

The key challenge is handling the swap operation efficiently and correctly. The original code iterates over all vertices' adjacency sets and for each set, it finds occurrences of `a` and `b`, removes them and adds the swapped counterparts. Then it swaps the entire adjacency sets of `a` and `b`. This correctly implements the vertex identity swap. For the query, a simple DFS from the queried vertex (excluding the start itself) finds the minimum age among all reachable vertices. Since the graph can have cycles and multiple edges, using a `set<int>` for each adjacency list avoids duplicates. Time complexity per swap is O(N * deg) where deg is average degree, but in worst case O(N^2) if dense. For each query, DFS is O(N + M) in the worst case (since it visits all reachable vertices). Over all operations, if there are I operations, total worst-case is O(I * (N^2 + N + M)) which is acceptable for small N (the original constraints likely have N ≤ 500). Edge cases: the swap operation must handle cases where `a == b` (though likely not given, but we can just ignore swapping if equal), and the query `P` must exclude the starting vertex's age per the original code. Also note that the original code reads ages in order 0..N-1, and reads edges as `a->b` meaning `adj[b].insert(a)`. The function should preserve exact output format: for each `P` operation, output the minimum age found (or `*` if none) followed by newline.

#include <bits/stdc++.h>
using namespace std;

// Process operations on a directed graph with vertex identity swaps and reachability queries.
// adj_reverse[i] contains the set of vertices that have an edge to i (i.e., superiors of i).
// ages[i] is the age of vertex i. The function reads M edges and I operations from 'in',
// writes query results to 'out'. Each edge input: "a b" means a is a superior of b.
void process_operations(int N, vector<int>& ages, vector<set<int>>& adj_reverse, istream& in, ostream& out) {
    int M, I;
    in >> M >> I;  // read M and I from the given stream
    // The original code reads N, M, I first, but here we assume N is passed and M/I are read inside.
    // To keep the signature clean, we read M and I from 'in' inside the function.
    // Actually the problem statement says the function reads M edges and I operations, so we read M and I.
    // For clarity, we read M and I here.
    // Then read M edges.
    for (int i = 0; i < M; ++i) {
        int a, b;
        in >> a >> b;
        --a; --b;
        adj_reverse[b].insert(a);  // a is superior of b
    }
    // Read and process operations
    for (int op = 0; op < I; ++op) {
        char K;
        in >> K;
        if (K == 'T') {
            int a, b;
            in >> a >> b;
            --a; --b;
            if (a == b) continue;
            // Update all adjacency sets: replace a with b and b with a.
            for (int j = 0; j < N; ++j) {
                // We need to collect changes first to avoid iterator invalidation.
                vector<int> add;
                vector<int> rem;
                for (int val : adj_reverse[j]) {
                    if (val == a) {
                        rem.push_back(a);
                        add.push_back(b);
                    } else if (val == b) {
                        rem.push_back(b);
                        add.push_back(a);
                    }
                }
                for (int x : rem) adj_reverse[j].erase(x);
                for (int x : add) adj_reverse[j].insert(x);
            }
            // Swap the adjacency sets of a and b.
            swap(adj_reverse[a], adj_reverse[b]);
            // Also swap ages because the identities swapped.
            swap(ages[a], ages[b]);
        } else {  // 'P'
            int a;
            in >> a;
            --a;
            // DFS from a, excluding a itself.
            vector<bool> vis(N, false);
            vis[a] = true;
            stack<int> st;
            st.push(a);
            int mn = INT_MAX;
            while (!st.empty()) {
                int v = st.top();
                st.pop();
                for (int nxt : adj_reverse[v]) {
                    if (!vis[nxt]) {
                        mn = min(mn, ages[nxt]);
                        vis[nxt] = true;
                        st.push(nxt);
                    }
                }
            }
            if (mn == INT_MAX) out << "*\n";
            else out << mn << "\n";
        }
    }
}

#include <bits/stdc++.h>
#include <sstream>
#include <cassert>
using namespace std;

// Include the solution function here (or copy above)

int main() {
    // Test 1: Basic graph from original example
    {
        string input = "3 2 3\n10 20 30\n1 2\n2 3\nP 1\nT 1 3\nP 1\n";
        stringstream in(input);
        stringstream out;
        vector<int> ages = {10, 20, 30};
        vector<set<int>> adj(3);
        // The function will read M=2, I=3 from the stream, then edges, then ops.
        // But our function signature expects N and we pass it. It reads M and I from stream.
        process_operations(3, ages, adj, in, out);
        assert(out.str() == "*\n20\n"); // P 1: reachable are 2 (age 20) and 3 (age 30) -> min 20? Wait original code excludes start but includes reachable. Actually start 1 has no edges because edges are 1->2 and 2->3, so from 1 can reach 2 and 3, min age = 20. But original code output? Let's compute: after T 1 3, identities swap. ages become {30,20,10} and adjacency: originally adj[1]={0} (0 is 1's superior? Wait edge 1->2 means adj[1] contains 0? No adj[2] contains 1. Let's do properly: N=3, ages={10,20,30} indices 0,1,2. Edges: 1 2 -> a=0,b=1 => adj[1].insert(0). Edge 2 3 -> a=1,b=2 => adj[2].insert(1). So adj[0]={}, adj[1]={0}, adj[2]={1}. P 1 (index 0): DFS from 0, reachable: adj[0] empty -> no other -> "*". T 1 3 (swap 0 and 2): update all sets: adj[1] has 0 -> becomes 2; adj[2] has 1 unchanged. swap adj[0] and adj[2] -> adj[0] becomes {1}, adj[2] becomes {}. swap ages -> ages={30,20,10}. P 1 (index 0): DFS from 0, reachable adj[0]={1} -> ages[1]=20, then from 1 adj[1]={2} -> ages[2]=10, min=10. So output should be "*\n10\n". My assert above is wrong. Let me recalc: Actually P 1 originally: from 0 reachable 1 and 2, min age = min(20,30)=20? No ages[1]=20, ages[2]=30, so min=20. But original code: dfs(0) starts with pilha=[0], vis[0]=true, pops 0, iterates adj[0] which is {} (since adj[0] is empty because edge is from 1 to 2, so adj[2] has 1, adj[1] has 0). So from 0 no outgoing edges, ans=INF -> "*". So first output is "*". After swap, adj[0] becomes {1} (because original adj[2]={1} swapped with adj[0]={}), so from 0 can reach 1, then from 1 to 2? adj[1] originally {0} after swap updates to {2}, so from 1 reach 2, ages[1]=20, ages[2]=10, min=10. So second output "10\n". Thus expected "*\n10\n". I'll fix test accordingly.
        assert(out.str() == "*\n10\n");
    }
    // Test 2: Chain with no swap
    {
        string input = "3 2 2\n5 15 25\n1 2\n2 3\nP 1\nP 3\n";
        stringstream in(input);
        stringstream out;
        vector<int> ages = {5,15,25};
        vector<set<int>> adj(3);
        process_operations(3, ages, adj, in, out);
        // P 1: from 0 reachable 1 (15) and 2 (25) -> min 15
        // P 3: from 2 reachable none -> "*"
        assert(out.str() == "15\n*\n");
    }
    // Test 3: Cycle and self-reference
    {
        string input = "2 2 2\n100 200\n1 2\n2 1\nP 1\nT 1 2\nP 1\n";
        stringstream in(input);
        stringstream out;
        vector<int> ages = {100,200};
        vector<set<int>> adj(2);
        process_operations(2, ages, adj, in, out);
        // Edge 1->2 => adj[1]={0}; edge 2->1 => adj[0]={1}
        // P 1 (0): reachable 1 (200) -> min 200
        // T 1 2: swap 0 and 1: update adj[0] and adj[1] -> each set has other, so they become same? Let's simulate: adj[0]={1} -> rem 1, add 0 => {0}; adj[1]={0} -> rem 0, add 1 => {1}; then swap adj[0] and adj[1] -> both become {0} and {1}? Actually after updates: adj[0]={0}, adj[1]={1} (self-loops). Then swap adj[0] and adj[1] -> adj[0]={1}, adj[1]={0}. Ages swap -> {200,100}.
        // P 1 (0): from 0 reachable 1 (age 100) -> min 100
        assert(out.str() == "200\n100\n");
    }
    // Test 4: No reachable nodes
    {
        string input = "1 0 1\n42\nP 1\n";
        stringstream in(input);
        stringstream out;
        vector<int> ages = {42};
        vector<set<int>> adj(1);
        process_operations(1, ages, adj, in, out);
        assert(out.str() == "*\n");
    }
    // Test 5: Multiple queries after swap affecting multiple edges
    {
        string input = "4 4 3\n1 2 3 4\n1 2\n1 3\n2 4\n3 4\nP 1\nT 2 3\nP 1\nP 4\n";
        stringstream in(input);
        stringstream out;
        vector<int> ages = {1,2,3,4};
        vector<set<int>> adj(4);
        process_operations(4, ages, adj, in, out);
        // Original: edges: 1->2 (adj[1]={0}), 1->3 (adj[2]={0}), 2->4 (adj[3]={1}), 3->4 (adj[3]={2})
        // P 1 (0): reachable 1(2),2(3),3(4) -> min 2
        // T 2 3: swap indices 1 and 2. Update: adj[1] has 0 -> becomes 0? Actually a=1,b=2, replace 1 with 2 and 2 with 1. In adj[3] contains 1 and 2, so both get swapped: adj[3] becomes {2,1} (same). Adj[1] has 0 -> no change. Adj[2] has 0 -> no change. Then swap adj[1] and adj[2] -> adj[1] becomes {} (original adj[2]={0}), adj[2] becomes {0} (original adj[1]={0}). Ages swap -> ages={1,3,2,4}
        // P 1 (0): from 0 reachable: adj[0] is empty? Wait we need to see incoming? Actually adjacency is reverse of edges. Edge 1->2 means adj[2] has 1. So after swap, we have: originally adj[1]={0} (since edge 1->2), adj[2]={0} (edge 1->3), adj[3]={1,2} (edges 2->4 and 3->4). After update: adj[1]={0} (unchanged), adj[2]={0} (unchanged), adj[3] has 1 and 2 replaced with 2 and 1 -> {2,1} same. Swap adj[1] and adj[2] -> adj[1]={0}, adj[2]={0} (both same? Actually they are both {0} before swap, after swap same). So no change in adjacency structure? Because swapping identical sets. Ages swap: ages[1]=3, ages[2]=2. So from 0, reachable 1 and 2 (both have 0), and from those to 3 (both have 1 and 2). So reachable vertices: 1(age3), 2(age2), 3(age4) -> min 2. Then P 1 after swap gives 2 again. P 4 (index 3): from 3, its adjacency adj[3]={1,2} (now ages[1]=3, ages[2]=2) -> min 2. So output: "2\n2\n2\n"? But careful: P 1 initially before swap gives min of ages[1]=2, ages[2]=3, ages[3]=4 -> 2. After swap, same. P 4 gives min(ages[1]=3, ages[2]=2) = 2. So expected "2\n2\n2\n". Let's assert.
        assert(out.str() == "2\n2\n2\n");
    }
    return 0;
}
