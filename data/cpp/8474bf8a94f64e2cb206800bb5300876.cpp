/*
Given a set of 2D points and a target number of active connections to a new central point, write a C++ function `findCentroidStubs` that computes the coordinates of a candidate point and a radius such that the point is equidistant (within a small tolerance) from at least `k` of the input points. The function should take a vector of `Point` structs (with `x` and `y` fields), an integer `k` (≥ 3), and return a `StubResult` struct containing the candidate `(x, y)` coordinates and the common radius. If no such point exists, return a `StubResult` with a boolean `found = false`. The algorithm should be based on non-linear least squares: initialize the candidate point as the centroid of the first `k` points, then iteratively adjust the point and radius to minimize the sum of squared differences between the actual distances and the current radius, using a gradient-descent-like update (you may use a simple Newton or gradient method). The search should stop when the maximum change in the point coordinates or radius is below `1e-6` or after at most `100` iterations. Seed the search with multiple initial guesses: the centroid of every combination of `k` points would be too slow; instead, use the centroid of each contiguous block of `k` points in the input order (wrapping around). For each initial guess, run the optimization and check if the final residual (sum of squared errors) is less than `1e-4`; if so, return the result. If multiple valid solutions are found, return the one with the smallest radius. Ensure the function is robust to duplicate points and degenerate configurations.
*/
#include <vector>
#include <cmath>
#include <limits>
#include <algorithm>

struct Point {
    double x, y;
};

struct StubResult {
    bool found;
    double x, y, radius;
};

// Solve a 3x3 linear system Ax = b using Gaussian elimination. Returns false if singular.
static bool solve3x3(const double A[3][3], const double b[3], double out[3]) {
    double m[3][4];
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) m[i][j] = A[i][j];
        m[i][3] = b[i];
    }
    // Forward elimination
    for (int col = 0; col < 3; ++col) {
        // Find pivot
        int pivot = col;
        double maxVal = std::abs(m[col][col]);
        for (int row = col + 1; row < 3; ++row) {
            if (std::abs(m[row][col]) > maxVal) {
                maxVal = std::abs(m[row][col]);
                pivot = row;
            }
        }
        if (maxVal < 1e-12) return false;
        if (pivot != col) {
            for (int j = 0; j < 4; ++j) std::swap(m[col][j], m[pivot][j]);
        }
        // Eliminate below
        for (int row = col + 1; row < 3; ++row) {
            double factor = m[row][col] / m[col][col];
            for (int j = col; j < 4; ++j) m[row][j] -= factor * m[col][j];
        }
    }
    // Back substitution
    for (int row = 2; row >= 0; --row) {
        double sum = m[row][3];
        for (int j = row + 1; j < 3; ++j) sum -= m[row][j] * out[j];
        out[row] = sum / m[row][row];
    }
    return true;
}

