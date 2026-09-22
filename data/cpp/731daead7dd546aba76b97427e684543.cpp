// Write a standalone C++ function `bool containsMonomial(const std::vector<std::vector<long long>>& matrix, long long targetCoeff)` that checks whether a given coefficient value (non‑zero) appears anywhere in a rectangular 2D matrix of `long long` values. The function must return `true` if at least one matrix cell equals `targetCoeff`, and `false` otherwise. The matrix may be empty (zero rows or zero columns), in which case the result is `false`. You may use only the C++ standard library (e.g., `<vector>`). Do not use any external libraries or classes (such as `polyjam`). The matrix is stored as a vector of rows, each row being a vector of `long long`. The function must be `const`‑correct (i.e., it should accept the matrix as a `const` reference and not modify it). Your solution must include the function definition and appropriate `#include` directives; do not provide a `main` function in the solution part.

The problem is a straightforward linear search over a 2D container. The main algorithm iterates over each row, and within each row iterates over each column. For each element, compare its value to `targetCoeff`. If any match is found, immediately return `true` (short‑circuiting). If all elements are checked and no match is found, return `false`. Edge cases: (1) An empty matrix (either zero rows or zero columns) is handled naturally because the outer loop runs zero times, yielding `false`. (2) The target may be negative, zero, or large; no special handling is needed because direct equality comparison works for all `long long` values. (3) To be efficient, iterate using range‑based `for` loops over const references to avoid copying rows (though copying `long long` is cheap, the const reference is still idiomatic). Time complexity is \(O(R \times C)\) in the worst case (when no match exists), where \(R\) is the number of rows and \(C\) is the number of columns. Space complexity is \(O(1)\) auxiliary, as we only use a couple of loop variables. No modifications to the matrix are required, so `const` correctness is easily satisfied.

#include <vector>

// Returns true if targetCoeff appears at least once in the given matrix.
bool containsMonomial(const std::vector<std::vector<long long>>& matrix, long long targetCoeff) {
    for (const auto& row : matrix) {
        for (long long value : row) {
            if (value == targetCoeff) {
                return true;
            }
        }
    }
    return false;
}

#include <cassert>
#include <vector>

// The function under test (copy the code from the Solution section here)
bool containsMonomial(const std::vector<std::vector<long long>>& matrix, long long targetCoeff) {
    for (const auto& row : matrix) {
        for (long long value : row) {
            if (value == targetCoeff) {
                return true;
            }
        }
    }
    return false;
}

int main() {
    // Basic case: target present
    std::vector<std::vector<long long>> m1 = {{1, 2}, {3, 4}};
    assert(containsMonomial(m1, 3) == true);

    // Target absent
    assert(containsMonomial(m1, 5) == false);

    // Single element matrix
    std::vector<std::vector<long long>> m2 = {{42}};
    assert(containsMonomial(m2, 42) == true);
    assert(containsMonomial(m2, -1) == false);

    // Negative values
    std::vector<std::vector<long long>> m3 = {{-7, 0}, {100, -3}};
    assert(containsMonomial(m3, -7) == true);
    assert(containsMonomial(m3, -3) == true);
    assert(containsMonomial(m3, 0) == true);
    assert(containsMonomial(m3, 7) == false);

    // Empty matrix (zero rows)
    std::vector<std::vector<long long>> m4;
    assert(containsMonomial(m4, 1) == false);

    // Matrix with zero columns (rows that are empty)
    std::vector<std::vector<long long>> m5 = {{}, {}};
    assert(containsMonomial(m5, 1) == false);

    // Duplicate values
    std::vector<std::vector<long long>> m6 = {{5, 5}, {5, 5}};
    assert(containsMonomial(m6, 5) == true);

    // Large values (within long long range)
    long long big = 9223372036854775807LL; // LLONG_MAX
    std::vector<std::vector<long long>> m7 = {{1, big}, {2, 3}};
    assert(containsMonomial(m7, big) == true);
    assert(containsMonomial(m7, big - 1) == false);

    return 0;
}
