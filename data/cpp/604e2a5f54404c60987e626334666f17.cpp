/*
Write a standalone C++ function named `depthFirstTraversal` that takes a directed graph (represented as an adjacency list using `std::map<int, std::list<int>>`), a starting vertex `v`, and returns a `std::vector<int>` containing the vertices visited in the order produced by an iterative depth-first search (DFS) using a stack. The graph may contain isolated vertices (not present as keys in the adjacency map) and should produce output only for vertices reachable from the starting vertex. The function must handle graphs with up to 10^5 edges and vertices, and must correctly handle cycles and multiple edges. The function should be `const`-correct (i.e., not modify the input graph), and should return the traversal order as a vector of integers. The graph is directed, so edges are one-way. The traversal should visit vertices in the order they are first discovered (i.e., the order they are pushed onto the stack and then popped), which is the standard iterative DFS order. For example, for a graph with edges 0->1, 0->2, 1->3, and starting at 0, a possible valid output order is [0, 2, 1, 3] or [0, 1, 3, 2] depending on the adjacency order. The function should also work when the starting vertex is not in the graph (i.e., it has no outgoing edges), in which case it should return a vector containing only the starting vertex.
*/
#include <map>
#include <list>
#include <vector>
#include <stack>
#include <unordered_set>

// Perform iterative depth-first traversal on a directed graph.
// Returns a vector of visited vertices in the order they are first discovered.
std::vector<int> depthFirstTraversal(const std::map<int, std::list<int>>& graph, int start) {
    std::vector<int> result;
    std::unordered_set<int> visited;
    std::stack<int> stack;
    
    stack.push(start);
    
    while (!stack.empty()) {
        int current = stack.top();
        stack.pop();
        
        if (visited.find(current) == visited.end()) {
            visited.insert(current);
            result.push_back(current);
            
            // Push all neighbors (in the list order) onto the stack.
            auto it = graph.find(current);
            if (it != graph.end()) {
                for (int neighbor : it->second) {
                    if (visited.find(neighbor) == visited.end()) {
                        stack.push(neighbor);
                    }
                }
            }
        }
    }
    
    return result;
}
#include <cassert>
#include <vector>
#include <map>
#include <list>
#include <algorithm>

int main() {
    // Test 1: Simple directed graph
    std::map<int, std::list<int>> g1 = {{0, {1, 2}}, {1, {3}}, {2, {}}};
    std::vector<int> result1 = depthFirstTraversal(g1, 0);
    std::vector<int> expected1 = {0, 2, 1, 3}; // or {0,1,3,2} depending on adjacency order
    // Since order may vary, check set equality and size
    assert(result1.size() == 4);
    std::vector<int> sorted1 = result1;
    std::sort(sorted1.begin(), sorted1.end());
    assert(sorted1 == std::vector<int>({0, 1, 2, 3}));

    // Test 2: Starting vertex not in graph (isolated)
    std::map<int, std::list<int>> g2 = {{1, {2}}};
    std::vector<int> result2 = depthFirstTraversal(g2, 5);
    assert(result2.size() == 1 && result2[0] == 5);

    // Test 3: Graph with cycle
    std::map<int, std::list<int>> g3 = {{0, {1}}, {1, {2}}, {2, {0}}};
    std::vector<int> result3 = depthFirstTraversal(g3, 0);
    assert(result3.size() == 3);
    std::vector<int> sorted3 = result3;
    std::sort(sorted3.begin(), sorted3.end());
    assert(sorted3 == std::vector<int>({0, 1, 2}));

    // Test 4: Single vertex with no edges
    std::map<int, std::list<int>> g4 = {{7, {}}};
    std::vector<int> result4 = depthFirstTraversal(g4, 7);
    assert(result4.size() == 1 && result4[0] == 7);

    // Test 5: Multiple edges and isolated components
    std::map<int, std::list<int>> g5 = {{0, {1, 1, 2}}, {1, {2}}, {3, {4}}};
    std::vector<int> result5 = depthFirstTraversal(g5, 0);
    assert(result5.size() == 3);
    std::vector<int> sorted5 = result5;
    std::sort(sorted5.begin(), sorted5.end());
    assert(sorted5 == std::vector<int>({0, 1, 2}));

    // Test 6: Larger graph with disconnected start
    std::map<int, std::list<int>> g6 = {{0, {1}}, {2, {3}}, {3, {2}}};
    std::vector<int> result6 = depthFirstTraversal(g6, 2);
    assert(result6.size() == 2);
    std::vector<int> sorted6 = result6;
    std::sort(sorted6.begin(), sorted6.end());
    assert(sorted6 == std::vector<int>({2, 3}));

    // Test 7: Const-correctness check (function should not modify graph)
    std::map<int, std::list<int>> g7 = {{0, {1}}, {1, {0}}};
    std::map<int, std::list<int>> copy_g7 = g7;
    depthFirstTraversal(g7, 0);
    assert(g7 == copy_g7);

    // Test 8: All vertices reachable in a chain
    std::map<int, std::list<int>> g8 = {{0, {1}}, {1, {2}}, {2, {3}}};
    std::vector<int> result8 = depthFirstTraversal(g8, 0);
    assert(result8.size() == 4);
    std::vector<int> sorted8 = result8;
    std::sort(sorted8.begin(), sorted8.end());
    assert(sorted8 == std::vector<int>({0, 1, 2, 3}));

    return 0;
}
// The solution employs an iterative DFS using an explicit stack to avoid recursion depth issues on large graphs. The algorithm initializes a stack with the starting vertex and a `std::unordered_set` (or `std::map<bool>`) to track visited vertices. The main loop pops a vertex, and if it has not been visited, it records it in the result vector, marks it as visited, and then pushes all of its neighbors (from the adjacency list) onto the stack. This ensures each vertex is processed exactly once. The order of traversal depends on the adjacency list order; since the problem only requires a valid DFS order, this is acceptable. Edge cases include: (1) the starting vertex not appearing as a key in the adjacency map—in that case, no neighbors exist, so the function returns `{start}`; (2) disconnected components—only reachable vertices are visited; (3) cycles—visited set prevents infinite loops; (4) multiple edges—duplicate neighbors are ignored after first visit. Time complexity is O(V + E) where V is the number of reachable vertices and E is the number of edges, since each vertex is pushed/popped once and each edge is examined once. Space complexity is O(V) for the stack, visited set, and result vector, plus the adjacency list itself which is part of the input (not counted as extra space). The function is marked `const` because it does not modify the graph.
