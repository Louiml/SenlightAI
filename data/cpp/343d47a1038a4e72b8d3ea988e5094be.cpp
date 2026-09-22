Write a C++ function `int maxMatching(const std::vector<std::vector<bool>>& graph)` that, given a bipartite graph with `M` applicants (rows) and `N` jobs (columns), returns the maximum number of applicants that can be matched to distinct jobs (i.e., the size of the maximum bipartite matching). The input `graph` is a rectangular boolean matrix where `graph[a][j] == true` means applicant `a` is qualified for job `j`. The function must handle graphs where `M` and `N` are not necessarily equal, and where some rows or columns may contain no `true` values. The function should NOT print anything; it should only return the count. Use a standard augmenting-path based algorithm (like the Kuhn / Hungarian DFS approach) to find the maximum matching.
// The problem is classic maximum bipartite matching. The solution uses a greedy assignment attempt for each applicant with backtracking via DFS. For each applicant `a` from 0 to M-1, we run a DFS that tries to assign an available job `j` such that `graph[a][j] == true` and `j` is not yet visited in this attempt. If job `j` is free (`match[j] == -1`) or we can reassign the applicant currently holding `j` to another job via recursion, we assign `match[j] = a` and increment the count. The visited array prevents infinite recursion in the same DFS attempt. Important edge cases: an applicant with no eligible jobs, a job with no applicants, or when M and N differ (the algorithm naturally stops once all applicants are processed; if M > N, the maximum matching is at most N). The algorithm’s worst-case time complexity is O(M * N * E) where E is the number of edges, but in practice for dense graphs it’s O(M^2 * N) due to recursive calls; space complexity is O(N + M) for the match and visited arrays.
#include <vector>
#include <cstring>

// Recursive DFS to find an augmenting path for applicant 'applicant'.
bool tryBpm(const std::vector<std::vector<bool>>& graph,
            int applicant,
            std::vector<int>& match,  // match[job] = applicant or -1
            std::vector<bool>& visited) {
    const int N = static_cast<int>(graph[0].size());
    for (int job = 0; job < N; ++job) {
        if (graph[applicant][job] && !visited[job]) {
            visited[job] = true;
            if (match[job] == -1 || tryBpm(graph, match[job], match, visited)) {
                match[job] = applicant;
                return true;
            }
        }
    }
    return false;
}

// Returns the maximum number of applicants that can be matched to distinct jobs.
int maxMatching(const std::vector<std::vector<bool>>& graph) {
    if (graph.empty() || graph[0].empty()) return 0;
    const int M = static_cast<int>(graph.size());
    const int N = static_cast<int>(graph[0].size());

    std::vector<int> match(N, -1);
    int count = 0;

    for (int applicant = 0; applicant < M; ++applicant) {
        std::vector<bool> visited(N, false);
        if (tryBpm(graph, applicant, match, visited)) {
            ++count;
        }
    }
    return count;
}
#include <cassert>
#include <vector>

// The solution function is declared above.
int main() {
    // Original example: 6x6 bipartite graph from the snippet
    std::vector<std::vector<bool>> g1 = {
        {0, 1, 1, 0, 0, 0},
        {1, 0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0, 0},
        {0, 0, 1, 1, 0, 0},
        {0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 1}
    };
    assert(maxMatching(g1) == 5);

    // All qualified: each applicant can take any job, max = min(M,N) = 3
    std::vector<std::vector<bool>> g2 = {
        {1, 1, 1},
        {1, 1, 1},
        {1, 1, 1}
    };
    assert(maxMatching(g2) == 3);

    // No edges
    std::vector<std::vector<bool>> g3 = {
        {0, 0, 0},
        {0, 0, 0}
    };
    assert(maxMatching(g3) == 0);

    // Single applicant, two jobs
    std::vector<std::vector<bool>> g4 = {{1, 0, 1}};
    assert(maxMatching(g4) == 1);

    // Two applicants, one job both want
    std::vector<std::vector<bool>> g5 = {{1}, {1}};
    assert(maxMatching(g5) == 1);

    // More applicants than jobs (M=4, N=2) – all connected
    std::vector<std::vector<bool>> g6 = {
        {1, 1},
        {1, 1},
        {1, 1},
        {1, 1}
    };
    assert(maxMatching(g6) == 2);

    // Empty graph edge case
    std::vector<std::vector<bool>> g7 = {};
    assert(maxMatching(g7) == 0);

    // Singleton job and applicant
    std::vector<std::vector<bool>> g8 = {{1}};
    assert(maxMatching(g8) == 1);

    return 0;
}
