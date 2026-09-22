Write a C++ function `double calculateThinPlateSplineValue(const std::vector<double>& x, const std::vector<double>& y, const std::vector<double>& z, double queryX, double queryY)` that performs thin plate spline interpolation. Given `n` control points `(x_i, y_i)` with associated scalar values `z_i`, the function computes the interpolated value at a query point `(queryX, queryY)` using the standard thin plate spline formulation:  
\[
f(x,y) = a_0 + a_1 x + a_2 y + \sum_{i=1}^{n} w_i \, \phi(||(x,y) - (x_i,y_i)||)
\]  
where the radial basis function is \(\phi(r) = r^2 \ln(r^2)\) (with \(\phi(0)=0\)), and the weights \(w_i\) and affine coefficients \(a_0, a_1, a_2\) are obtained by solving a linear system that includes the interpolation conditions \(f(x_i,y_i) = z_i\) plus the three constraints \(\sum w_i = 0\), \(\sum w_i x_i = 0\), \(\sum w_i y_i = 0\). The input vectors `x`, `y`, `z` must have equal length `n >= 3`, and the control points must be distinct. Handle degenerate cases: if `n < 3`, return 0.0; if exactly 3 non-collinear points, the solution is an affine plane; if the points are collinear, reduce to linear interpolation along the line. Use Gaussian elimination with partial pivoting to solve the system. Assume all inputs are valid doubles and no NaN values.

// The solution constructs a square linear system of size `m = n + 3`. The unknowns are `[w_1, ..., w_n, a_0, a_1, a_2]`. The system is:
// - For each control point `i` (rows 0 to n-1):  
//   `sum_j w_j * phi(i,j) + a_0 + a_1*x_i + a_2*y_i = z_i`  
//   where `phi(i,j) = (dx^2+dy^2) * log(dx^2+dy^2)` if `i != j`, else 0.
// - The last three rows enforce:  
//   `sum_j w_j = 0`, `sum_j w_j*x_j = 0`, `sum_j w_j*y_j = 0`.
//
// This yields an `(n+3) x (n+3)` matrix that is symmetric and invertible if the points are not collinear. We solve for the coefficients using Gaussian elimination with partial pivoting for numerical stability. After solving, the interpolated value at query point is computed as:
// `a_0 + a_1*queryX + a_2*queryY + sum_i w_i * phi((queryX,queryY),(x_i,y_i))`.
//
// Edge cases:
// - If `n < 3`, return 0.0.
// - If the points are collinear (check using cross product of differences), the matrix is singular. In that case, we can fall back to 1D linear interpolation along the line: project all points onto the principal direction (using the direction vector from the first two points), sort by projected coordinate, and linearly interpolate the z-values. If exactly 3 points and not collinear, the system still works and gives an affine plane.
// - Duplicate points cause singularity; we assume distinct points per problem statement but could detect and handle gracefully (e.g., average z-values).
//
// Time complexity: Solving the linear system via Gaussian elimination is O((n+3)^3) ≈ O(n^3). Evaluation at a query point is O(n). Space complexity is O(n^2) for the matrix.

#include <vector>
#include <cmath>
#include <algorithm>
#include <numeric>

// Helper: radial basis function phi(r^2) = r^2 * ln(r^2), with phi(0)=0.
static double phi(double dx, double dy) {
    double dist2 = dx*dx + dy*dy;
    if (dist2 == 0.0) return 0.0;
    return dist2 * std::log(dist2);
}

