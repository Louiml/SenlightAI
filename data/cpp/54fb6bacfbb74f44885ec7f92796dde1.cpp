Implement a standalone C++ function that simulates a simplified PID controller for a velocity control system. Given a goal velocity, a current velocity, and a history of previous states, the function must compute the control output using the discrete PID formula: `output = kp * error + ki * integral + kd * derivative`, where `error = goal - current`, `integral` accumulates `error * dt`, and `derivative = (error - prev_error) / dt`. The function should take as parameters: the goal velocity, the current velocity, the time step `dt`, the gains `kp`, `ki`, `kd`, and references to `prev_error` and `integral` (which must be updated each call). The function must return the computed output as a `double`. All parameters except the gain constants must be modifiable. Edge cases to handle: when `dt` is zero or negative, return 0.0 and do not update state; when `prev_error` is uninitialized (indicated by a separate boolean flag), initialize it to the current error and treat derivative as zero. The function must be free of ROS and any external dependencies, using only standard C++.

// The core algorithm is a discrete-time PID controller computation. Each call processes one time step. First, validate that `dt > 0`; if not, return 0.0 to avoid division by zero or nonsensical updates. Compute `error = goal - current`. If the controller is in its initial state (`is_first` flag true), set `prev_error = error` and set derivative to 0.0 (since there is no previous error to compare). Otherwise, compute `derivative = (error - prev_error) / dt`. Then accumulate `integral += error * dt`. Compute `output = kp * error + ki * integral + kd * derivative`. Finally, update `prev_error = error` and clear the `is_first` flag. The integral can accumulate unboundedly; if overflow or extreme values are a concern, clamping could be added, but the specification does not require it. Time complexity is O(1) per call, space complexity is O(1) (only references and local variables). The main edge case is non-positive `dt`, which must be handled gracefully without corrupting state.

#include <cmath>   // for std::isnan maybe, but not required
#include <cstddef> // for size_t, not needed

// Compute a PID control output for a velocity regulation system.
// Parameters:
//   goal       - desired velocity
//   current    - current velocity
//   dt         - time step in seconds (must be positive)
//   kp, ki, kd - PID gains
//   prev_error - reference to previous error (updated each call)
//   integral   - reference to accumulated integral (updated each call)
//   is_first   - reference to a bool flag indicating the first call
// Returns:
//   the control output, or 0.0 if dt <= 0 (state unchanged)
double pidVelocityControl(double goal, double current, double dt,
                          double kp, double ki, double kd,
                          double& prev_error, double& integral,
                          bool& is_first) {
    if (dt <= 0.0) {
        return 0.0; // invalid time step, do not update state
    }

    double error = goal - current;
    double derivative;

    if (is_first) {
        derivative = 0.0; // no previous error, derivative is zero
        prev_error = error;
        is_first = false;
    } else {
        derivative = (error - prev_error) / dt;
    }

    integral += error * dt;
    double output = kp * error + ki * integral + kd * derivative;

    prev_error = error;
    return output;
}

#include <cassert>
#include <cmath>

// Test helper to compare doubles with tolerance
bool approxEqual(double a, double b, double epsilon = 1e-9) {
    return std::fabs(a - b) < epsilon;
}

