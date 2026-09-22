// Write a C++ function `int findCelebrity(const std::vector<std::vector<int>>& matrix)` that takes a square adjacency matrix of size `n×n` (where `n` is the matrix dimension) representing a party of `n` people. Entry `matrix[i][j] == 1` means person `i` knows person `j` (directed edge from `i` to `j`), and `matrix[i][i]` is always 0. A celebrity is a person who is known by everyone else (i.e., every other person has a 1 in that person’s column) but knows nobody (i.e., has all 0s in their own row). Return the index (0-based) of the celebrity if exactly one exists; if no celebrity exists or multiple people satisfy the condition ambiguously, return -1. The function must be `const`-correct (accept a const reference) and not modify the input.
The problem is a classic graph indegree/outdegree check. For each directed edge `(i, j)` where `matrix[i][j] == 1`, increment the indegree of `j` (meaning `j` is known by `i`) and the outdegree of `i` (meaning `i` knows `j`). A celebrity must have indegree equal to `n-1` (everyone else knows them) and outdegree equal to 0 (they know nobody). After computing both degree arrays by iterating through all `n²` cells once, we scan all indices from 0 to n-1. The first index satisfying both conditions is returned. If none, return -1. Edge cases: a single person (n=1) trivially has indegree 0 and outdegree 0; since `n-1 = 0`, the conditions are satisfied, and index 0 is a celebrity. If multiple indices satisfy the conditions (impossible in a valid directed graph because if two people each have outdegree 0, they cannot know each other, so neither would have indegree n-1 unless n=1), but we still take the first match as per typical problem constraints. Time complexity is O(n²) for scanning the matrix, plus O(n) for the degree scan, giving O(n²) overall. Space complexity is O(n) for the two degree vectors.
#include <vector>

// Function to find a celebrity in a party given an adjacency matrix.
// Returns index of celebrity if exactly one exists, otherwise -1.
int findCelebrity(const std::vector<std::vector<int>>& matrix) {
    int n = static_cast<int>(matrix.size());
    if (n == 0) return -1;
    
    std::vector<int> inDegree(n, 0);
    std::vector<int> outDegree(n, 0);
    
    // Count indegree and outdegree for each person.
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (matrix[i][j] == 1) {
                inDegree[j]++;
                outDegree[i]++;
            }
        }
    }
    
    // A celebrity is known by everyone else (n-1) and knows nobody.
    for (int i = 0; i < n; ++i) {
        if (inDegree[i] == n - 1 && outDegree[i] == 0) {
            return i;
        }
    }
    return -1;
}
#include <cassert>
#include <vector>

// Include the solution function here or link it.

int main() {
    // Test 1: Basic 3-person party, person 2 is celebrity.
    std::vector<std::vector<int>> m1 = {
        {0, 1, 1},
        {0, 0, 1},
        {0, 0, 0}
    };
    assert(findCelebrity(m1) == 2);

    // Test 2: No celebrity (everyone knows someone).
    std::vector<std::vector<int>> m2 = {
        {0, 1, 0},
        {0, 0, 1},
        {1, 0, 0}
    };
    assert(findCelebrity(m2) == -1);

    // Test 3: Single person is trivially a celebrity.
    std::vector<std::vector<int>> m3 = {{0}};
    assert(findCelebrity(m3) == 0);

    // Test 4: Two people, person 1 is celebrity (known by person 0, knows nobody).
    std::vector<std::vector<int>> m4 = {
        {0, 1},
        {0, 0}
    };
    assert(findCelebrity(m4) == 1);

    // Test 5: Two people, neither is celebrity (both know each other).
    std::vector<std::vector<int>> m5 = {
        {0, 1},
        {1, 0}
    };
    assert(findCelebrity(m5) == -1);

    // Test 6: Empty matrix (should return -1).
    std::vector<std::vector<int>> m6;
    assert(findCelebrity(m6) == -1);

    // Test 7: 4-person party where celebrity index 3.
    std::vector<std::vector<int>> m7 = {
        {0, 0, 1, 1},
        {1, 0, 0, 1},
        {0, 1, 0, 1},
        {0, 0, 0, 0}
    };
    assert(findCelebrity(m7) == 3);

    // Test 8: Person 1 is celebrity but not everyone knows them.
    std::vector<std::vector<int>> m8 = {
        {0, 1, 0},
        {0, 0, 0},
        {0, 1, 0}
    };
    assert(findCelebrity(m8) == 1);

    // Test 9: Multiple candidates but none valid (person 2 knows person 0).
    std::vector<std::vector<int>> m9 = {
        {0, 0, 1},
        {0, 0, 1},
        {1, 0, 0}
    };
    assert(findCelebrity(m9) == -1);
}
