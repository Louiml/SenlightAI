Write a C++ function named `computeMean` that accepts a `std::vector<double>` by const reference and returns the arithmetic mean as a `double`. If the vector is empty, the function must throw a `std::domain_error` with the message `"Mean of empty vector is undefined"`. The function should not modify the input vector and must be declared with `const` correctness for parameters and internal checks. Use `std::accumulate` from `<numeric>` for summing, and ensure proper type handling for large vectors (e.g., use a `double` for the sum to avoid integer overflow accumulation). The task must be standalone and not rely on any external modules or non‑standard libraries (the original snippet uses a fictional `import statistics;`—your solution must replace that with standard C++ code).
// The core algorithm is a single pass over the vector: sum all elements using `std::accumulate` with an initial value of `0.0` (a `double`), then divide by the size (cast to `double` to avoid integer truncation). The primary edge case is an empty vector; check `vector.empty()` at the start and throw `std::domain_error` with a descriptive message. Other edge cases include vectors with one element (mean is that element), negative values, and very large values—the `double` accumulator handles standard magnitude ranges. Time complexity is O(n) for summing `n` elements, and space complexity is O(1) beyond the input vector (no additional storage). Ensure the function is `const`‑correct: the parameter is `const std::vector<double>&`, and the function itself can be marked `noexcept` only if the throw condition is removed, but since it throws, leave exception specification off. For testing, compare floating‑point results using `assert` with a small tolerance (e.g., `fabs(actual - expected) < 1e-12`) to handle floating‑point rounding.
#include <vector>
#include <numeric>
#include <stdexcept>
#include <string>

// Compute the arithmetic mean of a vector of doubles.
// Throws std::domain_error if the vector is empty.
double computeMean(const std::vector<double>& data) {
    if (data.empty()) {
        throw std::domain_error("Mean of empty vector is undefined");
    }
    double sum = std::accumulate(data.begin(), data.end(), 0.0);
    return sum / static_cast<double>(data.size());
}
#include <cassert>
#include <cmath>
#include <vector>
#include <stdexcept>

double computeMean(const std::vector<double>& data); // declared in solution

int main() {
    // Test empty vector throws
    bool threw = false;
    try {
        std::vector<double> empty;
        computeMean(empty);
    } catch (const std::domain_error& e) {
        threw = true;
        assert(std::string(e.what()) == "Mean of empty vector is undefined");
    }
    assert(threw);

    // Single element
    assert(std::fabs(computeMean({5.0}) - 5.0) < 1e-12);

    // Positive integers
    assert(std::fabs(computeMean({1.0, 2.0, 3.0, 4.0}) - 2.5) < 1e-12);

    // Negative and zero
    assert(std::fabs(computeMean({-1.0, 0.0, 1.0}) - 0.0) < 1e-12);

    // Large values
    std::vector<double> large(1000000, 1000.0);
    assert(std::fabs(computeMean(large) - 1000.0) < 1e-9);

    // Fractional values
    assert(std::fabs(computeMean({0.1, 0.2, 0.3}) - 0.2) < 1e-12);

    // Mixed signs
    assert(std::fabs(computeMean({-5.5, 2.5, 3.0}) - 0.0) < 1e-12);

    // const correctness: function takes const ref
    const std::vector<double> constVec = {10.0, 20.0};
    assert(std::fabs(computeMean(constVec) - 15.0) < 1e-12);

    return 0;
}
