/*
Given a symmetric non-negative distance matrix represented as a vector of vectors of `double`, write a C++ function `reorderForSmoothTrajectory` that performs a Monte Carlo optimization to reorder the rows/columns (frames) so that the total path cost (sum of squared distances between consecutive frames) is minimized. The function should take the matrix, a maximum number of iterations, a random seed, a Boltzmann temperature parameter (`kT`), and a convergence flag (whether to print progress). It should return the reordered matrix (i.e., the matrix with rows and columns permuted according to the optimized order) and also output (via a callback or reference parameter) the permutation indices (the original frame order). The algorithm must start by identifying the pair of frames with the largest pairwise distance, place them as the first and last frames, then iteratively attempt random swaps of interior frames, accepting swaps that reduce total path cost or with a probability based on the Metropolis criterion when `kT > 0`. If `kT == 0`, only downhill moves are accepted (no uphill steps). The function should track and return the best (minimum energy) configuration found during the search, not necessarily the final configuration after all iterations. The matrix is guaranteed to be square, symmetric, and have at least 3 rows. Only interior indices (from 1 to n-2 inclusive) should be considered for swapping; indices 0 and n-1 are fixed.
*/

#include <vector>
#include <random>
#include <algorithm>
#include <cmath>
#include <cassert>
#include <numeric>
#include <limits>

/**
 * Reorder a symmetric distance matrix to minimize the sum of squared distances
 * between consecutive frames, using Metropolis Monte Carlo.
 *
 * @param matrix    Symmetric non-negative n x n matrix (n >= 3).
 * @param maxiter   Maximum number of swap attempts.
 * @param seed      Random seed (if 0, use a fixed default).
 * @param kT        Temperature parameter; 0 disables uphill moves.
 * @param bestOrder Output parameter: the optimized permutation of original indices.
 * @return The reordered matrix (rows/columns permuted according to bestOrder).
 */
std::vector<std::vector<double>> reorderForSmoothTrajectory(
    const std::vector<std::vector<double>>& matrix,
    int maxiter,
    unsigned seed,
    double kT,
    std::vector<int>& bestOrder)
{
    int n = static_cast<int>(matrix.size());
    assert(n >= 3 && "Matrix must have at least 3 rows");
    assert(matrix.size() == matrix[0].size() && "Matrix must be square");

    // Initialize random engine
    if (seed == 0) {
        seed = 12345; // fixed default
    }
    std::mt19937 rng(seed);
    std::uniform_int_distribution<int> posDist(1, n - 2); // interior positions
    std::uniform_real_distribution<double> unitDist(0.0, 1.0);

    // Find the pair with maximum distance
    double maxDist = -1.0;
    int iMax = -1, jMax = -1;
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            if (matrix[i][j] > maxDist) {
                maxDist = matrix[i][j];
                iMax = i;
                jMax = j;
            }
        }
    }
    assert(maxDist >= 0 && "Matrix should contain non-negative entries");

    // Build initial order: place largest-distance pair at ends
    std::vector<int> order(n);
    order[0] = iMax;
    order[n - 1] = jMax;
    int idx = 1;
    for (int k = 0; k < n; ++k) {
        if (k != iMax && k != jMax) {
            order[idx++] = k;
        }
    }

    // Energy function: sum of squared distances between consecutive frames
    auto energy = [&](const std::vector<int>& ord) -> double {
        double e = 0.0;
        for (int i = 0; i < n - 1; ++i) {
            double d = matrix[ord[i]][ord[i + 1]];
            e += d * d;
        }
        return e;
    };

    double curEnergy = energy(order);
    double minEnergy = curEnergy;
    bestOrder = order;

    // If n == 3, only one interior position; no swaps possible
    if (n <= 3) {
        // Build reordered matrix
        std::vector<std::vector<double>> result(n, std::vector<double>(n));
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                result[i][j] = matrix[bestOrder[i]][bestOrder[j]];
            }
        }
        return result;
    }

    // Monte Carlo optimization
    for (int iter = 0; iter < maxiter; ++iter) {
        // Pick two distinct interior positions
        int posA, posB;
        do {
            posA = posDist(rng);
            posB = posDist(rng);
        } while (posA == posB);

        // Compute energy after swap (this can be optimized but for clarity we recompute)
        std::swap(order[posA], order[posB]);
        double newEnergy = energy(order);

        double prob = 0.0;
        if (newEnergy < curEnergy) {
            prob = 1.0; // downhill move always accepted
        } else if (kT > 0) {
            prob = std::exp(-(newEnergy - curEnergy) / (maxDist * kT));
        }

        if (prob >= 1.0 || unitDist(rng) < prob) {
            // Accept swap
            curEnergy = newEnergy;
            if (curEnergy < minEnergy) {
                minEnergy = curEnergy;
                bestOrder = order;
            }
        } else {
            // Reject swap
            std::swap(order[posA], order[posB]);
        }
    }

    // Build reordered matrix using the best found order
    std::vector<std::vector<double>> result(n, std::vector<double>(n));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            result[i][j] = matrix[bestOrder[i]][bestOrder[j]];
        }
    }
    return result;
}

#include <cassert>
#include <vector>
#include <cmath>

// (The solution function is assumed to be defined above this point.)

