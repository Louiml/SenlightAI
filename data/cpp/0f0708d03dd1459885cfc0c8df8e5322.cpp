/*
Given a list of N particles, each with position \(r\), velocity \(v\), mass \(m\), and smoothing length \(h\), write a standalone C++ function that computes the smoothed gravitational acceleration \(a\) and potential \(gpot\) for each particle using direct summation over all pairs. Use a cubic spline (M4) smoothing kernel. For each pair \((i,j)\), the acceleration contribution is \(a_i += m_j \cdot \nabla W(|r_i-r_j|, h_i, h_j)\) and potential contribution is \(gpot_i += m_j \cdot W_{pot}(|r_i-r_j|, h_i, h_j)\). The kernel uses a mean smoothing length \(h_{ij} = (h_i+h_j)/2\). Define the M4 kernel with dimensionless argument \(q = d/h_{ij}\): for \(0 \le q < 1\), \(W(q) = \frac{8}{\pi h^3} (1 - 6q^2 + 6q^3)\); for \(1 \le q < 2\), \(W(q) = \frac{8}{\pi h^3} (2 - q)^3\); otherwise \(W(q)=0\). The gravitational acceleration magnitude per pair is \(m_j \cdot \frac{1}{h_{ij}^2} \cdot \frac{dW}{dq} \cdot \frac{1}{d}\), directed along the vector from i to j. The potential uses \(\Phi(q) = \frac{1}{h_{ij}} \int W(q) dq\), but for simplicity, implement the potential as \(\Phi(q) = \frac{1}{h_{ij}} \cdot W(q)\) (or a simpler approximation like Newtonian \(\Phi = m_j/d\) for validity — but the task must be self-contained, so either is acceptable; choose Newtonian potential for simplicity if the kernel potential is too complex). Alternatively, to keep the task focused on the force calculation, you may ignore the potential and only compute the acceleration. The function should take vectors of positions, velocities, masses, and smoothing lengths, and output the acceleration vectors and potential values. Handle the case where two particles have identical positions by adding a small epsilon to the distance. Use double precision. The input size N can be up to 1000, but the algorithm is \(O(N^2)\).
*/
#include <vector>
#include <cmath>
#include <stdexcept>

// Cubic spline (M4) kernel derivative with respect to q, divided by h^3.
// Returns (1/h^3) * dW/dq. The kernel is normalized for 3D: W(q) = (8/(pi h^3)) * f(q)
// where f(q) = 1 - 6q^2 + 6q^3 for 0<=q<1, and f(q) = (2-q)^3 for 1<=q<2.
static double kernelDerivative(double q, double h) {
    if (q < 0.0 || q >= 2.0) return 0.0;
    double h3 = h * h * h;
    const double pi = 3.14159265358979323846;
    if (q < 1.0) {
        // derivative of (1 - 6q^2 + 6q^3) = -12q + 18q^2
        return (8.0 / (pi * h3)) * (-12.0 * q + 18.0 * q * q);
    } else {
        // derivative of (2-q)^3 = -3(2-q)^2
        double x = 2.0 - q;
        return (8.0 / (pi * h3)) * (-3.0 * x * x);
    }
}

// Compute smoothed gravitational accelerations for all particles using direct summation.
// Inputs: positions (N x 3), masses (N), smoothing lengths (N).
// Outputs: accelerations (N x 3) are modified in place (accumulated).
// For each pair (i,j), a_i += m_j * (1/h_ij^2) * (dW/dq)(q) * (1/d) * (r_j - r_i).
// Uses mean smoothing length h_ij = (h_i + h_j)/2.
// Handles coincident particles by adding a small epsilon to distance.
void computeSmoothedGravitationalAcceleration(
    std::vector<std::array<double,3>>& positions,
    const std::vector<double>& masses,
    const std::vector<double>& smoothingLengths,
    std::vector<std::array<double,3>>& accelerations)
{
    const size_t N = positions.size();
    if (N != masses.size() || N != smoothingLengths.size() || N != accelerations.size())
        throw std::invalid_argument("All vectors must have the same length");

    // Initialize accelerations to zero
    for (auto& a : accelerations) a = {0.0, 0.0, 0.0};

    const double eps = 1e-10; // small epsilon to avoid division by zero

    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < N; ++j) {
            if (i == j) continue;

            // Displacement vector from i to j
            double dx = positions[j][0] - positions[i][0];
            double dy = positions[j][1] - positions[i][1];
            double dz = positions[j][2] - positions[i][2];

            double distSq = dx*dx + dy*dy + dz*dz;
            double dist = std::sqrt(distSq) + eps;

            // Mean smoothing length
            double hMean = 0.5 * (smoothingLengths[i] + smoothingLengths[j]);
            double q = dist / hMean;

            // Kernel derivative factor: (1/h^2) * (dW/dq) * (1/d)
            double factor = kernelDerivative(q, hMean) / (hMean * hMean * dist) * masses[j];

            // Accumulate to particle i
            accelerations[i][0] += factor * dx;
            accelerations[i][1] += factor * dy;
            accelerations[i][2] += factor * dz;
        }
    }
}
#include <array>
#include <vector>
#include <cmath>
#include <cassert>

