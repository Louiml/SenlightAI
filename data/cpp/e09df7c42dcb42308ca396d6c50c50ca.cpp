// Write a C++ function that takes a positive integer `n` and returns a string representation of a random symmetric `n × n` matrix with entries uniformly distributed in the interval `[-1, 1]`, followed by its Householder tridiagonalization coefficients (a vector of length `n-2`). The output string must be formatted as: first the matrix rows, each row's entries separated by spaces and rows separated by newlines, then a blank line, then the coefficients separated by spaces. The matrix must be symmetric (i.e., `A[i][j] == A[j][i]`) and generated using a deterministic seed of `12345` for reproducibility. The function should handle `n < 3` by returning just the matrix (since no coefficients exist in that case). Use the Eigen library for matrix operations and tridiagonalization.

#include <cassert>
#include <sstream>
#include <string>
#include <vector>

// Helper to split a string by spaces/newlines into a vector of doubles.
std::vector<double> parseDoubles(const std::string& s) {
    std::istringstream iss(s);
    std::vector<double> vals;
    double v;
    while (iss >> v) {
        vals.push_back(v);
    }
    return vals;
}

int main() {
    // Test n=1: only a 1x1 matrix, no coefficients.
    std::string out1 = symmetricMatrixAndHouseholder(1);
    auto vals1 = parseDoubles(out1);
    assert(vals1.size() == 1);
    assert(vals1[0] >= -2.0 && vals1[0] <= 2.0); // from X+X^T, entry is 2*uniform(-1,1)

    // Test n=2: 2x2 matrix, no coefficients (n-2=0).
    std::string out2 = symmetricMatrixAndHouseholder(2);
    auto vals2 = parseDoubles(out2);
    assert(vals2.size() == 4);
    // Check symmetry property: A(0,1) == A(1,0)
    assert(vals2[1] == vals2[2]);

    // Test n=3: matrix + exactly 1 coefficient.
    std::string out3 = symmetricMatrixAndHouseholder(3);
    auto vals3 = parseDoubles(out3);
    assert(vals3.size() == 9 + 1); // 3x3 + 1 coefficient

    // Test n=4: matrix + 2 coefficients.
    std::string out4 = symmetricMatrixAndHouseholder(4);
    auto vals4 = parseDoubles(out4);
    assert(vals4.size() == 16 + 2);

    // Reproducibility: same call yields identical output string.
    assert(symmetricMatrixAndHouseholder(4) == out4);

    // Test symmetry for n=5: A is symmetric.
    std::string out5 = symmetricMatrixAndHouseholder(5);
    auto vals5 = parseDoubles(out5);
    // Check first 5x5 block for symmetry.
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            assert(vals5[i*5 + j] == vals5[j*5 + i]);
        }
    }
}

#include <Eigen/Dense>
#include <random>
#include <string>
#include <sstream>

// Generate a random symmetric n x n matrix with entries in [-1,1] and its Householder coefficients.
// Returns a string with the matrix rows (space-separated entries) followed by a blank line and
// then the coefficients (space-separated). For n < 3, only the matrix is returned.
std::string symmetricMatrixAndHouseholder(int n) {
    if (n <= 0) {
        return "";
    }

    // Fixed seed for reproducibility
    std::mt19937 gen(12345);
    std::uniform_real_distribution<double> dist(-1.0, 1.0);

    Eigen::MatrixXd X(n, n);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            X(i, j) = dist(gen);
        }
    }
    Eigen::MatrixXd A = X + X.transpose(); // symmetric

    std::ostringstream oss;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (j > 0) oss << " ";
            oss << A(i, j);
        }
        oss << "\n";
    }

    if (n >= 3) {
        Eigen::Tridiagonalization<Eigen::MatrixXd> triOfA(A);
        Eigen::VectorXd hc = triOfA.householderCoefficients(); // size n-2
        oss << "\n";
        for (int i = 0; i < hc.size(); ++i) {
            if (i > 0) oss << " ";
            oss << hc(i);
        }
    }

    return oss.str();
}

// The solution uses Eigen’s `MatrixXd` for dynamic-size matrices. Generate a random matrix `X` by filling entries with uniform random numbers in `[-1,1]` using a fixed seed for reproducibility (e.g., `std::mt19937` with seed `12345` and `std::uniform_real_distribution<double>(-1.0, 1.0)`). Compute the symmetric matrix `A = X + X.transpose()` to enforce symmetry. Then, if `n >= 3`, create a `Tridiagonalization<MatrixXd>` object with `A`, extract the Householder coefficients via `.householderCoefficients()` (a vector of size `n-2`). Format the matrix rows by iterating rows and columns, and format the coefficients sequentially. Edge cases: when `n < 3`, no tridiagonalization is performed and the output contains only the matrix. The matrix size is small in typical tests but the function works for any `n`. Time complexity is dominated by tridiagonalization, which is `O(n^3)` for general matrices; formatting is `O(n^2)`. Space is `O(n^2)` for the matrix.
