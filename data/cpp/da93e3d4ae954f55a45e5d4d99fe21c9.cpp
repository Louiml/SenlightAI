// Write a C++ function `matrixElementWise` that takes two 2D floating-point vectors (using `std::vector<std::vector<float>>`) of equal dimensions and returns a new vector containing the element-wise (Hadamard) product of the two matrices. The function must also return, via reference parameters, the sum of all elements in the element-wise product and the minimum value in the element-wise product. If the input matrices have different dimensions or are empty, the function should return an empty vector and set the sum and minimum to `0.0f`. Use only standard C++ libraries (no Eigen). The element-wise product is computed as `result[i][j] = A[i][j] * B[i][j]`.
#include <cassert>
#include <cmath>

int main() {
    // Test 1: 2x2 matrices
    std::vector<std::vector<float>> A1 = {{1, 2}, {3, 4}};
    std::vector<std::vector<float>> B1 = {{5, 6}, {7, 8}};
    float sum1 = 0, min1 = 0;
    auto res1 = matrixElementWise(A1, B1, sum1, min1);
    assert(res1.size() == 2 && res1[0].size() == 2);
    assert(res1[0][0] == 5 && res1[0][1] == 12);
    assert(res1[1][0] == 21 && res1[1][1] == 32);
    assert(sum1 == 70.0f);
    assert(min1 == 5.0f);

    // Test 2: Negative values
    std::vector<std::vector<float>> A2 = {{-1, 2}, {3, -4}};
    std::vector<std::vector<float>> B2 = {{5, -6}, {7, 8}};
    float sum2 = 0, min2 = 0;
    auto res2 = matrixElementWise(A2, B2, sum2, min2);
    assert(res2[0][0] == -5 && res2[0][1] == -12);
    assert(res2[1][0] == 21 && res2[1][1] == -32);
    assert(sum2 == -28.0f);
    assert(min2 == -32.0f);

    // Test 3: 1x1 matrix
    std::vector<std::vector<float>> A3 = {{7}};
    std::vector<std::vector<float>> B3 = {{3}};
    float sum3 = 0, min3 = 0;
    auto res3 = matrixElementWise(A3, B3, sum3, min3);
    assert(res3[0][0] == 21.0f);
    assert(sum3 == 21.0f);
    assert(min3 == 21.0f);

    // Test 4: Mismatched dimensions
    std::vector<std::vector<float>> A4 = {{1, 2}, {3, 4}};
    std::vector<std::vector<float>> B4 = {{1, 2, 3}, {4, 5, 6}};
    float sum4 = 1, min4 = 1;
    auto res4 = matrixElementWise(A4, B4, sum4, min4);
    assert(res4.empty());
    assert(sum4 == 0.0f && min4 == 0.0f);

    // Test 5: Empty matrix
    std::vector<std::vector<float>> A5 = {};
    std::vector<std::vector<float>> B5 = {{1}};
    float sum5 = 1, min5 = 1;
    auto res5 = matrixElementWise(A5, B5, sum5, min5);
    assert(res5.empty());
    assert(sum5 == 0.0f && min5 == 0.0f);

    return 0;
}
#include <vector>
#include <limits>

// Compute element-wise product of two matrices.
// If dimensions mismatch or either is empty, return empty vector and set sum and min to 0.0f.
// On success, writes sum of all product elements to totalSum and the smallest product element to minValue.
std::vector<std::vector<float>> matrixElementWise(
    const std::vector<std::vector<float>>& A,
    const std::vector<std::vector<float>>& B,
    float& totalSum,
    float& minValue) {
    
    // Validate inputs
    if (A.empty() || B.empty() || A.size() != B.size()) {
        totalSum = 0.0f;
        minValue = 0.0f;
        return {};
    }
    
    size_t rows = A.size();
    size_t cols = A[0].size();
    
    // Check all rows have consistent size and B has same dimensions
    for (size_t i = 0; i < rows; ++i) {
        if (A[i].empty() || B[i].empty() || A[i].size() != cols || B[i].size() != cols) {
            totalSum = 0.0f;
            minValue = 0.0f;
            return {};
        }
    }
    
    std::vector<std::vector<float>> result(rows, std::vector<float>(cols));
    totalSum = 0.0f;
    minValue = std::numeric_limits<float>::max();
    
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            float product = A[i][j] * B[i][j];
            result[i][j] = product;
            totalSum += product;
            if (product < minValue) {
                minValue = product;
            }
        }
    }
    
    return result;
}
// The solution first checks the input matrices for valid and matching dimensions: both must be non-empty (outer vector and inner rows non-empty) and have the same number of rows and columns. If invalid, return an empty vector and set reference outputs to `0.0f`. Otherwise, allocate a result vector of the same size. Iterate through each row and column, compute the product of corresponding elements, store into the result, accumulate the sum, and track the minimum value (initialized from the first product). The algorithm runs in O(R*C) time where R and C are dimensions, and uses O(R*C) space for the result (plus O(1) extra for the sum and min tracking). Edge cases: single‑element matrices, negative values (products can be positive or negative, min tracking works), and mismatched dimensions.
