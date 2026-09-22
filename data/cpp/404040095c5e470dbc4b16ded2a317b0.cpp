Write a standalone C++ function `estimateFundamentalMatrix7pts` that takes a vector of 7 point correspondences, each represented as four `double` values `(x1, y1, x2, y2)`, and returns a `std::vector<std::array<double, 9>>` containing one to three candidate fundamental matrices (each flattened in row‑major order). The function must implement the seven‑point algorithm: build the 7×9 linear system from the correspondence equations `p2^T F p1 = 0`, perform Gaussian elimination to row‑echelon form, construct the two‑parameter family of solutions `F = f1 + λ f2`, derive a cubic polynomial in `λ` from the rank‑2 constraint `det(F)=0`, solve the cubic for its real roots, and for each real root produce the corresponding fundamental matrix normalized so that the last element (if non‑zero) is 1. If the elimination fails, the cubic has no real roots, or division by zero / NaN occurs, return an empty vector. The input is guaranteed to contain exactly 7 correspondences; no normalization is required. The function must be self‑contained and not depend on OpenCV or Eigen.

// The seven‑point algorithm estimates the fundamental matrix from 7 point correspondences. For each correspondence, the epipolar constraint yields a linear equation in the 9 entries of `F` (row‑major flattening). Stacking 7 such equations gives a 7×9 matrix `A`; we seek the null space of dimension at least 2. Gaussian elimination transforms `A` to row‑echelon form, revealing two free variables (we choose the last two entries of the solution vector, setting them to particular combinations). This gives two particular solutions `f1` and `f2` such that any null‑space vector is a linear combination `F = f1 + λ f2` (with a formal scalar `μ` for generality, but we can set `μ=1` initially and then normalize). The fundamental matrix must be singular (rank 2), so `det(F)=0`, which produces a cubic polynomial in `λ`. Solving the cubic (e.g., by first reducing to depressed cubic and then using Cardano’s formula) yields 1 or 3 real roots. For each root, we form the matrix, attempt to normalize so that `F[8]` equals 1 (if `|F[8]|` is not too small, otherwise leave `F[8]=0`), and store the 9 entries in row‑major order. Edge cases: if elimination fails (pivot becomes zero), if the cubic solver returns no real roots, or if any computed value becomes NaN, return an empty vector. Time complexity is `O(1)` because the size is fixed (7×9 matrix, cubic degree 3); space complexity is `O(1)` excluding the output vector. The cubic solver must handle numerical stability, but for this task a direct implementation of Cardano’s method with a fallback to a simple Newton iteration is acceptable.

#include <array>
#include <vector>
#include <cmath>
#include <algorithm>
#include <limits>

// Solve cubic equation a*x^3 + b*x^2 + c*x + d = 0.
// Returns real roots in increasing order.
static std::vector<double> solveCubic(double a, double b, double c, double d) {
    const double eps = 1e-12;
    std::vector<double> roots;

    if (std::abs(a) < eps) {
        // Quadratic: b*x^2 + c*x + d = 0
        if (std::abs(b) < eps) {
            // Linear: c*x + d = 0
            if (std::abs(c) > eps) {
                roots.push_back(-d / c);
            }
            return roots;
        }
        double disc = c*c - 4*b*d;
        if (disc < -eps) return roots;
        if (std::abs(disc) < eps) {
            roots.push_back(-c / (2*b));
        } else {
            double sqrt_disc = std::sqrt(disc);
            roots.push_back((-c - sqrt_disc) / (2*b));
            roots.push_back((-c + sqrt_disc) / (2*b));
        }
        std::sort(roots.begin(), roots.end());
        return roots;
    }

    // Normalize to monic cubic: x^3 + p*x^2 + q*x + r = 0
    b /= a; c /= a; d /= a;

    // Depressed cubic: t^3 + P*t + Q = 0, where x = t - p/3
    double p = b;
    double q = c;
    double r = d;
    double P = q - p*p/3.0;
    double Q = (2.0*p*p*p - 9.0*p*q + 27.0*r) / 27.0;

    double disc = (Q*Q/4.0) + (P*P*P/27.0);

    double offset = -p/3.0;

    if (disc > eps) {
        // One real root
        double sqrt_disc = std::sqrt(disc);
        double u = std::cbrt(-Q/2.0 + sqrt_disc);
        double v = std::cbrt(-Q/2.0 - sqrt_disc);
        roots.push_back(u + v + offset);
    } else if (std::abs(disc) < eps) {
        // Multiple real roots (disc = 0)
        double u = std::cbrt(-Q/2.0);
        roots.push_back(2.0*u + offset);
        roots.push_back(-u + offset);
        std::sort(roots.begin(), roots.end());
        // Remove possible duplicate
        if (roots.size() > 1 && std::abs(roots[0] - roots[1]) < eps)
            roots.pop_back();
    } else {
        // Three real roots (casus irreducibilis)
        double phi = std::acos(-Q/2.0 * std::sqrt(27.0 / (-P*P*P))); // careful: P is negative
        double sqrt_mP = std::sqrt(-P);
        double factor = 2.0 * sqrt_mP;
        roots.push_back(factor * std::cos(phi/3.0) + offset);
        roots.push_back(factor * std::cos((phi + 2.0*M_PI)/3.0) + offset);
        roots.push_back(factor * std::cos((phi + 4.0*M_PI)/3.0) + offset);
        std::sort(roots.begin(), roots.end());
    }

    // Filter out roots that are not real (due to numerical errors)
    std::vector<double> filtered;
    for (double root : roots) {
        if (std::isfinite(root))
            filtered.push_back(root);
    }
    return filtered;
}

