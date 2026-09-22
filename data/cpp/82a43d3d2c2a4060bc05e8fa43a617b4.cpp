/*
Write a standalone C++ function named `computeLocalDotProduct` that computes the dot product of two vectors (represented as `std::vector<double>`) and returns the result as a `double`. The function must take a parameter `bool& isOptimized` and set it to `false` to indicate that it uses a simple, non-optimized implementation (analogous to the reference routine in the snippet). The function should handle empty vectors gracefully by returning `0.0`. Assume both input vectors have the same length; if they do not, the function should return `0.0` as well. The function must not modify the input vectors.
*/

#include <vector>

// Compute the dot product of two equal-length vectors.
// Sets isOptimized to false to indicate a reference (non-optimized) implementation.
// Returns 0.0 for empty or mismatched input vectors.
double computeLocalDotProduct(const std::vector<double>& x, const std::vector<double>& y, bool& isOptimized) {
    isOptimized = false; // reference implementation, not optimized

    if (x.size() != y.size() || x.empty()) {
        return 0.0;
    }

    double result = 0.0;
    for (std::size_t i = 0; i < x.size(); ++i) {
        result += x[i] * y[i];
    }
    return result;
}

#include <cassert>
#include <vector>

int main() {
    // Basic dot product with positive numbers
    std::vector<double> a1 = {1.0, 2.0, 3.0};
    std::vector<double> b1 = {4.0, 5.0, 6.0};
    bool opt = true;
    assert(computeLocalDotProduct(a1, b1, opt) == 32.0);
    assert(opt == false);

    // Negative numbers
    std::vector<double> a2 = {-1.0, -2.0};
    std::vector<double> b2 = {3.0, -4.0};
    assert(computeLocalDotProduct(a2, b2, opt) == 5.0); // (-1*3) + (-2*-4) = -3 + 8 = 5

    // Single element
    std::vector<double> a3 = {7.0};
    std::vector<double> b3 = {2.5};
    assert(computeLocalDotProduct(a3, b3, opt) == 17.5);

    // Empty vectors return 0
    std::vector<double> a4;
    std::vector<double> b4;
    assert(computeLocalDotProduct(a4, b4, opt) == 0.0);

    // Mismatched lengths return 0 (should not normally happen)
    std::vector<double> a5 = {1.0, 2.0};
    std::vector<double> b5 = {1.0};
    assert(computeLocalDotProduct(a5, b5, opt) == 0.0);

    // All zeros
    std::vector<double> a6 = {0.0, 0.0, 0.0};
    std::vector<double> b6 = {1.0, 2.0, 3.0};
    assert(computeLocalDotProduct(a6, b6, opt) == 0.0);
}

// The main algorithm is straightforward: iterate through both vectors element-by-element, multiply corresponding elements, and accumulate the sum. Important edge cases include:
// - Empty vectors: return `0.0` because the dot product of empty vectors is conventionally zero.
// - Vectors of unequal length: since the task specifies they are expected to be the same, but to be safe we check and return `0.0` to avoid out-of-bounds access.
// - Large sums may cause floating-point precision issues, but for a simple reference implementation that is acceptable.
// - The function must set `isOptimized = false` as required by the task, indicating it is the reference (non-optimized) version.
//
// Time complexity is O(n) where n is the length of the vectors, because we perform a single pass. Space complexity is O(1) aside from the input vectors themselves, since we only use a few local variables.
