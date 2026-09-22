// Implement a standalone C++ function named `computeRMSE` that takes two `std::vector<Eigen::VectorXd>` arguments: `estimations` and `ground_truth`. The function must return an `Eigen::VectorXd` of size 4 representing the Root Mean Square Error (RMSE) between each corresponding pair of 4-dimensional vectors. If either vector is empty or their sizes differ, return a zero vector of size 4 (as a sentinel for invalid input). The computation should follow: for each index `i`, compute the residual `estimations[i] - ground_truth[i]`, square each component, sum these squared residuals across all `i`, then divide by the total number of elements, and finally take the square root of each component. Use `Eigen::VectorXd` operations and avoid raw loops where possible. The function must be `const`-correct with respect to its parameters, and must not modify the input vectors. Assume the inputs always contain vectors of size exactly 4.
// The approach is straightforward: first validate input by checking if `estimations` is empty or if its size differs from `ground_truth.size()`. If invalid, return a zero-initialized `Eigen::VectorXd` of size 4. Otherwise, initialize an accumulator vector `rmse` of size 4 to all zeros. Iterate over each index `i` from 0 to `estimations.size()-1`, compute the residual vector as the element-wise difference between `estimations[i]` and `ground_truth[i]`, then square each component using `.array() * .array()`, and add to the accumulator. After the loop, divide the accumulator by the number of elements (using scalar division) to get the mean squared error, then apply the square root component-wise using `.array().sqrt()`. Edge cases: input vectors with mismatched sizes or empty inputs must be handled without crashing, returning the zero vector as specified. Division by zero is avoided because we only divide when size > 0. Time complexity is \(O(n)\) where \(n\) is the number of estimation/ground-truth pairs, and space complexity is \(O(1)\) aside from the output vector and temporary residuals (which hold at most 4 doubles).
#include <vector>
#include <Eigen/Dense>

// Compute the Root Mean Square Error between two vectors of 4D state vectors.
// Returns a 4D vector with each component being the RMSE for that coordinate.
// If inputs are empty or mismatched in size, returns a zero vector of size 4.
Eigen::VectorXd computeRMSE(const std::vector<Eigen::VectorXd>& estimations,
                            const std::vector<Eigen::VectorXd>& ground_truth) {
    const int state_size = 4;
    Eigen::VectorXd rmse = Eigen::VectorXd::Zero(state_size);

    // Validate inputs: empty or size mismatch -> return zero vector
    if (estimations.empty() || estimations.size() != ground_truth.size()) {
        return rmse;
    }

    // Accumulate squared residuals
    for (std::size_t i = 0; i < estimations.size(); ++i) {
        Eigen::VectorXd residual = estimations[i] - ground_truth[i];
        residual = residual.array() * residual.array();  // square each component
        rmse += residual;
    }

    // Divide by number of samples, then take square root
    rmse = rmse / static_cast<double>(estimations.size());
    rmse = rmse.array().sqrt();

    return rmse;
}
#include <cassert>
#include <vector>
#include <Eigen/Dense>

// The solution function is declared here (must be visible to main)
Eigen::VectorXd computeRMSE(const std::vector<Eigen::VectorXd>& estimations,
                            const std::vector<Eigen::VectorXd>& ground_truth);

int main() {
    // Test 1: Simple case with identical estimates
    std::vector<Eigen::VectorXd> est1 = {Eigen::VectorXd::Constant(4, 1.0)};
    std::vector<Eigen::VectorXd> truth1 = {Eigen::VectorXd::Constant(4, 1.0)};
    Eigen::VectorXd rmse1 = computeRMSE(est1, truth1);
    assert(rmse1.isApprox(Eigen::VectorXd::Zero(4), 1e-9));

    // Test 2: Two samples with known RMSE
    std::vector<Eigen::VectorXd> est2 = {
        (Eigen::VectorXd(4) << 0, 0, 0, 0).finished(),
        (Eigen::VectorXd(4) << 2, 2, 2, 2).finished()
    };
    std::vector<Eigen::VectorXd> truth2 = {
        (Eigen::VectorXd(4) << 0, 0, 0, 0).finished(),
        (Eigen::VectorXd(4) << 0, 0, 0, 0).finished()
    };
    // Residuals: [0,0,0,0] and [2,2,2,2]; squared sum = [4,4,4,4]; mean = [2,2,2,2]; sqrt = sqrt(2) ≈ 1.41421356
    Eigen::VectorXd rmse2 = computeRMSE(est2, truth2);
    Eigen::VectorXd expected2 = Eigen::VectorXd::Constant(4, std::sqrt(2.0));
    assert(rmse2.isApprox(expected2, 1e-9));

    // Test 3: Empty input returns zero vector
    std::vector<Eigen::VectorXd> est3;
    std::vector<Eigen::VectorXd> truth3;
    Eigen::VectorXd rmse3 = computeRMSE(est3, truth3);
    assert(rmse3.isApprox(Eigen::VectorXd::Zero(4), 1e-9));

    // Test 4: Mismatched sizes returns zero vector
    std::vector<Eigen::VectorXd> est4 = {Eigen::VectorXd::Constant(4, 1.0)};
    std::vector<Eigen::VectorXd> truth4 = {
        Eigen::VectorXd::Constant(4, 0.0),
        Eigen::VectorXd::Constant(4, 0.0)
    };
    Eigen::VectorXd rmse4 = computeRMSE(est4, truth4);
    assert(rmse4.isApprox(Eigen::VectorXd::Zero(4), 1e-9));

    // Test 5: Mixed values, manually compute expected
    std::vector<Eigen::VectorXd> est5 = {
        (Eigen::VectorXd(4) << 1, 2, 3, 4).finished(),
        (Eigen::VectorXd(4) << 5, 6, 7, 8).finished()
    };
    std::vector<Eigen::VectorXd> truth5 = {
        (Eigen::VectorXd(4) << 1, 1, 1, 1).finished(),
        (Eigen::VectorXd(4) << 3, 4, 5, 6).finished()
    };
    // Residuals: [0,1,2,3] and [2,2,2,2]; squares: [0,1,4,9] and [4,4,4,4]; sum: [4,5,8,13]; mean: [2,2.5,4,6.5]; sqrt: [1.41421,1.58114,2.0,2.54951]
    Eigen::VectorXd rmse5 = computeRMSE(est5, truth5);
    Eigen::VectorXd expected5(4);
    expected5 << std::sqrt(2.0), std::sqrt(2.5), std::sqrt(4.0), std::sqrt(6.5);
    assert(rmse5.isApprox(expected5, 1e-9));

    // Test 6: All zero estimates and truth gives zero
    std::vector<Eigen::VectorXd> est6 = {Eigen::VectorXd::Zero(4)};
    std::vector<Eigen::VectorXd> truth6 = {Eigen::VectorXd::Zero(4)};
    Eigen::VectorXd rmse6 = computeRMSE(est6, truth6);
    assert(rmse6.isApprox(Eigen::VectorXd::Zero(4), 1e-9));

    // Test 7: Negative values (should square them)
    std::vector<Eigen::VectorXd> est7 = {(Eigen::VectorXd(4) << -2, -2, -2, -2).finished()};
    std::vector<Eigen::VectorXd> truth7 = {Eigen::VectorXd::Zero(4)};
    Eigen::VectorXd rmse7 = computeRMSE(est7, truth7);
    // Residual: [-2,-2,-2,-2]; squared: [4,4,4,4]; mean: [4,4,4,4]; sqrt: [2,2,2,2]
    assert(rmse7.isApprox(Eigen::VectorXd::Constant(4, 2.0), 1e-9));

    return 0;
}