int main() {
    // Test 1: First call, derivative zero, integral accumulates
    double prev_err = 0.0, integral = 0.0;
    bool first = true;
    double out1 = pidVelocityControl(10.0, 5.0, 0.1, 1.0, 0.5, 0.1, prev_err, integral, first);
    assert(approxEqual(out1, 1.0*5.0 + 0.5*0.5 + 0.1*0.0)); // 5 + 0.25 + 0 = 5.25
    assert(approxEqual(integral, 0.5));
    assert(approxEqual(prev_err, 5.0));
    assert(!first);

    // Test 2: Second call, derivative computed from previous error
    double out2 = pidVelocityControl(10.0, 7.0, 0.1, 1.0, 0.5, 0.1, prev_err, integral, first);
    // error = 3, derivative = (3 - 5)/0.1 = -20, integral += 0.3 => 0.8
    assert(approxEqual(out2, 1.0*3.0 + 0.5*0.8 + 0.1*(-20.0))); // 3 + 0.4 - 2 = 1.4
    assert(approxEqual(integral, 0.8));
    assert(approxEqual(prev_err, 3.0));

    // Test 3: Non-positive dt returns 0 and does not modify state
    double prev_before = prev_err, integral_before = integral;
    bool first_before = first;
    double out3 = pidVelocityControl(10.0, 9.0, 0.0, 1.0, 1.0, 1.0, prev_err, integral, first);
    assert(out3 == 0.0);
    assert(prev_err == prev_before);
    assert(integral == integral_before);
    assert(first == first_before);

    // Test 4: Negative dt also returns 0 and no state change
    double out4 = pidVelocityControl(10.0, 8.0, -0.1, 1.0, 1.0, 1.0, prev_err, integral, first);
    assert(out4 == 0.0);
    assert(prev_err == prev_before);
    assert(integral == integral_before);

    // Test 5: Large gains and no error (goal == current) yields integral accumulation
    prev_err = 0.0; integral = 0.0; first = true;
    double out5 = pidVelocityControl(5.0, 5.0, 1.0, 10.0, 5.0, 2.0, prev_err, integral, first);
    // error = 0, derivative = 0, integral = 0, output = 0
    assert(out5 == 0.0);
    assert(integral == 0.0);
    assert(prev_err == 0.0);

    // Test 6: Sequential calls with goal change and steady state
    prev_err = 0.0; integral = 0.0; first = true;
    double a1 = pidVelocityControl(100.0, 90.0, 0.5, 2.0, 1.0, 0.5, prev_err, integral, first);
    // error=10, derivative=0, integral=5, output = 2*10 + 1*5 + 0 = 25
    assert(approxEqual(a1, 25.0));
    double a2 = pidVelocityControl(100.0, 95.0, 0.5, 2.0, 1.0, 0.5, prev_err, integral, first);
    // error=5, derivative=(5-10)/0.5=-10, integral+=2.5=>7.5, output=2*5+7.5+0.5*(-10)=10+7.5-5=12.5
    assert(approxEqual(a2, 12.5));
    assert(approxEqual(integral, 7.5));
    assert(approxEqual(prev_err, 5.0));

    // Test 7: Very small dt (epsilon positive) works without division by zero
    prev_err = 0.0; integral = 0.0; first = true;
    double b1 = pidVelocityControl(1.0, 0.0, 1e-12, 1.0, 1.0, 1.0, prev_err, integral, first);
    // error=1, integral=1e-12, output=1+1e-12 ≈ 1
    assert(approxEqual(b1, 1.0 + 1e-12));

    // Test 8: Initial derivative zero confirmed with large kd
    prev_err = 0.0; integral = 0.0; first = true;
    double out8 = pidVelocityControl(10.0, 0.0, 0.2, 0.0, 0.0, 1000.0, prev_err, integral, first);
    assert(out8 == 0.0); // kd doesn't affect first call

    // Test 9: Steady state with zero error and derivative, integral persists
    prev_err = 0.0; integral = 0.0; first = true;
    // call 1: error=2, integral=0.2, output = 0*2 + 0*0.2 + 0*0 = 0? but ki=0 so output 0
    double c1 = pidVelocityControl(2.0, 0.0, 0.1, 0.0, 5.0, 0.0, prev_err, integral, first);
    assert(approxEqual(c1, 5.0 * 0.2)); // 1.0
    // call 2: error=0, integral=0.2, output = 5*0.2=1.0
    double c2 = pidVelocityControl(2.0, 2.0, 0.1, 0.0, 5.0, 0.0, prev_err, integral, first);
    assert(approxEqual(c2, 1.0));
    assert(approxEqual(integral, 0.2));

    // Test 10: Oscillating error gives deterministic derivative sign
    prev_err = 0.0; integral = 0.0; first = true;
    double d1 = pidVelocityControl(0.0, 1.0, 1.0, 0.0, 0.0, 2.0, prev_err, integral, first); // first: derivative 0
    assert(d1 == 0.0);
    double d2 = pidVelocityControl(0.0, -1.0, 1.0, 0.0, 0.0, 2.0, prev_err, integral, first);
    // error = 1, derivative = (1 - (-1))/1? wait error = goal - current = 0 - (-1)=1, prev_error was? after first call prev_error = -1 (since error = 0-1=-1)
    // Let's recompute: first call: goal=0, current=1, error=-1, prev_error set to -1. Second call: goal=0, current=-1, error=1, derivative=(1 - (-1))/1 = 2. output = 0 + 0 + 2*2=4
    assert(approxEqual(d2, 4.0));

    return 0;
}
