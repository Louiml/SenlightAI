Write a C++ function `bool satisfiable(int n, const std::vector<std::pair<std::pair<int,bool>, std::pair<int,bool>>>& clauses, std::vector<int>& assignment)` that determines whether a 2-SAT formula over `n` boolean variables (indexed 0 to n-1) is satisfiable. Each clause is a disjunction of two literals, where each literal is a pair `(variable_index, is_negated)`. For example, `(2, true)` means `¬x2` and `(0, false)` means `x0`. The function must return `true` if the formula is satisfiable, and in that case fill `assignment` with `true`/`false` values (as 1/0) for each variable such that all clauses are satisfied. If unsatisfiable, return `false` and leave `assignment` unchanged. You may assume the input clauses are well-formed (variable indices in range), but duplicate clauses and clauses containing the same literal twice are allowed. The solution must be self-contained—do not use any external 2-SAT library.

#include <cassert>
#include <vector>
#include <functional>

// Include the solution code here (or link it).

int main() {
    using Clause = std::pair<std::pair<int,bool>, std::pair<int,bool>>;
    std::vector<int> assign;

    // Test 1: (x0) single literal clause, trivial satisfiable
    {
        std::vector<Clause> clauses = {{{0,false},{0,false}}};  // x0 OR x0 => x0
        assert(satisfiable(1, clauses, assign));
        assert(assign.size() == 1);
        assert(assign[0] == 1);
    }

    // Test 2: (x0) AND (~x0) -> unsat
    {
        std::vector<Clause> clauses = {{{0,false},{0,false}}, {{0,true},{0,true}}};
        assert(!satisfiable(1, clauses, assign));
    }

    // Test 3: (x0 OR x1) AND (~x0 OR x1) -> satisfiable with x1=1
    {
        std::vector<Clause> clauses = {{{0,false},{1,false}}, {{0,true},{1,false}}};
        assert(satisfiable(2, clauses, assign));
        assert(assign[1] == 1); // x1 must be true
    }

    // Test 4: (x0 OR x1) AND (~x0 OR ~x1) AND (x0 OR ~x1) AND (~x0 OR x1) -> unsat (all 2^2 combos excluded)
    {
        std::vector<Clause> clauses = {
            {{0,false},{1,false}},
            {{0,true},{1,true}},
            {{0,false},{1,true}},
            {{0,true},{1,false}}
        };
        assert(!satisfiable(2, clauses, assign));
    }

    // Test 5: No clauses, n=3 -> satisfiable, any assignment works
    {
        std::vector<Clause> clauses;
        assert(satisfiable(3, clauses, assign));
        assert(assign.size() == 3);
        for (int v : assign) assert(v == 0 || v == 1);
    }

    // Test 6: Larger unsat: (x0 OR x1) AND (x0 OR ~x1) AND (~x0 OR x1) AND (~x0 OR ~x1)
    {
        std::vector<Clause> clauses = {
            {{0,false},{1,false}},
            {{0,false},{1,true}},
            {{0,true},{1,false}},
            {{0,true},{1,true}}
        };
        assert(!satisfiable(2, clauses, assign));
    }

    // Test 7: Satisfiable with 3 vars, chain: (x0 -> x1) represented as (~x0 OR x1), (x1 -> x2) as (~x1 OR x2), and (x0)
    {
        std::vector<Clause> clauses = {
            {{0,true},{1,false}}, // ~x0 OR x1
            {{1,true},{2,false}}, // ~x1 OR x2
            {{0,false},{0,false}} // x0
        };
        assert(satisfiable(3, clauses, assign));
        assert(assign[0] == 1 && assign[1] == 1 && assign[2] == 1);
    }

    // Test 8: Duplicate clauses do not break anything
    {
        std::vector<Clause> clauses = {
            {{0,false},{1,false}},
            {{0,false},{1,false}},
            {{0,false},{1,false}}
        };
        assert(satisfiable(2, clauses, assign));
        assert(assign[0] == 0 && assign[1] == 0); // setting both false satisfies
    }

    return 0;
}

