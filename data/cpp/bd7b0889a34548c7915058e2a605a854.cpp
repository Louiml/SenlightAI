/*
Write a C++ function named `nystromLearnHalfStep` that simulates the core mathematical update of a 2nd-order Nystrom-Lear integrator for a single integration step. The function should accept a vector of current state values (`state`), a vector of first derivative values (`deriv0`), a vector of second derivative values (`deriv1`), and a time step `dt`. It must return the updated state vector after one complete integration step (i.e., from time `t` to `t + dt`). The algorithm must exactly follow the two-stage scheme: in the first stage (which you can think of as occurring internally), the first half of the state components are advanced by `dt/2` using `deriv0`, while the second half are held unchanged; in the second stage, the first half are further advanced by `dt/2` using the newly computed mid-point derivative for those components, and the second half are advanced by the full `dt` using `deriv1`. Specifically, for the first half of indices (`i < n/2`), the final update is: `state[i] + deriv0[i]*dt/2 + (state[n/2 + i] + deriv1[i]*dt)*dt/2`? Wait—no, that is not correct. Let me re-read the snippet: in case 0, `state_ws[1][i] = state[i] + deriv[0][i] * dto2` for first half, and for second half `state_ws[1][i] = state[i]`. Then in case 1, for second half `state_ws[0][i] += deriv[1][i] * dt` (so second half final = old state + deriv1*dt); for first half, they set `deriv[1][i] = state_ws[0][no2 + i]` (which is the updated second-half state, i.e., old second-half state + deriv1*dt), then `state_ws[0][i] = state_ws[1][i] + deriv[1][i] * dto2` where `state_ws[1][i]` is `state[i] + deriv0[i]*dto2` and `deriv[1][i]` is `state[no2+i] + deriv1[i]*dt`. So first half final = `state[i] + deriv0[i]*(dt/2) + (state[no2+i] + deriv1[i]*dt)*(dt/2)`. The function must handle both odd and even `n`; if `n` is odd, use `no2 = n/2` (integer division), so the first half is indices `0..no2-1` and second half is `no2..n-1`. The function should return a new vector (do not modify inputs). Assume all inputs have the same size, and `dt` is a positive double.
*/

#include <vector>
#include <cstddef>

// Simulate one full integration step of the 2nd-order Nystrom-Lear method.
// state: current state vector
// deriv0: first derivative at beginning of step
// deriv1: second derivative (or derivative used in second stage)
// dt: time step
// Returns the updated state vector after time dt.
std::vector<double> nystromLearnHalfStep(const std::vector<double>& state,
                                         const std::vector<double>& deriv0,
                                         const std::vector<double>& deriv1,
                                         double dt) {
    const std::size_t n = state.size();
    const std::size_t no2 = n / 2;               // number of first-half components
    const double dto2 = dt / 2.0;

    std::vector<double> result(n);

    // First half: advance by dto2 using deriv0, store in result temporarily
    // We'll compute the final result directly later, but need the mid-state.
    std::vector<double> mid_first(no2);
    for (std::size_t i = 0; i < no2; ++i) {
        mid_first[i] = state[i] + deriv0[i] * dto2;
    }

    // Second half: final value is old state + deriv1 * dt.
    // Also, these final values are used as the "new derivative" for first half.
    for (std::size_t i = no2; i < n; ++i) {
        result[i] = state[i] + deriv1[i] * dt;
    }

    // First half final: mid_first + (result[no2 + i]) * dto2
    for (std::size_t i = 0; i < no2; ++i) {
        // result[no2 + i] is the updated second-half state at the same offset
        result[i] = mid_first[i] + result[no2 + i] * dto2;
    }

    return result;
}

#include <cassert>
#include <cmath>
#include <vector>

// Declaration of the function under test (should match the solution)
std::vector<double> nystromLearnHalfStep(const std::vector<double>& state,
                                         const std::vector<double>& deriv0,
                                         const std::vector<double>& deriv1,
                                         double dt);

