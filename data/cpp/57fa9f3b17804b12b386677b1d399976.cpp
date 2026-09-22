// Write a C++ function `computeExecutionOrder` that, given a vector of integers representing node processing durations (where each positive value means the node runs that many times sequentially) and a vector of dependencies (where each dependency is a pair of indices `{depender, dependee}` indicating that `depender` cannot start until `dependee` finishes), returns a vector of node indices in the order they would be scheduled according to the following rule: nodes with no pending dependencies are executed in parallel groups, and within each group, nodes are processed in order of increasing node index. The function should simulate a topological execution where each node's execution time is the sum of all its run-duration counts (i.e., a node with duration `k` takes `k` time units). Process time is discrete: at each time step, all ready nodes start and finish in that step (they do not block each other). The returned order should be the order in which nodes *complete* their execution. If the graph has a cycle, return an empty vector. The input is guaranteed to have valid indices: `0 <= duration[i] <= 1000`, and each node appears at most once as a key in the dependency list, but the same pair may appear multiple times (ignore duplicates). The graph has at most 100 nodes.  
// **Example:** `durations = {1,3,2}`, `dependencies = {{1,0},{2,0}}` → Node 0 runs 1 unit, completes at time 1. Both nodes 1 and 2 depend on node 0, so at time 1 they both start and finish: node 1 runs 3 units, node 2 runs 2 units, both complete at time 4. Order of completion: [0,1,2] (ties broken by node index).  
// **Your function signature:** `std::vector<int> computeExecutionOrder(const std::vector<int>& durations, const std::vector<std::pair<int,int>>& dependencies)`.  
// **Constraints:** You may modify the input containers or copy them. Use `const` where appropriate.  
// **Output:** Return the completion order as a vector of node indices. If cycle or invalid input, return `{}`.
// The solution uses a topological sort with a priority queue (min-heap) keyed by node index to decide the order in which nodes *become ready* and *complete*. Since all ready nodes execute in parallel and take their own durations, the completion time of a node is the maximum completion time of all its dependencies plus its own duration. However, because all ready nodes start at the same time step, we can process them in any order; the completion order is determined by the readiness order (ties by index). To handle dependencies correctly, we need to track in-degree (number of distinct dependencies) and out-edges. First, we build an adjacency list and an in-degree array from the dependencies, ignoring duplicates by using a `set` or checking. We also build a reverse dependency graph to compute the earliest start time for each node: the start time is the maximum of all dependency completion times. But since we only care about the order of completion (not actual times), we can simulate a queue: maintain a min-heap of all nodes with in-degree 0. Pop the smallest index, append it to the result, then for each neighbor, decrement its in-degree, and if it becomes 0, push it into the heap. This gives a topological order but not necessarily the completion order if there are dependencies with different durations? Let's reason: Because all ready nodes start at the same time and finish after their own duration, the completion order is not simply the topological order; it depends on durations. For example, node A (duration 10) and node B (duration 1) both become ready at time 0. They start at time 0 and finish at time 10 and 1 respectively. The completion order is B, A. Our heap-based topological sort would pop the smallest index first, but that might give A before B if A has index 0 and B index 1. So we need to simulate actual time.  
// **Correct approach:** Use Dijkstra-like scheduling. Maintain a priority queue of events `(completion_time, node_index)`. For each node, we need to know when all its dependencies have finished (i.e., max finishing time of dependencies). That can be computed by processing nodes in order of their readiness events. Initialize a `ready_time` for each node as 0. For each node with no dependencies, push `(duration, index)` into the min-heap (ordered by completion_time, then index). When we pop an event `(time, node)`, that node finishes at `time`; we add it to result. Then for each neighbor `v` that depends on this node, we update `v`'s `max_dependency_finish` to be `max(current, time)`. When the count of dependencies that have finished for `v` equals its in-degree, we compute `v`'s start time = the max of all its dependency finish times, and its completion time = start_time + duration[v], then push onto heap. In-degree is the number of distinct dependencies. To handle cycles, we track how many nodes we have processed; if less than total, return empty. Edge cases: duplicate edges, self-loops (cycle), nodes with zero duration (finish immediately). Time complexity: Building adjacency O(V+E) ignoring duplicates. Each node is pushed and popped once; heap operations O(log V). So O((V+E) log V). Space O(V+E).
#include <vector>
#include <queue>
#include <set>
#include <algorithm>
#include <limits>

/**
 * Compute the completion order of nodes in a dependency graph where all ready nodes run in parallel.
 * @param durations - execution duration (number of time units) for each node.
 * @param dependencies - pairs {depender, dependee} meaning depender cannot start until dependee finishes.
 * @return completion order of node indices, or empty vector if there is a cycle.
 */
