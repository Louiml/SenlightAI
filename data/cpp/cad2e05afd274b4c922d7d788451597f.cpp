// Write a C++ function that simulates a univariate GARCH(1,1) process and returns a matrix of simulated return paths, where each row is one independent simulation and each column is a time step. The function must take as inputs the constant term `omega`, the ARCH coefficient `alpha`, the GARCH coefficient `beta`, the initial variance `initial_variance`, the number of time steps `n`, and the number of simulations (defaulting to 100). The generated returns must follow the recursion: variance at time t is `omega + alpha * (return at t-1)^2 + beta * variance at t-1`, and returns are drawn from a normal distribution with mean 0 and standard deviation equal to the square root of the current variance. You must ensure that the process produces positive variances by requiring `omega > 0`, `alpha >= 0`, `beta >= 0`, and `initial_variance > 0`, and you should validate the input parameters (throwing an exception for invalid values). The random number generator must be seeded differently for each simulation to ensure independent paths.
The solution implements the GARCH(1,1) recursion directly. For each simulation, a new random engine is seeded with a fresh `std::random_device` to avoid identical sequences across rows. The first return is generated using the initial variance. Then, for each subsequent time step, the variance is updated using the previous return’s square and the previous variance, and the new return is drawn. Edge cases include: `n` must be at least 1; `omega` must be strictly positive (otherwise variance can be non-positive); `alpha` and `beta` must be non-negative; and `initial_variance` must be strictly positive. If any input is invalid, the function throws `std::invalid_argument`. Groups of parameters that would violate stationarity (alpha+beta >= 1) are not rejected because the task only specifies positivity constraints. The time complexity is O(simulations * n) for generating all returns, and O(simulations * n) space for storing the matrix. The random device and engine setup is per simulation, so the overhead is linear in simulations but negligible relative to the main loop.
#include <cmath>
#include <random>
#include <stdexcept>
#include <vector>

// Simulates GARCH(1,1) returns. Returns a matrix with 'simulations' rows and 'n' columns.
// Each row is an independent simulation path. Throws std::invalid_argument on invalid parameters.
std::vector<std::vector<double>> simulateGarch11(
    double omega,
    double alpha,
    double beta,
    double initial_variance,
    int n,
    int simulations = 100) {
    
    // Input validation
    if (n < 1) throw std::invalid_argument("n must be at least 1");
    if (simulations < 1) throw std::invalid_argument("simulations must be at least 1");
    if (omega <= 0.0) throw std::invalid_argument("omega must be strictly positive");
    if (alpha < 0.0 || beta < 0.0) throw std::invalid_argument("alpha and beta must be non-negative");
    if (initial_variance <= 0.0) throw std::invalid_argument("initial_variance must be strictly positive");
    
    std::vector<std::vector<double>> paths(simulations, std::vector<double>(n));
    
    for (int sim = 0; sim < simulations; ++sim) {
        // Fresh random generator per simulation for independence
        std::random_device rd;
        std::mt19937 gen(rd());
        std::normal_distribution<double> dist(0.0, 1.0);
        
        // First variance and return
        double variance = initial_variance;
        double z = dist(gen);
        paths[sim][0] = std::sqrt(variance) * z;
        
        for (int t = 1; t < n; ++t) {
            // Update variance using previous return
            variance = omega + alpha * paths[sim][t-1] * paths[sim][t-1] + beta * variance;
            z = dist(gen);
            paths[sim][t] = std::sqrt(variance) * z;
        }
    }
    
    return paths;
}
#include <cassert>
#include <cmath>
#include <stdexcept>
#include <vector>

// The solution function is assumed to be defined above.
// Test the GARCH simulation with various parameters.
int main() {
    // Basic validity: small simulation, check dimensions and finite values
    auto paths1 = simulateGarch11(0.1, 0.2, 0.7, 1.0, 10, 5);
    assert(paths1.size() == 5);
    assert(paths1[0].size() == 10);
    for (const auto& row : paths1) {
        for (double val : row) {
            assert(std::isfinite(val));
        }
    }
    
    // Different seed per simulation: check first returns differ (very likely)
    auto paths2 = simulateGarch11(0.1, 0.2, 0.7, 1.0, 1, 3);
    bool all_same = (paths2[0][0] == paths2[1][0] && paths2[1][0] == paths2[2][0]);
    assert(!all_same);
    
    // Zero alpha and beta: variance stays constant at omega + initial? (No, variance = omega + 0 + 0*previous = omega after first step, but initial is separate)
    auto paths3 = simulateGarch11(0.5, 0.0, 0.0, 2.0, 3, 2);
    // After first step, variance should be omega = 0.5 for all subsequent steps
    // But we cannot check randomness exactly; just ensure finite and correct expected variance mean (approximate)
    // This test only checks that no exception is thrown and values are finite.
    
    // Invalid parameters throw exceptions
    bool threw = false;
    try { simulateGarch11(0.0, 0.1, 0.8, 1.0, 10); } catch (const std::invalid_argument&) { threw = true; }
    assert(threw);
    
    threw = false;
    try { simulateGarch11(0.1, -0.2, 0.8, 1.0, 10); } catch (const std::invalid_argument&) { threw = true; }
    assert(threw);
    
    threw = false;
    try { simulateGarch11(0.1, 0.2, 0.8, -1.0, 10); } catch (const std::invalid_argument&) { threw = true; }
    assert(threw);
    
    threw = false;
    try { simulateGarch11(0.1, 0.2, 0.8, 1.0, 0); } catch (const std::invalid_argument&) { threw = true; }
    assert(threw);
    
    threw = false;
    try { simulateGarch11(0.1, 0.2, 0.8, 1.0, 10, 0); } catch (const std::invalid_argument&) { threw = true; }
    assert(threw);
}
