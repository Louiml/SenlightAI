Write a C++ function that simulates a simplified synapse development rule inspired by the given code. The function should take a vector of synapse strength values (floats) and a single scalar `mDevelopmentSignal` (float) representing the current round's total development signal, along with a scalar `mBestDevelopmentSignal` (float) representing the best signal seen so far. For each synapse strength in the vector, apply the following update rules in place: if the synapse strength is less than `0.3f`, replace it with `0.0f` (simulating detachment); otherwise, if the synapse is "active" (define activity as strength ≥ 0.5f), add `0.1f * (mDevelopmentSignal - mBestDevelopmentSignal) / (mDevelopmentSignal + 0.001f)` clamped to the range `[-0.3f, 0.1f]`; if the synapse is inactive (strength < 0.5f), subtract `mDevelopmentSignal / (mBestDevelopmentSignal + 0.001f)` clamped to a maximum decrease of `-0.3f` (i.e., the decrease is at most 0.3 in magnitude). The function must handle the edge case where `mBestDevelopmentSignal` is zero or negative by treating the denominator as `0.001f` to avoid division by zero. Return the total absolute change applied to all synapses (sum of absolute value of each strength modification). The input vector is modified in place.
// The solution iterates over each synapse strength in the vector. For each element, we first check if the current strength is below the disconnect threshold (0.3f). If so, we set it to 0.0f and add the absolute change (which is the original strength) to the total change. Otherwise, we determine activity: active if strength ≥ 0.5f. For active synapses, we compute `change = 0.1f * (mDevelopmentSignal - mBestDevelopmentSignal) / (mDevelopmentSignal + 0.001f)`. To avoid division issues, if `mDevelopmentSignal` is negative or zero, we use a denominator of 0.001f (though the formula already adds 0.001f, but if mDevelopmentSignal is very negative, the denominator could be near zero; we can clamp by using `max(mDevelopmentSignal, 0.0f) + 0.001f` for robustness, but the specification says just use `mDevelopmentSignal + 0.001f`, so we'll follow that). Then clamp the change to the range `[-0.3f, 0.1f]`. For inactive synapses, compute `change = -mDevelopmentSignal / (mBestDevelopmentSignal + 0.001f)` but if `mBestDevelopmentSignal` is ≤ 0, use denominator `0.001f`. Then clamp the change to be at most `-0.3f` in magnitude, meaning the clamp is to `[-0.3f, 0.0f]`? The specification says "subtract ... clamped to a maximum decrease of -0.3f" meaning the resulting change is negative and cannot go below -0.3f, so clamp to `[-0.3f, 0.0f]` (since it's always negative if mDevelopmentSignal positive, but if mDevelopmentSignal negative, the change could be positive – but we should still clamp to that range). After computing change, add to strength and accumulate absolute change. Edge cases: if mBestDevelopmentSignal is zero, we use 0.001f denominator. If mDevelopmentSignal is negative, the formula may produce positive changes for inactive synapses; that's fine. Time complexity is O(n) where n is the number of synapses. Space complexity O(1) extra aside from the input vector.
#include <vector>
#include <algorithm>
#include <cmath>

/**
 * Applies synapse development rules to a vector of strengths in place.
 * Returns the sum of absolute changes made.
 */
