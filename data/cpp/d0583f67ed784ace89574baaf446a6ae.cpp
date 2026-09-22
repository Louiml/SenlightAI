You are given a structure `Regulator` with a constructor taking `std::vector<double>` parameters and a method `symulujSP()` that returns a setpoint value. Create a standalone C++ function (not a class) that simulates a PI (proportional-integral) controller given a control error sequence and controller gains. Specifically, write a function `double symulujPI(const std::vector<double>& parametryGeneratora, double k, double Ti, const std::vector<double>& errors, double initialSum = 0.0)` which, for each error value in `errors`, computes the controller output as `k * error + (k/Ti) * integral`, where the integral is the cumulative sum of all past errors (including the current one) multiplied by a sample time of 1.0. The function should process errors sequentially, updating an internal error sum, and return the final controller output after processing all errors. The function must handle an empty `errors` vector by returning `initialSum` (since no control action occurs). The setpoint from `symulujSP()` is not used directly; instead, the errors are provided as already computed deviations from setpoint. Use `const` correctly for parameters that are not modified.

// The solution simulates a discrete-time PI controller. The algorithm iterates through each error in the input vector, accumulating the total error sum (`integral`), and computes the output as `P + I`, where `P = k * error` and `I = (k / Ti) * totalSum`. Since the sample time is implicitly 1.0 (the integral is just the sum of errors), no additional multiplication by sampling period is needed. Important edge cases: (1) empty input vector – return the initial sum (which is 0 by default) to indicate no control action; (2) `Ti` could be zero – but we assume valid input (nonzero) to avoid division by zero; but we can add a guard returning `k * error` if `Ti` is zero to avoid crash. Time complexity is O(n) because we iterate over each error once, using O(1) extra space for the accumulator. The function should be `const`-correct: pass vectors by `const&`, use local variables, no mutation of inputs.

#include <vector>

/**
 * Simulates a discrete PI controller for a sequence of error values.
 * 
 * @param parametryGeneratora Unused parameter vector (preserved for interface compatibility).
 * @param k Proportional gain.
 * @param Ti Integral time constant (must be nonzero, otherwise P-only is used).
 * @param errors Sequence of error signals (deviation from setpoint).
 * @param initialSum Initial integral (accumulated error) value (default 0).
 * @return Final controller output after processing all errors.
 */
double symulujPI(const std::vector<double>& parametryGeneratora,
                 double k, double Ti,
                 const std::vector<double>& errors,
                 double initialSum = 0.0) {
    // Unused parameter to avoid warnings.
    (void)parametryGeneratora;

    double integral = initialSum;
    double output = 0.0;

    for (double e : errors) {
        // Update integral (discrete integration with sample time 1.0)
        integral += e;

        // Proportional term
        double p = k * e;

        // Integral term; guard against Ti = 0
        double i = (Ti != 0.0) ? (k / Ti) * integral : 0.0;

        // Controller output for this step
        output = p + i;
    }

    // If no errors were processed, return the initial integral (as zero control action)
    return (errors.empty()) ? initialSum : output;
}

#include <cassert>
#include <vector>

// Declaration of the solution function (include the above code here)
double symulujPI(const std::vector<double>&, double, double, const std::vector<double>&, double = 0.0);

int main() {
    // Test 1: Simple two errors with k=1, Ti=2
    std::vector<double> e1 = {2.0, 3.0};
    // Step 1: integral=2, P=2, I=(1/2)*2=1 -> output=3
    // Step 2: integral=5, P=3, I=(1/2)*5=2.5 -> output=5.5
    assert(symulujPI({}, 1.0, 2.0, e1) == 5.5);

    // Test 2: Negative errors
    std::vector<double> e2 = {-1.0, -2.0};
    // Step1: integral=-1, P=-1, I=(1/2)*(-1)=-0.5 -> output=-1.5
    // Step2: integral=-3, P=-2, I=(1/2)*(-3)=-1.5 -> output=-3.5
    assert(symulujPI({}, 1.0, 2.0, e2) == -3.5);

    // Test 3: Ti = 0 (should give P-only output)
    std::vector<double> e3 = {4.0, 5.0};
    // Step1: P=4, I=0 -> output=4; Step2: P=5, I=0 -> output=5
    assert(symulujPI({}, 1.0, 0.0, e3) == 5.0);

    // Test 4: Empty errors returns initialSum
    assert(symulujPI({}, 2.0, 1.0, {}) == 0.0);
    assert(symulujPI({}, 2.0, 1.0, {}, 7.5) == 7.5);

    // Test 5: Single error with initial sum
    std::vector<double> e5 = {10.0};
    // integral = 0+10=10, P=0.5*10=5, I=(0.5/2)*10=2.5 -> output=7.5
    assert(symulujPI({}, 0.5, 2.0, e5, 0.0) == 7.5);

    // Test 6: Larger gains and sequence
    std::vector<double> e6 = {1.0, -1.0, 2.0};
    // k=2, Ti=4
    // Step1: int=1, P=2, I=(2/4)*1=0.5 -> out=2.5
    // Step2: int=0, P=-2, I=0 -> out=-2
    // Step3: int=2, P=4, I=(2/4)*2=1 -> out=5
    assert(symulujPI({}, 2.0, 4.0, e6) == 5.0);

    // Test 7: Empty parametryGeneratora doesn't affect result
    std::vector<double> e7 = {0.5};
    assert(symulujPI({999.0}, 1.0, 1.0, e7) == 1.0); // int=0.5, P=0.5, I=0.5 -> 1.0

    return 0;
}
