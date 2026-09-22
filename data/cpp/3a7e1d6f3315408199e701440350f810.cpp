Write a standalone C++ function `applyElectronLangevinThermostat` that implements the core electron-force modification logic from the electron force field (eFF) Langevin thermostat. The function takes as input: a vector of 3D velocities `v` (one per particle), a vector of 3D forces `f` (same size), a vector of electron radial velocities `ervel` (size equal to number of particles, but only particles with `abs(spin)==1` are electrons), a vector of electron radial forces `erforce` (same size as `ervel`), a vector of integer spins `spin` (1 for electron, 0 or 2 for nucleus, but only `abs(spin)==1` matters), a scalar temperature `T` (in Kelvin), a scalar damping coefficient `gamma` (in inverse time units), and a constant `mefactor` (dimensionless, typically 0.75 for 3D). The function must update `f` and `erforce` in-place by adding deterministic damping forces and random stochastic forces. For each particle indexed `i`, the force contribution is: `f[i][j] += gamma1 * v[i][j] + gamma2 * (rand_uniform() - 0.5)` for each Cartesian component `j` (0,1,2), where `gamma1 = gamma * gfactor3` and `gamma2 = sqrt(2 * gamma * T) * gfactor3 * sqrt(4.0 * mefactor / dimension)` (assume dimension = 2 for simplicity, so mefactor = 0.5 for 2D, but mefactor is passed in). For electrons only (abs(spin[i])==1), additionally update `erforce[i] += mefactor * gamma1 * ervel[i] + sqrt(mefactor) * gamma2 * (rand_uniform() - 0.5)`. The factor `gfactor3` is a thermal partitioning factor computed as `(dof + nelectrons) / dofnuclei`, where `dof = dimension * particles - dimension` (the −1 accounts for center-of-mass removal), `nelectrons` is the total number of electrons in the entire particle list, and `dofnuclei = dof - dimension * nelectrons`. Use `nelectrons` counted from the input spins (no parallel reduction needed; assume single-threaded). Implement a simple pseudorandom generator (e.g., a linear congruential generator) inside the function, seeded with a fixed constant (e.g., 12345) for reproducibility. The function must be `const`-correct with respect to inputs that should not be modified (velocities, spins) and must return `void`. Assume all vectors are of equal length `N` (for `v`, `f`), and `ervel`, `erforce`, `spin` are also of length `N` (even for non-electrons, the values are unused). Ensure gamma1 and gamma2 are computed per particle but are the same for all particles because the type-dependent part is omitted (assume a single type). Provide the function signature exactly as: `void applyElectronLangevinThermostat(std::vector<std::array<double,3>>& v, std::vector<std::array<double,3>>& f, std::vector<double>& ervel, std::vector<double>& erforce, const std::vector<int>& spin, double T, double gamma, double mefactor)`. Use `std::array<double,3>` for 3D vectors. Do not include any `main` function in the solution.

// The algorithm iterates through every particle index `i` from 0 to N-1. First, count the number of electrons `nelectrons` by checking `abs(spin[i]) == 1`. Then compute the degrees of freedom as `dof = 2 * N - 2` (where dimension=2, minus one for center-of-mass), and `dofnuclei = dof - 2 * nelectrons`. If `dofnuclei` is zero (all electrons), avoid division by zero by setting `gfactor3 = 1.0` (or handle gracefully). Otherwise `gfactor3 = (dof + nelectrons) / dofnuclei`. For each particle, compute `gamma1 = gamma * gfactor3` and `gamma2 = sqrt(2.0 * gamma * T) * gfactor3 * sqrt(4.0 * mefactor / 2.0)` (since dimension=2, 4*mefactor/dimension = 2*mefactor). Use a linear congruential generator (LCG) with seed 12345 and constants a=1103515245, c=12345, m=2^31, producing values in [0,1) as `rand_uniform() = (state / (double)INT32_MAX)`. For each Cartesian component j (0,1,2), add `gamma1 * v[i][j] + gamma2 * (rand_uniform() - 0.5)` to `f[i][j]`. If `abs(spin[i]) == 1`, also update `erforce[i] += mefactor * gamma1 * ervel[i] + sqrt(mefactor) * gamma2 * (rand_uniform() - 0.5)`. Edge cases: empty vectors (N=0) should return immediately without errors; if `dofnuclei <= 0`, set `gfactor3` to 1 to avoid division by zero; negative temperature or negative gamma should be treated as zero to avoid sqrt of negative (clamp to 0). Time complexity is O(N) because each particle is processed once. Space complexity is O(1) additional, aside from the input vectors, since the LCG state is a local variable. The function modifies `f` and `erforce` in-place, and respects `const` for `v` and `spin`.

