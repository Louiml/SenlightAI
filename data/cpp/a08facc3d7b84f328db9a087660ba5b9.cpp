// Write a C++ function `std::vector<double> solveTriDiagonalSystem(const std::vector<double>& lower, const std::vector<double>& diagonal, const std::vector<double>& upper, const std::vector<double>& rhs)` that solves a tridiagonal system of linear equations \(A x = b\), where \(A\) is an \(n \times n\) tridiagonal matrix stored via three vectors: `lower` (length \(n-1\), sub-diagonal entries `lower[i]` is the entry at row `i+1`, column `i`), `diagonal` (length \(n\), main diagonal entries `diagonal[i]` is entry at row `i`, column `i`), and `upper` (length \(n-1\), super-diagonal entries `upper[i]` is the entry at row `i`, column `i+1`). The `rhs` vector has length \(n\). The function must use the Thomas algorithm (a specialized Gaussian elimination without pivoting) with forward elimination and back substitution. It should handle edge cases gracefully: if `n == 0`, return an empty vector; if `n == 1`, return a vector containing `rhs[0] / diagonal[0]` (assuming diagonal[0] is nonzero). The function must assume the matrix is diagonally dominant (which guarantees the algorithm works without pivoting), but should detect a zero pivot during the elimination and return an empty vector if encountered. The returned vector should contain the solution \(x\) such that \(A x = b\).
// The Thomas algorithm solves a tridiagonal system in \(O(n)\) time and \(O(1)\) extra space beyond the input and output. It consists of two phases:
//
// 1. **Forward elimination (modified coefficients):** For each row \(i\) from 1 to \(n-1\), compute a multiplier `m = lower[i-1] / diagonal[i-1]`. Update the diagonal element at row `i` as `diagonal[i] -= m * upper[i-1]`, and update the right-hand side as `rhs[i] -= m * rhs[i-1]`. If at any step the updated diagonal element becomes zero (or very close to zero), the system is singular or not strictly diagonally dominant enough, and we return an empty vector. The super-diagonal entries do not need modification because the new coefficient in row `i` for column `i+1` is unchanged (the eliminated term only affects column `i` which becomes zero). This phase eliminates the lower sub-diagonal, transforming the system into an upper-bidiagonal system.
//
// 2. **Back substitution:** Compute `x[n-1] = rhs[n-1] / diagonal[n-1]`. Then for `i = n-2` down to 0, compute `x[i] = (rhs[i] - upper[i] * x[i+1]) / diagonal[i]`. This yields the solution.
//
// Edge cases: 
// - `n == 0` returns an empty vector.
// - `n == 1` directly returns `{rhs[0] / diagonal[0]}` if `diagonal[0]` is nonzero, otherwise empty.
// - During elimination, if the pivot (`diagonal[i]` after updates) is near zero (e.g., absolute value less than a small epsilon like `1e-12`), return empty vector to signal failure.
// - The algorithm assumes the input vectors have the correct sizes (`lower.size() == n-1` or `n-1` when `n > 0`; `upper.size() == n-1` or `n-1` when `n > 0`). If sizes mismatch, return empty vector.
//
// Time complexity: \(O(n)\). Space complexity: \(O(n)\) for the output vector, but the algorithm mutates copies of `diagonal` and `rhs` to avoid modifying the caller's data, so it uses \(O(n)\) auxiliary space for those copies. If we were allowed to mutate input, it would be \(O(1)\) extra, but for safety we copy them.
#include <vector>
#include <cmath>
#include <cstddef>

// Solve a tridiagonal system A x = b using the Thomas algorithm.
// lower[i] = sub-diagonal at row i+1, column i (length n-1)
// diagonal[i] = main diagonal at row i, column i (length n)
// upper[i] = super-diagonal at row i, column i+1 (length n-1)
// rhs[i] = right-hand side at row i (length n)
// Returns the solution vector, or an empty vector if the sizes are invalid,
// a zero pivot is encountered, or n == 0.
std::vector<double> solveTriDiagonalSystem(
    const std::vector<double>& lower,
    const std::vector<double>& diagonal,
    const std::vector<double>& upper,
    const std::vector<double>& rhs)
{
    const std::size_t n = diagonal.size();
    if (n == 0) return {};
    if (lower.size() != n - 1 || upper.size() != n - 1 || rhs.size() != n) return {};

    // Work on copies to avoid mutating the input.
    std::vector<double> diag = diagonal;
    std::vector<double> b = rhs;

    const double epsilon = 1e-12;

    // Handle n == 1 directly.
    if (n == 1) {
        if (std::fabs(diag[0]) < epsilon) return {};
        return { b[0] / diag[0] };
    }

    // Forward elimination.
    for (std::size_t i = 1; i < n; ++i) {
        if (std::fabs(diag[i - 1]) < epsilon) return {}; // zero pivot in previous row
        const double m = lower[i - 1] / diag[i - 1];
        diag[i] -= m * upper[i - 1];
        b[i] -= m * b[i - 1];
        if (std::fabs(diag[i]) < epsilon && i == n - 1) {
            // Final pivot must also be nonzero; check here or after loop.
        }
    }

    // Check the final pivot.
    if (std::fabs(diag[n - 1]) < epsilon) return {};

    // Back substitution.
    std::vector<double> x(n);
    x[n - 1] = b[n - 1] / diag[n - 1];
    for (std::size_t i = n - 1; i-- > 0; ) {
        if (std::fabs(diag[i]) < epsilon) return {};
        x[i] = (b[i] - upper[i] * x[i + 1]) / diag[i];
    }

    return x;
}
#include <cassert>
#include <cmath>
#include <vector>

