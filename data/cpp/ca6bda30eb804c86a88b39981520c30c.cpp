Write a standalone C++ function `simulateKuramoto` that performs numerical integration of the Kuramoto model for N coupled oscillators using the explicit Euler method. The function must accept the number of oscillators N, a vector of initial phases x (size N), a time step dt, the coupling constant ka, an array of natural frequencies w (size N), and a 2D output matrix `x_vec` (size numSteps x N) plus a vector `times` (size numSteps). It must fill `times[0]=0.0` and `x_vec[0]` with the initial phases, then advance the system for numSteps-1 additional steps using the rule: `x[i] = x[i] + dt * (w[i] + ka * sum_{j=0}^{N-1} sin(x[j] - x[i]))`. No OpenMP directives are allowed; use plain sequential loops. The function must be self-contained, include necessary headers, and not rely on any external `functions.h`. Edge cases include N=0 (do nothing) and dt=0 (no change).

#include <cassert>
#include <cmath>
#include <vector>

// Declaration of the function under test
void simulateKuramoto(const int N, std::vector<double>& x, const double dt,
                      const double ka, const std::vector<double>& w,
                      std::vector<std::vector<double>>& x_vec,
                      std::vector<double>& times);

int main() {
    // Test 1: N=2, no coupling, frequencies 0 and 1, dt=0.1, 2 steps
    {
        std::vector<double> x = {0.0, 0.0};
        std::vector<double> w = {0.0, 1.0};
        double dt = 0.1;
        double ka = 0.0;
        std::vector<std::vector<double>> x_vec(3, std::vector<double>(2));
        std::vector<double> times(3);
        simulateKuramoto(2, x, dt, ka, w, x_vec, times);
        assert(std::fabs(times[0] - 0.0) < 1e-12);
        assert(std::fabs(times[1] - 0.1) < 1e-12);
        assert(std::fabs(times[2] - 0.2) < 1e-12);
        assert(std::fabs(x_vec[0][0] - 0.0) < 1e-12);
        assert(std::fabs(x_vec[0][1] - 0.0) < 1e-12);
        assert(std::fabs(x_vec[1][0] - 0.0) < 1e-12);
        assert(std::fabs(x_vec[1][1] - 0.1) < 1e-12);
        assert(std::fabs(x_vec[2][0] - 0.0) < 1e-12);
        assert(std::fabs(x_vec[2][1] - 0.2) < 1e-12);
    }

    // Test 2: N=1, any values, should follow natural frequency
    {
        std::vector<double> x = {1.0};
        std::vector<double> w = {2.0};
        double dt = 0.5;
        double ka = 10.0; // coupling irrelevant for N=1
        std::vector<std::vector<double>> x_vec(2, std::vector<double>(1));
        std::vector<double> times(2);
        simulateKuramoto(1, x, dt, ka, w, x_vec, times);
        assert(std::fabs(x_vec[0][0] - 1.0) < 1e-12);
        assert(std::fabs(x_vec[1][0] - (1.0 + 2.0*0.5)) < 1e-12);
        assert(std::fabs(x[0] - (1.0 + 1.0)) < 1e-12);
    }

    // Test 3: N=2 with strong coupling, check symmetry (both should have same derivative if w=0 and x equal)
    {
        std::vector<double> x = {0.0, 0.0};
        std::vector<double> w = {0.0, 0.0};
        double dt = 0.1;
        double ka = 1.0;
        std::vector<std::vector<double>> x_vec(2, std::vector<double>(2));
        std::vector<double> times(2);
        simulateKuramoto(2, x, dt, ka, w, x_vec, times);
        // Since both initial phases equal, sin(0)=0, so no change
        assert(std::fabs(x_vec[1][0] - 0.0) < 1e-12);
        assert(std::fabs(x_vec[1][1] - 0.0) < 1e-12);
        assert(std::fabs(x[0] - 0.0) < 1e-12);
        assert(std::fabs(x[1] - 0.0) < 1e-12);
    }

    // Test 4: N=0 should not crash
    {
        std::vector<double> x;
        std::vector<double> w;
        double dt = 0.1;
        double ka = 1.0;
        std::vector<std::vector<double>> x_vec(2); // empty rows
        std::vector<double> times(2);
        simulateKuramoto(0, x, dt, ka, w, x_vec, times);
        // No assertions needed; just ensure no exception
    }

    // Test 5: N=2, check known result manually with dt=1.0, w=[0,0], ka=1, phases [0, pi]
    {
        std::vector<double> x = {0.0, M_PI};
        std::vector<double> w = {0.0, 0.0};
        double dt = 1.0;
        double ka = 1.0;
        std::vector<std::vector<double>> x_vec(2, std::vector<double>(2));
        std::vector<double> times(2);
        simulateKuramoto(2, x, dt, ka, w, x_vec, times);
        // For i=0: sum = sin(pi-0)=0, derivative =0, so x[0] stays 0
        // For i=1: sum = sin(0-pi)= -sin(pi)=0, derivative=0, so x[1] stays pi
        assert(std::fabs(x_vec[1][0] - 0.0) < 1e-12);
        assert(std::fabs(x_vec[1][1] - M_PI) < 1e-12);
    }

    return 0;
}

#include <vector>
#include <cmath>

/**
 * Integrate the Kuramoto model using the explicit Euler method.
 * @param N Number of oscillators.
 * @param x Initial phases (size N), modified in place to final phases.
 * @param dt Time step.
 * @param ka Coupling constant.
 * @param w Natural frequencies (size N).
 * @param x_vec Output matrix (numSteps x N), each row is phases at a time.
 * @param times Output vector (size numSteps), each time value.
 */
void simulateKuramoto(const int N, std::vector<double>& x, const double dt,
                      const double ka, const std::vector<double>& w,
                      std::vector<std::vector<double>>& x_vec,
                      std::vector<double>& times) {
    const int nstep = static_cast<int>(times.size());
    if (N <= 0 || nstep <= 0) return;

    // Initialize first row and time
    times[0] = 0.0;
    for (int i = 0; i < N; ++i) {
        x_vec[0][i] = x[i];
    }

    std::vector<double> f(N, 0.0);
    for (int k = 1; k < nstep; ++k) {
        // Compute derivative for all i
        for (int i = 0; i < N; ++i) {
            double sum = 0.0;
            for (int j = 0; j < N; ++j) {
                sum += std::sin(x[j] - x[i]);
            }
            f[i] = w[i] + ka * sum;
        }
        // Euler update
        times[k] = k * dt;
        for (int i = 0; i < N; ++i) {
            x[i] += dt * f[i];
            x_vec[k][i] = x[i];
        }
    }
}

// The solution implements the explicit Euler update for a system of coupled first-order ODEs. At each time step, we first compute the derivative `f[i] = w[i] + ka * sum_{j} sin(x[j] - x[i])` for all i, then update each phase `x[i] = x[i] + dt * f[i]`. For numerical stability with the Euler method, the time step must be small enough relative to the maximum coupling strength; however, the task does not require stability checks. The main algorithm is O(N^2) per time step due to the double loop in the derivative computation, and total complexity is O(numSteps * N^2) with O(N) auxiliary space for the temporary derivative vector. Edge cases: N=0 should return immediately without modifying output; dt=0 should copy initial conditions and fill times with i*dt (which stays 0). The output matrix `x_vec` is assumed to be pre-sized correctly: numSteps rows (indexed by time) and N columns.