int main() {
    // Simple 3x3 case: no swaps possible, order [0,1,2] or [1,0,2] depending on max pair
    {
        std::vector<std::vector<double>> m = {
            {0.0, 1.0, 5.0},
            {1.0, 0.0, 2.0},
            {5.0, 2.0, 0.0}
        };
        std::vector<int> bestOrder;
        auto result = reorderForSmoothTrajectory(m, 100, 42, 0.0, bestOrder);
        // Max pair is (0,2) with distance 5
        assert(bestOrder[0] == 0 && bestOrder[2] == 2);
        assert(bestOrder[1] == 1); // only interior
        assert(result.size() == 3);
        // Reordered matrix should have 5.0 at [0][2]
        assert(fabs(result[0][2] - 5.0) < 1e-9);
    }

    // Deterministic case: control matrix so that optimal order is known
    {
        std::vector<std::vector<double>> m = {
            {0.0, 1.0, 10.0, 10.0},
            {1.0, 0.0, 10.0, 10.0},
            {10.0, 10.0, 0.0, 1.0},
            {10.0, 10.0, 1.0, 0.0}
        };
        // Largest pair is (0,2) and (0,3) etc., but (0,2) distance 10
        std::vector<int> bestOrder;
        auto result = reorderForSmoothTrajectory(m, 1000, 7, 0.0, bestOrder);
        // With kT=0, only downhill moves accepted
        // The optimal path should connect the two clusters: e.g., 0-1 (1) then 1-2 (10) then 2-3 (1) = sum squares = 1+100+1=102
        // Or 0-1-3-2 etc. Many orders give same energy. Check that endpoints are from max pair.
        // At least verify that the first and last are a max-distance pair
        double dFirstLast = m[bestOrder[0]][bestOrder.back()];
        assert(dFirstLast >= 9.99); // one of the 10.0 distances
        // Verify energy is better than a poor order like [0,2,1,3]
        double energy = 0;
        for (int i = 0; i < 3; ++i) {
            double d = result[i][i+1];
            energy += d*d;
        }
        // Poor order: 0-2 (10), 2-1 (10), 1-3 (10) -> 300
        assert(energy < 200);
    }

    // Larger matrix with trivial structure: all distances equal -> any order is optimal
    {
        int n = 5;
        std::vector<std::vector<double>> m(n, std::vector<double>(n, 1.0));
        for (int i = 0; i < n; ++i) m[i][i] = 0.0;
        std::vector<int> bestOrder;
        auto result = reorderForSmoothTrajectory(m, 100, 0, 0.1, bestOrder);
        // No matter the order, sum of squares = (n-1)*1 = 4
        double energy = 0;
        for (int i = 0; i < n-1; ++i) energy += result[i][i+1] * result[i][i+1];
        assert(fabs(energy - 4.0) < 1e-9);
        assert(bestOrder.size() == n);
        // Verify bestOrder is a permutation of 0..n-1
        std::vector<int> sorted = bestOrder;
        std::sort(sorted.begin(), sorted.end());
        for (int i = 0; i < n; ++i) assert(sorted[i] == i);
    }

    // Test that kT=0 never does uphill moves: create a case where a local minimum traps it,
    // but the function still returns the best found (which is the initial, which may not be global).
    {
        // Matrix with a clear optimum that requires a non-trivial swap
        std::vector<std::vector<double>> m = {
            {0.0, 0.1, 9.0, 9.0, 9.0},
            {0.1, 0.0, 0.2, 9.0, 9.0},
            {9.0, 0.2, 0.0, 0.3, 9.0},
            {9.0, 9.0, 0.3, 0.0, 0.4},
            {9.0, 9.0, 9.0, 0.4, 0.0}
        };
        std::vector<int> bestOrder;
        auto result = reorderForSmoothTrajectory(m, 50, 1, 0.0, bestOrder);
        // Endpoints must be the pair with max distance (9.0)
        assert(fabs(m[bestOrder[0]][bestOrder.back()] - 9.0) < 1e-9);
    }

    return 0;
}

// The core of the solution is a simulated annealing–style Metropolis Monte Carlo search over the permutation space of interior frame indices. The energy function is the sum of squared distances between consecutive frames in the current order: `E = Σ_{i=0}^{n-2} (matrix[order[i]][order[i+1]])^2`. Initially, we find the pair (i,j) with the maximum distance in the original matrix. We set the initial order so that frame `i` is first and frame `j` is last (the two frames with the largest distance are endpoints of the smoothest path). For interior positions, we keep the remaining frames in their original relative order. Then, for each iteration up to `maxiter`, we randomly select two distinct interior positions (both in [1, n-2]) and compute the energy change from swapping those two entries in the current order. If the new energy is lower, we accept the swap and update the current energy; if the new energy is higher and `kT > 0`, we accept with probability `exp(-(ΔE)/(maxDist * kT))` where `maxDist` is the maximum pairwise distance in the original matrix (used to normalize the energy scale). If `kT == 0`, uphill moves are always rejected. Whenever a new global minimum energy is found, we store a copy of the current order as the best permutation. After all iterations, we use the best permutation to reorder both the rows and columns of the matrix (since the matrix is symmetric, reordering rows and then columns with the same permutation yields the correct reordered matrix). Edge cases: n = 3 means only one interior index, so no swaps are possible; in that case, the initial order is already optimal and we return it. The distance matrix values are non-negative, and the matrix is symmetric, so we only need to read `matrix[i][j]` with `i < j` for energy computations after swapping. For time complexity: each energy evaluation after a swap can be done in O(1) by computing only the differences at the swapped positions (since only the edges incident to the swapped positions change), but for simplicity and correctness we compute the full energy in O(n) per iteration; with `maxiter` iterations this is O(maxiter * n). Space complexity is O(n) for the order and best-order arrays, plus O(n^2) for the output matrix. The implementation should use `std::vector` and `std::mt19937` for the random engine, with `std::uniform_int_distribution` for picking interior positions and `std::uniform_real_distribution` for the Metropolis acceptance.
