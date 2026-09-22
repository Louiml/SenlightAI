/*
Given a set of `n` Boolean variables (numbered 1 to `n`) and `m` constraints, each of the form `(a OR b == t)` where `t` is an integer from 0 to 3, write a C++ function `int solveParty(int n, const vector<array<int,3>>& constraints)` that returns the number of variables that must be set to `true` in a satisfying assignment, as well as the indices (1-indexed) of those true variables. The mapping of `t` is as follows: `t=0` means `(!a OR !b)` must be true; `t=1` means `(!a OR b)`; `t=2` means `(a OR !b)`; `t=3` means `(a OR b)`. Each constraint is a clause of two literals, and the problem asks to find any satisfying assignment and count how many variables are true. If no assignment exists, return -1. The solution should be based on constructing an implication graph for 2-SAT, finding strongly connected components (SCCs), and then determining truth values from SCC order. The function should return -1 if a variable and its negation are in the same SCC; otherwise it should return the count and also output the list of true variable indices (sorted ascending) to standard output. The input parameters are `n` (number of variables, 1 ≤ n ≤ 100) and a vector of constraints, each an array of three integers `{a, b, t}` where 1 ≤ a,b ≤ n and t ∈ {0,1,2,3}. The function must be deterministic and handle any valid input.
*/
#include <vector>
#include <algorithm>
#include <iostream>

// Solve the 2-SAT problem: returns number of true variables, or -1 if unsatisfiable.
// Also prints the indices (1-indexed) of variables set to true in ascending order.
int solveParty(int n, const std::vector<std::array<int,3>>& constraints) {
    const int totalNodes = 2 * n;
    std::vector<std::vector<int>> graph(totalNodes + 1);
    std::vector<std::vector<int>> reverseGraph(totalNodes + 1);

    // Helper lambda to add an implication edge: from -> to
    auto addEdge = [&](int from, int to) {
        graph[from].push_back(to);
        reverseGraph[to].push_back(from);
    };

    // Build implication graph from constraints
    for (const auto& c : constraints) {
        int a = c[0];
        int b = c[1];
        int t = c[2];
        // Literal representation: variable i -> node i (positive), node i+n (negative)
        if (t == 0) { // (¬a ∨ ¬b) ≡ (a → ¬b) and (b → ¬a)
            addEdge(a, b + n);
            addEdge(b, a + n);
        } else if (t == 1) { // (¬a ∨ b) ≡ (a → b) and (¬b → ¬a)
            addEdge(a, b);
            addEdge(b + n, a + n);
        } else if (t == 2) { // (a ∨ ¬b) ≡ (¬a → ¬b) and (b → a)
            addEdge(a + n, b + n);
            addEdge(b, a);
        } else if (t == 3) { // (a ∨ b) ≡ (¬a → b) and (¬b → a)
            addEdge(a + n, b);
            addEdge(b + n, a);
        }
    }

    // First DFS pass: compute finishing order
    std::vector<bool> visited(totalNodes + 1, false);
    std::vector<int> order(totalNodes + 1);
    int orderCounter = 0;

    std::function<void(int)> dfs1 = [&](int node) {
        visited[node] = true;
        for (int next : graph[node]) {
            if (!visited[next]) {
                dfs1(next);
            }
        }
        order[++orderCounter] = node;
    };

    for (int i = 1; i <= totalNodes; ++i) {
        if (!visited[i]) {
            dfs1(i);
        }
    }

    // Second DFS pass on reverse graph: find SCCs
    std::vector<int> component(totalNodes + 1, 0);
    int compCounter = 0;
    std::fill(visited.begin(), visited.end(), false);

    std::function<void(int)> dfs2 = [&](int node) {
        visited[node] = true;
        component[node] = compCounter;
        for (int next : reverseGraph[node]) {
            if (!visited[next]) {
                dfs2(next);
            }
        }
    };

    for (int i = totalNodes; i >= 1; --i) {
        int node = order[i];
        if (!visited[node]) {
            ++compCounter;
            dfs2(node);
        }
    }

    // Check unsatisfiability: if variable and its negation in same SCC
    for (int i = 1; i <= n; ++i) {
        if (component[i] == component[i + n]) {
            return -1;
        }
    }

    // Determine assignment: true if component[i] > component[i+n]
    int trueCount = 0;
    for (int i = 1; i <= n; ++i) {
        if (component[i] > component[i + n]) {
            ++trueCount;
        }
    }

    // Print the true variable indices in ascending order (they are already in order)
    for (int i = 1; i <= n; ++i) {
        if (component[i] > component[i + n]) {
            std::cout << i << "\n";
        }
    }

    return trueCount;
}
#include <cassert>
#include <vector>
#include <array>
#include <iostream>

