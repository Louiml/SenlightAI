// Implement a C++ function `predictUpdate` that performs one complete Kalman filter cycle for a 1D system: first predict, then update, given the state estimate `x`, state covariance `P`, transition matrix `F`, process noise covariance `Q`, measurement `z`, measurement matrix `H`, and measurement noise covariance `R`. All inputs are `double` values representing scalar quantities (so matrices are 1x1). The function should return a `std::pair<double, double>` containing the updated state estimate `x` and updated covariance `P` after the predict and update steps, in that order. The filter equations are: Predict: `x = F*x`, `P = F*P*F + Q`; Update: `S = H*P*H + R`, `K = P*H / S`, `x = x + K*(z - H*x)`, `P = (1 - K*H)*P`. For the scalar case, division is safe as long as `S` is non-zero (i.e., `H*P*H + R > 0`). Assume all inputs are finite and `R > 0`, so no division-by-zero occurs. The function must be `const`-correct (i.e., take inputs by value or const reference, not modify them) and be implemented as a free function in a header-only style.
The problem is a direct scalar implementation of the Kalman filter equations. Since all quantities are scalars, we avoid matrix operations entirely. The steps are:
1. **Predict**: Update the state estimate using the transition model: `x_pred = F * x`. Update the covariance using the process noise: `P_pred = F * F * P + Q` (since `F` is scalar, `F*P*F` = `F^2 * P`).
2. **Update**: Compute the innovation (difference between measurement and predicted measurement): `y = z - H * x_pred`. Compute the innovation covariance: `S = H * H * P_pred + R`. Compute the Kalman gain: `K = P_pred * H / S`. Update the state: `x_new = x_pred + K * y`. Update the covariance: `P_new = (1 - K * H) * P_pred`.
3. Return `{x_new, P_new}`.

Edge cases: The only risk is division by zero if `S == 0`, but since `R > 0`, `S >= R > 0` (because `H*H*P_pred` is non-negative as `P_pred` is non-negative). Thus safe. The solution must not modify inputs; we use local variables. Time complexity is O(1) (constant), space complexity O(1). The implementation should be clean, with comments explaining each step.
#include <utility> // for std::pair

// Perform one Kalman filter predict-update cycle for a 1D system.
// Returns (updated state, updated covariance) after both predict and update.
std::pair<double, double> predictUpdate(double x, double P, double F, double Q,
                                        double z, double H, double R) {
    // --- Predict step ---
    // State prediction: x_pred = F * x
    double x_pred = F * x;
    // Covariance prediction: P_pred = F * P * F + Q
    double P_pred = F * F * P + Q;

    // --- Update step ---
    // Innovation: difference between measurement and predicted measurement
    double y = z - H * x_pred;
    // Innovation covariance: S = H * P * H + R
    double S = H * H * P_pred + R;
    // Kalman gain: K = P * H / S
    double K = P_pred * H / S;
    // Updated state: x_new = x_pred + K * y
    double x_new = x_pred + K * y;
    // Updated covariance: P_new = (1 - K * H) * P
    double P_new = (1.0 - K * H) * P_pred;

    return {x_new, P_new};
}
#include <cassert>
#include <cmath>
#include <utility>

// (Solution function is assumed to be included above)

int main() {
    // Test 1: Simple identity system, no noise, perfect measurement
    // F=1, Q=0, H=1, R=0.1 (small measurement noise).
    // Predict: x'=1*1=1, P'=1*1*1+0=1
    // Update: S=1*1*1+0.1=1.1, K=1*1/1.1≈0.90909, y=2-1=1,
    // x_new=1+0.90909*1≈1.90909, P_new=(1-0.90909)*1≈0.09091
    auto result = predictUpdate(1.0, 1.0, 1.0, 0.0, 2.0, 1.0, 0.1);
    assert(std::abs(result.first - 1.9090909) < 1e-6);
    assert(std::abs(result.second - 0.0909091) < 1e-6);

    // Test 2: No measurement noise, identity system, measurement equals prediction
    // F=1, Q=0, H=1, R=0 → S=1, K=1, y=0 → x_new=1, P_new=0
    result = predictUpdate(1.0, 1.0, 1.0, 0.0, 1.0, 1.0, 0.0);
    assert(result.first == 1.0);
    assert(result.second == 0.0);

    // Test 3: State scale factor 2, no noise, measurement matches scaled prediction
    // x=3, P=0.5, F=2, Q=0 → x_pred=6, P_pred=4*0.5=2
    // H=1, R=0 → S=2, K=2*1/2=1, y=6-6=0 → x_new=6, P_new=0
    result = predictUpdate(3.0, 0.5, 2.0, 0.0, 6.0, 1.0, 0.0);
    assert(result.first == 6.0);
    assert(result.second == 0.0);

    // Test 4: Non-identity H, no measurement noise, update corrects
    // x=1, P=1, F=1, Q=0, H=2, R=0 → x_pred=1, P_pred=1
    // S=4*1=4, K=1*2/4=0.5, y=4-2=2 → x_new=1+0.5*2=2, P_new=(1-0.5*2)*1=0
    result = predictUpdate(1.0, 1.0, 1.0, 0.0, 4.0, 2.0, 0.0);
    assert(result.first == 2.0);
    assert(result.second == 0.0);

    // Test 5: Large process noise, small measurement noise, moderate update
    // To keep exact simple: x=0, P=1, F=1, Q=3, H=1, R=1 → predict: x_pred=0, P_pred=4
    // Update: S=4+1=5, K=4/5=0.8, y=10-0=10 → x_new=8, P_new=(1-0.8)*4=0.8
    result = predictUpdate(0.0, 1.0, 1.0, 3.0, 10.0, 1.0, 1.0);
    assert(std::abs(result.first - 8.0) < 1e-12);
    assert(std::abs(result.second - 0.8) < 1e-12);

    // Test 6: Negative measurement noise? Not allowed, but check R>0 ensures S>0
    // Already covered. Test with random-ish values to ensure no crash: use zero P, Q, H
    result = predictUpdate(5.0, 0.0, 1.0, 0.0, 7.0, 0.0, 1.0);
    // Predict: x=5, P=0. Update: S=0+1=1, K=0, y=7-0=7 → x_new=5, P_new=0
    assert(result.first == 5.0);
    assert(result.second == 0.0);
}
