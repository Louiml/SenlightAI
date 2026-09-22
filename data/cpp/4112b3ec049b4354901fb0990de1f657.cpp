/*
Write a C++ function named `makePartnerSets` that takes a vector of strings representing student names and a vector of valid combinations, where each combination is a vector of student indices (all combinations contain exactly 2 indices), and returns a `std::vector<std::vector<std::vector<std::string>>>` containing all possible round-robin pairings (also called perfect matchings or 1-factorizations) of the students. Each element of the outer vector represents one full round (a set of disjoint pairs covering all students exactly once), and each round is a vector of pairs (each pair being a vector of two student names). The function must use an exact-cover approach: build an incidence matrix where each row corresponds to a combination and each column corresponds to a student, then repeatedly solve the exact-cover problem (Algorithm X) to find one round at a time, remove the used combinations from future rounds, and continue until no more complete rounds can be formed. The input will always be such that the number of students is even and at least 2. The combinations are guaranteed to contain all possible pairs between students, but the order of indices within each combination is arbitrary, and combinations may appear in any order in the input vector. The returned rounds should not repeat any combination across different rounds.
*/

#include <vector>
#include <string>
#include <algorithm>

using StudentSet = std::vector<std::vector<std::vector<std::string>>>;

// Helper: recursive exact-cover solver (Algorithm X).
// Returns true if a complete cover is found, filling 'solution' with row indices.
bool solveExactCover(const std::vector<std::vector<bool>>& matrix,
                     std::vector<bool>& availableRows,
                     std::vector<std::size_t>& solution) {
    if (availableRows.empty()) return false; // No rows at all -> no solution.

    // Find the column with the fewest available rows covering it.
    int minCount = -1;
    std::size_t bestCol = 0;
    std::size_t numCols = matrix.empty() ? 0 : matrix[0].size();
    for (std::size_t col = 0; col < numCols; ++col) {
        int count = 0;
        for (std::size_t row = 0; row < matrix.size(); ++row) {
            if (availableRows[row] && matrix[row][col]) ++count;
        }
        if (count == 0) return false; // Uncovered column -> dead end.
        if (minCount == -1 || count < minCount) {
            minCount = count;
            bestCol = col;
            if (minCount == 1) break; // Can't do better.
        }
    }

    // Try each row covering the best column.
    for (std::size_t row = 0; row < matrix.size(); ++row) {
        if (!availableRows[row] || !matrix[row][bestCol]) continue;

        // Tentatively select this row.
        solution.push_back(row);
        std::vector<std::size_t> affectedCols;
        std::vector<std::size_t> affectedRows;
        // Delete all columns covered by this row, and all rows that intersect those columns.
        for (std::size_t col = 0; col < numCols; ++col) {
            if (matrix[row][col]) {
                affectedCols.push_back(col);
                for (std::size_t r = 0; r < matrix.size(); ++r) {
                    if (availableRows[r] && matrix[r][col]) {
                        affectedRows.push_back(r);
                        availableRows[r] = false;
                    }
                }
            }
        }

        // Recurse.
        if (solveExactCover(matrix, availableRows, solution)) {
            return true;
        }

        // Backtrack: restore rows.
        for (std::size_t r : affectedRows) {
            availableRows[r] = true;
        }
        solution.pop_back();
    }
    return false;
}

// Main function: produce all round-robin rounds.
StudentSet makePartnerSets(const std::vector<std::string>& allStudents,
                           const std::vector<std::vector<std::size_t>>& combos) {
    StudentSet rounds;

    // Build incidence matrix: rows = combos, columns = all students.
    std::size_t numRows = combos.size();
    std::size_t numCols = allStudents.size();
    std::vector<std::vector<bool>> matrix(numRows, std::vector<bool>(numCols, false));
    for (std::size_t i = 0; i < numRows; ++i) {
        for (std::size_t j = 0; j < combos[i].size(); ++j) {
            matrix[i][combos[i][j]] = true;
        }
    }

    // Track which rows are still available.
    std::vector<bool> availableRows(numRows, true);

    // Repeatedly find one exact-cover solution per round.
    while (true) {
        std::vector<std::size_t> solution;
        std::vector<bool> availCopy = availableRows; // Solve on a copy so we can restore if needed.
        if (!solveExactCover(matrix, availCopy, solution)) {
            break;
        }

        // Convert solution rows into a round (vector of pairs).
        std::vector<std::vector<std::string>> thisRound;
        for (std::size_t rowIdx : solution) {
            std::vector<std::string> pair;
            for (std::size_t col = 0; col < numCols; ++col) {
                if (matrix[rowIdx][col]) {
                    pair.push_back(allStudents[col]);
                }
            }
            thisRound.push_back(pair);
        }
        rounds.push_back(thisRound);

        // Remove used rows from future rounds.
        for (std::size_t rowIdx : solution) {
            availableRows[rowIdx] = false;
        }
    }

    return rounds;
}

