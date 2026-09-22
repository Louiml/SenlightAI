/*
Write a C++ function named `canFinishAllCourses` that takes an integer `numCourses` and a vector of pairs `prerequisites` (where each pair `{course, prerequisite}` means `course` depends on `prerequisite`) and returns `true` if it is possible to complete all courses given the prerequisite structure, otherwise `false`. The course numbers are integers from `0` to `numCourses - 1`. The function must detect whether a valid topological ordering of all courses exists; if there is any cycle in the prerequisite dependencies, it is impossible to finish all courses. The function should be robust to empty prerequisite lists, duplicate edges (though duplicates do not affect correctness), and self-loops (a course depending on itself). Input constraints: `numCourses` can be 0 or positive, and `prerequisites` may be empty. Use the standard library and do not modify the input vector.
*/

#include <vector>
#include <queue>

// Returns true if all courses can be completed given the prerequisite pairs.
// Each pair {course, prerequisite} means course depends on prerequisite.
bool canFinishAllCourses(int numCourses, const std::vector<std::pair<int, int>>& prerequisites) {
    // Handle edge case: no courses or no prerequisites.
    if (numCourses == 0) return true;
    
    // Build adjacency list: prerequisite -> [dependent courses]
    std::vector<std::vector<int>> adjacency(numCourses);
    std::vector<int> indegree(numCourses, 0);
    
    for (const auto& prereq : prerequisites) {
        int course = prereq.first;
        int prerequisite = prereq.second;
        adjacency[prerequisite].push_back(course);
        indegree[course]++;
    }
    
    // Queue of courses with no remaining prerequisites.
    std::queue<int> readyQueue;
    for (int i = 0; i < numCourses; ++i) {
        if (indegree[i] == 0) {
            readyQueue.push(i);
        }
    }
    
    int processed = 0;
    while (!readyQueue.empty()) {
        int current = readyQueue.front();
        readyQueue.pop();
        processed++;
        for (int dependent : adjacency[current]) {
            indegree[dependent]--;
            if (indegree[dependent] == 0) {
                readyQueue.push(dependent);
            }
        }
    }
    
    return processed == numCourses;
}

#include <cassert>
#include <vector>
#include <utility>

// The solution function is declared above (or included from header).
// Test cases for canFinishAllCourses.

int main() {
    // Case 1: Simple linear chain: 0 -> 1 -> 2
    {
        int numCourses = 3;
        std::vector<std::pair<int, int>> prerequisites = {{1, 0}, {2, 1}};
        assert(canFinishAllCourses(numCourses, prerequisites) == true);
    }
    
    // Case 2: Cycle: 0 -> 1, 1 -> 0
    {
        int numCourses = 2;
        std::vector<std::pair<int, int>> prerequisites = {{1, 0}, {0, 1}};
        assert(canFinishAllCourses(numCourses, prerequisites) == false);
    }
    
    // Case 3: No prerequisites
    {
        int numCourses = 4;
        std::vector<std::pair<int, int>> prerequisites = {};
        assert(canFinishAllCourses(numCourses, prerequisites) == true);
    }
    
    // Case 4: Self-loop
    {
        int numCourses = 1;
        std::vector<std::pair<int, int>> prerequisites = {{0, 0}};
        assert(canFinishAllCourses(numCourses, prerequisites) == false);
    }
    
    // Case 5: Duplicate edges (should still be possible)
    {
        int numCourses = 3;
        std::vector<std::pair<int, int>> prerequisites = {{1, 0}, {1, 0}, {2, 1}};
        assert(canFinishAllCourses(numCourses, prerequisites) == true);
    }
    
    // Case 6: Multiple independent chains
    {
        int numCourses = 5;
        std::vector<std::pair<int, int>> prerequisites = {{1, 0}, {3, 2}, {4, 3}};
        assert(canFinishAllCourses(numCourses, prerequisites) == true);
    }
    
    // Case 7: Cycle hidden among valid edges
    {
        int numCourses = 4;
        std::vector<std::pair<int, int>> prerequisites = {{1, 0}, {2, 1}, {1, 2}, {3, 1}};
        assert(canFinishAllCourses(numCourses, prerequisites) == false);
    }
    
    // Case 8: Zero courses
    {
        int numCourses = 0;
        std::vector<std::pair<int, int>> prerequisites = {};
        assert(canFinishAllCourses(numCourses, prerequisites) == true);
    }
    
    // Case 9: Single course without prerequisite
    {
        int numCourses = 1;
        std::vector<std::pair<int, int>> prerequisites = {};
        assert(canFinishAllCourses(numCourses, prerequisites) == true);
    }
    
    // Case 10: Disconnected but acyclic
    {
        int numCourses = 6;
        std::vector<std::pair<int, int>> prerequisites = {{1, 0}, {3, 2}, {4, 3}, {5, 4}};
        assert(canFinishAllCourses(numCourses, prerequisites) == true);
    }
    
    return 0;
}

// The problem is equivalent to detecting whether a directed graph (where nodes are courses and directed edges go from prerequisite to dependent course) has a topological ordering that includes all nodes. If such an ordering exists, all courses can be completed; otherwise, a cycle exists and completion is impossible. The solution uses Kahn’s algorithm (BFS-based topological sort). First, build an adjacency list where each prerequisite node points to all courses that directly depend on it. Compute the indegree (number of prerequisites) for each course. Initialize a queue with all courses that have indegree 0 (they have no prerequisites). Repeatedly dequeue a course, increment a counter of processed courses, and for each neighbor (course that depends on it), decrement the neighbor’s indegree; when that indegree becomes 0, push it into the queue. After processing, if the counter equals `numCourses`, a valid ordering exists; otherwise, there is a cycle. Edge cases: `numCourses` = 0 returns `true` (vacuously possible). Duplicate edges cause the indegree to count the same dependency multiple times, which is harmless as long as the adjacency list also has duplicates; however, the algorithm correctly decrements each time, so no issue. A self-loop (course depends on itself) creates a cycle and will be detected because the node will never reach indegree 0. Time complexity is O(N + P) where N is the number of courses and P is the number of prerequisite pairs (since each edge is visited once). Space complexity is O(N + P) for the adjacency list and indegree array.
