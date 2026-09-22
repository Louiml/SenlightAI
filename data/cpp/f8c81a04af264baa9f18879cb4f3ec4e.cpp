/*
Write a C++ function that takes a 3x3 matrix of doubles and a 3-dimensional vector of doubles as parameters, and returns a 3-dimensional vector containing the result of multiplying the matrix by the vector. The function must first normalize the input matrix by adding 1.2 to every element and then multiplying every element by 50 (i.e., new_matrix = (matrix + 1.2) * 50 elementwise), then compute the product of this transformed matrix with the input vector. The function should be `const`-correct, take inputs by const reference, and return the resulting vector by value. The input matrix and vector should not be modified. Use Eigen library types `Eigen::Matrix3d` and `Eigen::Vector3d`. Do not print anything; only perform the computation and return the result.
*/
#include <Eigen/Dense>

// Compute (matrix + 1.2) * 50, then multiply by vector.
Eigen::Vector3d transformAndMultiply(
    const Eigen::Matrix3d& matrix,
    const Eigen::Vector3d& vector
) {
    // Add 1.2 to every element, then multiply every element by 50.
    Eigen::Matrix3d transformed = (matrix + Eigen::Matrix3d::Constant(1.2)) * 50.0;
    // Return the matrix-vector product.
    return transformed * vector;
}
#include <Eigen/Dense>
#include <cassert>

// Declare the function from the solution.
Eigen::Vector3d transformAndMultiply(
    const Eigen::Matrix3d& matrix,
    const Eigen::Vector3d& vector
);

int main() {
    // Test 1: Identity matrix, vector (1,2,3)
    Eigen::Matrix3d m1 = Eigen::Matrix3d::Identity();
    Eigen::Vector3d v1(1, 2, 3);
    Eigen::Vector3d result1 = transformAndMultiply(m1, v1);
    // Transformed identity: diag(50*(1+1.2)) = diag(110), off-diagonal 60.
    // result = (110*1 + 60*2 + 60*3, 60*1 + 110*2 + 60*3, 60*1 + 60*2 + 110*3)
    // = (110+120+180, 60+220+180, 60+120+330) = (410, 460, 510)
    assert(result1 == Eigen::Vector3d(410, 460, 510));

    // Test 2: Zero matrix, vector (0,0,0)
    Eigen::Matrix3d m2 = Eigen::Matrix3d::Zero();
    Eigen::Vector3d v2(0, 0, 0);
    Eigen::Vector3d result2 = transformAndMultiply(m2, v2);
    // Transformed zero matrix: all entries = 60, vector zero -> result zero.
    assert(result2 == Eigen::Vector3d(0, 0, 0));

    // Test 3: All ones matrix, vector (1,1,1)
    Eigen::Matrix3d m3 = Eigen::Matrix3d::Ones();
    Eigen::Vector3d v3(1, 1, 1);
    Eigen::Vector3d result3 = transformAndMultiply(m3, v3);
    // Transformed ones matrix: each entry = (1 + 1.2)*50 = 110, sum of vector = 3, result each = 330.
    assert(result3 == Eigen::Vector3d(330, 330, 330));

    // Test 4: Random matrix, verify against manual calculation.
    Eigen::Matrix3d m4;
    m4 << 1, 2, 3,
          4, 5, 6,
          7, 8, 9;
    Eigen::Vector3d v4(2, -1, 0.5);
    Eigen::Vector3d result4 = transformAndMultiply(m4, v4);
    // Manually compute transformed matrix: (m4 + 1.2)*50 = 
    // (110, 160, 210) row1
    // (260, 310, 360) row2
    // (410, 460, 510) row3
    // Dot with v4: row1: 110*2 + 160*(-1) + 210*0.5 = 220 - 160 + 105 = 165
    // row2: 260*2 - 310 + 360*0.5 = 520 - 310 + 180 = 390
    // row3: 410*2 - 460 + 510*0.5 = 820 - 460 + 255 = 615
    assert(result4 == Eigen::Vector3d(165, 390, 615));

    // Test 5: Ensure input not modified (matrix remains unchanged).
    Eigen::Matrix3d original = m4;
    Eigen::Vector3d originalV = v4;
    transformAndMultiply(m4, v4);
    assert(m4 == original);
    assert(v4 == originalV);

    return 0;
}
// The solution needs to transform the input matrix elementwise: add a constant 1.2 to every element, then multiply the entire matrix by a scalar 50. In Eigen, adding a scalar to a matrix is elementwise via `matrix + Eigen::Matrix3d::Constant(1.2)` or simply `matrix + Eigen::Matrix3d::Constant(1.2)`, and multiplication by a scalar is elementwise. After building the transformed matrix, compute the matrix-vector product `transformed_matrix * vector`. The operation is straightforward: for each row of the matrix, compute the dot product with the vector. Since the matrix is 3x3 and vector is 3-dimensional, the result is a 3-dimensional vector. No edge cases arise because the dimensions are fixed at 3. Time complexity is O(1) because the size is constant (9 multiply-adds for transformation, then 3 dot products each with 3 multiplications and 2 additions). Space complexity is O(1) for the temporary transformed matrix and result vector. The function should take `const Eigen::Matrix3d&` and `const Eigen::Vector3d&` parameters to avoid copying and to preserve const correctness, and return `Eigen::Vector3d` by value. Include necessary headers `<Eigen/Dense>`.
