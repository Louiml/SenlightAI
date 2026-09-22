Implement a standalone C++ function `computeChargeTransfer` that simulates an iterative electronegativity equalization process on a simplified molecular graph. The function takes: (1) a vector of atom properties, where each atom has an index, an electronegativity parameter `A`, a hardness parameter `B`, and a boolean flag `isHydrogen`; (2) a vector of bonds, each represented as a pair of atom indices; (3) a positive integer `iterations`; and (4) a double `electronegativityFactor` (corresponding to the bond `f` parameter) and a double `hydrogenScale` (corresponding to the `Hplus` parameter). Initially, all atomic charges `q` and electronegativities `chi` are zero. For each iteration from 1 to `iterations` (inclusive), update `chi[i] = B[i] * q[i] + A[i]` for every atom. Then, for each bond between atoms `i` and `j`, compute the current `chi` values; if `chi[i] > chi[j]`, swap the roles so the smaller `chi` atom is the first. Compute the transfer amount `diff = (electronegativityFactor^iteration) * (chi2 - chi1) / d`, where `d` is `hydrogenScale` if the first atom (the one with smaller `chi`) is hydrogen, otherwise `d = A[first] + B[first]`. Add `diff` to the charge of the first atom and subtract `diff` from the charge of the second atom. Return the final vector of charges. Handle edge cases: empty atom list (return empty vector), zero iterations (return all zeros), and bonds referencing invalid indices (ignore such bonds). The provided parameters are all finite positive numbers, but charges may become negative or positive.

// The algorithm directly follows the iterative scheme from the snippet: we maintain two vectors of size `n` (number of atoms) for charges `q` and electronegativities `chi`. Each iteration first recomputes `chi` from the current `q` using the linear relation `B*q + A`. Then, for each bond, we fetch the two `chi` values, ensure the first has the smaller `chi` (swap if necessary, but note that swapping only matters for deciding which atom receives positive `diff`; the magnitude uses `chi2 - chi1` which is nonnegative after sorting). The denominator `d` is chosen based on the element type of the smaller-`chi` atom: if it's hydrogen, use `hydrogenScale`; otherwise use `A + B` of that atom. The transfer `diff` scales with `electronegativityFactor^iteration`, meaning later iterations have exponentially smaller transfers if the factor is less than 1. We then update charges: the smaller-`chi` atom gains `diff` (becomes more positive), the larger-`chi` atom loses `diff`. After all iterations, return `q`. Edge cases: if `n==0`, return empty. If `iterations==0`, return all zeros because the loop doesn't execute. For bonds with invalid indices (out of range), skip them to avoid undefined behavior. Complexity: For each of `I` iterations, we do `O(n)` work for updating `chi` and `O(m)` for bonds, where `m` is the number of valid bonds. Total O(I*(n+m)) time, O(n) auxiliary space. The use of `pow` for each bond per iteration could be optimized by precomputing powers if needed, but the snippet computes it inline; we'll follow the same pattern for clarity.

#include <vector>
#include <cmath>
#include <utility>

// Atom properties: index, A, B, isHydrogen
struct AtomInfo {
    size_t index;
    double A;
    double B;
    bool isHydrogen;
};

// Compute charges via iterative electronegativity equalization
std::vector<double> computeChargeTransfer(
    const std::vector<AtomInfo>& atoms,
    const std::vector<std::pair<size_t, size_t>>& bonds,
    int iterations,
    double electronegativityFactor,
    double hydrogenScale
) {
    const size_t n = atoms.size();
    std::vector<double> q(n, 0.0);
    if (n == 0 || iterations <= 0) return q;

    std::vector<double> chi(n, 0.0);

    for (int alpha = 1; alpha <= iterations; ++alpha) {
        // Update electronegativities from current charges
        for (size_t i = 0; i < n; ++i) {
            chi[i] = atoms[i].B * q[i] + atoms[i].A;
        }

        // Process each bond
        for (const auto& bond : bonds) {
            size_t i = bond.first;
            size_t j = bond.second;
            if (i >= n || j >= n) continue; // invalid indices

            double chi1 = chi[i];
            double chi2 = chi[j];
            size_t first = i;
            size_t second = j;

            if (chi1 > chi2) {
                std::swap(first, second);
                std::swap(chi1, chi2);
            }

            // Determine denominator d
            double d;
            if (atoms[first].isHydrogen) {
                d = hydrogenScale;
            } else {
                d = atoms[first].A + atoms[first].B;
            }

            double diff = std::pow(electronegativityFactor, alpha) * (chi2 - chi1) / d;
            q[first] += diff;
            q[second] -= diff;
        }
    }

    return q;
}

