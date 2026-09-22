// Write a standalone C++ function that mimics the core accumulation pattern of the free energy dispatch logic: given two parallel arrays of atomic indices (representing an interaction pair list), an array of per-atom force vector components with different types, and arrays of "lambda" parameters for two perturbation states, compute a simplified "soft-core-like" energy contribution for each pair and accumulate per-atom forces into a force buffer, returning the total energy. Specifically, for each pair (i, j) from the index arrays, calculate a distance between the positions of atom i and atom j (provided as arrays of 3D coordinates), apply a modified Lennard-Jones-like potential using the lambda values (linear interpolation between state A and B parameters), accumulate the derivative of the potential with respect to each atom's coordinates into the force buffer (negated gradient), and accumulate the total energy. The function must handle a variable number of pairs, correctly handle atoms appearing in multiple pairs (accumulating forces), and return the total energy as a `double`. Use `std::vector<size_t>` for indices, `std::vector<std::array<double,3>>` for positions, `std::array<double,3>` for per-atom force output (initialized to zero, accumulated in place), and scalar `double` lambda values. The potential for a pair is: `epsilon_A = 1.0`, `epsilon_B = 2.0`, `sigma_A = 1.0`, `sigma_B = 1.5`, `epsilon = (1.0 - lambda) * epsilon_A + lambda * epsilon_B`, `sigma = (1.0 - lambda) * sigma_A + lambda * sigma_B`, then `r = distance`, `sr = sigma / r`, `sr6 = sr^6`, `energy = 4 * epsilon * (sr6^2 - sr6)`. Force on each atom is negative gradient of that energy with respect to that atom's position; for two-body central force, the magnitude is `-dU/dr` along the unit vector from i to j, split equally in opposite directions (force on i is opposite of force on j). Ensure the function is `const`-correct, uses references for outputs, and returns the total energy.
#include <cassert>
#include <cmath>
#include <vector>
#include <array>

// (The solution function is assumed to be included above)

