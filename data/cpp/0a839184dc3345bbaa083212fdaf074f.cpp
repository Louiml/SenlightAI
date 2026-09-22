Write a C++ function named `shortestPathCost` that, given a starting node and a destination node in a directed weighted graph (with non-negative edge weights), returns the minimum total cost to reach the destination from the start. The graph is represented using the provided `Node` class, where each node stores its outgoing neighbors in a `std::map<Node*, uint32_t>`. The function must handle graphs with up to hundreds of nodes, including cases where the destination is unreachable (return a sentinel value like `UINT32_MAX`) and where nodes have multiple edges (only the smallest weight per neighbor is stored by `addNeigbor`). The function should not modify the graph and must be const-correct where possible.
#include <cassert>
#include <iostream>

int main() {
    // Test case 1: simple path
    Node a("A"), b("B"), c("C");
    a.addNeighbor(b, 5);
    b.addNeighbor(c, 3);
    assert(shortestPathCost(a, c) == 8);

    // Test case 2: direct edge vs. indirect longer path
    Node s("S"), t("T"), u("U");
    s.addNeighbor(t, 10);
    s.addNeighbor(u, 2);
    u.addNeighbor(t, 3);
    assert(shortestPathCost(s, t) == 5);

    // Test case 3: unreachable destination
    Node x("X"), y("Y"), z("Z");
    x.addNeighbor(y, 1);
    assert(shortestPathCost(x, z) == UINT32_MAX);

    // Test case 4: zero-weight edges
    Node p("P"), q("Q"), r("R");
    p.addNeighbor(q, 0);
    q.addNeighbor(r, 0);
    assert(shortestPathCost(p, r) == 0);

    // Test case 5: multiple neighbors, cycle
    Node c1("C1"), c2("C2"), c3("C3"), end("END");
    c1.addNeighbor(c2, 2);
    c2.addNeighbor(c1, 1);
    c2.addNeighbor(c3, 4);
    c3.addNeighbor(c2, 1);
    c3.addNeighbor(end, 6);
    assert(shortestPathCost(c1, end) == 12);

    // Test case 6: start equals finish
    Node self("SELF");
    assert(shortestPathCost(self, self) == 0);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
#include <cstdint>
#include <map>
#include <queue>
#include <vector>

class Node {
public:
    using weight_t = uint32_t;
    using key = Node*;
    using value = weight_t;
    using iterator = std::map<key, value>::iterator;
    using const_iterator = std::map<key, value>::const_iterator;

    explicit Node(std::string name) : name(std::move(name)) {}

    iterator begin() { return neighbors.begin(); }
    iterator end() { return neighbors.end(); }
    const_iterator begin() const { return neighbors.begin(); }
    const_iterator end() const { return neighbors.end(); }

    void addNeighbor(Node& node, weight_t wt) {
        if (!neighbors.count(&node)) {
            neighbors[&node] = wt;
        }
    }

    weight_t getWeight(Node& node) const {
        auto it = neighbors.find(&node);
        return (it != neighbors.end()) ? it->second : UINT32_MAX;
    }

    const std::string& getName() const { return name; }

private:
    std::string name;
    std::map<key, value> neighbors;
};

// Compute the shortest path cost from start to finish using Dijkstra's algorithm.
// Returns UINT32_MAX if finish is unreachable.
uint32_t shortestPathCost(const Node& start, const Node& finish) {
    const uint32_t INF = UINT32_MAX;

    // Distance map using node addresses as keys.
    std::map<const Node*, uint32_t> dist;
    std::set<const Node*> visited;

    dist[&start] = 0;

    // Min-heap: pair (distance, node pointer). Larger distance first for ordering.
    using QueueItem = std::pair<uint32_t, const Node*>;
    std::priority_queue<QueueItem, std::vector<QueueItem>, std::greater<QueueItem>> pq;
    pq.push({0, &start});

    while (!pq.empty()) {
        auto [currentDist, currentNode] = pq.top();
        pq.pop();

        if (currentNode == &finish) {
            return currentDist;
        }

        if (visited.count(currentNode)) {
            continue;
        }
        visited.insert(currentNode);

        for (const auto& [neighbor, weight] : *currentNode) {
            if (visited.count(neighbor)) {
                continue;
            }
            uint32_t newDist = currentDist + weight;
            if (newDist < dist[neighbor]) {
                dist[neighbor] = newDist;
                pq.push({newDist, neighbor});
            }
        }
    }

    return INF;
}
// The solution uses Dijkstra’s algorithm with a priority queue (min-heap) to efficiently find the shortest path. Initialize a distance map with all nodes set to infinity except the start node (distance 0). Use a `std::priority_queue` of pairs `(distance, node pointer)` ordered by distance. Repeatedly pop the node with the smallest tentative distance; if it has been processed already (visited set), skip it. Otherwise, mark it visited and relax all its outgoing edges: for each neighbor, if the new distance through the current node is smaller than the recorded distance, update the distance map and push the neighbor into the queue with the new distance. The algorithm terminates when the priority queue empties or the destination is popped (early exit). Edge cases: unreachable destination remains at `UINT32_MAX`; zero-weight cycles are handled by visited set; multiple edges are pre‑filtered by `addNeigbor` (only one edge per neighbor, so no conflict). Time complexity: \(O((V+E)\log V)\) for a graph with \(V\) nodes and \(E\) edges, dominated by heap operations. Space complexity: \(O(V)\) for distance, visited, and heap storage.