#include <vector>
#include <algorithm>

// Determines satisfiability of a 2-SAT formula and returns a satisfying assignment.
// n: number of variables (0..n-1)
// clauses: each clause is a pair of literals, literal = (var, is_negated)
// assignment: filled with 0/1 for each variable if satisfiable
// Returns true if satisfiable, false otherwise.
bool satisfiable(int n,
                 const std::vector<std::pair<std::pair<int,bool>, std::pair<int,bool>>>& clauses,
                 std::vector<int>& assignment) {
    int nodeCount = 2 * n;
    std::vector<std::vector<int>> adj(nodeCount);
    std::vector<std::vector<int>> rev(nodeCount);

    // Add implications for each clause: (~a -> b) and (~b -> a)
    for (const auto& clause : clauses) {
        int v = clause.first.first << 1 | clause.first.second;   // literal a
        int u = clause.second.first << 1 | clause.second.second; // literal b
        int neg_v = v ^ 1; // negation of a
        int neg_u = u ^ 1; // negation of b
        adj[neg_v].push_back(u);
        adj[neg_u].push_back(v);
        rev[u].push_back(neg_v);
        rev[v].push_back(neg_u);
    }

    // Kosaraju's algorithm for SCC
    std::vector<bool> visited(nodeCount, false);
    std::vector<int> order;
    order.reserve(nodeCount);

    // First DFS pass: finish order
    std::function<void(int)> dfs1 = [&](int node) {
        visited[node] = true;
        for (int next : adj[node]) {
            if (!visited[next]) {
                dfs1(next);
            }
        }
        order.push_back(node);
    };

    for (int i = 0; i < nodeCount; ++i) {
        if (!visited[i]) {
            dfs1(i);
        }
    }

    std::reverse(order.begin(), order.end());

    // Second DFS pass on reversed graph
    std::vector<int> comp(nodeCount, -1);
    int compId = 0;

    std::function<void(int)> dfs2 = [&](int node) {
        comp[node] = compId;
        for (int next : rev[node]) {
            if (comp[next] == -1) {
                dfs2(next);
            }
        }
    };

    for (int node : order) {
        if (comp[node] == -1) {
            dfs2(node);
            ++compId;
        }
    }

    // Check satisfiability and build assignment
    assignment.assign(n, 0);
    for (int i = 0; i < n; ++i) {
        if (comp[2*i] == comp[2*i+1]) {
            return false;
        }
        // Node 2*i = false literal, 2*i+1 = true literal.
        // We assign true if the true literal's component is greater (as per standard 2-SAT ordering).
        assignment[i] = (comp[2*i+1] > comp[2*i]) ? 1 : 0;
    }
    return true;
}

// The standard approach is to model each variable `x_i` as two nodes in an implication graph: node `2*i` represents `x_i` (false literal) and node `2*i+1` represents `¬x_i` (true literal), or equivalently use `v << 1 | nv` as in the snippet. Each clause `(a ∨ b)` is equivalent to two implications: `¬a → b` and `¬b → a`. Build the adjacency list and its reverse (for Kosaraju's algorithm). Run two DFS passes: first compute finish order on the original graph, then process nodes in reverse finish order on the reversed graph to assign component IDs. The formula is satisfiable iff for every variable `i`, component of node `2*i` differs from component of node `2*i+1`. If satisfiable, a valid assignment is `assignment[i] = (comp[2*i] > comp[2*i+1])` (or the opposite, depending on convention—the key is that the two literals get opposite truth values; the snippet uses `cmp[i] > cmp[i+1]` where node `2*i` is false literal and `2*i+1` is true literal). The algorithm is O(V+E) time and O(V) space, where V=2n and E=2 * number of clauses (each clause adds 2 directed edges). Edge cases: n=0 (vacuous, return true), duplicate clauses (harmless), clauses where both literals are identical (still works), and a formula with no clauses (trivially satisfiable). The implementation must handle large n and many clauses efficiently.
