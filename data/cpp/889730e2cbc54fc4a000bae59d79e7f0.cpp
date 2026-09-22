/*
Write a C++ function `computeEigenvaluesSummary` that takes as input a positive integer `n` representing the number of lowest eigenmodes to compute (analogous to the `nev` parameter in the snippet), and a vector of doubles representing the computed eigenvalues (in ascending order, as returned by the LOBPCG solver). The function should return a `std::string` summarizing the first `n` eigenvalues in the format: `"Mode 1: <value1>\nMode 2: <value2>\n...\nMode n: <valuen>"`, where each value is formatted to 6 decimal places. Additionally, the function should validate that the input vector has at least `n` elements and contains no negative values (since eigenvalues of the Laplacian with Dirichlet BC are non-negative); if validation fails, throw a `std::invalid_argument` exception with a descriptive message. The function must be `const`-correct, free of global state, and self-contained with only standard library headers.
*/
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <stdexcept>

// Summarizes the first n eigenvalues in a formatted string.
// Throws std::invalid_argument if the vector has fewer than n elements
// or if any of the first n eigenvalues is negative.
std::string computeEigenvaluesSummary(int n, const std::vector<double>& eigenvalues) {
    if (n < 0) {
        throw std::invalid_argument("n must be non-negative");
    }
    if (static_cast<size_t>(n) > eigenvalues.size()) {
        throw std::invalid_argument("eigenvalues vector has fewer than n elements");
    }
    for (int i = 0; i < n; ++i) {
        if (eigenvalues[i] < 0.0) {
            throw std::invalid_argument("eigenvalues must be non-negative");
        }
    }

    std::ostringstream result;
    result << std::fixed << std::setprecision(6);
    for (int i = 0; i < n; ++i) {
        result << "Mode " << (i + 1) << ": " << eigenvalues[i] << "\n";
    }
    return result.str();
}
#include <cassert>
#include <cmath>
#include <string>
#include <vector>

// The solution function is assumed to be declared above.

int main() {
    // Basic case: three positive eigenvalues.
    std::vector<double> eig1 = {1.0, 4.0, 9.0, 16.0};
    std::string out1 = computeEigenvaluesSummary(3, eig1);
    assert(out1 == "Mode 1: 1.000000\nMode 2: 4.000000\nMode 3: 9.000000\n");

    // n equals the vector size.
    std::vector<double> eig2 = {0.5, 2.5};
    std::string out2 = computeEigenvaluesSummary(2, eig2);
    assert(out2 == "Mode 1: 0.500000\nMode 2: 2.500000\n");

    // n = 0 returns an empty string.
    std::vector<double> eig3 = {3.14};
    assert(computeEigenvaluesSummary(0, eig3).empty());

    // Extra values beyond n are ignored.
    std::vector<double> eig4 = {0.0, 1.1, 2.2};
    assert(computeEigenvaluesSummary(2, eig4) == "Mode 1: 0.000000\nMode 2: 1.100000\n");

    // Validation: requesting more elements than available throws.
    bool threw = false;
    try {
        computeEigenvaluesSummary(4, eig1);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    // Validation: negative eigenvalue throws.
    threw = false;
    std::vector<double> eig5 = {-1.0, 2.0};
    try {
        computeEigenvaluesSummary(2, eig5);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    // Validation: negative n throws.
    threw = false;
    try {
        computeEigenvaluesSummary(-1, eig1);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    // Large values and precision formatting.
    std::vector<double> eig6 = {123456.789, 0.000001};
    std::string out6 = computeEigenvaluesSummary(2, eig6);
    assert(out6 == "Mode 1: 123456.789000\nMode 2: 0.000001\n");

    // All good.
    return 0;
}
// The solution involves iterating over the first `n` eigenvalues from the input vector and formatting each with fixed precision. The main steps are: first, validate the input size (must be at least `n`) and that all eigenvalues (from index 0 to n-1) are non-negative (using a check that throws `std::invalid_argument` on failure). Then, use `std::ostringstream` to accumulate the output string, setting the precision with `std::fixed << std::setprecision(6)`. For each index `i` from 0 to `n-1`, append `"Mode " << (i+1) << ": " << eigenvalues[i]` followed by a newline (except possibly after the last, but the format spec includes a trailing newline as shown). Edge cases: `n` could be 0 (return empty string), the vector may have extra elements beyond `n` (ignore them), and negative values should be rejected. Time complexity is O(n) because we iterate exactly `n` times and each formatting operation is O(1) (assuming the double-to-string conversion is constant-time per element). Space complexity is O(n) for the output string, plus O(1) auxiliary space for the stream and loop variables. The implementation avoids any reliance on MFEM or external libraries, making it standalone.
