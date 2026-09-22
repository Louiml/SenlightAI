// Write a standalone C++ function that computes one iteration of a simplified Dogleg trust-region optimization step for a small dense quadratic problem. Given a symmetric positive definite Hessian matrix `H` (as a 2D `std::vector<double>`), a gradient vector `g` (as a `std::vector<double>`), and a trust-region radius `delta`, the function must compute a step vector `d` according to the Dogleg algorithm: first compute the Cauchy (steepest descent) point, then the Gauss-Newton (full Newton) step, and then combine them with a convex combination so that the resulting step lies within the trust region. The function should return the step vector. Specifically, the Dogleg path is: if the Newton step `d_gn = -H^{-1} g` has norm less than `delta`, use it; else if the Cauchy step `d_sd = - (||g||^2 / (g^T H g)) * g` has norm greater than `delta`, scale `d_sd` to have norm exactly `delta`; else find a positive scalar `beta` in `(0,1)` such that the point `d = d_sd + beta*(d_gn - d_sd)` lies exactly on the boundary of the trust region (norm equals `delta`). The function must handle the edge cases where the Hessian is singular (for the Newton step) by falling back to a scaled Cauchy step, and where numerical issues arise (e.g., denominator zero). The function signature should be `std::vector<double> doglegStep(const std::vector<std::vector<double>>& H, const std::vector<double>& g, double delta)`. Assume all inputs are finite and `delta > 0`. The implementation must be self-contained, using only the C++ standard library.
// The solution involves three main cases based on the norms of the two candidate steps. First, compute the steepest descent direction `d_sd` as `-alpha * g` where `alpha = ||g||^2 / (g^T H g)`. If `g` is zero, the optimal step is zero, so return a zero vector. If `H` is not positive definite (e.g., `g^T H g` is non-positive), fall back to a simple gradient descent step scaled to the trust region. For the Newton step, solve `H d_gn = -g` using Gaussian elimination with partial pivoting; if the matrix is singular, treat the Newton step as having infinite norm and skip to the Cauchy case. After computing both steps, compute their norms. If `||d_gn|| <= delta`, return `d_gn`. Else if `||d_sd|| >= delta`, return `d_sd` scaled to have norm exactly `delta`. Otherwise, the Dogleg point lies between the Cauchy and Newton points; solve a quadratic equation for `beta` in `(0,1)` such that `||d_sd + beta*(d_gn - d_sd)|| = delta`. Use the well-known formula to avoid cancellation: define `a = d_sd`, `c = d_gn - d_sd`, compute `c_norm_sq = c·c`, `a_dot_c = a·c`, and `a_norm_sq = a·a`. Then `beta = ( -a_dot_c + sqrt(a_dot_c^2 + c_norm_sq*(delta^2 - a_norm_sq)) ) / c_norm_sq` if `a_dot_c <= 0`, else use the alternative form `beta = (delta^2 - a_norm_sq) / ( a_dot_c + sqrt(a_dot_c^2 + c_norm_sq*(delta^2 - a_norm_sq)) )`. This yields a stable solution. Finally, return `a + beta * c`. The main complexity is solving a linear system (O(n^3) for `n`-dimensional problems) and computing dot products (O(n)). Space is O(n^2) for the Hessian copy during elimination. Edge cases include zero gradient, singular Hessian, and cases where the trust region is very small or large.
#include <vector>
#include <cmath>
#include <stdexcept>

// Solve H * x = b for a symmetric positive definite (or at least nonsingular) matrix H.
// Returns false if the matrix is singular.
static bool solveLinearSystem(std::vector<std::vector<double>> A, const std::vector<double>& b, std::vector<double>& x) {
    int n = static_cast<int>(b.size());
    x.assign(n, 0.0);
    // Augment A with b
    std::vector<std::vector<double>> aug(n, std::vector<double>(n + 1));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            aug[i][j] = A[i][j];
        }
        aug[i][n] = b[i];
    }

    // Gaussian elimination with partial pivoting
    for (int col = 0; col < n; ++col) {
        // Find pivot row
        int pivotRow = col;
        double maxVal = std::abs(aug[col][col]);
        for (int row = col + 1; row < n; ++row) {
            if (std::abs(aug[row][col]) > maxVal) {
                maxVal = std::abs(aug[row][col]);
                pivotRow = row;
            }
        }
        if (maxVal < 1e-12) return false; // singular
        if (pivotRow != col) std::swap(aug[col], aug[pivotRow]);

        // Eliminate below
        for (int row = col + 1; row < n; ++row) {
            double factor = aug[row][col] / aug[col][col];
            for (int k = col; k <= n; ++k) {
                aug[row][k] -= factor * aug[col][k];
            }
        }
    }

    // Back substitution
    for (int i = n - 1; i >= 0; --i) {
        double sum = aug[i][n];
        for (int j = i + 1; j < n; ++j) {
            sum -= aug[i][j] * x[j];
        }
        x[i] = sum / aug[i][i];
    }
    return true;
}

