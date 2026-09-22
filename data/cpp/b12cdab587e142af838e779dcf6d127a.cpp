// Write a C++ function named `computeAndCompareStepError` that takes no arguments and performs an experiment comparing the numerical solution of the ordinary differential equation `x'(t) = 3/(2*t^2) + x/(2*t)` with initial condition `x(1) = 0` using two different numeric types: a fixed-step fourth-order Runge-Kutta (RK4) integrator with step size `dt = 0.5`, halved repeatedly, once using `double` precision and once using `boost::multiprecision::cpp_dec_float_50` multiprecision. For each step size, the function must compute the relative error at `t = 1 + dt` compared to the analytic solution `x(t) = sqrt(t) - 1/t`, and return a `std::vector<std::pair<double, double>>` where each pair contains the relative errors for the multiprecision and the double precision computations, respectively, for the sequence of step sizes `0.5, 0.25, 0.125, ...` down to `~1e-20`. The function should output (to standard output) a table with columns `dt`, `mp`, and `double` for each step size, and should stop the loop when the multiprecision step size is less than or equal to `1e-20`. Use `boost::numeric::odeint` and the provided stepper types exactly as in the snippet. The function must be self-contained, include all necessary headers, and use `const` correctness where appropriate. You may assume that the user will have Boost installed (including `boost/numeric/odeint.hpp` and `boost/multiprecision/cpp_dec_float.hpp`).

#include <cassert>
#include <cmath>
#include <vector>
#include <utility>

// Declaration of the function to test (provided by the solution)
std::vector<std::pair<double, double>> computeAndCompareStepError();

int main() {
    auto errors = computeAndCompareStepError();

    // At least one pair should be present
    assert(errors.size() > 0);

    // For the first step (dt = 0.5), both errors should be non-negative finite numbers
    assert(std::isfinite(errors[0].first));
    assert(std::isfinite(errors[0].second));
    assert(errors[0].first >= 0.0);
    assert(errors[0].second >= 0.0);

    // As dt decreases, errors should generally decrease (though not strictly monotonic due to rounding, but the trend should hold)
    // We check that the last error is much smaller than the first error for both types
    assert(errors.back().first < errors.front().first);
    assert(errors.back().second < errors.front().second);

    // Check that the number of steps is around 67 (since 0.5 * 2^-n <= 1e-20)
    // 0.5 * 2^-n <= 1e-20 => 2^-n <= 2e-20 => n >= log2(1/(2e-20)) ~ 66.2, so steps = 67
    assert(errors.size() >= 60 && errors.size() <= 70);

    // Verify that the first step size printed would be 0.5; we can just check that the errors are positive.
    // A simple sanity check: the last step size is around 1e-20, so errors should be very small (below 1e-10 typically)
    assert(errors.back().first < 1e-10);
    assert(errors.back().second < 1e-10);

    // Ensure all values are finite
    for (const auto& p : errors) {
        assert(std::isfinite(p.first));
        assert(std::isfinite(p.second));
    }

    // Optionally, we can also check that double error is larger than multiprecision for small dt
    // (but not always for large dt due to rounding, so we only check for the last few)
    for (size_t i = errors.size() - 3; i < errors.size(); ++i) {
        assert(errors[i].second > errors[i].first);
    }

    return 0;
}

#include <iostream>
#include <vector>
#include <utility>
#include <cmath>
#include <boost/numeric/odeint.hpp>
#include <boost/multiprecision/cpp_dec_float.hpp>

using namespace std;
using namespace boost::numeric::odeint;

typedef boost::multiprecision::cpp_dec_float_50 mp_50;

// RHS for multiprecision
void rhs_m(const mp_50 x, mp_50 &dxdt, const mp_50 t) {
    dxdt = mp_50(3) / (mp_50(2) * t * t) + x / (mp_50(2) * t);
}

// RHS for double
void rhs_d(const double x, double &dxdt, const double t) {
    dxdt = 3.0 / (2.0 * t * t) + x / (2.0 * t);
}

// RK4 steppers with fixed types
typedef runge_kutta4<mp_50, mp_50, mp_50, mp_50, vector_space_algebra, default_operations, never_resizer> stepper_type_m;
typedef runge_kutta4<double, double, double, double, vector_space_algebra, default_operations, never_resizer> stepper_type_d;

// Returns vector of pairs (mp_error, double_error) for each dt
std::vector<std::pair<double, double>> computeAndCompareStepError() {
    stepper_type_m stepper_m;
    stepper_type_d stepper_d;

    mp_50 dt_m(0.5);
    double dt_d(0.5);

    std::vector<std::pair<double, double>> errors;
    errors.reserve(70);

    cout << "dt" << '\t' << "mp" << '\t' << "double" << endl;

    while (dt_m > 1E-20) {
        // Multiprecision step
        mp_50 x_m = 0; // x(1) = 0
        stepper_m.do_step(rhs_m, x_m, mp_50(1), dt_m);
        mp_50 exact_m = sqrt(mp_50(1) + dt_m) - mp_50(1) / (mp_50(1) + dt_m);
        double rel_err_m = abs((x_m - exact_m) / x_m).convert_to<double>();

        // Double step
        double x_d = 0;
        stepper_d.do_step(rhs_d, x_d, 1.0, dt_d);
        double exact_d = sqrt(1.0 + dt_d) - 1.0 / (1.0 + dt_d);
        double rel_err_d = abs((x_d - exact_d) / x_d);

        errors.emplace_back(rel_err_m, rel_err_d);

        cout << dt_m << '\t' << rel_err_m << '\t' << rel_err_d << endl;

        dt_m /= 2;
        dt_d /= 2;
    }

    return errors;
}

// The solution is a direct translation of the reference code into a standalone function. The core idea is to instantiate two RK4 steppers from Boost’s odeint library, one specialized for `mp_50` (a 50-digit decimal multiprecision type) and one for `double`. For each step size `dt` starting at `0.5` and halving each iteration, we perform a single step from `t = 1` with initial condition `x(1) = 0` for both types. The relative error is computed as `abs((x_numerical - x_exact) / x_numerical)`, where the exact value is `sqrt(1+dt) - 1/(1+dt)`. The loop condition is `dt_m > 1E-20`, and the step size is halved at the end of each iteration. The function returns a vector of pairs of relative errors (first = multiprecision, second = double). Edge cases: ensure that the `dt` values are exactly representable as `mp_50` and `double`; for extremely small `dt` near the stopping threshold, the loop will eventually terminate after `dt_m` becomes smaller than `1E-20`, but since each halving eventually underflows or becomes too small, the loop will terminate correctly. The time complexity is `O(log(0.5/1e-20)) = O(1)` in practice (about 67 iterations), each requiring constant-time arithmetic, so overall `O(1)` time and `O(1)` auxiliary space (excluding the vector and the Boost internals). The main algorithmic challenge is correctly applying the `do_step` function with the proper template arguments and ensuring the type conversions are explicit where needed (e.g., `mp_50(1)` and `mp_50(3)` in the RHS).
