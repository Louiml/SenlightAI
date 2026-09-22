// Given a vector of friendship logs where each log is a vector of three integers `{timestamp, personA, personB}` indicating that persons `A` and `B` became friends at that exact timestamp, and an integer `n` representing the total number of people labeled `0` through `n-1`, write a C++ function `int earliestAcq(vector<vector<int>>& logs, int n)` that returns the earliest timestamp at which every person becomes connected (i.e., all individuals are in a single friend group through direct or transitive friendships). If it is impossible for everyone to become connected given the logs, return `-1`. Assume friendship is symmetric and cumulative (once friends, always friends). The logs may be unsorted, may contain duplicate timestamps, and may contain duplicate or contradictory friendship pairs (e.g., the same pair appearing multiple times). If `n` is 0 or 1, the condition is trivially satisfied at time 0; return 0 in those cases.
// The core problem is to determine when all `n` nodes become connected in an undirected graph where edges are added over time. The earliest timestamp at which the graph becomes fully connected can be found by processing the friendship logs in chronological order while maintaining a Union-Find (disjoint set) structure. Sort the logs by timestamp so that we always consider events in time order. Initialize a Union-Find with `n` nodes; keep a counter of the number of connected components, initially `n`. For each log in sorted order, attempt to union the two persons. If the union actually merges two previously separate components (i.e., their roots differ), decrement the component count and record the current timestamp as the candidate answer. After each successful union, if the component count reaches 1, return the current timestamp immediately. The key edge cases: if `n` is 0 or 1, return 0 because everyone is already connected. If after processing all logs the component count is still greater than 1, return -1. Duplicate pairs are harmless because unionizing an already connected pair does nothing. The time complexity is O(m log m) for sorting `m` logs plus O(m α(n)) for the union-find operations (where α is the inverse Ackermann function, nearly constant). Space complexity is O(n) for the parent array and O(m) for storing the sorted logs if done in-place.
#include <vector>
#include <algorithm>

// Union-Find (Disjoint Set Union) with path compression and union by rank.
class UnionFind {
private:
    std::vector<int> parent;
    std::vector<int> rank;
    int components;

    int find(int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]); // path compression
        }
        return parent[x];
    }

public:
    explicit UnionFind(int n) : parent(n), rank(n, 0), components(n) {
        for (int i = 0; i < n; ++i) {
            parent[i] = i;
        }
    }

    // Union the sets containing a and b. Returns true if they were previously separate.
    bool unite(int a, int b) {
        int rootA = find(a);
        int rootB = find(b);
        if (rootA == rootB) return false;

        // Union by rank to keep tree shallow.
        if (rank[rootA] < rank[rootB]) {
            parent[rootA] = rootB;
        } else if (rank[rootA] > rank[rootB]) {
            parent[rootB] = rootA;
        } else {
            parent[rootB] = rootA;
            rank[rootA]++;
        }
        --components;
        return true;
    }

    bool allConnected() const {
        return components == 1;
    }
};

// Returns the earliest timestamp when all n people are connected, or -1 if impossible.
int earliestAcq(std::vector<std::vector<int>>& logs, int n) {
    if (n <= 1) return 0;

    // Sort by timestamp (first element of each log).
    std::sort(logs.begin(), logs.end(),
              [](const std::vector<int>& a, const std::vector<int>& b) {
                  return a[0] < b[0];
              });

    UnionFind uf(n);
    for (const auto& log : logs) {
        int timestamp = log[0];
        int a = log[1];
        int b = log[2];
        if (uf.unite(a, b)) {
            if (uf.allConnected()) {
                return timestamp;
            }
        }
    }
    return -1;
}
#include <cassert>
#include <vector>

// The solution function is defined above.
int main() {
    // Example from the snippet: earliest connection at time 3.
    std::vector<std::vector<int>> logs1 = {
        {9, 3, 0}, {0, 2, 1}, {8, 0, 1}, {1, 3, 2}, {2, 2, 0}, {3, 3, 1}
    };
    assert(earliestAcq(logs1, 4) == 3);

    // Already connected from the start with n=1.
    std::vector<std::vector<int>> logs2 = {};
    assert(earliestAcq(logs2, 1) == 0);

    // n=0 edge case.
    assert(earliestAcq(logs2, 0) == 0);

    // Impossible: two separate groups never connect.
    std::vector<std::vector<int>> logs3 = {
        {1, 0, 1}, {2, 2, 3}, {3, 4, 5}
    };
    assert(earliestAcq(logs3, 6) == -1);

    // Duplicate pairs and unsorted input.
    std::vector<std::vector<int>> logs4 = {
        {10, 0, 1}, {5, 1, 2}, {7, 0, 1}, {3, 2, 3}, {1, 0, 1}, {20, 3, 4}
    };
    // Sorted: {1,0,1}, {3,2,3}, {5,1,2}, {7,0,1}, {10,0,1}, {20,3,4}
    // After {3,2,3}: components {0,1} and {2,3} and {4} -> 3 components
    // After {5,1,2}: components {0,1,2,3} and {4} -> 2 components
    // After {20,3,4}: all connected -> timestamp 20
    assert(earliestAcq(logs4, 5) == 20);

    // Trivial fully connected with one log for n=2.
    std::vector<std::vector<int>> logs5 = {{42, 0, 1}};
    assert(earliestAcq(logs5, 2) == 42);

    // No logs but n=3 impossible.
    std::vector<std::vector<int>> logs6 = {};
    assert(earliestAcq(logs6, 3) == -1);

    return 0;
}
