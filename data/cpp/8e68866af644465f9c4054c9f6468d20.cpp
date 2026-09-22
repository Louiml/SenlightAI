Write a C++ function named `canCompleteAllCourses` that takes an integer `numCourses` and a vector of prerequisite pairs `prerequisites`, where each pair `[a, b]` means course `b` must be completed before course `a`. The function must return `true` if it is possible to finish all courses (i.e., the course dependency graph has no cycle), and `false` otherwise. The input may contain duplicate prerequisite pairs, and `numCourses` is guaranteed to be non-negative. If `numCourses` is zero or there are no prerequisites, the function should return `true`.

The problem reduces to detecting whether a directed graph (built from the prerequisites) contains a cycle. Use Kahn’s algorithm (BFS-based topological sort). First, build an adjacency list where for each pair `[a, b]`, we add an edge from `b` to `a` (since `b` must be completed before `a`). Then compute the in-degree (number of incoming edges) for each node. Initialize a queue with all nodes having in-degree zero (courses that can be taken immediately). Process nodes from the queue: for each popped node, decrement the in-degree of all its neighbors; if a neighbor's in-degree becomes zero, push it into the queue. Count how many nodes have been processed. If the count equals `numCourses`, then all courses can be taken (no cycle); otherwise a cycle prevents completion. Edge cases: empty prerequisites (returns true), single course without dependencies (true), and a self-loop (false). Time complexity is O(V + E) where V = numCourses and E = number of prerequisite pairs. Space complexity is O(V + E) for the adjacency list and in-degree array.

#include <vector>
#include <queue>
#include <cstddef>

// Returns true if all courses can be finished given prerequisite pairs.
// Each pair {a, b} means course b must be taken before course a.
bool canCompleteAllCourses(int numCourses, const std::vector<std::vector<int>>& prerequisites) {
    // Build adjacency list: edge from prerequisite to dependent course
    std::vector<std::vector<int>> adj(numCourses);
    for (const auto& prereq : prerequisites) {
        // prereq[0] = course that needs prereq[1] as prerequisite
        adj[prereq[1]].push_back(prereq[0]);
    }

    // Compute in-degrees
    std::vector<int> indegree(numCourses, 0);
    for (int i = 0; i < numCourses; ++i) {
        for (int neighbor : adj[i]) {
            indegree[neighbor]++;
        }
    }

    // Initialize queue with nodes having zero in-degree
    std::queue<int> q;
    for (int i = 0; i < numCourses; ++i) {
        if (indegree[i] == 0) {
            q.push(i);
        }
    }

    // Process nodes in topological order
    int processed = 0;
    while (!q.empty()) {
        int course = q.front();
        q.pop();
        ++processed;
        for (int neighbor : adj[course]) {
            --indegree[neighbor];
            if (indegree[neighbor] == 0) {
                q.push(neighbor);
            }
        }
    }

    // All courses processed => no cycle
    return processed == numCourses;
}

#include <cassert>
#include <vector>

int main() {
    // Case 1: No prerequisites
    assert(canCompleteAllCourses(3, {}) == true);

    // Case 2: Simple linear dependency
    std::vector<std::vector<int>> prereq1 = {{1, 0}};
    assert(canCompleteAllCourses(2, prereq1) == true);

    // Case 3: Cycle (0 -> 1 -> 0)
    std::vector<std::vector<int>> prereq2 = {{1, 0}, {0, 1}};
    assert(canCompleteAllCourses(2, prereq2) == false);

    // Case 4: Self-loop
    std::vector<std::vector<int>> prereq3 = {{0, 0}};
    assert(canCompleteAllCourses(1, prereq3) == false);

    // Case 5: Larger DAG (multiple paths, no cycle)
    std::vector<std::vector<int>> prereq4 = {{1, 0}, {2, 0}, {3, 1}, {3, 2}};
    assert(canCompleteAllCourses(4, prereq4) == true);

    // Case 6: Disconnected components with a cycle in one component
    std::vector<std::vector<int>> prereq5 = {{1, 0}, {0, 1}, {2, 3}};
    assert(canCompleteAllCourses(4, prereq5) == false);

    // Case 7: Zero courses
    assert(canCompleteAllCourses(0, {}) == true);

    // Case 8: Duplicate edges (still no cycle)
    std::vector<std::vector<int>> prereq6 = {{1, 0}, {1, 0}};
    assert(canCompleteAllCourses(2, prereq6) == true);

    // Case 9: Long chain that forms no cycle
    std::vector<std::vector<int>> prereq7 = {{1, 0}, {2, 1}, {3, 2}};
    assert(canCompleteAllCourses(4, prereq7) == true);

    // Case 10: Cycle in the middle
    std::vector<std::vector<int>> prereq8 = {{1, 0}, {2, 1}, {1, 2}};
    assert(canCompleteAllCourses(3, prereq8) == false);
}