int main() {
    // Test 1: Two particles separated by distance 1, equal masses and h.
    std::vector<std::array<double,3>> pos = {{{0,0,0}, {1,0,0}}};
    std::vector<double> mass = {1.0, 1.0};
    std::vector<double> h = {1.0, 1.0};
    std::vector<std::array<double,3>> acc = {{0,0,0}, {0,0,0}};
    computeSmoothedGravitationalAcceleration(pos, mass, h, acc);
    // Symmetry: a0 should be positive x, a1 negative x, equal magnitude.
    assert(std::fabs(acc[0][0] + acc[1][0]) < 1e-12);
    assert(std::fabs(acc[0][1]) < 1e-12);
    assert(std::fabs(acc[0][2]) < 1e-12);
    assert(std::fabs(acc[1][0] - (-acc[0][0])) < 1e-12);

    // Test 2: Single particle should have zero acceleration.
    std::vector<std::array<double,3>> pos2 = {{{2,3,4}}};
    std::vector<double> mass2 = {1.0};
    std::vector<double> h2 = {1.0};
    std::vector<std::array<double,3>> acc2 = {{0,0,0}};
    computeSmoothedGravitationalAcceleration(pos2, mass2, h2, acc2);
    assert(acc2[0][0] == 0.0 && acc2[0][1] == 0.0 && acc2[0][2] == 0.0);

    // Test 3: Coincident particles (identical positions) should not crash.
    std::vector<std::array<double,3>> pos3 = {{{0,0,0}, {0,0,0}}};
    std::vector<double> mass3 = {1.0, 2.0};
    std::vector<double> h3 = {1.0, 1.0};
    std::vector<std::array<double,3>> acc3 = {{0,0,0}, {0,0,0}};
    computeSmoothedGravitationalAcceleration(pos3, mass3, h3, acc3);
    // Should produce finite values (not NaN)
    assert(std::isfinite(acc3[0][0]) && std::isfinite(acc3[1][0]));

    // Test 4: Three particles, verify symmetry of total momentum change (sum of mass*acc = 0).
    std::vector<std::array<double,3>> pos4 = {{{0,0,0}, {1,0,0}, {0,2,0}}};
    std::vector<double> mass4 = {1.0, 2.0, 3.0};
    std::vector<double> h4 = {0.5, 1.0, 0.7};
    std::vector<std::array<double,3>> acc4 = {{0,0,0}, {0,0,0}, {0,0,0}};
    computeSmoothedGravitationalAcceleration(pos4, mass4, h4, acc4);
    double psum_x = 0, psum_y = 0, psum_z = 0;
    for (size_t i = 0; i < 3; ++i) {
        psum_x += mass4[i] * acc4[i][0];
        psum_y += mass4[i] * acc4[i][1];
        psum_z += mass4[i] * acc4[i][2];
    }
    assert(std::fabs(psum_x) < 1e-10);
    assert(std::fabs(psum_y) < 1e-10);
    assert(std::fabs(psum_z) < 1e-10);

    return 0;
}
// The main algorithm is a direct double loop over all particle pairs, skipping self-interactions. For each pair \((i,j)\), compute the displacement vector \(dr = r_j - r_i\), its magnitude \(d = |dr|\), and the mean smoothing length \(h_{ij} = 0.5(h_i + h_j)\). The kernel argument is \(q = d/h_{ij}\). The cubic spline kernel derivative with respect to \(q\) is: for \(0 \le q < 1\), \(dW/dq = \frac{8}{\pi h^3} (-12q + 18q^2)\); for \(1 \le q < 2\), \(dW/dq = \frac{8}{\pi h^3} (-3)(2-q)^2\); otherwise zero. The acceleration contribution to particle i is \(a_i += m_j \cdot (1/h_{ij}^2) \cdot (dW/dq) \cdot (1/d) \cdot dr\). By Newton’s third law, the contribution to particle j is \(a_j -= m_j \cdot (1/h_{ij}^2) \cdot (dW/dq) \cdot (1/d) \cdot dr\), but note that the mass used is still \(m_j\) (since the force on i due to j uses \(m_j\) only; for the reverse, the force on j due to i uses \(m_i\)). Therefore, for symmetry, when computing both directions, use \(m_j\) for a_i and \(m_i\) for a_j. Alternatively, accumulate only the i contribution in a single loop and symmetrize by adding the reverse contribution explicitly. The edge case of identical positions (\(d=0\)) is handled by adding a small epsilon (e.g., 1e-10) to d before division to avoid division by zero. The potential is optional; if implemented as Newtonian, \(\Phi_i += m_j / d\), which is simpler and avoids kernel integration. Time complexity is \(O(N^2)\), space complexity is \(O(N)\) for the output arrays.
