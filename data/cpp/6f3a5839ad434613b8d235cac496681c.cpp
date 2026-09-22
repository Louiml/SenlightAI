/*
Implement a C++ function `biquadFilter` that simulates the behavior of the digital biquad filter described in the snippet. The function should take as input: a constant reference to a vector of 5 floating-point coefficients `cte` (representing Cte[0] through Cte[4]), a constant reference to a vector of input samples `input` (each sample being a float), and a boolean `resetActive` indicating whether the filter should start in a reset state. The function must return a vector of floats containing the output samples produced by processing the input sequentially. Before processing, if `resetActive` is true, the internal delay line `Del[0..3]` must be initialized to 0.0. For each input sample, compute the accumulator as `Acc = Cte[0] * sample + Cte[1]*Del[0] + Cte[2]*Del[1] + Cte[3]*Del[2] + Cte[4]*Del[3]`, then divide `Acc` by 1024.0 to get the output sample. After computing the output, shift the delay line: `Del[1] = Del[0]; Del[0] = sample; Del[3] = Del[2]; Del[2] = Acc;` (where `Acc` here is the output sample). The function should handle an empty input vector by returning an empty vector, and should not modify the input vector or the coefficient vector. Ensure the function is `const`-correct and does not rely on any global state.
*/

#include <vector>
#include <cstddef> // for size_t

// Simulates a biquad filter with a fixed delay line of length 4.
// Coefficients cte must contain at least 5 elements: cte[0] is the input gain,
// cte[1..4] are the delays' weights. The denominator is fixed at 1024.
// The delay line is always reset to zero at the start of the call.
std::vector<float> biquadFilter(const std::vector<float>& cte, const std::vector<float>& input, bool /*resetActive*/) {
    // If coefficients are insufficient, return empty (or could throw).
    if (cte.size() < 5) {
        return {};
    }

    // Delay line: Del[0] = previous input (before current), Del[1] = older input,
    // Del[2] = previous output, Del[3] = older output.
    // Initialize all to zero (as required by reset and by lack of prior state).
    float Del[4] = {0.0f, 0.0f, 0.0f, 0.0f};

    std::vector<float> output;
    output.reserve(input.size());

    for (float sample : input) {
        // Compute accumulator using current sample and delay line.
        float Acc = cte[0] * sample;
        for (int i = 0; i < 4; ++i) {
            Acc += cte[i + 1] * Del[i];
        }
        // Scale by the fixed denominator.
        float result = Acc / 1024.0f;

        // Update delay line: shift old values.
        // Del[1] = old Del[0], Del[0] = current sample (input)
        // Del[3] = old Del[2], Del[2] = current output
        Del[1] = Del[0];
        Del[0] = sample;
        Del[3] = Del[2];
        Del[2] = result;

        output.push_back(result);
    }

    return output;
}

#include <cassert>
#include <cmath>
#include <vector>

// Function declaration (assume the above definition is linked).
std::vector<float> biquadFilter(const std::vector<float>& cte, const std::vector<float>& input, bool resetActive);