float developSynapses(std::vector<float>& synapseStrengths,
                      float mDevelopmentSignal,
                      float mBestDevelopmentSignal) {
    const float DISCONNECT_THRESHOLD = 0.3f;
    const float ACTIVITY_THRESHOLD = 0.5f;
    const float MAX_STRENGTHEN = 0.1f;
    const float MAX_WEAKEN = -0.3f;
    const float EPSILON = 0.001f;

    float totalAbsoluteChange = 0.0f;

    for (float& strength : synapseStrengths) {
        float original = strength;
        
        // Disconnect weak synapses
        if (strength < DISCONNECT_THRESHOLD) {
            strength = 0.0f;
            totalAbsoluteChange += std::fabs(original);
            continue;
        }

        float change = 0.0f;
        bool active = strength >= ACTIVITY_THRESHOLD;

        if (active) {
            // Strengthen/weaken based on signal difference
            float denominator = mDevelopmentSignal + EPSILON;
            if (std::fabs(denominator) < EPSILON) {
                denominator = EPSILON;
            }
            change = MAX_STRENGTHEN * (mDevelopmentSignal - mBestDevelopmentSignal) / denominator;
            // Clamp to [MAX_WEAKEN, MAX_STRENGTHEN]
            change = std::max(MAX_WEAKEN, std::min(MAX_STRENGTHEN, change));
        } else {
            // Weaken inactive synapses proportionally
            float denominator = mBestDevelopmentSignal + EPSILON;
            if (std::fabs(denominator) < EPSILON) {
                denominator = EPSILON;
            }
            change = -mDevelopmentSignal / denominator;
            // Clamp to [MAX_WEAKEN, 0.0] (maximum decrease of 0.3)
            change = std::max(MAX_WEAKEN, std::min(0.0f, change));
        }

        strength += change;
        totalAbsoluteChange += std::fabs(change);
    }

    return totalAbsoluteChange;
}
#include <cassert>
#include <cmath>
#include <vector>

// The solution function would be declared here or included from above
// float developSynapses(std::vector<float>&, float, float);

int main() {
    // Basic test: active synapse strengthens when signal exceeds best
    std::vector<float> v1 = {0.6f, 0.8f};
    float change1 = developSynapses(v1, 100.0f, 50.0f);
    assert(std::fabs(v1[0] - 0.6f - 0.1f*(50.0f)/100.001f) < 0.0001f);
    assert(std::fabs(v1[1] - 0.8f - 0.1f*(50.0f)/100.001f) < 0.0001f);
    assert(change1 > 0.0f);

    // Inactive synapse weakens
    std::vector<float> v2 = {0.4f, 0.45f};
    float change2 = developSynapses(v2, 100.0f, 100.0f);
    assert(std::fabs(v2[0] - (0.4f - 1.0f)) < 0.0001f); // change = -100/100.001 ≈ -1, but clamped to -0.3
    assert(std::fabs(v2[0] - 0.1f) < 0.0001f);
    assert(std::fabs(v2[1] - 0.15f) < 0.0001f);

    // Disconnect weak synapses
    std::vector<float> v3 = {0.2f, 0.5f};
    float change3 = developSynapses(v3, 50.0f, 40.0f);
    assert(v3[0] == 0.0f);
    assert(std::fabs(v3[1] - (0.5f + 0.1f*(10.0f)/50.001f)) < 0.0001f);

    // Edge case: zero best signal
    std::vector<float> v4 = {0.6f, 0.4f};
    float change4 = developSynapses(v4, 10.0f, 0.0f);
    // Active: change = 0.1*(10-0)/10.001 ≈ 0.1, strength = 0.7
    // Inactive: change = -10/0.001 = -10000, clamped to -0.3, strength = 0.1
    assert(std::fabs(v4[0] - 0.7f) < 0.0001f);
    assert(std::fabs(v4[1] - 0.1f) < 0.0001f);

    // Edge case: negative development signal
    std::vector<float> v5 = {0.6f, 0.4f};
    float change5 = developSynapses(v5, -10.0f, 5.0f);
    // Active: change = 0.1*(-15)/-9.999 ≈ 0.15, clamped to 0.1, strength = 0.7
    // Inactive: change = -(-10)/5.001 ≈ 2.0, clamped to 0.0, strength = 0.4
    assert(std::fabs(v5[0] - 0.7f) < 0.0001f);
    assert(std::fabs(v5[1] - 0.4f) < 0.0001f);

    // Total absolute change is non-negative
    assert(change5 >= 0.0f);
}
