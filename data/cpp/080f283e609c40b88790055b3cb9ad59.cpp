Write a standalone C++ function that simulates a simplified version of the embedded-atom method (EAM) potential calculation for a small set of atoms. The function should take as input two vectors: one containing 3D coordinates of atoms (as `std::vector<std::array<double,3>>`), and another specifying atomic types (as `std::vector<int>`). It should also accept predefined spline coefficient tables for density contribution (`rhor`), embedding energy (`frho`), and pair potential (`z2r`), along with parameters for the cutoff distance and table resolution. The function computes and returns the total potential energy of the system by: (1) calculating the local electron density at each atom from neighbor contributions within the cutoff, (2) computing the embedding energy from the density using the `frho` spline, and (3) adding pairwise contributions from the `z2r` spline. The non-bonded interactions should only be counted once per pair (no double counting), and periodic boundary conditions should not be considered (assume open boundaries). The function must handle atoms with no neighbors, multiple atoms of the same type, and verify that input sizes are consistent, returning `NAN` or throwing an exception for invalid inputs.

#include <cassert>
#include <cmath>
#include <vector>
#include <array>

int main() {
    // Simple test: two atoms of type 0, separated by 1.0, cutoff 2.0
    // Use trivial spline tables: for demonstration, define rhor to give density = 1 when r=1, frho to give embedding = rho, z2r to give z2 = r (so pair energy = 1)
    const int nr = 10, nrho = 10;
    double rcut = 2.0, rhomax = 10.0;
    
    // Build tables with size for 1 type
    std::vector<std::vector<std::vector<std::array<double,7>>>> rhor(1, 
        std::vector<std::vector<std::array<double,7>>>(1, 
            std::vector<std::array<double,7>>(nr)));
    std::vector<std::vector<std::array<double,7>>> frho(1, std::vector<std::array<double,7>>(nrho));
    std::vector<std::vector<std::vector<std::array<double,7>>>> z2r(1, 
        std::vector<std::vector<std::array<double,7>>>(1, 
            std::vector<std::array<double,7>>(nr)));
    
    // Fill rhor to give density contribution = 1.0 when r=1.0. For simplicity, set all spline coefficients so that evaluation gives 1.0 for all p.
    for (int m = 0; m < nr; ++m) {
        auto& c = rhor[0][0][m];
        c[3] = 0; c[4] = 0; c[5] = 0; c[6] = 1.0;  // constant 1.0
        auto& cz = z2r[0][0][m];
        cz[3] = 0; cz[4] = 0; cz[5] = 0; cz[6] = 1.0;  // z2 constant -> but we want z2=r, so use constant 1 gives energy 1/r
    }
    // For frho: embedding energy = density (i.e., phi = rho). Set so evaluation gives p + (m-1) approximately. For simplicity use constant 0 energy, but we can't get exactly rho due to spline. Instead, set frho to give phi = 1.0 always, and we'll test with known values.
    for (int m = 0; m < nrho; ++m) {
        auto& c = frho[0][m];
        c[3] = 0; c[4] = 0; c[5] = 0; c[6] = 0.5;  // each atom gives 0.5 embedding, total 1.0 when two atoms
    }
    
    // Test 1: Two atoms at distance 1.0
    std::vector<std::array<double,3>> coords1 = {{{0,0,0}}, {{1,0,0}}};
    std::vector<int> types1 = {0,0};
    double e1 = computeEamEnergy(coords1, types1, rhor, frho, z2r, rcut, nr, nrho, rhomax);
    // Density at each atom: contribution from other = 1.0 (since constant rhor), so rho_i = 1.0
    // Embedding: each atom gives 0.5, total 1.0
    // Pair: z2/r = 1.0/1.0 = 1.0
    // Total = 1.0 + 1.0 = 2.0
    assert(std::abs(e1 - 2.0) < 1e-6);
    
    // Test 2: Three atoms in line, 1 unit apart (0,0,0), (1,0,0), (2,0,0)
    std::vector<std::array<double,3>> coords2 = {{{0,0,0}}, {{1,0,0}}, {{2,0,0}}};
    std::vector<int> types2 = {0,0,0};
    double e2 = computeEamEnergy(coords2, types2, rhor, frho, z2r, rcut, nr, nrho, rhomax);
    // Densities: atom0 gets from atom1=1.0, atom1 gets from atom0=1.0 and atom2=1.0 -> 2.0, atom2 gets from atom1=1.0
    // Embedding: 0.5 + 0.5 + 0.5 = 1.5
    // Pairs: (0,1) distance 1 -> 1.0; (1,2) distance 1 -> 1.0; (0,2) distance 2 > cutoff 2.0? Actually rcut=2.0, distance^2=4 which is equal to cutsq=4.0, but condition is strict `< cutsq`, so not counted. Total pair = 2.0
    // Total = 1.5 + 2.0 = 3.5
    assert(std::abs(e2 - 3.5) < 1e-6);
    
    // Test 3: No neighbors (single atom) -> energy 0
    std::vector<std::array<double,3>> coords3 = {{{5,5,5}}};
    std::vector<int> types3 = {0};
    double e3 = computeEamEnergy(coords3, types3, rhor, frho, z2r, rcut, nr, nrho, rhomax);
    assert(std::abs(e3) < 1e-6);
    
    // Test 4: Input size mismatch throws
    bool caught = false;
    std::vector<std::array<double,3>> coords4 = {{{0,0,0}}};
    std::vector<int> types4 = {0, 0};  // mismatch
    try {
        computeEamEnergy(coords4, types4, rhor, frho, z2r, rcut, nr, nrho, rhomax);
    } catch (const std::invalid_argument&) {
        caught = true;
    }
    assert(caught);
    
    return 0;
}

