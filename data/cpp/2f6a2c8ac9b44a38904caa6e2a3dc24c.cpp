Write a C++ function `computeCombination(const Eigen::MatrixXf& m, const Eigen::MatrixXf& n) -> Eigen::MatrixXf` that returns a new matrix equal to `((m.array() * n.array()).matrix() * m)`, i.e., the element-wise (Hadamard) product of `m` and `n`, then multiplied on the right by `m`. The matrices `m` and `n` are square and of the same size (both `size x size`). The function must not modify the inputs and must be `const`-correct. It should use Eigen's array and matrix views to achieve the computation in a single expression. The returned matrix should have the same dimensions and type as `m`. You may assume both matrices are non-empty and square, but handle the case where dimensions mismatch by throwing a `std::invalid_argument` with message `"Matrix dimensions mismatch for element-wise product"`.
#include <Eigen/Dense>
#include <cassert>
#include <stdexcept>

// Declaration of the function under test (from solution).
Eigen::MatrixXf computeCombination(const Eigen::MatrixXf& m, const Eigen::MatrixXf& n);

int main() {
    // Test 1: 2x2 example from the snippet.
    Eigen::MatrixXf m(2,2);
    m << 1, 2,
         3, 4;
    Eigen::MatrixXf n(2,2);
    n << 5, 6,
         7, 8;
    Eigen::MatrixXf expected(2,2);
    expected << 5, 12,
                21, 32; // element-wise: 5,12,21,32; times m: row1: 5*1+12*3=41? Wait compute properly.
    // Let's compute manually: element-wise product = [5,12;21,32]; multiply by m = [1,2;3,4]:
    // result(0,0) = 5*1 + 12*3 = 5+36=41; result(0,1)=5*2+12*4=10+48=58;
    // result(1,0)=21*1+32*3=21+96=117; result(1,1)=21*2+32*4=42+128=170.
    Eigen::MatrixXf expected_correct(2,2);
    expected_correct << 41, 58,
                        117, 170;
    assert(computeCombination(m, n) == expected_correct);

    // Test 2: 3x3 identity-like.
    Eigen::MatrixXf a(3,3);
    a << 1, 0, 0,
         0, 2, 0,
         0, 0, 3;
    Eigen::MatrixXf b(3,3);
    b << 2, 0, 0,
         0, 3, 0,
         0, 0, 4;
    // Element-wise: [2,0,0; 0,6,0; 0,0,12]; times a:
    // result(i,j) = sum_k (elem(i,k)*a(k,j)). Since elem is diagonal, result = elem(i,i)*a(i,j) for each row.
    // row0: [2*1,2*0,2*0] = [2,0,0]; row1: [0,6*2=12,0]; row2: [0,0,12*3=36]
    Eigen::MatrixXf expected3(3,3);
    expected3 << 2,0,0,
                 0,12,0,
                 0,0,36;
    assert(computeCombination(a, b) == expected3);

    // Test 3: 1x1 matrices.
    Eigen::MatrixXf c(1,1);
    c << 4;
    Eigen::MatrixXf d(1,1);
    d << 5;
    Eigen::MatrixXf expected1(1,1);
    expected1 << 4*5*4; // element-wise=20, times m=4 => 80
    assert(computeCombination(c, d) == expected1);

    // Test 4: dimension mismatch throws.
    Eigen::MatrixXf e(2,3);
    Eigen::MatrixXf f(3,2);
    bool threw = false;
    try {
        computeCombination(e, f);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    // Test 5: verify const correctness (inputs unchanged).
    Eigen::MatrixXf original_m = m;
    Eigen::MatrixXf original_n = n;
    computeCombination(m, n);
    assert(m == original_m);
    assert(n == original_n);

    // Test 6: zero matrix.
    Eigen::MatrixXf z(2,2);
    z << 0,0,
         0,0;
    Eigen::MatrixXf identity(2,2);
    identity << 1,0,
                0,1;
    Eigen::MatrixXf zero_result(2,2);
    zero_result << 0,0,
                   0,0;
    assert(computeCombination(z, identity) == zero_result);
    assert(computeCombination(identity, z) == zero_result);
}
#include <Eigen/Dense>
#include <stdexcept>

// Returns ((m ∘ n) * m) where ∘ denotes element-wise (Hadamard) product.
// Throws std::invalid_argument if m and n have different dimensions.
Eigen::MatrixXf computeCombination(const Eigen::MatrixXf& m, const Eigen::MatrixXf& n) {
    if (m.rows() != n.rows() || m.cols() != n.cols()) {
        throw std::invalid_argument("Matrix dimensions mismatch for element-wise product");
    }
    // Element-wise product via .array(), then convert to matrix and multiply by m.
    return (m.array() * n.array()).matrix() * m;
}
// The solution uses Eigen's `array()` method to perform element-wise multiplication and `matrix()` to convert back for matrix multiplication. The main algorithm is: first verify that `m` and `n` have identical dimensions; if not, throw `std::invalid_argument`. Then form the element-wise product `m.array() * n.array()` which yields an `ArrayXXf` of the same shape, convert that to a `MatrixXf` via `.matrix()`, and multiply on the right by `m` using `operator*`. The result is a new `MatrixXf` of dimension `size x size`. Edge cases: if the matrices are 1x1, the element-wise product and matrix multiplication reduce to scalar multiplication, which is handled naturally. Complexity: element-wise multiplication is O(size^2), matrix multiplication of size x size matrices is O(size^3) using standard Eigen implementation (though Eigen may use SIMD/blocking optimizations, asymptotic complexity remains cubic). Auxiliary space is O(size^2) for the temporary array and the result matrix.
