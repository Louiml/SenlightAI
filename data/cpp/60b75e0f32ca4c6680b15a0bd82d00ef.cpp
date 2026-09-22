// Given a sequence of binary implication rules as described by a vector of pairs, where each pair `(u, v)` represents the clause `(~u OR v)` (i.e., `u => v`), and a starting true literal `start`, write a C++ function that computes the set of literals that are forced to be true by unit propagation under these binary implications. The literals are represented as integers where a positive integer `x` represents the positive literal of boolean variable `x` and a negative integer `-x` represents the negative literal of variable `x` (with `0` representing an invalid literal). The function should return a vector of all forced literals, in the order they become true during propagation (BFS order from the start literal), including the `start` literal itself. If the propagation leads to a contradiction (i.e., both a literal and its negation become true), the function should return an empty vector to indicate inconsistency. The propagation should be performed in a breadth-first manner, where for each newly true literal `l`, we examine all implications `(l => v)` and all implications `(~v => ~l)` (which are equivalent to `l => v`), marking `v` as true if it is not already assigned. Duplicate implications and self-implications should be handled gracefully.

The core algorithm is a graph traversal (BFS) over the implication graph formed by the binary clauses. Each clause `(u, v)` from input means `u => v` (equivalent to `~u OR v`), which also gives `~v => ~u` by contraposition. We build adjacency lists for each literal: for a literal `l`, its outgoing neighbors are the literals `v` such that `(l, v)` is a clause, plus the literals `w` such that `(w, ~l)` is a clause (since `~l => ~w`, so `l` is the antecedent of `~w`? Wait, careful: `(u, v)` means `u => v`. So if we have `(u, v)`, then for literal `u`, outgoing neighbor is `v`. Also by contraposition, `~v => ~u`, so for literal `~v`, outgoing neighbor is `~u`. Therefore, for a given literal `l`, we add edge `l -> v` for each clause where the first element is `l`, and edge `l -> ~u` for each clause where the second element is `~l` (because `(~l) => (~u)` from `(~l, ~u)`? Let's re-derive: If we have clause `(a, b)`, that is `a => b`. So for any literal `x`, if `x` is the first element of a clause, then `x` implies the second element. Also, if `x` is the negation of the second element, then `x` implies the negation of the first element because `a => b` is equivalent to `~b => ~a`. So for clause `(a, b)`, we add two edges: `a -> b` and `~b -> ~a`. So to build the graph, for each clause `(u, v)`, add adjacency `u -> v` and adjacency `~v -> ~u`, where `~x` is `-x` for a positive literal and `+x` for a negative literal (i.e., multiply by -1). Then starting from the `start` literal, perform BFS: mark `start` as true, enqueue it, and for each dequeue `l`, for each neighbor `w` in adjacency[l], if `w` is not yet assigned, assign it true and enqueue it; if `w` is already assigned false (i.e., its negation is true), then we have a conflict and return empty. At the end, collect all literals in the order they were dequeued (or the order they were assigned, which is the same for BFS: the order they are dequeued is the order they become true; but the first element is the start, and each subsequent is the order of propagation). The result should include the start literal first. Edge cases: self-implication `(l, l)` is a tautology and can be ignored or handled since `l => l` doesn't propagate anything new; duplicates should not cause issues since we only add edges once. Also, if the start literal is 0, return empty. Time complexity: O(n + m) where n is the number of distinct literals and m is the number of clauses, since each edge is traversed at most once. Space complexity: O(n + m) for adjacency lists and the visited flag array. The literals are stored as integers with sign, and we can use an unordered_map or a fixed-size array if we know the maximum variable; but since the problem doesn't provide a max, we can use a map from literal to boolean, or unlock the maximum variable by scanning the input first. To keep it standalone, we can store the adjacency as a map from literal to vector of literals, and use a map from literal to state (unvisited, true, false) or a set of true literals.

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>

/**
 * Perform BFS unit propagation on binary implications.
 * Each clause (u, v) represents (u implies v), i.e., (~u OR v).
 * Literals are integers; positive x is variable x, negative -x is negation.
 * Returns the literals forced true in BFS order, or empty vector on contradiction.
 */
std::vector<int> propagateBinaryImplications(int start, const std::vector<std::pair<int,int>>& clauses) {
    if (start == 0) return {}; // invalid literal

    // Build adjacency: for clause (u,v), add u -> v and ~v -> ~u.
    std::unordered_map<int, std::vector<int>> adj;
    for (const auto& p : clauses) {
        int u = p.first, v = p.second;
        if (u == 0 || v == 0) continue;
        // avoid self-implications: u => u is tautology but harmless; we'll still add but it won't propagate.
        adj[u].push_back(v);
        adj[-v].push_back(-u); // contraposition
    }

    // State: true literals set and false literals set (or we track assigned values).
    std::unordered_set<int> true_lits;
    std::unordered_set<int> false_lits; // literals that are false (i.e., their negation is true)

    std::queue<int> q;
    q.push(start);
    true_lits.insert(start);
    false_lits.insert(-start); // -start is false

    std::vector<int> result;
    while (!q.empty()) {
        int l = q.front();
        q.pop();
        result.push_back(l);
        // For each neighbor w of l, w must be true.
        auto it = adj.find(l);
        if (it != adj.end()) {
            for (int w : it->second) {
                if (true_lits.count(w)) continue; // already true
                if (false_lits.count(w)) return {}; // conflict: w is false but required true
                // w becomes true
                true_lits.insert(w);
                false_lits.insert(-w);
                q.push(w);
            }
        }
    }
    return result;
}

#include <cassert>
#include <vector>
#include <utility>

// The solution function is assumed to be defined above.
// Add the solution code here or include it via a header.

int main() {
    // Test 1: simple chain: 1 => 2, 2 => 3, start=1, expect [1,2,3]
    {
        std::vector<std::pair<int,int>> clauses = {{1,2},{2,3}};
        auto res = propagateBinaryImplications(1, clauses);
        assert(res == std::vector<int>({1,2,3}));
    }

    // Test 2: reverse propagation via contraposition: 3 => 1 means 1 => 3? Actually (1,3) means 1=>3, contraposition ~3=>~1.
    // Let's test: clause (1,3), start = -3 (i.e., not 3), then -3 => -1, so -1 becomes true.
    {
        std::vector<std::pair<int,int>> clauses = {{1,3}};
        auto res = propagateBinaryImplications(-3, clauses);
        // BFS: start -3, then neighbor from -3? From clause (1,3), we added edge 1->3 and -3->-1.
        // So -3 -> -1, so result [-3, -1]
        assert(res == std::vector<int>({-3,-1}));
    }

    // Test 3: contradiction: 1 => 2 and 1 => -2, start=1 should cause conflict.
    {
        std::vector<std::pair<int,int>> clauses = {{1,2},{1,-2}};
        auto res = propagateBinaryImplications(1, clauses);
        assert(res.empty());
    }

    // Test 4: start with no outgoing edges: expect [start].
    {
        std::vector<std::pair<int,int>> clauses = {{1,2}};
        auto res = propagateBinaryImplications(3, clauses);
        assert(res == std::vector<int>({3}));
    }

    // Test 5: BFS order with branching: 1 => 2 and 1 => 3, and 2 => 4. Expected order [1,2,3,4] (BFS level order).
    {
        std::vector<std::pair<int,int>> clauses = {{1,2},{1,3},{2,4}};
        auto res = propagateBinaryImplications(1, clauses);
        // BFS: start 1, then enqueue 2 and 3 in that order, then dequeue 2, enqueue 4, dequeue 3, dequeue 4.
        // Expected: [1,2,3,4]
        assert(res == std::vector<int>({1,2,3,4}));
    }

    // Test 6: cycle: 1 => 2 and 2 => 1, start=1, expect [1,2] (no infinite loop).
    {
        std::vector<std::pair<int,int>> clauses = {{1,2},{2,1}};
        auto res = propagateBinaryImplications(1, clauses);
        assert(res == std::vector<int>({1,2}));
    }

    // Test 7: duplicate clauses should not affect result.
    {
        std::vector<std::pair<int,int>> clauses = {{1,2},{1,2},{1,2}};
        auto res = propagateBinaryImplications(1, clauses);
        assert(res == std::vector<int>({1,2}));
    }

    // Test 8: self-implication: (1,1) is tautology, start=1, no new propagation.
    {
        std::vector<std::pair<int,int>> clauses = {{1,1}};
        auto res = propagateBinaryImplications(1, clauses);
        assert(res == std::vector<int>({1}));
    }

    // Test 9: indirect contradiction via contraposition: 1 => 2, 3 => 1, and 3 => -2, start=3 should conflict.
    {
        std::vector<std::pair<int,int>> clauses = {{1,2},{3,1},{3,-2}};
        auto res = propagateBinaryImplications(3, clauses);
        // 3 => 1 => 2, and 3 => -2, conflict.
        assert(res.empty());
    }

    // Test 10: start with negation of a variable that appears as antecedent? e.g., -1 => 2 from clause (1,2) by contraposition.
    {
        std::vector<std::pair<int,int>> clauses = {{1,2}};
        auto res = propagateBinaryImplications(-1, clauses);
        // From clause (1,2), we have edge -2 -> -1. Starting at -1, no outgoing edges from -1? Actually we added edge 1->2 and -2->-1.
        // So -1 has no outgoing edges, so result just [-1].
        assert(res == std::vector<int>({-1}));
    }

    return 0;
}
