Write a standalone C++ function named `compute_quartic_bond_forces` that simulates the core force and energy calculation for a quartic bond potential. The function should take as parameters: a vector of bond types (1-indexed integers), a vector of equilibrium bond lengths `r0` (indexed by bond type), three vectors of force constants `k2`, `k3`, `k4` (indexed by bond type), and a vector of bond structures, where each bond structure contains indices `i1`, `i2` (0-indexed into particle positions) and a `type`. Additionally, take a 2D vector of particle positions `x` (size N x 3) and a 2D vector of forces `f` (size N x 3) that should be updated in-place. The function should return a struct containing the total bond energy (sum of `k2*dr^2 + k3*dr^3 + k4*dr^4` over all bonds) and a boolean flag indicating whether any bond length was zero (to avoid division by zero). For each bond, compute `dr = r - r0[type]`, then the force magnitude scalar `fbond = -(2*k2*dr + 3*k3*dr^2 + 4*k4*dr^3) / r` (if r > 0, else fbond = 0). Apply `fbond * (delx, dely, delz)` to atom `i1` and the negative to atom `i2`, where `del = x[i1] - x[i2]`. Ensure all indices are valid (assume they are), and handle the case where `r0`, `k2`, `k3`, `k4` vectors are 1-indexed (size at least max bond type + 1). The function should be `const`-correct: do not modify the input bond list, positions, or coefficient vectors. Return the struct.
// The solution iterates over each bond in the bond list. For each bond, it extracts the two atom indices and the bond type, then computes the displacement vector `del` between the two atoms. The squared distance `rsq` is computed, then `r = sqrt(rsq)`. The deviation `dr` from equilibrium `r0[type]` is found. The energy contribution is a polynomial in `dr`: `k2*dr^2 + k3*dr^3 + k4*dr^4`. The derivative with respect to `dr` gives `de/dr = 2*k2*dr + 3*k3*dr^2 + 4*k4*dr^3`. The force on each atom is derived from the negative gradient of energy with respect to positions. Since `r` depends on `del`, the force magnitude scalar is `fbond = -(de/dr)/r`. If `r` is zero (two atoms exactly at the same position), the force is undefined; we set `fbond = 0` and flag that a zero bond length occurred. Then the force vector is `fbond * del` for atom `i1` and `-fbond * del` for atom `i2`. The total energy accumulates across all bonds. Edge cases: a bond with `r=0` is handled; bond types must be within the coefficient vector bounds (assumed valid); the coefficient vectors may have extra unused entries at index 0. Time complexity is O(B) where B is the number of bonds, and space complexity is O(1) extra (besides the input/output vectors).
#include <vector>
#include <cmath>
#include <utility>

struct BondResult {
    double total_energy;
    bool zero_length_detected;
};

struct Bond {
    int i1;
    int i2;
    int type;
};

// Computes quartic bond energy and updates forces in-place.
// r0, k2, k3, k4 are 1-indexed by bond type (index 0 unused).
// x: positions (N x 3), f: forces (N x 3) to be updated.
// Returns total energy and a flag if any bond had r == 0.
BondResult compute_quartic_bond_forces(
    const std::vector<Bond>& bonds,
    const std::vector<double>& r0,
    const std::vector<double>& k2,
    const std::vector<double>& k3,
    const std::vector<double>& k4,
    const std::vector<std::vector<double>>& x,
    std::vector<std::vector<double>>& f)
{
    double total_energy = 0.0;
    bool zero_length = false;

    for (const auto& bond : bonds) {
        int i1 = bond.i1;
        int i2 = bond.i2;
        int type = bond.type;

        double delx = x[i1][0] - x[i2][0];
        double dely = x[i1][1] - x[i2][1];
        double delz = x[i1][2] - x[i2][2];

        double rsq = delx * delx + dely * dely + delz * delz;
        double r = std::sqrt(rsq);
        double dr = r - r0[type];
        double dr2 = dr * dr;
        double dr3 = dr2 * dr;
        double dr4 = dr3 * dr;

        // Energy: k2*dr^2 + k3*dr^3 + k4*dr^4
        total_energy += k2[type] * dr2 + k3[type] * dr3 + k4[type] * dr4;

        // Force magnitude: -(dE/dr) / r
        double de_dr = 2.0 * k2[type] * dr + 3.0 * k3[type] * dr2 + 4.0 * k4[type] * dr3;
        double fbond;
        if (r > 0.0) {
            fbond = -de_dr / r;
        } else {
            fbond = 0.0;
            zero_length = true;
        }

        // Update forces
        f[i1][0] += delx * fbond;
        f[i1][1] += dely * fbond;
        f[i1][2] += delz * fbond;

        f[i2][0] -= delx * fbond;
        f[i2][1] -= dely * fbond;
        f[i2][2] -= delz * fbond;
    }

    return {total_energy, zero_length};
}
#include <cassert>
#include <cmath>
#include <vector>
#include "solution.h" // assuming the above code is in solution.h

