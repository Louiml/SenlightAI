// Write a C++ function that, given an undirected weighted graph represented by a fixed-size adjacency matrix (with a maximum of 50 nodes), two node names (source and destination), and an array of node names, computes the shortest path distance between the two nodes using Dijkstra's algorithm. The graph uses `INF` (a large value like 1e9) to indicate no direct connection, and each valid edge has a positive integer weight (distance in meters). The function must return the shortest distance as an integer; if either node does not exist or no path exists, it must return `-1`. The graph is provided as a structure containing a 2D array `adjMatrix[50][50]`, an integer `nodeCount`, and an array `nodeNames[50]` of strings. The function should also handle duplicate node names gracefully (assume they are unique in valid input). You are not required to print the path, only return the distance.
The solution uses Dijkstra's algorithm because the graph has non-negative edge weights and we need the shortest path from a single source to a specific destination. First, map each node name to its index by scanning the `nodeNames` array; if either the source or destination is not found, return `-1`. Initialize a distance array of size `nodeCount` with `INF` (or a very large number), set the source's distance to 0, and use a boolean visited array. In each iteration, select the unvisited node with the smallest tentative distance. If that node is unreachable (distance is `INF`), break early. Then relax all its neighbors: for each neighbor `v` that is unvisited and has a direct edge `adjMatrix[u][v] != INF`, update `dist[v] = min(dist[v], dist[u] + adjMatrix[u][v])`. After the loop, if `dist[destinationIndex]` is still `INF`, return `-1`; otherwise return that value. Edge cases: missing nodes, disconnected graph (no path), single node (source == destination -> returns 0), and edges with zero or negative weights (though specification says positive, but function should still handle if non-positive by ignoring or treating as no edge? The task says positive integer weights, so we assume valid). Time complexity is O(V^2) due to the simple linear search for the minimum each iteration, where V is the number of nodes (at most 50). Space complexity is O(V) for distances and visited arrays.
#include <string>
#include <climits>

const int MAX_NODES = 50;
const int INF = 1e9; // Large value representing no direct connection

struct Graph {
    int adjMatrix[MAX_NODES][MAX_NODES];
    int nodeCount;
    std::string nodeNames[MAX_NODES];
};

// Helper to find index of a node by name, returns -1 if not found
int findNodeIndex(const Graph& g, const std::string& name) {
    for (int i = 0; i < g.nodeCount; ++i) {
        if (g.nodeNames[i] == name) return i;
    }
    return -1;
}

// Returns the shortest distance between two nodes, or -1 if unreachable or invalid
int shortestDistance(const Graph& g, const std::string& source, const std::string& destination) {
    int src = findNodeIndex(g, source);
    int dst = findNodeIndex(g, destination);
    if (src == -1 || dst == -1) return -1;

    int dist[MAX_NODES];
    bool visited[MAX_NODES] = {false};
    for (int i = 0; i < g.nodeCount; ++i) {
        dist[i] = INF;
    }
    dist[src] = 0;

    for (int i = 0; i < g.nodeCount; ++i) {
        // Find unvisited node with minimum distance
        int u = -1;
        int minDist = INF;
        for (int j = 0; j < g.nodeCount; ++j) {
            if (!visited[j] && dist[j] < minDist) {
                minDist = dist[j];
                u = j;
            }
        }
        if (u == -1) break; // No reachable unvisited nodes
        visited[u] = true;

        // Relax edges from u
        for (int v = 0; v < g.nodeCount; ++v) {
            if (!visited[v] && g.adjMatrix[u][v] != INF) {
                int newDist = dist[u] + g.adjMatrix[u][v];
                if (newDist < dist[v]) {
                    dist[v] = newDist;
                }
            }
        }
    }

    return (dist[dst] == INF) ? -1 : dist[dst];
}
#include <cassert>
#include <iostream>

