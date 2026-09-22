// Write a standalone C++ function `computeLJEnergy` that, given a list of \(N\) particles with positions, types, and a potential-energy parameter table (per type pair), computes the total Lennard-Jones (LJ) potential energy of the system using the \(12\)-\(6\) potential form. The function should take as input: the number of particles, arrays of particle positions (`double x[][3]` or `std::vector<std::array<double,3>>`), an array of particle types (1-indexed integers), a 2D vector/table of epsilon values, a 2D vector/table of sigma values, and a cutoff distance `rcut`. The energy calculation must: (1) consider all unique pairs \((i,j)\) with \(i<j\), (2) only include pairs whose squared distance is strictly less than the squared cutoff (`rsq < rcutsq`) — equality is excluded, (3) use the standard \(12\)-\(6\) formula \(4\epsilon[(\sigma/r)^{12} - (\sigma/r)^6]\), and (4) ignore any pair whose distance is beyond the cutoff. The function must return the total energy as a `double`. Assume all particles are within a box and periodic boundary conditions are not applied (i.e., real distances, no wrapping). The function should be self-contained and not rely on external libraries beyond `<vector>`, `<array>`, `<cmath>`, and `<cstddef>`. No mixing rules are needed; the epsilon and sigma tables are already provided for each type pair.
The solution iterates over all unordered pairs \((i,j)\) with \(i<j\). For each pair, it computes the squared distance as the sum of squared component differences. If the squared distance is greater than or equal to the squared cutoff, the pair is skipped (strict inequality). Otherwise, compute \(r = \sqrt{rsq}\), then \(r6 = ((\sigma/r)^6)\), and energy \(= 4 \epsilon (r6^2 - r6)\). Sum all contributions. Edge cases: (1) if any sigma or epsilon is zero, the energy contribution is zero, but that's naturally handled; (2) if the distance is zero (coincident particles), the division by zero occurs — in practice, such pairs are invalid; the function can guard by skipping zero distances (treat as infinite energy, but we ignore for simplicity); (3) type indexing: particle types are 1-indexed, so tables must be at least `(maxType+1) x (maxType+1)`, and access using `[type[i]][type[j]]`. Time complexity: \(O(N^2)\) because there are \(\binom{N}{2}\) pairs. Space complexity: \(O(1)\) additional space beyond the input arrays. The function is simple and robust, with no external dependencies beyond standard C++ headers.
#include <vector>
#include <array>
#include <cmath>
#include <cstddef>

// Compute the total 12-6 Lennard-Jones potential energy for a set of particles.
// Parameters:
//   positions: vector of particle positions (x,y,z)
//   types:     vector of particle types (1-indexed)
//   epsilon:   2D table (size >= (maxType+1) x (maxType+1)) of LJ epsilon values
//   sigma:     2D table (size >= (maxType+1) x (maxType+1)) of LJ sigma values
//   rcut:      cutoff distance (pairs with distance >= rcut are excluded)
// Returns total energy.
double computeLJEnergy(
    const std::vector<std::array<double,3>>& positions,
    const std::vector<int>& types,
    const std::vector<std::vector<double>>& epsilon,
    const std::vector<std::vector<double>>& sigma,
    double rcut)
{
    const size_t N = positions.size();
    const double rcutsq = rcut * rcut;
    double energy = 0.0;

    for (size_t i = 0; i < N; ++i) {
        for (size_t j = i + 1; j < N; ++j) {
            double dx = positions[i][0] - positions[j][0];
            double dy = positions[i][1] - positions[j][1];
            double dz = positions[i][2] - positions[j][2];
            double rsq = dx*dx + dy*dy + dz*dz;

            // Strict inequality: exclude pairs at or beyond cutoff
            if (rsq >= rcutsq) continue;

            // Guard against zero distance (coincident particles) – skip
            if (rsq == 0.0) continue;

            double r = std::sqrt(rsq);
            double s = sigma[types[i]][types[j]];
            double e = epsilon[types[i]][types[j]];

            double sr = s / r;
            double sr6 = sr * sr * sr * sr * sr * sr;
            double sr12 = sr6 * sr6;

            energy += 4.0 * e * (sr12 - sr6);
        }
    }
    return energy;
}
#include <cassert>
#include <vector>
#include <array>