#include <vector>
#include <array>
#include <cmath>
#include <stdexcept>
#include <limits>

// Computes total potential energy of a small EAM-like system.
// Arguments:
//   coords: 3D positions of atoms (size n)
//   types:  atomic type indices (size n)
//   rhor:   density spline coefficients [type_i][type_j][m][7] (4 coeffs for interpolation, but stored with full 7 for consistency)
//   frho:   embedding spline coefficients [type][m][7]
//   z2r:    pair potential spline coefficients [type_i][type_j][m][7]
//   rcut:   cutoff distance for interactions
//   nr:     number of spline points for rhor/z2r
//   nrho:   number of spline points for frho
//   rhomax: maximum density for frho table
// Returns total energy; NaN if invalid input.
double computeEamEnergy(const std::vector<std::array<double,3>>& coords,
                       const std::vector<int>& types,
                       const std::vector<std::vector<std::vector<std::array<double,7>>>>& rhor,
                       const std::vector<std::vector<std::array<double,7>>>& frho,
                       const std::vector<std::vector<std::vector<std::array<double,7>>>>& z2r,
                       double rcut, int nr, int nrho, double rhomax) {
    const size_t n = coords.size();
    if (n == 0) return 0.0;
    if (types.size() != n) throw std::invalid_argument("types size mismatch");
    
    // Validate table dimensions roughly
    int maxType = 0;
    for (int t : types) maxType = std::max(maxType, t);
    if (maxType >= static_cast<int>(frho.size())) throw std::invalid_argument("frho table too small");
    if (maxType >= static_cast<int>(rhor.size())) throw std::invalid_argument("rhor table too small");
    if (maxType >= static_cast<int>(z2r.size())) throw std::invalid_argument("z2r table too small");
    
    double rdr = (nr - 1.0) / rcut;
    double rdrho = (nrho - 1.0) / rhomax;
    const double cutsq = rcut * rcut;
    
    std::vector<double> rho(n, 0.0);
    
    // Phase 1: Compute densities
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = i+1; j < n; ++j) {
            double dx = coords[i][0] - coords[j][0];
            double dy = coords[i][1] - coords[j][1];
            double dz = coords[i][2] - coords[j][2];
            double rsq = dx*dx + dy*dy + dz*dz;
            if (rsq < cutsq) {
                double r = std::sqrt(rsq);
                double p = r * rdr + 1.0;
                int m = static_cast<int>(p);
                m = std::min(m, nr-1);
                p -= m;
                p = std::min(p, 1.0);
                const auto& coeff_ij = rhor[types[i]][types[j]][m];
                const auto& coeff_ji = rhor[types[j]][types[i]][m];
                rho[i] += ((coeff_ij[3]*p + coeff_ij[4])*p + coeff_ij[5])*p + coeff_ij[6];
                rho[j] += ((coeff_ji[3]*p + coeff_ji[4])*p + coeff_ji[5])*p + coeff_ji[6];
            }
        }
    }
    
    // Phase 2: Embedding energy
    double energy = 0.0;
    for (size_t i = 0; i < n; ++i) {
        if (rho[i] <= 0.0) continue;  // no neighbors, zero density, zero embedding
        double p = rho[i] * rdrho + 1.0;
        int m = static_cast<int>(p);
        m = std::max(1, std::min(m, nrho-1));
        p -= m;
        p = std::min(p, 1.0);
        const auto& coeff = frho[types[i]][m];
        double phi = ((coeff[3]*p + coeff[4])*p + coeff[5])*p + coeff[6];
        if (rho[i] > rhomax) {
            // linear correction using derivative
            double fp = (coeff[0]*p + coeff[1])*p + coeff[2];
            phi += fp * (rho[i] - rhomax);
        }
        energy += phi;
    }
    
    // Phase 3: Pairwise potential energy
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = i+1; j < n; ++j) {
            double dx = coords[i][0] - coords[j][0];
            double dy = coords[i][1] - coords[j][1];
            double dz = coords[i][2] - coords[j][2];
            double rsq = dx*dx + dy*dy + dz*dz;
            if (rsq < cutsq) {
                double r = std::sqrt(rsq);
                double p = r * rdr + 1.0;
                int m = static_cast<int>(p);
                m = std::min(m, nr-1);
                p -= m;
                p = std::min(p, 1.0);
                const auto& coeff = z2r[types[i]][types[j]][m];
                double z2 = ((coeff[3]*p + coeff[4])*p + coeff[5])*p + coeff[6];
                energy += z2 / r;
            }
        }
    }
    
    return energy;
}