StubResult findCentroidStubs(const std::vector<Point>& points, int k) {
    StubResult result;
    result.found = false;
    int n = static_cast<int>(points.size());
    if (n < k || k < 3) return result;

    // Try each contiguous block of k points, wrapping around.
    for (int start = 0; start < n; ++start) {
        // Build the set of indices for this block
        std::vector<int> idx;
        idx.reserve(k);
        for (int t = 0; t < k; ++t) {
            idx.push_back((start + t) % n);
        }

        // Initial guess: centroid of the selected points
        double cx = 0, cy = 0;
        for (int i : idx) {
            cx += points[i].x;
            cy += points[i].y;
        }
        cx /= k;
        cy /= k;

        // Initial radius: average distance to those points
        double r = 0;
        for (int i : idx) {
            double dx = points[i].x - cx;
            double dy = points[i].y - cy;
            r += std::sqrt(dx*dx + dy*dy);
        }
        r /= k;
        if (r < 1e-12) continue; // degenerate: all points coincident

        // Iterative optimization (Gauss-Newton)
        double x = cx, y = cy, radius = r;
        const double tol = 1e-6;
        const int maxIter = 100;
        bool converged = false;
        for (int iter = 0; iter < maxIter; ++iter) {
            // Build Jacobian and residual for the k equations
            // J is k x 3, we need J^T J (3x3) and J^T f (3 vector)
            double JTJ[3][3] = {{0,0,0},{0,0,0},{0,0,0}};
            double JTf[3] = {0,0,0};
            for (int i : idx) {
                double dx = points[i].x - x;
                double dy = points[i].y - y;
                double dist = std::sqrt(dx*dx + dy*dy);
                if (dist < 1e-12) {
                    // Degenerate: point exactly at center. Skip? Or set derivative to small.
                    continue;
                }
                double j0 = dx / dist;
                double j1 = dy / dist;
                double j2 = -1.0;
                double fi = dist - radius;
                // Update J^T J
                JTJ[0][0] += j0*j0;
                JTJ[0][1] += j0*j1;
                JTJ[0][2] += j0*j2;
                JTJ[1][1] += j1*j1;
                JTJ[1][2] += j1*j2;
                JTJ[2][2] += j2*j2;
                // Symmetric
                JTJ[1][0] = JTJ[0][1];
                JTJ[2][0] = JTJ[0][2];
                JTJ[2][1] = JTJ[1][2];
                // Update J^T f
                JTf[0] += j0 * fi;
                JTf[1] += j1 * fi;
                JTf[2] += j2 * fi;
            }
            // Solve JTJ * delta = -JTf
            double B[3][3];
            for (int a = 0; a < 3; ++a)
                for (int b = 0; b < 3; ++b) B[a][b] = JTJ[a][b];
            double rhs[3] = {-JTf[0], -JTf[1], -JTf[2]};
            double delta[3];
            if (!solve3x3(B, rhs, delta)) {
                // Singular matrix; break out of this initial guess
                break;
            }
            // Apply damping if needed? Keep simple: no damping
            x += delta[0];
            y += delta[1];
            radius += delta[2];
            if (radius < 0) radius = 0.0; // enforce non-negative radius

            // Check convergence
            double change = std::max(std::abs(delta[0]), std::max(std::abs(delta[1]), std::abs(delta[2])));
            if (change < tol) {
                converged = true;
                break;
            }
        }

        // After optimization, compute total residual sum of squares
        double sumSq = 0;
        double meanR = 0;
        for (int i : idx) {
            double dx = points[i].x - x;
            double dy = points[i].y - y;
            double dist = std::sqrt(dx*dx + dy*dy);
            double diff = dist - radius;
            sumSq += diff * diff;
            meanR += dist;
        }
        meanR /= k;
        if (sumSq < 1e-4) {
            // Refine radius to mean of distances
            double refinedRadius = meanR;
            // Check that the refinement still satisfies the residual
            double newSumSq = 0;
            for (int i : idx) {
                double dx = points[i].x - x;
                double dy = points[i].y - y;
                double dist = std::sqrt(dx*dx + dy*dy);
                newSumSq += (dist - refinedRadius) * (dist - refinedRadius);
            }
            if (newSumSq < 1e-4) {
                // If we already have a found solution, keep the one with smaller radius
                if (!result.found || refinedRadius < result.radius) {
                    result.found = true;
                    result.x = x;
                    result.y = y;
                    result.radius = refinedRadius;
                }
            }
        }
    }
    return result;
}
#include <cassert>
#include <cmath>

// Helper to check approximate equality
static bool close(double a, double b, double eps=1e-3) {
    return std::abs(a - b) < eps;
}

