// You are given a task inspired by LLVM's list scheduler: implement a function that performs top-down list scheduling on a simplified instruction dependency graph. The input is a directed acyclic graph (DAG) represented as an adjacency structure where each node has a unique integer ID in `[0, N)`, a non-negative latency, and a list of successor edges with latencies (the minimum number of cycles that must pass after scheduling the predecessor before the successor can begin). You must produce a schedule: an ordered sequence (a vector of node IDs, with the special value `-1` meaning a "noop" or idle cycle) that respects all dependencies (each successor must appear at a position whose cycle index is at least the predecessor's cycle index plus the edge latency) and minimizes the total number of cycles (the schedule length). The scheduler is greedy: at each cycle, among all nodes whose predecessors have all been scheduled and whose earliest start cycle is ≤ the current cycle, pick the one with the smallest ID (to make the result deterministic). If multiple are ready, pick the smallest ID. If none are ready, insert `-1` (noop) and advance to the next cycle. The first cycle is cycle 0. The function must return the sequence of node IDs (including `-1`s) that constitutes the schedule.

// The problem is a classic resource-constrained list scheduling variant, simplified to only dependency constraints. The main algorithm follows the LLVM top-down list scheduler structure:
// - Maintain a "ready" set (available queue) of nodes whose predecessors have all been scheduled.
// - For each cycle, move all nodes whose earliest start time has been reached from a pending set into the ready set.
// - Sort/pick from the ready set the node with the smallest ID, schedule it, update the earliest start times of its successors (each successor's earliest start = max(current, current + edge latency)), and decrement a predecessor count.
// - If the ready set is empty at a cycle, emit `-1` and advance the cycle; otherwise emit the chosen node and advance by 1 cycle (since each instruction takes 1 cycle; latency only affects when successors become available).
// - Continue until all N nodes are scheduled and no pending nodes remain.
//
// Edge cases: 
// - A node with no predecessors is ready at cycle 0.
// - Self-loops or cycles are not present (given DAG), but the algorithm should handle nodes that become ready exactly at the current cycle.
// - Multiple edges between the same pair of nodes: take the maximum latency for the dependency.
// - The final schedule may end with `-1`s only if there are dependent chains; but the loop exits once all nodes are scheduled, so trailing noops are not added.
// - The process terminates because each node is scheduled exactly once; the maximum number of cycles is bounded by the sum of latencies along the longest path, but the loop naturally exits.
//
// Time complexity: O(C * R + E) where C is number of cycles (bounded by sum of latencies, worst-case O(N*L) where L is max latency), R is the size of the ready set (bounded by N), and E is number of edges. For each cycle, we scan pending nodes to find ready ones. A more efficient implementation uses a heap keyed by earliest start time, but for N up to, say, 1000, a simple linear scan is acceptable. Space: O(N + E).

#include <vector>
#include <queue>
#include <algorithm>

// A node in the dependency graph.
struct SchedNode {
    int id;                 // unique node ID (0-based)
    int latency;            // operation latency (cycles after issue before result ready)
    std::vector<int> succ;  // successor node IDs
    std::vector<int> edgeLatency; // latency from this node to each successor
    int numPreds;           // number of predecessors (not counting edges from already-scheduled nodes)
    int earliestStart;      // earliest cycle this node can be scheduled
};