int main()
{
    // Test case 1: single pair along x-axis, lambda=0 (state A), sigma=1, epsilon=1
    std::vector<size_t> i_idx = {0};
    std::vector<size_t> j_idx = {1};
    std::vector<std::array<double,3>> pos = {{ {0,0,0}, {2,0,0} }};
    std::vector<std::array<double,3>> forces(2, {0,0,0});

    double energy = accumulateSoftCorePairEnergy(i_idx, j_idx, pos, 0.0, forces);
    // r=2, sr=1/2=0.5, sr6=0.015625, sr12=0.000244140625, energy=4*1*(0.000244140625-0.015625)= -0.0615234375
    assert(std::abs(energy - (-0.0615234375)) < 1e-9);

    // Force magnitude on each atom: |dUdr| = | -48*1*(0.000244140625 - 0.5*0.015625)/2 | = | -48*(0.000244140625-0.0078125)/2 | = | -48*(-0.007568359375)/2 | = |0.36328125| = 0.36328125
    // Force on atom0 is +x direction (since force = dU/dr * u, and dU/dr positive? Let's compute: dUdr = -48*(0.000244-0.007812)/2 = -48*(-0.007568)/2 = 0.181640625? Wait recalc: 0.000244140625 - 0.0078125 = -0.007568359375, times -48 = 0.36328125, divided by 2 = 0.181640625. So dUdr = 0.181640625. Unit vector from 0 to 1 is +x. So force on atom0 is +0.181640625 in x.
    assert(std::abs(forces[0][0] - 0.181640625) < 1e-9);
    assert(std::abs(forces[0][1]) < 1e-9);
    assert(std::abs(forces[0][2]) < 1e-9);
    // Force on atom1 is opposite: -0.181640625 in x
    assert(std::abs(forces[1][0] - (-0.181640625)) < 1e-9);
    assert(std::abs(forces[1][1]) < 1e-9);
    assert(std::abs(forces[1][2]) < 1e-9);

    // Test case 2: lambda=1, must have different sigma and epsilon
    std::vector<std::array<double,3>> forces2(2, {0,0,0});
    double energy2 = accumulateSoftCorePairEnergy(i_idx, j_idx, pos, 1.0, forces2);
    // Now epsilon=2, sigma=1.5, so sr=1.5/2=0.75, sr6=0.177978515625, sr12=0.031676... compute: 0.177978515625^2 = 0.031676... let's compute exactly: 0.177978515625^2 = 0.031676... Actually compute: 0.177978515625 * 0.177978515625 = 0.031676... Let's do precise: 0.177978515625^2 = 0.031676... I'll use approximate for test: sr6=0.177978515625, sr12=0.031676... energy = 4*2*(0.031676 - 0.177978) = 8*(-0.146302) = -1.170416. Use approximate tolerance 1e-3 for this test.
    assert(std::abs(energy2 - (-1.170416)) < 1e-3);

    // Test case 3: empty pair list returns 0 and does not modify forces
    std::vector<size_t> empty_i;
    std::vector<size_t> empty_j;
    std::vector<std::array<double,3>> forces3(2, {1.0, 2.0, 3.0});
    double energy3 = accumulateSoftCorePairEnergy(empty_i, empty_j, pos, 0.5, forces3);
    assert(energy3 == 0.0);
    assert(forces3[0][0] == 1.0 && forces3[0][1] == 2.0 && forces3[0][2] == 3.0);

    // Test case 4: multiple pairs, atom appears multiple times, forces accumulate
    std::vector<size_t> i_idx2 = {0, 1, 2};
    std::vector<size_t> j_idx2 = {1, 2, 0};
    std::vector<std::array<double,3>> pos2 = {{ {0,0,0}, {1,0,0}, {0,1,0} }};
    std::vector<std::array<double,3>> forces4(3, {0,0,0});
    double energy4 = accumulateSoftCorePairEnergy(i_idx2, j_idx2, pos2, 0.0, forces4);
    // For r=1, sigma=1, sr=1, sr6=1, sr12=1, energy per pair = 4*1*(1-1)=0, total 0
    assert(energy4 == 0.0);
    // Force for r=1 and sr=1: dUdr = -48*1*(1 - 0.5)/1 = -48*0.5 = -24. For pair (0,1): u=(1,0,0), force on atom0 = -24*1 = -24 in x, on atom1=+24. For pair (1,2): disp from 1 to 2 = (0,1,0)-(1,0,0)=(-1,1,0), r=sqrt(2), sr=1/sqrt(2), but we can just check total force magnitude is symmetric. For simplicity, check that forces[0] is nonzero.
    assert(forces4[0][0] != 0.0 || forces4[0][1] != 0.0);
    assert(forces4[0][0] + forces4[1][0] + forces4[2][0] == 0.0);

    // Test case 5: degenerate zero distance pair should not crash, returns 0 additional energy
    std::vector<size_t> i_idx3 = {0};
    std::vector<size_t> j_idx3 = {0};
    std::vector<std::array<double,3>> pos3 = {{ {1,2,3}, {1,2,3} }};
    std::vector<std::array<double,3>> forces5(1, {0,0,0});
    double energy5 = accumulateSoftCorePairEnergy(i_idx3, j_idx3, pos3, 0.5, forces5);
    assert(energy5 == 0.0);
    assert(forces5[0][0] == 0.0 && forces5[0][1] == 0.0 && forces5[0][2] == 0.0);

    return 0;
}
#include <array>
#include <cmath>
#include <cstddef>
#include <vector>

