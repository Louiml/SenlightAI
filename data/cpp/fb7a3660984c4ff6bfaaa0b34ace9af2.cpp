// Write a C++ function `template<typename T> list<T> shortestPath(const Graph<T>& graph, T src, T dest)` that, given a graph constructed with the provided `Graph` class template (supporting `addEdge` for undirected edges and `printAdjList`), returns the shortest path from `src` to `dest` as a `std::list<T>` of vertices in order from `src` to `dest`. If `src` and `dest` are the same, return a list with only that vertex. If no path exists, return an empty list. The graph is unweighted and may contain disconnected components. Your function must use Breadth-First Search (BFS) and must not modify the graph. Note that the graph’s internal map stores adjacency lists; you may access them via a public `getAdjList` method (add this to the class if needed, but for this task assume it exists as `map<T, list<T>> getAdjList() const` or add a `const` accessor). For simplicity, assume `T` is a type that supports `==`, `!=`, and default construction.

#include <cassert>
#include <iostream>
#include <list>

int main() {
    // Test 1: Simple graph
    Graph<std::string> g;
    g.addEdge("A", "B");
    g.addEdge("B", "C");
    g.addEdge("A", "D");
    g.addEdge("D", "C");
    auto path1 = shortestPath(g, std::string("A"), std::string("C"));
    std::list<std::string> expected1 = {"A", "D", "C"};
    assert(path1 == expected1);

    // Test 2: Same source and destination
    auto path2 = shortestPath(g, std::string("A"), std::string("A"));
    std::list<std::string> expected2 = {"A"};
    assert(path2 == expected2);

    // Test 3: Disconnected graph (no path)
    g.addEdge("X", "Y");
    auto path3 = shortestPath(g, std::string("A"), std::string("X"));
    assert(path3.empty());

    // Test 4: Single-edge path
    auto path4 = shortestPath(g, std::string("X"), std::string("Y"));
    std::list<std::string> expected4 = {"X", "Y"};
    assert(path4 == expected4);

    // Test 5: Integer graph
    Graph<int> g2;
    g2.addEdge(1, 2);
    g2.addEdge(2, 3);
    g2.addEdge(3, 4);
    auto path5 = shortestPath(g2, 1, 4);
    std::list<int> expected5 = {1, 2, 3, 4};
    assert(path5 == expected5);

    // Test 6: Larger graph with multiple possible paths (shortest expected)
    Graph<char> g3;
    g3.addEdge('s', 'a');
    g3.addEdge('s', 'b');
    g3.addEdge('a', 't');
    g3.addEdge('b', 't');
    auto path6 = shortestPath(g3, 's', 't');
    assert(path6.size() == 3); // s -> a -> t or s -> b -> t
    assert(path6.front() == 's' && path6.back() == 't');

    std::cout << "All tests passed!" << std::endl;
    return 0;
}

#include <list>
#include <queue>
#include <unordered_map>
#include <map>

// Graph class template (simplified, with const accessor)
template<typename T>
class Graph {
public:
    std::map<T, std::list<T>> m; // adjacency list

    void addEdge(T u, T v, bool bidir = true) {
        m[u].push_back(v);
        if (bidir) m[v].push_back(u);
    }

    // const accessor to adjacency list
    const std::map<T, std::list<T>>& getAdjList() const { return m; }
};

// Returns the shortest path from src to dest using BFS.
// If no path exists, returns an empty list.
template<typename T>
std::list<T> shortestPath(const Graph<T>& graph, T src, T dest) {
    if (src == dest) {
        return {src};
    }

    const auto& adj = graph.getAdjList();
    std::queue<T> q;
    std::unordered_map<T, bool> visited;
    std::unordered_map<T, T> parent;

    q.push(src);
    visited[src] = true;
    parent[src] = src;

    while (!q.empty()) {
        T current = q.front();
        q.pop();

        // Find the adjacency list for current (if it exists)
        auto it = adj.find(current);
        if (it == adj.end()) continue;

        for (const T& neighbor : it->second) {
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                parent[neighbor] = current;
                q.push(neighbor);

                if (neighbor == dest) {
                    // Reconstruct path from dest back to src
                    std::list<T> path;
                    T temp = dest;
                    while (temp != src) {
                        path.push_front(temp);
                        temp = parent[temp];
                    }
                    path.push_front(src);
                    return path;
                }
            }
        }
    }

    return {}; // no path found
}

// The solution uses BFS from `src` to explore the graph level by level, which naturally gives the shortest path in an unweighted graph. We maintain a `queue<T>` for BFS traversal, an `unordered_map<T, bool> visited` to mark visited vertices, and an `unordered_map<T, T> parent` to record the previous vertex for each visited vertex (initialized to `src` for `src`). Starting from `src`, we enqueue it, mark as visited, and set its parent to itself. While the queue is not empty, we pop a vertex, and for each neighbor from its adjacency list (obtained from the graph’s adjacency map), if not visited, we set its parent to the current vertex, mark visited, and enqueue it. Once BFS completes, if `dest` is not visited, no path exists, so return an empty list. Otherwise, reconstruct the path by starting from `dest` and following `parent` until we reach `src`, collecting vertices in reverse order, then reverse to get from `src` to `dest`. Edge cases include: `src == dest` (return a list with one element), disconnected graphs (return empty list for unreachable dest), and large graphs (BFS is O(V+E)). Time complexity is O(V+E) where V is the number of vertices and E the number of edges. Space complexity is O(V) for the maps and queue.
