// Write a C++ function `std::vector<double> compute_cnp(const std::vector<std::vector<double>>& coords, double cutoff)` that computes a common-neighbor parameter (CNP) value for each atom in a 3D system of point particles. The system consists of `N` atoms with coordinates given in a vector of `N` vectors, each containing exactly three doubles (x, y, z). For each atom `i`, the function must:
// 1. Determine all neighbors within the given `cutoff` distance (using a strict `<` comparison, and excluding the atom itself).
// 2. For each neighbor pair `(i, j)`, compute the set of common neighbors — atoms `k` that are within `cutoff` distance of both `i` and `j`. For each common neighbor `k`, compute the vector `2*x_k - x_i - x_j` (component-wise for x, y, z). Sum the squared Euclidean norm of these vectors over all common neighbors for the pair `(i, j)`.
// 3. For atom `i`, sum the above pair contributions over all its neighbors, then divide by the number of neighbors of `i`. If atom `i` has zero neighbors, its CNP value is 0.0.
// Return a `std::vector<double>` of length `N` containing the computed CNP value for each atom, in the same order as the input. Assume the input coordinates are non-empty, and each coordinate vector has exactly three elements. The cutoff is a positive double.
#include <cassert>
#include <cmath>
#include <vector>

// Function under test is declared above; here we provide a small main with assertions.
int main() {
    // Single atom: no neighbors → CNP = 0
    {
        std::vector<std::vector<double>> coords = {{0.0, 0.0, 0.0}};
        auto cnp = compute_cnp(coords, 1.0);
        assert(cnp.size() == 1);
        assert(std::fabs(cnp[0] - 0.0) < 1e-12);
    }

    // Two atoms far apart: no neighbors → both CNP = 0
    {
        std::vector<std::vector<double>> coords = {{0.0, 0.0, 0.0}, {10.0, 0.0, 0.0}};
        auto cnp = compute_cnp(coords, 1.0);
        assert(std::fabs(cnp[0]) < 1e-12);
        assert(std::fabs(cnp[1]) < 1e-12);
    }

    // Two atoms close: each has one neighbor, but no common neighbors → CNP = 0
    {
        std::vector<std::vector<double>> coords = {{0.0, 0.0, 0.0}, {0.5, 0.0, 0.0}};
        auto cnp = compute_cnp(coords, 1.0);
        assert(std::fabs(cnp[0]) < 1e-12);
        assert(std::fabs(cnp[1]) < 1e-12);
    }

    // Linear chain of 3 atoms at spacing 1: cutoff 1.5, each atom has two neighbors.
    // For the middle atom, its two neighbors have no common neighbors (the middle's
    // neighbor list contains both ends, but each end is not within cutoff of the other end? 
    // Actually ends are 2.0 apart, cutoff 1.5 → not neighbors, so no common neighbors → CNP=0)
    {
        std::vector<std::vector<double>> coords = {{0.0, 0.0, 0.0}, {1.0, 0.0, 0.0}, {2.0, 0.0, 0.0}};
        auto cnp = compute_cnp(coords, 1.5);
        assert(std::fabs(cnp[0]) < 1e-12);
        assert(std::fabs(cnp[1]) < 1e-12);
        assert(std::fabs(cnp[2]) < 1e-12);
    }

    // Equilateral triangle of side length 1.0, cutoff 1.1:
    // Each atom has 2 neighbors. For any pair (i,j), the third atom is common neighbor.
    // For each pair, common neighbor vector = 2*x_k - x_i - x_j.
    // For equilateral triangle, this vector is zero (since centroid property? Actually
    // for any triangle, the sum of vectors from i and j to the third point equals something,
    // but let's compute numerically). 
    // Coordinates: A=(0,0,0), B=(1,0,0), C=(0.5, sqrt(3)/2, 0)
    // For pair (A,B), common neighbor C: vec = 2*C - A - B = (2*0.5-0-1, 2*0.866025-0-0, 0) = (0, 1.73205, 0), squared norm = 3.0.
    // So for atom A, pair with B contributes 3.0, pair with C also contributes 3.0 (symmetrically).
    // Atom A has 2 neighbors → CNP = (3+3)/2 = 3.0.
    {
        double sqrt3 = std::sqrt(3.0);
        std::vector<std::vector<double>> coords = {
            {0.0, 0.0, 0.0},
            {1.0, 0.0, 0.0},
            {0.5, sqrt3/2.0, 0.0}
        };
        auto cnp = compute_cnp(coords, 1.1);
        assert(std::fabs(cnp[0] - 3.0) < 1e-12);
        assert(std::fabs(cnp[1] - 3.0) < 1e-12);
        assert(std::fabs(cnp[2] - 3.0) < 1e-12);
    }

    // Square planar (2D) with side length 1.0, cutoff 1.5 (so each atom has 3 neighbors: 
    // two adjacent, one diagonal). For each atom, test one pair explicitly?
    // Simpler: verify that CNP is same for all atoms by symmetry and >0.
    {
        std::vector<std::vector<double>> coords = {
            {0.0, 0.0, 0.0},
            {1.0, 0.0, 0.0},
            {1.0, 1.0, 0.0},
            {0.0, 1.0, 0.0}
        };
        auto cnp = compute_cnp(coords, 1.5);
        assert(cnp.size() == 4);
        for (double val : cnp) {
            assert(val > 0.0);
        }
        // All should be equal due to symmetry
        assert(std::fabs(cnp[0] - cnp[1]) < 1e-12);
        assert(std::fabs(cnp[0] - cnp[2]) < 1e-12);
        assert(std::fabs(cnp[0] - cnp[3]) < 1e-12);
    }

    // Test cutoff exactly equal to distance: strict <, so no neighbor
    {
        std::vector<std::vector<double>> coords = {{0.0, 0.0, 0.0}, {1.0, 0.0, 0.0}};
        auto cnp = compute_cnp(coords, 1.0);
        assert(std::fabs(cnp[0]) < 1e-12);
        assert(std::fabs(cnp[1]) < 1e-12);
    }

    return 0;
}
#include <vector>
#include <cmath>