std::vector<int> computeExecutionOrder(const std::vector<int>& durations,
                                       const std::vector<std::pair<int,int>>& dependencies) {
    int n = durations.size();
    if (n == 0) return {};

    std::vector<std::set<int>> adj(n);          // for each node, set of dependents (nodes that depend on it)
    std::vector<int> in_degree(n, 0);           // number of distinct dependencies for each node
    for (const auto& dep : dependencies) {
        int depender = dep.first;
        int dependee = dep.second;
        if (depender < 0 || depender >= n || dependee < 0 || dependee >= n) return {}; // invalid index
        if (depender == dependee) return {};    // self-loop is a cycle
        if (adj[dependee].insert(depender).second) {
            ++in_degree[depender];               // new distinct edge
        }
    }

    // Event queue: (completion_time, node_index), ordered by time then index.
    using Event = std::pair<long long, int>;
    std::priority_queue<Event, std::vector<Event>, std::greater<Event>> pq;

    std::vector<long long> max_dep_finish(n, 0); // earliest start time = max of all dependency finish times
    std::vector<int> finished_deps(n, 0);        // how many distinct dependencies have finished

    for (int i = 0; i < n; ++i) {
        if (in_degree[i] == 0) {
            pq.push({durations[i], i});
        }
    }

    std::vector<int> order;
    order.reserve(n);

    while (!pq.empty()) {
        auto [finish_time, node] = pq.top();
        pq.pop();
        order.push_back(node);

        // Update dependents
        for (int dependent : adj[node]) {
            max_dep_finish[dependent] = std::max(max_dep_finish[dependent], finish_time);
            if (++finished_deps[dependent] == in_degree[dependent]) {
                long long start = max_dep_finish[dependent];
                pq.push({start + durations[dependent], dependent});
            }
        }
    }

    // If not all nodes processed, there is a cycle
    if ((int)order.size() != n) {
        return {};
    }
    return order;
}
#include <cassert>
#include <vector>

// Function under test is assumed to be available from the solution above.
// We will include a minimal copy here for completeness (but in real answer, we just call the function).
// To keep the test self-contained, we assume the function is already defined elsewhere.
// We'll simply write the tests:

int main() {
    // Basic example from description
    std::vector<int> d1 = {1,3,2};
    std::vector<std::pair<int,int>> deps1 = {{1,0},{2,0}};
    assert(computeExecutionOrder(d1, deps1) == std::vector<int>({0,1,2}));

    // Nodes ready at same time, different durations -> shorter finishes first.
    std::vector<int> d2 = {10,1,5};
    std::vector<std::pair<int,int>> deps2 = {}; // all independent
    std::vector<int> result2 = computeExecutionOrder(d2, deps2);
    // Order should be 1 (time 1), 2 (time 5), 0 (time 10)
    assert(result2 == std::vector<int>({1,2,0}));

    // Chain: A<-B (B depends on A), and A has duration 5, B has 2, also C independent with duration 1.
    std::vector<int> d3 = {5,2,1};
    std::vector<std::pair<int,int>> deps3 = {{1,0}};
    // Time 0: A (finish 5), C (finish 1) start. C finishes at 1, A at 5. B starts after A finishes at 5, finishes at 7.
    // Completion order: C (1), A (5), B (7)
    assert(computeExecutionOrder(d3, deps3) == std::vector<int>({2,0,1}));

    // Cycle detection
    std::vector<int> d4 = {1,1};
    std::vector<std::pair<int,int>> deps4 = {{0,1},{1,0}};
    assert(computeExecutionOrder(d4, deps4).empty());

    // Duplicate edges should be ignored
    std::vector<int> d5 = {1,2};
    std::vector<std::pair<int,int>> deps5 = {{1,0},{1,0}};
    assert(computeExecutionOrder(d5, deps5) == std::vector<int>({0,1}));

    // Single node with zero duration
    std::vector<int> d6 = {0};
    std::vector<std::pair<int,int>> deps6 = {};
    assert(computeExecutionOrder(d6, deps6) == std::vector<int>({0}));

    // Node with self-loop -> cycle
    std::vector<int> d7 = {1,1};
    std::vector<std::pair<int,int>> deps7 = {{0,0}};
    assert(computeExecutionOrder(d7, deps7).empty());

    // Complex: 0 and 1 independent, 2 depends on both 0 and 1, 0 duration 3, 1 duration 1, 2 duration 4
    // Time 0: 0 (finish 3), 1 (finish 1). 1 finishes at 1, 0 finishes at 3. Then 2 starts at max(3,1)=3, finishes at 7.
    // Completion order: 1 (time1), 0 (time3), 2 (time7)
    std::vector<int> d8 = {3,1,4};
    std::vector<std::pair<int,int>> deps8 = {{2,0},{2,1}};
    assert(computeExecutionOrder(d8, deps8) == std::vector<int>({1,0,2}));

    // Large input performance sanity (100 nodes, linear chain). Should not crash.
    const int N = 100;
    std::vector<int> d9(N, 1);
    std::vector<std::pair<int,int>> deps9;
    for (int i = 1; i < N; ++i) deps9.push_back({i, i-1});
    std::vector<int> expected9;
    for (int i = 0; i < N; ++i) expected9.push_back(i);
    assert(computeExecutionOrder(d9, deps9) == expected9);

    return 0;
}
