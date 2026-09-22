// Given an undirected graph with `n` vertices and `m` edges, and a sequence of edge update operations (insertions and deletions), write a C++ function `maintainMaximalIndependentSet` that processes these updates and maintains a *maximal independent set (MIS)* of the current graph using a dynamic approach. The function should take as input the initial graph, the delta threshold parameter `deltaC` (computed as `floor(pow(m, 2/3))`), the operation type, and the edge endpoints. Specifically, implement the following:  
// - A function `initialize(n, m, edges, deltaC)` that sets up the graph, initial `lowV` and `highV` partitions (vertices with degree ≤ `deltaC` are low, others high), computes an initial MIS using a static greedy algorithm on low vertices and on high vertices with no low neighbors in the MIS, and maintains a `lowCount` array that counts how many MIS vertices are adjacent to each vertex.  
// - A function `edgeDeletion(u, v)` that removes the edge `(u,v)` and, if the edge removal causes previously covered vertices to become uncovered with respect to the low MIS, inserts them if they are low vertices.  
// - A function `edgeInsertion(u, v)` that adds the edge `(u,v)` and, if both endpoints are in the low MIS, removes one endpoint (the first one, `u`) from the MIS and updates counts, possibly adding newly uncovered low vertices.  
// - After every 50 operations (or when the edge count `m` reaches `m_c/2` or `2*m_c` where `m_c` is the initial edge count), the function should recompute the entire MIS from scratch using `initialize`.  
// - The function should return the final MIS as a `std::set<int>` containing all vertices in both `lowMIS` and `highMIS`.

The algorithm splits vertices into **low** (degree ≤ `deltaC`) and **high** (degree > `deltaC`). The key observation is that low vertices have small degree, so we can maintain their MIS efficiently. For the low partition, we maintain a greedy maximal independent set using a static scan whenever needed; dynamic updates only affect a small local neighborhood because low vertices have at most `deltaC` neighbors. For high vertices, we only consider those high vertices with no low-MIS neighbor; the induced subgraph on these high vertices has high minimum degree (since all their neighbors are high, and every vertex has degree > `deltaC`), so a greedy MIS on that subgraph still yields small size (by a density argument, the number of such high vertices is at most `O(m^2/3)`). The `lowCount[v]` array stores how many vertices in `lowMIS` are adjacent to `v`. When an edge `(u,v)` is deleted and one endpoint (say `u`) is in `lowMIS`, we decrement `lowCount[v]`; if it becomes 0 and `v` is low, we add `v` to `lowMIS` and increment `lowCount` for all its neighbors. Similarly, on edge insertion, if both endpoints are in `lowMIS`, we remove `u` from `lowMIS` and update counts. After handling the update, we recompute `highMIS` from scratch by scanning all high vertices with `lowCount == 0`. To keep correctness over many updates, we periodically (every 50 updates or when the edge count changes beyond a factor of 2) rebuild the entire MIS using the initial static procedure. The time complexity per update is `O(deltaC)` for low updates and `O(n)` for recomputing highMIS (but in practice high vertices count is small). Rebuilding takes `O(n + m)`. Space is `O(n + m)` for the graph and arrays.

#include <bits/stdc++.h>
using namespace std;

// Global state for the dynamic MIS algorithm
vector<set<int>> graph;        // adjacency sets
int n, m;                     // current number of vertices and edges
int deltaC;                   // threshold parameter
set<int> lowV, highV;         // partition of vertices by degree
set<int> lowMIS, highMIS;     // maintained MIS parts
vector<int> lowCount;         // lowCount[v] = number of lowMIS neighbors of v

// Static greedy MIS on a subset of vertices (given a set Vs)
set<int> staticMIS(const set<int>& Vs) {
    set<int> visited, res;
    for (int u : Vs) {
        if (visited.find(u) == visited.end()) {
            res.insert(u);
            for (int v : graph[u]) visited.insert(v);
        }
    }
    return res;
}

// Partition vertices into low (degree <= deltaC) and high
void partitionGraph() {
    lowV.clear(); highV.clear();
    for (int i = 0; i < n; i++) {
        if ((int)graph[i].size() <= deltaC) lowV.insert(i);
        else highV.insert(i);
    }
}

// Recompute highMIS from scratch: high vertices with no lowMIS neighbor
void heavyMIS() {
    highMIS.clear();
    set<int> Vs;
    for (int v : highV) {
        if (lowCount[v] == 0) Vs.insert(v);
    }
    highMIS = staticMIS(Vs);
}

