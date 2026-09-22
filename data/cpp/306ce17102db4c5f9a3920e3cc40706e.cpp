// Write a standalone C++ function that simulates the evolution of a susceptible-infected-removed (SIR) epidemic model over a series of discrete time steps, but simplified to operate on integer population counts rather than individual entities. The function should accept five parameters: the initial counts of susceptible (`S`), infectious (`I`), and removed (`R`) individuals (all non-negative integers, with `S > 0` and `I > 0`), the transmission rate `beta` (a double in `[0,1]`), the recovery rate `gamma` (a double in `[0,1]`), and the number of time steps `steps` (a positive integer). It should return a `std::vector<std::array<int,3>>` where each inner array holds `{S, I, R}` after each simulation step, starting with the initial state as the first entry. Use a standard discrete SIR update: for each step, compute `new_S = S - round(beta * S * I / N)`, `new_I = I + round(beta * S * I / N) - round(gamma * I)`, `new_R = R + round(gamma * I)`, where `N = S + I + R` at the beginning of that step. Ensure that no count ever becomes negative (clamp at 0) and that if `N == 0`, the state remains unchanged for that step. The total population must remain constant (i.e., `S + I + R` should equal the initial total after every step), so adjust the computed values to enforce this by distributing any integer rounding discrepancy to the compartment with the largest absolute change. The function should be pure and deterministic, not relying on random numbers.
// The solution iterates `steps` times, maintaining the current `{S, I, R}` from the previous state. For each step, the total `N` is computed first. If `N == 0`, the state is appended unchanged and continues. Otherwise, compute the raw changes: `delta_S = round(beta * S * I / N)`, `delta_I = round(beta * S * I / N) - round(gamma * I)`, `delta_R = round(gamma * I)`. This yields `new_S = S - delta_S`, `new_I = I + delta_I`, `new_R = R + delta_R`. The sum of these new counts may differ from the initial total `total` due to independent rounding. To enforce constant total, compute `sum_new = new_S + new_I + new_R` and `diff = total - sum_new`. We need to add `diff` to one compartment. The choice of compartment should be based on which one has the largest absolute change from its original value (i.e., the one that is most sensitive to rounding), and we add `diff` there, but ensure the result doesn't become negative. If a negative would result, choose the next largest absolute change, and so on. To simplify, a robust method: after computing `sum_new`, iterate over the three compartments in order of decreasing absolute change (from their original values), and if adding `diff` to that compartment keeps it non-negative, do that; otherwise, continue. This guarantees total preservation and non-negativity. After that, clamp any negative values to 0 (shouldn't happen but as safety) and recompute total; if total is still off by a small amount due to clamping, adjust by distributing the remaining difference to the compartment with the highest current value (without making it negative). The time complexity is O(steps) because each step does constant work. Space complexity is O(steps) for the output vector, plus O(1) auxiliary. Edge cases include `steps = 0` (return just initial state), `beta = 0` or `gamma = 0`, and cases where `I = 0` (then no infection and recovery only if gamma > 0, but if I=0, the model stays with I=0 forever). Also cases where `S` becomes 0, etc. The rounding function `std::round` returns a double; cast to int. Use `#include <vector>`, `<array>`, `<cmath>`, `<algorithm>`, `<numeric>`, `<utility>`.
#include <vector>
#include <array>
#include <cmath>
#include <algorithm>
#include <numeric>

