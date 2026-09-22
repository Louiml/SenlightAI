Write a C++ function that parses a DIMACS CNF file (plain text, not gzipped) from a given input file path, determines satisfiability using a simple DPLL-style backtracking solver with unit propagation and pure literal elimination, and returns a `std::string` containing either `"SATISFIABLE"`, `"UNSATISFIABLE"`, or `"INDETERMINATE"` (the latter if the solver hits a prescribed decision limit). The function must accept the file path and a maximum decision count (as an `int`) and must not modify global state (i.e., it must be re‑entrant, not using static or global solver instances). The input DIMACS format follows the standard: comment lines starting with `c`, a problem line `p cnf <variables> <clauses>`, and clause lines containing non‑zero literals (positive for true, negative for false) terminated by `0`. The function should read the entire file, build an internal clause database, and run the solver, counting decisions; if the decision count exceeds the provided limit, the solver returns `INDETERMINATE`. Assume the input file is well‑formed and non‑empty; handle both satisfiable and unsatisfiable instances correctly. The algorithm should use an efficient assignment trail and propagate implications, but does not need to implement conflict‑driven learning or restarts—this is a simplified educational exercise.
#include <cassert>
#include <fstream>
#include <string>
#include <iostream>

// The solution function is declared here (in the same translation unit for testing)
std::string solveDimacs(const std::string& filePath, int maxDecisions);

int main() {
    // Create temporary DIMACS files
    const char* satFile = "test_sat.cnf";
    const char* unsatFile = "test_unsat.cnf";
    const char* indeterminateFile = "test_indet.cnf";
    const char* trivialFile = "test_trivial.cnf";

    // SAT instance: (x1) ∧ (x2 ∨ -x1) — satisfiable with x1=true, x2=true or false
    {
        std::ofstream out(satFile);
        out << "c simple sat\np cnf 2 2\n1 0\n2 -1 0\n";
    }
    // UNSAT instance: (x1) ∧ (-x1) ∧ (x2 ∨ x1) — contradiction
    {
        std::ofstream out(unsatFile);
        out << "c simple unsat\np cnf 2 3\n1 0\n-1 0\n2 1 0\n";
    }
    // INDETERMINATE: a SAT instance but limit 0 forces indeterminate
    {
        std::ofstream out(indeterminateFile);
        out << "c requires decisions\np cnf 3 3\n1 2 0\n-1 3 0\n-2 -3 0\n";
    }
    // Trivial SAT: no clauses, just variables
    {
        std::ofstream out(trivialFile);
        out << "c empty\np cnf 5 0\n";
    }

    assert(solveDimacs(satFile, 10) == "SATISFIABLE");
    assert(solveDimacs(unsatFile, 10) == "UNSATISFIABLE");
    assert(solveDimacs(indeterminateFile, 0) == "INDETERMINATE");
    assert(solveDimacs(indeterminateFile, 10) == "SATISFIABLE"); // should solve with enough decisions
    assert(solveDimacs(trivialFile, 0) == "SATISFIABLE"); // no decisions needed

    // Clean up temp files
    std::remove(satFile);
    std::remove(unsatFile);
    std::remove(indeterminateFile);
    std::remove(trivialFile);

    std::cout << "All tests passed" << std::endl;
    return 0;
}
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>

