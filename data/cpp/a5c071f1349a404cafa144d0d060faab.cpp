Write a C++ function that simulates a simplified "course prerequisite" analysis. Given a list of course names, a list of target courses with their direct prerequisite lists, and a set of initially taken courses, determine for each target course the number of direct prerequisites that remain "critical" after considering transitive prerequisite dependencies. A direct prerequisite is critical if it is the only remaining path to satisfy that prerequisite, meaning that if you remove that prerequisite, the target course cannot be completed even with indirect prerequisites. The function should take the list of course names, the target list (with their prerequisite lists), and the initial taken courses as parameters, and return a vector of pairs (course name, count, list of critical prerequisite names) in the same order as the targets. The input will have unique course names, no cycles in the prerequisite graph, and prerequisites are sorted alphabetically. For each target, count how many of its direct prerequisites are not implied by any combination of other prerequisites (either directly or transitively) among the taken courses. In practice, a direct prerequisite is non-critical if it can be reached through another direct prerequisite's chain. The output should list the critical direct prerequisites in alphabetical order.
// The problem reduces to, for each target course, identifying which of its direct prerequisites are "covered" by other direct prerequisites. If prerequisite A can be reached (directly or transitively) from prerequisite B (where B is also a direct prerequisite of the target), then A is redundant and should not be counted as critical. The algorithm: for each target, build a graph of all courses reachable from each direct prerequisite. Then for each direct prerequisite, perform a BFS/DFS from that prerequisite. If during the traversal we encounter another direct prerequisite (that is not the starting one), then that other prerequisite is marked as non-critical because it can be satisfied by following the chain from the starting prerequisite. We repeat for every starting prerequisite. At the end, prerequisites that were never marked as non-critical are critical. Edge cases: empty prerequisite list (then count 0 and no names), a prerequisite that is also in the initial taken list? The problem statement implies the initial taken courses might be taken as given, but the task does not explicitly ask to consider taken courses; actually re-reading the task: it says "given a set of initially taken courses" but the code snippet does not include that. Since the snippet is what we base this on, we interpret "initially taken" as the direct prerequisites themselves—the target's prerequisites are the "taken" set. So the task is exactly what the snippet does: for each target, among its direct prerequisites, find which ones are essential (not reachable from any other direct prerequisite through transitive dependencies). Complexity: For each target with d direct prerequisites, we run BFS from each, visiting at most all k courses, so O(k * d) per target, and overall O(j * k * max_d) worst case, which is O(j * k^2) if d can be O(k). Space is O(k) for visited arrays and queue.
#include <vector>
#include <string>
#include <map>
#include <queue>
#include <algorithm>

// Result type: for each target, the course name, critical count, and list of critical prerequisites
struct CriticalResult {
    std::string course;
    int count;
    std::vector<std::string> prerequisites;
};

// Determine critical direct prerequisites for each target course.
// courseNames: all unique course names (sorted alphabetically)
// targets: indices of target courses in courseNames
// prereqs: for each course index, a sorted vector of direct prerequisite indices
// Returns results in the same order as targets.
std::vector<CriticalResult> findCriticalPrereqs(
    const std::vector<std::string>& courseNames,
    const std::vector<int>& targets,
    const std::vector<std::vector<int>>& prereqs) {
    
    int k = courseNames.size();
    std::vector<CriticalResult> results;
    
    for (int targetIdx : targets) {
        const auto& direct = prereqs[targetIdx];
        int d = direct.size();
        
        // isCritical[p] starts true for all direct prerequisites, later set false if reachable from another
        std::vector<bool> isCritical(d, true);
        
        // For each direct prerequisite as a starting point, BFS to see which other direct prerequisites are reachable
        for (int startPos = 0; startPos < d; ++startPos) {
            int start = direct[startPos];
            std::vector<bool> visited(k, false);
            std::queue<int> q;
            q.push(start);
            visited[start] = true;
            
            while (!q.empty()) {
                int cur = q.front();
                q.pop();
                for (int next : prereqs[cur]) {
                    if (!visited[next]) {
                        visited[next] = true;
                        q.push(next);
                        // If this next is another direct prerequisite (not the start itself)
                        // but we haven't marked it non-critical yet from another path
                        for (int p = 0; p < d; ++p) {
                            if (p != startPos && direct[p] == next) {
                                isCritical[p] = false;
                            }
                        }
                    }
                }
            }
        }
        
        // Collect critical prerequisites
        std::vector<std::string> criticalNames;
        int count = 0;
        for (int i = 0; i < d; ++i) {
            if (isCritical[i]) {
                criticalNames.push_back(courseNames[direct[i]]);
                ++count;
            }
        }
        // They are already in sorted order because direct is sorted and we iterate in order
        results.push_back({courseNames[targetIdx], count, criticalNames});
    }
    return results;
}
#include <cassert>
#include <vector>
#include <string>

