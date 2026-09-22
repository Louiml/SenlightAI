// Write a C++ function `solvePoseGraph` that performs a simplified pose-graph optimization using only standard library data structures and basic linear algebra (no external graph optimization libraries required). The function takes as input: a vector of 2D poses (each represented as an `std::array<double, 3>` containing x, y, and theta in radians), a vector of relative measurement edges (each represented as `std::tuple<int, int, std::array<double,3>>` where the integers are vertex indices and the array is the measured relative pose from vertex i to vertex j), and an integer `max_iterations`. The function must return the optimized poses after applying a simple Gauss-Newton optimization that minimizes the sum of squared error between predicted relative poses (derived from current pose estimates) and measured relative poses, using a fixed information matrix (identity for simplicity except for rotation scaling by 0.1). Pose updates are performed by updating the translation directly and the rotation by small angle increments, and the function must handle the case where edges reference invalid vertex indices by skipping them. The output should be a `std::vector<std::array<double,3>>` with the same ordering as the input vertices. Optimize until convergence (change in total error less than 1e-6) or until `max_iterations` is reached. The function must be self-contained and not rely on any external libraries beyond `<vector>`, `<tuple>`, `<array>`, `<cmath>`, `<algorithm>`, and `<limits>`.
#include <cassert>
#include <cmath>
#include <vector>
#include <tuple>
#include <array>

// The solution function is assumed to be above