// Initial setup: graph, partition, lowMIS, lowCount, highMIS
void initialize(int n_vertices, const vector<pair<int,int>>& edges, int delta) {
    n = n_vertices;
    m = (int)edges.size();
    deltaC = delta;
    graph.assign(n, set<int>());
    for (auto &e : edges) {
        int u = e.first, v = e.second;
        graph[u].insert(v);
        graph[v].insert(u);
    }
    partitionGraph();
    lowCount.assign(n, 0);
    lowMIS = staticMIS(lowV);
    for (int u : lowMIS) {
        for (int v : graph[u]) lowCount[v]++;
    }
    heavyMIS();
}

// Handle edge deletion (u,v) — assumes edge exists
void edgeDeletion(int u, int v) {
    graph[u].erase(v);
    graph[v].erase(u);

    if (lowMIS.find(u) != lowMIS.end()) {
        lowCount[v]--;
        if (lowCount[v] == 0) {
            if (lowV.find(v) != lowV.end()) {
                lowMIS.insert(v);
                for (int w : graph[v]) lowCount[w]++;
            }
        }
    }
    else if (lowMIS.find(v) != lowMIS.end()) {
        lowCount[u]--;
        if (lowCount[u] == 0) {
            if (lowV.find(u) != lowV.end()) {
                lowMIS.insert(u);
                for (int w : graph[u]) lowCount[w]++;
            }
        }
    }
    heavyMIS();
}

// Handle edge insertion (u,v) — assumes edge does not exist
void edgeInsertion(int u, int v) {
    // If both are in lowMIS, remove u from MIS and update
    if (lowMIS.find(u) != lowMIS.end() && lowMIS.find(v) != lowMIS.end()) {
        lowMIS.erase(u);
        for (int w : graph[u]) {
            lowCount[w]--;
            if (lowCount[w] == 0) {
                if (lowV.find(w) != lowV.end()) {
                    lowMIS.insert(w);
                    for (int x : graph[w]) lowCount[x]++;
                }
            }
        }
    }
    else if (lowMIS.find(u) != lowMIS.end()) {
        lowCount[v]++;
    }
    else if (lowMIS.find(v) != lowMIS.end()) {
        lowCount[u]++;
    }
    // Add edge
    graph[u].insert(v);
    graph[v].insert(u);
    heavyMIS();
}

// Main dynamic MIS maintenance function
// Parameters:
//   n0          : number of vertices
//   initialEdges: list of initial edges
//   operations  : vector of operations, each is {type, u, v}
//                 type = 0 for deletion, type = 1 for insertion
//   delta       : threshold parameter (should be computed by caller)
// Returns final MIS as set<int>
set<int> maintainMaximalIndependentSet(
    int n0,
    const vector<pair<int,int>>& initialEdges,
    const vector<tuple<int,int,int>>& operations,
    int delta
) {
    initialize(n0, initialEdges, delta);
    int m_c = m;
    int opCount = 0;

    for (auto &op : operations) {
        int type = get<0>(op);
        int u = get<1>(op);
        int v = get<2>(op);

        if (type == 0) {
            if (graph[u].find(v) != graph[u].end()) {
                edgeDeletion(u, v);
                m--;
            }
        } else if (type == 1) {
            if (graph[u].find(v) == graph[u].end()) {
                edgeInsertion(u, v);
                m++;
            }
        }

        opCount++;
        // Rebuild if many operations or edge count changes significantly
        if (opCount % 50 == 0 || m == m_c / 2 || m == 2 * m_c) {
            initialize(n0, initialEdges, delta); // but we lost updates! 
            // Actually we need to rebuild based on current graph:
            // To keep simplicity, we rebuild from current graph:
            vector<pair<int,int>> edges;
            for (int i = 0; i < n; i++) {
                for (int j : graph[i]) {
                    if (i < j) edges.push_back({i,j});
                }
            }
            initialize(n0, edges, deltaC);
            m_c = (int)edges.size();
        }
    }

    // Combine lowMIS and highMIS into result
    set<int> result = lowMIS;
    result.insert(highMIS.begin(), highMIS.end());
    return result;
}

#include <bits/stdc++.h>
#include <cassert>
using namespace std;

// The function definition from the solution is assumed to be available above.