#include <cassert>
#include <vector>
#include <string>

// Include the solution function here (or link it).

int main() {
    // Test 1: 2 students -> 1 round, 1 pair.
    {
        std::vector<std::string> students = {"A", "B"};
        std::vector<std::vector<std::size_t>> combos = {{0,1}};
        auto result = makePartnerSets(students, combos);
        assert(result.size() == 1);
        assert(result[0].size() == 1);
        assert(result[0][0] == std::vector<std::string>({"A","B"}));
    }

    // Test 2: 4 students -> 3 rounds, each round has 2 pairs covering all students.
    {
        std::vector<std::string> students = {"A", "B", "C", "D"};
        std::vector<std::vector<std::size_t>> combos = {
            {0,1}, {0,2}, {0,3}, {1,2}, {1,3}, {2,3}
        };
        auto result = makePartnerSets(students, combos);
        assert(result.size() == 3);
        for (const auto& round : result) {
            assert(round.size() == 2);
            // Check each student appears exactly once in the round.
            std::vector<bool> seen(4, false);
            for (const auto& pair : round) {
                assert(pair.size() == 2);
                for (const std::string& name : pair) {
                    int idx = (name == "A") ? 0 : (name == "B") ? 1 : (name == "C") ? 2 : 3;
                    assert(!seen[idx]);
                    seen[idx] = true;
                }
            }
        }
        // Ensure all pairs are unique across rounds.
        std::vector<std::string> allPairs;
        for (const auto& round : result) {
            for (const auto& pair : round) {
                std::string p = pair[0] + pair[1];
                if (p[0] > p[1]) std::swap(p[0], p[1]);
                allPairs.push_back(p);
            }
        }
        assert(allPairs.size() == 6);
        std::sort(allPairs.begin(), allPairs.end());
        assert(std::unique(allPairs.begin(), allPairs.end()) == allPairs.end());
    }

    // Test 3: 6 students -> 5 rounds.
    {
        std::vector<std::string> students = {"S0", "S1", "S2", "S3", "S4", "S5"};
        std::vector<std::vector<std::size_t>> combos;
        for (std::size_t i = 0; i < 6; ++i) {
            for (std::size_t j = i+1; j < 6; ++j) {
                combos.push_back({i, j});
            }
        }
        auto result = makePartnerSets(students, combos);
        assert(result.size() == 5);
        assert(result[0].size() == 3);
        // Check total pairs across all rounds = 15.
        size_t totalPairs = 0;
        for (const auto& round : result) totalPairs += round.size();
        assert(totalPairs == 15);
    }

    // Test 4: combos order does not matter (random shuffled input).
    {
        std::vector<std::string> students = {"X", "Y", "Z", "W"};
        std::vector<std::vector<std::size_t>> combos = {
            {2,3}, {0,2}, {1,3}, {0,1}, {1,2}, {0,3}
        };
        auto result = makePartnerSets(students, combos);
        assert(result.size() == 3);
    }

    // Test 5: Use a larger even number (8 students) to ensure termination.
    {
        std::vector<std::string> students = {"0","1","2","3","4","5","6","7"};
        std::vector<std::vector<std::size_t>> combos;
        for (std::size_t i = 0; i < 8; ++i) {
            for (std::size_t j = i+1; j < 8; ++j) {
                combos.push_back({i, j});
            }
        }
        auto result = makePartnerSets(students, combos);
        assert(result.size() == 7); // n-1 rounds
        size_t totalPairs = 0;
        for (const auto& round : result) totalPairs += round.size();
        assert(totalPairs == 28); // all pairs used exactly once.
    }

    return 0;
}

// The core challenge is to partition all unordered pairs of students into maximal sets of disjoint pairs that cover every student exactly once. This is exactly the problem of finding all perfect matchings in a complete graph with an even number of vertices, also known as a round-robin tournament schedule (standard circle method yields n-1 rounds for n students). The intended algorithm uses Algorithm X (Donald Knuth's dancing links or a simpler recursive backtracking) to solve the exact cover problem. The incidence matrix is built with rows = combinations (all unordered pairs) and columns = students. A row covers exactly two columns (the two student indices). The exact-cover problem asks to select a set of rows such that each column is covered exactly once — that yields a perfect matching. After finding one solution (one round), we mark those rows as unavailable (delete them from future considerations) and solve again. Each iteration yields one round, and we continue until no solution exists. Edge cases: if the number of students is odd, no perfect matching exists, but the input guarantees even. If there are duplicate combinations, they must be handled gracefully (but input guarantees unique pairs). The algorithm must ensure that each round uses each student exactly once, and that across rounds no pair repeats. Time complexity: Algorithm X has exponential worst-case behavior, but for the complete graph of size n, the number of solutions is (n-1)!! (double factorial), and the algorithm will terminate quickly for reasonable n (e.g., up to 10-12). Space complexity: O(n^2) for the incidence matrix and the output structure.
