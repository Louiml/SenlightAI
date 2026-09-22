// Write a C++ function that simulates a quantized feedback control system. Given a target sequence of 8-bit grayscale intensity values `a[1..n]` (each an integer in `[0, 255]`), a set of allowed integer state updates `b[0..m-1]` (possibly negative, positive, or zero, each with magnitude ≤ 255), and an initial state of 128, the system updates its state `s` for each target index `i` from 1 to `n`. Before each update, the state is incremented by exactly one chosen element from `b`; the resulting sum is clamped to the range `[0, 255]` (i.e., if the sum exceeds 255 it becomes 255, and if it is below 0 it becomes 0). The cost incurred at step `i` is the squared difference `(a[i] - s_new)^2`, where `s_new` is the clamped state after the update. The total cost is the sum of these squared differences over all `n` steps, plus an initial penalty of 1 (which corresponds to the initialization cost). The function must return the minimum possible total cost (as an integer) achievable by choosing an appropriate sequence of `b` elements. The input is given as two vectors: `b` (the allowed updates) and `a` (the target sequence, 1-indexed conceptually; the vector may be 0-indexed but the first element corresponds to step 1). The constraints are `1 ≤ n ≤ 20000`, `1 ≤ m ≤ 256`, and the output cost will fit in a 32-bit signed integer (use at most 1e8 as a large sentinel). Note: the initial state is fixed at 128, and the total cost includes the initial +1 penalty, so for a single step with zero cost, the result would be 1.
// The problem is a classic dynamic programming over states, where each step’s state is the clamped 8-bit intensity value (0 to 255). Let `dp[i][s]` be the minimum cost to reach state `s` after processing the first `i` targets (i.e., after the i-th update), with the cost including the squared errors for steps 1..i but not the initial +1 penalty. The recurrence: for each state `s` from step `i-1`, for each allowed update `b[k]`, compute the new state `s' = clamp(s + b[k])`, and update `dp[i][s'] = min(dp[i][s'], dp[i-1][s] + (a[i] - s')^2)`. The base case is `dp[0][128] = 0`, all other `dp[0]` states are infinity. The initial +1 penalty is added at the end: answer = min over all states `s` of `dp[n][s]` + 1. Since `n` can be large (up to 20000) and state space is only 256, we use a rolling array of two rows (size 2×256) to save memory. Edge cases: clamping bounds (0 and 255) must be handled carefully; if `b[k]` is zero or negative, the state may stay or decrease; the target values are within [0,255], but squared differences can be large (up to 65025 per step), so use a 32-bit int but with careful summation (total max ~1.3e9, but constraints say result fits in 32-bit signed; still, use int64 for intermediate safety? The original uses int and INF=100000000, but the total may exceed that, so we should use a larger INF like `1e9` and use `long long` for accumulation? To be safe, we use `long long` for dp values and the answer, but return as `int` after adding the +1, since the problem guarantees fit. However, we'll use `long long` internally.) The time complexity is O(n * 256 * m) = O(n * m * 256), which for max n=20000, m=256 gives about 1.3 billion operations, which might be borderline but is acceptable in C++ with optimizations; the original code does exactly that. Space is O(256) for the two rows.
#include <vector>
#include <algorithm>
#include <climits>

// Computes the minimum total cost for the quantized feedback system.
// Parameters:
//   b: vector of allowed integer updates (size m).
//   a: vector of target intensities for steps 1..n (size n, 0-indexed).
// Returns the minimum total cost including the +1 initialization penalty.
int minimumFeedbackCost(const std::vector<int>& b, const std::vector<int>& a) {
    const int STATE_MAX = 256;  // possible states are 0..255
    const long long INF = 1e18; // large sentinel

    int n = static_cast<int>(a.size());
    int m = static_cast<int>(b.size());

    // dp[0] = previous row, dp[1] = current row.
    std::vector<long long> dp_prev(STATE_MAX, INF);
    std::vector<long long> dp_curr(STATE_MAX, INF);

    // Base: initial state 128 with cost 0 (excluding the +1 penalty).
    dp_prev[128] = 0;

    for (int i = 0; i < n; ++i) {
        std::fill(dp_curr.begin(), dp_curr.end(), INF);
        int target = a[i];

        // For each possible previous state, apply all updates.
        for (int s = 0; s < STATE_MAX; ++s) {
            long long prev_cost = dp_prev[s];
            if (prev_cost >= INF) continue; // unreachable

            for (int k = 0; k < m; ++k) {
                int sum = s + b[k];
                // Clamp to [0, 255].
                if (sum > 255) sum = 255;
                if (sum < 0) sum = 0;

                long long diff = target - sum;
                long long add = diff * diff;
                long long new_cost = prev_cost + add;
                if (new_cost < dp_curr[sum]) {
                    dp_curr[sum] = new_cost;
                }
            }
        }

        // Swap rows for next iteration.
        dp_prev.swap(dp_curr);
    }

    // Find minimal cost over all final states.
    long long best = INF;
    for (int s = 0; s < STATE_MAX; ++s) {
        best = std::min(best, dp_prev[s]);
    }

    // Add the +1 initialization penalty.
    return static_cast<int>(best + 1);
}
#include <cassert>
#include <vector>