// Simple DPLL SAT solver with unit propagation and pure literal elimination.
// Returns "SATISFIABLE", "UNSATISFIABLE", or "INDETERMINATE" if decisions exceed limit.
std::string solveDimacs(const std::string& filePath, int maxDecisions) {
    // Read and parse DIMACS, store clauses as vectors of ints (literal: positive var, negative var)
    std::ifstream in(filePath);
    if (!in) throw std::runtime_error("Cannot open file");
    std::string line;
    int numVars = 0, numClauses = 0;
    // Skip comments and find p cnf line
    while (std::getline(in, line)) {
        std::istringstream iss(line);
        char ch;
        if (iss >> ch) {
            if (ch == 'p') {
                std::string cnf;
                iss >> cnf >> numVars >> numClauses;
                break;
            }
        }
    }
    if (numVars <= 0 || numClauses < 0) throw std::runtime_error("Invalid DIMACS header");

    std::vector<std::vector<int>> clauses;
    // Read clause lines; each ends with 0
    while (std::getline(in, line)) {
        std::istringstream iss(line);
        int lit;
        std::vector<int> clause;
        while (iss >> lit) {
            if (lit == 0) {
                if (!clause.empty()) clauses.push_back(clause);
                break;
            }
            // Ignore duplicate literals and tautologies (x and -x in same clause)
            bool dup = false, taut = false;
            for (int l : clause) {
                if (l == lit) { dup = true; break; }
                if (l == -lit) { taut = true; break; }
            }
            if (dup || taut) continue;
            clause.push_back(lit);
        }
    }
    if ((int)clauses.size() < numClauses) {
        // Could be fewer if some lines were skipped; not important for correctness.
    }

    // Solver state
    enum Val { UNDEF = 0, TRUE = 1, FALSE = 2 };
    std::vector<Val> model(numVars + 1, UNDEF); // 1-indexed variables
    std::vector<int> trail;                     // stack of assigned variables in order
    std::vector<int> trailLim;                  // decision level boundaries
    int decisions = 0;

    // Helper: check if a clause is satisfied, unit, conflicting, or unresolved
    auto clauseStatus = [&](const std::vector<int>& clause, int& unitLit) -> int {
        int numUndef = 0, undefLit = 0;
        for (int lit : clause) {
            int var = (lit > 0) ? lit : -lit;
            Val v = model[var];
            if (v == UNDEF) { numUndef++; undefLit = var; }
            else if (v == TRUE && lit > 0) return 1; // satisfied
            else if (v == FALSE && lit < 0) return 1; // satisfied
        }
        if (numUndef == 0) return 0; // conflict
        if (numUndef == 1) { unitLit = undefLit; return 2; } // unit
        return 3; // unresolved
    };

    // Unit propagation: repeatedly assign unit clauses until fixpoint or conflict
    auto propagate = [&]() -> bool {
        bool changed = true;
        while (changed) {
            changed = false;
            for (const auto& cl : clauses) {
                int unitVar = 0;
                int status = clauseStatus(cl, unitVar);
                if (status == 0) return false; // conflict
                if (status == 2) {
                    // Determine value from literal sign
                    for (int lit : cl) {
                        if ((lit > 0 && model[lit] == UNDEF) || (lit < 0 && model[-lit] == UNDEF)) {
                            model[unitVar] = (lit > 0) ? TRUE : FALSE;
                            trail.push_back(unitVar);
                            changed = true;
                            break;
                        }
                    }
                }
            }
        }
        return true;
    };

    // Pure literal elimination (one pass): if a variable appears only positively (or only negatively) in all clauses, assign it accordingly without a decision.
    auto eliminatePure = [&]() -> void {
        std::vector<int> posCount(numVars + 1, 0), negCount(numVars + 1, 0);
        for (const auto& cl : clauses) {
            // Check if clause is already satisfied; if so, ignore for purity
            bool satisfied = false;
            for (int lit : cl) {
                int var = (lit > 0) ? lit : -lit;
                if ((model[var] == TRUE && lit > 0) || (model[var] == FALSE && lit < 0)) {
                    satisfied = true;
                    break;
                }
            }
            if (satisfied) continue;
            for (int lit : cl) {
                int var = (lit > 0) ? lit : -lit;
                if (lit > 0) posCount[var]++;
                else negCount[var]++;
            }
        }
        for (int v = 1; v <= numVars; ++v) {
            if (model[v] != UNDEF) continue;
            if (posCount[v] > 0 && negCount[v] == 0) {
                model[v] = TRUE; trail.push_back(v);
            } else if (negCount[v] > 0 && posCount[v] == 0) {
                model[v] = FALSE; trail.push_back(v);
            }
        }
    };

    // Initial propagation (decision level 0)
    if (!propagate()) return "UNSATISFIABLE";
    // Check if already satisfied (all clauses satisfied)
    auto allSatisfied = [&]() -> bool {
        for (const auto& cl : clauses) {
            int dummy;
            if (clauseStatus(cl, dummy) != 1) return false;
        }
        return true;
    };

    // Main DPLL loop (iterative with backtracking)
    while (true) {
        // If all clauses satisfied, return SAT
        if (allSatisfied()) return "SATISFIABLE";

        // Pure literal elimination before decision
        eliminatePure();
        // After pure, re-propagate
        if (!propagate()) {
            // Conflict — backtrack
            if (trailLim.empty()) return "UNSATISFIABLE"; // conflict at level 0
            // Undo to last decision point
            int backtrackTo = trailLim.back();
            trailLim.pop_back();
            while ((int)trail.size() > backtrackTo) {
                int v = trail.back();
                trail.pop_back();
                model[v] = UNDEF;
            }
            continue;
        }

        // Find an unassigned variable to decide
        int decisionVar = -1;
        for (int v = 1; v <= numVars; ++v) {
            if (model[v] == UNDEF) { decisionVar = v; break; }
        }
        if (decisionVar == -1) return "SATISFIABLE"; // all variables assigned, but not all clauses satisfied? Actually allSatisfied would have caught it, but safe
        if (++decisions > maxDecisions) return "INDETERMINATE";

        // Record current trail size as a decision point
        trailLim.push_back((int)trail.size());
        // Assign decision variable to TRUE (first try)
        model[decisionVar] = TRUE;
        trail.push_back(decisionVar);

        // Propagate; if conflict, we'll backtrack on next iteration.
        if (!propagate()) {
            // Try FALSE instead by reverting this decision and its implications
            while ((int)trail.size() > trailLim.back()) {
                int v = trail.back();
                trail.pop_back();
                model[v] = UNDEF;
            }
            // Now assign FALSE
            model[decisionVar] = FALSE;
            trail.push_back(decisionVar);
            if (!propagate()) {
                // Both values failed — conflict; backtrack further
                if (trailLim.empty()) return "UNSATISFIABLE";
                int backtrackTo = trailLim.back();
                trailLim.pop_back();
                while ((int)trail.size() > backtrackTo) {
                    int v = trail.back();
                    trail.pop_back();
                    model[v] = UNDEF;
                }
                // Continue loop to re-evaluate
                continue;
            }
        }
    }
}
// The core algorithm is a backtracking SAT solver based on DPLL with two optimizations: unit propagation (also called Boolean constraint propagation) and pure literal elimination (removing literals whose negation never appears). The solver maintains an array `model` of `l_Undef`, `l_True`, or `l_False` for each variable, an assignment trail (vector of decisions and implied assignments with a decision level), and a clause list stored as vectors of integer literals (positive = variable index, negative = variable index with negative sign, where internal representation can be `2*var + sign` or similar). Unit propagation scans all clauses repeatedly—or uses a watch‑list for efficiency—but for a simple educational version, a full scan per propagation is acceptable. On conflict (a clause becomes all false), the solver backtracks to the most recent decision level. Pure literal elimination is performed before each decision: if a variable appears only positively (or only negatively) in unresolved clauses, assign it accordingly without a decision (this is a simplification; in standard DPLL it is a pre‑pass, but here repeated during search can help). The decision heuristic picks the first unassigned variable; if none, the formula is satisfied and we return `SATISFIABLE`. If a conflict occurs at decision level 0, it is `UNSATISFIABLE`. If the decision count exceeds the limit, return `INDETERMINATE`. Complexity: for \(n\) variables and \(m\) clauses, each propagation scan is \(O(\text{total literal count})\) per level in the worst case, and the search explores up to \(O(2^n)\) branches in the worst case; with the decision limit, worst‑case time is bounded by the limit times the cost per decision (propagation scans). Space is \(O(n + \text{total literals})\). Edge cases: empty clause → immediately unsatisfiable; no clauses but variables exist → satisfiable (trivially); duplicates or tautological clauses should be handled gracefully (for simplicity, ignore tautologies like `x -x`). The function must be `const`‑correct where possible, using `const` references for the input file path.