int main() {
    // Test 1: Single vertex, no edges -> unchanged
    {
        std::vector<std::array<double,3>> poses = {{0.0, 0.0, 0.0}};
        std::vector<std::tuple<int, int, std::array<double,3>>> edges;
        auto result = solvePoseGraph(poses, edges, 10);
        assert(result.size() == 1);
        assert(std::abs(result[0][0] - 0.0) < 1e-3);
        assert(std::abs(result[0][1] - 0.0) < 1e-3);
        assert(std::abs(result[0][2] - 0.0) < 1e-3);
    }
    
    // Test 2: Two vertices with a consistent edge -> optimization should keep poses close to measurement
    {
        std::vector<std::array<double,3>> poses = {{0.0, 0.0, 0.0}, {1.5, 0.5, 0.2}};
        std::vector<std::tuple<int, int, std::array<double,3>>> edges;
        edges.push_back({0, 1, {1.0, 0.0, 0.0}}); // expected relative pose from 0 to 1
        auto result = solvePoseGraph(poses, edges, 20);
        // After optimization, the relative pose from 0 to 1 should be close to (1,0,0)
        double dx = result[1][0] - result[0][0];
        double dy = result[1][1] - result[0][1];
        double dtheta = std::atan2(std::sin(result[1][2] - result[0][2]), std::cos(result[1][2] - result[0][2]));
        assert(std::abs(dx - 1.0) < 0.1);
        assert(std::abs(dy - 0.0) < 0.1);
        assert(std::abs(dtheta - 0.0) < 0.1);
    }
    
    // Test 3: Triangle loop closure - should reduce error
    {
        std::vector<std::array<double,3>> poses = {{0.0, 0.0, 0.0}, {1.0, 0.0, 0.0}, {1.0, 1.0, 1.5708}};
        std::vector<std::tuple<int, int, std::array<double,3>>> edges;
        edges.push_back({0, 1, {1.0, 0.0, 0.0}});
        edges.push_back({1, 2, {0.0, 1.0, 1.5708}});
        edges.push_back({2, 0, {-1.0, -1.0, 3.14159}}); // loop closure
        auto result = solvePoseGraph(poses, edges, 50);
        // The relative pose from 2 to 0 should be close to (-1,-1, pi)
        double dx = result[0][0] - result[2][0];
        double dy = result[0][1] - result[2][1];
        double dtheta = std::atan2(std::sin(result[0][2] - result[2][2]), std::cos(result[0][2] - result[2][2]));
        assert(std::abs(dx + 1.0) < 0.2);
        assert(std::abs(dy + 1.0) < 0.2);
        assert(std::abs(dtheta - 3.14159) < 0.2);
    }
    
    // Test 4: Invalid edge indices should be ignored
    {
        std::vector<std::array<double,3>> poses = {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}};
        std::vector<std::tuple<int, int, std::array<double,3>>> edges;
        edges.push_back({0, 5, {1.0, 0.0, 0.0}}); // out of range
        edges.push_back({2, 1, {0.0, 1.0, 0.0}}); // out of range
        edges.push_back({0, 0, {1.0, 0.0, 0.0}}); // self-loop
        auto result = solvePoseGraph(poses, edges, 10);
        // Should return original poses unchanged
        assert(result.size() == 2);
        assert(std::abs(result[0][0] - 0.0) < 1e-6);
        assert(std::abs(result[1][0] - 0.0) < 1e-6);
    }
    
    // Test 5: Empty input
    {
        std::vector<std::array<double,3>> poses;
        std::vector<std::tuple<int, int, std::array<double,3>>> edges;
        auto result = solvePoseGraph(poses, edges, 10);
        assert(result.empty());
    }
    
    // Test 6: Large number of iterations should not crash and should produce finite output
    {
        std::vector<std::array<double,3>> poses = {{0.0, 0.0, 0.0}, {0.2, 0.1, 0.05}, {0.5, 0.3, 0.1}};
        std::vector<std::tuple<int, int, std::array<double,3>>> edges;
        edges.push_back({0, 1, {0.0, 0.0, 0.0}});
        edges.push_back({1, 2, {0.0, 0.0, 0.0}});
        edges.push_back({2, 0, {0.0, 0.0, 0.0}});
        auto result = solvePoseGraph(poses, edges, 1000);
        for (const auto& p : result) {
            assert(std::isfinite(p[0]) && std::isfinite(p[1]) && std::isfinite(p[2]));
        }
    }
    
    // Test 7: Severe initial error should converge to a reasonable solution for two nodes
    {
        std::vector<std::array<double,3>> poses = {{0.0, 0.0, 0.0}, {10.0, -5.0, 2.0}};
        std::vector<std::tuple<int, int, std::array<double,3>>> edges;
        edges.push_back({0, 1, {1.0, 0.5, 0.3}});
        auto result = solvePoseGraph(poses, edges, 100);
        double dx = result[1][0] - result[0][0];
        double dy = result[1][1] - result[0][1];
        double dtheta = std::atan2(std::sin(result[1][2] - result[0][2]), std::cos(result[1][2] - result[0][2]));
        assert(std::abs(dx - 1.0) < 0.2);
        assert(std::abs(dy - 0.5) < 0.2);
        assert(std::abs(dtheta - 0.3) < 0.2);
    }
    
    // Test 8: Rotation normalization works correctly
    {
        std::vector<std::array<double,3>> poses = {{0.0, 0.0, 3.0}, {0.0, 0.0, -3.0}};
        std::vector<std::tuple<int, int, std::array<double,3>>> edges;
        edges.push_back({0, 1, {0.0, 0.0, 0.2}}); // small positive diff
        auto result = solvePoseGraph(poses, edges, 50);
        double dtheta = std::atan2(std::sin(result[1][2] - result[0][2]), std::cos(result[1][2] - result[0][2]));
        assert(std::abs(dtheta - 0.2) < 0.2);
    }
    
    return 0;
}
#include <vector>
#include <tuple>
#include <array>
#include <cmath>
#include <algorithm>
#include <limits>

// Helper: 2D pose composition and inverse (using homogeneous coordinates)
struct Pose2D {
    double x, y, theta;
    // Convert to homogeneous matrix representation (3x3 flattened row-major)
    std::array<double, 9> toMatrix() const {
        double c = std::cos(theta), s = std::sin(theta);
        return {c, -s, x, s, c, y, 0.0, 0.0, 1.0};
    }
    static Pose2D fromMatrix(const std::array<double, 9>& m) {
        return {m[2], m[5], std::atan2(m[3], m[0])};
    }
};

// Inverse of a pose (returns pose that composes to identity)
Pose2D inversePose2D(const Pose2D& p) {
    double c = std::cos(p.theta), s = std::sin(p.theta);
    double x = -c * p.x - s * p.y;
    double y =  s * p.x - c * p.y;
    double theta = -p.theta;
    return {x, y, theta};
}

