Write a standalone C++ function named `heatMethodDistanceOnGrid` that computes an approximate geodesic distance field from a single source vertex on a simple square-grid triangle mesh. The function takes a 2D array of `double` heights (a height field), the grid dimensions `rows` and `cols`, and the integer coordinates of a source grid point `(srcRow, srcCol)`. It returns a `std::vector<double>` of length `rows * cols` containing the approximate distances from the source to every vertex, using the heat method with a lumped mass matrix, cotangent Laplacian, and a short-time diffusion step. The mesh is constructed implicitly: each grid cell is split into two triangles with vertices ordered row-major (index `r * cols + c`). Edge lengths and cotangent weights are computed from the 3D positions (row, col, height). The algorithm should be self-contained, not relying on any external libraries beyond standard C++ headers, and must handle degenerate triangles gracefully by perturbing co-planar or collinear configurations.
#include <cassert>
#include <cmath>
#include <vector>
#include <iostream>

// (The solution function is assumed to be available in the same translation unit.)

int main() {
    // Test 1: Flat grid 2x2, source at (0,0). Distances should be proportional to Euclidean distances in XY plane.
    {
        std::vector<std::vector<double>> heights = {
            {0.0, 0.0},
            {0.0, 0.0}
        };
        auto dist = heatMethodDistanceOnGrid(heights, 2, 2, 0, 0);
        // Grid vertices: (0,0) index0, (1,0) index1, (0,1) index2, (1,1) index3
        // Source at 0, distance should be ~0, ~1, ~1, ~sqrt(2) (approx).
        assert(dist.size() == 4);
        assert(std::abs(dist[0]) < 1e-6);
        assert(std::abs(dist[1] - 1.0) < 0.2);
        assert(std::abs(dist[2] - 1.0) < 0.2);
        assert(std::abs(dist[3] - std::sqrt(2.0)) < 0.3);
    }

    // Test 2: Single vertex grid (1x1)
    {
        std::vector<std::vector<double>> heights = {{5.0}};
        auto dist = heatMethodDistanceOnGrid(heights, 1, 1, 0, 0);
        assert(dist.size() == 1);
        assert(std::abs(dist[0]) < 1e-6);
    }

    // Test 3: 3x3 flat grid, source at center (1,1). All distances should be non-negative and symmetric.
    {
        std::vector<std::vector<double>> heights(3, std::vector<double>(3, 0.0));
        auto dist = heatMethodDistanceOnGrid(heights, 3, 3, 1, 1);
        assert(dist.size() == 9);
        for (double d : dist) {
            assert(d >= -1e-6);
        }
        // Symmetry: vertex (0,0) and (0,2) should have same distance
        assert(std::abs(dist[0] - dist[2]) < 1e-6);
        assert(std::abs(dist[6] - dist[8]) < 1e-6);
    }

    // Test 4: Check that source is zero for a slightly sloped grid
    {
        std::vector<std::vector<double>> heights = {
            {1.0, 2.0},
            {3.0, 4.0}
        };
        auto dist = heatMethodDistanceOnGrid(heights, 2, 2, 1, 1); // source at bottom-right
        assert(std::abs(dist[3]) < 1e-6);
        // Distance to top-left should be larger than to adjacent vertices
        assert(dist[0] > dist[1]);
        assert(dist[0] > dist[2]);
    }

    // Test 5: Degenerate flat grid (all heights equal, but still valid)
    {
        std::vector<std::vector<double>> heights = {
            {7.0, 7.0, 7.0},
            {7.0, 7.0, 7.0}
        };
        auto dist = heatMethodDistanceOnGrid(heights, 2, 3, 0, 1);
        assert(dist.size() == 6);
        assert(std::abs(dist[1]) < 1e-6); // source
        // All distances finite
        for (double d : dist) assert(std::isfinite(d));
    }

    // Test 6: Invalid source coordinates should throw
    {
        bool threw = false;
        try {
            std::vector<std::vector<double>> h = {{0.0}};
            heatMethodDistanceOnGrid(h, 1, 1, 2, 0);
        } catch (const std::invalid_argument&) {
            threw = true;
        }
        assert(threw);
    }

    std::cout << "All tests passed." << std::endl;
    return 0;
}
#include <vector>
#include <cmath>
#include <algorithm>
#include <stdexcept>