#include <array>
#include <cmath>
#include <cstdint>
#include <vector>

// Apply a Langevin thermostat to both Cartesian and electron radial degrees of freedom.
void applyElectronLangevinThermostat(
    std::vector<std::array<double,3>>& v,
    std::vector<std::array<double,3>>& f,
    std::vector<double>& ervel,
    std::vector<double>& erforce,
    const std::vector<int>& spin,
    double T,
    double gamma,
    double mefactor) {

    const std::size_t N = v.size();
    if (N == 0) return;

    // Count electrons
    int nelectrons = 0;
    for (std::size_t i = 0; i < N; ++i) {
        if (std::abs(spin[i]) == 1) ++nelectrons;
    }

    // Degrees of freedom: dimension=2, remove center-of-mass (2 DOF), electrons have 2 DOF each
    int particles = static_cast<int>(N);
    int dof = 2 * particles - 2; // dimension=2, minus CM
    int dofnuclei = dof - 2 * nelectrons;
    double gfactor3 = 1.0;
    if (dofnuclei > 0) {
        gfactor3 = static_cast<double>(dof + nelectrons) / static_cast<double>(dofnuclei);
    }

    // Deterministic and stochastic coefficients based on dimension=2
    double gamma1 = gamma * gfactor3;
    // sqrt(2 * gamma * T) * sqrt(4 * mefactor / dimension) with dimension=2
    double sqrt_term = std::sqrt(std::max(0.0, 2.0 * gamma * T));
    double gamma2 = sqrt_term * gfactor3 * std::sqrt(4.0 * mefactor / 2.0);

    // Reproducible LCG for uniform [0,1)
    uint32_t state = 12345;
    auto rand_unit = [&state]() {
        state = state * 1103515245u + 12345u;
        return static_cast<double>((state & 0x7FFFFFFF)) / 2147483648.0;
    };

    const double sqrt_mefactor = std::sqrt(mefactor);

    for (std::size_t i = 0; i < N; ++i) {
        // Cartesian degrees of freedom
        for (int j = 0; j < 3; ++j) {
            f[i][j] += gamma1 * v[i][j] + gamma2 * (rand_unit() - 0.5);
        }
        // Electron radial degree of freedom
        if (std::abs(spin[i]) == 1) {
            erforce[i] += mefactor * gamma1 * ervel[i] + sqrt_mefactor * gamma2 * (rand_unit() - 0.5);
        }
    }
}

#include <cassert>
#include <cmath>
#include <vector>
#include <array>

// The solution function is assumed to be defined above; here we redeclare for linking.
void applyElectronLangevinThermostat(
    std::vector<std::array<double,3>>& v,
    std::vector<std::array<double,3>>& f,
    std::vector<double>& ervel,
    std::vector<double>& erforce,
    const std::vector<int>& spin,
    double T,
    double gamma,
    double mefactor);

