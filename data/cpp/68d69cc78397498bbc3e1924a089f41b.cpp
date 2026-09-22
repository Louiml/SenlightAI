Write a C++ function that simulates a proportional-derivative (PD) controller for a single error state. The function should accept four parameters: the current proportional gain, the derivative gain, the current measured value, and the target value. It must maintain a persistent (static) internal error accumulator that is updated each call as follows: compute the raw error as `target - currentVal`, add this to the previous accumulated error, clamp the new accumulated error to the range [-100, 100], and then compute the controller output as `proportionalGain * (target - currentVal) + derivativeGain * (previousAccumulatedError - newAccumulatedError)`. Finally, the function should update the stored accumulated error to the new clamped value and return the computed output. The function must be standalone (no classes), use appropriate `const` for read-only parameters, and handle the first call correctly by initializing the accumulated error to zero. Test for clamping, sign changes, and repeated calls.
#include <cassert>
#include <cmath>

int main() {
    // Reset the static state by calling the function once with same values? 
    // Since static persists, we need to design tests carefully. 
    // We'll assume the function is called sequentially; we'll test with known sequences.

    // Test 1: First call with zero gains => output 0, accumulator unchanged.
    float out1 = pdController(0.0f, 0.0f, 100.0f, 100.0f);
    assert(std::fabs(out1 - 0.0f) < 1e-6);

    // Test 2: Proportional only, target 10, current 5 => error 5, output 5*1=5.
    // After first call, accumulator = 0 + (10-5)=5, clamped to 5.
    float out2 = pdController(1.0f, 0.0f, 5.0f, 10.0f);
    assert(std::fabs(out2 - 5.0f) < 1e-6);

    // Test 3: Derivative only, with previous accumulator = 5 (from out2).
    // Now call with current 5, target 10 again: rawError=5, newAcc=5+5=10, clamped=10.
    // derivative term = 0 (since derivativeGain=0), but we test derivative separately.
    // Let's use derivative gain=1, proportional=0.
    // previous accumulator is 5 (from out2). rawError = 10-5=5, newAcc=5+5=10, clamped=10.
    // derivative term = 1*(5-10) = -5.
    float out3 = pdController(0.0f, 1.0f, 5.0f, 10.0f);
    assert(std::fabs(out3 - (-5.0f)) < 1e-6);

    // Test 4: Clamping high. Set target 200, current 0. Raw error = 200.
    // previous accumulator = 10 (from out3). newAcc = 10+200=210, clamp to 100.
    // proportional gain=1, derivative=0 => output = 1*200 = 200.
    float out4 = pdController(1.0f, 0.0f, 0.0f, 200.0f);
    assert(std::fabs(out4 - 200.0f) < 1e-6);
    // After this, accumulator = 100.

    // Test 5: Clamping low. Set target -200, current 0. Raw error = -200.
    // previous accumulator = 100. newAcc = 100 - 200 = -100, clamp to -100 (no change).
    // proportional=1, derivative=0 => output = -200.
    float out5 = pdController(1.0f, 0.0f, 0.0f, -200.0f);
    assert(std::fabs(out5 - (-200.0f)) < 1e-6);
    // Accumulator now = -100.

    // Test 6: Derivative with clamping. Now accumulator=-100, rawError = 50 (target 50, current 0).
    // newAcc = -100 + 50 = -50, clamped = -50. derivative term = 2 * (-100 - (-50)) = 2*(-50) = -100.
    float out6 = pdController(0.0f, 2.0f, 0.0f, 50.0f);
    assert(std::fabs(out6 - (-100.0f)) < 1e-6);

    // Test 7: Sign change. Now accumulator = -50 (from out6). Set target -10, current 0 => rawError = -10.
    // newAcc = -50 -10 = -60, clamped = -60. derivative=0 => output = -10.
    float out7 = pdController(1.0f, 0.0f, 0.0f, -10.0f);
    assert(std::fabs(out7 - (-10.0f)) < 1e-6);

    // Test 8: Multiple calls, ensure state is updated correctly.
    // After out7, accumulator = -60. Call with target 0, current 0 => rawError = 0.
    // newAcc = -60 + 0 = -60, clamped = -60. derivative=0 => output = 0.
    float out8 = pdController(1.0f, 0.0f, 0.0f, 0.0f);
    assert(std::fabs(out8 - 0.0f) < 1e-6);
}
#include <algorithm> // for std::clamp

// Simulate a PD controller with a persistent error accumulator.
// Gains and measurements are floats for flexibility.
// The accumulator is clamped to [-100, 100] after each update.
float pdController(float proportionalGain, float derivativeGain,
                   float currentVal, float target) {
    // Persistent state across calls (initialized to 0 on first call).
    static float accumulatedError = 0.0f;

    // Raw error: how far we are from target.
    const float rawError = target - currentVal;

    // Proposed new accumulated error (raw error added to previous accumulator).
    const float newAccumulatedError = accumulatedError + rawError;

    // Clamp to the allowed range.
    const float clampedError = std::clamp(newAccumulatedError, -100.0f, 100.0f);

    // Compute PD output using the difference between previous and new accumulator
    // (this is the derivative of the error signal).
    const float output = proportionalGain * rawError +
                         derivativeGain * (accumulatedError - clampedError);

    // Update persistent state.
    accumulatedError = clampedError;

    return output;
}
// The core is to implement a discrete-time PD controller with an integral-like error accumulator (though it's not true integration—it's a "derivative on error" approach where the derivative term uses the change in the accumulated error). The algorithm:  
// 1. Maintain a `static float` variable (inside the function) that holds the previous accumulated error, initialized to 0 on first call.  
// 2. On each call, compute `rawError = target - currentVal`.  
// 3. Compute `newError = previousAccumulatedError + rawError`.  
// 4. Clamp `newError` to the range [-100, 100] using `std::clamp` (or manual if/else).  
// 5. Compute output = `proportionalGain * rawError + derivativeGain * (previousAccumulatedError - newError)`.  
// 6. Update the static variable to `newError` and return the output.  
//
// Edge cases:  
// - First call: previous accumulator is 0, so derivative term is `0 - newError = -newError`.  
// - Clamping: if the sum exceeds 100 or drops below -100, the derivative term uses the difference between the clamped and unclamped? No—we must use the clamped value for both the update and the derivative term, as per spec.  
// - Inputs can be negative, gains can be zero or negative (though typically PD gains are positive).  
// - Repeated calls: the static variable persists between calls, so the function is stateful.  
//
// Time complexity: O(1) per call. Space complexity: O(1) (only a static float). No dynamic allocation.