// Given the graph description, return the schedule as a vector of node IDs,
// with -1 meaning a noop (idle) cycle.
// The graph is provided as:
//   - n: number of nodes
//   - latencies: vector of size n, latencies[i] = latency of node i
//   - preds: vector of vectors, preds[i] contains the list of predecessor IDs of node i.
//           Each predecessor edge has latency given in the same index in predLatency[i].
//   - predLatency: vector of vectors, predLatency[i][j] is the latency of edge from preds[i][j] to i.
// The graph is a DAG. For each node i, the predecessors are guaranteed to have smaller IDs? Not necessarily.
std::vector<int> listScheduleTopDown(
    int n,
    const std::vector<int>& latencies,
    const std::vector<std::vector<int>>& preds,
    const std::vector<std::vector<int>>& predLatency)
{
    // Build adjacency: for each node, store its successors and edge latencies.
    std::vector<std::vector<int>> succList(n);
    std::vector<std::vector<int>> succLatency(n);
    std::vector<int> numPreds(n, 0);
    for (int i = 0; i < n; ++i) {
        numPreds[i] = static_cast<int>(preds[i].size());
        for (size_t j = 0; j < preds[i].size(); ++j) {
            int p = preds[i][j];
            succList[p].push_back(i);
            succLatency[p].push_back(predLatency[i][j]);
        }
    }

    // earliestStart[i] = earliest cycle node i can be scheduled.
    std::vector<int> earliestStart(n, 0);
    // Remaining unresolved predecessors.
    std::vector<int> remPreds = numPreds;

    // Priority queue for ready nodes: sorted by smallest ID (use min-heap).
    // We'll use a simple vector as "ready set" and pick min each cycle.
    std::vector<int> ready;

    // Initialize: all nodes with no predecessors are ready at cycle 0.
    for (int i = 0; i < n; ++i) {
        if (remPreds[i] == 0) {
            ready.push_back(i);
        }
    }

    std::vector<int> schedule;
    int curCycle = 0;
    int scheduledCount = 0;

    while (scheduledCount < n) {
        // If ready set empty, emit a noop and advance.
        if (ready.empty()) {
            schedule.push_back(-1);
            ++curCycle;
            continue;
        }

        // Pick the ready node with smallest ID.
        // Since ready is not sorted (might have been partially re-added), find min.
        auto minIt = std::min_element(ready.begin(), ready.end());
        int node = *minIt;
        ready.erase(minIt);

        // Schedule this node at current cycle.
        schedule.push_back(node);
        ++scheduledCount;
        // Update successors' earliest start times.
        for (size_t e = 0; e < succList[node].size(); ++e) {
            int succ = succList[node][e];
            int edgeLat = succLatency[node][e];
            // The successor can be scheduled at earliest curCycle + edgeLat, but
            // only after all its predecessors are scheduled. We'll decrement remPreds.
            earliestStart[succ] = std::max(earliestStart[succ], curCycle + edgeLat);
            --remPreds[succ];
            if (remPreds[succ] == 0) {
                // It becomes ready, but only if earliestStart[succ] <= curCycle? No,
                // it might not be ready yet. We'll add to a pending structure.
                // For simplicity, we'll add to ready only if earliestStart <= curCycle.
                // Otherwise we'll need a pending list. To keep it simple, we'll use
                // a pending vector and check at each cycle start.
            }
        }

        // Advance cycle (since each node takes 1 cycle).
        ++curCycle;

        // Actually, we need to handle the pending mechanism properly.
        // This simplified version would miss nodes that become ready but not yet ready.
        // Let's revert to a more faithful implementation.
    }

    // The above block is incomplete; let's implement correctly.
    // We'll redo the whole function with a pending queue approach.
    // (The scratch above is left as a placeholder; correct implementation follows.)
}

// Correct implementation:
std::vector<int> listScheduleCorrect(
    int n,
    const std::vector<int>& /*latencies*/,
    const std::vector<std::vector<int>>& preds,
    const std::vector<std::vector<int>>& predLatency)
{
    // Build successor lists.
    std::vector<std::vector<int>> succList(n);
    std::vector<std::vector<int>> succLatency(n);
    std::vector<int> numPreds(n);
    for (int i = 0; i < n; ++i) {
        numPreds[i] = static_cast<int>(preds[i].size());
        for (size_t j = 0; j < preds[i].size(); ++j) {
            int p = preds[i][j];
            succList[p].push_back(i);
            succLatency[p].push_back(predLatency[i][j]);
        }
    }

    std::vector<int> earliestStart(n, 0);
    std::vector<int> remPreds = numPreds;
    std::vector<int> pending;  // nodes whose preds are all scheduled but not yet ready
    std::vector<int> ready;    // nodes ready at current cycle

    // Initialize: nodes with no preds go to pending with earliestStart=0.
    for (int i = 0; i < n; ++i) {
        if (remPreds[i] == 0) {
            pending.push_back(i);
        }
    }

    std::vector<int> schedule;
    int curCycle = 0;
    int scheduledCount = 0;

    while (scheduledCount < n) {
        // Move from pending to ready when earliestStart <= curCycle.
        for (size_t i = 0; i < pending.size(); ++i) {
            if (earliestStart[pending[i]] <= curCycle) {
                ready.push_back(pending[i]);
                pending[i] = pending.back();
                pending.pop_back();
                --i;
            }
        }

        if (ready.empty()) {
            schedule.push_back(-1);
            ++curCycle;
            continue;
        }

        // Pick smallest ID from ready.
        auto minIt = std::min_element(ready.begin(), ready.end());
        int node = *minIt;
        ready.erase(minIt);

        schedule.push_back(node);
        ++scheduledCount;
        // Update successors.
        for (size_t e = 0; e < succList[node].size(); ++e) {
            int succ = succList[node][e];
            int edgeLat = succLatency[node][e];
            earliestStart[succ] = std::max(earliestStart[succ], curCycle + edgeLat);
            --remPreds[succ];
            if (remPreds[succ] == 0) {
                pending.push_back(succ);
            }
        }
        ++curCycle;
    }
    return schedule;
}

