// Given a directed graph represented as an adjacency list where each node has a set of integer-valued successor IDs and each node also carries a set of integer-valued "points-to" data, write a C++ function that implements a worklist-based fixed-point computation to propagate points-to sets along copy edges. Specifically, the graph is given as a `std::vector<std::pair<std::set<size_t>, std::set<size_t>>>` where each element represents a node: the first `set` is the node's current copy-successor edges (IDs of nodes to propagate to), and the second `set` is the node's current points-to set (IDs of abstract objects). The algorithm must repeatedly process nodes that have pending work: when a node is popped from the worklist, it must merge its current points-to set into each of its copy-successors' points-to sets, and if any successor's points-to set changes, that successor must be pushed back onto the worklist. The function must also deduplicate copy edges dynamically: if a node has duplicate successor IDs, remove duplicates. The worklist is a simple queue (FIFO) of node indices, and the algorithm terminates when the worklist is empty. The function signature is `void propagateCopyEdges(std::vector<std::pair<std::set<size_t>, std::set<size_t>>>& graph)`, and it modifies the graph in place. The input may have nodes with empty points-to sets or empty successor lists, and the graph may be large. The function must be correct for all such inputs.
The solution is a standard worklist algorithm for monotone dataflow analysis. We maintain a queue of node indices that need processing. Initially, we push all nodes that have a non-empty points-to set (since they might have successors to update). We also need a boolean or a counter to avoid processing nodes repeatedly unless their points-to set actually changes. The main loop pops a node index. For that node, we first deduplicate its copy-successor list (using a temporary set to collect unique successor IDs, then replace the node's successor set with the unique set). Then, for each unique successor, we compute the union of the successor's current points-to set with the current node's points-to set. If the union differs from the original (i.e., the successor's points-to set changed), we update the successor's set and push that successor onto the worklist (if not already queued, or we can allow duplicates and rely on a check inside the loop). To avoid infinite loops due to duplicate pushes, we can use a simple approach: just push every time a successor changes, and use a `processed` timestamp or a `changed` flag to skip re-processing nodes that haven't changed since last time. A simpler and correct approach is to maintain a `std::vector<bool> inWorklist` and only push a node if it's not already in the worklist. When we pop, we mark it as not in worklist. This guarantees each node is processed at most once per change in its own points-to set, but because changes propagate, a node might be processed multiple times overall, which is fine. The algorithm terminates because points-to sets only grow (monotonic) and are finite (bounded by the number of nodes in the graph). The number of times a node's points-to set can change is at most the total number of distinct elements that could ever be added, which is at most N (total number of nodes) per node, so worst-case O(N^2) updates overall. In practice, each edge processing is O(K) where K is the size of the points-to set, but we can assume sets have small sizes. Time complexity is O(E * N * S) where E is total copy edges (including dynamic additions, but each edge is only processed after a change to the source's points-to set), N is the number of nodes, and S is the average points-to set size. Space complexity is O(N + E) for the graph plus O(N) for the worklist and bookkeeping.
#include <vector>
#include <set>
#include <queue>

// Propagate points-to sets along copy edges in a fixed-point manner.
// The graph is a vector of (copy-successors, points-to set) pairs.
// Modifies the graph in place: successors' points-to sets are enlarged.
void propagateCopyEdges(std::vector<std::pair<std::set<size_t>, std::set<size_t>>>& graph) {
    size_t n = graph.size();
    if (n == 0) return;

    // Queue for worklist, plus a flag to avoid duplicate enqueueing.
    std::queue<size_t> worklist;
    std::vector<bool> inWorklist(n, false);

    // Initially, enqueue all nodes with non-empty points-to sets.
    for (size_t i = 0; i < n; ++i) {
        if (!graph[i].second.empty()) {
            worklist.push(i);
            inWorklist[i] = true;
        }
    }

    while (!worklist.empty()) {
        size_t current = worklist.front();
        worklist.pop();
        inWorklist[current] = false;

        auto& node = graph[current];
        auto& successors = node.first;
        auto& pts_to = node.second;

        // Deduplicate successor edges (they might contain duplicates).
        if (successors.size() > 1) {
            std::set<size_t> unique_succ(successors.begin(), successors.end());
            if (unique_succ.size() != successors.size()) {
                successors.swap(unique_succ);
            }
        }

        // Propagate points-to set to all successors.
        for (size_t succ : successors) {
            if (succ >= n) continue; // defensive
            auto& succ_pts = graph[succ].second;
            // Check if there is any new element to add.
            bool changed = false;
            for (size_t val : pts_to) {
                if (succ_pts.insert(val).second) {
                    changed = true;
                }
            }
            if (changed && !inWorklist[succ]) {
                worklist.push(succ);
                inWorklist[succ] = true;
            }
        }
    }
}
#include <cassert>
#include <vector>
#include <set>
#include <utility>

// Include the solution function here (or link to it).
// For standalone testing, copy the function definition above.

int main() {
    // Test 1: Simple chain 0 -> 1 with pts_to on 0.
    std::vector<std::pair<std::set<size_t>, std::set<size_t>>> g1;
    g1.push_back({{1}, {10}}); // node 0
    g1.push_back({{}, {}});    // node 1
    propagateCopyEdges(g1);
    assert(g1[0].second == std::set<size_t>({10}));
    assert(g1[1].second == std::set<size_t>({10}));

    // Test 2: Cycle 0 -> 1 -> 0, both initially empty, then node 0 gets points-to via external? Not possible, but ensure no infinite loop.
    std::vector<std::pair<std::set<size_t>, std::set<size_t>>> g2;
    g2.push_back({{1}, {}});
    g2.push_back({{0}, {}});
    propagateCopyEdges(g2);
    assert(g2[0].second.empty());
    assert(g2[1].second.empty());

    // Test 3: Duplicate edges and multiple propagation.
    std::vector<std::pair<std::set<size_t>, std::set<size_t>>> g3;
    g3.push_back({{1,1,2}, {1,2,3}}); // node 0 has duplicate succ 1
    g3.push_back({{}, {}});            // node 1
    g3.push_back({{1}, {}});           // node 2
    propagateCopyEdges(g3);
    assert(g3[0].second == std::set<size_t>({1,2,3}));
    assert(g3[1].second == std::set<size_t>({1,2,3}));
    assert(g3[2].second == std::set<size_t>({1,2,3}));
    // Also check that duplicate edges were removed (set size is 2 for node 0)
    assert(g3[0].first.size() == 2);

    // Test 4: Larger graph with intermediate node.
    std::vector<std::pair<std::set<size_t>, std::set<size_t>>> g4;
    g4.push_back({{1}, {5}});    // 0
    g4.push_back({{2}, {}});     // 1
    g4.push_back({{3}, {}});     // 2
    g4.push_back({{}, {}});      // 3
    propagateCopyEdges(g4);
    assert(g4[3].second == std::set<size_t>({5}));

    // Test 5: No initial points-to, no changes.
    std::vector<std::pair<std::set<size_t>, std::set<size_t>>> g5;
    g5.push_back({{1}, {}});
    g5.push_back({{}, {}});
    propagateCopyEdges(g5);
    assert(g5[0].second.empty());
    assert(g5[1].second.empty());

    // Test 6: Empty graph.
    std::vector<std::pair<std::set<size_t>, std::set<size_t>>> g6;
    propagateCopyEdges(g6); // should not crash

    // Test 7: Self-loop with non-empty points-to.
    std::vector<std::pair<std::set<size_t>, std::set<size_t>>> g7;
    g7.push_back({{0}, {42}});
    propagateCopyEdges(g7);
    assert(g7[0].second == std::set<size_t>({42}));

    // Test 8: Branching and merging.
    std::vector<std::pair<std::set<size_t>, std::set<size_t>>> g8;
    g8.push_back({{2}, {1}});      // 0 -> 2
    g8.push_back({{2}, {2}});      // 1 -> 2
    g8.push_back({{}, {}});        // 2
    propagateCopyEdges(g8);
    assert(g8[2].second == std::set<size_t>({1,2}));

    // Test 9: Node with many successors, ensure all get data.
    std::vector<std::pair<std::set<size_t>, std::set<size_t>>> g9;
    g9.push_back({{1,2,3,4}, {7}});
    for (int i = 1; i <= 4; ++i) g9.push_back({{}, {}});
    propagateCopyEdges(g9);
    assert(g9[1].second == std::set<size_t>({7}));
    assert(g9[2].second == std::set<size_t>({7}));
    assert(g9[3].second == std::set<size_t>({7}));
    assert(g9[4].second == std::set<size_t>({7}));

    // Test 10: Indirect cycle with initial data on one node, verify fixed point.
    std::vector<std::pair<std::set<size_t>, std::set<size_t>>> g10;
    g10.push_back({{1}, {99}}); // 0 -> 1
    g10.push_back({{2}, {}});   // 1 -> 2
    g10.push_back({{0}, {}});   // 2 -> 0
    propagateCopyEdges(g10);
    assert(g10[0].second == std::set<size_t>({99}));
    assert(g10[1].second == std::set<size_t>({99}));
    assert(g10[2].second == std::set<size_t>({99}));

    return 0;
}
