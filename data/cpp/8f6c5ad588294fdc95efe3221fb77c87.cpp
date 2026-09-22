Given a square matrix of doubles stored in a row-major `std::vector<std::vector<double>>`, write a C++ function `rowProducts` that returns a `std::vector<double>` where each element is the product of all elements in the corresponding row of the input matrix. The matrix is guaranteed to be non-empty (at least one row and one column), but rows may contain zeros, negative values, and very large/small magnitudes that could cause overflow or underflow. The function must handle empty rows by returning `1.0` for that row (the multiplicative identity). The input matrix must not be modified.

// The solution iterates over each row of the matrix, maintaining a running product initialized to `1.0`. For each element in the row, multiply the running product by the element. After processing all elements in a row, push the running product into the result vector. Edge cases:  
// - Empty row: The loop body never executes, so the product remains `1.0`, which matches the multiplicative identity.  
// - Zeros: If a zero appears, the product becomes `0.0` and remains `0.0` regardless of subsequent factors; no special handling needed.  
// - Negative values: Handled naturally by multiplication.  
// - Overflow/underflow: In C++, floating-point multiplication does not trap; it may produce `inf` or `0.0` silently. The task does not require special handling for these extreme values (would need `<cmath>` checks like `std::isfinite`, but that's out of scope).  
//
// Time complexity: O(R*C) where R is number of rows and C is average row length, as each element is visited exactly once. Space complexity: O(R) for the output vector (plus O(1) auxiliary for the running product). The input is not modified, and we pass it by const reference.

#include <vector>

// Compute the product of each row of a square matrix, returning a vector of row products.
// Empty rows yield 1.0 (identity). The input matrix is read-only.
std::vector<double> rowProducts(const std::vector<std::vector<double>>& matrix) {
    std::vector<double> result;
    result.reserve(matrix.size());
    
    for (const auto& row : matrix) {
        double product = 1.0;
        for (double value : row) {
            product *= value;
        }
        result.push_back(product);
    }
    return result;
}

#include <cassert>
#include <vector>

int main() {
    // Basic 2x2 matrix
    std::vector<std::vector<double>> m1 = {{2.0, 3.0}, {4.0, 5.0}};
    assert(rowProducts(m1) == std::vector<double>({6.0, 20.0}));
    
    // Matrix with zeros and negatives
    std::vector<std::vector<double>> m2 = {{0.0, -1.0, 2.0}, {-3.0, 4.0}};
    assert(rowProducts(m2) == std::vector<double>({0.0, -12.0}));
    
    // Single element matrix
    std::vector<std::vector<double>> m3 = {{7.5}};
    assert(rowProducts(m3) == std::vector<double>({7.5}));
    
    // Matrix with an empty row
    std::vector<std::vector<double>> m4 = {{}, {1.0, 2.0}};
    assert(rowProducts(m4) == std::vector<double>({1.0, 2.0}));
    
    // All ones (large row count)
    std::vector<std::vector<double>> m5(100, std::vector<double>(100, 1.0));
    auto result5 = rowProducts(m5);
    for (double val : result5) assert(val == 1.0);
    
    // Matrix with fractional values
    std::vector<std::vector<double>> m6 = {{0.5, 2.0}, {0.25, 4.0}};
    assert(rowProducts(m6) == std::vector<double>({1.0, 1.0}));
    
    // Negative only row
    std::vector<std::vector<double>> m7 = {{-2.0, -3.0}, {-1.0, 1.0}};
    assert(rowProducts(m7) == std::vector<double>({6.0, -1.0}));
    
    // Single row with many elements (including negatives)
    std::vector<std::vector<double>> m8 = {{1.0, -2.0, 3.0, -4.0}};
    assert(rowProducts(m8) == std::vector<double>({24.0}));
    
    return 0;
}
