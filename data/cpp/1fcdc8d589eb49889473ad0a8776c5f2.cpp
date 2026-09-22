// Given an undirected graph represented by an adjacency list (using the provided `Graph` and `LLNode` structures) and two integers `s` and `d` representing source and destination vertices, write a C++ function that performs a breadth-first search (BFS) from source `s` and returns (via output to standard output) the shortest path from `s` to `d` as a sequence of vertices separated by spaces, following the format `s v1 v2 ... d`. If no path exists, the function should print nothing (or an empty line). The function must handle graphs with up to a few hundred vertices, must avoid revisiting nodes, and must correctly format the path even if `s == d` (printing just that single vertex). You may assume all vertex indices are valid (0 ≤ u, v < `nv`), the graph may contain multiple edges (parallel edges) and self-loops, and the graph is undirected.

The core challenge is to find and reconstruct a shortest path in an unweighted undirected graph using BFS, which guarantees the shortest path in terms of number of edges. The algorithm works as follows:
1. Initialize a `visited` boolean array of size `g->nv`, all set to `false`, and a `parent` integer array of size `g->nv` initialized to `-1` (or any sentinel indicating "no parent").
2. Create a queue for BFS, push `s`, mark `visited[s] = true`, and set `parent[s] = s` (or a special marker like `-1` to indicate it is the root).
3. While the queue is not empty:
   - Pop the front vertex `u`.
   - If `u == d`, we have reached the destination; break out of the loop.
   - For each neighbor `v` in the adjacency list of `u` (using the `LLNode` linked list), if `visited[v]` is false, then set `visited[v] = true`, set `parent[v] = u`, and push `v` onto the queue.
4. After BFS, if `visited[d]` is false, there is no path; print nothing (or optionally a newline).
5. Else, reconstruct the path from `d` back to `s` using the `parent` array:
   - Start with a vector (or array) to accumulate vertices in reverse order.
   - While the current vertex is not `s` (or until we reach the root), push the current vertex, then move to `parent[current]`.
   - Finally, push `s`.
   - Reverse the vector before printing to get the correct order from `s` to `d`.
6. Print each vertex followed by a space, then a newline at the end.

Important edge cases:
- `s == d`: The BFS immediately finds the destination, and the path should print just `s` (or `s` followed by a newline).
- No path exists: The BFS completes without visiting `d`, so print nothing (or just a newline; the specification says "print nothing (or an empty line)").
- Self-loops: A self-loop `u -> u` is an edge that connects a vertex to itself. In BFS, when processing `u`, if we see `v == u`, we check if `visited[u]` is false—but it is already true (since we just popped it), so we skip it. That's correct because a self-loop does not create a new distinct path to a different vertex. However, if `d == s`, the path is just `s` regardless of self-loops.
- Parallel edges: Multiple edges between the same pair of vertices do not affect shortest path length; BFS will only mark `v` as visited the first time it encounters it, which is fine.
- Graph with `nv` vertices but some isolated (no edges): BFS will only traverse reachable vertices. If `d` is not reachable, no path.

Time complexity: BFS visits each vertex at most once and processes each edge twice (once per direction) in an undirected graph. Therefore, the time complexity is O(V + E), where V = number of vertices (`nv`) and E = number of edges (`ne`). Space complexity is O(V) for the `visited`, `parent`, and the BFS queue, plus the space for the path reconstruction vector (which is at most O(V)).

#include <iostream>
#include <queue>
#include <vector>
#include <cstring>
#include "../graph.h"  // Assumes Graph and LLNode are defined here
#include "../llnode.h" // Assumes insertInBegin, deleteNodeWithValue, traverse are defined

// Use declaration to access the LLNode structure
using namespace std;

/**
 * Prints the shortest path from source s to destination d in an undirected graph
 * using Breadth-First Search (BFS). If no path exists, prints a newline.
 * The function assumes valid vertex indices and that the graph is undirected.
 */