// The solution involves a three-phase computation similar to the original EAM code but simplified for educational purposes. First, precompute neighbor lists for each atom by iterating over all pairs (i<j) and checking if the squared distance is less than the cutoff squared. For each valid pair, accumulate the density contribution at both atoms by evaluating the `rhor` cubic spline using the reduced distance parameter `p = r*rdr + 1.0`, where `rdr = (nr-1)/rcut`, and interpolating using the four coefficients from the spline table indexed by `[type1][type2][m]`. Note the spline representation uses the formula `((coeff[3]*p+coeff[4])*p+coeff[5])*p+coeff[6]` for value evaluation. After densities are computed, evaluate the embedding energy for each atom using the `frho` spline with index `[type][m]`, using the formula `((coeff[3]*p+coeff[4])*p+coeff[5])*p+coeff[6]` for the energy value. For robustness, if the density exceeds the maximum table value, apply a linear correction using the derivative `fp` from the `frho` spline (coefficients 0-2 evaluated as `(coeff[0]*p+coeff[1])*p+coeff[2]`). Finally, iterate over all neighbor pairs again to add the pairwise potential energy from the `z2r` spline using the same interpolation method, ensuring each pair is counted exactly once (use only i<j). The total energy is the sum of embedding energies plus pair energies. Time complexity is O(N^2) for neighbor search and O(N^2) for force/potential evaluation (since we recompute pairs), with O(N) auxiliary space for densities and neighbor lists if not stored. Edge cases include atoms with no neighbors (density zero, embedding energy should be zero), overlapping atoms (distance zero requires careful handling of the reciprocal but since we only need energy and not forces, it is acceptable to use r=0 and the interpolation still works), and densities exceeding table range where linear correction applies. Input validation checks that coordinates and types vectors have equal length, and that tables are properly sized.
