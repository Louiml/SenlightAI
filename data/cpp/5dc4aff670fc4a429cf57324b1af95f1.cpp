// Write a C++ function `computeMaxEigenvalue(int n, const std::vector<double>& eigenvalues, double cfl)` that simulates the core time-step stability check from the given CFD solver code. The function should take an array of local eigenvalues (representing the maximum wave speed in each cell), apply an optional MPI-like reduction pattern (for this standalone task, assume all eigenvalues are on a single processor), determine the global maximum eigenvalue, and return the maximum allowable time step `dt` computed as `0.9 * cfl / maxEigenvalue`. Handle the edge case where `maxEigenvalue` is zero (indicating no signal propagation) by returning `std::numeric_limits<double>::max()`. The function must be `const`-correct and include proper validation for an empty input vector.

// The solution iterates through the input vector to find the maximum eigenvalue using the standard `std::max_element` algorithm, which runs in O(n) time. The critical edge cases are: (1) an empty vector should return the maximum possible time step, and (2) a zero maximum eigenvalue (e.g., all zeros) should also return the maximum time step since division by zero is undefined. The time step is computed using the formula `dt = 0.9 * cfl / maxEig`, where the 0.9 factor is a safety margin typical in CFD stability analysis. For space complexity, the algorithm uses O(1) auxiliary space since it only tracks the maximum value while scanning the input. The function should use `std::max_element` or a manual loop, and for the empty/zero cases, use `std::numeric_limits<double>::max()` as the sentinel return value.

#include <algorithm>
#include <limits>
#include <vector>

/**
 * Computes the maximum allowable time step for a CFD simulation based on
 * the local eigenvalues (maximum wave speeds) and the CFL number.
 *
 * @param eigenvalues Vector of local eigenvalues (one per cell).
 * @param cfl The CFL number (must be positive).
 * @return The maximum time step, or std::numeric_limits<double>::max()
 *         if the vector is empty or all eigenvalues are zero.
 */
double computeMaxEigenvalue(const std::vector<double>& eigenvalues, double cfl) {
    if (eigenvalues.empty()) {
        return std::numeric_limits<double>::max();
    }

    double maxEig = *std::max_element(eigenvalues.begin(), eigenvalues.end());

    if (maxEig == 0.0) {
        return std::numeric_limits<double>::max();
    }

    // Use a safety factor of 0.9 as in the original solver code
    return 0.9 * cfl / maxEig;
}

#include <cassert>
#include <vector>

// Forward declaration of the function under test
double computeMaxEigenvalue(const std::vector<double>& eigenvalues, double cfl);

int main() {
    // Normal case with positive eigenvalues
    std::vector<double> eigenvalues1 = {1.0, 2.0, 0.5};
    double dt1 = computeMaxEigenvalue(eigenvalues1, 0.5);
    assert(dt1 == 0.9 * 0.5 / 2.0);  // 0.225

    // All eigenvalues equal
    std::vector<double> eigenvalues2 = {3.0, 3.0, 3.0};
    double dt2 = computeMaxEigenvalue(eigenvalues2, 1.0);
    assert(dt2 == 0.9 * 1.0 / 3.0);  // 0.3

    // Negative eigenvalues (should take absolute? No, just max)
    std::vector<double> eigenvalues3 = {-2.0, -1.0, -3.0};
    double dt3 = computeMaxEigenvalue(eigenvalues3, 0.8);
    assert(dt3 == 0.9 * 0.8 / (-1.0));  // -0.72 (but max is -1.0)

    // Empty vector
    std::vector<double> eigenvalues4;
    double dt4 = computeMaxEigenvalue(eigenvalues4, 0.5);
    assert(dt4 == std::numeric_limits<double>::max());

    // All zeros
    std::vector<double> eigenvalues5 = {0.0, 0.0, 0.0};
    double dt5 = computeMaxEigenvalue(eigenvalues5, 0.5);
    assert(dt5 == std::numeric_limits<double>::max());

    // Single element
    std::vector<double> eigenvalues6 = {4.0};
    double dt6 = computeMaxEigenvalue(eigenvalues6, 0.25);
    assert(dt6 == 0.9 * 0.25 / 4.0);  // 0.05625

    // Large values
    std::vector<double> eigenvalues7 = {1000.0, 2000.0};
    double dt7 = computeMaxEigenvalue(eigenvalues7, 2.0);
    assert(dt7 == 0.9 * 2.0 / 2000.0);  // 0.0009

    // Mixed positive and negative
    std::vector<double> eigenvalues8 = {-5.0, 10.0, 0.0, 7.0};
    double dt8 = computeMaxEigenvalue(eigenvalues8, 1.0);
    assert(dt8 == 0.9 * 1.0 / 10.0);  // 0.09

    // Very small eigenvalues
    std::vector<double> eigenvalues9 = {0.001, 0.0005};
    double dt9 = computeMaxEigenvalue(eigenvalues9, 0.5);
    assert(dt9 == 0.9 * 0.5 / 0.001);  // 450.0

    return 0;
}