int main() {
    // Test 1: Reset state, simple impulse input, coefficients all 1.
    std::vector<float> cte(5, 1.0f);
    std::vector<float> input = {1024.0f, 0.0f, 0.0f};
    std::vector<float> out = biquadFilter(cte, input, true);
    // First sample: Acc = 1024*1 + 0 = 1024, result = 1.0
    // Then Del: [0]=1024, [1]=0 (initial), [2]=1.0, [3]=0
    // Second sample: sample=0, Acc = 0 + 1*1024 + 1*0 + 1*1 + 1*0 = 1025, result = 1025/1024 ≈ 1.0009765625
    // Third sample: sample=0, Acc = 0 + 1*0 + 1*1024 + 1*1.0 + 1*1.0009765625 = 1026.0009765625? Actually let's compute precisely.
    assert(out.size() == 3);
    assert(std::fabs(out[0] - 1.0f) < 1e-6);
    assert(std::fabs(out[1] - (1025.0f/1024.0f)) < 1e-6);
    assert(std::fabs(out[2] - ((1024.0f + 1.0f + (1025.0f/1024.0f))/1024.0f)) < 1e-5); // more precise: 0 + 1*0? Actually compute: Third sample: Acc = 1*0 + 1*Del[0] (which is 0, because after second sample Del[0]=0) + 1*Del[1] (which is 1024, the first sample) + 1*Del[2] (which is result from second = 1025/1024) + 1*Del[3] (which is 1.0, first result) = 0 + 0 + 1024 + (1025/1024) + 1 = 1025 + (1025/1024). So result = (1025 + 1025/1024)/1024.
    assert(std::fabs(out[2] - (1025.0f + 1025.0f/1024.0f)/1024.0f) < 1e-5);

    // Test 2: Empty input returns empty output.
    assert(biquadFilter(cte, {}, true).empty());

    // Test 3: Zero coefficients produce all zeros.
    std::vector<float> zeroCte(5, 0.0f);
    std::vector<float> out2 = biquadFilter(zeroCte, {1.0f, 2.0f}, false);
    assert(out2.size() == 2);
    assert(out2[0] == 0.0f);
    assert(out2[1] == 0.0f);

    // Test 4: Insufficient coefficients returns empty.
    std::vector<float> shortCte = {1.0f, 2.0f};
    assert(biquadFilter(shortCte, {1.0f}, true).empty());

    // Test 5: Single sample with cte[0]=1024 and other zeros -> output = sample.
    std::vector<float> gainCte = {1024.0f, 0.0f, 0.0f, 0.0f, 0.0f};
    std::vector<float> out3 = biquadFilter(gainCte, {3.0f}, false);
    assert(out3.size() == 1);
    assert(std::fabs(out3[0] - 3.0f) < 1e-6);

    // Test 6: Check that resetActive is ignored (since delay always starts zero).
    std::vector<float> out4 = biquadFilter(cte, {1024.0f, 0.0f}, false);
    // Same as first two samples of Test 1.
    assert(out4.size() == 2);
    assert(std::fabs(out4[0] - 1.0f) < 1e-6);
    assert(std::fabs(out4[1] - (1025.0f/1024.0f)) < 1e-6);

    return 0;
}

// The solution simulates a finite impulse response (FIR) and infinite impulse response (IIR) biquad structure with a fixed denominator of 1024 (effectively a scaling factor). The core algorithm is straightforward: maintain a delay line of 4 floats, initialized to zero if `resetActive` is true; otherwise, the delay line is initialized to zero as well because the function cannot preserve state across calls (it is a standalone function, and the original snippet uses persistent state within a process; here, we recreate that state each call, so we always start with zeros regardless of `resetActive`? Clarify: The task says "if resetActive is true, the internal delay line must be initialized to 0.0." It does not say what to do if false. To be safe, in a standalone function, we have no prior state, so it is reasonable to initialize to zero in all cases, but to respect the specification exactly, we can still initialize to zero at the start, and the `resetActive` flag is essentially redundant because there is no historical state. However, to make the task meaningful, we can interpret that if `resetActive` is false, the delay line is assumed to be in some prior state? But since the function is stateless, we must define: we will always start with delay line zeros. The `resetActive` flag can be ignored or used to confirm that behavior. To avoid ambiguity, the task says "if resetActive is true, the internal delay line must be initialized to 0.0" – but since we have no state otherwise, we can safely always initialize to zero. I will implement it that way, and the `resetActive` parameter can be used for documentation but not affect behavior. Edge cases: empty input returns empty output; coefficients size must be exactly 5 (I will assert that in the function, or handle by throwing? Since we are in a standalone function, I will assume the input is valid and add a check that returns an empty vector if coefficients size != 5, or throw an exception. For simplicity, I'll use `assert` in the function, but that requires `<cassert>`. Alternatively, I'll check and return empty vector if invalid. For the reference solution, I will document that the coefficients vector must have at least 5 elements (I'll use the first 5). Time complexity: O(N) where N is number of input samples; space complexity: O(1) extra (excluding output vector), plus O(N) for the output vector.
