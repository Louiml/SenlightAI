// Given three vectors `a`, `b`, and `c` of equal length (`std::vector<double>`), write a C++ function `computeWeightedDistance` that returns a `double` representing the weighted Euclidean distance between `a` and `b`, where each dimension `i` is scaled by the corresponding element of `c` (which represents per-dimension weights). Specifically, the function should compute `sqrt( sum_i ( c[i] * (a[i] - b[i]) )^2 )`. The input vectors are guaranteed to be non-empty and of equal length. The function must be `const`-correct, use `size_t` for indexing, and throw a `std::invalid_argument` if any vector is empty or if lengths mismatch. You may assume all weights in `c` are non-negative. Return `0.0` if the vectors are empty? No—must throw. The solution should be standalone, contain all necessary headers, and not rely on external libraries like Eigen.

// The core operation is a straightforward element-wise computation over three equal-length vectors. First, validate that the input vectors are non-empty and have matching sizes; otherwise, throw `std::invalid_argument` with a descriptive message. Then initialize `double sum = 0.0` and iterate over indices from `0` to `size-1`. For each index `i`, compute the difference `diff = a[i] - b[i]`, scale it by the weight `c[i]`, square the product, and accumulate into `sum`. After the loop, return `std::sqrt(sum)`. Edge cases: if all differences are zero, the result is `0.0`; if weights are zero, those dimensions contribute nothing. Complexity: time is `O(n)` where `n` is the vector size, space is `O(1)` additional. Since we use `std::sqrt`, include `<cmath>`. The function signature should use `const std::vector<double>&` for all parameters.

#include <vector>
#include <cmath>
#include <stdexcept>

// Compute weighted Euclidean distance between vectors a and b using weights c.
double computeWeightedDistance(const std::vector<double>& a,
                               const std::vector<double>& b,
                               const std::vector<double>& c) {
    const std::size_t n = a.size();
    if (n == 0) {
        throw std::invalid_argument("Vectors must be non-empty.");
    }
    if (b.size() != n || c.size() != n) {
        throw std::invalid_argument("All vectors must have equal length.");
    }

    double sum = 0.0;
    for (std::size_t i = 0; i < n; ++i) {
        const double diff = a[i] - b[i];
        const double weighted = c[i] * diff;
        sum += weighted * weighted;
    }
    return std::sqrt(sum);
}

#include <cassert>
#include <cmath>

int main() {
    // Basic case with equal weights.
    std::vector<double> a1 = {1.0, 2.0, 3.0};
    std::vector<double> b1 = {4.0, 6.0, 3.0};
    std::vector<double> c1 = {1.0, 1.0, 1.0};
    double result1 = computeWeightedDistance(a1, b1, c1);
    // sqrt( (1-4)^2 + (2-6)^2 + (3-3)^2 ) = sqrt(9+16+0)=5
    assert(std::fabs(result1 - 5.0) < 1e-9);

    // Weighted case: weights scale differences.
    std::vector<double> a2 = {0.0, 0.0};
    std::vector<double> b2 = {2.0, 0.0};
    std::vector<double> c2 = {0.5, 2.0};
    double result2 = computeWeightedDistance(a2, b2, c2);
    // sqrt( (0.5*2)^2 + (2*0)^2 ) = sqrt(1) = 1
    assert(std::fabs(result2 - 1.0) < 1e-9);

    // Identical vectors produce zero.
    std::vector<double> a3 = {1.0, -2.0, 3.5};
    std::vector<double> b3 = {1.0, -2.0, 3.5};
    std::vector<double> c3 = {0.1, 2.0, 5.0};
    assert(std::fabs(computeWeightedDistance(a3, b3, c3) - 0.0) < 1e-12);

    // Zero weights suppress contributions.
    std::vector<double> a4 = {10.0, 10.0};
    std::vector<double> b4 = {20.0, 30.0};
    std::vector<double> c4 = {0.0, 0.0};
    assert(std::fabs(computeWeightedDistance(a4, b4, c4) - 0.0) < 1e-12);

    // Single-element vectors.
    std::vector<double> a5 = {3.0};
    std::vector<double> b5 = {0.0};
    std::vector<double> c5 = {2.0};
    // sqrt( (2*3)^2 ) = 6
    assert(std::fabs(computeWeightedDistance(a5, b5, c5) - 6.0) < 1e-9);

    // Test exception on empty input.
    std::vector<double> empty, vec1 = {1.0}, vec2 = {1.0};
    bool threw = false;
    try {
        computeWeightedDistance(empty, vec1, vec2);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    // Test exception on mismatch lengths.
    threw = false;
    std::vector<double> a6 = {1.0, 2.0};
    try {
        computeWeightedDistance(a6, vec1, vec2);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    return 0;
}