// Estimate fundamental matrix using 7-point algorithm.
// Each element of `points` is {x1, y1, x2, y2}.
// Returns up to 3 candidate fundamental matrices, each flattened row-major (9 entries).
std::vector<std::array<double, 9>> estimateFundamentalMatrix7pts(
    const std::vector<std::array<double, 4>>& points) {

    if (points.size() != 7) return {};

    const int m = 7, n = 9;
    double a[63]; // 7x9 matrix (row-major)
    for (int i = 0; i < m; ++i) {
        double x1 = points[i][0];
        double y1 = points[i][1];
        double x2 = points[i][2];
        double y2 = points[i][3];
        double* row = &a[i*n];
        row[0] = x2*x1;
        row[1] = x2*y1;
        row[2] = x2;
        row[3] = y2*x1;
        row[4] = y2*y1;
        row[5] = y2;
        row[6] = x1;
        row[7] = y1;
        row[8] = 1.0;
    }

    // Gaussian elimination to row-echelon form (no pivot normalization)
    const double eps = 1e-12;
    for (int col = 0; col < m; ++col) {
        // Find pivot
        int pivot_row = col;
        double max_val = std::abs(a[col*n + col]);
        for (int r = col+1; r < m; ++r) {
            double val = std::abs(a[r*n + col]);
            if (val > max_val) {
                max_val = val;
                pivot_row = r;
            }
        }
        if (max_val < eps) {
            return {}; // Singular system, not enough rank
        }
        if (pivot_row != col) {
            for (int j = col; j < n; ++j)
                std::swap(a[col*n + j], a[pivot_row*n + j]);
        }
        double pivot = a[col*n + col];
        for (int r = col+1; r < m; ++r) {
            double factor = a[r*n + col] / pivot;
            if (std::abs(factor) < eps) continue;
            for (int j = col; j < n; ++j)
                a[r*n + j] -= factor * a[col*n + j];
        }
    }

    // Back-substitute considering two free variables (f[7] and f[8]).
    // We set up two solutions:
    //   solution1: f1[8] = 1, f1[7] = 0, then solve upward
    //   solution2: f2[8] = 1, f2[7] = -a[6*n+8]/a[6*n+7], f2[6] = 0
    // Using formulas from the snippet (note the snippet sets f1[6] from the last row).
    // We'll follow a systematic back-substitution.

    double f1[9] = {0};
    double f2[9] = {0};

    // Choose two basis vectors for the null space.
    // From the last row (row index 6, since m=7 rows 0..6):
    // a[6][6]*x6 + a[6][7]*x7 + a[6][8]*x8 = 0
    // We can set x8=1 and choose two different assignments for x6,x7.
    // To match common implementation:
    // basis1: x6 = -a[6][8]/a[6][6] if pivot non-zero, else 0; x7 = 0; x8 = 1
    // basis2: x6 = 0; x7 = -a[6][8]/a[6][7]; x8 = 1
    // But we must be careful: if a[6][6] is zero (pivot is zero after elimination),
    // then basis1 is not valid, but that would have caused return earlier because
    // elimination guarantees a[6][6] non-zero (pivot). So it's safe.

    f1[8] = 1.0;
    f1[7] = 0.0;
    f1[6] = -a[6*n+8] / a[6*n+6];

    f2[8] = 1.0;
    f2[7] = -a[6*n+8] / a[6*n+7];
    f2[6] = 0.0;

    // Back-substitution for rows i = 5 down to 0
    for (int i = m-2; i >= 0; --i) {
        double acc1 = 0.0, acc2 = 0.0;
        for (int j = i+1; j < n; ++j) {
            acc1 -= a[i*n+j] * f1[j];
            acc2 -= a[i*n+j] * f2[j];
        }
        double pivot = a[i*n+i];
        if (std::abs(pivot) < eps) return {};
        f1[i] = acc1 / pivot;
        f2[i] = acc2 / pivot;
        if (std::isnan(f1[i]) || std::isnan(f2[i]))
            return {};
    }

    // Now we have f1 and f2 such that any solution is f1 + λ*f2.
    // The rank-2 constraint det(F) = 0 gives a cubic in λ.
    // Compute coefficients of the cubic: c[λ^3] + c2[λ^2] + c1[λ] + c0 = 0.
    // We form F(λ) = f1 + λ*f2 (as 9-vector), compute det of 3x3 matrix.

    // Define helper to get 3x3 determinant from 9-vector (row-major).
    auto det3x3 = [](const double* v) -> double {
        return v[0]*(v[4]*v[8] - v[5]*v[7])
             - v[1]*(v[3]*v[8] - v[5]*v[6])
             + v[2]*(v[3]*v[7] - v[4]*v[6]);
    };

    // det(f1 + λ*f2) = c0 + c1*λ + c2*λ^2 + c3*λ^3
    // We can compute by evaluating at 4 points (λ = 0,1,2,3) and interpolating,
    // or symbolically. Simpler and robust: sample and solve linear system for coefficients.
    // Use O(3) sample: λ = 0,1,2 gives three equations for c0,c1,c2? But cubic needs 4.
    // Instead do symbolic: expand determinant as polynomial in λ.
    // Let f = f1 + λ*f2. The determinant is sum over permutations.
    // We compute coefficients by collecting powers of λ from each term.
    // Since degree is 3, we can compute via brute force: iterate all 6 permutations.

    double c0 = 0, c1 = 0, c2 = 0, c3 = 0;

    // Loop over permutations of (0,1,2) for rows
    int perms[6][3] = {{0,1,2},{0,2,1},{1,0,2},{1,2,0},{2,0,1},{2,1,0}};
    int signs[6] = {1, -1, -1, 1, 1, -1}; // Actually need to derive: for permutation (i,j,k), sign = sign of permutation.
    // For (0,1,2) +, (0,2,1) -, (1,0,2) -, (1,2,0) +, (2,0,1) +, (2,1,0) -.
    signs[0] = 1; signs[1] = -1; signs[2] = -1; signs[3] = 1; signs[4] = 1; signs[5] = -1;

    auto getEntry = [](const double* f1v, const double* f2v, int row, int col) -> std::pair<double,double> {
        // f = f1 + λ*f2, entry = f1[idx] + λ*f2[idx]
        int idx = row*3 + col;
        return {f1v[idx], f2v[idx]}; // constant, λ coefficient
    };

    for (int p = 0; p < 6; ++p) {
        int i = perms[p][0];
        int j = perms[p][1];
        int k = perms[p][2];
        // product of three entries: (a0 + b0*λ)*(a1 + b1*λ)*(a2 + b2*λ)
        // Expand.
        auto e0 = getEntry(f1, f2, 0, i);
        auto e1 = getEntry(f1, f2, 1, j);
        auto e2 = getEntry(f1, f2, 2, k);

        double a0 = e0.first, b0 = e0.second;
        double a1 = e1.first, b1 = e1.second;
        double a2 = e2.first, b2 = e2.second;

        // Polynomial multiplication: (a0 + b0 x)(a1 + b1 x)(a2 + b2 x)
        double coeff0 = a0*a1*a2;
        double coeff1 = a0*a1*b2 + a0*b1*a2 + b0*a1*a2;
        double coeff2 = a0*b1*b2 + b0*a1*b2 + b0*b1*a2;
        double coeff3 = b0*b1*b2;

        int s = signs[p];
        c0 += s*coeff0;
        c1 += s*coeff1;
        c2 += s*coeff2;
        c3 += s*coeff3;
    }

    // Solve cubic: c3*λ^3 + c2*λ^2 + c1*λ + c0 = 0
    std::vector<double> lambdas = solveCubic(c3, c2, c1, c0);

    std::vector<std::array<double, 9>> models;
    for (double lambda : lambdas) {
        if (!std::isfinite(lambda)) continue;
        std::array<double, 9> F;
        double mu = 1.0;
        double s = f1[8]*lambda + f2[8];
        const double FLT_EPSILON = std::numeric_limits<float>::epsilon();
        if (std::abs(s) > FLT_EPSILON) {
            mu = 1.0 / s;
            lambda *= mu;
            F[8] = 1.0;
        } else {
            F[8] = 0.0;
        }
        for (int idx = 0; idx < 8; ++idx) {
            F[idx] = f1[idx]*lambda + f2[idx]*mu;
        }
        // Sanity check: ensure finite
        bool ok = true;
        for (double v : F) if (!std::isfinite(v)) { ok = false; break; }
        if (ok) models.push_back(F);
    }
    return models;
}

