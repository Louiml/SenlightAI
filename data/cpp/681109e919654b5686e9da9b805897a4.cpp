// Write a C++ function that takes an undirected graph represented as an adjacency list, a list of special nodes (called "important" nodes), and performs a modified BFS from each special node that has degree at least 2. For each such starting node, compute the average of the shortest distances from that node to all *other* important nodes (excluding itself). Return the starting node (by its 1-based index) that yields the smallest average; if multiple nodes tie, return the one with the smallest index. If no important node has degree ≥ 2, return -1. The graph has `n` nodes labeled 1..n, and the input provides a list of edges. The function signature: `int findBestImportantNode(int n, const std::vector<std::pair<int,int>>& edges, const std::vector<int>& importantNodes)`. Note that important nodes may repeat in the list, but each unique node is considered once. Distances are measured in number of edges.

// The problem requires performing BFS from each candidate important node. A candidate is an important node that appears in at least two edges (i.e., has degree ≥ 2 in the graph). For each candidate, run BFS to compute shortest distances to all nodes. Then, among the set of unique important nodes (excluding the source itself), compute the average of their distances. The source is considered only if it has at least one other important node reachable; if not, the candidate is skipped (or average undefined). Keep track of the minimum average and the smallest node index achieving it. Edge cases: isolated important nodes (degree 0) are ignored. Since BFS is performed per candidate, the algorithm visits all nodes for each candidate, giving O(k * (V+E)) time where k is the number of candidates. Space is O(V) for BFS visited arrays. Graph adjacency list is built once.

#include <vector>
#include <queue>
#include <limits>
#include <unordered_set>
#include <algorithm>

// Given an undirected graph with n nodes (1-indexed), a list of important nodes,
// and edges, return the important node with degree >= 2 that minimizes the average
// shortest distance to all other important nodes. Ties break by smallest index.
// If no such node qualifies, return -1.
int findBestImportantNode(int n, const std::vector<std::pair<int,int>>& edges,
                          const std::vector<int>& importantNodes) {
    // Build adjacency list (0-indexed internally)
    std::vector<std::vector<int>> adj(n);
    for (const auto& e : edges) {
        int u = e.first - 1;
        int v = e.second - 1;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // Unique important nodes and their degree count from edges
    std::unordered_set<int> importantSet;
    std::vector<int> degree(n, 0);
    for (int imp : importantNodes) {
        if (imp >= 1 && imp <= n) {
            importantSet.insert(imp - 1);  // convert to 0-index
        }
    }
    // Compute degree from edges (count each edge once)
    for (const auto& e : edges) {
        degree[e.first - 1]++;
        degree[e.second - 1]++;
    }

    const double INF = std::numeric_limits<double>::infinity();
    double bestAvg = INF;
    int bestNode = -1;

    // Consider each unique important node that has degree >= 2
    for (int source : importantSet) {
        if (degree[source] < 2) continue;

        // BFS from source
        std::vector<int> dist(n, -1);
        std::queue<int> q;
        dist[source] = 0;
        q.push(source);
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (int v : adj[u]) {
                if (dist[v] == -1) {
                    dist[v] = dist[u] + 1;
                    q.push(v);
                }
            }
        }

        // Collect distances to all other important nodes
        double sum = 0.0;
        int count = 0;
        for (int imp : importantSet) {
            if (imp == source) continue;
            if (dist[imp] != -1) {
                sum += dist[imp];
                count++;
            }
        }
        if (count == 0) continue;  // no other important nodes reachable

        double avg = sum / count;
        // Update best if smaller, or equal but smaller index (source is 0-indexed, output 1-indexed)
        if (avg < bestAvg || (avg == bestAvg && (bestNode == -1 || source < bestNode))) {
            bestAvg = avg;
            bestNode = source;
        }
    }

    return bestNode == -1 ? -1 : bestNode + 1;  // convert to 1-indexed
}

#include <cassert>
#include <vector>
#include <utility>

int main() {
    // Test 1: Simple path with important nodes at ends and middle
    // Graph: 1-2-3, important nodes {1,2,3}, degrees: 1:1, 2:2, 3:1
    // Candidate: node 2 only. Distances: to 1 =1, to 3 =1, avg=1.0
    {
        std::vector<std::pair<int,int>> edges = {{1,2},{2,3}};
        std::vector<int> important = {1,2,3};
        assert(findBestImportantNode(3, edges, important) == 2);
    }

    // Test 2: No candidate (all important nodes degree < 2)
    {
        std::vector<std::pair<int,int>> edges = {{1,2},{2,3}};
        std::vector<int> important = {1,3};
        assert(findBestImportantNode(3, edges, important) == -1);
    }

    // Test 3: Two candidates, tie broken by smallest index
    // Graph: square 1-2-3-4-1, important {1,3}. Degrees: 1:2,3:2
    // Node 1 distances: to 3 =2, avg=2; Node 3 distances: to 1=2, avg=2. Tie -> smallest index 1
    {
        std::vector<std::pair<int,int>> edges = {{1,2},{2,3},{3,4},{4,1}};
        std::vector<int> important = {1,3};
        assert(findBestImportantNode(4, edges, important) == 1);
    }

    // Test 4: Disconnected graph, one candidate with no other important reachable -> -1
    {
        std::vector<std::pair<int,int>> edges = {{1,2},{3,4}};
        std::vector<int> important = {1,3};
        // Node 1 degree=1, node3 degree=1 => no candidates
        assert(findBestImportantNode(4, edges, important) == -1);
    }

    // Test 5: Complex with multiple important nodes
    // Graph: star with center 1 connected to 2,3,4; edge 2-3.
    // important {1,2,3,4}. degrees: 1:3,2:2,3:2,4:1
    // Candidates: 1,2,3
    // Node1 distances: to2=1,to3=1,to4=1 avg=1
    // Node2 distances: to1=1,to3=1,to4=2 avg=4/3≈1.33
    // Node3 distances: to1=1,to2=1,to4=2 avg=4/3
    // Best is node1 with avg=1
    {
        std::vector<std::pair<int,int>> edges = {{1,2},{1,3},{1,4},{2,3}};
        std::vector<int> important = {1,2,3,4};
        assert(findBestImportantNode(4, edges, important) == 1);
    }

    // Test 6: Multiple components, one candidate with reachable others
    // Component A: 1-2-3, important {1,2,3}
    // Component B: 4-5, important {4,5}
    // Candidates: node2 (degree2), nodes 4 and 5 (degree1 each) not candidates
    // Node2 avg = (1+1)/2=1
    // Node4 and 5 are not degree>=2, so only node2. returns 2
    {
        std::vector<std::pair<int,int>> edges = {{1,2},{2,3},{4,5}};
        std::vector<int> important = {1,2,3,4,5};
        assert(findBestImportantNode(5, edges, important) == 2);
    }

    // Test 7: Duplicate important nodes in input, count once
    {
        std::vector<std::pair<int,int>> edges = {{1,2},{2,3}};
        std::vector<int> important = {1,2,2,3};
        assert(findBestImportantNode(3, edges, important) == 2);
    }

    // Test 8: Single node graph with no edges
    {
        std::vector<std::pair<int,int>> edges;
        std::vector<int> important = {1};
        assert(findBestImportantNode(1, edges, important) == -1);
    }
}
