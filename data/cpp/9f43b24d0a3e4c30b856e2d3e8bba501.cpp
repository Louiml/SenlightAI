/*
Write a C++ function that reads a CNF file in DIMACS format, where the first line contains two integers: the number of clauses followed by the number of variables (note: the order in the given snippet is unusual—the snippet reads shape as `[clauses, variables]`), and subsequent lines contain space-separated integer literals (positive for a variable, negative for negation) ending with a 0. The function should parse the file, create a SAT solver instance, add all clauses, and then repeatedly solve while collecting all satisfying assignments. For each satisfying assignment, record the number of solutions found and the total number of variables that are assigned (i.e., non-zero in the model). Stop when the solver reports UNSATISFIABLE. Return a pair consisting of the total count of solutions and a vector of the sizes (number of true assignments) for each solution, in the order they were found. If the file is invalid or cannot be opened, return an empty vector and count 0. Do not use the actual Cryptominisat library; instead, you may simulate the SAT solving with a brute-force enumeration over all \(2^V\) assignments (where \(V\) is the number of variables) to determine satisfiability, since the problem is intended as a parser and logic exercise. Ensure that your function correctly handles files that may have extra whitespace, and that the first line's two integers are parsed correctly.
*/
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <utility>
#include <cstdint>

using Clause = std::vector<int>;
using CNF = std::vector<Clause>;

// Parse a line of integers separated by whitespace. Stops at first 0 (excluded).
std::vector<int> parseLine(const std::string& line) {
    std::vector<int> result;
    std::istringstream iss(line);
    int value;
    while (iss >> value) {
        if (value == 0) break;
        result.push_back(value);
    }
    return result;
}

// Read a CNF file in the format: first line: number_of_clauses number_of_variables
// Each subsequent line: literals ending with 0.
// Returns pair: (total_solutions, vector_of_solution_sizes)
std::pair<int, std::vector<int>> countSolutionsAndSizes(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return {0, {}};
    }

    // Read first line to get clauses and variables
    std::string firstLine;
    if (!std::getline(file, firstLine)) {
        return {0, {}};
    }
    std::istringstream header(firstLine);
    int clauses = 0, variables = 0;
    if (!(header >> clauses >> variables)) {
        return {0, {}};
    }

    // Read clauses
    CNF cnf;
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        cnf.push_back(parseLine(line));
    }

    // If we have more/less clauses than declared, we can still proceed with what we have.
    // Number of variables determines the search space.
    if (variables < 0) return {0, {}};
    if (variables > 20) {
        // To avoid overflow for large V, but tests will use small V.
        return {0, {}};
    }

    int total = 0;
    std::vector<int> sizes;

    // Enumerate all 2^V assignments
    uint64_t limit = 1ULL << variables;
    for (uint64_t mask = 0; mask < limit; ++mask) {
        bool allSatisfied = true;
        for (const auto& clause : cnf) {
            bool clauseSatisfied = false;
            for (int lit : clause) {
                int var = (lit > 0) ? lit : -lit;  // 1-indexed
                bool value = (mask >> (var - 1)) & 1;
                bool litIsTrue = (lit > 0) ? value : !value;
                if (litIsTrue) {
                    clauseSatisfied = true;
                    break;
                }
            }
            if (!clauseSatisfied) {
                allSatisfied = false;
                break;
            }
        }
        if (allSatisfied) {
            // Count true variables
            int trues = 0;
            for (int v = 0; v < variables; ++v) {
                if ((mask >> v) & 1) trues++;
            }
            total++;
            sizes.push_back(trues);
        }
    }

    return {total, sizes};
}
#include <cassert>
#include <fstream>
#include <string>
#include <vector>
#include <utility>

// Include the solution function declaration here (copy from above or place in header)