// Solve linear system A*x = b using Gaussian elimination with partial pivoting.
// A is n x n (row-major), b is n-vector. Returns true on success, false if singular.
static bool solveLinearSystem(std::vector<double>& A, std::vector<double>& b, int n, std::vector<double>& x) {
    // Augmented matrix: A (n*n) + b (n)
    std::vector<double> aug(n*(n+1), 0.0);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            aug[i*(n+1) + j] = A[i*n + j];
        }
        aug[i*(n+1) + n] = b[i];
    }

    for (int col = 0; col < n; ++col) {
        // Find pivot row
        int pivotRow = col;
        double maxAbs = std::fabs(aug[col*(n+1) + col]);
        for (int row = col+1; row < n; ++row) {
            double val = std::fabs(aug[row*(n+1) + col]);
            if (val > maxAbs) {
                maxAbs = val;
                pivotRow = row;
            }
        }
        if (maxAbs < 1e-12) return false; // singular

        // Swap rows
        if (pivotRow != col) {
            for (int j = col; j <= n; ++j) {
                std::swap(aug[col*(n+1) + j], aug[pivotRow*(n+1) + j]);
            }
        }

        // Normalize pivot row
        double pivot = aug[col*(n+1) + col];
        for (int j = col; j <= n; ++j) {
            aug[col*(n+1) + j] /= pivot;
        }

        // Eliminate column col from other rows
        for (int row = 0; row < n; ++row) {
            if (row == col) continue;
            double factor = aug[row*(n+1) + col];
            for (int j = col; j <= n; ++j) {
                aug[row*(n+1) + j] -= factor * aug[col*(n+1) + j];
            }
        }
    }

    // Extract solution
    x.resize(n);
    for (int i = 0; i < n; ++i) {
        x[i] = aug[i*(n+1) + n];
    }
    return true;
}

// Main function: thin plate spline interpolation at a query point.
double calculateThinPlateSplineValue(const std::vector<double>& x,
                                     const std::vector<double>& y,
                                     const std::vector<double>& z,
                                     double queryX, double queryY) {
    int n = static_cast<int>(x.size());
    if (n < 3) return 0.0;

    // Check for collinearity: if all points lie on a line, use 1D linear interpolation
    bool collinear = true;
    if (n > 2) {
        double dx0 = x[1] - x[0];
        double dy0 = y[1] - y[0];
        for (int i = 2; i < n; ++i) {
            double dx = x[i] - x[0];
            double dy = y[i] - y[0];
            double cross = dx0 * dy - dy0 * dx;
            if (std::fabs(cross) > 1e-12 * (std::fabs(dx0*dx0 + dy0*dy0) + std::fabs(dx*dx + dy*dy))) {
                collinear = false;
                break;
            }
        }
    }

    if (collinear) {
        // Project points onto the line direction from first two points
        double dx = x[1] - x[0];
        double dy = y[1] - y[0];
        double norm2 = dx*dx + dy*dy;
        if (norm2 < 1e-12) return 0.0; // all points identical
        std::vector<std::pair<double,double>> proj(n);
        for (int i = 0; i < n; ++i) {
            double t = ((x[i]-x[0])*dx + (y[i]-y[0])*dy) / norm2;
            proj[i] = {t, z[i]};
        }
        std::sort(proj.begin(), proj.end());
        double qt = ((queryX-x[0])*dx + (queryY-y[0])*dy) / norm2;
        // Clamp and linear interpolate
        if (qt <= proj[0].first) return proj[0].second;
        if (qt >= proj[n-1].first) return proj[n-1].second;
        for (int i = 1; i < n; ++i) {
            if (qt <= proj[i].first) {
                double t0 = proj[i-1].first;
                double t1 = proj[i].first;
                double frac = (qt - t0) / (t1 - t0);
                return proj[i-1].second + frac * (proj[i].second - proj[i-1].second);
            }
        }
        return proj[n-1].second;
    }

    int m = n + 3;
    std::vector<double> A(m*m, 0.0);
    std::vector<double> b(m, 0.0);

    // Build the system
    for (int i = 0; i < n; ++i) {
        // Interpolation rows
        for (int j = 0; j < n; ++j) {
            double phi_ij = phi(x[i]-x[j], y[i]-y[j]);
            A[i*m + j] = phi_ij;
        }
        A[i*m + n] = 1.0;
        A[i*m + n+1] = x[i];
        A[i*m + n+2] = y[i];
        b[i] = z[i];
    }

    // Constraint rows: sum w = 0, sum w*x = 0, sum w*y = 0
    for (int j = 0; j < n; ++j) {
        A[n*m + j] = 1.0;
        A[(n+1)*m + j] = x[j];
        A[(n+2)*m + j] = y[j];
    }
    // Other entries in these rows are already 0 (affine coefficients multiply 0)

    std::vector<double> solution;
    bool ok = solveLinearSystem(A, b, m, solution);
    if (!ok) return 0.0;

    // solution[0..n-1] are weights w, solution[n] = a0, solution[n+1] = a1, solution[n+2] = a2
    double result = solution[n] + solution[n+1]*queryX + solution[n+2]*queryY;
    for (int i = 0; i < n; ++i) {
        result += solution[i] * phi(queryX - x[i], queryY - y[i]);
    }
    return result;
}

