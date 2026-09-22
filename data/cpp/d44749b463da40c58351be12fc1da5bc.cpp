Given a dense square matrix represented as a flat `std::vector<double>` in row-major order, write a C++ function that computes the product of this matrix with a given vector, returning the result as a `std::vector<double>`. The matrix size is provided separately. The function must handle the general case where the matrix may contain zero entries and the vector may have arbitrary values, including negative and fractional numbers. The function should be named `matrixVectorProduct` and must not modify the input matrix or vector. Ensure the computation is done with straightforward nested loops, with the outer loop over rows and inner loop over columns, to demonstrate basic linear algebra operations.

#include <cassert>
#include <cmath>

int main() {
    // Test 1: Identity matrix 2x2 times vector [1,2] should give [1,2]
    {
        std::vector<double> mat = {1,0, 0,1};
        std::vector<double> vec = {1,2};
        std::vector<double> res = matrixVectorProduct(mat, vec, 2);
        assert(res.size() == 2);
        assert(std::fabs(res[0] - 1.0) < 1e-12);
        assert(std::fabs(res[1] - 2.0) < 1e-12);
    }

    // Test 2: Zero matrix times any vector should give zero vector
    {
        std::vector<double> mat = {0,0, 0,0};
        std::vector<double> vec = {3,-4};
        std::vector<double> res = matrixVectorProduct(mat, vec, 2);
        assert(res[0] == 0.0 && res[1] == 0.0);
    }

    // Test 3: Non-square? But n is 3 for 3x3 matrix, standard multiplication
    {
        std::vector<double> mat = {1,2,3, 4,5,6, 7,8,10};
        std::vector<double> vec = {1,1,1};
        std::vector<double> res = matrixVectorProduct(mat, vec, 3);
        assert(res[0] == 6.0); // 1+2+3
        assert(res[1] == 15.0); // 4+5+6
        assert(res[2] == 25.0); // 7+8+10
    }

    // Test 4: Negative and fractional values
    {
        std::vector<double> mat = {0.5, -1.0, 2.0, 0.25};
        std::vector<double> vec = {-2.0, 4.0};
        std::vector<double> res = matrixVectorProduct(mat, vec, 2);
        // row0: 0.5*(-2) + (-1)*4 = -1 -4 = -5
        // row1: 2*(-2) + 0.25*4 = -4 + 1 = -3
        assert(std::fabs(res[0] - (-5.0)) < 1e-12);
        assert(std::fabs(res[1] - (-3.0)) < 1e-12);
    }

    // Test 5: 1x1 matrix
    {
        std::vector<double> mat = {7.0};
        std::vector<double> vec = {3.0};
        std::vector<double> res = matrixVectorProduct(mat, vec, 1);
        assert(res[0] == 21.0);
    }

    // Test 6: Empty matrix (n=0) returns empty vector
    {
        std::vector<double> mat = {};
        std::vector<double> vec = {};
        std::vector<double> res = matrixVectorProduct(mat, vec, 0);
        assert(res.empty());
    }

    // Test 7: Larger random-like matrix, verify against manual computation
    {
        std::vector<double> mat = {1, 0, 0, 0, 2, 0, 0, 0, 3};
        std::vector<double> vec = {5, -2, 4};
        std::vector<double> res = matrixVectorProduct(mat, vec, 3);
        assert(res[0] == 5.0); // 1*5
        assert(res[1] == -4.0); // 2*(-2)
        assert(res[2] == 12.0); // 3*4
    }

    // Test 8: Ensure original inputs are not modified
    {
        std::vector<double> mat = {1,2,3,4};
        std::vector<double> vec = {1,1};
        std::vector<double> original_mat = mat;
        std::vector<double> original_vec = vec;
        std::vector<double> res = matrixVectorProduct(mat, vec, 2);
        assert(mat == original_mat);
        assert(vec == original_vec);
    }

    // Test 9: Non-diagonal dense matrix
    {
        std::vector<double> mat = {2, -1, 0, -1, 2, -1, 0, -1, 2};
        std::vector<double> vec = {1, 2, 3};
        // row0: 2*1 + (-1)*2 + 0*3 = 2 -2 = 0
        // row1: -1*1 + 2*2 + (-1)*3 = -1 +4 -3 = 0
        // row2: 0*1 + (-1)*2 + 2*3 = -2 +6 = 4
        std::vector<double> res = matrixVectorProduct(mat, vec, 3);
        assert(res[0] == 0.0);
        assert(res[1] == 0.0);
        assert(res[2] == 4.0);
    }

    // Test 10: Large n=1000, spot check first and last entries
    {
        size_t n = 1000;
        std::vector<double> mat(n*n, 1.0); // all ones matrix
        std::vector<double> vec(n, 2.0);   // all twos vector
        std::vector<double> res = matrixVectorProduct(mat, vec, n);
        assert(res.size() == n);
        // Each entry = sum of n * 2 = 2000
        assert(res[0] == 2000.0);
        assert(res[n-1] == 2000.0);
    }

    return 0;
}

#include <vector>

// Compute the product of a dense square matrix (stored row-major) and a vector.
// matrix: size n*n, row-major order (element [i][j] at index i*n + j)
// vector: size n
// Returns a new vector of size n containing the matrix-vector product.
std::vector<double> matrixVectorProduct(const std::vector<double>& matrix, const std::vector<double>& vector, size_t n) {
    std::vector<double> result(n, 0.0);
    if (n == 0) return result; // Edge case: empty matrix and vector

    for (size_t i = 0; i < n; ++i) {
        double sum = 0.0;
        for (size_t j = 0; j < n; ++j) {
            sum += matrix[i * n + j] * vector[j];
        }
        result[i] = sum;
    }
    return result;
}

// The solution requires implementing a standard matrix-vector multiplication. Given an \(n \times n\) matrix \(A\) stored in row-major order (i.e., element \(A[i][j]\) is at index `i * n + j` in the vector) and a vector \(x\) of length \(n\), the result vector \(y\) is computed as \(y_i = \sum_{j=0}^{n-1} A[i][j] \cdot x_j\) for each \(i\). The main edge cases include: (1) an empty matrix (where `n` is 0, but the input vectors are empty, so we return an empty result), (2) a matrix with zero rows or columns but inconsistent sizes (we assume the input is valid per contract, so no validation needed), and (3) potential overflow or underflow, but since the function uses `double`, no special handling is required. The algorithm is straightforward: iterate over each row, accumulate the dot product with the vector, and store the result. Time complexity is \(O(n^2)\) because there are \(n^2\) multiplications and additions. Space complexity is \(O(n)\) for the result vector, plus constant extra space. The function should be `const`-correct by taking the input vectors as `const std::vector<double>&` and returning a new vector, leaving inputs unchanged.
