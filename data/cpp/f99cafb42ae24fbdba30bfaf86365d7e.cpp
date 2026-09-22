Write a C++ function `matrixMultiplyAdd` that performs an in-place rank-1 update of a square matrix `C` by adding the outer product of two vectors `a` and `b` to it, mimicking the semantics of `C.noalias() += a * b` without relying on any external linear algebra library. The function should accept three parameters: a constant reference to a vector `a`, a constant reference to a vector `b`, and a reference to a matrix `C` (all represented as `std::vector<std::vector<double>>`). The function must validate that all dimensions are consistent: `a.size()`, `b.size()`, and both dimensions of `C` must be equal to a common size `n`, and `C` must be square. If any dimension mismatch is detected, the function should throw `std::invalid_argument` with a descriptive message. The update must be performed element-wise as `C[i][j] += a[i] * b[j]` for all `i, j` in `[0, n)`. The function must not create any temporary copies of the matrix or vectors beyond what is necessary, and it must be `const`-correct on input parameters.
#include <cassert>
#include <vector>
#include <cmath>

int main() {
    // Basic 2x2 case
    std::vector<double> a = {1.0, 2.0};
    std::vector<double> b = {3.0, 4.0};
    std::vector<std::vector<double>> C = {{0.0, 0.0}, {0.0, 0.0}};
    matrixMultiplyAdd(a, b, C);
    assert(C[0][0] == 3.0); // 1*3
    assert(C[0][1] == 4.0); // 1*4
    assert(C[1][0] == 6.0); // 2*3
    assert(C[1][1] == 8.0); // 2*4

    // Accumulate: call again on same C
    matrixMultiplyAdd(a, b, C);
    assert(C[0][0] == 6.0);
    assert(C[1][1] == 16.0);

    // 1x1 case
    std::vector<double> a1 = {5.0};
    std::vector<double> b1 = {7.0};
    std::vector<std::vector<double>> C1 = {{0.0}};
    matrixMultiplyAdd(a1, b1, C1);
    assert(C1[0][0] == 35.0);

    // Empty case (n=0) should not throw
    std::vector<double> a0, b0;
    std::vector<std::vector<double>> C0;
    matrixMultiplyAdd(a0, b0, C0); // no throw, no-op

    // Dimension mismatch: vector sizes differ
    std::vector<double> a2 = {1.0, 2.0};
    std::vector<double> b2 = {1.0};
    std::vector<std::vector<double>> C2 = {{0.0, 0.0}, {0.0, 0.0}};
    bool threw = false;
    try { matrixMultiplyAdd(a2, b2, C2); } catch (const std::invalid_argument&) { threw = true; }
    assert(threw);

    // Dimension mismatch: matrix not square
    std::vector<double> a3 = {1.0, 2.0};
    std::vector<double> b3 = {1.0, 2.0};
    std::vector<std::vector<double>> C3 = {{0.0, 0.0}, {0.0, 0.0}, {0.0, 0.0}}; // 3x2
    threw = false;
    try { matrixMultiplyAdd(a3, b3, C3); } catch (const std::invalid_argument&) { threw = true; }
    assert(threw);

    // Dimension mismatch: matrix wrong row count
    std::vector<std::vector<double>> C4 = {{0.0, 0.0}, {0.0, 0.0}, {0.0, 0.0}}; // 3x2? no, actually 3 rows 2 cols
    threw = false;
    try { matrixMultiplyAdd(a3, b3, C4); } catch (const std::invalid_argument&) { threw = true; }
    assert(threw);

    // Negative values and floating point
    std::vector<double> a5 = {-1.5, 2.0};
    std::vector<double> b5 = {0.5, -3.0};
    std::vector<std::vector<double>> C5 = {{10.0, 20.0}, {30.0, 40.0}};
    matrixMultiplyAdd(a5, b5, C5);
    assert(std::fabs(C5[0][0] - (10.0 + (-1.5*0.5))) < 1e-12);
    assert(std::fabs(C5[0][1] - (20.0 + (-1.5*-3.0))) < 1e-12);
    assert(std::fabs(C5[1][0] - (30.0 + (2.0*0.5))) < 1e-12);
    assert(std::fabs(C5[1][1] - (40.0 + (2.0*-3.0))) < 1e-12);

    return 0;
}
#include <vector>
#include <stdexcept>

// Performs C += a * b^T (rank-1 update) in place.
// Throws std::invalid_argument if dimensions are inconsistent.
void matrixMultiplyAdd(const std::vector<double>& a,
                       const std::vector<double>& b,
                       std::vector<std::vector<double>>& C) {
    const size_t n = a.size();

    // Validate vector sizes match
    if (b.size() != n) {
        throw std::invalid_argument("Vectors a and b must have the same size");
    }

    // Validate matrix is square and matches n
    if (C.size() != n) {
        throw std::invalid_argument("Matrix C must have n rows");
    }
    for (const auto& row : C) {
        if (row.size() != n) {
            throw std::invalid_argument("Matrix C must be square (all rows must have size n)");
        }
    }

    // If n == 0, nothing to do
    if (n == 0) return;

    // Perform rank-1 update: C[i][j] += a[i] * b[j]
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            C[i][j] += a[i] * b[j];
        }
    }
}
// The core algorithm is straightforward: given vectors `a` and `b` of size `n`, and a square matrix `C` of size `n x n`, we compute the outer product `a * b^T` and add it element-wise to `C`. The main steps are: (1) validate that all input matrices/vectors have consistent dimensions—specifically, `a.size() == b.size() == C.size()` and for every row in `C`, `C[i].size() == C.size()` (square). If the matrix is empty or dimensions mismatch, throw `std::invalid_argument`. (2) Iterate over all rows `i` and columns `j`, performing `C[i][j] += a[i] * b[j]`. This is an `O(n^2)` operation because there are `n^2` elements to update. The auxiliary space used is `O(1)` besides the input containers themselves. Edge cases include: `n = 0`—an empty matrix and empty vectors—in which case the function does nothing (but still must not throw because dimensions match trivially). Also, `n = 1`—a single scalar update. The function must be careful not to use `const` on `C` because it modifies it. Time complexity is `O(n^2)`, space is `O(1)` extra.
