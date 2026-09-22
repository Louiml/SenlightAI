// Write a C++ function `bool canFinishAllCourses(int numCourses, const std::vector<std::vector<int>>& prerequisites)` that determines whether it is possible to finish all courses given the number of courses and a list of prerequisite pairs. Each prerequisite is a pair `[course, prerequisite]` meaning that `course` cannot be taken until `prerequisite` is completed. The function should return `true` if there exists a valid order to take all courses without violating any prerequisite, and `false` otherwise. The graph may contain cycles, duplicate prerequisites, and courses with no prerequisites. You may assume `numCourses >= 0` and that course IDs are integers between `0` and `numCourses-1`. The function must be `const`-correct and should not modify the input.
#include <cassert>
#include <vector>

int main() {
    // Test 1: Simple chain 0 -> 1 -> 2
    assert(canFinishAllCourses(3, {{1, 0}, {2, 1}}) == true);
    
    // Test 2: Cycle 0 <-> 1
    assert(canFinishAllCourses(2, {{0, 1}, {1, 0}}) == false);
    
    // Test 3: No prerequisites
    assert(canFinishAllCourses(4, {}) == true);
    
    // Test 4: Single course with no prerequisites
    assert(canFinishAllCourses(1, {}) == true);
    
    // Test 5: Duplicate prerequisites and cycle
    assert(canFinishAllCourses(3, {{0, 1}, {0, 1}, {1, 0}, {2, 0}}) == false);
    
    // Test 6: Disconnected graph with no cycles
    assert(canFinishAllCourses(5, {{1, 0}, {3, 2}, {4, 2}}) == true);
    
    // Test 7: Self-loop (course requires itself)
    assert(canFinishAllCourses(2, {{0, 0}}) == false);
    
    // Test 8: Multiple dependencies
    assert(canFinishAllCourses(4, {{3, 0}, {3, 1}, {3, 2}}) == true);
    
    // Test 9: Large cycle involving all nodes
    assert(canFinishAllCourses(4, {{1, 0}, {2, 1}, {3, 2}, {0, 3}}) == false);
    
    // Test 10: All courses independent after processing
    assert(canFinishAllCourses(3, {{0, 2}, {1, 2}}) == true);
    
    return 0;
}
#include <vector>
#include <queue>

// Returns true if all courses can be finished given the prerequisite pairs.
bool canFinishAllCourses(int numCourses, const std::vector<std::vector<int>>& prerequisites) {
    // Build adjacency list: adj[prereq] stores list of courses that require prereq.
    std::vector<std::vector<int>> adj(numCourses);
    for (const auto& pair : prerequisites) {
        int course = pair[0];
        int prereq = pair[1];
        adj[prereq].push_back(course);
    }
    
    // Compute in-degree (number of prerequisites) for each course.
    std::vector<int> inDegree(numCourses, 0);
    for (const auto& pair : prerequisites) {
        inDegree[pair[0]]++;
    }
    
    // Initialize queue with all courses having zero prerequisites.
    std::queue<int> q;
    for (int i = 0; i < numCourses; ++i) {
        if (inDegree[i] == 0) {
            q.push(i);
        }
    }
    
    // Process courses in topological order.
    int processed = 0;
    while (!q.empty()) {
        int course = q.front();
        q.pop();
        ++processed;
        
        for (int dependent : adj[course]) {
            --inDegree[dependent];
            if (inDegree[dependent] == 0) {
                q.push(dependent);
            }
        }
    }
    
    // If we processed all courses, no cycle exists; otherwise, cycle prevents completion.
    return processed == numCourses;
}
// This is a classic cycle-detection problem on a directed graph, solvable via **Kahn’s algorithm** (BFS-based topological sort). Build an adjacency list where each course points to the courses that require it as a prerequisite (i.e., `adj[prereq]` contains `course`). Compute the in-degree (number of prerequisites) for each course. Initialize a queue with all courses having in-degree 0. Repeatedly remove a course from the queue, add it to a sorted list, and for each course that depends on it, decrement that dependent's in-degree; if it becomes 0, push it into the queue. If the number of processed courses equals `numCourses`, then no cycle exists and all courses can be finished; otherwise, a cycle prevents finishing. Edge cases: empty prerequisite list (`numCourses` courses with no dependencies) always returns `true`; `numCourses` of 0 or 1 trivially returns `true`; duplicate prerequisites are handled naturally because we process each edge once when building adjacency. Time complexity is O(V + E) where V is number of courses and E is number of prerequisite pairs. Space complexity is O(V + E) for adjacency list, in-degree array, and queue.
