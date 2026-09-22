Write a C++ function `bool topologicalSortFromAdjacency(int vertices, const int* const* adj, const int* sizes, std::vector<int>& result)` that takes the number of vertices, an adjacency list stored as a 2D array (each row is dynamically allocated and `sizes[i]` gives the number of neighbors of vertex `i`), and performs a depth-first search (DFS) based topological sort. The graph is directed and may be disconnected (some vertices may have no outgoing edges). The function must return `true` if a topological order exists (i.e., the graph is a DAG) and fill `result` with one valid topological ordering. If the graph contains a cycle, return `false` and leave `result` empty. The function must handle vertices with zero out-degree, isolated vertices, and multiple components. It should use iterative or recursive DFS with a visited and recursion-stack mechanism to detect cycles. The order produced must be such that for every directed edge `u -> v`, `u` appears before `v` in `result`. The function must not modify the input adjacency structure.

// The solution uses a standard DFS-based topological sort. For each vertex, we run DFS if it has not been visited yet. During DFS, we mark the vertex as visited and push it onto a recursion stack. For every neighbor `v` of current vertex `u`, if `v` is not visited, recurse; if `v` is already on the recursion stack, that indicates a back edge, meaning a cycle exists, so we return `false`. After processing all neighbors, we pop `u` from the recursion stack and push `u` onto a stack that stores the topological order. Because we push vertices after their descendants, the stack will contain vertices in reverse topological order, so we later pop them to obtain the correct order. Edge cases: isolated vertices (no outgoing edges) are simply pushed after their DFS (which is trivial), and they appear anywhere in the order; vertices with zero out-degree are processed without recursion; disconnected components are handled by the outer loop. Time complexity is `O(V + E)` where `V` is vertices and `E` is the total number of directed edges (sum of all `sizes[i]`). Space complexity is `O(V)` for visited, recursion stack, and the result vector (excluding the input adjacency).

#include <vector>
#include <stack>

// Perform DFS-based topological sort on a directed graph represented by adjacency lists.
// adj is a 2D array: adj[i] points to an array of size sizes[i] containing neighbors of vertex i.
// Returns true if graph is a DAG and fills result with a topological ordering; false if cycle exists (result left empty).
bool topologicalSortFromAdjacency(int vertices, const int* const* adj, const int* sizes, std::vector<int>& result) {
    std::vector<bool> visited(vertices, false);
    std::vector<bool> inStack(vertices, false);
    std::stack<int> orderStack;

    // Recursive lambda for DFS
    std::function<bool(int)> dfs = [&](int u) -> bool {
        visited[u] = true;
        inStack[u] = true;
        for (int i = 0; i < sizes[u]; ++i) {
            int v = adj[u][i];
            if (!visited[v]) {
                if (!dfs(v)) {
                    return false;
                }
            } else if (inStack[v]) {
                return false; // back edge -> cycle
            }
        }
        inStack[u] = false;
        orderStack.push(u);
        return true;
    };

    for (int i = 0; i < vertices; ++i) {
        if (!visited[i]) {
            if (!dfs(i)) {
                result.clear();
                return false;
            }
        }
    }

    result.clear();
    while (!orderStack.empty()) {
        result.push_back(orderStack.top());
        orderStack.pop();
    }
    return true;
}

#include <cassert>
#include <vector>
#include <functional>

// Include the solution function here (or link it) – for standalone, define it above.

int main() {
    // Test 1: Simple DAG from the snippet (vertices 0..7, edges: 5->2,5->0,4->0,4->1,2->3,3->1)
    {
        int v = 8;
        int* adj[8];
        for (int i = 0; i < v; ++i) adj[i] = new int[8];
        int sizes[8] = {0,0,1,1,2,2,0,0};
        adj[5][0]=2; adj[5][1]=0;
        adj[4][0]=0; adj[4][1]=1;
        adj[2][0]=3;
        adj[3][0]=1;
        std::vector<int> result;
        bool ok = topologicalSortFromAdjacency(v, adj, sizes, result);
        assert(ok);
        assert(result.size() == v);
        // Check edge constraints: for each edge, u appears before v in result
        auto pos = [&](int x){ for (size_t i=0;i<result.size();++i) if(result[i]==x) return (int)i; return -1; };
        assert(pos(5) < pos(2) && pos(5) < pos(0));
        assert(pos(4) < pos(0) && pos(4) < pos(1));
        assert(pos(2) < pos(3));
        assert(pos(3) < pos(1));
        for (int i=0;i<v;++i) delete[] adj[i];
    }

    // Test 2: Cycle detection (simple cycle 0->1->2->0)
    {
        int v = 3;
        int* adj[3];
        for (int i = 0; i < v; ++i) adj[i] = new int[3];
        int sizes[3] = {1,1,1};
        adj[0][0]=1; adj[1][0]=2; adj[2][0]=0;
        std::vector<int> result;
        bool ok = topologicalSortFromAdjacency(v, adj, sizes, result);
        assert(!ok);
        assert(result.empty());
        for (int i=0;i<v;++i) delete[] adj[i];
    }

    // Test 3: Disconnected graph with isolated vertices
    {
        int v = 4;
        int* adj[4];
        for (int i = 0; i < v; ++i) adj[i] = new int[1];
        int sizes[4] = {0,0,0,0}; // no edges
        std::vector<int> result;
        bool ok = topologicalSortFromAdjacency(v, adj, sizes, result);
        assert(ok);
        assert(result.size() == v);
        // any order is valid
        for (int i=0;i<v;++i) delete[] adj[i];
    }

    // Test 4: Single edge 0->1
    {
        int v = 2;
        int* adj[2];
        adj[0] = new int[1]; adj[1] = new int[0];
        int sizes[2] = {1,0};
        adj[0][0] = 1;
        std::vector<int> result;
        bool ok = topologicalSortFromAdjacency(v, adj, sizes, result);
        assert(ok);
        assert(result.size() == 2);
        assert(result[0] == 0 && result[1] == 1);
        delete[] adj[0]; delete[] adj[1];
    }

    // Test 5: Multiple components with edges (0->1, 2->3) no cycles
    {
        int v = 4;
        int* adj[4];
        for (int i=0;i<v;++i) adj[i] = new int[1];
        adj[0][0]=1; adj[2][0]=3;
        int sizes[4] = {1,0,1,0};
        std::vector<int> result;
        bool ok = topologicalSortFromAdjacency(v, adj, sizes, result);
        assert(ok);
        assert(result.size() == v);
        auto pos = [&](int x){ for (size_t i=0;i<result.size();++i) if(result[i]==x) return (int)i; return -1; };
        assert(pos(0) < pos(1));
        assert(pos(2) < pos(3));
        for (int i=0;i<v;++i) delete[] adj[i];
    }

    return 0;
}
