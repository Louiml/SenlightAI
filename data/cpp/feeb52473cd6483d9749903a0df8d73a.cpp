// Write a standalone C++ function that takes a fixed-size `Eigen::Matrix4d` (a 4x4 double-precision matrix) and returns a new `Eigen::Matrix4d` where every element has been passed through a hard rectifier (ReLU) activation: any negative value becomes 0, any non-negative value remains unchanged. The function must not modify the input matrix and must use Eigen's coefficient-wise operations (specifically `unaryExpr` with a lambda or function pointer). Ensure the function is const-correct and works for any possible 4x4 input, including all zeros, all negatives, and mixed values.
// The core idea is to apply a unary function to each element of the input matrix. Eigen provides `unaryExpr` for exactly this purpose, which accepts a callable (lambda or function pointer) and returns a new matrix of the same size and type. We define a local helper (either a lambda inside the function or a static free function) that implements the ReLU: return `x` if `x > 0`, else `0`. Since we want the input unchanged, the parameter is `const Matrix4d&`, and we return a `Matrix4d` by value. The operation is element-wise, so it is trivially parallelizable, has O(16) time complexity (since the matrix size is fixed), and uses O(1) extra space beyond the returned matrix. Edge cases: exactly 0 should map to 0 (since `x > 0` is false, returns 0); negative numbers become 0; positive numbers stay. The lambda must capture nothing and take a `double` argument.
#include <Eigen/Core>

// Apply element-wise ReLU (rectified linear unit) to a 4x4 double matrix.
// Returns a new matrix where each element is max(x, 0).
Eigen::Matrix4d applyReLU(const Eigen::Matrix4d& input) {
    // Use unaryExpr with a lambda that returns 0 for non-positive values.
    return input.unaryExpr([](double x) -> double {
        return x > 0.0 ? x : 0.0;
    });
}
#include <Eigen/Core>
#include <cassert>

// Include the solution function (assuming it is in the same translation unit)
Eigen::Matrix4d applyReLU(const Eigen::Matrix4d& input);

int main() {
    // Test 1: Mixed values
    Eigen::Matrix4d m1;
    m1 << 1.0, -2.0, 3.0, -4.0,
          0.0, 5.0, -6.0, 7.0,
         -8.0, 9.0, -10.0, 11.0,
          12.0, -13.0, 14.0, -15.0;
    Eigen::Matrix4d result1 = applyReLU(m1);
    Eigen::Matrix4d expected1;
    expected1 << 1.0, 0.0, 3.0, 0.0,
                 0.0, 5.0, 0.0, 7.0,
                 0.0, 9.0, 0.0, 11.0,
                 12.0, 0.0, 14.0, 0.0;
    assert(result1 == expected1);

    // Test 2: All negative values
    Eigen::Matrix4d m2 = Eigen::Matrix4d::Constant(-3.5);
    Eigen::Matrix4d result2 = applyReLU(m2);
    assert(result2 == Eigen::Matrix4d::Zero());

    // Test 3: All positive values (including zero? check zero separately)
    Eigen::Matrix4d m3 = Eigen::Matrix4d::Constant(2.5);
    Eigen::Matrix4d result3 = applyReLU(m3);
    assert(result3 == m3);

    // Test 4: All zeros
    Eigen::Matrix4d m4 = Eigen::Matrix4d::Zero();
    Eigen::Matrix4d result4 = applyReLU(m4);
    assert(result4 == Eigen::Matrix4d::Zero());

    // Test 5: Verify input not modified (const-correctness)
    Eigen::Matrix4d m5;
    m5 << -1.0, -2.0, -3.0, -4.0,
         -5.0, -6.0, -7.0, -8.0,
         -9.0, -10.0, -11.0, -12.0,
         -13.0, -14.0, -15.0, -16.0;
    Eigen::Matrix4d copy = m5;
    applyReLU(m5);
    assert(m5 == copy); // unchanged

    // Test 6: Include positive and negative infinity (if supported)
    Eigen::Matrix4d m6;
    m6 << 1e308, -1e308, 0.0, 1.0,
         -0.0001, 0.0001, -0.0, -0.0,
         3.14, -3.14, 2.71, -2.71,
         1.0, -1.0, 0.0, -0.0;
    Eigen::Matrix4d result6 = applyReLU(m6);
    assert(result6(0,0) == 1e308);
    assert(result6(0,1) == 0.0);
    assert(result6(0,2) == 0.0);
    assert(result6(1,0) == 0.0);
    assert(result6(1,1) == 0.0001);
    assert(result6(1,2) == 0.0);

    return 0;
}