// Compute approximate geodesic distances on a height-field grid using the heat method.
// The mesh is an (rows x cols) grid where each cell is split into two triangles.
// Vertices are stored row-major, index = r * cols + c.
// Returns a vector of distances (length rows*cols) from the source vertex.
std::vector<double> heatMethodDistanceOnGrid(const std::vector<std::vector<double>>& heights,
                                             int rows, int cols,
                                             int srcRow, int srcCol) {
    if (rows <= 0 || cols <= 0) return {};
    if (srcRow < 0 || srcRow >= rows || srcCol < 0 || srcCol >= cols) {
        throw std::invalid_argument("Source coordinates out of bounds");
    }

    const int n = rows * cols;
    const int srcIdx = srcRow * cols + srcCol;

    // 3D positions of vertices
    std::vector<std::array<double, 3>> pos(n);
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            int idx = r * cols + c;
            pos[idx] = { static_cast<double>(c), static_cast<double>(r), heights[r][c] };
        }
    }

    // Build triangle list: each cell (r,c) has two triangles
    std::vector<std::array<int, 3>> tris;
    tris.reserve(2 * (rows - 1) * (cols - 1));
    for (int r = 0; r < rows - 1; ++r) {
        for (int c = 0; c < cols - 1; ++c) {
            int i00 = r * cols + c;
            int i10 = (r + 1) * cols + c;
            int i01 = r * cols + (c + 1);
            int i11 = (r + 1) * cols + (c + 1);
            // triangle 1: (i00, i10, i01)
            tris.push_back({i00, i10, i01});
            // triangle 2: (i10, i11, i01)
            tris.push_back({i10, i11, i01});
        }
    }
    int m = static_cast<int>(tris.size());

    // Compute edge lengths per triangle
    auto edgeLen = [&](int a, int b) {
        double dx = pos[a][0] - pos[b][0];
        double dy = pos[a][1] - pos[b][1];
        double dz = pos[a][2] - pos[b][2];
        return std::sqrt(dx*dx + dy*dy + dz*dz);
    };

    // Mean edge length for time constant
    double totalLen = 0.0;
    for (int t = 0; t < m; ++t) {
        auto& tri = tris[t];
        totalLen += edgeLen(tri[0], tri[1]);
        totalLen += edgeLen(tri[1], tri[2]);
        totalLen += edgeLen(tri[2], tri[0]);
    }
    double meanEdge = totalLen / (3.0 * m);
    double t = 0.1 * meanEdge * meanEdge; // short-time parameter

    // Build dense matrices for simplicity: L (n x n), M (diagonal as vector)
    std::vector<std::vector<double>> L(n, std::vector<double>(n, 0.0));
    std::vector<double> M(n, 0.0);

    auto addCotan = [&](int i, int j, double w) {
        L[i][j] -= w;
        L[j][i] -= w;
        L[i][i] += w;
        L[j][j] += w;
    };

    for (int tIdx = 0; tIdx < m; ++tIdx) {
        auto& tri = tris[tIdx];
        int i = tri[0], j = tri[1], k = tri[2];

        // Edge lengths
        double lij = edgeLen(i, j);
        double ljk = edgeLen(j, k);
        double lki = edgeLen(k, i);

        // Area via Heron's formula
        double s = (lij + ljk + lki) / 2.0;
        double area = std::sqrt(std::max(0.0, s * (s - lij) * (s - ljk) * (s - lki)));
        if (area < 1e-12) continue; // degenerate, skip

        // Cotangent weights: cot(angle opposite edge) = (a^2 + b^2 - c^2) / (4 * area)
        // For edge ij, opposite angle is at k, sides are lik and ljk
        double cot_k = (lik2(lki, ljk, lij)) / (4.0 * area); // helper below
        // For edge jk, opposite angle at i
        double cot_i = (lij*lij + lki*lki - ljk*ljk) / (4.0 * area);
        // For edge ki, opposite angle at j
        double cot_j = (lij*lij + ljk*ljk - lki*lki) / (4.0 * area);

        // Add to Laplacian
        addCotan(i, j, cot_k);
        addCotan(j, k, cot_i);
        addCotan(k, i, cot_j);

        // Lumped mass: one-third of area per vertex
        double mass = area / 3.0;
        M[i] += mass;
        M[j] += mass;
        M[k] += mass;
    }

    // Build heat operator H = M + t*L (dense)
    std::vector<std::vector<double>> H(n, std::vector<double>(n, 0.0));
    for (int r = 0; r < n; ++r) {
        for (int c = 0; c < n; ++c) {
            H[r][c] = (r == c ? M[r] : 0.0) + t * L[r][c];
        }
    }

    // Solve H u = b, where b is 1 at source, 0 elsewhere (dense Gaussian elimination)
    std::vector<double> b(n, 0.0);
    b[srcIdx] = 1.0;
    std::vector<double> u = solveLinearSystem(H, b);

    // Compute per-face normalized gradient and divergence
    std::vector<double> div(n, 0.0);
    for (int tIdx = 0; tIdx < m; ++tIdx) {
        auto& tri = tris[tIdx];
        int i = tri[0], j = tri[1], k = tri[2];

        // Compute edge vectors in 2D (x,y) ignoring z? Actually use 3D and project? Simpler: use 2D tangent plane based on mesh plane.
        // For a height field, we can use 2D coordinates (x,y) and gradients in that plane.
        // Compute local gradient of u using finite differences on triangle.
        // Use formula: grad_u = ( (u_j - u_i) * perp(e_k) + ... ) / (2*area) but simpler: solve linear system.
        double u0 = u[i], u1 = u[j], u2 = u[k];
        double x0 = pos[i][0], y0 = pos[i][1];
        double x1 = pos[j][0], y1 = pos[j][1];
        double x2 = pos[k][0], y2 = pos[k][1];

        // Compute gradient in (x,y) plane: solve [ (x1-x0) (x2-x0) ; (y1-y0) (y2-y0) ] * grad = [u1-u0 ; u2-u0]
        double A = x1 - x0, B = x2 - x0;
        double C = y1 - y0, D = y2 - y0;
        double det = A*D - B*C;
        if (std::abs(det) < 1e-12) continue;
        double gx = (D * (u1 - u0) - B * (u2 - u0)) / det;
        double gy = (-C * (u1 - u0) + A * (u2 - u0)) / det;
        double gradMag = std::sqrt(gx*gx + gy*gy);
        if (gradMag < 1e-12) continue;
        gx /= gradMag;
        gy /= gradMag;

        // Divergence per edge: for edge (i,j), contribution = cot_k * dot(e_ij, grad)
        // Use edge vector from i to j: (x1-x0, y1-y0)
        double ex = x1 - x0, ey = y1 - y0;
        double dot_ij = ex * gx + ey * gy;
        // cot_k computed earlier
        double l_ij = edgeLen(i,j);
        double l_jk = edgeLen(j,k);
        double l_ki = edgeLen(k,i);
        double s = (l_ij + l_jk + l_ki) / 2.0;
        double area_here = std::sqrt(std::max(0.0, s * (s - l_ij) * (s - l_jk) * (s - l_ki)));
        double cot_k = (l_ki*l_ki + l_jk*l_jk - l_ij*l_ij) / (4.0 * area_here); // recompute

        // Edge (i,j): contribution to divergence: + at i, - at j? Actually div formula: sum over edges cot * dot(e,grad) * sign
        // Standard: div(f) at vertex v = sum over incident triangles of cot * (grad dot edge) with appropriate signs.
        // We'll follow simpler: for each edge (a,b) in triangle, add w * dot(e_ab, grad) to div[a] - div[b]? Need consistency.
        // Better: use formula from heat method: div = sum over edges cot * (grad dot e) * (indicator at tail - indicator at head)
        // Let's compute for each halfedge (i->j): add cot_k * dot(e_ij, grad) to div[i] and subtract from div[j]
        // Repeat for other edges.
        double ex_jk = x2 - x1, ey_jk = y2 - y1;
        double cot_i = (l_ij*l_ij + l_ki*l_ki - l_jk*l_jk) / (4.0 * area_here);
        double dot_jk = ex_jk * gx + ey_jk * gy;
        double ex_ki = x0 - x2, ey_ki = y0 - y2;
        double cot_j = (l_ij*l_ij + l_jk*l_jk - l_ki*l_ki) / (4.0 * area_here);
        double dot_ki = ex_ki * gx + ey_ki * gy;

        div[i] += cot_k * dot_ij;
        div[j] -= cot_k * dot_ij;
        div[j] += cot_i * dot_jk;
        div[k] -= cot_i * dot_jk;
        div[k] += cot_j * dot_ki;
        div[i] -= cot_j * dot_ki;
    }

    // Solve Poisson: (L + eps*I) d = -div (with small regularization)
    std::vector<std::vector<double>> P(n, std::vector<double>(n, 0.0));
    const double eps = 1e-6;
    for (int r = 0; r < n; ++r) {
        for (int c = 0; c < n; ++c) {
            P[r][c] = L[r][c];
        }
        P[r][r] += eps;
    }
    std::vector<double> rhs(n);
    for (int r = 0; r < n; ++r) rhs[r] = -div[r];
    std::vector<double> d = solveLinearSystem(P, rhs);

    // Shift so distance at source is zero
    double shift = d[srcIdx];
    for (int i = 0; i < n; ++i) d[i] -= shift;

    return d;
}

