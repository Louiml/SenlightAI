Write a C++ function `computeReactionState` that simulates a simplified version of the heating-cooling reaction step in a cosmological simulation. Given two arrays `S_old` and `S_new` of length `n` representing the state variables (e.g., density and internal energy) before and after a hydrodynamics step, a time step `delta_time`, and two scale factors `a_old` and `a_new` (with `a_new > a_old > 0`), the function should update `S_new` in-place to account for a cooling process modeled as:  
`S_new[i] = S_new[i] - k * (S_new[i] - S_old[i]) * delta_time` where `k` is a reaction rate constant equal to `1.0 / (a_old * delta_time)`. This models a simple exponential relaxation toward the old state, representing energy loss. Additionally, the function should compute and return the total change in the state (sum of absolute differences between the final `S_new` and the original `S_new`) as a `double`. The function must handle `delta_time > 0`, `a_old > 0`, and `n > 0`. If any input is invalid, return `0.0` and leave `S_new` unchanged. The arrays are accessed via raw pointers or `std::vector<double>`; choose the signature that is most natural. Provide a free function with appropriate `const` correctness and comments.
#include <cassert>
#include <vector>
#include <cmath>

// Declaration from solution
double computeReactionState(std::vector<double>& S_new, const std::vector<double>& S_old,
                            double delta_time, double a_old);

int main() {
    // Test 1: Simple case, a_old = 1, delta_time = 1
    {
        std::vector<double> old_state = {10.0, 20.0, 30.0};
        std::vector<double> new_state = {12.0, 24.0, 36.0};
        double change = computeReactionState(new_state, old_state, 1.0, 1.0);
        // Expected: S_new[i] = S_new[i] - (S_new[i]-S_old[i])/1 = S_old[i]
        assert(new_state[0] == 10.0);
        assert(new_state[1] == 20.0);
        assert(new_state[2] == 30.0);
        // Change = |2| + |4| + |6| = 12
        assert(std::abs(change - 12.0) < 1e-12);
    }

    // Test 2: Null-like case with empty vectors
    {
        std::vector<double> old_state;
        std::vector<double> new_state;
        assert(computeReactionState(new_state, old_state, 1.0, 1.0) == 0.0);
    }

    // Test 3: Zero delta_time should be invalid
    {
        std::vector<double> old_state = {5.0};
        std::vector<double> new_state = {5.0};
        assert(computeReactionState(new_state, old_state, 0.0, 1.0) == 0.0);
        assert(new_state[0] == 5.0); // unchanged
    }

    // Test 4: Negative a_old invalid
    {
        std::vector<double> old_state = {5.0};
        std::vector<double> new_state = {7.0};
        assert(computeReactionState(new_state, old_state, 1.0, -1.0) == 0.0);
        assert(new_state[0] == 7.0); // unchanged
    }

    // Test 5: Mismatched sizes invalid
    {
        std::vector<double> old_state = {1.0, 2.0};
        std::vector<double> new_state = {1.0};
        assert(computeReactionState(new_state, old_state, 1.0, 1.0) == 0.0);
        assert(new_state[0] == 1.0); // unchanged
    }

    // Test 6: Larger a_old reduces effect; verify formula
    {
        std::vector<double> old_state = {10.0, 20.0};
        std::vector<double> new_state = {20.0, 30.0};
        double change = computeReactionState(new_state, old_state, 0.5, 2.0);
        // k = 1/(2*0.5) = 1, update = 1*(20-10)*0.5 = 5
        // new[0] = 15, new[1] = 10*0.5? wait: update for [1] = (30-20)*0.5=5 => 25
        assert(std::abs(new_state[0] - 15.0) < 1e-12);
        assert(std::abs(new_state[1] - 25.0) < 1e-12);
        // change = 5 + 5 = 10
        assert(std::abs(change - 10.0) < 1e-12);
    }

    // Test 7: No difference -> no change
    {
        std::vector<double> old_state = {5.0, 6.0};
        std::vector<double> new_state = {5.0, 6.0};
        double change = computeReactionState(new_state, old_state, 1.0, 1.0);
        assert(change == 0.0);
        assert(new_state[0] == 5.0 && new_state[1] == 6.0);
    }

    // Test 8: Single element with different values
    {
        std::vector<double> old_state = {3.0};
        std::vector<double> new_state = {6.0};
        double change = computeReactionState(new_state, old_state, 2.0, 3.0);
        // k = 1/(3*2)=1/6, update = (6-3)/3 = 1 => new = 5
        assert(std::abs(new_state[0] - 5.0) < 1e-12);
        assert(std::abs(change - 1.0) < 1e-12);
    }

    return 0;
}
#include <vector>
#include <cmath>
#include <algorithm>

// Compute a simplified cooling reaction step for a state array.
// Updates S_new in-place using S_old as reference. Returns the total absolute change.
double computeReactionState(std::vector<double>& S_new, const std::vector<double>& S_old,
                            double delta_time, double a_old) {
    // Input validation
    if (S_new.empty() || S_old.size() != S_new.size() || delta_time <= 0.0 || a_old <= 0.0) {
        return 0.0;
    }

    const size_t n = S_new.size();
    double totalChange = 0.0;

    // Reaction rate constant k = 1/(a_old * delta_time)
    const double k = 1.0 / (a_old * delta_time);

    // Apply update per element: S_new[i] -= k * (S_new[i] - S_old[i]) * delta_time
    // Since k * delta_time = 1/a_old, this simplifies to S_new[i] -= (S_new[i] - S_old[i]) / a_old
    for (size_t i = 0; i < n; ++i) {
        double original = S_new[i];
        double update = (S_new[i] - S_old[i]) / a_old;
        S_new[i] -= update;
        totalChange += std::abs(S_new[i] - original);
    }

    return totalChange;
}
// The core algorithm is a simple linear update per element: for each index `i`, compute the updated value as `S_new[i] = S_new[i] - (1.0/(a_old*delta_time)) * (S_new[i] - S_old[i]) * delta_time`. Simplifying, the factor `delta_time` cancels, giving `S_new[i] = S_new[i] - (S_new[i] - S_old[i]) / a_old`. This is numerically stable and direct. Edge cases: if any input is invalid (empty arrays, non-positive `delta_time` or `a_old`, or null pointers when vectors are used), the function should return `0.0` and not modify `S_new`. The total change is computed as the sum of absolute differences between the new and original values of `S_new` after the update. Time complexity is O(n) because we iterate once over the array. Space complexity is O(1) beyond the input arrays, as we only use a few scalar temporaries. The function must be safe with `const` for the input arrays (`S_old`), but `S_new` is non-const since it is modified. The result is a double to allow fractional changes.
