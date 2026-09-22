Write a C++ function `simulateDSMC` that models a simplified Direct Simulation Monte Carlo (DSMC) collision process for a single computational cell. The function takes as input: (1) a vector of particle velocities (each represented as `std::array<double,3>` for the x, y, z velocity components), (2) an integer `number_of_collisions` specifying how many collision attempts to perform, and (3) a `double reduced_mass` for the particle pair. The function must perform the collision attempts using a uniform random number generator (e.g., `std::mt19937` seeded with a fixed value for reproducibility). For each collision attempt, randomly select two distinct particles from the list (ensuring the same particle is not chosen twice; if the same index is picked twice, skip that attempt and retry without counting it). Then compute the relative velocity magnitude between the selected particles, and with a probability `min(1.0, relative_velocity * reduced_mass / 10.0)` (where the denominator 10.0 represents a maximum collision rate parameter), scatter the pair elastically: conserve total momentum and kinetic energy by randomly rotating the relative velocity vector uniformly on a sphere while keeping its magnitude unchanged. After all attempts, return the updated vector of particle velocities. The function should be deterministic given the same seed and input, and must handle edge cases such as fewer than 2 particles (return input unchanged) and zero reduced mass (treat probability as 0).

The solution simulates DSMC collisions in a single cell. The main algorithm: (1) Loop exactly `number_of_collisions` times, but within each iteration, if the random pair picks the same particle index twice, decrement the loop counter (so the attempt is retried) and continue; this matches the provided snippet's logic. To pick random indices, use a uniform integer distribution over `[0, N-1]`. For probability calculation, compute the relative velocity magnitude `|v_i - v_j|` for the chosen pair; if `reduced_mass <= 0` or `relative_velocity` is zero, set probability to 0; otherwise probability = `min(1.0, relative_velocity * reduced_mass / 10.0)`. If a uniform random double in `[0,1)` is less than the probability, perform elastic scattering: compute the center-of-mass velocity `v_cm = (v_i + v_j)/2` (since masses are equal in this simplified model, but for generality use reduced mass and masses? The snippet uses different formulas for same-type vs different-type; here we simplify with same mass, so `v_cm = (v_i + v_j)/2`). Then generate a uniformly random unit vector `u` on the sphere (using spherical coordinates: `phi = acos(2*rand-1)`, `theta = 2*pi*rand`, then `u = (sin(phi)*cos(theta), sin(phi)*sin(theta), cos(phi))`), and set `v_i = v_cm + (relative_velocity/2)*u` and `v_j = v_cm - (relative_velocity/2)*u`, which conserves momentum and kinetic energy. Edge cases: if `number_of_collisions <= 0` or `N < 2`, return the input unchanged. If the same index is selected, skip and retry (do not count the attempt). To ensure reproducibility, use a fixed seed (e.g., 12345) for the PRNG. Complexity: each collision attempt is O(1) (with possible retries), and there are exactly `number_of_collisions` successful attempts, so overall O(number_of_collisions) time and O(1) extra space (excluding the output copy). The function modifies a copy of the input vector passed by value, applying `const` correctness to the inputs.

#include <array>
#include <cmath>
#include <random>
#include <vector>

// Simulate a simplified DSMC collision process for a single cell.
// Input: velocities list, number of collision attempts, reduced mass.
// Returns updated velocities after collisions (in-place modification of local copy).
// Uses a fixed seed for deterministic behavior.
std::vector<std::array<double,3>> simulateDSMC(
    std::vector<std::array<double,3>> velocities,
    int number_of_collisions,
    double reduced_mass)
{
    const std::size_t N = velocities.size();
    if (N < 2 || number_of_collisions <= 0) {
        return velocities;
    }

    // Fixed seed for reproducibility.
    std::mt19937 rng(12345);
    std::uniform_int_distribution<std::size_t> index_dist(0, N - 1);
    std::uniform_real_distribution<double> unit_dist(0.0, 1.0);

    for (int attempt = 0; attempt < number_of_collisions; ++attempt) {
        std::size_t i = index_dist(rng);
        std::size_t j = index_dist(rng);
        // If the same particle is selected, retry this attempt without counting it.
        if (i == j) {
            --attempt;
            continue;
        }

        // Relative velocity vector and magnitude.
        double dv[3];
        dv[0] = velocities[i][0] - velocities[j][0];
        dv[1] = velocities[i][1] - velocities[j][1];
        dv[2] = velocities[i][2] - velocities[j][2];
        double relative_velocity_sq = dv[0]*dv[0] + dv[1]*dv[1] + dv[2]*dv[2];
        double relative_velocity = std::sqrt(relative_velocity_sq);

        // Collision probability: min(1, relative_velocity * reduced_mass / 10.0).
        double probability = 0.0;
        if (reduced_mass > 0.0 && relative_velocity > 0.0) {
            probability = std::min(1.0, relative_velocity * reduced_mass / 10.0);
        }

        if (unit_dist(rng) < probability) {
            // Center of mass velocity (equal masses assumed).
            double vcm[3];
            vcm[0] = (velocities[i][0] + velocities[j][0]) * 0.5;
            vcm[1] = (velocities[i][1] + velocities[j][1]) * 0.5;
            vcm[2] = (velocities[i][2] + velocities[j][2]) * 0.5;

            // Generate a uniform random unit vector on the sphere.
            double phi = std::acos(2.0 * unit_dist(rng) - 1.0);
            double theta = 2.0 * M_PI * unit_dist(rng);
            double u[3] = {
                std::sin(phi) * std::cos(theta),
                std::sin(phi) * std::sin(theta),
                std::cos(phi)
            };

            // Half the relative velocity magnitude for scattering.
            double half_mag = relative_velocity * 0.5;

            // Assign new velocities conserving momentum and kinetic energy.
            velocities[i][0] = vcm[0] + half_mag * u[0];
            velocities[i][1] = vcm[1] + half_mag * u[1];
            velocities[i][2] = vcm[2] + half_mag * u[2];
            velocities[j][0] = vcm[0] - half_mag * u[0];
            velocities[j][1] = vcm[1] - half_mag * u[1];
            velocities[j][2] = vcm[2] - half_mag * u[2];
        }
    }

    return velocities;
}

