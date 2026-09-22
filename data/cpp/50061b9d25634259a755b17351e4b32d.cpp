// Write a C++ function that takes two vectors of floating-point values, `x` and `y`, and returns the outer product matrix `A` where `A(i,j) = x(i) * y(j)`. The function should be templated on the value type, handle empty inputs gracefully by returning an empty matrix, and preserve the exact order of elements. The solution must not rely on external libraries like Blitz++; use standard C++ containers and algorithms.

#include <cassert>
#include <vector>
#include <cmath>

int main() {
    // Basic test with integers.
    std::vector<int> x1 = {1, 2, 3};
    std::vector<int> y1 = {4, 5};
    auto A1 = outerProduct(x1, y1);
    assert(A1.size() == 3 && A1[0].size() == 2);
    assert(A1[0][0] == 4 && A1[0][1] == 5);
    assert(A1[1][0] == 8 && A1[1][1] == 10);
    assert(A1[2][0] == 12 && A1[2][1] == 15);

    // Test with doubles.
    std::vector<double> x2 = {0.5, -1.0};
    std::vector<double> y2 = {2.0, 3.0, -4.0};
    auto A2 = outerProduct(x2, y2);
    assert(A2.size() == 2 && A2[0].size() == 3);
    assert(std::fabs(A2[0][0] - 1.0) < 1e-9);
    assert(std::fabs(A2[0][1] - 1.5) < 1e-9);
    assert(std::fabs(A2[0][2] - (-2.0)) < 1e-9);
    assert(std::fabs(A2[1][0] - (-2.0)) < 1e-9);
    assert(std::fabs(A2[1][1] - (-3.0)) < 1e-9);
    assert(std::fabs(A2[1][2] - 4.0) < 1e-9);

    // Test with empty input.
    std::vector<int> empty;
    std::vector<int> y3 = {1, 2};
    auto A3 = outerProduct(empty, y3);
    assert(A3.empty());
    auto A4 = outerProduct(y3, empty);
    assert(A4.empty());
    auto A5 = outerProduct(empty, empty);
    assert(A5.empty());

    // Test with single-element vectors.
    std::vector<int> x6 = {7};
    std::vector<int> y6 = {3};
    auto A6 = outerProduct(x6, y6);
    assert(A6.size() == 1 && A6[0].size() == 1);
    assert(A6[0][0] == 21);

    return 0;
}

#include <vector>

// Compute the outer product of two vectors x and y.
// Returns a matrix A where A[i][j] = x[i] * y[j].
template<typename T>
std::vector<std::vector<T>> outerProduct(const std::vector<T>& x, const std::vector<T>& y) {
    const size_t rows = x.size();
    const size_t cols = y.size();
    
    // If either input is empty, return an empty matrix.
    if (rows == 0 || cols == 0) {
        return {};
    }
    
    std::vector<std::vector<T>> result(rows, std::vector<T>(cols));
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            result[i][j] = x[i] * y[j];
        }
    }
    return result;
}

// The outer product of two vectors `x` (size m) and `y` (size n) produces an m×n matrix. Each element `A(i,j)` is the product of `x[i]` and `y[j]`. The naive implementation uses two nested loops: for each row index `i` from 0 to m-1, and for each column index `j` from 0 to n-1, set `A[i][j] = x[i] * y[j]`. Edge cases: if either input vector is empty, the result should be an empty matrix (0 rows or 0 columns). The time complexity is O(m·n) because every output element must be computed exactly once. The space complexity is also O(m·n) for the output matrix itself, plus O(1) auxiliary space for loop counters and temporaries. The function should be `const`-correct (inputs passed by const reference), and the output type should be a 2D `std::vector` of vectors.