// Forward declaration of the solution function (already defined above).
double computeLJEnergy(
    const std::vector<std::array<double,3>>& positions,
    const std::vector<int>& types,
    const std::vector<std::vector<double>>& epsilon,
    const std::vector<std::vector<double>>& sigma,
    double rcut);

int main() {
    // Test 1: Two particles, known values.
    // Types 1 and 2. epsilon[1][2] = 0.5, sigma[1][2] = 1.0, distance = 2.0, rcut = 5.0
    std::vector<std::array<double,3>> pos1 = {{{0,0,0},{2,0,0}}};
    std::vector<int> types1 = {1,2};
    std::vector<std::vector<double>> eps1 = {{0,0,0},{0,0,0.5},{0,0.5,0}};
    std::vector<std::vector<double>> sig1 = {{0,0,0},{0,0,1.0},{0,1.0,0}};
    // r=2, sr=0.5, sr6=0.015625, sr12=0.000244140625, energy=4*0.5*(0.000244140625-0.015625)= -0.03076171875
    double e1 = computeLJEnergy(pos1, types1, eps1, sig1, 5.0);
    assert(std::abs(e1 - (-0.03076171875)) < 1e-12);

    // Test 2: Three particles, two pairs outside cutoff, one inside.
    // Particles 0 and 1 at distance 1.0 (epsilon=1, sigma=1): energy = 0 (since r=sigma, sr6=1, sr12=1, energy=0)
    // Particles 0 and 2 at distance 0.5 (epsilon=2, sigma=0.5): sr=1, energy=0
    // Particles 1 and 2 at distance sqrt(1.25)≈1.118, with rcut=1.0, excluded.
    std::vector<std::array<double,3>> pos2 = {{{0,0,0},{1,0,0},{0.5,0.5,0}}};
    std::vector<int> types2 = {1,1,1};
    std::vector<std::vector<double>> eps2 = {{0,0,0},{0,1.0,0},{0,0,2.0}};
    std::vector<std::vector<double>> sig2 = {{0,0,0},{0,1.0,0},{0,0,0.5}};
    double e2 = computeLJEnergy(pos2, types2, eps2, sig2, 1.0);
    // Both included pairs have sr=1, so energy=0.0
    assert(std::abs(e2) < 1e-12);

    // Test 3: All pairs beyond cutoff -> zero energy.
    std::vector<std::array<double,3>> pos3 = {{{0,0,0},{10,0,0},{20,0,0}}};
    std::vector<int> types3 = {1,2,3};
    std::vector<std::vector<double>> eps3 = {{0,0,0,0},{0,0,1.0,1.0},{0,1.0,0,1.0},{0,1.0,1.0,0}};
    std::vector<std::vector<double>> sig3 = {{0,0,0,0},{0,0,1.0,1.0},{0,1.0,0,1.0},{0,1.0,1.0,0}};
    double e3 = computeLJEnergy(pos3, types3, eps3, sig3, 5.0);
    assert(std::abs(e3) < 1e-12);

    // Test 4: Single particle -> zero energy.
    std::vector<std::array<double,3>> pos4 = {{{1,2,3}}};
    std::vector<int> types4 = {1};
    std::vector<std::vector<double>> eps4 = {{0,0},{0,1.0}};
    std::vector<std::vector<double>> sig4 = {{0,0},{0,1.0}};
    double e4 = computeLJEnergy(pos4, types4, eps4, sig4, 5.0);
    assert(std::abs(e4) < 1e-12);

    // Test 5: Distance exactly at cutoff is excluded.
    // Two particles at distance 2.0, rcut=2.0. rsq=4.0 >= rcutsq=4.0 -> skip.
    std::vector<std::array<double,3>> pos5 = {{{0,0,0},{2,0,0}}};
    std::vector<int> types5 = {1,1};
    std::vector<std::vector<double>> eps5 = {{0,0},{0,1.0}};
    std::vector<std::vector<double>> sig5 = {{0,0},{0,1.0}};
    double e5 = computeLJEnergy(pos5, types5, eps5, sig5, 2.0);
    assert(std::abs(e5) < 1e-12);

    return 0;
}
