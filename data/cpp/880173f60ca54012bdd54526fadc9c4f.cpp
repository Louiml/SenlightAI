/*
Write a C++ function that accepts a 3x3 matrix `v` and a 3-element vector `h` of Householder coefficients (both using `double` precision), and returns the 3x3 matrix product of the three Householder reflections `H0 * H1 * H2`, where each reflection is defined as `H_i = I - h(i) * v_i * v_i^T` and the Householder vectors `v_i` are derived from the columns of `v` in a strictly lower-triangular fashion: `v_0 = (1, v(1,0), v(2,0))`, `v_1 = (0, 1, v(2,1))`, `v_2 = (0, 0, 1)`. The function must compute the result directly by constructing the three reflection matrices and multiplying them in order, without using any external linear algebra library (only standard C++ containers and `<cmath>`). Ensure the function is `const`-correct, takes inputs by `const` reference, and returns a `std::array<std::array<double,3>,3>` representing the resulting matrix row-major.
*/
#include <array>
#include <cstddef>

// Compute the product H0 * H1 * H2 of Householder reflections
// derived from the lower-triangular portions of matrix v and coefficients h.
std::array<std::array<double,3>,3> householderProduct(
    const std::array<std::array<double,3>,3>& v,
    const std::array<double,3>& h)
{
    // Helper lambda to build a Householder vector v_i.
    auto buildVector = [&](std::size_t i) -> std::array<double,3> {
        std::array<double,3> vec = {0.0, 0.0, 0.0};
        vec[i] = 1.0;
        for (std::size_t r = i+1; r < 3; ++r) {
            vec[r] = v[r][i];
        }
        return vec;
    };

    // Helper lambda to build the reflection matrix H_i = I - h_i * v_i * v_i^T.
    auto buildReflection = [&](std::size_t i) -> std::array<std::array<double,3>,3> {
        auto vec = buildVector(i);
        std::array<std::array<double,3>,3> H{};
        for (std::size_t r = 0; r < 3; ++r) {
            for (std::size_t c = 0; c < 3; ++c) {
                // Identity part
                H[r][c] = (r == c) ? 1.0 : 0.0;
                // Subtract h_i * v_i[r] * v_i[c]
                H[r][c] -= h[i] * vec[r] * vec[c];
            }
        }
        return H;
    };

    // Build the three reflection matrices.
    auto H0 = buildReflection(0);
    auto H1 = buildReflection(1);
    auto H2 = buildReflection(2);

    // Multiply H0 * H1 * H2.
    std::array<std::array<double,3>,3> temp{};
    // H0 * H1
    for (std::size_t r = 0; r < 3; ++r) {
        for (std::size_t c = 0; c < 3; ++c) {
            double sum = 0.0;
            for (std::size_t k = 0; k < 3; ++k) {
                sum += H0[r][k] * H1[k][c];
            }
            temp[r][c] = sum;
        }
    }
    // (H0*H1) * H2
    std::array<std::array<double,3>,3> result{};
    for (std::size_t r = 0; r < 3; ++r) {
        for (std::size_t c = 0; c < 3; ++c) {
            double sum = 0.0;
            for (std::size_t k = 0; k < 3; ++k) {
                sum += temp[r][k] * H2[k][c];
            }
            result[r][c] = sum;
        }
    }
    return result;
}
#include <cassert>
#include <cmath>
#include <array>
#include <iostream>

// Include the solution function definition here (or link it).