int main() {
    // Test 1: Simple graph with known path
    Graph g1;
    g1.nodeCount = 4;
    g1.nodeNames[0] = "A"; g1.nodeNames[1] = "B"; g1.nodeNames[2] = "C"; g1.nodeNames[3] = "D";
    // Initialize matrix with INF except diagonal 0
    for (int i = 0; i < g1.nodeCount; ++i)
        for (int j = 0; j < g1.nodeCount; ++j)
            g1.adjMatrix[i][j] = (i == j) ? 0 : INF;
    g1.adjMatrix[0][1] = 10; g1.adjMatrix[1][0] = 10;
    g1.adjMatrix[1][2] = 20; g1.adjMatrix[2][1] = 20;
    g1.adjMatrix[0][2] = 50; g1.adjMatrix[2][0] = 50;
    g1.adjMatrix[2][3] = 5;  g1.adjMatrix[3][2] = 5;
    assert(shortestDistance(g1, "A", "C") == 30); // A->B->C = 10+20
    assert(shortestDistance(g1, "A", "D") == 35); // A->B->C->D = 10+20+5
    assert(shortestDistance(g1, "B", "D") == 25); // B->C->D = 20+5

    // Test 2: Disconnected node
    Graph g2;
    g2.nodeCount = 3;
    g2.nodeNames[0] = "X"; g2.nodeNames[1] = "Y"; g2.nodeNames[2] = "Z";
    for (int i = 0; i < g2.nodeCount; ++i)
        for (int j = 0; j < g2.nodeCount; ++j)
            g2.adjMatrix[i][j] = (i == j) ? 0 : INF;
    g2.adjMatrix[0][1] = 5; g2.adjMatrix[1][0] = 5;
    // No edge to Z
    assert(shortestDistance(g2, "X", "Z") == -1);

    // Test 3: Source equals destination
    Graph g3;
    g3.nodeCount = 1;
    g3.nodeNames[0] = "Solo";
    g3.adjMatrix[0][0] = 0;
    assert(shortestDistance(g3, "Solo", "Solo") == 0);

    // Test 4: Invalid node name
    Graph g4 = g1;
    assert(shortestDistance(g4, "A", "NonExistent") == -1);
    assert(shortestDistance(g4, "Missing", "A") == -1);

    // Test 5: Multiple paths - choose shorter
    Graph g5;
    g5.nodeCount = 3;
    g5.nodeNames[0] = "P"; g5.nodeNames[1] = "Q"; g5.nodeNames[2] = "R";
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            g5.adjMatrix[i][j] = (i == j) ? 0 : INF;
    g5.adjMatrix[0][1] = 7; g5.adjMatrix[1][0] = 7;
    g5.adjMatrix[1][2] = 7; g5.adjMatrix[2][1] = 7;
    g5.adjMatrix[0][2] = 15; g5.adjMatrix[2][0] = 15;
    assert(shortestDistance(g5, "P", "R") == 14); // P->Q->R = 7+7, not direct 15

    // Test 6: Larger graph with more nodes
    Graph g6;
    g6.nodeCount = 5;
    g6.nodeNames[0] = "a"; g6.nodeNames[1] = "b"; g6.nodeNames[2] = "c"; g6.nodeNames[3] = "d"; g6.nodeNames[4] = "e";
    for (int i = 0; i < 5; ++i)
        for (int j = 0; j < 5; ++j)
            g6.adjMatrix[i][j] = (i == j) ? 0 : INF;
    g6.adjMatrix[0][1] = 2; g6.adjMatrix[1][0] = 2;
    g6.adjMatrix[1][2] = 3; g6.adjMatrix[2][1] = 3;
    g6.adjMatrix[2][3] = 1; g6.adjMatrix[3][2] = 1;
    g6.adjMatrix[3][4] = 4; g6.adjMatrix[4][3] = 4;
    g6.adjMatrix[0][4] = 20; g6.adjMatrix[4][0] = 20;
    assert(shortestDistance(g6, "a", "e") == 10); // a-b-c-d-e = 2+3+1+4

    // Test 7: Edge with large weight but still valid
    Graph g7;
    g7.nodeCount = 2;
    g7.nodeNames[0] = "u"; g7.nodeNames[1] = "v";
    g7.adjMatrix[0][0] = 0; g7.adjMatrix[1][1] = 0;
    g7.adjMatrix[0][1] = 1000000; g7.adjMatrix[1][0] = 1000000;
    assert(shortestDistance(g7, "u", "v") == 1000000);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