int main() {
    // Test 1: n=4, all constants
    {
        std::vector<double> state = {0.0, 0.0, 0.0, 0.0};
        std::vector<double> d0 = {1.0, 2.0, 3.0, 4.0};
        std::vector<double> d1 = {5.0, 6.0, 7.0, 8.0};
        double dt = 2.0;
        auto out = nystromLearnHalfStep(state, d0, d1, dt);
        // no2 = 2, dto2 = 1
        // mid_first[0] = 0 + 1*1 = 1, mid_first[1] = 0 + 2*1 = 2
        // result[2] = 0 + 7*2 = 14, result[3] = 0 + 8*2 = 16
        // result[0] = 1 + 14*1 = 15, result[1] = 2 + 16*1 = 18
        assert(out.size() == 4);
        assert(std::fabs(out[0] - 15.0) < 1e-12);
        assert(std::fabs(out[1] - 18.0) < 1e-12);
        assert(std::fabs(out[2] - 14.0) < 1e-12);
        assert(std::fabs(out[3] - 16.0) < 1e-12);
    }

    // Test 2: n=1 (odd, no2=0)
    {
        std::vector<double> state = {10.0};
        std::vector<double> d0 = {2.0};
        std::vector<double> d1 = {3.0};
        double dt = 0.5;
        auto out = nystromLearnHalfStep(state, d0, d1, dt);
        // result[0] = 10 + 3*0.5 = 11.5
        assert(std::fabs(out[0] - 11.5) < 1e-12);
    }

    // Test 3: n=3 (odd, no2=1)
    {
        std::vector<double> state = {1.0, 2.0, 3.0};
        std::vector<double> d0 = {4.0, 5.0, 6.0};
        std::vector<double> d1 = {7.0, 8.0, 9.0};
        double dt = 1.0;
        auto out = nystromLearnHalfStep(state, d0, d1, dt);
        // no2=1, dto2=0.5
        // mid_first[0] = 1 + 4*0.5 = 3
        // result[1] = 2 + 8*1 = 10, result[2] = 3 + 9*1 = 12
        // result[0] = 3 + result[1]*0.5 = 3 + 5 = 8
        assert(std::fabs(out[0] - 8.0) < 1e-12);
        assert(std::fabs(out[1] - 10.0) < 1e-12);
        assert(std::fabs(out[2] - 12.0) < 1e-12);
    }

    // Test 4: n=2, dt=0.1, non-zero initial states
    {
        std::vector<double> state = {1.0, 2.0};
        std::vector<double> d0 = {0.5, 0.25};
        std::vector<double> d1 = {0.1, 0.2};
        double dt = 0.1;
        auto out = nystromLearnHalfStep(state, d0, d1, dt);
        // no2=1, dto2=0.05
        // mid_first[0] = 1 + 0.5*0.05 = 1.025
        // result[1] = 2 + 0.2*0.1 = 2.02
        // result[0] = 1.025 + 2.02*0.05 = 1.025 + 0.101 = 1.126
        assert(std::fabs(out[0] - 1.126) < 1e-12);
        assert(std::fabs(out[1] - 2.02) < 1e-12);
    }

    // Test 5: Verify inputs are not modified (pass by const ref)
    {
        std::vector<double> state = {0.0, 0.0};
        std::vector<double> d0 = {1.0, 1.0};
        std::vector<double> d1 = {2.0, 2.0};
        auto state_copy = state;
        auto d0_copy = d0;
        auto d1_copy = d1;
        auto out = nystromLearnHalfStep(state, d0, d1, 1.0);
        assert(state == state_copy);
        assert(d0 == d0_copy);
        assert(d1 == d1_copy);
    }

    return 0;
}

// The key is to translate the two-stage update from the integrator’s `intermediate_step` logic into a single function that computes the final state after both stages. The algorithm is straightforward: let `n = state.size()`, `dto2 = dt/2`, `no2 = n/2`. First, for indices `i` in `0..no2-1`, compute `mid_state_i = state[i] + deriv0[i]*dto2`. For indices `i` in `no2..n-1`, the mid-state is just `state[i]`. Then, the second-half final values are `state[i] + deriv1[i]*dt` for `i >= no2`. For the first half, the new derivative is taken as the final second-half mid-state? Actually, per the code, `deriv[1][i] = state_ws[0][no2 + i]` where `state_ws[0][no2+i]` after case 1 is `state[no2+i] + deriv1[no2+i]*dt` (since it was updated in the same case). So the new derivative for first half is the updated second-half state at the corresponding offset. Then first half final = `(state[i] + deriv0[i]*dto2) + (state[no2+i] + deriv1[no2+i]*dt) * dto2`. Note that this couples the first half and second half: the first half’s final update uses the second half’s final value. Edge cases: `n` can be 0? You may assume non-empty. If `n=1`, `no2=0`, so first half is empty, second half covers index 0, and final state is `state[0] + deriv1[0]*dt`. If `n=2`, `no2=1`: index 0 uses deriv0 and the updated index 1; index 1 uses deriv1 directly. Time complexity is O(n), space O(n) for the returned vector (plus a few temporaries). Be careful with const correctness: pass inputs by const reference. Use `size_t` for indices to avoid signed/unsigned warnings.
