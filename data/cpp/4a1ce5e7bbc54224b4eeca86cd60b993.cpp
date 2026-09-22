Write a C++ function named `bifurcationAndLyapunovCalculator` that, for a given system defined by a function pointer `std::vector<double>(*system)(std::vector<double>, double)` and its Jacobian `std::vector<std::vector<double>> (*jacobian)(std::vector<double>&, double)`, computes and returns the Lyapunov exponents spectrum for a single parameter value. Specifically, the function must accept the system function, Jacobian function, an initial condition vector `initialCond` of size `n`, a parameter value `rho`, a time step `tau`, a total integration time `totalTime`, and a system dimension `n`. It should simulate the system using a fixed-step Runge-Kutta 4th order method (you may implement a simple RK4 step), simultaneously evolving both the state and the variational matrix to compute the Lyapunov exponents via QR decomposition (Gram-Schmidt orthonormalization). The function should return a `std::vector<long double>` of length `n` containing the Lyapunov exponents (not the numbers). The exponents should be computed as the average logarithmic growth rates over the total integration time. Handle edge cases such as zero time step, zero total time, or empty initial condition by returning an empty vector.

The solution requires implementing a numerical integrator for a system of ODEs and tracking the evolution of infinitesimal perturbations to compute Lyapunov exponents. The main steps are: (1) Initialize the state vector `x = initialCond` and a matrix `W` representing the tangent space basis vectors, initially the identity matrix of size `n×n`. (2) For each time step from 0 to totalTime with step tau, perform one RK4 step for the system to get the new state. Simultaneously, compute the Jacobian at the current state (the Jacobian function provided). Since the Jacobian returns a matrix, we need to evolve the variational equations: `dW/dt = J(x) * W`. For simplicity, use a first-order Euler step for the variational matrix (acceptable for demonstration), or implement a RK4 for both together. After each step (or periodically), perform Gram-Schmidt orthonormalization on the columns of `W`, accumulate `log(norm_of_each_column)` before normalization, then normalize. At the end, divide each accumulated sum by `totalTime` to get the Lyapunov exponents. Time complexity is `O(numSteps * n^3)` due to matrix multiplication and Gram-Schmidt. Space complexity is `O(n^2)` for matrices. Edge cases: if tau <= 0 or totalTime <= 0 or initialCond empty, return empty vector. Also ensure the RK4 step function handles the system correctly.

#include <vector>
#include <cmath>
#include <numeric>
#include <algorithm>

// Helper to compute matrix-vector product: returns J * v where J is n x n and v is n
std::vector<double> matVecMultiply(const std::vector<std::vector<double>>& mat, const std::vector<double>& vec) {
    size_t n = mat.size();
    std::vector<double> result(n, 0.0);
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            result[i] += mat[i][j] * vec[j];
        }
    }
    return result;
}

// One RK4 step for the system dx/dt = f(x, rho)
std::vector<double> rk4Step(const std::vector<double>& x, double rho, double tau,
                            std::vector<double>(*system)(std::vector<double>, double)) {
    size_t n = x.size();
    std::vector<double> k1 = system(x, rho);
    std::vector<double> x2(n);
    for (size_t i = 0; i < n; ++i) x2[i] = x[i] + 0.5 * tau * k1[i];
    std::vector<double> k2 = system(x2, rho);
    std::vector<double> x3(n);
    for (size_t i = 0; i < n; ++i) x3[i] = x[i] + 0.5 * tau * k2[i];
    std::vector<double> k3 = system(x3, rho);
    std::vector<double> x4(n);
    for (size_t i = 0; i < n; ++i) x4[i] = x[i] + tau * k3[i];
    std::vector<double> k4 = system(x4, rho);

    std::vector<double> next(n);
    for (size_t i = 0; i < n; ++i) {
        next[i] = x[i] + (tau / 6.0) * (k1[i] + 2.0 * k2[i] + 2.0 * k3[i] + k4[i]);
    }
    return next;
}