// Compute common-neighbor parameter (CNP) for a set of 3D coordinates.
// `coords` is a vector of points, each point is a vector of 3 doubles.
// `cutoff` is the neighbor cutoff distance (strictly less than cutoff counts).
// Returns a vector of CNP values for each input point, in the same order.
std::vector<double> compute_cnp(const std::vector<std::vector<double>>& coords, double cutoff) {
    const size_t N = coords.size();
    std::vector<double> cnp(N, 0.0);
    
    // Precompute squared cutoff for efficiency
    double cutsq = cutoff * cutoff;
    
    // Build neighbor lists for each atom: indices of all atoms within cutoff
    std::vector<std::vector<size_t>> neighbors(N);
    for (size_t i = 0; i < N; ++i) {
        const auto& xi = coords[i];
        for (size_t j = 0; j < N; ++j) {
            if (i == j) continue;
            const auto& xj = coords[j];
            double dx = xi[0] - xj[0];
            double dy = xi[1] - xj[1];
            double dz = xi[2] - xj[2];
            double rsq = dx*dx + dy*dy + dz*dz;
            if (rsq < cutsq) {
                neighbors[i].push_back(j);
            }
        }
    }
    
    // For each atom i, process each neighbor j to find common neighbors
    for (size_t i = 0; i < N; ++i) {
        const auto& xi = coords[i];
        const auto& neigh_i = neighbors[i];
        if (neigh_i.empty()) continue; // zero neighbors => CNP = 0.0
        
        double sum_all_pairs = 0.0;
        
        for (size_t j_idx = 0; j_idx < neigh_i.size(); ++j_idx) {
            size_t j = neigh_i[j_idx];
            const auto& xj = coords[j];
            
            // Find common neighbors of i and j: atoms k such that k is neighbor of i
            // and k is within cutoff of j, and k != j. Also k != i by construction.
            double pair_sum = 0.0;
            for (size_t k_idx = 0; k_idx < neigh_i.size(); ++k_idx) {
                size_t k = neigh_i[k_idx];
                if (k == j) continue; // exclude j itself
                
                // Check if k is within cutoff of j
                double dx = xj[0] - coords[k][0];
                double dy = xj[1] - coords[k][1];
                double dz = xj[2] - coords[k][2];
                double rsq = dx*dx + dy*dy + dz*dz;
                if (rsq < cutsq) {
                    // Vector: 2*x_k - x_i - x_j
                    double rx = 2.0*coords[k][0] - xi[0] - xj[0];
                    double ry = 2.0*coords[k][1] - xi[1] - xj[1];
                    double rz = 2.0*coords[k][2] - xi[2] - xj[2];
                    pair_sum += rx*rx + ry*ry + rz*rz;
                }
            }
            sum_all_pairs += pair_sum;
        }
        
        // Normalize by number of neighbors
        cnp[i] = sum_all_pairs / neigh_i.size();
    }
    
    return cnp;
}
// The algorithm follows a brute-force all-pairs approach because the task is standalone and does not require an optimized neighbor list implementation. For each atom `i`, we first scan all other atoms to build a list of its neighbors (those within the cutoff). Then for each neighbor `j` of `i`, we find common neighbors by scanning the neighbor list of `i` and checking whether each candidate `k` (excluding `j` itself) is within the cutoff of `j`. This avoids double-counting because we iterate over ordered pairs `(i,j)` and `j` is a neighbor of `i`; for a given `i`, we do not symmetrize, which is consistent with the original LAMMPS code that sums over ordered neighbors. For each common neighbor `k`, we compute the vector `2*x_k - x_i - x_j` component-wise, accumulate the squared norm into a running sum for the pair, and then after processing all common neighbors for that pair, add the pair sum to `cnp[i]`. After processing all neighbors of `i`, we divide `cnp[i]` by the number of neighbors (if zero, we set the value to 0.0 to avoid division by zero). Edge cases include isolated atoms (zero neighbors), atoms at the boundary where the cutoff may produce varying neighbor counts, and degenerate configurations where common neighbors might include the atom itself? The algorithm explicitly excludes `k == j` when finding common neighbors, and because we use the neighbor list of `i` (which excludes `i`), `k` never equals `i`. However, when checking whether `k` is within cutoff of `j`, the original code uses a strict `<`, so we must replicate that. Time complexity is `O(N^3)` in the worst case because for each pair `(i,j)` we potentially scan all atoms to check common neighbors; more precisely, for each `i`, we scan all atoms to build neighbors (`O(N)`), then for each neighbor `j`, we scan the neighbor list of `i` (up to `N`) and check each against `j` — leading to `O(N^3)` overall. Space complexity is `O(N)` for the neighbor lists (stored as vectors per atom) plus the output vector, so `O(N)` auxiliary space.