// Simulate discrete SIR model over given steps.
// Returns vector of {S, I, R} states, starting with initial state.
std::vector<std::array<int, 3>> simulateSIR(
    int S, int I, int R,
    double beta, double gamma,
    int steps) {
    
    std::vector<std::array<int, 3>> result;
    result.reserve(steps + 1);
    result.push_back({S, I, R});
    
    const int total = S + I + R; // constant population
    
    for (int step = 0; step < steps; ++step) {
        int curS = S, curI = I, curR = R;
        int N = curS + curI + curR;
        
        if (N == 0) {
            // No individuals, state unchanged
            result.push_back({curS, curI, curR});
            continue;
        }
        
        // Compute raw changes with rounding
        int infect = static_cast<int>(std::round(beta * curS * curI / static_cast<double>(N)));
        int recover = static_cast<int>(std::round(gamma * curI));
        
        // New counts before total adjustment
        int newS = curS - infect;
        int newI = curI + infect - recover;
        int newR = curR + recover;
        
        // Clamp negatives just in case
        if (newS < 0) newS = 0;
        if (newI < 0) newI = 0;
        if (newR < 0) newR = 0;
        
        // Total must be exactly 'total'
        int sum = newS + newI + newR;
        int diff = total - sum;
        
        // Adjust one compartment to fix total, preferring largest absolute change
        // Compute absolute changes from original values
        int absS = std::abs(newS - curS);
        int absI = std::abs(newI - curI);
        int absR = std::abs(newR - curR);
        
        // Order indices by decreasing absolute change, tie-break by compartment order
        std::array<int, 3> order = {0, 1, 2};
        std::sort(order.begin(), order.end(), [&](int a, int b) {
            int absA = (a == 0) ? absS : (a == 1) ? absI : absR;
            int absB = (b == 0) ? absS : (b == 1) ? absI : absR;
            if (absA != absB) return absA > absB;
            return a < b; // stable tie-break
        });
        
        bool adjusted = false;
        for (int idx : order) {
            int* val = (idx == 0) ? &newS : (idx == 1) ? &newI : &newR;
            if (*val + diff >= 0) {
                *val += diff;
                adjusted = true;
                break;
            }
        }
        if (!adjusted) {
            // If all would become negative (shouldn't happen with sensible inputs),
            // add to the largest current value
            int maxVal = std::max({newS, newI, newR});
            if (maxVal == newS) newS += diff;
            else if (maxVal == newI) newI += diff;
            else newR += diff;
        }
        
        // Final safety: ensure non-negative (should hold already)
        if (newS < 0) newS = 0;
        if (newI < 0) newI = 0;
        if (newR < 0) newR = 0;
        
        // Update state
        S = newS; I = newI; R = newR;
        result.push_back({S, I, R});
    }
    
    return result;
}
#include <cassert>
#include <vector>
#include <array>
#include <numeric>

// Function declaration (would be in header)
std::vector<std::array<int, 3>> simulateSIR(int, int, int, double, double, int);

int main() {
    // Basic decline to zero infection
    auto res = simulateSIR(90, 10, 0, 0.3, 0.5, 5);
    assert(res.size() == 6);
    assert(res[0] == std::array<int,3>{90, 10, 0});
    // Total population constant
    for (const auto& state : res) {
        assert(state[0] + state[1] + state[2] == 100);
    }
    // Infectious should eventually decrease
    assert(res.back()[1] <= res.front()[1]);

    // No recovery, zero beta -> nothing changes
    res = simulateSIR(50, 5, 0, 0.0, 0.0, 3);
    assert(res.size() == 4);
    for (const auto& state : res) {
        assert(state[0] == 50 && state[1] == 5 && state[2] == 0);
    }

    // Beta=0, gamma>0 => recovery only
    res = simulateSIR(100, 10, 0, 0.0, 0.2, 2);
    assert(res[0] == std::array<int,3>{100, 10, 0});
    assert(res.back()[1] == 10 - static_cast<int>(std::round(0.2*10)) - static_cast<int>(std::round(0.2 * (10 - static_cast<int>(std::round(0.2*10)))));
    // Total constant
    for (const auto& state : res) {
        assert(state[0]+state[1]+state[2] == 110);
    }

    // Gamma=0, beta>0 => infection only (S decreases, I increases)
    res = simulateSIR(80, 20, 0, 0.5, 0.0, 2);
    assert(res[0] == std::array<int,3>{80, 20, 0});
    assert(res.back()[0] < 80);
    assert(res.back()[1] > 20);
    // Total constant
    for (const auto& state : res) {
        assert(state[0]+state[1]+state[2] == 100);
    }

    // Zero total population edge case (S=0, I=0, R=0) not allowed by spec (S>0), but if I=0 and S>0, R any
    res = simulateSIR(10, 0, 5, 0.5, 0.5, 3);
    assert(res.size() == 4);
    assert(res.front() == std::array<int,3>{10, 0, 5});
    assert(res.back() == std::array<int,3>{10, 0, 5}); // no change because I=0

    // Steps=0 returns only initial state
    res = simulateSIR(30, 30, 40, 0.1, 0.1, 0);
    assert(res.size() == 1);
    assert(res[0] == std::array<int,3>{30, 30, 40});

    // Large numbers, check total constant
    res = simulateSIR(1000000, 500000, 500000, 0.8, 0.3, 10);
    for (const auto& state : res) {
        assert(state[0]+state[1]+state[2] == 2000000);
        assert(state[0] >= 0 && state[1] >= 0 && state[2] >= 0);
    }

    return 0;
}
