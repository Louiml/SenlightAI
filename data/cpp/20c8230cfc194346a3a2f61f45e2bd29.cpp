Write a standalone C++ function `computeKineticEnergyTransfer` that simulates the core energy-swapping logic from the given LAMMPS fix: given a 1D array of `n` atom kinetic energies (all non-negative doubles), a number of bins `nbin` (positive even integer ≥ 4), and a number of swaps `nswap` (positive integer), the function must repeatedly perform the following until `nswap` swaps are completed or no valid pair remains: identify the hottest atom in the lowest bin (the first `prd/nbin` fraction of the domain, assuming domain length is the range from 0 to 1) and the coldest atom in the "upper" bin (the bin starting at bin index `nbin/2`), record the energy difference (hottest − coldest), then remove both from consideration for future swaps (so each atom is involved in at most one swap). The function returns the total energy transferred (sum of differences). Assume the input array is the only data; define the domain as [0,1) with bins of equal width, and treat the array indices 0..n-1 as ordered along the spatial dimension (cartesian coordinate = index / (n-1) if n>1, else 0). Handle edge cases: if the lower or upper bin is empty or contains no candidate, skip that swap. The function must be deterministic and not modify the input array. Use double precision and return the sum as a double. The number of valid swaps is at most `min(nswap, count_lower, count_upper)` but may be less due to ordering. Provide a descriptively named free function with signature `double computeKineticEnergyTransfer(const std::vector<double>& energies, int nbin, int nswap);`
// The algorithm interprets the input vector `energies` as atoms placed along a 1D domain from coordinate 0 to 1 (exclusive). The domain is divided into `nbin` equal-width bins. The "low slab" is bin 0 (coordinates [0, 1/nbin)), and the "high slab" is bin `nbin/2` (coordinates [nbin/2 / nbin, (nbin/2+1)/nbin)). For each swap iteration (up to `nswap` times), we select the atom with the maximum kinetic energy among all atoms in the low slab that have not yet been used, and the atom with the minimum kinetic energy among all atoms in the high slab not yet used. We compute the difference `max_low - min_high` (which is non-negative because we always pick max from low and min from high; but if no candidates exist, skip). Add this difference to a running total. Mark both atoms as used so they are not selected again. Continue until either `nswap` iterations complete or at least one slab has no remaining unused candidate. Implementation uses two priority queues or simple linear scans; given small constraints, a simple repeated linear search across the original array (tracking used flags) is straightforward and correct. Time complexity per iteration is O(n), so total O(n * nswap) worst-case; space O(n) for used flags. Edge cases: n=0 or n=1, bins may be empty, duplicate energies, nbin even but maybe not divisible evenly (but since we compare coordinate to bin boundaries, floating point comparisons must be careful; use direct index-based assignment: coordinate = (double)i / (n-1) if n>1 else 0. For n=1, coordinate 0 is in bin 0, so low slab has that atom, high slab is empty if nbin/2 > 0? Actually with n=1, bin 0 has atom, bin n/2 is a different bin, so high slab empty. For n>1, coordinate i/(n-1) ranges 0 to 1 inclusive; we treat coordinate >= 1 as in last bin, but our slabs are bin 0 and bin n/2, so we ensure coordinate is compared with `< 1.0`? To avoid boundary issues, we define coordinate = i / (n-1) if n>1, else 0. Then bin index = floor(coordinate * nbin), but for coordinate exactly 1.0 (last atom) bin index = nbin, which is out of range; clamp to nbin-1. However, our high slab is bin n/2, so if n is small and n/2 may be > nbin-1? But nbin is even and ≥4, so n/2 is between 2 and nbin-2, always within range. For coordinate exactly 1.0, bin index is nbin, but we clamp to nbin-1, so that atom goes to last bin, not high slab. That's fine. Also for coordinate exactly 0, bin 0. So our mapping ensures each atom gets a bin index integer between 0 and nbin-1. Then low slab = bin 0, high slab = bin nbin/2.
#include <vector>
#include <algorithm>
#include <limits>
#include <cmath>

// Compute total kinetic energy transferred by repeatedly swapping hottest atom
// from the lowest bin and coldest atom from the middle bin, up to nswap swaps.
// Domain is [0,1) divided into nbin equal bins. Lowest bin is index 0,
// middle bin is index nbin/2. Atoms are placed along domain index/(n-1).
double computeKineticEnergyTransfer(const std::vector<double>& energies, int nbin, int nswap) {
    const int n = static_cast<int>(energies.size());
    if (n == 0 || nswap <= 0 || nbin <= 0 || (nbin % 2) != 0) return 0.0;

    // Determine bin index for each atom.
    std::vector<int> binOf(n);
    for (int i = 0; i < n; ++i) {
        double coord;
        if (n == 1) {
            coord = 0.0;
        } else {
            coord = static_cast<double>(i) / (n - 1);
        }
        int bin = std::min(static_cast<int>(coord * nbin), nbin - 1);
        binOf[i] = bin;
    }

    int lowBin = 0;
    int highBin = nbin / 2;

    // Collect candidate indices for low and high slabs.
    std::vector<int> lowCandidates, highCandidates;
    for (int i = 0; i < n; ++i) {
        if (binOf[i] == lowBin) lowCandidates.push_back(i);
        if (binOf[i] == highBin) highCandidates.push_back(i);
    }

    std::vector<bool> used(n, false);
    double totalExchange = 0.0;
    int swapsDone = 0;

    while (swapsDone < nswap) {
        // Find best unused low and high candidates.
        int lowIdx = -1, highIdx = -1;
        double bestLowEnergy = -1.0; // want max
        double bestHighEnergy = std::numeric_limits<double>::max(); // want min

        for (int idx : lowCandidates) {
            if (!used[idx] && energies[idx] > bestLowEnergy) {
                bestLowEnergy = energies[idx];
                lowIdx = idx;
            }
        }
        for (int idx : highCandidates) {
            if (!used[idx] && energies[idx] < bestHighEnergy) {
                bestHighEnergy = energies[idx];
                highIdx = idx;
            }
        }

        if (lowIdx == -1 || highIdx == -1) break; // no valid pair

        double diff = bestLowEnergy - bestHighEnergy; // non-negative
        totalExchange += diff;
        used[lowIdx] = true;
        used[highIdx] = true;
        ++swapsDone;
    }

    return totalExchange;
}
#include <cassert>
#include <vector>
#include <cmath>