// Compute Lyapunov exponents for a system using Gram-Schmidt orthonormalization
std::vector<long double> bifurcationAndLyapunovCalculator(
    std::vector<double>(*system)(std::vector<double>, double),
    std::vector<std::vector<double>> (*jacobian)(std::vector<double>&, double),
    std::vector<double> initialCond,
    double rho,
    double tau,
    double totalTime,
    int n) {

    // Edge case checks
    if (tau <= 0.0 || totalTime <= 0.0 || initialCond.size() != static_cast<size_t>(n) || n <= 0) {
        return {};
    }

    // Initialize state
    std::vector<double> x = initialCond;

    // Initialize tangent space matrix W as identity
    std::vector<std::vector<double>> W(n, std::vector<double>(n, 0.0));
    for (int i = 0; i < n; ++i) W[i][i] = 1.0;

    // Accumulated logs of vector norms
    std::vector<long double> sumLogs(n, 0.0);

    int numSteps = static_cast<int>(totalTime / tau);
    for (int step = 0; step < numSteps; ++step) {
        // Step the system forward
        x = rk4Step(x, rho, tau, system);

        // Get Jacobian at new state (or old; for simplicity use old state as per snippet style)
        std::vector<std::vector<double>> J = jacobian(x, rho);

        // Euler step for variational equation: W_new = W_old + tau * J * W_old
        // This is a simplified but functional approach; for accuracy use RK4 here too.
        std::vector<std::vector<double>> W_new(n, std::vector<double>(n, 0.0));
        for (int i = 0; i < n; ++i) {
            std::vector<double> JWi = matVecMultiply(J, W[i]);
            for (int j = 0; j < n; ++j) {
                W_new[i][j] = W[i][j] + tau * JWi[j];
            }
        }
        W = W_new;

        // Gram-Schmidt orthonormalization on columns of W
        // Treat columns as vectors (each column j has elements W[i][j] for i=0..n-1)
        for (int j = 0; j < n; ++j) {
            // Orthogonalize against previous columns
            for (int k = 0; k < j; ++k) {
                // Projection coefficient
                long double dot = 0.0;
                for (int i = 0; i < n; ++i) {
                    dot += static_cast<long double>(W[i][j]) * static_cast<long double>(W[i][k]);
                }
                // Subtract projection
                for (int i = 0; i < n; ++i) {
                    W[i][j] -= static_cast<double>(dot * static_cast<long double>(W[i][k]));
                }
            }
            // Compute norm
            long double norm = 0.0;
            for (int i = 0; i < n; ++i) {
                norm += static_cast<long double>(W[i][j]) * static_cast<long double>(W[i][j]);
            }
            norm = sqrt(norm);
            if (norm > 1e-12) { // avoid division by zero
                sumLogs[j] += log(norm);
                // Normalize
                for (int i = 0; i < n; ++i) {
                    W[i][j] /= static_cast<double>(norm);
                }
            }
        }
    }

    // Average over total time
    std::vector<long double> exponents(n, 0.0);
    for (int i = 0; i < n; ++i) {
        exponents[i] = sumLogs[i] / static_cast<long double>(totalTime);
    }
    return exponents;
}

#include <cassert>
#include <cmath>
#include <iostream>

// Example system: Lorenz system (dimension 3)
std::vector<double> lorenz(std::vector<double> state, double rho) {
    double sigma = 10.0;
    double beta = 8.0 / 3.0;
    double x = state[0], y = state[1], z = state[2];
    return {sigma * (y - x), x * (rho - z) - y, x * y - beta * z};
}

std::vector<std::vector<double>> lorenzJacobian(std::vector<double>& state, double rho) {
    double sigma = 10.0;
    double beta = 8.0 / 3.0;
    double x = state[0], y = state[1], z = state[2];
    return {
        {-sigma, sigma, 0.0},
        {rho - z, -1.0, -x},
        {y, x, -beta}
    };
}

// Simple 1D linear system for testing: dx/dt = -x (decay) has exponent -1
std::vector<double> decay(std::vector<double> state, double param) {
    return {-state[0]};
}
std::vector<std::vector<double>> decayJacobian(std::vector<double>& state, double param) {
    return {{-1.0}};
}

int main() {
    // Test 1D linear decay: exponent should be -1 (approximately)
    auto exp1 = bifurcationAndLyapunovCalculator(decay, decayJacobian, {1.0}, 0.0, 0.01, 10.0, 1);
    assert(exp1.size() == 1);
    assert(std::fabs(exp1[0] - (-1.0)) < 0.1); // Euler approximation error

    // Test 3D Lorenz system at rho=28: sum of exponents ~ -13.667 (approx), each around (0.9, 0.0, -14.5)
    std::vector<double> initial = {1.0, 1.0, 1.0};
    auto exp3 = bifurcationAndLyapunovCalculator(lorenz, lorenzJacobian, initial, 28.0, 0.01, 100.0, 3);
    assert(exp3.size() == 3);
    // Sum should be close to -(sigma+1+beta) = -13.667
    long double sum = exp3[0] + exp3[1] + exp3[2];
    assert(std::fabs(sum + 13.667) < 0.5); // rough tolerance
    // First exponent typically positive for chaotic system
    assert(exp3[0] > 0.1);

    // Edge cases
    assert(bifurcationAndLyapunovCalculator(lorenz, lorenzJacobian, initial, 28.0, 0.0, 100.0, 3).empty());
    assert(bifurcationAndLyapunovCalculator(lorenz, lorenzJacobian, initial, 28.0, 0.01, 0.0, 3).empty());
    assert(bifurcationAndLyapunovCalculator(lorenz, lorenzJacobian, initial, 28.0, 0.01, 100.0, -1).empty());

    std::cout << "All tests passed.\n";
    return 0;
}
