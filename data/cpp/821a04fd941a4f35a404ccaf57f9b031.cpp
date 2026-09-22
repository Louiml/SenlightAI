Write a C++ function that takes a generic graph (with vertices of any hashable type, directed/undirected edges, and integer weights) and returns a vector containing all vertices in the order they are visited by a depth-first search (DFS) starting from a given source vertex, but only for the connected component that contains the source. The graph may contain disconnected components, cycles, and self-loops; the DFS should visit each vertex exactly once, and the order should follow the adjacency list order exactly as edges were added. The function should work for both `int` and `char` vertex types and must not modify the original graph.
#include <bits/stdc++.h>
#include <cassert>
using namespace std;

// Assume the graph class and dfsTraversal function are defined above (or include here)

int main() {
    // Test 1: Directed graph from snippet (int)
    graph<int> g1;
    g1.addEdge(1, 2, 5, 1);
    g1.addEdge(2, 3, 10, 1);
    g1.addEdge(3, 1, 12, 1);
    g1.addEdge(3, 4, 2, 1);
    g1.addEdge(4, 2, 7, 1);
    vector<int> result1 = dfsTraversal(g1, 1);
    // Starting at 1: neighbors in order: 2 → 2's neighbors: 3 → 3's neighbors: 1 (visited), 4 → 4's neighbor: 2 (visited)
    vector<int> expected1 = {1, 2, 3, 4};
    assert(result1 == expected1);

    // Test 2: Undirected graph (char) with disconnected components (from snippet)
    graph<char> g2;
    g2.addEdge('a', 'b', 5, 0);
    g2.addEdge('c', 'd', 20, 0);
    g2.addEdge('c', 'e', 50, 0);
    g2.addEdge('d', 'e', 20, 0);
    g2.addEdge('e', 'f', 50, 0);
    vector<char> result2 = dfsTraversal(g2, 'a');
    vector<char> expected2 = {'a', 'b'};
    assert(result2 == expected2);

    // Test 3: DFS from 'c' in same graph
    vector<char> result3 = dfsTraversal(g2, 'c');
    // Starting at c: neighbors (in insertion order): d, e. 
    // From d: e (already visited? no, but visited after c's neighbor loop) — careful: order depends on adjacency list order.
    // g2.adjList['c'] = [('d',20), ('e',50)] → first visit d, then d's neighbors: c(visited), e → visit e, then e's neighbors: c(visited), d(visited), f → visit f
    // Result: c, d, e, f
    vector<char> expected3 = {'c', 'd', 'e', 'f'};
    assert(result3 == expected3);

    // Test 4: Source not in graph
    graph<int> g4;
    g4.addEdge(1, 2, 1, 0);
    vector<int> result4 = dfsTraversal(g4, 99);
    assert(result4.empty());

    // Test 5: Self-loop and cycle
    graph<char> g5;
    g5.addEdge('x', 'x', 1, 1); // self-loop directed
    g5.addEdge('x', 'y', 2, 1);
    g5.addEdge('y', 'x', 3, 1);
    vector<char> result5 = dfsTraversal(g5, 'x');
    vector<char> expected5 = {'x', 'y'};
    assert(result5 == expected5);

    // Test 6: Isolated single vertex (undirected)
    graph<int> g6;
    g6.addEdge(10, 20, 1, 0);
    g6.addEdge(30, 40, 1, 0);
    vector<int> result6 = dfsTraversal(g6, 30);
    assert(result6 == vector<int>({30, 40}));

    return 0;
}
#include <bits/stdc++.h>
using namespace std;

// Template class as given in the problem statement
template<typename T>
class graph {
public:
    unordered_map<T, vector<pair<T, int>>> adjList;

    void addEdge(T u, T v, int wt, bool dir) {
        if (dir == 0) {
            adjList[u].push_back({v, wt});
            adjList[v].push_back({u, wt});
        } else {
            adjList[u].push_back({v, wt});
        }
    }
};

// Helper recursive function for DFS traversal
template<typename T>
void dfsHelper(const graph<T>& g, T node, unordered_map<T, bool>& visited, vector<T>& result) {
    visited[node] = true;
    result.push_back(node);
    auto it = g.adjList.find(node);
    if (it != g.adjList.end()) {
        for (const auto& neighbor : it->second) {
            if (!visited[neighbor.first]) {
                dfsHelper(g, neighbor.first, visited, result);
            }
        }
    }
}

// Public function: returns DFS order from source within its connected component
template<typename T>
vector<T> dfsTraversal(const graph<T>& g, T source) {
    vector<T> result;
    unordered_map<T, bool> visited;
    // If source doesn't exist in graph, return empty
    if (g.adjList.find(source) == g.adjList.end())
        return result;
    dfsHelper(g, source, visited, result);
    return result;
}
// The solution uses the provided `graph` template class. Since the graph class stores adjacency lists in an `unordered_map`, the iteration order of vertices for a given vertex follows the order in which edges were added (vector order). The DFS implementation should mirror the original `dfs` method but collect visited vertices into a vector instead of printing them. Important edge cases: (1) the source vertex may not exist in the graph — then return an empty vector; (2) the graph may be disconnected — only the component reachable from source is returned; (3) self-loops and cycles must not cause infinite recursion (handled by marking visited before recursion). The algorithm is recursive, with a base case when all neighbors are visited. Time complexity is O(V + E) for the visited component, and space complexity is O(V) for the recursion stack and visited map. Since the function takes the graph by const reference, we need to make the `adjList` accessible — but the provided class exposes it as public, so we can use it directly. We must ensure we only mark a vertex as visited when we first encounter it (before recursing) to avoid duplicates.