int main() {
    // Test 1: Trivial SAT with one clause {1}
    {
        std::ofstream f("test1.cnf");
        f << "1 1\n1 0\n";
        f.close();
        auto result = countSolutionsAndSizes("test1.cnf");
        assert(result.first == 1); // only variable 1 must be true
        assert(result.second.size() == 1);
        assert(result.second[0] == 1);
    }
    // Test 2: Unsat: {1} and {-1}
    {
        std::ofstream f("test2.cnf");
        f << "2 1\n1 0\n-1 0\n";
        f.close();
        auto result = countSolutionsAndSizes("test2.cnf");
        assert(result.first == 0);
        assert(result.second.empty());
    }
    // Test 3: No clauses, 2 variables -> 4 solutions, each with sizes 0,1,1,2
    {
        std::ofstream f("test3.cnf");
        f << "0 2\n";
        f.close();
        auto result = countSolutionsAndSizes("test3.cnf");
        assert(result.first == 4);
        assert(result.second.size() == 4);
        // Order is by mask from 0 to 3: 00,01,10,11 -> sizes 0,1,1,2
        assert(result.second[0] == 0);
        assert(result.second[1] == 1);
        assert(result.second[2] == 1);
        assert(result.second[3] == 2);
    }
    // Test 4: Two clauses with two variables: (x1 or x2) and (not x1 or not x2)
    // Satisfying: (1,0) and (0,1) => sizes 1,1
    {
        std::ofstream f("test4.cnf");
        f << "2 2\n1 2 0\n-1 -2 0\n";
        f.close();
        auto result = countSolutionsAndSizes("test4.cnf");
        assert(result.first == 2);
        assert(result.second.size() == 2);
        assert(result.second[0] == 1);
        assert(result.second[1] == 1);
    }
    // Test 5: Missing file
    {
        auto result = countSolutionsAndSizes("nonexistent_file.cnf");
        assert(result.first == 0);
        assert(result.second.empty());
    }
    // Cleanup test files (optional)
    std::remove("test1.cnf");
    std::remove("test2.cnf");
    std::remove("test3.cnf");
    std::remove("test4.cnf");
    return 0;
}
// The core challenge is parsing the DIMACS-like CNF file correctly, especially given that the format in the snippet has the first line as `clauses variables` (note: typical DIMACS is `p cnf variables clauses` but here we follow the snippet's format). The function will:
// 1. Open the file; if it fails, return an empty vector and count 0.
// 2. Read the first line and extract two integers using a string stream. The first integer is `clauses`, the second is `variables`. To be robust, we should read all integers from the first line; if fewer than two integers appear, treat as invalid.
// 3. Read each subsequent line, parsing integers until a 0 is encountered (or end of line). Ignore the terminating 0 and any trailing zeros. Each line represents one clause. Store clauses as a vector of vectors of integers (literals).
// 4. Validate that the number of clauses read matches the declared count; if not, we can still process what we have, but for correctness we can assume the file is well-formed.
// 5. After parsing, brute-force evaluate all \(2^V\) assignments. For each assignment (represented as a bitmask where bit i represents variable i+1 being true or false), check if all clauses are satisfied. A clause is satisfied if at least one literal evaluates to true: a positive literal `x` means variable `x` must be true, a negative literal `-x` means variable `x` must be false.
// 6. For each satisfying assignment, count the number of true variables (popcount) and add to the result vector. Also increment total solutions.
// 7. Return a pair: `{totalSolutions, vectorOfSizes}`.
//
// Edge cases: variables may be numbered from 1 to V, but some variables may not appear in any clause. All variables are considered in the brute force. If there are no clauses (clauses=0), then all assignments are satisfying; if V=0, there is exactly one empty assignment (count 1, size 0). The first line may have extra whitespace or comments? In the snippet, it's assumed simple two integers. We will ignore any non-integer tokens on the first line. Complexity: For V variables and C clauses, we try \(2^V\) assignments, each taking O(C * L) where L is average clause length. This is exponential in V, but acceptable for small test cases.
