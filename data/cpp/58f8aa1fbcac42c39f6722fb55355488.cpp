Write a C++ function named `solveLinearSystem` that takes a vector of vectors of doubles representing an augmented matrix of a linear system (where each inner vector has length `n+1` for `n` variables, the last element being the constant term) and returns a string summarizing the solution. The output must follow these rules: if the system is empty (no equations), return `"Empty system"`; if inconsistent, return `"System is not consistent"`; if it has a unique solution, return each variable as `"x_i = value"` (use fixed notation, one per line, with variables indexed from 0); if it has infinitely many solutions, return a parametric solution in the form `"x_i = particular_i + c0*coeff0_i + c1*coeff1_i + ..."` (where `particular_i` is the particular solution component, coefficients are shown with fixed notation, and if the system is homogeneous, omit the particular part entirely). Assume the matrix is already in row-echelon form after Gaussian elimination with partial pivoting, but you must still handle zero rows, dependent variables, and edge cases like zero row for consistency checks. The function must not modify the input matrix and must be `const`-correct.
#include <cassert>
#include <vector>
#include <string>

// The solution function is declared above; assume it's included.

int main() {
    // Empty system
    std::vector<std::vector<double>> empty = {};
    assert(solveLinearSystem(empty) == "Empty system");

    // Inconsistent system: 0 = 5
    std::vector<std::vector<double>> inconsistent = {{0.0, 0.0, 5.0}};
    assert(solveLinearSystem(inconsistent) == "System is not consistent");

    // Unique solution: x0 = 1, x1 = 2 from x0 + x1 = 3, x0 - x1 = -1
    std::vector<std::vector<double>> unique = {{1.0, 1.0, 3.0}, {1.0, -1.0, -1.0}};
    std::string ures = solveLinearSystem(unique);
    assert(ures.find("x0 = 1.000000") != std::string::npos);
    assert(ures.find("x1 = 2.000000") != std::string::npos);

    // Dependent homogeneous: x0 + x1 = 0 -> x0 = -x1, free x1
    std::vector<std::vector<double>> depHom = {{1.0, 1.0, 0.0}};
    std::string dures = solveLinearSystem(depHom);
    assert(dures.find("x0 = -1.000000*c0") != std::string::npos);
    assert(dures.find("x1 = 1.000000*c0") != std::string::npos);

    // Dependent non-homogeneous: x0 + x1 = 2 -> x0 = 2 - x1, free x1
    std::vector<std::vector<double>> depNonHom = {{1.0, 1.0, 2.0}};
    std::string dnres = solveLinearSystem(depNonHom);
    assert(dnres.find("x0 = 1.000000 + -1.000000*c0") != std::string::npos);
    assert(dnres.find("x1 = 1.000000 + 1.000000*c0") != std::string::npos);

    // System with zero row that is removed: x0 = 3, 0 = 0
    std::vector<std::vector<double>> withZero = {{1.0, 0.0, 3.0}, {0.0, 0.0, 0.0}};
    std::string zres = solveLinearSystem(withZero);
    assert(zres.find("x0 = 3.000000") != std::string::npos);

    // Three variables, two equations: x0 + x1 + x2 = 4, x1 + x2 = 1 -> x0 = 3, free x2, x1 = 1 - x2
    std::vector<std::vector<double>> threeVar = {{1.0, 1.0, 1.0, 4.0}, {0.0, 1.0, 1.0, 1.0}};
    std::string tvres = solveLinearSystem(threeVar);
    assert(tvres.find("x0 = 3.000000") != std::string::npos);
    assert(tvres.find("x1 = 1.000000 + -1.000000*c0") != std::string::npos);
    assert(tvres.find("x2 = 0.000000 + 1.000000*c0") != std::string::npos);

    return 0;
}
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <algorithm>