// Include the solution above (or copy here)

int main() {
    // Case 1: Simple chain: A -> B -> C, target C has direct prerequisites A and B
    std::vector<std::string> names = {"A", "B", "C"};
    std::vector<std::vector<int>> prereqs(3);
    prereqs[0] = {}; // A has none
    prereqs[1] = {0}; // B requires A
    prereqs[2] = {0, 1}; // C requires A and B
    std::vector<int> targets = {2};
    auto res = findCriticalPrereqs(names, targets, prereqs);
    assert(res.size() == 1);
    assert(res[0].course == "C");
    assert(res[0].count == 1);
    assert(res[0].prerequisites == std::vector<std::string>{"B"});

    // Case 2: Independent prerequisites, all critical
    std::vector<std::string> names2 = {"X", "Y", "Z"};
    std::vector<std::vector<int>> prereqs2(3);
    prereqs2[0] = {}; // X
    prereqs2[1] = {}; // Y
    prereqs2[2] = {0, 1}; // Z requires X and Y
    std::vector<int> targets2 = {2};
    auto res2 = findCriticalPrereqs(names2, targets2, prereqs2);
    assert(res2[0].count == 2);
    assert(res2[0].prerequisites == (std::vector<std::string>{"X", "Y"}));

    // Case 3: Cyclic? No, but test a diamond: A -> B, A -> C, C -> D, B -> D, target D has direct B and C
    // Both B and C are critical because B cannot reach C and C cannot reach B
    std::vector<std::string> names3 = {"A", "B", "C", "D"};
    std::vector<std::vector<int>> prereqs3(4);
    prereqs3[0] = {}; // A
    prereqs3[1] = {0}; // B requires A
    prereqs3[2] = {0}; // C requires A
    prereqs3[3] = {1, 2}; // D requires B and C
    std::vector<int> targets3 = {3};
    auto res3 = findCriticalPrereqs(names3, targets3, prereqs3);
    assert(res3[0].count == 2);
    assert(res3[0].prerequisites == (std::vector<std::string>{"B", "C"}));

    // Case 4: Target with no prerequisites
    std::vector<std::string> names4 = {"Only"};
    std::vector<std::vector<int>> prereqs4(1);
    prereqs4[0] = {};
    std::vector<int> targets4 = {0};
    auto res4 = findCriticalPrereqs(names4, targets4, prereqs4);
    assert(res4[0].count == 0);
    assert(res4[0].prerequisites.empty());

    // Case 5: Multiple targets
    std::vector<std::string> names5 = {"A", "B", "C", "D"};
    std::vector<std::vector<int>> prereqs5(4);
    prereqs5[0] = {}; // A
    prereqs5[1] = {0}; // B requires A
    prereqs5[2] = {1}; // C requires B (so A is reachable from B)
    prereqs5[3] = {0, 2}; // D requires A and C; C requires B, and B requires A, so can C be reached from A? No, A has no prereqs. From C we can reach B and A, so A is non-critical when starting from C. So only C is critical.
    std::vector<int> targets5 = {3};
    auto res5 = findCriticalPrereqs(names5, targets5, prereqs5);
    assert(res5[0].count == 1);
    assert(res5[0].prerequisites == (std::vector<std::string>{"C"}));

    return 0;
}
