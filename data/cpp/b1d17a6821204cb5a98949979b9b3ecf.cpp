/*
Write a C++ function `computeMaxNodeOverlap` that takes a `Path` object and returns a `std::map<NodeId, size_t>` (or `std::vector` indexed by node ID) mapping each node that appears in the path to the maximum number of consecutive positions covered by the path on that node. A `Path` represents a walk through a directed graph (with nodes having string sequences), defined by a start position on its first node, a list of node IDs, and an end position on its last node. For internal nodes (neither first nor last), the path covers the entire node sequence length (from position 0 to length-1 inclusive, so `length` positions). For the first node, it covers from `startPosition` to the end of the node (so `length - startPosition` positions). For the last node (if different from first), it covers from position 0 to `endPosition` inclusive (so `endPosition + 1` positions). If the path has a single node, it covers from `startPosition` to `endPosition` inclusive (so `endPosition - startPosition + 1` positions). The function must handle paths that start and end on the same node, and must correctly compute the overlap for each distinct node ID that occurs in the path (a node ID may appear multiple times, but return the maximum overlap for that ID). Assume the `Path` class provides: `nodeIds()` returning `std::vector<NodeId>`, `numNodes()` returning `size_t`, `startPosition()` returning `int32_t`, `endPosition()` returning `int32_t`, `getNodeOverlapLengthByIndex(size_t)` returning `size_t` (but for simplicity, you may compute it from the graph if available, or assume a helper function `nodeLength(NodeId)` exists via `path.graphRawPtr()->nodeSeq(id).length()`). The task is to implement this function standalone, using only the public interface described, and not relying on any private or internal methods beyond those listed.
*/
#include <map>
#include <vector>
#include <cstdint>
#include <stdexcept>

// Forward declaration of Graph type (minimal interface)
namespace graphtools {
class Graph;
using NodeId = uint32_t;
}

// Minimal Path class interface (actual implementation would be provided)
class Path {
public:
    Path(graphtools::Graph* graph, int32_t start, const std::vector<graphtools::NodeId>& nodes, int32_t end)
        : graph_(graph), start_(start), nodes_(nodes), end_(end) {}

    const std::vector<graphtools::NodeId>& nodeIds() const { return nodes_; }
    size_t numNodes() const { return nodes_.size(); }
    int32_t startPosition() const { return start_; }
    int32_t endPosition() const { return end_; }
    const graphtools::Graph* graphRawPtr() const { return graph_; }

private:
    graphtools::Graph* graph_;
    int32_t start_;
    std::vector<graphtools::NodeId> nodes_;
    int32_t end_;
};

// Helper to get node sequence length (assumes graph provides nodeSeq)
namespace graphtools {
    inline size_t nodeLength(const Graph& graph, NodeId id) {
        return graph.nodeSeq(id).length();
    }
}

// Main function: returns map from node ID to maximum overlap length
std::map<graphtools::NodeId, size_t> computeMaxNodeOverlap(const Path& path) {
    std::map<graphtools::NodeId, size_t> overlapMap;
    const auto& nodes = path.nodeIds();
    const size_t n = path.numNodes();
    if (n == 0) return overlapMap;

    const auto& graph = *path.graphRawPtr();
    const graphtools::NodeId firstNode = nodes.front();
    const graphtools::NodeId lastNode = nodes.back();

    for (size_t i = 0; i < n; ++i) {
        const graphtools::NodeId id = nodes[i];
        size_t overlap = 0;
        if (n == 1) {
            // Single node: from startPosition to endPosition inclusive
            overlap = static_cast<size_t>(path.endPosition() - path.startPosition() + 1);
        } else if (i == 0) {
            // First node: from startPosition to end of node
            overlap = graphtools::nodeLength(graph, id) - static_cast<size_t>(path.startPosition());
        } else if (i == n - 1) {
            // Last node: from 0 to endPosition inclusive
            overlap = static_cast<size_t>(path.endPosition() + 1);
        } else {
            // Internal node: full length
            overlap = graphtools::nodeLength(graph, id);
        }
        // Update the maximum for this node ID
        auto it = overlapMap.find(id);
        if (it == overlapMap.end()) {
            overlapMap[id] = overlap;
        } else {
            it->second = std::max(it->second, overlap);
        }
    }
    return overlapMap;
}
#include <cassert>
#include <map>
#include <vector>
#include <cstdint>
#include <string>