#include <array>
#include <cassert>
#include <cmath>
#include <vector>

// Solution function declaration.
std::vector<std::array<double,3>> simulateDSMC(
    std::vector<std::array<double,3>> velocities,
    int number_of_collisions,
    double reduced_mass);

int main() {
    // Helper to compare vectors with tolerance.
    auto close = [](double a, double b) { return std::fabs(a - b) < 1e-9; };
    auto vec_close = [&](const std::array<double,3>& a, const std::array<double,3>& b) {
        return close(a[0], b[0]) && close(a[1], b[1]) && close(a[2], b[2]);
    };

    // Test 1: Fewer than 2 particles -> unchanged.
    std::vector<std::array<double,3>> one = {{{1.0, 2.0, 3.0}}};
    auto result1 = simulateDSMC(one, 10, 1.0);
    assert(result1.size() == 1 && vec_close(result1[0], one[0]));

    // Test 2: Zero collisions -> unchanged.
    std::vector<std::array<double,3>> two = {{{0.0, 0.0, 0.0}, {1.0, 0.0, 0.0}}};
    auto result2 = simulateDSMC(two, 0, 1.0);
    assert(result2.size() == 2 && vec_close(result2[0], two[0]) && vec_close(result2[1], two[1]));

    // Test 3: With zero reduced mass, no scattering occurs (probability 0).
    auto result3 = simulateDSMC(two, 100, 0.0);
    // Deterministic output; with seed 12345, verify the result matches exactly a reference run.
    // Since we cannot precompute easily, check that velocities are unchanged (because probability is 0).
    assert(vec_close(result3[0], two[0]) && vec_close(result3[1], two[1]));

    // Test 4: With a high reduced mass, probability is 1, collisions happen.
    // Input two particles with zero velocities -> relative velocity is 0, so probability 0.
    std::vector<std::array<double,3>> zeros = {{{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}}};
    auto result4 = simulateDSMC(zeros, 10, 10.0);
    // Since initial velocities are zero, relative velocity is 0, so no scattering.
    assert(vec_close(result4[0], zeros[0]) && vec_close(result4[1], zeros[1]));

    // Test 5: Nonzero velocities with high reduced mass -> after many collisions, momentum and energy approximately conserved.
    std::vector<std::array<double,3>> particles = {{{1.0, 0.0, 0.0}, {-1.0, 0.0, 0.0}}};
    // Before: total momentum (0,0,0), kinetic energy = 0.5*(1^2) + 0.5*((-1)^2) = 1.0
    auto result5 = simulateDSMC(particles, 100, 100.0); // high mass -> probability min(1, rel_vel*100/10) = 1
    // Check momentum conservation: v1 + v2 = (0,0,0)
    assert(vec_close(std::array<double,3>{result5[0][0]+result5[1][0], result5[0][1]+result5[1][1], result5[0][2]+result5[1][2]},
                     std::array<double,3>{0.0, 0.0, 0.0}));
    // Check kinetic energy: 0.5*(|v1|^2 + |v2|^2) should be same as before (1.0), within tolerance.
    double ke_before = 0.5 * (1.0 + 1.0);
    double ke_after = 0.5 * (result5[0][0]*result5[0][0] + result5[0][1]*result5[0][1] + result5[0][2]*result5[0][2]
                             + result5[1][0]*result5[1][0] + result5[1][1]*result5[1][1] + result5[1][2]*result5[1][2]);
    assert(std::fabs(ke_after - ke_before) < 1e-9);

    // Test 6: Determinism with same seed.
    std::vector<std::array<double,3>> input = {{{0.5, -0.2, 1.3}, {2.1, 0.0, -0.7}, {-1.2, 0.8, 0.4}}};
    auto res_a = simulateDSMC(input, 50, 2.0);
    auto res_b = simulateDSMC(input, 50, 2.0);
    assert(res_a.size() == res_b.size());
    for (std::size_t k = 0; k < res_a.size(); ++k) {
        assert(vec_close(res_a[k], res_b[k]));
    }

    // Test 7: Same particle picked (retry mechanism) doesn't cause out-of-bounds or infinite loop.
    // With N=2, many attempts, ensure function terminates and returns exactly 2 velocities.
    auto result7 = simulateDSMC(std::vector<std::array<double,3>>{{{1.0, 1.0, 1.0}, {2.0, 2.0, 2.0}}}, 1000, 5.0);
    assert(result7.size() == 2);

    return 0;
}