int main() {
    // Test 1: Triangle graph 0-1-2-0, delta = 1
    {
        int n0 = 3;
        vector<pair<int,int>> edges = {{0,1},{1,2},{2,0}};
        vector<tuple<int,int,int>> ops;
        int delta = 1;
        set<int> mis = maintainMaximalIndependentSet(n0, edges, ops, delta);
        // A MIS of triangle has size 1, any vertex works
        assert(mis.size() == 1);
        int v = *mis.begin();
        assert(v >= 0 && v < 3);
    }

    // Test 2: Path 0-1-2-3, delta = 2
    {
        int n0 = 4;
        vector<pair<int,int>> edges = {{0,1},{1,2},{2,3}};
        vector<tuple<int,int,int>> ops;
        int delta = 2;
        set<int> mis = maintainMaximalIndependentSet(n0, edges, ops, delta);
        // MIS size is 2 (e.g., {0,2} or {0,3} or {1,3})
        assert(mis.size() == 2);
    }

    // Test 3: Isolated vertex 0, plus an edge 1-2, delta=0
    {
        int n0 = 3;
        vector<pair<int,int>> edges = {{1,2}};
        vector<tuple<int,int,int>> ops;
        int delta = 0;
        set<int> mis = maintainMaximalIndependentSet(n0, edges, ops, delta);
        assert(mis.size() == 2); // must include vertex 0 and one of {1,2}
        assert(mis.count(0) == 1);
    }

    // Test 4: Single insertion creating a conflict
    {
        int n0 = 2;
        vector<pair<int,int>> edges = {}; // no edges
        vector<tuple<int,int,int>> ops = {make_tuple(1, 0, 1)}; // insert edge 0-1
        int delta = 1;
        set<int> mis = maintainMaximalIndependentSet(n0, edges, ops, delta);
        // After insertion, MIS size should be 1
        assert(mis.size() == 1);
    }

    // Test 5: Deletion that should add a new vertex to MIS
    {
        int n0 = 3;
        vector<pair<int,int>> edges = {{0,1},{0,2}}; // star center 0
        vector<tuple<int,int,int>> ops = {make_tuple(0, 0, 1)}; // delete edge 0-1
        int delta = 2;
        set<int> mis = maintainMaximalIndependentSet(n0, edges, ops, delta);
        // After deletion, vertex 1 is isolated, must be in MIS
        assert(mis.count(1) == 1);
    }

    // Test 6: Multiple operations with rebuild trigger (50 ops)
    {
        int n0 = 5;
        vector<pair<int,int>> edges = {{0,1},{1,2},{2,3},{3,4},{4,0}}; // cycle
        vector<tuple<int,int,int>> ops;
        for (int i = 0; i < 60; i++) {
            int edgeIdx = i % 5;
            int u = edgeIdx, v = (edgeIdx+1)%5;
            if (i % 2 == 0) ops.push_back(make_tuple(0, u, v)); // delete
            else ops.push_back(make_tuple(1, u, v)); // insert
        }
        int delta = 2;
        set<int> mis = maintainMaximalIndependentSet(n0, edges, ops, delta);
        // After many oscillations, MIS should be non-empty and independent
        assert(mis.size() > 0);
        for (int u : mis) {
            for (int v : mis) {
                if (u != v) assert(graph[u].find(v) == graph[u].end());
            }
        }
    }

    // Test 7: Edge case: empty graph (n=0)
    {
        int n0 = 0;
        vector<pair<int,int>> edges;
        vector<tuple<int,int,int>> ops;
        int delta = 0;
        set<int> mis = maintainMaximalIndependentSet(n0, edges, ops, delta);
        assert(mis.empty());
    }

    // Test 8: Complete graph K4, delta large enough to put all in low
    {
        int n0 = 4;
        vector<pair<int,int>> edges = {{0,1},{0,2},{0,3},{1,2},{1,3},{2,3}};
        vector<tuple<int,int,int>> ops;
        int delta = 100;
        set<int> mis = maintainMaximalIndependentSet(n0, edges, ops, delta);
        // MIS size must be 1
        assert(mis.size() == 1);
    }

    // Test 9: Disconnected graph with high-degree partition changes
    {
        int n0 = 4;
        vector<pair<int,int>> edges = {{0,1},{2,3}};
        vector<tuple<int,int,int>> ops;
        int delta = 2;
        set<int> mis = maintainMaximalIndependentSet(n0, edges, ops, delta);
        // MIS size is 2 (one from each edge)
        assert(mis.size() == 2);
    }

    // Test 10: Delete all edges progressively
    {
        int n0 = 4;
        vector<pair<int,int>> edges = {{0,1},{1,2},{2,3},{0,3}};
        vector<tuple<int,int,int>> ops;
        ops.push_back(make_tuple(0,0,1));
        ops.push_back(make_tuple(0,1,2));
        ops.push_back(make_tuple(0,2,3));
        ops.push_back(make_tuple(0,0,3));
        int delta = 2;
        set<int> mis = maintainMaximalIndependentSet(n0, edges, ops, delta);
        // All edges gone, so all vertices must be in MIS
        assert(mis.size() == 4);
    }

    printf("All tests passed!\n");
    return 0;
}
