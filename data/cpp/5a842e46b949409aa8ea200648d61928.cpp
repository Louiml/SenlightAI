// Write a C++ function `double rbfDistance(const std::vector<double>& x, const std::vector<double>& y, double gamma)` that computes the RBF (radial basis function) kernel distance between two dense feature vectors. The function should return `exp(-gamma * ||x - y||^2)` where `||x - y||^2` is the squared Euclidean distance. The input vectors are of equal length (you may assume this precondition without checking). Handle edge cases: if both vectors are empty, return 1.0 (since squared distance is 0, exp(0)=1). If `gamma` is 0, return exactly 1.0 regardless of input. If the squared distance is very large, the result should gradually approach 0 without overflowing; you can compute the squared distance directly in double precision and then apply `std::exp` — but be careful that large squared distances (e.g., 1e6 * 1e6 = 1e12) are still comfortably representable in double. The function should be `const`‑correct and take the vectors by const reference. Provide a self‑contained implementation with the required headers and a descriptively named free function.

#include <cassert>
#include <cmath>
#include <vector>

// Include the solution (or paste it here)
double rbfDistance(const std::vector<double>& x, const std::vector<double>& y, double gamma) {
    if (gamma == 0.0) {
        return 1.0;
    }
    double squaredDistance = 0.0;
    for (size_t i = 0; i < x.size(); ++i) {
        double diff = x[i] - y[i];
        squaredDistance += diff * diff;
    }
    return std::exp(-gamma * squaredDistance);
}

int main() {
    // Test empty vectors: distance is 0, exp(0)=1.
    std::vector<double> empty1, empty2;
    assert(std::fabs(rbfDistance(empty1, empty2, 0.5) - 1.0) < 1e-12);

    // Test identical vectors: distance 0, result 1 regardless of gamma.
    std::vector<double> a = {1.0, 2.0, 3.0};
    std::vector<double> b = {1.0, 2.0, 3.0};
    assert(std::fabs(rbfDistance(a, b, 2.0) - 1.0) < 1e-12);

    // Test gamma = 0: always 1.
    std::vector<double> c = {0.0, 0.0};
    std::vector<double> d = {1.0, 1.0};
    assert(std::fabs(rbfDistance(c, d, 0.0) - 1.0) < 1e-12);

    // Test a non-trivial case: x = (1,1), y = (1,2), squared dist = 1, gamma = 0.5 -> exp(-0.5)
    std::vector<double> e = {1.0, 1.0};
    std::vector<double> f = {1.0, 2.0};
    double expected = std::exp(-0.5 * 1.0);
    assert(std::fabs(rbfDistance(e, f, 0.5) - expected) < 1e-12);

    // Test larger distance: x = (0,0), y = (3,4), squared dist = 25, gamma = 0.1 -> exp(-2.5)
    std::vector<double> g = {0.0, 0.0};
    std::vector<double> h = {3.0, 4.0};
    expected = std::exp(-0.1 * 25.0);
    assert(std::fabs(rbfDistance(g, h, 0.1) - expected) < 1e-12);

    // Test negative values: x = (-1, -1), y = (1, 1), squared dist = 8, gamma = 1.0 -> exp(-8)
    std::vector<double> i = {-1.0, -1.0};
    std::vector<double> j = {1.0, 1.0};
    expected = std::exp(-8.0);
    assert(std::fabs(rbfDistance(i, j, 1.0) - expected) < 1e-12);

    return 0;
}

#include <cmath>
#include <vector>

// Compute the RBF kernel value exp(-gamma * ||x - y||^2) for two dense vectors.
// Precondition: x and y have the same size (not checked).
double rbfDistance(const std::vector<double>& x, const std::vector<double>& y, double gamma) {
    // Handle the trivial case where gamma is zero: exp(0) = 1 for any distance.
    if (gamma == 0.0) {
        return 1.0;
    }

    double squaredDistance = 0.0;
    for (size_t i = 0; i < x.size(); ++i) {
        double diff = x[i] - y[i];
        squaredDistance += diff * diff;
    }

    return std::exp(-gamma * squaredDistance);
}

// The solution computes the squared Euclidean distance by iterating over corresponding elements of the two vectors and accumulating `(x[i] - y[i]) * (x[i] - y[i])`. This is a straightforward element‑wise accumulation that runs in O(n) time where n is the vector size, and uses O(1) auxiliary space. The only edge cases are: (1) empty vectors — the loop does nothing, distance remains 0, so `exp(-gamma*0) = exp(0) = 1`; (2) `gamma = 0` — the exponent becomes 0 regardless of distance, so we can return 1.0 immediately (though the direct computation would also give 1.0). For numerical stability, we rely on double precision; typical feature values are within reasonable ranges, and even large squared distances (e.g., 1e12) are well within the range of double, and `std::exp` handles negative large arguments gracefully by returning a very small positive number, eventually underflowing to 0 — but that is acceptable behavior. We do not need to worry about overflow in the accumulation because we are summing non‑negative values; the largest possible sum is bounded by n * (max_value - min_value)^2, which for typical data is far below DBL_MAX. We simply use `std::exp` from `<cmath>`. The function is marked `const`‑correctly by accepting `const std::vector<double>&` parameters, and we avoid any mutation of inputs.