void printShortestPathBFS(Graph* graph, int source, int destination) {
    int numVertices = graph->nv;
    
    // Arrays to track visited states and parent pointers for BFS
    bool* visited = new bool[numVertices];
    int* parent = new int[numVertices];
    for (int i = 0; i < numVertices; ++i) {
        visited[i] = false;
        parent[i] = -1;  // -1 indicates no parent yet
    }
    
    // BFS queue
    queue<int> bfsQueue;
    bfsQueue.push(source);
    visited[source] = true;
    parent[source] = source;  // Mark source as its own parent for path termination
    
    // Perform BFS until queue is empty or we find the destination
    while (!bfsQueue.empty()) {
        int current = bfsQueue.front();
        bfsQueue.pop();
        
        // If we reached the destination, stop early
        if (current == destination) {
            break;
        }
        
        // Traverse adjacency list of current vertex
        LLNode* neighborNode = graph->adjList[current];
        while (neighborNode != nullptr) {
            int neighbor = neighborNode->data;
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                parent[neighbor] = current;
                bfsQueue.push(neighbor);
            }
            neighborNode = neighborNode->next;
        }
    }
    
    // Check if destination was reached
    if (!visited[destination]) {
        // No path exists; print empty line (or nothing). Print newline for consistency.
        cout << endl;
    } else {
        // Reconstruct path from destination back to source using parent array
        vector<int> path;
        int current = destination;
        while (current != source) {
            path.push_back(current);
            current = parent[current];
        }
        path.push_back(source);  // Add source
        
        // Print in reverse order (from source to destination)
        // Since we built from destination backwards, reverse it
        for (int i = static_cast<int>(path.size()) - 1; i >= 0; --i) {
            cout << path[i];
            if (i > 0) cout << " ";
        }
        cout << endl;
    }
    
    // Clean up dynamically allocated arrays
    delete[] visited;
    delete[] parent;
}

#include <cassert>
#include <iostream>
#include <sstream>

// Assume the Graph and LLNode structures are defined in the header files
// and the helper functions (createGraph, insertEdge, printShortestPathBFS) are available.
// For a self-contained test, we include the necessary definitions here.
// (In practice, you would include the actual header files.)

// Minimal definitions for testing (just for demonstration; normally from headers)
struct LLNode { int data; LLNode* next; };
LLNode* insertInBegin(LLNode* head, int val) {
    LLNode* newNode = new LLNode{val, head};
    return newNode;
}
struct Graph {
    int nv;
    int ne;
    LLNode** adjList;
};

// Declare the solution function
void printShortestPathBFS(Graph* graph, int source, int destination);

// For testing, capture output of printShortestPathBFS
std::string captureOutput(Graph* g, int s, int d) {
    std::ostringstream oss;
    std::streambuf* old = std::cout.rdbuf(oss.rdbuf());
    printShortestPathBFS(g, s, d);
    std::cout.rdbuf(old);
    return oss.str();
}

int main() {
    // Test case 1: Simple path 0-1-2
    Graph* g1 = new Graph;
    g1->nv = 3;
    g1->ne = 0;
    g1->adjList = new LLNode*[3] {nullptr, nullptr, nullptr};
    insertEdge(g1, 0, 1);
    insertEdge(g1, 1, 2);
    assert(captureOutput(g1, 0, 2) == "0 1 2\n");
    
    // Test case 2: Source equals destination
    assert(captureOutput(g1, 1, 1) == "1\n");
    
    // Test case 3: Disconnected vertices no path
    Graph* g2 = new Graph;
    g2->nv = 4;
    g2->ne = 0;
    g2->adjList = new LLNode*[4] {nullptr, nullptr, nullptr, nullptr};
    insertEdge(g2, 0, 1);
    insertEdge(g2, 2, 3);
    assert(captureOutput(g2, 0, 3) == "\n");
    
    // Test case 4: Graph with self-loop and parallel edges, shortest path still correct
    Graph* g3 = new Graph;
    g3->nv = 5;
    g3->ne = 0;
    g3->adjList = new LLNode*[5] {nullptr, nullptr, nullptr, nullptr, nullptr};
    insertEdge(g3, 0, 0); // self-loop
    insertEdge(g3, 0, 1);
    insertEdge(g3, 1, 2);
    insertEdge(g3, 2, 3);
    insertEdge(g3, 3, 4);
    insertEdge(g3, 0, 4); // direct edge makes path 0-4 shorter
    assert(captureOutput(g3, 0, 4) == "0 4\n");
    
    // Test case 5: Larger graph, ensure BFS gives shortest due to breadth
    Graph* g4 = new Graph;
    g4->nv = 6;
    g4->ne = 0;
    g4->adjList = new LLNode*[6] {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};
    insertEdge(g4, 0, 1);
    insertEdge(g4, 1, 2);
    insertEdge(g4, 2, 3);
    insertEdge(g4, 0, 4);
    insertEdge(g4, 4, 5);
    insertEdge(g4, 5, 3); // creates shorter path 0-4-5-3 (length 3) vs 0-1-2-3 (length 3)
    assert(captureOutput(g4, 0, 3) == "0 1 2 3\n"); // BFS picks the first found; both are equal length
    
    // Test case 6: Star graph, check path to leaf
    Graph* g5 = new Graph;
    g5->nv = 4;
    g5->ne = 0;
    g5->adjList = new LLNode*[4] {nullptr, nullptr, nullptr, nullptr};
    insertEdge(g5, 0, 1);
    insertEdge(g5, 0, 2);
    insertEdge(g5, 0, 3);
    assert(captureOutput(g5, 0, 3) == "0 3\n");
    
    // Clean up (simplified; real code would use destroyGraph)
    // Not fully implemented here for brevity.
    
    return 0;
}