int main() {
    // Test 1: Points on a circle of radius 2 centered at (1, -1), k=4
    {
        std::vector<Point> pts;
        for (int i = 0; i < 4; ++i) {
            double angle = 2.0 * 3.141592653589793 * i / 4;
            pts.push_back({1.0 + 2.0 * std::cos(angle), -1.0 + 2.0 * std::sin(angle)});
        }
        StubResult r = findCentroidStubs(pts, 4);
        assert(r.found);
        assert(close(r.x, 1.0, 1e-2));
        assert(close(r.y, -1.0, 1e-2));
        assert(close(r.radius, 2.0, 1e-2));
    }

    // Test 2: Exactly 3 points forming an equilateral triangle, k=3
    {
        std::vector<Point> pts;
        pts.push_back({0.0, 0.0});
        pts.push_back({1.0, 0.0});
        pts.push_back({0.5, std::sqrt(3.0)/2.0});
        StubResult r = findCentroidStubs(pts, 3);
        assert(r.found);
        assert(close(r.radius, 1.0/std::sqrt(3.0), 1e-2));
        assert(close(r.x, 0.5, 1e-2));
        assert(close(r.y, std::sqrt(3.0)/6.0, 1e-2));
    }

    // Test 3: Points not on a common circle – should not find a stub for all k
    {
        std::vector<Point> pts = {{0,0},{1,0},{2,0},{3,0}};
        StubResult r = findCentroidStubs(pts, 4);
        assert(!r.found);
    }

    // Test 4: Duplicate points on a circle, k=3 (two duplicates)
    {
        std::vector<Point> pts;
        pts.push_back({0,0});
        pts.push_back({1,0});
        pts.push_back({0,0}); // duplicate
        pts.push_back({1,0}); // duplicate
        StubResult r = findCentroidStubs(pts, 3);
        assert(r.found);
        assert(close(r.radius, 0.5, 1e-2));
        assert(close(r.x, 0.5, 1e-2));
        assert(close(r.y, 0.0, 1e-2));
    }

    // Test 5: All points same location – degenerate, radius=0, should be found
    {
        std::vector<Point> pts = {{2,3},{2,3},{2,3},{2,3}};
        StubResult r = findCentroidStubs(pts, 4);
        assert(r.found);
        assert(close(r.radius, 0.0, 1e-3));
        assert(close(r.x, 2.0, 1e-3));
        assert(close(r.y, 3.0, 1e-3));
    }

    // Test 6: k > n should fail
    {
        std::vector<Point> pts = {{0,0},{1,1}};
        StubResult r = findCentroidStubs(pts, 3);
        assert(!r.found);
    }

    // Test 7: large circle, random points on it, k=5
    {
        std::vector<Point> pts;
        double cx = 10.0, cy = -5.0, radius = 7.0;
        for (int i = 0; i < 5; ++i) {
            double angle = 2.0 * 3.141592653589793 * (i * 2.0 / 5.0 + 0.13);
            pts.push_back({cx + radius * std::cos(angle), cy + radius * std::sin(angle)});
        }
        StubResult r = findCentroidStubs(pts, 5);
        assert(r.found);
        assert(close(r.x, cx, 1e-1));
        assert(close(r.y, cy, 1e-1));
        assert(close(r.radius, radius, 1e-1));
    }

    // Test 8: If no solution found, found=false
    {
        std::vector<Point> pts = {{0,0},{100,0},{0,100},{1,1}};
        StubResult r = findCentroidStubs(pts, 4);
        // Might find a solution if points are almost concyclic? Not here, so assert false.
        assert(!r.found);
    }

    return 0;
}
// The problem is to find a point `(x, y)` and radius `r` such that for at least `k` points, the Euclidean distance from `(x, y)` to each point is approximately `r`. This is equivalent to solving a system of equations: for each point `i` in the selected set, `sqrt((x - xi)^2 + (y - yi)^2) - r = 0`. We have 3 unknowns (`x`, `y`, `r`) and `k` equations, so it's overdetermined for `k > 3`. We use a non-linear least squares approach: define the residual vector `f_i = dist_i - r`. The Jacobian for each equation is `[ (x - xi)/dist_i, (y - yi)/dist_i, -1 ]`. We start from an initial guess for `(x, y, r)` where `r` is the average distance from the centroid to the `k` points. Then we perform Levenberg-Marquardt or a simple Gauss-Newton update: solve the normal equations `(J^T J) delta = -J^T f`, where `delta` is the update to `(x, y, r)`. Since `k` can be small, we can solve this 3x3 system directly via Gaussian elimination. We iterate until the change is small or max iterations reached. For robustness, we try multiple initial guesses: for each starting index `s` in `0..n-1`, take points `s, s+1, ..., s+k-1` modulo `n` (where `n` is the total number of points). This gives `n` initializations (or `n` if `k < n`, else only one). For each, we run the optimization and check if the final sum of squared residuals is below `1e-4`. If yes, compute the actual radius as the mean distance from the found center to all points that are close to `r` to refine it, then return. To handle duplicates, the algorithm naturally converges because the residuals become zero. Complexity: with `n` points and `G` initial guesses (at most `n`), each optimization involves `I` iterations (at most 100), each computing `k` residuals and a 3x3 solve (O(k)). So total time is `O(G * I * k)` which is at most `O(n * 100 * n)` in worst case if `k = n`, but typically `k` is small, so `O(n^2)` in worst case. Space is `O(n)` for storing points.