int main() {
    // Test 1: Simple two-atom bond at equilibrium
    {
        std::vector<Bond> bonds = {{0, 1, 1}};
        std::vector<double> r0 = {0.0, 1.0};
        std::vector<double> k2 = {0.0, 2.0};
        std::vector<double> k3 = {0.0, 0.0};
        std::vector<double> k4 = {0.0, 0.0};
        std::vector<std::vector<double>> x = {{0.0, 0.0, 0.0}, {1.0, 0.0, 0.0}};
        std::vector<std::vector<double>> f = {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}};
        BondResult res = compute_quartic_bond_forces(bonds, r0, k2, k3, k4, x, f);
        assert(std::fabs(res.total_energy) < 1e-12);
        assert(!res.zero_length_detected);
        assert(f[0][0] == 0.0 && f[0][1] == 0.0 && f[0][2] == 0.0);
        assert(f[1][0] == 0.0 && f[1][1] == 0.0 && f[1][2] == 0.0);
    }

    // Test 2: Stretched bond with harmonic only (k2=2, r0=1, r=2) -> dr=1
    // Energy = 2*1 = 2; de/dr = 4*1 = 4; fbond = -4/2 = -2
    // del = (-1,0,0)? actually x[0]-x[1] = (0-2) = -2? Wait x[0]=(0,0,0), x[1]=(2,0,0) => delx=-2
    // fbond = -4/2 = -2; f[0] += delx*fbond = (-2)*(-2)=4? Wait fbond is scalar, so f[0].x += (-2)*(-2) = 4; f[1].x -= (-2)*(-2) = -4
    {
        std::vector<Bond> bonds = {{0, 1, 1}};
        std::vector<double> r0 = {0.0, 1.0};
        std::vector<double> k2 = {0.0, 2.0};
        std::vector<double> k3 = {0.0, 0.0};
        std::vector<double> k4 = {0.0, 0.0};
        std::vector<std::vector<double>> x = {{0.0, 0.0, 0.0}, {2.0, 0.0, 0.0}};
        std::vector<std::vector<double>> f = {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}};
        BondResult res = compute_quartic_bond_forces(bonds, r0, k2, k3, k4, x, f);
        assert(std::fabs(res.total_energy - 2.0) < 1e-12);
        assert(!res.zero_length_detected);
        assert(std::fabs(f[0][0] - 4.0) < 1e-12); // -2 * (-2)
        assert(std::fabs(f[1][0] - (-4.0)) < 1e-12);
        assert(f[0][1] == 0.0 && f[0][2] == 0.0 && f[1][1] == 0.0 && f[1][2] == 0.0);
    }

    // Test 3: Two bonds sharing an atom
    {
        std::vector<Bond> bonds = {{0, 1, 1}, {1, 2, 2}};
        std::vector<double> r0 = {0.0, 1.0, 1.0};
        std::vector<double> k2 = {0.0, 1.0, 1.0};
        std::vector<double> k3 = {0.0, 0.0, 0.0};
        std::vector<double> k4 = {0.0, 0.0, 0.0};
        std::vector<std::vector<double>> x = {{0.0, 0.0, 0.0}, {1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}};
        std::vector<std::vector<double>> f = {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}};
        BondResult res = compute_quartic_bond_forces(bonds, r0, k2, k3, k4, x, f);
        // Both bonds at equilibrium? Bond0: r=1, r0=1 => dr=0, no force. Bond1: x[1]=(1,0,0), x[2]=(0,1,0) => r=sqrt(2), dr=sqrt(2)-1 ~0.414, energy=dr^2, force applied.
        // Check total energy: (sqrt(2)-1)^2 ≈ 0.1716
        double expected_energy = (std::sqrt(2.0) - 1.0) * (std::sqrt(2.0) - 1.0);
        assert(std::fabs(res.total_energy - expected_energy) < 1e-12);
        assert(!res.zero_length_detected);
        // Manual force check not necessary; just ensure no crash and sum of forces zero
        double sum_fx = f[0][0] + f[1][0] + f[2][0];
        double sum_fy = f[0][1] + f[1][1] + f[2][1];
        assert(std::fabs(sum_fx) < 1e-12);
        assert(std::fabs(sum_fy) < 1e-12);
    }

    // Test 4: Zero-length bond (atoms overlapping) sets flag, no crash
    {
        std::vector<Bond> bonds = {{0, 1, 1}};
        std::vector<double> r0 = {0.0, 1.0};
        std::vector<double> k2 = {0.0, 1.0};
        std::vector<double> k3 = {0.0, 0.0};
        std::vector<double> k4 = {0.0, 0.0};
        std::vector<std::vector<double>> x = {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}};
        std::vector<std::vector<double>> f = {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}};
        BondResult res = compute_quartic_bond_forces(bonds, r0, k2, k3, k4, x, f);
        assert(res.zero_length_detected);
        assert(res.total_energy == 1.0); // dr = 0 - 1 = -1, energy = 1
        assert(f[0][0] == 0.0 && f[0][1] == 0.0 && f[0][2] == 0.0);
        assert(f[1][0] == 0.0 && f[1][1] == 0.0 && f[1][2] == 0.0);
    }

    // Test 5: Cubic and quartic terms affect energy but force still correct
    {
        std::vector<Bond> bonds = {{0, 1, 1}};
        std::vector<double> r0 = {0.0, 1.0};
        std::vector<double> k2 = {0.0, 1.0};
        std::vector<double> k3 = {0.0, 2.0};
        std::vector<double> k4 = {0.0, 3.0};
        std::vector<std::vector<double>> x = {{0.0, 0.0, 0.0}, {1.5, 0.0, 0.0}}; // r=1.5, dr=0.5
        std::vector<std::vector<double>> f = {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}};
        BondResult res = compute_quartic_bond_forces(bonds, r0, k2, k3, k4, x, f);
        double dr = 0.5;
        double energy = 1.0*dr*dr + 2.0*dr*dr*dr + 3.0*dr*dr*dr*dr; // 0.25 + 0.25 + 0.1875 = 0.6875
        assert(std::fabs(res.total_energy - 0.6875) < 1e-12);
        double de_dr = 2*1.0*dr + 3*2.0*dr*dr + 4*3.0*dr*dr*dr; // 1.0 + 1.5 + 1.5 = 4.0? Wait compute: 2*0.5=1, 3*2*0.25=1.5? Actually 3*2=6 *0.25=1.5, 4*3=12*0.125=1.5, sum=4.0
        double fbond = -4.0 / 1.5;
        double delx = 0.0 - 1.5 = -1.5;
        double fx_expected = delx * fbond; // = (-1.5) * (-4/1.5) = 4
        assert(std::fabs(f[0][0] - fx_expected) < 1e-12);
        assert(std::fabs(f[1][0] + fx_expected) < 1e-12);
    }

    return 0;
}