// Helper: solve dense linear system Ax=b using Gaussian elimination with partial pivoting
std::vector<double> solveLinearSystem(const std::vector<std::vector<double>>& A,
                                      const std::vector<double>& b) {
    int n = b.size();
    // Make a copy of A and b
    std::vector<std::vector<double>> mat(n, std::vector<double>(n + 1, 0.0));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            mat[i][j] = A[i][j];
        }
        mat[i][n] = b[i];
    }

    // Gaussian elimination with partial pivoting
    for (int col = 0; col < n; ++col) {
        // Find pivot
        int pivot = col;
        double maxVal = std::abs(mat[col][col]);
        for (int row = col + 1; row < n; ++row) {
            if (std::abs(mat[row][col]) > maxVal) {
                maxVal = std::abs(mat[row][col]);
                pivot = row;
            }
        }
        if (maxVal < 1e-12) continue; // singular

        // Swap rows
        if (pivot != col) std::swap(mat[pivot], mat[col]);

        // Eliminate
        for (int row = col + 1; row < n; ++row) {
            double factor = mat[row][col] / mat[col][col];
            for (int j = col; j <= n; ++j) {
                mat[row][j] -= factor * mat[col][j];
            }
        }
    }

    // Back substitution
    std::vector<double> x(n, 0.0);
    for (int i = n - 1; i >= 0; --i) {
        double sum = mat[i][n];
        for (int j = i + 1; j < n; ++j) {
            sum -= mat[i][j] * x[j];
        }
        if (std::abs(mat[i][i]) < 1e-12) {
            // Under-determined; set zero (shouldn't happen with regularization)
            x[i] = 0.0;
        } else {
            x[i] = sum / mat[i][i];
        }
    }
    return x;
}
// The heat method for geodesic distance works in three stages: (1) solve a short-time heat diffusion problem with initial heat at the source, (2) compute the normalized gradient of the heat field and evaluate its negative divergence, and (3) solve a Poisson equation to integrate the divergence into a distance field. On a triangle mesh, the heat diffusion step uses the lumped mass matrix `M` (diagonal containing one-third of the adjacent triangle areas per vertex) and the cotangent Laplacian `L`. The heat operator is `H = M + t L`, where `t` is a short time parameter proportional to the square of the mean edge length. After solving `H u = b` (with `b` the initial heat distribution, 1 at the source vertex, 0 elsewhere), the normalized gradient per face is computed using edge vectors in the tangent plane. For each face, the gradient is normalized (with a cutoff to avoid division by zero), then the divergence is accumulated by computing per-edge cotangent-weighted contributions. Finally, solving `L d = -div` (with a small regularization shift to ensure non-singularity) yields the distance values. The result is shifted so the source has distance zero. Edge cases include degenerate triangles where the gradient magnitude is zero—these are handled by skipping normalization. The mesh is generated from a height field, but the same algorithm works for arbitrary triangle meshes. Complexity is dominated by solving two sparse linear systems; for an implicit grid, we use a dense matrix for simplicity (size `n x n`, where `n = rows * cols`), yielding `O(n^3)` time and `O(n^2)` space for small grids. For a production exercise, a direct solver using Gaussian elimination or Cholesky is acceptable.