// The solution function is declared above; included here for completeness.
// (In actual test, include the solution header or paste function above.)

int main() {
    // Test 1: nbin=4, two atoms in low bin (indices 0 and 1), one in high bin (index 2)
    // Domain: index 0 -> coord 0, index1 -> 0.5, index2 ->1.0 -> clamped to bin 3, not high bin.
    // So high bin (bin 2) is empty, no swaps -> 0.0
    {
        std::vector<double> e = {10.0, 5.0, 7.0};
        double res = computeKineticEnergyTransfer(e, 4, 1);
        assert(res == 0.0);
    }

    // Test 2: nbin=4, n=3, index 0->bin0, index1->bin2 (high), index2->bin3
    // low bin has index0 (energy 10), high bin has index1 (energy 5)
    // diff=5, swap done, total=5
    {
        std::vector<double> e = {10.0, 5.0, 20.0};
        double res = computeKineticEnergyTransfer(e, 4, 1);
        assert(std::fabs(res - 5.0) < 1e-12);
    }

    // Test 3: multiple swaps, nbin=4, n=5, each bin has atom? 
    // Let's construct: n=5, coordinates: 0,0.25,0.5,0.75,1.0
    // bin0: index0 (coord0), index1(coord0.25) -> low
    // bin2: index2(coord0.5) -> high (bin2)
    // bin3: index3(coord0.75), index4(coord1.0 clamped to bin3) -> not high
    // energies: low: 10, 8; high: 3
    // swaps=2, first swap: best low=10 (idx0), best high=3 (idx2) diff=7
    // second swap: best low now 8 (idx1), best high none (only idx2 used) -> break, total=7
    {
        std::vector<double> e = {10.0, 8.0, 3.0, 1.0, 2.0};
        double res = computeKineticEnergyTransfer(e, 4, 2);
        assert(std::fabs(res - 7.0) < 1e-12);
    }

    // Test 4: two swaps possible, nbin=6, n=7
    // coords: 0,1/6,2/6,3/6,4/6,5/6,1.0
    // bin0: idx0 (0), idx1(1/6) -> low
    // bin3 (nbin/2=3): idx3 (3/6=0.5) -> high
    // other indices: idx2->bin2, idx4->bin4, idx5->bin5, idx6->bin5 clamp
    // energies: low: 10, 7; high: 2. swaps=2: first diff=10-2=8, second diff=7-? no other high -> break after first? 
    // Actually second swap: low candidates unused: idx1 (7), high candidates: none (idx3 used) -> break -> total=8
    {
        std::vector<double> e = {10.0, 7.0, 100.0, 2.0, 0.0, 0.0, 0.0};
        double res = computeKineticEnergyTransfer(e, 6, 2);
        assert(std::fabs(res - 8.0) < 1e-12);
    }

    // Test 5: empty input
    {
        std::vector<double> e;
        double res = computeKineticEnergyTransfer(e, 4, 3);
        assert(res == 0.0);
    }

    // Test 6: all same energy, one swap
    {
        std::vector<double> e = {5.0, 5.0, 5.0, 5.0};
        // n=4, coords: 0,1/3,2/3,1.0->bin3
        // bin0: idx0 (coord0) and idx1 (1/3 <0.25? nbin=4, width 0.25, index1=0.333 -> bin1, not bin0)
        // So low bin only idx0; high bin (bin2) covers [0.5,0.75): idx2 (0.666) -> high.
        // diff=5-5=0, total=0
        double res = computeKineticEnergyTransfer(e, 4, 1);
        assert(std::fabs(res) < 1e-12);
    }

    // Test 7: nswap larger than possible, multiple pairs
    {
        // nbin=4, n=6, coords: 0,0.2,0.4,0.6,0.8,1.0
        // bin0: idx0(0), idx1(0.2) -> low
        // bin2: idx3(0.6) -> high
        // bin3: idx4(0.8), idx5(1.0)
        // bin1: idx2(0.4)
        // energies low: 9,4; high: 2. swaps=3: swap1: 9-2=7, swap2: low left 4, high none -> break total=7
        std::vector<double> e = {9.0, 4.0, 1.0, 2.0, 3.0, 0.0};
        double res = computeKineticEnergyTransfer(e, 4, 3);
        assert(std::fabs(res - 7.0) < 1e-12);
    }

    return 0;
}