#include <cassert>
#include <cmath>
#include <vector>

// Assume the solution function and struct are defined above.

int main() {
    // Simple two-atom molecule: H and Cl (A/B values arbitrary)
    std::vector<AtomInfo> atoms = {
        {0, 2.0, 1.0, true},   // H
        {1, 3.0, 1.5, false}   // Cl
    };
    std::vector<std::pair<size_t,size_t>> bonds = {{0,1}};
    auto q = computeChargeTransfer(atoms, bonds, 1, 0.5, 2.0);
    // After 1 iteration: chi1=2.0, chi2=3.0, diff=0.5*(1.0)/2.0=0.25
    // q[0]=0.25, q[1]=-0.25
    assert(std::fabs(q[0] - 0.25) < 1e-9);
    assert(std::fabs(q[1] + 0.25) < 1e-9);

    // Multiple iterations, factor<1, charges converge toward something
    auto q2 = computeChargeTransfer(atoms, bonds, 5, 0.5, 2.0);
    // Iteration 2: chi0=2.0+0.25=2.25, chi1=3.0-0.375=2.625, diff=0.25*0.375/2=0.046875
    // q0=0.296875, q1=-0.296875, etc. Just check sign and larger magnitude
    assert(q2[0] > 0.25 && q2[1] < -0.25);

    // Zero iterations returns zeros
    auto q0 = computeChargeTransfer(atoms, bonds, 0, 0.5, 2.0);
    assert(q0[0] == 0.0 && q0[1] == 0.0);

    // Invalid bond indices are ignored
    std::vector<std::pair<size_t,size_t>> badBonds = {{0,5}};
    auto qbad = computeChargeTransfer(atoms, badBonds, 3, 0.5, 2.0);
    assert(qbad[0] == 0.0 && qbad[1] == 0.0);

    // Empty atom list returns empty
    std::vector<AtomInfo> emptyAtoms;
    auto qempty = computeChargeTransfer(emptyAtoms, {}, 2, 1.0, 1.0);
    assert(qempty.empty());

    // Three atoms with bond chain: 0-1, 1-2; check charge conservation
    std::vector<AtomInfo> atoms3 = {
        {0, 1.0, 1.0, false},
        {1, 2.0, 1.0, false},
        {2, 3.0, 1.0, false}
    };
    std::vector<std::pair<size_t,size_t>> bonds3 = {{0,1},{1,2}};
    auto q3 = computeChargeTransfer(atoms3, bonds3, 2, 0.8, 1.0);
    double total = 0.0;
    for (double val : q3) total += val;
    assert(std::fabs(total) < 1e-9); // charges sum to zero

    // All atoms with same A and B -> no transfer because chi equal
    std::vector<AtomInfo> atomsEq = {
        {0, 1.0, 0.5, false},
        {1, 1.0, 0.5, false}
    };
    std::vector<std::pair<size_t,size_t>> bondsEq = {{0,1}};
    auto qEq = computeChargeTransfer(atomsEq, bondsEq, 10, 0.5, 1.0);
    assert(qEq[0] == 0.0 && qEq[1] == 0.0);

    // Hydrogen with hydrogenScale small -> larger transfer
    std::vector<AtomInfo> atomsH = {
        {0, 2.0, 1.0, true},
        {1, 3.0, 1.0, false}
    };
    auto qH = computeChargeTransfer(atomsH, bondsH, 1, 0.5, 0.1);
    // chi1=2.0, chi2=3.0, diff=0.5*1.0/0.1=5.0 -> q0=5.0, q1=-5.0
    assert(std::fabs(qH[0] - 5.0) < 1e-9);
    assert(std::fabs(qH[1] + 5.0) < 1e-9);
}