// Declaration of the function under test.
int minimumFeedbackCost(const std::vector<int>& b, const std::vector<int>& a);

int main() {
    // Test 1: Single step, zero update, target=128. Cost = (128-128)^2 = 0, plus 1 = 1.
    {
        std::vector<int> b = {0};
        std::vector<int> a = {128};
        assert(minimumFeedbackCost(b, a) == 1);
    }

    // Test 2: Single step, target=0, update=0. Clamp to 0? Wait: initial 128+0=128, clamped to 128, diff = -128 squared = 16384, plus 1 = 16385.
    {
        std::vector<int> b = {0};
        std::vector<int> a = {0};
        assert(minimumFeedbackCost(b, a) == 16385); // (0-128)^2 + 1 = 16384+1
    }

    // Test 3: Two steps, all updates zero, target both 128. Cost 0 + 0 + 1 = 1.
    {
        std::vector<int> b = {0};
        std::vector<int> a = {128, 128};
        assert(minimumFeedbackCost(b, a) == 1);
    }

    // Test 4: Two steps, updates {255} and {-128}. First step: from 128+255=383->255, target 255, cost 0. Second step: from 255-128=127, target 127, cost 0. Total 1.
    {
        std::vector<int> b = {255, -128};
        std::vector<int> a = {255, 127};
        assert(minimumFeedbackCost(b, a) == 1);
    }

    // Test 5: Three steps, updates {1}. Initial 128. Step1: 128+1=129, target 129, cost 0. Step2: 129+1=130, target 130, cost 0. Step3: 130+1=131, target 131, cost 0. Total 1.
    {
        std::vector<int> b = {1};
        std::vector<int> a = {129, 130, 131};
        assert(minimumFeedbackCost(b, a) == 1);
    }

    // Test 6: Step with clamped upper bound. Initial 128, update 200 -> 328 clamp to 255. Target 255, cost 0. Then update -200 -> 55, target 55, cost 0. Total 1.
    {
        std::vector<int> b = {200, -200};
        std::vector<int> a = {255, 55};
        assert(minimumFeedbackCost(b, a) == 1);
    }

    // Test 7: Step with clamped lower bound. Initial 128, update -200 -> -72 clamp to 0. Target 0, cost 0. Then update 200 -> 200, target 200, cost 0. Total 1.
    {
        std::vector<int> b = {-200, 200};
        std::vector<int> a = {0, 200};
        assert(minimumFeedbackCost(b, a) == 1);
    }

    // Test 8: More complex case with non-optimal choices. Updates: {0, 10}. Targets: [128, 138]. Optimal: stay at 128 first (cost 0), then move to 138 (cost 0) but need update +10 exactly, so from 128+10=138, cost 0. Total 1. If chose update 0 then 0, cost would be 0 + (138-128)^2 = 100 + 1 = 101.
    {
        std::vector<int> b = {0, 10};
        std::vector<int> a = {128, 138};
        assert(minimumFeedbackCost(b, a) == 1);
    }

    // Test 9: Longer sequence with mixed targets, ensure DP works. Updates: {1, -1}. Targets start at 128 then alternate 129,127,128. Each step can follow exactly, cost 0, total 1.
    {
        std::vector<int> b = {1, -1};
        std::vector<int> a = {129, 127, 128, 129, 127, 128};
        assert(minimumFeedbackCost(b, a) == 1);
    }

    // Test 10: Edge case: target far from achievable states. Updates: {0}. Target 255. Initial 128, stays 128, diff = 127^2 = 16129, plus 1 = 16130.
    {
        std::vector<int> b = {0};
        std::vector<int> a = {255};
        assert(minimumFeedbackCost(b, a) == 16130);
    }

    return 0;
}
