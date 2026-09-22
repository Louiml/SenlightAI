// Write a standalone C++ function that, given a square matrix represented as an `std::vector<std::vector<double>>`, returns a new matrix that is the adjoint (conjugate transpose) of the input. The input matrix is guaranteed to be square (n×n), may contain complex-like data via real values (since we use `double`, conjugation is the identity), and the function must preserve the original matrix unchanged. The function should handle the edge case of a 1×1 matrix gracefully, returning the same value. The returned matrix must have the property that `result[i][j] == input[j][i]` for all valid indices. The function must be `const`-correct, meaning it takes a `const` reference to the input and returns a new matrix by value.
// The adjoint of a matrix is the transpose of its conjugate. Since the data type is `double` (real numbers), conjugation is the identity, so the adjoint reduces to the transpose. The main algorithm is straightforward: create a new matrix of the same size, then for each `i` and `j`, set `result[i][j] = input[j][i]`. This is a simple nested loop over the n×n elements. Edge cases: a 1×1 matrix transposes to itself, and empty matrices (n=0) should return an empty matrix without crashing. Time complexity is O(n²) because we visit every element of the n×n matrix exactly once. Space complexity is O(n²) for the returned matrix; no additional auxiliary space beyond that is required.
#include <vector>

// Compute the adjoint (conjugate transpose) of a square matrix.
// Since the input uses real `double` values, conjugation is identity,
// so the adjoint is simply the transpose.
std::vector<std::vector<double>> matrixAdjoint(const std::vector<std::vector<double>>& matrix) {
    // Handle empty or non-square input gracefully.
    if (matrix.empty()) {
        return {};
    }
    const std::size_t n = matrix.size();
    
    // Create a result matrix of the same dimensions.
    std::vector<std::vector<double>> result(n, std::vector<double>(n));
    
    // Transpose: result[i][j] = matrix[j][i].
    for (std::size_t i = 0; i < n; ++i) {
        for (std::size_t j = 0; j < n; ++j) {
            result[i][j] = matrix[j][i];
        }
    }
    return result;
}
#include <cassert>
#include <vector>

int main() {
    // The solution function is defined above; here we test it.
    // Test 1: 2x2 matrix
    std::vector<std::vector<double>> m1 = {{1.0, 2.0}, {3.0, 4.0}};
    auto r1 = matrixAdjoint(m1);
    assert(r1.size() == 2);
    assert(r1[0][0] == 1.0 && r1[0][1] == 3.0);
    assert(r1[1][0] == 2.0 && r1[1][1] == 4.0);

    // Test 2: 3x3 with negative values
    std::vector<std::vector<double>> m2 = {{-1.5, 0.5, 2.0}, {3.0, -2.0, 1.0}, {4.0, 5.0, -6.0}};
    auto r2 = matrixAdjoint(m2);
    for (std::size_t i = 0; i < 3; ++i)
        for (std::size_t j = 0; j < 3; ++j)
            assert(r2[i][j] == m2[j][i]);

    // Test 3: 1x1 matrix
    std::vector<std::vector<double>> m3 = {{7.25}};
    auto r3 = matrixAdjoint(m3);
    assert(r3.size() == 1 && r3[0][0] == 7.25);

    // Test 4: Empty matrix
    std::vector<std::vector<double>> m4 = {};
    auto r4 = matrixAdjoint(m4);
    assert(r4.empty());

    // Test 5: Symmetric matrix, adjoint equals original
    std::vector<std::vector<double>> m5 = {{1.0, 2.0, 3.0}, {2.0, 4.0, 5.0}, {3.0, 5.0, 6.0}};
    auto r5 = matrixAdjoint(m5);
    assert(r5 == m5);

    // Test 6: Verify original matrix is unchanged
    std::vector<std::vector<double>> m6 = {{1.0, 2.0}, {3.0, 4.0}};
    auto original = m6;
    auto r6 = matrixAdjoint(m6);
    assert(m6 == original);

    return 0;
}