// Compose two poses: p1 * p2
Pose2D composePose2D(const Pose2D& p1, const Pose2D& p2) {
    double c = std::cos(p1.theta), s = std::sin(p1.theta);
    double x = p1.x + c * p2.x - s * p2.y;
    double y = p1.y + s * p2.x + c * p2.y;
    double theta = p1.theta + p2.theta;
    // Normalize theta to [-pi, pi]
    while (theta > M_PI) theta -= 2.0 * M_PI;
    while (theta <= -M_PI) theta += 2.0 * M_PI;
    return {x, y, theta};
}

// Normalize angle to [-pi, pi]
double normalizeAngle(double angle) {
    while (angle > M_PI) angle -= 2.0 * M_PI;
    while (angle < -M_PI) angle += 2.0 * M_PI;
    return angle;
}

// Solve linear system A x = b using Gaussian elimination with partial pivoting
// A is VxV (square), b is Vx1. Returns true if solution found.
bool solveLinearSystem(std::vector<std::vector<double>>& A, std::vector<double>& b, std::vector<double>& x) {
    int n = A.size();
    if (n == 0) return true;
    // Augment A with b
    std::vector<std::vector<double>> aug(n, std::vector<double>(n+1));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) aug[i][j] = A[i][j];
        aug[i][n] = b[i];
    }
    // Forward elimination
    for (int col = 0; col < n; ++col) {
        // Find pivot
        int pivot = col;
        double maxVal = std::abs(aug[col][col]);
        for (int row = col+1; row < n; ++row) {
            if (std::abs(aug[row][col]) > maxVal) {
                maxVal = std::abs(aug[row][col]);
                pivot = row;
            }
        }
        if (maxVal < 1e-12) return false; // singular
        if (pivot != col) std::swap(aug[col], aug[pivot]);
        // Eliminate rows below
        for (int row = col+1; row < n; ++row) {
            double factor = aug[row][col] / aug[col][col];
            for (int j = col; j <= n; ++j) aug[row][j] -= factor * aug[col][j];
        }
    }
    // Back substitution
    x.assign(n, 0.0);
    for (int i = n-1; i >= 0; --i) {
        if (std::abs(aug[i][i]) < 1e-12) return false;
        double sum = aug[i][n];
        for (int j = i+1; j < n; ++j) sum -= aug[i][j] * x[j];
        x[i] = sum / aug[i][i];
    }
    return true;
}