// Compute soft-core-like energy and accumulate forces for a list of pairs.
// Params:
//   i_indices, j_indices: parallel arrays of atom indices (size P).
//   positions: per-atom 3D coordinates (size >= number of atoms referenced).
//   lambda: perturbation parameter between 0 (state A) and 1 (state B).
//   forces: output per-atom force vectors, size must match positions, initialized to zero.
// Returns: total potential energy.
double accumulateSoftCorePairEnergy(const std::vector<size_t>& i_indices,
                                    const std::vector<size_t>& j_indices,
                                    const std::vector<std::array<double,3>>& positions,
                                    double lambda,
                                    std::vector<std::array<double,3>>& forces)
{
    const size_t numPairs = i_indices.size();
    double totalEnergy = 0.0;

    // State A parameters
    const double epsilonA = 1.0;
    const double sigmaA = 1.0;
    // State B parameters
    const double epsilonB = 2.0;
    const double sigmaB = 1.5;

    // Linear interpolation
    const double epsilon = (1.0 - lambda) * epsilonA + lambda * epsilonB;
    const double sigma = (1.0 - lambda) * sigmaA + lambda * sigmaB;

    for (size_t p = 0; p < numPairs; ++p)
    {
        const size_t ai = i_indices[p];
        const size_t aj = j_indices[p];

        const std::array<double,3>& posI = positions[ai];
        const std::array<double,3>& posJ = positions[aj];

        // Displacement vector from i to j
        std::array<double,3> disp = { posJ[0] - posI[0], posJ[1] - posI[1], posJ[2] - posI[2] };
        double r = std::sqrt(disp[0] * disp[0] + disp[1] * disp[1] + disp[2] * disp[2]);

        // Defensive: avoid division by zero
        if (r < 1e-12)
        {
            continue; // zero distance, skip pair
        }

        double sr = sigma / r;
        double sr6 = sr * sr * sr * sr * sr * sr;
        double sr12 = sr6 * sr6;

        // Energy: 4 * epsilon * (sr12 - sr6)
        double energy = 4.0 * epsilon * (sr12 - sr6);
        totalEnergy += energy;

        // Gradient magnitude: dU/dr = -48 * epsilon * (sr12 - 0.5 * sr6) / r
        double dUdr = -48.0 * epsilon * (sr12 - 0.5 * sr6) / r;

        // Unit vector from i to j: disp / r
        double ux = disp[0] / r;
        double uy = disp[1] / r;
        double uz = disp[2] / r;

        // Force on atom i = (dU/dr) * unit_vector (since force = -grad U, and grad U = dU/dr * unit vector)
        // Force on atom j = -force_i
        double fx = dUdr * ux;
        double fy = dUdr * uy;
        double fz = dUdr * uz;

        forces[ai][0] += fx;
        forces[ai][1] += fy;
        forces[ai][2] += fz;

        forces[aj][0] -= fx;
        forces[aj][1] -= fy;
        forces[aj][2] -= fz;
    }

    return totalEnergy;
}
// The solution involves iterating over each pair index from the two arrays. For each pair, extract the coordinates of atoms `i` and `j`, compute the displacement vector from `i` to `j`, and the distance `r`. If `r` is zero (or extremely small), to avoid division by zero, treat the energy as zero and skip force accumulation (a degenerate case that should be handled defensively). Otherwise, compute the modified parameters `epsilon` and `sigma` based on linear interpolation of the lambda value. Then compute `sr = sigma / r`, `sr6 = sr^6`, `sr12 = sr6 * sr6`, and `energy = 4 * epsilon * (sr12 - sr6)`. The derivative with respect to `r` is `dU/dr = -48 * epsilon * (sr12 - 0.5 * sr6) / r`. The force vector on atom `i` is `force_i = (dU/dr) * (displacement / r)`, and on atom `j` is the negative of that. Accumulate these into the output force arrays. Add the energy to the total. Edge cases: empty index arrays → return 0.0 with no changes. Duplicate atoms in multiple pairs are handled naturally as we accumulate in place. The time complexity is O(P) where P is the number of pairs, each pair requiring constant work; space complexity is O(1) auxiliary besides the input and output vectors. `const` correctness: indices and positions are read-only references; force output is a non-const reference to be modified.