// Return a string describing the solution of a linear system from its augmented matrix.
std::string solveLinearSystem(const std::vector<std::vector<double>>& augmented) {
    const double EPS = 1e-9;
    // Copy the matrix to avoid modifying input
    std::vector<std::vector<double>> mat = augmented;
    int rows = static_cast<int>(mat.size());
    if (rows == 0) return "Empty system";
    int cols = static_cast<int>(mat[0].size()); // number of variables + 1
    int n = cols - 1;

    // Remove zero rows (all coefficients and constant zero)
    std::vector<std::vector<double>> cleaned;
    for (const auto& row : mat) {
        bool allZero = true;
        for (double val : row) {
            if (std::fabs(val) > EPS) { allZero = false; break; }
        }
        if (!allZero) cleaned.push_back(row);
    }
    mat = cleaned;
    rows = static_cast<int>(mat.size());
    if (rows == 0) return "Empty system";

    // Check consistency: last row with all zero coefficients but nonzero constant
    int lastRow = rows - 1;
    bool allCoeffZero = true;
    for (int i = 0; i < n; ++i) {
        if (std::fabs(mat[lastRow][i]) > EPS) { allCoeffZero = false; break; }
    }
    if (allCoeffZero && std::fabs(mat[lastRow][n]) > EPS) {
        return "System is not consistent";
    }

    // Determine if system is independent (unique solution)
    bool independent = (rows == n);

    std::ostringstream out;
    out << std::fixed;

    if (independent) {
        // Back-substitution for unique solution
        std::vector<double> result(n, 0.0);
        for (int i = n - 1; i >= 0; --i) {
            double sum = mat[i][n];
            for (int j = i + 1; j < n; ++j) {
                sum -= mat[i][j] * result[j];
            }
            if (std::fabs(mat[i][i]) < EPS) {
                // Should not happen if independent and properly reduced, but guard
                result[i] = 0.0;
            } else {
                result[i] = sum / mat[i][i];
            }
        }
        for (int i = 0; i < n; ++i) {
            out << "x" << i << " = " << result[i] << "\n";
        }
        std::string res = out.str();
        if (!res.empty() && res.back() == '\n') res.pop_back();
        return res;
    }

    // Dependent system: identify pivot and free variables
    std::vector<int> pivotCols;    // column indices that are pivot columns
    std::vector<int> freeCols;     // column indices that are free variables
    int rowIdx = 0;
    for (int col = 0; col < n && rowIdx < rows; ++col) {
        // Find first row with non-zero in this column at or below rowIdx
        int pivotRow = rowIdx;
        while (pivotRow < rows && std::fabs(mat[pivotRow][col]) <= EPS) pivotRow++;
        if (pivotRow < rows) {
            pivotCols.push_back(col);
            rowIdx++;
        } else {
            freeCols.push_back(col);
        }
    }
    // Add remaining columns as free
    for (int col = (pivotCols.empty() ? 0 : pivotCols.back() + 1); col < n; ++col) {
        if (std::find(pivotCols.begin(), pivotCols.end(), col) == pivotCols.end()) {
            freeCols.push_back(col);
        }
    }

    // Check homogeneity
    bool homogeneous = true;
    for (const auto& row : mat) {
        if (std::fabs(row[n]) > EPS) { homogeneous = false; break; }
    }

    // Helper to back-substitute given values for free variables
    auto backSubstitute = [&](const std::vector<double>& freeValues) {
        std::vector<double> full(n, 0.0);
        for (size_t i = 0; i < freeCols.size(); ++i) {
            full[freeCols[i]] = freeValues[i];
        }
        // Process pivot rows from bottom to top
        for (int i = rows - 1; i >= 0; --i) {
            int pivotCol = -1;
            for (int j = 0; j < n; ++j) {
                if (std::fabs(mat[i][j]) > EPS) { pivotCol = j; break; }
            }
            if (pivotCol == -1) continue;
            double sum = mat[i][n];
            for (int j = pivotCol + 1; j < n; ++j) {
                sum -= mat[i][j] * full[j];
            }
            if (std::fabs(mat[i][pivotCol]) > EPS) {
                full[pivotCol] = sum / mat[i][pivotCol];
            }
        }
        return full;
    };

    // Find particular solution (for non-homogeneous) or zero for homogeneous
    std::vector<double> particular(n, 0.0);
    if (!homogeneous) {
        // Set all free variables to 1 (as per original code's convention)
        std::vector<double> freeVals(freeCols.size(), 1.0);
        particular = backSubstitute(freeVals);
    } else {
        particular.assign(n, 0.0);
    }

    // Generate fundamental solutions: one per free variable
    std::vector<std::vector<double>> fundamentals;
    for (size_t f = 0; f < freeCols.size(); ++f) {
        std::vector<double> freeVals(freeCols.size(), 0.0);
        freeVals[f] = 1.0;
        fundamentals.push_back(backSubstitute(freeVals));
    }

    // Build output
    const int numFree = static_cast<int>(freeCols.size());
    for (int i = 0; i < n; ++i) {
        out << "x" << i << " = ";
        if (!homogeneous) {
            out << particular[i];
            if (numFree > 0) out << " + ";
        }
        for (int j = 0; j < numFree; ++j) {
            out << fundamentals[j][i] << "*c" << j;
            if (j < numFree - 1) out << " + ";
        }
        if (numFree == 0 && homogeneous) {
            out << 0;
        }
        out << "\n";
    }
    std::string res = out.str();
    if (!res.empty() && res.back() == '\n') res.pop_back();
    return res;
}
// The solution mimics the classic Gaussian elimination back-substitution and general solution derivation. First, we remove any all-zero rows (both coefficients and constant) since they do not affect the solution; if this leaves an empty matrix, return "Empty system". To check consistency, if the last remaining row has all zero coefficients but a nonzero constant, the system is inconsistent. If the number of remaining rows equals the number of variables, it’s independent: use back-substitution from bottom to top, solving for each pivot variable, and produce `"x_i = value"` lines. If there are more variables than equations (dependent system), we identify free variables as columns with no pivot. For a homogeneous system (all constants zero), we construct the fundamental solution by setting each free variable to 1 in turn while others are 0, and back-substitute to find dependent variables; format as `"x_i = c0*coeff + c1*..."`. For a non-homogeneous dependent system, we first find a particular solution by setting all free variables to 1 (or 0 typically, but the given code uses 1; we follow that) and back-substituting, then solve the homogeneous system for the fundamental solutions, and combine them as `"x_i = particular + c0*... + c1*..."`. Edge cases include zero rows at the bottom (already removed), rows with leading zero columns, and floating-point comparisons using an epsilon (e.g., 1e-9). Time complexity is O(n^3) in the worst case for back-substitution on dependent systems, but typically O(n^2) for independent; space complexity is O(n) for the result, plus O(n^2) for fundamental solutions in dependent cases.