// (The solution function is assumed to be defined above.)

int main() {
    // Example 1: 2x2 system
    // [3 1] [x0]   [9]
    // [1 2] [x1] = [8]
    // Solving: 3x0 + x1 = 9; x0 + 2x1 = 8 => x0=2, x1=3
    {
        std::vector<double> lower = {1.0};
        std::vector<double> diag  = {3.0, 2.0};
        std::vector<double> upper = {1.0};
        std::vector<double> rhs   = {9.0, 8.0};
        auto x = solveTriDiagonalSystem(lower, diag, upper, rhs);
        assert(x.size() == 2);
        assert(std::fabs(x[0] - 2.0) < 1e-9);
        assert(std::fabs(x[1] - 3.0) < 1e-9);
    }

    // Example 2: 3x3 system (diagonally dominant)
    // [4 1 0] [x0]   [10]
    // [1 4 1] [x1] = [14]
    // [0 1 4] [x2] = [14]
    // Solution: x0=2, x1=2, x2=3
    {
        std::vector<double> lower = {1.0, 1.0};
        std::vector<double> diag  = {4.0, 4.0, 4.0};
        std::vector<double> upper = {1.0, 1.0};
        std::vector<double> rhs   = {10.0, 14.0, 14.0};
        auto x = solveTriDiagonalSystem(lower, diag, upper, rhs);
        assert(x.size() == 3);
        assert(std::fabs(x[0] - 2.0) < 1e-9);
        assert(std::fabs(x[1] - 2.0) < 1e-9);
        assert(std::fabs(x[2] - 3.0) < 1e-9);
    }

    // Example 3: n == 1
    {
        std::vector<double> lower;
        std::vector<double> diag  = {5.0};
        std::vector<double> upper;
        std::vector<double> rhs   = {15.0};
        auto x = solveTriDiagonalSystem(lower, diag, upper, rhs);
        assert(x.size() == 1);
        assert(std::fabs(x[0] - 3.0) < 1e-9);
    }

    // Example 4: n == 0 returns empty
    {
        std::vector<double> lower, diag, upper, rhs;
        auto x = solveTriDiagonalSystem(lower, diag, upper, rhs);
        assert(x.empty());
    }

    // Example 5: invalid sizes returns empty
    {
        std::vector<double> lower = {1.0, 2.0}; // wrong size (should be 2 for n=3, but diag has 3)
        std::vector<double> diag  = {1.0, 1.0, 1.0};
        std::vector<double> upper = {1.0};
        std::vector<double> rhs   = {1.0, 1.0, 1.0};
        auto x = solveTriDiagonalSystem(lower, diag, upper, rhs);
        assert(x.empty());
    }

    // Example 6: zero pivot detection (singular system)
    // [0 1]  [x0]   [1]
    // [1 1]  [x1] = [2]  -- first pivot is zero -> should fail
    {
        std::vector<double> lower = {1.0};
        std::vector<double> diag  = {0.0, 1.0};
        std::vector<double> upper = {1.0};
        std::vector<double> rhs   = {1.0, 2.0};
        auto x = solveTriDiagonalSystem(lower, diag, upper, rhs);
        assert(x.empty());
    }

    // Example 7: larger system (5x5) with known solution x[i] = i+1
    {
        int n = 5;
        std::vector<double> lower(n-1, 1.0);
        std::vector<double> diag(n, 3.0);
        std::vector<double> upper(n-1, 1.0);
        std::vector<double> rhs(n, 0.0);
        // Compute b = A * x where x[i] = i+1
        for (int i = 0; i < n; ++i) {
            if (i > 0) rhs[i] += lower[i-1] * (i);       // x[i-1] = (i-1)+1 = i
            rhs[i] += diag[i] * (i+1);
            if (i < n-1) rhs[i] += upper[i] * (i+2);     // x[i+1] = (i+1)+1 = i+2
        }
        auto x = solveTriDiagonalSystem(lower, diag, upper, rhs);
        assert(x.size() == static_cast<size_t>(n));
        for (int i = 0; i < n; ++i) {
            assert(std::fabs(x[i] - (i+1)) < 1e-9);
        }
    }

    return 0;
}