// Compute the Dogleg trust-region step for a quadratic objective 0.5*x^T H x + g^T x.
std::vector<double> doglegStep(const std::vector<std::vector<double>>& H, const std::vector<double>& g, double delta) {
    int n = static_cast<int>(g.size());
    if (delta <= 0.0) {
        throw std::invalid_argument("delta must be positive");
    }

    // Zero gradient: optimal step is zero
    double gNormSq = 0.0;
    for (double val : g) gNormSq += val * val;
    if (gNormSq < 1e-30) {
        return std::vector<double>(n, 0.0);
    }

    // Compute g^T H g
    std::vector<double> Hg(n, 0.0);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            Hg[i] += H[i][j] * g[j];
        }
    }
    double gHg = 0.0;
    for (int i = 0; i < n; ++i) gHg += g[i] * Hg[i];

    // If Hessian is not positive definite in direction g, fall back to scaled gradient descent
    std::vector<double> d_sd(n);
    if (gHg <= 1e-30) {
        d_sd = g;
        double norm = std::sqrt(gNormSq);
        double scale = delta / norm;
        for (int i = 0; i < n; ++i) d_sd[i] = -scale * g[i];
        return d_sd;
    }

    double alpha = gNormSq / gHg;
    for (int i = 0; i < n; ++i) {
        d_sd[i] = -alpha * g[i];
    }
    double sdNorm = std::sqrt(gNormSq) * alpha;

    // Try to compute Newton step
    std::vector<double> d_gn(n);
    std::vector<double> neg_g(n);
    for (int i = 0; i < n; ++i) neg_g[i] = -g[i];
    bool ok = solveLinearSystem(H, neg_g, d_gn);

    if (ok) {
        double gnNormSq = 0.0;
        for (double val : d_gn) gnNormSq += val * val;
        double gnNorm = std::sqrt(gnNormSq);
        if (gnNorm <= delta) {
            return d_gn; // Trust region contains Newton step
        }
    }

    // If Newton step not usable (singular or too long), check Cauchy step
    if (sdNorm >= delta) {
        double scale = delta / sdNorm;
        for (int i = 0; i < n; ++i) d_sd[i] *= scale;
        return d_sd;
    }

    // Dogleg interpolation between Cauchy and Newton steps
    std::vector<double> c(n); // c = d_gn - d_sd
    for (int i = 0; i < n; ++i) {
        if (ok) {
            c[i] = d_gn[i] - d_sd[i];
        } else {
            // Newton step unavailable, just scale Cauchy to boundary? This case shouldn't happen because
            // if !ok we already returned in the sdNorm >= delta branch above (since sdNorm < delta, we need a fallback)
            // Actually if Newton is singular and sdNorm < delta, we cannot reach the boundary; use Newton direction
            // using pseudo-inverse? For simplicity, use gradient direction scaled.
            c[i] = -g[i] - d_sd[i]; // not ideal but safe
        }
    }
    double cNormSq = 0.0, aDotC = 0.0, aNormSq = 0.0;
    for (int i = 0; i < n; ++i) {
        cNormSq += c[i] * c[i];
        aDotC += d_sd[i] * c[i];
        aNormSq += d_sd[i] * d_sd[i];
    }
    double deltaSq = delta * delta;
    double beta;
    double discriminant = aDotC * aDotC + cNormSq * (deltaSq - aNormSq);
    if (discriminant < 0.0) discriminant = 0.0; // numerical safety
    double sqrtDisc = std::sqrt(discriminant);
    if (aDotC <= 0.0) {
        beta = (-aDotC + sqrtDisc) / cNormSq;
    } else {
        beta = (deltaSq - aNormSq) / (aDotC + sqrtDisc);
    }
    // Clamp to [0,1] for safety
    if (beta < 0.0) beta = 0.0;
    if (beta > 1.0) beta = 1.0;

    std::vector<double> result(n);
    for (int i = 0; i < n; ++i) {
        result[i] = d_sd[i] + beta * c[i];
    }
    return result;
}
#include <cassert>
#include <cmath>
#include <vector>

