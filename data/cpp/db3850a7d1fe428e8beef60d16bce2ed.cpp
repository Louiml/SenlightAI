/*
Implement a C++ function named `batchGradientDescent` that performs logistic regression using gradient descent. The function takes a feature matrix `X` (each row is a sample, each column is a feature), a binary label vector `y` (containing 0 or 1), an initial coefficient vector `beta`, a learning rate `alpha`, and a number of iterations `niter`. It must return the updated `beta` after applying the iterative update rule: `beta = beta + alpha * (X^T * (y - logistic(X * beta)) / n)`, where `n` is the number of samples, and the logistic function is applied element-wise as `1 / (1 + exp(-z))`. The implementation must use the Eigen library for vector/matrix operations and the `exp` function from `<cmath>` applied element-wise via `.array()`. Handle the edge case where `X` has zero rows by returning the initial `beta` unchanged, and ensure all operations are numerically stable (e.g., avoid overflow in `exp` for large negative inputs, which is naturally handled by Eigen's array operations). The function must be `const`-correct where appropriate and include necessary headers.
*/

#include <Eigen/Dense>
#include <cmath>

// Apply the logistic function element-wise to a vector.
Eigen::VectorXd logistic(const Eigen::VectorXd& x) {
    return 1.0 / (1.0 + (-x).array().exp());
}

// Perform batch gradient descent for logistic regression.
Eigen::VectorXd batchGradientDescent(const Eigen::MatrixXd& X,
                                     const Eigen::VectorXd& y,
                                     const Eigen::VectorXd& beta,
                                     double alpha,
                                     int niter) {
    const int n = X.rows();
    if (n == 0) {
        return beta;  // avoid division by zero
    }

    Eigen::VectorXd current_beta = beta;
    const Eigen::MatrixXd Xt = X.transpose();

    for (int i = 0; i < niter; ++i) {
        Eigen::VectorXd y_pred = logistic(X * current_beta);
        Eigen::VectorXd resid = y - y_pred;
        Eigen::VectorXd grad = Xt * resid / n;
        current_beta += alpha * grad;
    }

    return current_beta;
}

#include <cassert>
#include <cmath>
#include <Eigen/Dense>

// Include the solution function here (or link to it).
// For the test, we copy the solution code above.

int main() {
    // Test 1: Single sample, single feature, beta initialized to zero.
    Eigen::MatrixXd X1(1, 1);
    X1 << 1.0;
    Eigen::VectorXd y1(1);
    y1 << 1.0;
    Eigen::VectorXd beta1(1);
    beta1 << 0.0;
    Eigen::VectorXd result1 = batchGradientDescent(X1, y1, beta1, 0.1, 1);
    // Expected: beta = 0 + 0.1 * (X^T * (y - logistic(X*0)) / 1) = 0.1 * (1 - 0.5) = 0.05
    assert(std::abs(result1(0) - 0.05) < 1e-12);

    // Test 2: Two samples, one feature, multiple iterations.
    Eigen::MatrixXd X2(2, 1);
    X2 << 1.0, -1.0;
    Eigen::VectorXd y2(2);
    y2 << 1.0, 0.0;
    Eigen::VectorXd beta2(1);
    beta2 << 0.0;
    Eigen::VectorXd result2 = batchGradientDescent(X2, y2, beta2, 0.5, 2);
    // Iteration 1: y_pred = [0.5, 0.5], resid = [0.5, -0.5], grad = (1*0.5 + -1*-0.5)/2 = 0.5, beta = 0.25
    // Iteration 2: y_pred = logistic(0.25) and logistic(-0.25) = ~0.5622, 0.4378, resid = [0.4378, -0.4378], grad = (1*0.4378 + -1*-0.4378)/2 = 0.4378, beta = 0.25 + 0.5*0.4378 = 0.4689
    double expected2 = 0.25 + 0.5 * ( (1 * (1 - 1.0/(1.0+std::exp(-0.25)))) + (-1 * (0 - 1.0/(1.0+std::exp(0.25)))) ) / 2.0;
    assert(std::abs(result2(0) - expected2) < 1e-10);

    // Test 3: Edge case: zero rows.
    Eigen::MatrixXd X3(0, 2);
    Eigen::VectorXd y3(0);
    Eigen::VectorXd beta3(2);
    beta3 << 1.0, 2.0;
    Eigen::VectorXd result3 = batchGradientDescent(X3, y3, beta3, 0.1, 5);
    assert(result3.isApprox(beta3));

    // Test 4: All labels zero, beta stays zero (since gradient is zero for symmetric predictions).
    Eigen::MatrixXd X4(2, 1);
    X4 << 1.0, 1.0;
    Eigen::VectorXd y4(2);
    y4 << 0.0, 0.0;
    Eigen::VectorXd beta4(1);
    beta4 << 0.0;
    Eigen::VectorXd result4 = batchGradientDescent(X4, y4, beta4, 0.1, 10);
    // Initial prediction 0.5, resid = -0.5, grad = (1*-0.5 + 1*-0.5)/2 = -0.5, beta becomes -0.05, etc. So not zero; but we can check it's negative.
    assert(result4(0) < 0.0);

    // Test 5: Large negative input, no overflow in logistic.
    Eigen::MatrixXd X5(1, 1);
    X5 << -1000.0;
    Eigen::VectorXd y5(1);
    y5 << 0.0;
    Eigen::VectorXd beta5(1);
    beta5 << 1.0;
    Eigen::VectorXd result5 = batchGradientDescent(X5, y5, beta5, 0.01, 1);
    // logistic(-1000) ≈ 0, resid ≈ 0, beta unchanged.
    assert(std::abs(result5(0) - 1.0) < 1e-12);

    // Test 6: Multiple features.
    Eigen::MatrixXd X6(3, 2);
    X6 << 1.0, 2.0,
          3.0, 4.0,
          5.0, 6.0;
    Eigen::VectorXd y6(3);
    y6 << 1.0, 0.0, 1.0;
    Eigen::VectorXd beta6(2);
    beta6 << 0.1, -0.1;
    Eigen::VectorXd result6 = batchGradientDescent(X6, y6, beta6, 0.01, 3);
    // Compute expected manually from the same update formula.
    Eigen::VectorXd cur = beta6;
    for (int it = 0; it < 3; ++it) {
        Eigen::VectorXd pred = 1.0 / (1.0 + (-(X6 * cur)).array().exp());
        Eigen::VectorXd resid = y6 - pred;
        cur = cur + 0.01 * (X6.transpose() * resid) / 3.0;
    }
    assert(result6.isApprox(cur, 1e-10));

    return 0;
}

// The main algorithm is batch gradient descent for logistic regression. For each iteration, compute the predicted probabilities using the logistic function applied to the linear combination `X * beta`. The residual is the difference between the true labels `y` and predictions. The gradient is `X^T * resid / n`, and `beta` is updated by adding `alpha * grad`. The logistic function must be applied element-wise: use `(-x).array().exp()` within the denominator to avoid issues with large negative values. Edge cases: if `n == 0`, the division by zero would occur, so return `beta` directly. Time complexity is `O(niter * n * p)` where `p` is the number of features (because each iteration does a matrix-vector product and a transpose product). Space complexity is `O(n + p)` for intermediate vectors (prediction, residual, gradient), plus the input matrices. Ensure the input matrix `X` and vector `y` are passed as `const` references to avoid copies, and return the result by value. The function should be declared in a header-like manner but can be self-contained in a single file without `main`.
