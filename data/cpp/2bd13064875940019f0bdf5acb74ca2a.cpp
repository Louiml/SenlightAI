// Write a C++ function `countUniqueMatches` that takes two vectors of integers representing feature indices (e.g., keypoint indices in two images) and returns the number of unique matches, where a match is defined as a pair `(a, b)` with `a` from the first vector and `b` from the second vector. However, each index from the first vector can be matched to at most one index from the second vector, and vice versa (i.e., a bijective matching must be enforced). The function should consider pairs where `a == b` (i.e., the same index value) as valid matches, but duplicates are not allowed: if the same pair appears more than once, it counts only once. The function must handle empty inputs gracefully (returning 0) and ensure the matching is maximal—i.e., it returns the maximum number of such unique, bijective matches possible given the input pairs. You may assume the vectors are the same length, but each vector may contain repeated indices. The goal is to compute the size of a maximum matching in a bipartite graph where left vertices are distinct values from the first vector and right vertices are distinct values from the second vector, and edges exist exactly for pairs `(a, b)` that appear in the input. Return the maximum cardinality of such a matching.

#include <cassert>
#include <vector>

int countUniqueMatches(const std::vector<int>& left_indices, const std::vector<int>& right_indices);

int main() {
    // Simple distinct pairs
    assert(countUniqueMatches({1,2,3}, {1,2,3}) == 3);
    // Duplicate edges: pair (1,0) appears twice, but only one match possible
    assert(countUniqueMatches({1,1,2}, {0,0,1}) == 2); // left 1-0 and left 2-1
    // Conflicting matches: left 1 can match 0 or 1, left 2 only 0 -> maximal 2
    assert(countUniqueMatches({1,2}, {0,0}) == 1); // both left want 0, only one matches
    // Empty input
    assert(countUniqueMatches({}, {}) == 0);
    // Self-matching values
    assert(countUniqueMatches({5,5,6}, {5,7,6}) == 2); // 5-5, 6-6 (5 cannot match both 5 and 7)
    // Larger random check: 3 left, 2 right, all edges present
    assert(countUniqueMatches({10,20,30}, {50,60,70}) == 3);
    // Left with no edges: no matches
    assert(countUniqueMatches({1,2,3}, {4,5,6}) == 0);
    // Multiple duplicates on both sides
    assert(countUniqueMatches({1,1,1,2}, {1,1,2,2}) == 2); // 1-1 and 2-2
    return 0;
}

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <functional>

// Count maximum unique bijective matches between two equal-length vectors of integers.
// Each pair (a,b) from the input is a potential match; each index value on the left
// and right can be used at most once. Returns maximum cardinality matching size.
int countUniqueMatches(const std::vector<int>& left_indices, const std::vector<int>& right_indices) {
    // Build adjacency list: left value -> set of right neighbors (deduplicate edges)
    std::unordered_map<int, std::unordered_set<int>> adj;
    for (size_t i = 0; i < left_indices.size(); ++i) {
        adj[left_indices[i]].insert(right_indices[i]);
    }

    // Map each distinct left value to a contiguous ID for simplicity (optional)
    std::vector<int> left_vertices;
    left_vertices.reserve(adj.size());
    for (const auto& [left_val, _] : adj) {
        left_vertices.push_back(left_val);
    }

    // Map right value -> matched left value (or -1 if free)
    std::unordered_map<int, int> match_right_to_left;

    // DFS augmenting path from a given left vertex
    std::function<bool(int, std::unordered_set<int>&)> dfs = 
        [&](int left_val, std::unordered_set<int>& visited) -> bool {
            for (int right_val : adj[left_val]) {
                if (visited.find(right_val) != visited.end()) continue;
                visited.insert(right_val);
                if (match_right_to_left.find(right_val) == match_right_to_left.end() ||
                    dfs(match_right_to_left[right_val], visited)) {
                    match_right_to_left[right_val] = left_val;
                    return true;
                }
            }
            return false;
        };

    int match_count = 0;
    for (int left_val : left_vertices) {
        std::unordered_set<int> visited;
        if (dfs(left_val, visited)) {
            ++match_count;
        }
    }
    return match_count;
}

// The problem reduces to finding the size of a maximum bipartite matching. Each distinct value in the first vector is a left vertex, each distinct value in the second vector is a right vertex, and an edge exists if the corresponding pair appears in the input. Because each left and right vertex can be used at most once, we traverse the input to build an adjacency list: for each pair `(a, b)`, add `b` to the adjacency list of `a` (we can use a `std::map` or `std::unordered_map` to map each distinct left value to a list of distinct right neighbors, avoiding duplicate edges). Then run a standard DFS-based augmenting path algorithm (Kuhn's algorithm) over all distinct left vertices. For each left vertex, attempt to find an augmenting path: try to match it to an unvisited right neighbor; if that neighbor is already matched, try to reassign that neighbor's current left match recursively. Keep a `matchR` array (or map) storing for each right vertex which left vertex it is matched to, and a `visited` array per DFS run. The total number of successful assignments is the answer. Edge cases: empty input returns 0; if there are duplicate edges, they are naturally handled because we store neighbors in a set or avoid duplicates; if a left vertex has no neighbors, it contributes 0. Time complexity: building adjacency takes O(N) for N pairs (with a set to deduplicate edges per left vertex, each insertion is O(log degree) leading to O(N log N) worst case, but using a hash set average O(N)). The matching algorithm runs in O(V * E) worst case, where V is number of distinct left vertices and E is number of distinct edges. With N up to 10^5, this is acceptable.