#include <cassert>
#include <cmath>
#include <vector>

// The solution function is declared in the same translation unit or header.
// For the test, we include the implementation above.

int main() {
    // Test 1: trivial constant function (all z = 5)
    {
        std::vector<double> x = {0.0, 1.0, 0.0, 1.0};
        std::vector<double> y = {0.0, 0.0, 1.0, 1.0};
        std::vector<double> z = {5.0, 5.0, 5.0, 5.0};
        double val = calculateThinPlateSplineValue(x, y, z, 0.5, 0.5);
        assert(std::fabs(val - 5.0) < 1e-6);
    }

    // Test 2: linear function f(x,y) = 2x - y + 1, should reproduce exactly
    {
        std::vector<double> x = {0.0, 2.0, 0.0, 2.0, 1.0};
        std::vector<double> y = {0.0, 0.0, 3.0, 3.0, 1.5};
        std::vector<double> z;
        for (size_t i = 0; i < x.size(); ++i) {
            z.push_back(2.0*x[i] - y[i] + 1.0);
        }
        double val = calculateThinPlateSplineValue(x, y, z, 1.2, 0.8);
        double expected = 2.0*1.2 - 0.8 + 1.0;
        assert(std::fabs(val - expected) < 1e-6);
    }

    // Test 3: interpolation passes through control points
    {
        std::vector<double> x = {0.0, 1.0, 0.0, 1.0};
        std::vector<double> y = {0.0, 0.0, 1.0, 1.0};
        std::vector<double> z = {1.0, -2.0, 3.0, 0.5};
        for (int i = 0; i < 4; ++i) {
            double val = calculateThinPlateSplineValue(x, y, z, x[i], y[i]);
            assert(std::fabs(val - z[i]) < 1e-6);
        }
    }

    // Test 4: collinear points -> linear interpolation
    {
        std::vector<double> x = {0.0, 1.0, 2.0, 3.0};
        std::vector<double> y = {0.0, 0.0, 0.0, 0.0};
        std::vector<double> z = {0.0, 2.0, 4.0, 6.0}; // z = 2*x
        double val = calculateThinPlateSplineValue(x, y, z, 1.5, 0.0);
        assert(std::fabs(val - 3.0) < 1e-6);
    }

    // Test 5: fewer than 3 points returns 0
    {
        std::vector<double> x = {0.0, 1.0};
        std::vector<double> y = {0.0, 0.0};
        std::vector<double> z = {1.0, 2.0};
        assert(calculateThinPlateSplineValue(x, y, z, 0.5, 0.5) == 0.0);
    }

    // Test 6: non-linear function approximated but interpolation at points is exact
    {
        std::vector<double> x = {0.0, 1.0, 0.0, 1.0, 0.5};
        std::vector<double> y = {0.0, 0.0, 1.0, 1.0, 0.5};
        std::vector<double> z;
        for (size_t i = 0; i < x.size(); ++i) {
            z.push_back(std::sin(x[i]) + std::cos(y[i]));
        }
        for (size_t i = 0; i < x.size(); ++i) {
            double val = calculateThinPlateSplineValue(x, y, z, x[i], y[i]);
            assert(std::fabs(val - z[i]) < 1e-6);
        }
    }

    return 0;
}
