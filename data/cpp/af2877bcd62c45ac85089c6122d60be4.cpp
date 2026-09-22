Write a C++ function that solves a linear system \(Ax = f\) twice using two different numerical methods: (1) LU decomposition with partial pivoting and (2) QR decomposition via Givens rotations. The function must accept a dimension \(N\) and construct a dense \(N \times N\) matrix \(A\) and right-hand side vector \(f\) according to the given formulas: \(A_{ij} = 100\) if \(i=j\), otherwise \(A_{ij} = 1 - 0.1(i+1) - 0.2(j+1)\) (zero‑based indices), and \(f_i = \sum_{j=0}^{N-1} A_{ij}\). The function must return a struct containing the relative error \(\delta = \frac{\|x - x_{\text{true}}\|_2}{\|x_{\text{true}}\|_2}\) (with \(x_{\text{true}} = (1,1,\ldots,1)^T\)) from each method, as well as the average runtime (in seconds) for a given number of repetitions. The function must be efficient, handle degenerate matrices gracefully (returning flags or appropriate error indications), and must not rely on any external linear algebra library.

The solution involves constructing the matrix and vector as specified, then benchmarking two independent solvers. For LU decomposition, we perform Gaussian elimination with partial pivoting: at each step \(k\), find the pivot row with maximum absolute value in column \(k\), swap rows, compute multipliers, and update the trailing submatrix. The decomposition is stored in‑place in the matrix, with a pivot vector recording row swaps. Solving proceeds by forward substitution (with pivoting applied to the right‑hand side) and then backward substitution. For QR decomposition, we use Givens rotations to zero out subdiagonal entries column by column: for each pair \((i,j)\) with \(i>j\), compute rotation parameters \(c\) and \(s\) from \(a=R_{jj}\) and \(b=R_{ij}\), apply the rotation to the remaining columns and the right‑hand side, then solve the upper triangular system by backward substitution. Important edge cases include a zero pivot during LU (return false) and a nearly‑zero diagonal in the triangular factor (treat as singular). Time complexity is \(O(N^3)\) for both methods (LU has a smaller constant), and space complexity is \(O(N^2)\) (or \(O(N)\) for the right‑hand side and solution). The relative error is computed using the Euclidean norm; for the given construction, the solution should be exactly \((1,\ldots,1)\), so errors are expected to be tiny, but the comparison is still valid.

#include <vector>
#include <cmath>
#include <chrono>
#include <algorithm>

using hr_clock = std::chrono::high_resolution_clock;
using seconds_d = std::chrono::duration<double>;

struct LinSolveBench {
    double lu_time{0.0}, qr_time{0.0};
    double lu_delta{0.0}, qr_delta{0.0};
};

inline size_t lin_idx(size_t i, size_t j, size_t N) { return i * N + j; }

void buildSystem(size_t N, std::vector<double>& A, std::vector<double>& f) {
    A.resize(N * N);
    f.resize(N);
    for (size_t i = 0; i < N; ++i) {
        double row_sum = 0.0;
        for (size_t j = 0; j < N; ++j) {
            double val = (i == j) ? 100.0 : 1.0 - 0.1 * static_cast<double>(i + 1) - 0.2 * static_cast<double>(j + 1);
            A[lin_idx(i, j, N)] = val;
            row_sum += val;
        }
        f[i] = row_sum;
    }
}

bool luDecompose(std::vector<double>& A, std::vector<size_t>& piv, size_t N) {
    piv.assign(N, 0);
    for (size_t k = 0; k < N; ++k) {
        size_t max_row = k;
        double max_val = std::fabs(A[lin_idx(k, k, N)]);
        for (size_t i = k + 1; i < N; ++i) {
            double v = std::fabs(A[lin_idx(i, k, N)]);
            if (v > max_val) { max_val = v; max_row = i; }
        }
        if (max_val < 1e-15) return false;
        piv[k] = max_row;
        if (max_row != k) {
            for (size_t j = 0; j < N; ++j)
                std::swap(A[lin_idx(k, j, N)], A[lin_idx(max_row, j, N)]);
        }
        for (size_t i = k + 1; i < N; ++i) {
            double lik = A[lin_idx(i, k, N)] / A[lin_idx(k, k, N)];
            A[lin_idx(i, k, N)] = lik;
            for (size_t j = k + 1; j < N; ++j)
                A[lin_idx(i, j, N)] -= lik * A[lin_idx(k, j, N)];
        }
    }
    return true;
}

void luSolve(const std::vector<double>& LU, const std::vector<size_t>& piv,
             std::vector<double>& x, const std::vector<double>& b, size_t N) {
    x = b;
    for (size_t k = 0; k < N; ++k) {
        if (piv[k] != k) std::swap(x[k], x[piv[k]]);
    }
    // forward substitution
    for (size_t i = 0; i < N; ++i) {
        double sum = 0.0;
        for (size_t j = 0; j < i; ++j) sum += LU[lin_idx(i, j, N)] * x[j];
        x[i] -= sum; // because diagonal of L is 1
    }
    // backward substitution
    for (int i = static_cast<int>(N) - 1; i >= 0; --i) {
        double sum = 0.0;
        for (size_t j = i + 1; j < N; ++j) sum += LU[lin_idx(i, j, N)] * x[j];
        x[i] = (x[i] - sum) / LU[lin_idx(i, i, N)];
    }
}