int main() {
    // Test 1: Single nucleus (no electrons), zero temperature and gamma => no force change
    {
        std::vector<std::array<double,3>> v = {{{1.0, 2.0, 3.0}}};
        std::vector<std::array<double,3>> f = {{{0.0, 0.0, 0.0}}};
        std::vector<double> ervel = {0.0};
        std::vector<double> erforce = {0.0};
        std::vector<int> spin = {0};
        applyElectronLangevinThermostat(v, f, ervel, erforce, spin, 0.0, 0.0, 0.5);
        for (int j = 0; j < 3; ++j) {
            assert(std::abs(f[0][j]) < 1e-12);
        }
        assert(std::abs(erforce[0]) < 1e-12);
    }

    // Test 2: One nucleus and one electron with zero temperature and gamma => only deterministic damping zero
    {
        std::vector<std::array<double,3>> v = {{{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}}};
        std::vector<std::array<double,3>> f = {{{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}}};
        std::vector<double> ervel = {0.0, 1.0};
        std::vector<double> erforce = {0.0, 0.0};
        std::vector<int> spin = {0, 1};
        applyElectronLangevinThermostat(v, f, ervel, erforce, spin, 0.0, 0.5, 0.5);
        // gamma1 = gamma * gfactor3, gfactor3 = (dof+nelectrons)/dofnuclei
        // dof = 2*2-2=2, nelectrons=1, dofnuclei = 2-2=0 => gfactor3=1 (fallback)
        // gamma1 = 0.5, gamma2=0
        // f[0] gets -0.5 (velocity -1), f[1] gets +0.5 (velocity +1)
        assert(std::abs(f[0][0] - (-0.5)) < 1e-12);
        assert(std::abs(f[0][1]) < 1e-12);
        assert(std::abs(f[0][2]) < 1e-12);
        assert(std::abs(f[1][0]) < 1e-12);
        assert(std::abs(f[1][1] - 0.5) < 1e-12);
        assert(std::abs(f[1][2]) < 1e-12);
        // Electron radial: erforce[1] = mefactor*gamma1*ervel = 0.5*0.5*1 = 0.25
        assert(std::abs(erforce[0]) < 1e-12);
        assert(std::abs(erforce[1] - 0.25) < 1e-12);
    }

    // Test 3: With temperature, stochastic forces should change values deterministically given seed
    {
        std::vector<std::array<double,3>> v = {{{0.0, 0.0, 0.0}}};
        std::vector<std::array<double,3>> f = {{{0.0, 0.0, 0.0}}};
        std::vector<double> ervel = {0.0};
        std::vector<double> erforce = {0.0};
        std::vector<int> spin = {1};
        double T = 100.0, gamma = 1.0, mef = 0.5;
        applyElectronLangevinThermostat(v, f, ervel, erforce, spin, T, gamma, mef);
        // We expect forces non-zero due to random numbers; test that they are finite and deterministic
        double f0_x = f[0][0];
        assert(std::isfinite(f0_x) && f0_x != 0.0);
        // Call again on fresh vectors to ensure deterministic (same seed)
        std::vector<std::array<double,3>> v2 = {{{0.0, 0.0, 0.0}}};
        std::vector<std::array<double,3>> f2 = {{{0.0, 0.0, 0.0}}};
        std::vector<double> ervel2 = {0.0};
        std::vector<double> erforce2 = {0.0};
        static std::vector<int> spin2 = {1};
        applyElectronLangevinThermostat(v2, f2, ervel2, erforce2, spin2, T, gamma, mef);
        assert(std::abs(f2[0][0] - f0_x) < 1e-12);
        assert(std::abs(f2[0][1] - f[0][1]) < 1e-12);
        assert(std::abs(erforce2[0] - erforce[0]) < 1e-12);
    }

    // Test 4: Empty input does nothing
    {
        std::vector<std::array<double,3>> v;
        std::vector<std::array<double,3>> f;
        std::vector<double> ervel;
        std::vector<double> erforce;
        std::vector<int> spin;
        applyElectronLangevinThermostat(v, f, ervel, erforce, spin, 1.0, 1.0, 0.5);
        assert(v.empty() && f.empty() && ervel.empty() && erforce.empty());
    }

    // Test 5: Negative temperature is clamped to zero (no random, no damping)
    {
        std::vector<std::array<double,3>> v = {{{2.0, 0.0, 0.0}}};
        std::vector<std::array<double,3>> f = {{{0.0, 0.0, 0.0}}};
        std::vector<double> ervel = {0.0};
        std::vector<double> erforce = {0.0};
        std::vector<int> spin = {1};
        applyElectronLangevinThermostat(v, f, ervel, erforce, spin, -10.0, 2.0, 0.5);
        // gamma2=0 due to sqrt(0), gamma1=2.0, gfactor3=1 (single electron => dofnuclei=0 => fallback)
        assert(std::abs(f[0][0] - (-4.0)) < 1e-12); // 2 * (-2) = -4 (gamma1=2, v=2, sign negative)
        assert(std::abs(f[0][1]) < 1e-12);
        assert(std::abs(erforce[0] + 2.0) < 1e-12); // mefactor*gamma1*ervel = 0.5*2*0 =0, plus random 0, so wait: ervel=0 => 0
        // Actually ervel=0 so erforce stays 0; but we assert it's 0
        assert(std::abs(erforce[0]) < 1e-12);
    }

    return 0;
}
