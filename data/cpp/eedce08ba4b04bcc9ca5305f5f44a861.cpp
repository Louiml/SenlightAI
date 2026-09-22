// Given a sequence of cross-track error measurements (doubles) and proportional, integral, and derivative coefficients (Kp, Ki, Kd), write a C++ function `std::vector<double> pidController(const std::vector<double>& cteHistory, double Kp, double Ki, double Kd)` that simulates a PID controller and returns a vector of control outputs for each measurement. The controller must maintain an integral error accumulated from the start, a proportional error equal to the current cte, and a derivative error equal to the difference between the current cte and the previous cte (for the first measurement, use the current cte as the previous value, so derivative is 0.0). The output for each step is `-(Kp * p_error + Kd * d_error + Ki * i_error)`. The input vector may be empty; in that case return an empty vector. Edge cases include negative coefficients, zero coefficients, and very large values; ensure no division or overflow issues beyond the natural double precision.

// The solution iterates over the input `cteHistory` in order. Maintain three state variables: `i_error` (integral, starts at 0.0), `p_error` (previous proportional, initialize to the first cte before the loop so that for the first element `d_error = cte - p_error = cte - cte = 0.0`), and `d_error` (derivative, updated each step as `cte - p_error`). For each cte, compute the output as `-(Kp * cte + Kd * d_error + Ki * i_error)` because `p_error` becomes the current cte. Then update `i_error += cte` and `p_error = cte` for the next iteration. If the input is empty, no state is needed, and the loop is skipped, returning an empty vector. Time complexity is O(n) where n is the number of measurements, and space complexity is O(n) for the output vector (which is required to hold the results). The algorithm is straightforward; edge cases like zero coefficients simply result in zero contribution from that term, and negative coefficients flip the sign of the contribution as expected.

#include <vector>

// Simulate a PID controller given a history of cross-track errors and gains.
// Returns a vector of control outputs for each measurement in order.
std::vector<double> pidController(const std::vector<double>& cteHistory, double Kp, double Ki, double Kd) {
    std::vector<double> outputs;
    if (cteHistory.empty()) {
        return outputs;
    }

    double i_error = 0.0;
    double p_error = cteHistory[0]; // previous cte, initially the first one
    double d_error = 0.0;

    for (double cte : cteHistory) {
        d_error = cte - p_error;      // derivative error
        p_error = cte;                // update proportional error to current cte
        i_error += cte;               // accumulate integral error

        double output = -(Kp * p_error + Kd * d_error + Ki * i_error);
        outputs.push_back(output);
    }

    return outputs;
}

#include <cassert>
#include <vector>

// The solution function is declared above, but we replicate it here for the test.
std::vector<double> pidController(const std::vector<double>& cteHistory, double Kp, double Ki, double Kd) {
    std::vector<double> outputs;
    if (cteHistory.empty()) {
        return outputs;
    }

    double i_error = 0.0;
    double p_error = cteHistory[0];
    double d_error = 0.0;

    for (double cte : cteHistory) {
        d_error = cte - p_error;
        p_error = cte;
        i_error += cte;
        outputs.push_back(-(Kp * p_error + Kd * d_error + Ki * i_error));
    }
    return outputs;
}

int main() {
    // Test 1: Basic case with non-zero gains
    std::vector<double> cte1 = {1.0, 2.0, 3.0};
    std::vector<double> out1 = pidController(cte1, 1.0, 0.5, 0.1);
    assert(out1.size() == 3);
    // Step 1: d_error=0, p_error=1, i_error=1 -> -(1*1 + 0.1*0 + 0.5*1) = -1.5
    assert(out1[0] == -1.5);
    // Step 2: d_error=2-1=1, p_error=2, i_error=1+2=3 -> -(1*2 + 0.1*1 + 0.5*3) = -(2+0.1+1.5) = -3.6
    assert(out1[1] == -3.6);
    // Step 3: d_error=3-2=1, p_error=3, i_error=3+3=6 -> -(1*3 + 0.1*1 + 0.5*6) = -(3+0.1+3) = -6.1
    assert(out1[2] == -6.1);

    // Test 2: Empty input
    std::vector<double> cte2 = {};
    std::vector<double> out2 = pidController(cte2, 1.0, 1.0, 1.0);
    assert(out2.empty());

    // Test 3: Single measurement, derivative zero
    std::vector<double> cte3 = {5.0};
    std::vector<double> out3 = pidController(cte3, 2.0, 1.0, 3.0);
    assert(out3.size() == 1);
    // p=5, d=0, i=5 -> -(2*5 + 3*0 + 1*5) = -15
    assert(out3[0] == -15.0);

    // Test 4: All zero coefficients -> outputs all zero
    std::vector<double> cte4 = {1.0, -1.0, 2.0};
    std::vector<double> out4 = pidController(cte4, 0.0, 0.0, 0.0);
    for (double val : out4) {
        assert(val == 0.0);
    }

    // Test 5: Negative coefficients and negative cte
    std::vector<double> cte5 = {-1.0, -2.0};
    std::vector<double> out5 = pidController(cte5, -1.0, 0.0, -0.5);
    // Step1: p=-1, d=0, i=-1 -> -(-1*-1 + 0 + 0) = -1? Actually: -(Kp*p + Kd*d + Ki*i) = -((-1)*(-1) + (-0.5)*0 + 0*(-1)) = -(1+0+0) = -1
    assert(out5[0] == -1.0);
    // Step2: p=-2, d=-2-(-1)=-1, i=-1-2=-3 -> -((-1)*(-2) + (-0.5)*(-1) + 0*(-3)) = -(2 + 0.5 + 0) = -2.5
    assert(out5[1] == -2.5);

    // Test 6: Large values, check no crash and reasonable output
    std::vector<double> cte6 = {1e10, -1e10};
    std::vector<double> out6 = pidController(cte6, 1.0, 0.0, 0.0);
    assert(out6.size() == 2);
    // Step1: output = -1e10
    assert(out6[0] == -1e10);
    // Step2: p=-1e10, d=-1e10 - 1e10 = -2e10 -> output = -1*(-1e10) = 1e10? Wait: -(1*(-1e10) + 0 + 0) = 1e10
    assert(out6[1] == 1e10);

    return 0;
}
