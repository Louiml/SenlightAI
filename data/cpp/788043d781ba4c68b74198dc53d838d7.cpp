// Write a C++ function `bool canFinishCourses(int numCourses, const std::vector<std::pair<int,int>>& prerequisites)` that determines whether it is possible to finish all courses given `numCourses` courses labeled from `0` to `numCourses-1` and a list of prerequisite pairs where `prerequisites[i] = {a, b}` means course `b` must be taken before course `a`. The function should return `true` if there is no cyclic dependency preventing completion of all courses, and `false` otherwise. The input graph may be disconnected, may contain duplicate prerequisite pairs, and may have self-loops (which are always impossible to satisfy). The function should be const-correct, take the prerequisites vector by const reference, and handle edge cases such as zero courses (return `true`), an empty prerequisites list (return `true` if all courses can be taken independently), and a graph with a cycle (return `false`).
// The problem reduces to detecting whether the directed graph formed by courses and prerequisites contains a cycle. A standard approach is Kahn’s algorithm for topological sorting using indegree counting and a queue of nodes with zero indegree. First, initialize an indegree array of size `numCourses` to zero. Then, for each prerequisite pair `(a,b)`, increment the indegree of `a` (the course that depends on `b`) and store `a` in an adjacency list for `b`. Next, push all courses with indegree zero into a queue; these are courses that have no prerequisites and can be taken immediately. While the queue is not empty, pop a course, count it as completed, and for each of its outgoing neighbors (courses that depend on this course), decrement their indegree; if any neighbor’s indegree becomes zero, push it onto the queue. After processing, if the number of completed courses equals `numCourses`, a topological order exists and the answer is `true`; otherwise, there is a cycle. This handles duplicate pairs gracefully (they just increment the indegree multiple times, which is correct because the same dependency is listed twice, but if duplicate pairs are not supposed to affect the count, one could deduplicate, but here the given snippet counts duplicates, so we follow that). Self-loops (e.g., `{0,0}`) cause indefinite degree and will not be processable, returning `false`. The time complexity is O(V + E) where V is numCourses and E is the number of prerequisite pairs, and space is O(V + E) for the adjacency list and O(V) for the indegree array and queue.
#include <vector>
#include <queue>

// Determines if all courses can be finished given prerequisite constraints.
bool canFinishCourses(int numCourses, const std::vector<std::pair<int,int>>& prerequisites) {
    std::vector<int> indegree(numCourses, 0);
    std::vector<std::vector<int>> adjacency(numCourses);
    
    for (const auto& pair : prerequisites) {
        int course = pair.first;
        int prereq = pair.second;
        indegree[course]++;
        adjacency[prereq].push_back(course);
    }
    
    std::queue<int> ready;
    for (int i = 0; i < numCourses; ++i) {
        if (indegree[i] == 0) {
            ready.push(i);
        }
    }
    
    int completed = 0;
    while (!ready.empty()) {
        int current = ready.front();
        ready.pop();
        completed++;
        for (int next : adjacency[current]) {
            indegree[next]--;
            if (indegree[next] == 0) {
                ready.push(next);
            }
        }
    }
    
    return completed == numCourses;
}
#include <cassert>
#include <vector>
#include <utility>

// The solution function is declared above; this main tests it.
int main() {
    // Empty prerequisites, all courses independent
    assert(canFinishCourses(4, {}) == true);

    // Single linear chain: 0 -> 1 -> 2
    std::vector<std::pair<int,int>> linear = {{1,0}, {2,1}};
    assert(canFinishCourses(3, linear) == true);

    // Simple cycle: 0 requires 1, 1 requires 0
    std::vector<std::pair<int,int>> cycle = {{0,1}, {1,0}};
    assert(canFinishCourses(2, cycle) == false);

    // Disconnected with one cycle
    std::vector<std::pair<int,int>> mixed = {{1,0}, {2,3}, {3,2}};
    assert(canFinishCourses(4, mixed) == false);

    // Duplicate prerequisites (same pair twice) - still acyclic
    std::vector<std::pair<int,int>> duplicates = {{1,0}, {1,0}, {2,1}};
    assert(canFinishCourses(3, duplicates) == true);

    // Self-loop is impossible
    std::vector<std::pair<int,int>> selfLoop = {{2,2}};
    assert(canFinishCourses(3, selfLoop) == false);

    // Zero courses
    assert(canFinishCourses(0, {}) == true);

    // Larger acyclic graph with multiple roots
    std::vector<std::pair<int,int>> acyclic = {{2,0}, {2,1}, {3,2}, {4,2}};
    assert(canFinishCourses(5, acyclic) == true);

    // Larger graph with a cycle deep inside
    std::vector<std::pair<int,int>> deepCycle = {{1,0}, {2,1}, {3,2}, {1,3}, {4,0}};
    assert(canFinishCourses(5, deepCycle) == false);

    // Course with prerequisite not in range (should still handle gracefully if input valid? Here we assume valid input; but test a valid case)
    std::vector<std::pair<int,int>> validSingle = {{1,0}};
    assert(canFinishCourses(2, validSingle) == true);
}