int main() {
    // Test 1: Identity-like v with zeros below diagonal, and arbitrary h.
    // For v as identity, v0=(1,0,0), v1=(0,1,0), v2=(0,0,1).
    // Then H_i = I - h_i * (unit vector outer product), which is diagonal with 1-h_i on the diagonal.
    std::array<std::array<double,3>,3> v1 = {{
        {1.0, 0.0, 0.0},
        {0.0, 1.0, 0.0},
        {0.0, 0.0, 1.0}
    }};
    std::array<double,3> h1 = {0.5, -0.2, 1.0};
    auto res1 = householderProduct(v1, h1);
    // Expected H0 has diag (0.5, 1, 1), H1 diag (1, 1.2, 1), H2 diag (1,1,0)
    // Product diag = (0.5*1*1, 1*1.2*1, 1*1*0) = (0.5, 1.2, 0)
    // Off-diagonals are zero.
    assert(std::fabs(res1[0][0] - 0.5) < 1e-12);
    assert(std::fabs(res1[1][1] - 1.2) < 1e-12);
    assert(std::fabs(res1[2][2] - 0.0) < 1e-12);
    assert(std::fabs(res1[0][1]) < 1e-12);
    assert(std::fabs(res1[1][0]) < 1e-12);
    assert(std::fabs(res1[2][1]) < 1e-12);

    // Test 2: All h = 0 => all reflections are identity, product is identity.
    std::array<std::array<double,3>,3> v2 = {{
        {1.0, 2.0, 3.0},
        {4.0, 5.0, 6.0},
        {7.0, 8.0, 9.0}
    }};
    std::array<double,3> h2 = {0.0, 0.0, 0.0};
    auto res2 = householderProduct(v2, h2);
    for (int r=0; r<3; ++r) {
        for (int c=0; c<3; ++c) {
            assert(std::fabs(res2[r][c] - (r==c ? 1.0 : 0.0)) < 1e-12);
        }
    }

    // Test 3: A simple non-trivial case manually computed.
    // v = [[1,0,0],[2,1,0],[3,4,1]], h = [1,1,1]
    // v0=(1,2,3), v1=(0,1,4), v2=(0,0,1)
    // H0 = I - v0*v0^T, H1 = I - v1*v1^T, H2 = I - v2*v2^T
    // Compute H0*v0 = v0 - v0*(v0^T v0) = v0 - 14*v0 = -13*v0
    // So H0*H1*H2 * v2 = H0*H1*(v2 - 1*v2) = H0*H1*0 = 0 (since v2^T v2=1)
    // Check that applying product to v2 yields zero vector.
    std::array<std::array<double,3>,3> v3 = {{
        {1.0, 0.0, 0.0},
        {2.0, 1.0, 0.0},
        {3.0, 4.0, 1.0}
    }};
    std::array<double,3> h3 = {1.0, 1.0, 1.0};
    auto res3 = householderProduct(v3, h3);
    // Multiply res3 by v2 column vector (0,0,1) to get third column of res3.
    // That column should be near zero.
    assert(std::fabs(res3[0][2]) < 1e-12);
    assert(std::fabs(res3[1][2]) < 1e-12);
    assert(std::fabs(res3[2][2]) < 1e-12);

    // Test 4: Product of reflections is orthogonal? Not necessarily with arbitrary h, but
    // if h=2, each reflection is involution (H^2=I). Product of three is not necessarily identity.
    // Just check that with h=2, each H is symmetric and its square is identity.
    // We'll check that the returned matrix when h=2 satisfies: (product)^T * product = I? No, not generally.
    // Instead just check a known simple case: v as identity, h=2 => each H_i = diag(-1 for the active axis, 1 others)
    // Product = diag(-1, -1, -1) = -I. Because H0 diag(-1,1,1), H1 diag(1,-1,1), H2 diag(1,1,-1) multiply to -I.
    std::array<double,3> h4 = {2.0, 2.0, 2.0};
    auto res4 = householderProduct(v1, h4);
    for (int r=0; r<3; ++r) {
        for (int c=0; c<3; ++c) {
            double expected = (r==c) ? -1.0 : 0.0;
            assert(std::fabs(res4[r][c] - expected) < 1e-12);
        }
    }

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
// The solution directly implements the mathematical definition. For each index `i` from 0 to 2, we build the corresponding Householder vector `v_i` from the input matrix `v`. The vector has exactly three entries, with zeros above the diagonal (entries before index `i`) and a 1 on the diagonal, and the remaining entries come from the column of `v`. Then we compute the outer product `v_i * v_i^T` (a 3x3 matrix where entry `(r,c)` is `v_i[r] * v_i[c]`). The Householder reflection is `H_i = I - h(i) * outer`, where `I` is the identity matrix. We then multiply the three matrices in order: result = `H0 * H1 * H2`. Matrix multiplication is standard triple-loop over rows, columns, and inner dimension. Edge cases: any real `double` values are acceptable; if `h(i)` is zero, the reflection reduces to the identity. The function does not need to check for numerical stability, as the task is purely algebraic. Time complexity: each reflection construction and multiplication is constant size (3x3), so the whole operation is O(1) with small constant factors. Space complexity: O(1) aside from the returned matrix.
