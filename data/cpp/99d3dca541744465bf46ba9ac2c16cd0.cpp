// Write a C++ function that implements a discrete Hamiltonian lattice system with nearest-neighbor interactions. The system evolves according to the equations of motion: `dp_i/dt = -V_i * q_i^3 - beta*(q_i - q_{i-1})^3 + beta*(q_{i+1} - q_i)^3`, where `q` is the displacement vector, `p` is the momentum vector, `V` is a periodic potential vector, and `beta` is a coupling constant. The function should compute the time derivative `dpdt` of the momentum for given position coordinates `q`, taking into account periodic boundary conditions (i.e., indices wrap around). The function must be callable repeatedly with different sizes of `q` and `dpdt`, and it must not modify the input `q` or the potential vector. The potential vector is provided as a reference, and you may assume it is at least as large as the size of `q`. The function signature should be: `void compute_dpdt(const std::vector<double>& q, const std::vector<double>& V, double beta, std::vector<double>& dpdt)`.
#include <cassert>
#include <vector>
#include <cmath>

// Forward declaration of the function under test
void compute_dpdt(const std::vector<double>& q, const std::vector<double>& V,
                  double beta, std::vector<double>& dpdt);

int main() {
    // Test 1: N=3, simple integer values, beta=1
    {
        std::vector<double> q = {1.0, 2.0, 3.0};
        std::vector<double> V = {1.0, 1.0, 1.0};
        std::vector<double> dpdt(3, 0.0);
        compute_dpdt(q, V, 1.0, dpdt);
        // Manually compute expected values:
        // For i=0: -1*1^3 - 1*(1-3)^3 + 1*(2-1)^3 = -1 -(-8) + 1 = 8
        // For i=1: -1*8 - 1*(2-1)^3 + 1*(3-2)^3 = -8 -1 +1 = -8
        // For i=2: -1*27 - 1*(3-2)^3 + 1*(1-3)^3 = -27 -1 +(-8) = -36
        assert(std::abs(dpdt[0] - 8.0) < 1e-12);
        assert(std::abs(dpdt[1] - (-8.0)) < 1e-12);
        assert(std::abs(dpdt[2] - (-36.0)) < 1e-12);
    }

    // Test 2: N=1, only one site, all differences are zero
    {
        std::vector<double> q = {2.0};
        std::vector<double> V = {0.5};
        std::vector<double> dpdt(1, 0.0);
        compute_dpdt(q, V, 2.0, dpdt);
        // Only on-site term: -0.5 * 8 = -4
        assert(std::abs(dpdt[0] - (-4.0)) < 1e-12);
    }

    // Test 3: N=2, check periodic wrap-around
    {
        std::vector<double> q = {1.0, 2.0};
        std::vector<double> V = {1.0, 2.0};
        std::vector<double> dpdt(2, 0.0);
        compute_dpdt(q, V, 0.5, dpdt);
        // For i=0: -1*1 - 0.5*(1-2)^3 + 0.5*(2-1)^3 = -1 -0.5*(-1) +0.5*(1) = -1 +0.5 +0.5 = 0
        // For i=1: -2*8 - 0.5*(2-1)^3 + 0.5*(1-2)^3 = -16 -0.5 +0.5*(-1) = -16 -0.5 -0.5 = -17
        assert(std::abs(dpdt[0] - 0.0) < 1e-12);
        assert(std::abs(dpdt[1] - (-17.0)) < 1e-12);
    }

    // Test 4: beta=0, only on-site terms remain
    {
        std::vector<double> q = {1.0, -2.0, 3.0};
        std::vector<double> V = {2.0, 1.0, 4.0};
        std::vector<double> dpdt(3, 0.0);
        compute_dpdt(q, V, 0.0, dpdt);
        // dpdt = -V_i * q_i^3
        assert(std::abs(dpdt[0] - (-2.0 * 1.0)) < 1e-12);
        assert(std::abs(dpdt[1] - (-1.0 * (-8.0))) < 1e-12); // -1 * -8 = 8
        assert(std::abs(dpdt[2] - (-4.0 * 27.0)) < 1e-12);
    }

    // Test 5: N=4, larger values to ensure no overflow issues with doubles
    {
        std::vector<double> q = {0.5, 1.0, 1.5, 2.0};
        std::vector<double> V = {0.1, 0.2, 0.3, 0.4};
        std::vector<double> dpdt(4, 0.0);
        compute_dpdt(q, V, 1.0, dpdt);
        // Spot-check i=0: -0.1*0.125 - 1*(0.5-2.0)^3 + 1*(1.0-0.5)^3
        // = -0.0125 - (-3.375) + 0.125 = 3.4875
        assert(std::abs(dpdt[0] - 3.4875) < 1e-12);
        // Spot-check i=1: -0.2*1.0 - 1*(1.0-0.5)^3 + 1*(1.5-1.0)^3
        // = -0.2 -0.125 +0.125 = -0.2
        assert(std::abs(dpdt[1] - (-0.2)) < 1e-12);
    }

    return 0;
}
#include <vector>
#include <cmath>

// Compute the time derivative of momentum for a discrete Hamiltonian lattice system.
// dp_i/dt = -V_i * q_i^3 - beta*(q_i - q_{i-1})^3 + beta*(q_{i+1} - q_i)^3
// The system uses periodic boundary conditions: indices wrap around.
// The potential vector V must have at least as many elements as q.
// The output vector dpdt must already have the same size as q (it will be overwritten).
void compute_dpdt(const std::vector<double>& q, const std::vector<double>& V,
                  double beta, std::vector<double>& dpdt) {
    const std::size_t N = q.size();
    if (N == 0) return; // no sites, nothing to compute

    // Precompute initial difference for i=0: q[0] - q[N-1]
    double diff = q[0] - q[N-1];
    
    for (std::size_t i = 0; i < N; ++i) {
        // On-site potential term: -V[i] * q[i]^3
        const double q_cubed = q[i] * q[i] * q[i];
        dpdt[i] = - V[i] * q_cubed;
        
        // Left coupling term: -beta * (q[i] - q[i-1])^3
        // Here diff currently holds (q[i] - q[i-1]) because we updated it at the end of the previous iteration
        dpdt[i] -= beta * diff * diff * diff;
        
        // Compute the right difference for this site: q[(i+1)%N] - q[i]
        diff = q[(i + 1) % N] - q[i];
        
        // Right coupling term: +beta * (q[i+1] - q[i])^3
        dpdt[i] += beta * diff * diff * diff;
    }
}
// The core algorithm computes the derivative for each lattice site `i` by evaluating three contributions: (1) the on-site potential term `-V_i * q_i^3`, (2) the left-neighbor coupling `-beta * (q_i - q_{i-1})^3`, and (3) the right-neighbor coupling `+beta * (q_{i+1} - q_i)^3`. Periodic boundary conditions are handled by using modular indexing: `(i+N-1) % N` for the left neighbor and `(i+1) % N` for the right neighbor. To avoid redundant computation, the difference `diff = q[0] - q[N-1]` is computed once before the loop, representing `(q_i - q_{i-1})` for `i=0`. Then in each iteration, the current `diff` is used for the left coupling, and the next difference `diff = q[(i+1)%N] - q[i]` is computed for the right coupling of the current site and stored for the next iteration. The output vector `dpdt` must be pre-sized to match `q` (or resized inside if needed, but the spec implies it is correctly sized). Edge cases include `N=1` where all differences are zero (since the only neighbor is itself), and potential values may be arbitrary doubles (including zeros or negatives). Time complexity is O(N) per call, and space complexity is O(1) additional beyond the output vector.