// The task asks for a descriptively named free function. We'll use listScheduleTopDown.
std::vector<int> listScheduleTopDown(
    int n,
    const std::vector<int>& latencies,
    const std::vector<std::vector<int>>& preds,
    const std::vector<std::vector<int>>& predLatency)
{
    // The latencies parameter is not directly used except for potential future needs,
    // but we ignore it per the problem statement (each instruction takes 1 cycle).
    // We call the correct implementation.
    return listScheduleCorrect(n, latencies, preds, predLatency);
}

#include <cassert>
#include <vector>

int main() {
    // Test 1: Simple chain 0->1 with latency 0; both take 1 cycle.
    // Expected: [0, 1]
    {
        int n = 2;
        std::vector<int> lat = {1, 1};
        std::vector<std::vector<int>> preds = {{}, {0}};
        std::vector<std::vector<int>> predLat = {{}, {0}};
        auto sched = listScheduleTopDown(n, lat, preds, predLat);
        std::vector<int> expected = {0, 1};
        assert(sched == expected);
    }

    // Test 2: Two independent nodes, smallest ID first.
    // Expected: [0, 1]
    {
        int n = 2;
        std::vector<int> lat = {1, 1};
        std::vector<std::vector<int>> preds = {{}, {}};
        std::vector<std::vector<int>> predLat = {{}, {}};
        auto sched = listScheduleTopDown(n, lat, preds, predLat);
        std::vector<int> expected = {0, 1};
        assert(sched == expected);
    }

    // Test 3: Chain with latency 2: 0 must be scheduled at cycle 0, 1 at cycle >= 2.
    // Since 1 can't be at cycle 1, we must insert a noop at cycle 1.
    // Expected: [0, -1, 1]  (noop at cycle 1, then 1 at cycle 2)
    {
        int n = 2;
        std::vector<int> lat = {1, 1};
        std::vector<std::vector<int>> preds = {{}, {0}};
        std::vector<std::vector<int>> predLat = {{}, {2}};
        auto sched = listScheduleTopDown(n, lat, preds, predLat);
        std::vector<int> expected = {0, -1, 1};
        assert(sched == expected);
    }

    // Test 4: Diamond: 0 -> 1, 0 -> 2, 1 -> 3, 2 -> 3, all latencies 0.
    // Ready order: 0, then after 0, both 1 and 2 are ready; pick 1 then 2; then 3.
    // Expected: [0, 1, 2, 3]
    {
        int n = 4;
        std::vector<int> lat = {1,1,1,1};
        std::vector<std::vector<int>> preds = {{}, {0}, {0}, {1,2}};
        std::vector<std::vector<int>> predLat = {{}, {0}, {0}, {0,0}};
        auto sched = listScheduleTopDown(n, lat, preds, predLat);
        std::vector<int> expected = {0, 1, 2, 3};
        assert(sched == expected);
    }

    // Test 5: Node with multiple predecessors, one has long latency causing noops.
    // 0->2 latency 3, 1->2 latency 0; 0 and 1 independent.
    // Expected schedule: cycle0:0, cycle1:1, cycle2:-, cycle3:-, cycle4:2 (since 2 earliest at max(0+3,1+0)=3, but we advance each cycle)
    // Actually after 0 at cycle0, 1 at cycle1, at cycle2 no ready, noop, cycle3 noop? At cycle3, 2's earliestStart is 0+3=3, so ready. So schedule: [0,1,-1,-1,2]
    {
        int n = 3;
        std::vector<int> lat = {1,1,1};
        std::vector<std::vector<int>> preds = {{}, {}, {0,1}};
        std::vector<std::vector<int>> predLat = {{}, {}, {3,0}};
        auto sched = listScheduleTopDown(n, lat, preds, predLat);
        std::vector<int> expected = {0, 1, -1, -1, 2};
        assert(sched == expected);
    }

    return 0;
}