#include <cassert>
#include <cmath>
#include <vector>
#include <array>
#include <iostream>

int main() {
    // Test 1: Simple synthetic data from a known fundamental matrix.
    // Choose a simple F = [[0,0,0],[0,0,-1],[0,1,0]] (identity-like degenerate).
    // For points p1 = (x1,y1,1), p2 = (u,v,1), constraint: -v + u? Wait F*p1 gives [0, -1, y1]? 
    // Actually F = [[0,0,0],[0,0,-1],[0,1,0]], then p2^T F p1 = (u,v,1) * (0, -1, y1)^T? careful:
    // F*p1 = [0, -1, y1]^T? Let's compute: F = 
    // [0 0 0]
    // [0 0 -1]
    // [0 1 0]
    // p1=(x1,y1,1) -> F*p1 = (0, -1, y1) then dot with p2=(x2,y2,1) -> -y2 + y1 = 0 => y1=y2.
    // So any points with same y satisfy. Generate 7 such points.
    std::vector<std::array<double,4>> pts;
    for (int i = 0; i < 7; ++i) {
        double y = 1.0 + i*0.1;
        pts.push_back({i*1.0, y, i*2.0, y}); // x1 != x2, but y1=y2
    }
    auto models = estimateFundamentalMatrix7pts(pts);
    // Should produce at least one solution; verify that each model satisfies epipolar constraint approximately.
    assert(!models.empty());
    bool all_constraints_ok = true;
    for (const auto& F : models) {
        // For each point, check p2^T F p1 ~ 0
        for (const auto& p : pts) {
            double x1=p[0], y1=p[1], x2=p[2], y2=p[3];
            // F is 3x3 row-major
            double val = x2*(F[0]*x1 + F[1]*y1 + F[2])
                       + y2*(F[3]*x1 + F[4]*y1 + F[5])
                       + (F[6]*x1 + F[7]*y1 + F[8]);
            if (std::abs(val) > 1e-6) {
                all_constraints_ok = false;
                break;
            }
        }
        if (!all_constraints_ok) break;
    }
    assert(all_constraints_ok);

    // Test 2: Degenerate input where all points are collinear? Should still produce something or empty.
    // Simple: all points on a line, e.g., y1=y2=0, x1=x2 = i.
    std::vector<std::array<double,4>> pts_line;
    for (int i = 0; i < 7; ++i) {
        pts_line.push_back({i*1.0, 0.0, i*1.0, 0.0});
    }
    auto models_line = estimateFundamentalMatrix7pts(pts_line);
    // Might be empty or produce degenerate matrices. Just ensure no crash.
    // We don't assert non-empty because degenerate may fail.

    // Test 3: Random points with no relation - ensure function returns something or empty but doesn't crash.
    // Not deterministic, so just call with random-ish points.
    std::vector<std::array<double,4>> pts_rand;
    for (int i = 0; i < 7; ++i) {
        pts_rand.push_back({i*1.3, i*0.7+1, i*0.2-2, i*1.1+0.5});
    }
    auto models_rand = estimateFundamentalMatrix7pts(pts_rand);
    // No assertion, just ensure it doesn't crash.

    // Test 4: Ensure that for known non-singular F, the model has rank 2 (det ≈ 0).
    if (!models.empty()) {
        for (const auto& F : models) {
            double det = F[0]*(F[4]*F[8]-F[5]*F[7]) - F[1]*(F[3]*F[8]-F[5]*F[6]) + F[2]*(F[3]*F[7]-F[4]*F[6]);
            assert(std::abs(det) < 1e-6);
        }
    }

    // Test 5: Check that the function returns at most 3 models.
    assert(models.size() <= 3);
    // Test 6: For the first test, check that each model's last element is 1 if not too small.
    for (const auto& F : models) {
        if (std::abs(F[8]) > 1e-6) {
            assert(std::abs(F[8] - 1.0) < 1e-9);
        }
    }

    std::cout << "All tests passed." << std::endl;
    return 0;
}
