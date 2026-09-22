// Write a C++ function that processes a series of friendship queries. Given an undirected social network model where friendships can be added (bidirectional) or removed (bidirectional), implement a function that takes an integer `n` (number of people, from 1 to n), an integer `k` (number of operations), and a vector of operations, each being a triple `(type, a, b)`. If `type == 1`, add a bidirectional friendship between `a` and `b`. If `type == 2`, remove the bidirectional friendship between `a` and `b`. If `type == 3`, check whether `a` and `b` are currently friends (mutual friendship) and return `true` if they are, `false` otherwise. The function should return a vector of booleans in the order the type-3 queries appear. The network is initially empty. Removal of a non-existent friendship should be ignored. Duplicate additions are allowed but should not create multiple edges (the friendship remains a single boolean relation). The input values are 1-indexed for people, and all queries are valid (1 ≤ a, b ≤ n, a ≠ b). The function must handle up to n = 10^5 and k = 10^5 efficiently.

#include <cassert>
#include <vector>

// Include the solution function here

int main() {
    // Test 1: add, check, remove, check
    {
        int n = 3;
        int k = 4;
        std::vector<std::vector<int>> ops = {
            {1, 1, 2},
            {3, 1, 2},
            {2, 1, 2},
            {3, 1, 2}
        };
        std::vector<bool> res = processFriendships(n, k, ops);
        assert(res.size() == 2);
        assert(res[0] == true);
        assert(res[1] == false);
    }

    // Test 2: duplicate add, non-existent removal
    {
        int n = 2;
        int k = 4;
        std::vector<std::vector<int>> ops = {
            {1, 1, 2},
            {1, 1, 2}, // duplicate
            {2, 1, 2},
            {3, 1, 2}
        };
        std::vector<bool> res = processFriendships(n, k, ops);
        assert(res.size() == 1);
        assert(res[0] == false);
    }

    // Test 3: multiple friendships, check different pairs
    {
        int n = 4;
        int k = 5;
        std::vector<std::vector<int>> ops = {
            {1, 1, 2},
            {1, 2, 3},
            {3, 1, 2},  // true
            {3, 1, 3},  // false
            {3, 2, 3}   // true
        };
        std::vector<bool> res = processFriendships(n, k, ops);
        assert(res.size() == 3);
        assert(res[0] == true);
        assert(res[1] == false);
        assert(res[2] == true);
    }

    // Test 4: remove non-existent, then add and check
    {
        int n = 2;
        int k = 3;
        std::vector<std::vector<int>> ops = {
            {2, 1, 2}, // ignore
            {1, 1, 2},
            {3, 1, 2}
        };
        std::vector<bool> res = processFriendships(n, k, ops);
        assert(res.size() == 1);
        assert(res[0] == true);
    }

    // Test 5: interleaved add/remove with repeated checks
    {
        int n = 3;
        int k = 7;
        std::vector<std::vector<int>> ops = {
            {1, 1, 2},
            {3, 1, 2}, // true
            {2, 1, 2},
            {3, 1, 2}, // false
            {1, 1, 2},
            {3, 2, 1}, // true (symmetric)
            {2, 2, 1}
        };
        std::vector<bool> res = processFriendships(n, k, ops);
        assert(res.size() == 3);
        assert(res[0] == true);
        assert(res[1] == false);
        assert(res[2] == true);
    }

    return 0;
}

#include <vector>
#include <unordered_set>

// Process friendship operations. Returns a vector of bool results for type-3 queries.
std::vector<bool> processFriendships(int n, int k, const std::vector<std::vector<int>>& ops) {
    // Adjacency sets: index from 1 to n
    std::vector<std::unordered_set<int>> adj(n + 1);
    std::vector<bool> results;

    for (const auto& op : ops) {
        int type = op[0];
        int a = op[1];
        int b = op[2];

        if (type == 1) {
            // Add bidirectional friendship
            adj[a].insert(b);
            adj[b].insert(a);
        } else if (type == 2) {
            // Remove bidirectional friendship (ignore if non-existent)
            adj[a].erase(b);
            adj[b].erase(a);
        } else { // type == 3
            // Check mutual friendship
            // Since we maintain symmetry, one check is enough, but double-check for clarity
            bool isFriend = (adj[a].count(b) > 0 && adj[b].count(a) > 0);
            results.push_back(isFriend);
        }
    }

    return results;
}

// The straightforward approach is to store friendships in a 2D boolean matrix, but that would be O(n^2) memory, impossible for large n. Instead, use an adjacency set for each person: `vector<unordered_set<int>>` or `vector<set<int>>`. For each query:
// - Type 1: Insert `b` into set of `a`, and `a` into set of `b` (to ensure bidirectionality). Using `unordered_set` gives average O(1) insert and lookup.
// - Type 2: Erase `b` from set of `a`, and `a` from set of `b`. If either erase fails (non-existent), it's ignored.
// - Type 3: Check if both `set[a].count(b)` and `set[b].count(a)` are true. Since we always maintain symmetric insertion, checking either one is sufficient but checking both is safe.
// Edge cases: duplicate additions (insert twice) – second insert has no effect because it's a set. Removal of non-existent friendship – erase returns 0, ignored. Queries may be repeated, and the network changes over time. Time complexity: O(k) average, because each operation is O(1) average for unordered_set. Space complexity: O(number of active friendships) = O(k) worst case. Use `vector<unordered_set<int>>` initialized with size n+1 (since people are 1-indexed).