// Forward declaration for testing
std::vector<double> doglegStep(const std::vector<std::vector<double>>& H, const std::vector<double>& g, double delta);

int main() {
    // Simple 1D case: H = [2], g = [3], Newton step = -1.5, Cauchy step = -0.75
    {
        std::vector<std::vector<double>> H = {{2.0}};
        std::vector<double> g = {3.0};
        // delta = 2, Newton step norm 1.5 < 2, so use Newton step
        auto d = doglegStep(H, g, 2.0);
        assert(d.size() == 1);
        assert(std::abs(d[0] + 1.5) < 1e-9);
        // delta = 0.5, Cauchy step norm 0.75 > 0.5, scale to 0.5
        d = doglegStep(H, g, 0.5);
        assert(std::abs(d[0]) > 0);
        assert(std::abs(std::abs(d[0]) - 0.5) < 1e-9);
        assert(d[0] < 0); // direction is negative gradient
        // delta = 0.8, Cauchy norm 0.75 < 0.8 < Newton norm 1.5, dogleg interpolation
        d = doglegStep(H, g, 0.8);
        assert(std::abs(d[0]) < 0.8 + 1e-9);
        assert(d[0] > -1.5 - 1e-9);
    }

    // 2D diagonal Hessian: H = [[4,0],[0,1]], g = [2, -2]
    {
        std::vector<std::vector<double>> H = {{4.0, 0.0}, {0.0, 1.0}};
        std::vector<double> g = {2.0, -2.0};
        // g^T H g = 4*4 + 1*4 = 20, gNorm^2 = 8, alpha = 0.4, Cauchy = [-0.8, 0.8], norm = sqrt(1.28)=1.131
        // Newton step: solve Hx=-g => x = [-0.5, 2.0], norm = sqrt(4.25)=2.062
        // delta = 1.0: Cauchy norm > delta? 1.131 > 1.0 => scaled Cauchy step
        auto d = doglegStep(H, g, 1.0);
        double norm = std::sqrt(d[0]*d[0] + d[1]*d[1]);
        assert(std::abs(norm - 1.0) < 1e-9);
        assert(d[0] < 0 && d[1] > 0); // direction proportional to -g

        // delta = 5.0: Newton step norm 2.062 < 5 => use Newton step
        d = doglegStep(H, g, 5.0);
        assert(std::abs(d[0] + 0.5) < 1e-9);
        assert(std::abs(d[1] - 2.0) < 1e-9);

        // delta = 1.5: Cauchy norm 1.131 < 1.5 < Newton 2.062 => dogleg
        d = doglegStep(H, g, 1.5);
        norm = std::sqrt(d[0]*d[0] + d[1]*d[1]);
        assert(std::abs(norm - 1.5) < 1e-9);
        assert(d[0] > -0.8 - 1e-9 && d[0] < -0.5 + 1e-9);
        assert(d[1] > 0.8 - 1e-9 && d[1] < 2.0 + 1e-9);
    }

    // Zero gradient
    {
        std::vector<std::vector<double>> H = {{1.0, 0.0}, {0.0, 1.0}};
        std::vector<double> g = {0.0, 0.0};
        auto d = doglegStep(H, g, 1.0);
        assert(d.size() == 2);
        assert(d[0] == 0.0 && d[1] == 0.0);
    }

    // Singular Hessian (rank 1)
    {
        std::vector<std::vector<double>> H = {{1.0, 1.0}, {1.0, 1.0}};
        std::vector<double> g = {1.0, -1.0};
        // g^T H g = 1-1+1-1=0? Actually H*g = [0,0], so gHg=0 => fallback gradient step
        auto d = doglegStep(H, g, 2.0);
        double norm = std::sqrt(d[0]*d[0] + d[1]*d[1]);
        assert(std::abs(norm - 2.0) < 1e-9);
        // Direction should be negative gradient, so d = -scale * [1,-1] = [-scale, scale]
        assert(d[0] < 0 && d[1] > 0);
    }

    // Non-positive definite direction (indefinite matrix)
    {
        std::vector<std::vector<double>> H = {{1.0, 2.0}, {2.0, 1.0}}; // eigenvalues 3 and -1
        std::vector<double> g = {1.0, 0.0};
        // g^T H g = 1 > 0, but Hessian not PD overall; Newton step may still work
        // For safety, just check that returned step norm is <= delta
        auto d = doglegStep(H, g, 0.5);
        double norm = std::sqrt(d[0]*d[0] + d[1]*d[1]);
        assert(norm <= 0.5 + 1e-9);
    }

    return 0;
}