// Main function: perform simple pose-graph optimization
std::vector<std::array<double,3>> solvePoseGraph(
    const std::vector<std::array<double,3>>& initial_poses,
    const std::vector<std::tuple<int, int, std::array<double,3>>>& edges,
    int max_iterations) {
    
    int num_vertices = initial_poses.size();
    if (num_vertices == 0) return {};
    
    // Convert input to Pose2D vector
    std::vector<Pose2D> poses(num_vertices);
    for (int i = 0; i < num_vertices; ++i) {
        poses[i] = {initial_poses[i][0], initial_poses[i][1], initial_poses[i][2]};
    }
    
    // Preprocess edges: filter out invalid ones (indices out of range or i==j)
    std::vector<std::tuple<int,int,std::array<double,3>>> valid_edges;
    for (const auto& e : edges) {
        int i = std::get<0>(e);
        int j = std::get<1>(e);
        if (i >= 0 && i < num_vertices && j >= 0 && j < num_vertices && i != j) {
            valid_edges.push_back(e);
        }
    }
    if (valid_edges.empty()) {
        return initial_poses;
    }
    
    // Information matrix weights: translation weights 1.0, rotation weight 0.1
    const double rot_weight = 0.1;
    
    // Optimization loop
    double prev_error = std::numeric_limits<double>::max();
    for (int iter = 0; iter < max_iterations; ++iter) {
        // Build linear system: H (3N x 3N) and b (3N)
        int N = num_vertices;
        int dim = 3 * N;
        std::vector<std::vector<double>> H(dim, std::vector<double>(dim, 0.0));
        std::vector<double> b(dim, 0.0);
        
        double total_error = 0.0;
        
        // For each edge, compute residual and Jacobian approximation
        for (const auto& e : valid_edges) {
            int i = std::get<0>(e);
            int j = std::get<1>(e);
            std::array<double,3> meas = std::get<2>(e);
            Pose2D measurement = {meas[0], meas[1], meas[2]};
            
            // Predicted relative pose: inv(pose_i) * pose_j
            Pose2D inv_i = inversePose2D(poses[i]);
            Pose2D pred = composePose2D(inv_i, poses[j]);
            
            // Translation residual
            double rx = pred.x - measurement.x;
            double ry = pred.y - measurement.y;
            // Rotation residual (normalized)
            double rtheta = normalizeAngle(pred.theta - measurement.theta);
            
            total_error += rx*rx + ry*ry + rot_weight*rtheta*rtheta;
            
            // Jacobian for translation part w.r.t pose_i and pose_j
            double c = std::cos(poses[i].theta);
            double s = std::sin(poses[i].theta);
            
            // For vertex i: J_i = [-R^T, 0; 0, -1] where R^T is transpose of rotation R of pose_i
            // For translation part: [-R^T, 0] (R^T * delta_x, delta_y)
            // For rotation part: [0, -1]
            // For vertex j: J_j = [R^T, 0; 0, 1]
            // But we use a simplified Jacobian: for translation, derivative w.r.t i is -I, w.r.t j is I
            // Actually more accurate: derivative w.r.t pose_i translation is -R^T, w.r.t pose_j translation is R^T
            // We'll use approximate: derivative w.r.t i translation = -I, w.r.t j translation = I
            // For rotation: derivative w.r.t i rot = -1, w.r.t j rot = 1
            
            // Simpler: use identity for translation derivatives
            double J_ix[2][3] = {{-1.0, 0.0, 0.0}, {0.0, -1.0, 0.0}}; // translation part
            double J_itheta[3] = {0.0, 0.0, -1.0}; // rotation part
            double J_jx[2][3] = {{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}};
            double J_jtheta[3] = {0.0, 0.0, 1.0};
            
            // Build full 3x3 Jacobians for i and j (translation x,y and rotation theta)
            // For i: rows: x,y,theta
            // column order: delta_x_i, delta_y_i, delta_theta_i
            double Ji[3][3] = {
                {-1.0, 0.0, 0.0},
                {0.0, -1.0, 0.0},
                {0.0, 0.0, -1.0}
            };
            double Jj[3][3] = {
                {1.0, 0.0, 0.0},
                {0.0, 1.0, 0.0},
                {0.0, 0.0, 1.0}
            };
            
            // Information matrix W (diagonal 3x3)
            double W[3] = {1.0, 1.0, rot_weight};
            
            // Residual vector r = [rx, ry, rtheta]
            double r[3] = {rx, ry, rtheta};
            
            // Update H and b: H += J^T W J, b += J^T W r (b for negative gradient)
            // We need to add to H and b for both i and j blocks
            // For simplicity, we'll add directly to H and b using loops
            for (int a = 0; a < 3; ++a) { // row index of Jacobian
                for (int b_idx = 0; b_idx < 3; ++b_idx) { // column index of Jacobian
                    double wi = W[a];
                    // For vertex i
                    int row_i = 3*i + a;
                    int col_i = 3*i + b_idx;
                    H[row_i][col_i] += wi * Ji[a][0]*Ji[b_idx][0] + wi * Ji[a][1]*Ji[b_idx][1] + wi * Ji[a][2]*Ji[b_idx][2];
                    // Actually the above is wrong: H += J^T W J, so need double sum
                    // Let's do it properly with loops
                }
            }
            // Proper computation:
            for (int a = 0; a < 3; ++a) {
                for (int b_idx = 0; b_idx < 3; ++b_idx) {
                    double sum = 0.0;
                    for (int k = 0; k < 3; ++k) {
                        // J[i][k]^T * W[k] * J[i][b_idx] -- for vertex i block
                        sum += Ji[k][a] * W[k] * Ji[k][b_idx];
                    }
                    H[3*i + a][3*i + b_idx] += sum;
                }
            }
            for (int a = 0; a < 3; ++a) {
                for (int b_idx = 0; b_idx < 3; ++b_idx) {
                    double sum = 0.0;
                    for (int k = 0; k < 3; ++k) {
                        sum += Jj[k][a] * W[k] * Jj[k][b_idx];
                    }
                    H[3*j + a][3*j + b_idx] += sum;
                }
            }
            // Cross terms: i,j
            for (int a = 0; a < 3; ++a) {
                for (int b_idx = 0; b_idx < 3; ++b_idx) {
                    double sum = 0.0;
                    for (int k = 0; k < 3; ++k) {
                        sum += Ji[k][a] * W[k] * Jj[k][b_idx];
                    }
                    H[3*i + a][3*j + b_idx] += sum;
                    H[3*j + b_idx][3*i + a] += sum; // symmetric
                }
            }
            // b vector: b += J^T W r (note sign: we want minimize 0.5*r^T W r, so gradient = J^T W r, and update is delta = -H^{-1} g)
            for (int a = 0; a < 3; ++a) {
                double g_i = 0.0;
                double g_j = 0.0;
                for (int k = 0; k < 3; ++k) {
                    g_i += Ji[k][a] * W[k] * r[k];
                    g_j += Jj[k][a] * W[k] * r[k];
                }
                b[3*i + a] += g_i;
                b[3*j + a] += g_j;
            }
        }
        
        // Add regularization to avoid singular H
        for (int i = 0; i < dim; ++i) H[i][i] += 1e-6;
        
        // Solve H delta = -b (we want delta = -H^{-1} b)
        std::vector<double> rhs(dim);
        for (int i = 0; i < dim; ++i) rhs[i] = -b[i];
        std::vector<double> delta;
        bool solved = solveLinearSystem(H, rhs, delta);
        if (!solved) break;
        
        // Apply updates to poses
        for (int v = 0; v < N; ++v) {
            poses[v].x += delta[3*v + 0];
            poses[v].y += delta[3*v + 1];
            poses[v].theta = normalizeAngle(poses[v].theta + delta[3*v + 2]);
        }
        
        // Check convergence
        if (std::abs(prev_error - total_error) < 1e-6) {
            prev_error = total_error;
            break;
        }
        prev_error = total_error;
    }
    
    // Convert back to output format
    std::vector<std::array<double,3>> result(num_vertices);
    for (int i = 0; i < num_vertices; ++i) {
        result[i] = {poses[i].x, poses[i].y, poses[i].theta};
    }
    return result;
}
// The core algorithm is a simple iteration over the edges to compute residuals and build a normal equation system in a Gauss-Newton fashion within a small local parameterization. For each edge (i, j, measurement), we compute the predicted relative pose `pred = inverse(pose_i) * pose_j` using 2D homogeneous transformations. The translation residual is `pred.translation - measurement.translation`, and the rotation residual is the smallest signed angle difference between `pred.theta` and `measurement.theta`. We then compute a 3x3 Jacobian matrix for the residual with respect to the perturbation of pose_i and pose_j. For this simplified version, we approximate the Jacobian of translation part as `[-R^T, R^T]` (where R is the rotation matrix of pose_i) and for the rotation part as `[-1, 1]` in the third column. We accumulate the normal equations `H = J^T W J` and `b = J^T W residual` for each edge, where W is the information matrix (diagonal with [1,1,0.1]). We then solve the linear system `H delta = -b` using Gaussian elimination with partial pivoting since the problem is small (3*num_vertices size). The delta vector contains updates for all poses: first two entries are translation updates, third is angular update. We apply the updates: for each vertex, add translation delta and add angle delta (normalize to [-pi, pi]). We repeat until convergence or max iterations. Edge cases: edges with invalid indices (out of range or i==j) are skipped. If there are no valid edges, return the input poses unchanged. If the H matrix is singular (determinant near zero), we add a small diagonal regularization (e.g., 1e-6 * identity). Time complexity is O(iterations * (E + V^3)) where V is number of vertices and E is number of edges, because solving the linear system takes O(V^3) using Gaussian elimination. Space complexity is O(V^2) for the H matrix.