bool qrGivensSolve(const std::vector<double>& A_in, const std::vector<double>& f_in,
                   std::vector<double>& x, size_t N) {
    std::vector<double> R = A_in;
    std::vector<double> b = f_in;
    for (size_t j = 0; j < N; ++j) {
        for (size_t i = j + 1; i < N; ++i) {
            double a = R[lin_idx(j, j, N)];
            double b_ij = R[lin_idx(i, j, N)];
            if (std::fabs(b_ij) < 1e-14) continue;
            double r = std::hypot(a, b_ij);
            double c = a / r;
            double s = -b_ij / r;
            for (size_t k = j; k < N; ++k) {
                double old_j = R[lin_idx(j, k, N)];
                double old_i = R[lin_idx(i, k, N)];
                R[lin_idx(j, k, N)] = c * old_j - s * old_i;
                R[lin_idx(i, k, N)] = s * old_j + c * old_i;
            }
            double old_bj = b[j];
            double old_bi = b[i];
            b[j] = c * old_bj - s * old_bi;
            b[i] = s * old_bj + c * old_bi;
        }
    }
    // back substitution
    x.assign(N, 0.0);
    for (int i = static_cast<int>(N) - 1; i >= 0; --i) {
        if (std::fabs(R[lin_idx(i, i, N)]) < 1e-15) return false;
        double sum = b[i];
        for (size_t j = i + 1; j < N; ++j) sum -= R[lin_idx(i, j, N)] * x[j];
        x[i] = sum / R[lin_idx(i, i, N)];
    }
    return true;
}

// Main benchmark function
LinSolveBench benchmarkLinearSolve(size_t N, int reps) {
    std::vector<double> A, f;
    buildSystem(N, A, f);
    const double norm_x_star = std::sqrt(static_cast<double>(N));
    LinSolveBench result;

    // LU benchmark
    double t_sum_lu = 0.0;
    double delta_lu = 0.0;
    for (int r = 0; r < reps; ++r) {
        auto M = A;
        std::vector<size_t> piv;
        std::vector<double> x;
        auto t0 = hr_clock::now();
        bool ok = luDecompose(M, piv, N);
        if (ok) luSolve(M, piv, x, f, N);
        auto t1 = hr_clock::now();
        t_sum_lu += seconds_d(t1 - t0).count();
        if (r == 0 && ok) {
            double diff2 = 0.0;
            for (double xi : x) diff2 += (xi - 1.0) * (xi - 1.0);
            delta_lu = std::sqrt(diff2) / norm_x_star;
        }
    }
    result.lu_time = t_sum_lu / reps;
    result.lu_delta = delta_lu;

    // QR benchmark
    double t_sum_qr = 0.0;
    double delta_qr = 0.0;
    for (int r = 0; r < reps; ++r) {
        std::vector<double> x;
        auto t0 = hr_clock::now();
        bool ok = qrGivensSolve(A, f, x, N);
        auto t1 = hr_clock::now();
        t_sum_qr += seconds_d(t1 - t0).count();
        if (r == 0 && ok) {
            double diff2 = 0.0;
            for (double xi : x) diff2 += (xi - 1.0) * (xi - 1.0);
            delta_qr = std::sqrt(diff2) / norm_x_star;
        }
    }
    result.qr_time = t_sum_qr / reps;
    result.qr_delta = delta_qr;

    return result;
}

#include <cassert>
#include <cmath>
#include <vector>

// The solution function prototype (must match implementation)
LinSolveBench benchmarkLinearSolve(size_t N, int reps);

int main() {
    // Test with small N where the solution is exactly known
    LinSolveBench r1 = benchmarkLinearSolve(10, 5);
    assert(r1.lu_delta < 1e-10 && "LU error should be tiny for well-conditioned matrix");
    assert(r1.qr_delta < 1e-10 && "QR error should be tiny for well-conditioned matrix");

    // Test that both methods produce nearly the same solution (within reason)
    LinSolveBench r2 = benchmarkLinearSolve(50, 2);
    assert(std::fabs(r2.lu_delta - r2.qr_delta) < 1e-8 && "LU and QR should agree closely");

    // Test time ordering: LU is generally faster than QR for dense matrices
    LinSolveBench r3 = benchmarkLinearSolve(100, 3);
    assert(r3.lu_time <= r3.qr_time * 1.5 && "LU should not be much slower than QR");

    // Test that the system is solvable: errors are non-negative and finite
    assert(std::isfinite(r3.lu_delta) && std::isfinite(r3.qr_delta));

    // Additional sanity: repeated runs should produce similar times (order of magnitude)
    LinSolveBench r4 = benchmarkLinearSolve(20, 10);
    LinSolveBench r5 = benchmarkLinearSolve(20, 10);
    assert(std::fabs(r4.lu_time - r5.lu_time) < 1e-4 && "LU timing should be stable");
    assert(std::fabs(r4.qr_time - r5.qr_time) < 1e-4 && "QR timing should be stable");

    return 0;
}