// Minimal Graph implementation for testing
namespace graphtools {
class Graph {
public:
    Graph(const std::vector<std::string>& sequences) : seqs_(sequences) {}
    const std::string& nodeSeq(graphtools::NodeId id) const { return seqs_[id]; }
private:
    std::vector<std::string> seqs_;
};
}

// Include the solution (or paste here) — assuming computeMaxNodeOverlap is defined above.

int main() {
    // Graph with 4 nodes, each of length 5
    graphtools::Graph graph({"AAAAA", "CCCCC", "GGGGG", "TTTTT"});

    // Path: single node, start=1, end=3 => length 3
    {
        Path p(&graph, 1, {0}, 3);
        auto result = computeMaxNodeOverlap(p);
        assert(result.size() == 1);
        assert(result[0] == 3);
    }

    // Path: two nodes (0 then 1), start=2 on node0, end=4 on node1
    // Node0 overlap: 5-2=3; Node1 overlap: 4+1=5
    {
        Path p(&graph, 2, {0, 1}, 4);
        auto result = computeMaxNodeOverlap(p);
        assert(result.size() == 2);
        assert(result[0] == 3);
        assert(result[1] == 5);
    }

    // Path: three nodes (0,1,2), start=0, end=4 => full lengths for all
    {
        Path p(&graph, 0, {0, 1, 2}, 4);
        auto result = computeMaxNodeOverlap(p);
        assert(result.size() == 3);
        assert(result[0] == 5);
        assert(result[1] == 5);
        assert(result[2] == 5);
    }

    // Path with repeated node: {0,1,0}, start=0, end=3
    // Node0 appears twice: first time length 5, second time (last node) end+1=4 => max=5
    // Node1 internal length 5
    {
        Path p(&graph, 0, {0, 1, 0}, 3);
        auto result = computeMaxNodeOverlap(p);
        assert(result.size() == 2);
        assert(result[0] == 5);
        assert(result[1] == 5);
    }

    // Path where first and last are same node but path has multiple nodes (cycle)
    // {0,1,0}, start=2, end=4: first node overlap 3 (5-2), last node overlap 5 (4+1) => max 5
    {
        Path p(&graph, 2, {0, 1, 0}, 4);
        auto result = computeMaxNodeOverlap(p);
        assert(result.size() == 2);
        assert(result[0] == 5);
        assert(result[1] == 5);
    }

    // Path with single node start=end => length 1
    {
        Path p(&graph, 2, {2}, 2);
        auto result = computeMaxNodeOverlap(p);
        assert(result.size() == 1);
        assert(result[2] == 1);
    }

    return 0;
}
// The solution iterates over the path's node list, tracking the first and last node separately. For each node index, compute the overlap length: for index 0 (the first node), if there is more than one node, overlap is `nodeLength(first) - startPosition`; for the last index (if different from first), overlap is `endPosition + 1`; for internal nodes, overlap is the full node length. For the single-node case (first==last), overlap is `endPosition - startPosition + 1`. We maintain a `std::unordered_map<NodeId, size_t>` (or `std::map` for deterministic order) that stores the maximum overlap seen for each node ID. When a node ID repeats, we update the map with the maximum of the current and the newly computed overlap. The main edge cases are: path with one node, path with two nodes, path where first and last are the same node but path has multiple nodes (e.g., a cycle), and repeated node IDs. The algorithm runs in O(n) time where n is the number of nodes in the path, and O(k) space where k is the number of distinct node IDs. No special handling is needed for empty paths (numNodes() > 0 is assumed).
