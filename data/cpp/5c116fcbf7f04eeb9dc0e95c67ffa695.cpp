// Write a C++ function that takes an integer `n` (where `n >= 1`) and returns a `std::vector<double>` containing the eigenvalues of the `n x n` matrix where every entry is `1.0`. Use the Eigen library’s `EigenSolver` on a `MatrixXd` constructed with `MatrixXd::Ones(n, n)`. The function should return only the real parts of the eigenvalues (since for this matrix, all eigenvalues are real), sorted in ascending order. Ensure the function handles the edge case `n = 1` correctly (where the sole eigenvalue is `1`). The function signature should be: `std::vector<double> eigenvaluesOfOnesMatrix(int n)`.

// The eigenvalues of an `n x n` all-ones matrix are well-known: for `n >= 2`, one eigenvalue is `n` (corresponding to the vector of all ones) and the remaining `n-1` eigenvalues are `0` (corresponding to vectors orthogonal to the all-ones vector). For `n = 1`, the matrix is `[1]`, so the only eigenvalue is `1` (which equals `n`).  
// Algorithm: Construct a `MatrixXd` using `MatrixXd::Ones(n, n)`. Use `EigenSolver<MatrixXd>` with the parameter `false` to avoid computing eigenvectors (saving computation). Extract the eigenvalues via `es.eigenvalues()`. Since the eigenvalues are real (the matrix is symmetric), take the real part of each complex eigenvalue using `.real()`. Append these to a `std::vector<double>`, then sort the vector in ascending order using `std::sort`.  
// Edge cases: `n = 1` yields a single eigenvalue `1`. `n` can be large, but the matrix is dense, so memory usage is `O(n^2)`. Time complexity is dominated by the EigenSolver, which for a dense matrix is `O(n^3)` in general (though for this specific matrix, the structure could be exploited, but we follow the prompt's instruction to use EigenSolver). Space complexity is `O(n^2)` for the matrix plus `O(n)` for the output vector. The sorting step is `O(n log n)`.  
// Important: EigenSolver returns complex eigenvalues even for real matrices, so we must extract `.real()` to get double values. Also, due to floating-point precision, eigenvalues that should be exactly 0 may appear as tiny numbers like `1e-15`; after sorting, they will be near 0, which is acceptable for the test cases (we can compare with approximate tolerance).

#include <vector>
#include <algorithm>
#include <Eigen/Dense>

// Returns the real eigenvalues of the n x n all-ones matrix, sorted ascending.
std::vector<double> eigenvaluesOfOnesMatrix(int n) {
    // Construct the n x n matrix with all entries equal to 1.0.
    Eigen::MatrixXd ones = Eigen::MatrixXd::Ones(n, n);

    // Compute eigenvalues only (no eigenvectors) for efficiency.
    Eigen::EigenSolver<Eigen::MatrixXd> solver(ones, /* computeEigenvectors = */ false);

    // Extract the eigenvalues (complex) and take real parts.
    std::vector<double> eigenvalues;
    eigenvalues.reserve(n);
    for (int i = 0; i < n; ++i) {
        eigenvalues.push_back(solver.eigenvalues()[i].real());
    }

    // Sort in ascending order.
    std::sort(eigenvalues.begin(), eigenvalues.end());
    return eigenvalues;
}

#include <cassert>
#include <cmath>
#include <vector>

// The solution function is assumed to be available from the section.

int main() {
    // Helper to compare vectors with absolute tolerance.
    auto approxEqual = [](const std::vector<double>& a, const std::vector<double>& b, double tol = 1e-9) {
        if (a.size() != b.size()) return false;
        for (size_t i = 0; i < a.size(); ++i) {
            if (std::fabs(a[i] - b[i]) > tol) return false;
        }
        return true;
    };

    // n = 1
    {
        auto result = eigenvaluesOfOnesMatrix(1);
        assert(approxEqual(result, {1.0}));
    }

    // n = 2: eigenvalues are 2 and 0
    {
        auto result = eigenvaluesOfOnesMatrix(2);
        assert(approxEqual(result, {0.0, 2.0}));
    }

    // n = 3: eigenvalues are 3, 0, 0
    {
        auto result = eigenvaluesOfOnesMatrix(3);
        assert(approxEqual(result, {0.0, 0.0, 3.0}));
    }

    // n = 4: eigenvalues are 4, 0, 0, 0
    {
        auto result = eigenvaluesOfOnesMatrix(4);
        assert(approxEqual(result, {0.0, 0.0, 0.0, 4.0}));
    }

    // n = 5: sorted ascending
    {
        auto result = eigenvaluesOfOnesMatrix(5);
        assert(approxEqual(result, {0.0, 0.0, 0.0, 0.0, 5.0}));
    }

    // n = 10: check that the largest is 10 and the rest are near 0
    {
        auto result = eigenvaluesOfOnesMatrix(10);
        assert(result.size() == 10);
        assert(std::fabs(result.back() - 10.0) < 1e-9);
        for (size_t i = 0; i + 1 < result.size(); ++i) {
            assert(std::fabs(result[i]) < 1e-9);
        }
    }

    return 0;
}