int solveParty(int n, const std::vector<std::array<int,3>>& constraints);

int main() {
    // Test 1: simple satisfiable, no constraints
    std::vector<std::array<int,3>> c1;
    assert(solveParty(3, c1) == 0);

    // Test 2: (x1) alone, constraint (x1 OR x1) i.e. t=3, a=1,b=1
    std::vector<std::array<int,3>> c2 = {{1,1,3}};
    assert(solveParty(1, c2) == 1);

    // Test 3: contradiction (x1 AND ¬x1) via constraints
    // (x1 OR x1) and (¬x1 OR ¬x1) which forces x1 true and false
    std::vector<std::array<int,3>> c3 = {{1,1,3}, {1,1,0}};
    assert(solveParty(1, c3) == -1);

    // Test 4: classic (x1 OR x2) and (¬x1 OR x2) and (x1 OR ¬x2) → x2 must be true
    std::vector<std::array<int,3>> c4 = {{1,2,3}, {1,2,1}, {1,2,2}};
    int res4 = solveParty(2, c4);
    assert(res4 == 1); // only x2 is forced true

    // Test 5: two independent variables with no constraints
    std::vector<std::array<int,3>> c5;
    assert(solveParty(2, c5) == 0);

    // Test 6: unsatisfiable 2-cycle: (x1 OR x1) and (¬x1 OR ¬x1) already tested, test longer
    std::vector<std::array<int,3>> c6 = {{1,2,0}, {1,2,3}};
    // (¬1 ∨ ¬2) and (1 ∨ 2) is satisfiable (one true one false)
    int res6 = solveParty(2, c6);
    assert(res6 >= 1 && res6 <= 1); // exactly one true

    // Test 7: n=1, constraint (¬x1 OR ¬x1) forces x1 false
    std::vector<std::array<int,3>> c7 = {{1,1,0}};
    assert(solveParty(1, c7) == 0);

    // Test 8: n=3, constraints that force exactly one true among 1 and 2, and 3 free
    std::vector<std::array<int,3>> c8 = {{1,2,1}, {1,2,2}};
    int res8 = solveParty(3, c8);
    assert(res8 >= 1 && res8 <= 2); // variable 3 free, one of 1/2 true

    // Test 9: duplicate constraints don't affect
    std::vector<std::array<int,3>> c9 = {{1,2,3}, {1,2,3}, {2,1,3}};
    assert(solveParty(2, c9) == 2); // both forced true

    // Test 10: large trivial case n=100, m=0 → returns 0
    std::vector<std::array<int,3>> c10;
    assert(solveParty(100, c10) == 0);

    return 0;
}
// The problem is a classic 2-SAT satisfiability problem. Each variable `i` is represented by two nodes: `i` (for positive literal `x_i`) and `i+n` (for negative literal `¬x_i`). Each constraint `(a OR b) == t` can be rewritten as a disjunction of two literals, which is equivalent to two implications: `¬L1 → L2` and `¬L2 → L1`. For example, when `t=0`, the clause is `(¬a OR ¬b)`, so implications are `a → ¬b` and `b → ¬a`. Similarly, `t=1` gives `(¬a OR b)`, implications `a → b` and `¬b → ¬a`, and so on. We build a directed graph with `2n` nodes. First, we run DFS on the original graph to get a finishing order, then we run DFS on the reverse graph in reverse finishing order to find SCCs. Each SCC is assigned a component id (`ctc`). A variable `i` is set to true if the component id of its positive literal `i` is greater than the component id of its negative literal `i+n` (this is the standard 2-SAT assignment based on topological order of SCC condensation). If for any `i`, `ctc[i] == ctc[i+n]`, then both a variable and its negation are in the same SCC, meaning the formula is unsatisfiable; return -1. Otherwise, count the variables where `ctc[i] > ctc[i+n]` and return that count (also print those indices). The algorithm runs in O(n + m) time and O(n + m) space, since each constraint adds two directed edges. Edge cases include n=1, m=0 (all variables false, returns 0), constraints with a==b, and duplicate constraints, all handled naturally by SCC detection. The reference solution uses two DFS passes and handles any graph.
