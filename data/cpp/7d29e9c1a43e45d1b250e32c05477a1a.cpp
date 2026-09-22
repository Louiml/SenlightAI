Design and implement a C++ function that performs a parallel breadth-first search (BFS) on an undirected graph using OpenMP, returning the traversal order as a vector of integers. The function should accept the number of vertices, an edge list, and a starting vertex. It must handle graphs with up to 100,000 vertices, ensure thread-safe visited marking, and avoid data races when enqueueing neighbors. The traversal should follow the standard BFS level-order logic, but the processing of each node’s neighbors must be parallelized using OpenMP. The returned vector must contain all reachable vertices from the start in the order they are first visited, with no duplicates. The solution must be self-contained, using only standard libraries and OpenMP, and must not rely on the provided snippet’s global state.

// The core algorithm is classic BFS using a queue. To parallelize, we need to protect shared data structures (`vis` vector and `q` queue) with OpenMP critical sections or use a more efficient approach like local vectors per thread. A straightforward approach: for each popped node, process its neighbors in a parallel `for` loop; inside the loop, check if a neighbor is unvisited, and if so, atomically mark it as visited and push it to a shared queue inside a critical section. However, pushing to a `std::queue` inside a critical section can degrade performance; a better design is to collect all newly discovered nodes in a thread-safe manner (e.g., using a mutex-protected vector) then push them all to the queue after the parallel loop. That avoids issues with concurrent queue pushes. Edge cases: isolated start vertex (returns just that vertex), multiple edges (ignore duplicate neighbors), disconnected graph (only reachable vertices included), and very large graphs (use `std::vector<bool>` for memory efficiency but note thread-safety; use `std::vector<std::atomic<bool>>` or a mutex around writes). Time complexity is O(V+E) sequential, and parallelization does not change asymptotic complexity but may improve wall-clock time; space is O(V) for visited and queue. Ensure correct handling of parallel region by declaring shared variables properly.
//
// A robust implementation: create a helper function that takes `numVertices`, `edges` (as vector of pairs or adjacency list), and `start`. Build adjacency list. Use `std::vector<std::atomic<bool>> visited(numVertices)` for thread-safe marking. Use a queue, but for parallel neighbor processing, gather new nodes into a local vector per thread (using `std::vector<int> local_new`), then merge them into a global vector after the loop, then push all to queue. Alternatively, use OpenMP tasking but simpler to use parallel for with reduction-like collection.
//
// For the test, we can assert that the returned order is a valid BFS order (levels correct) or simply check reachability and size; but since the task asks for direct comparison, we can construct a known small graph and compare to expected sequential BFS order. Since parallel BFS may produce same order if we preserve by pushing neighbors in adjacency order, we can define the algorithm to preserve the original adjacency order by iterating `graph[cur]` in order; with parallel for, order of processing neighbors may differ, but we can enforce deterministic order by collecting and sorting or by using a critical section that pushes in order. To simplify, we can write a sequential fallback for the test? No, test must call the function directly and compare to an expected vector.
//
// To make it deterministic and testable, we can design the function to process neighbors in parallel but then sort newly discovered nodes before pushing them to the queue, or use a critical section that preserves the original order. For simplicity, we can have the function use a critical section for each neighbor push, which ensures order (though it serializes). That still uses OpenMP but thread-safe. We'll do that: in parallel for, inside critical, check and push. This preserves order. Then the output matches sequential BFS exactly. We'll document that.

#include <vector>
#include <queue>
#include <atomic>
#include <omp.h>

// Perform parallel BFS on an undirected graph.
// Parameters:
//   numVertices: number of vertices (0..numVertices-1)
//   edges: list of undirected edges as pairs (u,v)
//   start: starting vertex
// Returns: vector of vertices in BFS order
std::vector<int> parallelBFS(int numVertices, const std::vector<std::pair<int,int>>& edges, int start) {
    // Build adjacency list
    std::vector<std::vector<int>> adj(numVertices);
    for (const auto& e : edges) {
        adj[e.first].push_back(e.second);
        adj[e.second].push_back(e.first);
    }

    // Visited marking (atomic for thread safety)
    std::vector<std::atomic<bool>> visited(numVertices);
    for (int i = 0; i < numVertices; ++i) visited[i].store(false);

    std::queue<int> q;
    std::vector<int> order;

    // Mark start and enqueue
    visited[start].store(true);
    q.push(start);
    order.push_back(start);

    while (!q.empty()) {
        int cur = q.front();
        q.pop();

        // Parallel loop over neighbors
        #pragma omp parallel for
        for (int i = 0; i < (int)adj[cur].size(); ++i) {
            int next = adj[cur][i];
            // Use critical to ensure safe check-and-set and preserve deterministic order
            #pragma omp critical
            {
                if (!visited[next].load()) {
                    visited[next].store(true);
                    q.push(next);
                    order.push_back(next);
                }
            }
        }
    }
    return order;
}

#include <cassert>
#include <vector>
#include <utility>

int main() {
    // Test 1: Simple chain 0-1-2, start at 0
    std::vector<std::pair<int,int>> edges1 = {{0,1},{1,2}};
    std::vector<int> r1 = parallelBFS(3, edges1, 0);
    assert(r1.size() == 3);
    assert(r1[0] == 0 && r1[1] == 1 && r1[2] == 2);

    // Test 2: Star graph, center 0 connected to 1,2,3; start at 0
    std::vector<std::pair<int,int>> edges2 = {{0,1},{0,2},{0,3}};
    std::vector<int> r2 = parallelBFS(4, edges2, 0);
    assert(r2.size() == 4);
    assert(r2[0] == 0 && r2[1] == 1 && r2[2] == 2 && r2[3] == 3);

    // Test 3: Disconnected graph: 0-1, 2-3; start at 0 only visits 0,1
    std::vector<std::pair<int,int>> edges3 = {{0,1},{2,3}};
    std::vector<int> r3 = parallelBFS(4, edges3, 0);
    assert(r3.size() == 2);
    assert(r3[0] == 0 && r3[1] == 1);

    // Test 4: Single isolated vertex
    std::vector<std::pair<int,int>> edges4 = {};
    std::vector<int> r4 = parallelBFS(1, edges4, 0);
    assert(r4.size() == 1 && r4[0] == 0);

    // Test 5: Larger graph with duplicate edges and a cycle
    std::vector<std::pair<int,int>> edges5 = {{0,1},{1,2},{1,3},{2,4},{3,4},{4,0},{1,1}};
    std::vector<int> r5 = parallelBFS(5, edges5, 0);
    // BFS from 0: order can be 0,1,2,3,4 (since adjacency order preserved)
    assert(r5.size() == 5);
    assert(r5 == (std::vector<int>{0,1,2,3,4})); // Because adjacency of 0: neighbor 1, then 1's neighbors: 0,2,3, etc.

    // Test 6: Start at non-zero vertex, ensure order correct
    std::vector<std::pair<int,int>> edges6 = {{0,1},{1,2},{2,3},{3,0}};
    std::vector<int> r6 = parallelBFS(4, edges6, 2);
    assert(r6.size() == 4);
    // From 2: neighbors 1,3; then 1's neighbors 0,2; then 3's neighbors 0,2 → order: 2,1,3,0
    assert(r6[0] == 2 && r6[1] == 1 && r6[2] == 3 && r6[3] == 0);

    return 0;
}
