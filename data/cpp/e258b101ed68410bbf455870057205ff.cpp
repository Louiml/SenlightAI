// Write a C++ function `bool unitPropagationSimplify(std::vector<std::vector<int>>& cnf)` that performs unit-clause propagation on a CNF formula represented as a vector of clauses, where each clause is a vector of non-zero integers (positive = variable true, negative = variable false). The function should repeatedly find unit clauses (clauses of length 1) and apply their implied assignments: remove any clause containing the positive literal, remove the negated literal from clauses containing it, and detect conflicts (a clause becomes empty, or both `x` and `-x` appear as units). After propagation, the function should return `true` if the formula is satisfiable (possibly with fewer clauses/literals, but no empty clauses), and `false` if a conflict is detected (formula is unsatisfiable under unit propagation). The function must modify `cnf` in-place to the simplified formula. Empty input (no clauses) is satisfiable.
// The core idea is to maintain a queue (or vector) of unit literals to process. Start by scanning all clauses: if a clause has exactly one literal `l`, add it to the queue (but first check for contradictions: if `-l` is already in the queue, return `false`; avoid duplicates). If any clause is empty, return `false`. Then iteratively pop a unit literal `l` from the queue. For each remaining clause:
// - If the clause contains `l`, the clause is satisfied and can be removed.
// - If the clause contains `-l`, remove that literal from the clause. If the clause becomes empty, return `false`. If it becomes a unit clause (size 1) with literal `m`, add `m` to the queue (checking for conflict with existing `-m`).
// - Otherwise, keep the clause unchanged.
// This process may discover new unit clauses, so continue until the queue is empty. After processing, construct a new formula consisting only of the kept clauses. Key edge cases: duplicate unit literals (use a set or check), conflict when both `x` and `-x` appear as units, and empty clause detection. Complexity: Each time we process a unit, we scan all remaining clauses, so worst-case O(n·m) where n = number of unit literals and m = total literals across clauses (in practice, after each assignment many clauses are removed, so often faster). Space is O(n + m) for the queue and new formula.
#include <vector>
#include <algorithm>
#include <unordered_set>

// Perform unit propagation on a CNF formula in-place.
// Returns true if no conflict is found (formula is satisfiable under unit propagation),
// false if a conflict (empty clause) is detected.
bool unitPropagationSimplify(std::vector<std::vector<int>>& cnf) {
    // Queue of unit literals to process.
    std::vector<int> unit_queue;
    // Set of unit literals already known (to avoid duplicates and detect conflicts).
    std::unordered_set<int> unit_set;

    // Initial scan: find all unit clauses and check for empty clauses.
    for (const auto& clause : cnf) {
        if (clause.empty()) {
            return false; // empty clause -> unsatisfiable (under UP)
        }
        if (clause.size() == 1) {
            int lit = clause[0];
            if (unit_set.count(-lit)) {
                return false; // both lit and -lit are units -> conflict
            }
            if (!unit_set.count(lit)) {
                unit_set.insert(lit);
                unit_queue.push_back(lit);
            }
        }
    }

    // Process units.
    size_t head = 0;
    while (head < unit_queue.size()) {
        int l = unit_queue[head++]; // current unit literal

        std::vector<std::vector<int>> new_cnf;
        new_cnf.reserve(cnf.size());

        for (const auto& clause : cnf) {
            // If clause contains l, it's satisfied -> skip.
            if (std::find(clause.begin(), clause.end(), l) != clause.end()) {
                continue;
            }
            // Otherwise, remove -l if present.
            std::vector<int> new_clause;
            new_clause.reserve(clause.size());
            bool conflict = false;
            for (int lit : clause) {
                if (lit == -l) {
                    continue; // remove literal
                }
                new_clause.push_back(lit);
            }
            if (new_clause.empty()) {
                return false; // clause became empty -> conflict
            }
            if (new_clause.size() == 1) {
                int m = new_clause[0];
                if (unit_set.count(-m)) {
                    return false; // conflict
                }
                if (!unit_set.count(m)) {
                    unit_set.insert(m);
                    unit_queue.push_back(m);
                }
            }
            new_cnf.push_back(std::move(new_clause));
        }

        cnf = std::move(new_cnf); // replace formula with simplified version
    }

    return true; // no conflict, formula is satisfiable under unit propagation
}
#include <cassert>
#include <vector>

int main() {
    // Test 1: basic propagation
    std::vector<std::vector<int>> cnf1 = {{1}, {-1, 2}, {3}};
    assert(unitPropagationSimplify(cnf1) == true);
    // After UP: clause {1} satisfied -> remove it; clause {-1,2} becomes {2} (unit) -> then {2} satisfied -> remove it; {3} remains.
    assert(cnf1 == std::vector<std::vector<int>>{{3}});

    // Test 2: conflict via contradiction
    std::vector<std::vector<int>> cnf2 = {{1}, {-1}};
    assert(unitPropagationSimplify(cnf2) == false);

    // Test 3: conflict via empty clause
    std::vector<std::vector<int>> cnf3 = {{1}, {}};
    assert(unitPropagationSimplify(cnf3) == false);

    // Test 4: no units, unchanged
    std::vector<std::vector<int>> cnf4 = {{1, 2}, {-1, 3}};
    assert(unitPropagationSimplify(cnf4) == true);
    assert(cnf4 == std::vector<std::vector<int>>{{1, 2}, {-1, 3}});

    // Test 5: propagated unit leads to new unit
    std::vector<std::vector<int>> cnf5 = {{-1, 2}, {1}};
    assert(unitPropagationSimplify(cnf5) == true);
    // After UP: remove {1}, clause {-1,2} -> {2}, then remove {2} the formula is empty.
    assert(cnf5.empty());

    // Test 6: duplicate units are handled
    std::vector<std::vector<int>> cnf6 = {{1}, {1}, {2}};
    assert(unitPropagationSimplify(cnf6) == true);
    // All clauses satisfied -> empty formula
    assert(cnf6.empty());

    // Test 7: conflict from propagated unit
    std::vector<std::vector<int>> cnf7 = {{-1, 2}, {1}, {-2}};
    assert(unitPropagationSimplify(cnf7) == false);

    // Test 8: empty formula is satisfiable
    std::vector<std::vector<int>> cnf8 = {};
    assert(unitPropagationSimplify(cnf8) == true);
    assert(cnf8.empty());

    // Test 9: larger example, no conflict
    std::vector<std::vector<int>> cnf9 = {{1, 2}, {-1, 3}, {2, -3}, {1}};
    assert(unitPropagationSimplify(cnf9) == true);
    // UP: {1} removes first and last clause, leaves {-1,3} -> {3} and {2,-3} -> {2}; then {3} and {2} are units -> remove them -> empty.
    assert(cnf9.empty());

    // Test 10: chain of units
    std::vector<std::vector<int>> cnf10 = {{1}, {-1, 2}, {-2, 3}};
    assert(unitPropagationSimplify(cnf10) == true);
    assert(cnf10.empty());
}
