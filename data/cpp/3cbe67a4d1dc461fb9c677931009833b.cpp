// Write a standalone C++ function that computes the nonaffinity measure of a two-dimensional triangular lattice network subjected to affine shear deformation. The function should take as input a constant reference to a `std::vector<double>` representing the current positions of `N` nodes (where `N` is a perfect square integer), along with a base size parameter `L` (the number of rows/columns in the lattice, so `L*L = N`), and a shear strain magnitude `gamma` (a positive scalar). The affine displacement for a node at row `r` and column `c` (using zero-based indexing) is given by `u_aff_x = gamma * c` and `u_aff_y = 0`, assuming the lattice is initially unstretched in the y-direction but the x-coordinate also has a small base offset `base_offset * ((2*r)/(L-1) - 1) * gamma` to mimic a tilting effect—this offset is part of the affine prediction. The nonaffinity measure is defined as `Gamma = (1/(N * gamma^2)) * sum_over_all_nodes( (current_x - affine_x)^2 + (current_y - affine_y)^2 )`. If `gamma` is zero or the position vector size is inconsistent (not exactly `2*N`), return 0.0. The function must handle any grid size `L >= 2`, be numerically stable, and not use any global variables.

The solution iterates over all nodes in row-major order, extracting their current x and y coordinates from the linear index `2*(r*L + c)` and `2*(r*L + c)+1`. For each node, compute the affine predicted x-position as `gamma*c + base_offset * gamma * (2.0*r/(L-1) - 1.0)` (where `base_offset` is a given constant parameter, e.g., 0.1) and the affine y-position as 0.0. Accumulate the squared Euclidean difference into a running sum. After processing all nodes, multiply by `1.0/(L*L*gamma*gamma)`. Edge cases: if `gamma` is near zero (use a small epsilon like 1e-12) return 0.0 to avoid division by zero; if the position vector size is not exactly `2*L*L`, return 0.0 for safety. The formula correctly reduces to zero when all nodes exactly match the affine positions. Time complexity is O(N) where N = L*L, and space complexity is O(1) beyond the input vector itself.

#include <vector>
#include <cmath>
#include <cstddef>

// Compute the nonaffinity measure for a triangular lattice under shear.
// positions: flattened array of size 2*L*L, layout is [x0,y0,x1,y1,...]
// L: number of rows/columns (grid is L x L)
// gamma: applied shear strain (positive)
// base_offset: small constant for tilting component in affine prediction
double nonAffinityMeasure(const std::vector<double>& positions, int L, double gamma, double base_offset = 0.1) {
    if (L < 2) return 0.0;
    if (std::abs(gamma) < 1e-12) return 0.0;
    const std::size_t expected_size = 2 * static_cast<std::size_t>(L) * static_cast<std::size_t>(L);
    if (positions.size() != expected_size) return 0.0;

    double sqr_disp = 0.0;
    const double inv_denom = 1.0 / (static_cast<double>(L) * static_cast<double>(L) * gamma * gamma);

    for (int r = 0; r < L; ++r) {
        const double tilt_factor = (2.0 * r) / (L - 1.0) - 1.0;
        for (int c = 0; c < L; ++c) {
            const std::size_t idx = 2 * static_cast<std::size_t>(r) * L + 2 * c;
            const double cur_x = positions[idx];
            const double cur_y = positions[idx + 1];

            // Affine prediction under shear with tilt offset
            const double aff_x = gamma * c + base_offset * gamma * tilt_factor;
            const double aff_y = 0.0;

            const double dx = cur_x - aff_x;
            const double dy = cur_y - aff_y;
            sqr_disp += dx * dx + dy * dy;
        }
    }

    return inv_denom * sqr_disp;
}

#include <cassert>
#include <cmath>
#include <vector>

// Declaration from solution
double nonAffinityMeasure(const std::vector<double>& positions, int L, double gamma, double base_offset = 0.1);

int main() {
    // Case 1: Perfect affine deformation -> nonaffinity = 0
    {
        int L = 3;
        double gamma = 0.5;
        double base = 0.2;
        std::vector<double> pos(2 * L * L, 0.0);
        int idx = 0;
        for (int r = 0; r < L; ++r) {
            double tilt = (2.0 * r) / (L - 1.0) - 1.0;
            for (int c = 0; c < L; ++c) {
                pos[idx++] = gamma * c + base * gamma * tilt;
                pos[idx++] = 0.0;
            }
        }
        double result = nonAffinityMeasure(pos, L, gamma, base);
        assert(std::abs(result) < 1e-12);
    }

    // Case 2: Zero strain -> returns 0
    {
        int L = 2;
        std::vector<double> pos(8, 1.0);
        assert(nonAffinityMeasure(pos, L, 0.0) == 0.0);
    }

    // Case 3: Invalid size -> returns 0
    {
        int L = 2;
        std::vector<double> pos(7, 0.0);
        assert(nonAffinityMeasure(pos, L, 0.3) == 0.0);
    }

    // Case 4: Single-node displacement from affine -> nonaffinity > 0 and calculable
    {
        int L = 2;
        double gamma = 1.0;
        double base = 0.0; // simplify: no tilt
        // Perfect affine positions for L=2, gamma=1
        std::vector<double> pos(8, 0.0);
        pos[0] = 0.0; pos[1] = 0.0; // (0,0)
        pos[2] = 1.0; pos[3] = 0.0; // (0,1)
        pos[4] = 0.0; pos[5] = 0.0; // (1,0)
        pos[6] = 1.0; pos[7] = 0.0; // (1,1)

        // Move node (1,1) y upward by 1.0
        pos[7] = 1.0;

        double result = nonAffinityMeasure(pos, L, gamma, base);
        // Only contribution: (0-0)^2 + (1-0)^2 = 1
        // N = 4, gamma^2 = 1, so Gamma = 1/4 = 0.25
        assert(std::abs(result - 0.25) < 1e-12);
    }

    // Case 5: All nodes shifted uniformly by constant vector -> nonaffinity > 0 if shift is not affine
    {
        int L = 2;
        double gamma = 0.5;
        double base = 0.1;
        std::vector<double> pos(8, 0.0);
        // Set all to expected affine positions but add constant y=2.0 to all
        int idx = 0;
        for (int r = 0; r < L; ++r) {
            double tilt = (2.0 * r) / (L - 1.0) - 1.0;
            for (int c = 0; c < L; ++c) {
                pos[idx++] = gamma * c + base * gamma * tilt;
                pos[idx++] = 2.0; // constant offset
            }
        }
        double result = nonAffinityMeasure(pos, L, gamma, base);
        // Each node contributes (2.0)^2 = 4.0, N=4, gamma^2=0.25
        // sum = 16, so Gamma = 16/(4*0.25) = 16
        assert(std::abs(result - 16.0) < 1e-12);
    }

    return 0;
}
