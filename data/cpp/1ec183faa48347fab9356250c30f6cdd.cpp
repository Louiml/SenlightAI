/*
Write a C++ function `int maxMatches(const std::vector<Person>& groupA, const std::vector<Person>& groupB)` that, given two groups of people (each described by height, age, and a division value), returns the maximum number of people from groupA that can be matched to distinct people from groupB. A match is allowed only if all three conditions hold: the absolute difference in height is at most 12, the absolute difference in age is at most 5, and the division values are equal. Each person from groupB can be used at most once, and each person from groupA is matched at most once. A person in groupA may remain unmatched if no suitable partner exists. The function must compute the maximum cardinality matching in the bipartite graph where edges exist between compatible pairs. Define a struct `Person` with `int height, age, dv;` (dv = division value). You may assume all vectors are non-empty, all values are non-negative integers, and no duplicates within a group for simplification (though the algorithm should handle duplicates if present). The function should be self-contained, use only standard C++ libraries, and not rely on global mutable state.
*/

#include <vector>
#include <cstring>
#include <cmath>

struct Person {
    int height;
    int age;
    int dv;
};

// Maximum bipartite matching using augmenting paths (Kuhn's algorithm)
int maxMatches(const std::vector<Person>& groupA, const std::vector<Person>& groupB) {
    int n = (int)groupA.size();
    int m = (int)groupB.size();

    // Build adjacency list: for each person in A, store compatible indices in B
    std::vector<std::vector<int>> adj(n);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (std::abs(groupA[i].height - groupB[j].height) <= 12 &&
                std::abs(groupA[i].age - groupB[j].age) <= 5 &&
                groupA[i].dv == groupB[j].dv) {
                adj[i].push_back(j);
            }
        }
    }

    // leftMatch[i] = j if A[i] matched to B[j], else -1
    // rightMatch[j] = i if B[j] matched to A[i], else -1
    std::vector<int> leftMatch(n, -1);
    std::vector<int> rightMatch(m, -1);

    // visited array for DFS
    std::vector<bool> visited;

    // Recursive DFS to find augmenting path starting from left vertex u
    std::function<bool(int)> dfs = [&](int u) -> bool {
        for (int v : adj[u]) {
            if (visited[v]) continue;
            visited[v] = true;
            if (rightMatch[v] == -1 || dfs(rightMatch[v])) {
                leftMatch[u] = v;
                rightMatch[v] = u;
                return true;
            }
        }
        return false;
    };

    int result = 0;
    for (int i = 0; i < n; ++i) {
        visited.assign(m, false);
        if (dfs(i)) {
            ++result;
        }
    }
    return result;
}

#include <cassert>
#include <vector>

// The provided Person struct and maxMatches function are assumed to be included above.

int main() {
    // Test 1: Simple case with all matches possible
    std::vector<Person> A1 = {{170, 25, 1}, {175, 30, 1}};
    std::vector<Person> B1 = {{165, 23, 1}, {180, 33, 1}};
    assert(maxMatches(A1, B1) == 2);

    // Test 2: No matches because division differs
    std::vector<Person> A2 = {{170, 25, 1}};
    std::vector<Person> B2 = {{170, 25, 2}};
    assert(maxMatches(A2, B2) == 0);

    // Test 3: Only one match possible because both A persons want same B person
    std::vector<Person> A3 = {{170, 25, 1}, {172, 26, 1}};
    std::vector<Person> B3 = {{175, 28, 1}};
    assert(maxMatches(A3, B3) == 1);

    // Test 4: Height difference just outside limit
    std::vector<Person> A4 = {{170, 25, 1}};
    std::vector<Person> B4 = {{183, 25, 1}}; // diff = 13, not allowed
    assert(maxMatches(A4, B4) == 0);

    // Test 5: Age difference exactly at limit
    std::vector<Person> A5 = {{170, 25, 1}};
    std::vector<Person> B5 = {{170, 20, 1}}; // diff = 5, allowed
    assert(maxMatches(A5, B5) == 1);

    // Test 6: Age difference just over limit
    std::vector<Person> A6 = {{170, 25, 1}};
    std::vector<Person> B6 = {{170, 19, 1}}; // diff = 6, not allowed
    assert(maxMatches(A6, B6) == 0);

    // Test 7: Multiple candidates, max matching picks two
    std::vector<Person> A7 = {{160, 20, 3}, {180, 40, 3}};
    std::vector<Person> B7 = {{155, 22, 3}, {175, 38, 3}, {165, 21, 3}};
    assert(maxMatches(A7, B7) == 2);

    // Test 8: Empty adjacency for some A persons
    std::vector<Person> A8 = {{170, 25, 1}, {200, 50, 5}};
    std::vector<Person> B8 = {{170, 25, 1}};
    assert(maxMatches(A8, B8) == 1);

    // Test 9: Larger dataset, all distinct compatible partners (simple chain)
    std::vector<Person> A9 = {{170, 20, 0}, {172, 21, 0}};
    std::vector<Person> B9 = {{180, 30, 0}, {182, 31, 0}};
    assert(maxMatches(A9, B9) == 2);

    // Test 10: Duplicate persons in A (but distinct objects)
    std::vector<Person> A10 = {{170, 25, 1}, {170, 25, 1}};
    std::vector<Person> B10 = {{170, 25, 1}};
    assert(maxMatches(A10, B10) == 1);

    return 0;
}

// The problem is a classic maximum bipartite matching. Build a graph where each person in groupA is a left vertex (indexed 0..n-1) and each person in groupB is a right vertex (indexed 0..m-1). Create an adjacency list for the left vertices: for every pair (i,j), if the compatibility conditions are satisfied, add an edge from i to j. Then run the standard augmenting path algorithm (Kuhn's algorithm) for maximum bipartite matching. The algorithm tries to find an augmenting path for each left vertex using depth-first search with a `visited` array (reset for each left vertex). Arrays `leftMatch` and `rightMatch` store the current matchings (`-1` if unmatched). The DFS returns true if it can either match the current left vertex to an unmatched right vertex, or reassign a matched right vertex's previous left partner to another option. Total matches are counted by summing successful DFS calls.
//
// Edge cases: (1) Empty adjacency for a left vertex means it can never be matched, so DFS immediately returns false. (2) Duplicate persons: if two identical persons exist in groupA and both are compatible with only one person in groupB, only one can be matched, and the algorithm correctly returns 1. (3) Large numbers: complexity is O(n * (n + m)) in worst case because each DFS visits each edge at most once per left vertex. More precisely, for each left vertex, DFS traverses at most O(E) edges, so total O(n*E). With E ≤ n*m, worst-case O(n^2 * m). Space complexity is O(n + m + E) for adjacency and matching arrays.
//
// The algorithm must be implemented with careful handling of recursion depth (if n up to 1000, recursion may be okay; if larger, consider iterative, but here typical constraints are small). The reference solution uses a vector of vectors for adjacency and a recursive DFS.
