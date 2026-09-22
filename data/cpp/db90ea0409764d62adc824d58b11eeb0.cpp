Implement a C++ function that simulates a simplified version of the OpenCL inner-product layer’s forward pass for the single-row case (`M == 1`). Given a weight matrix stored as a flat `std::vector<float>` in row-major order (dimensions `K x N`, where `N` is the number of outputs and `K` is the number of inputs), a single input row of length `K`, and an optional bias vector of length `N`, the function must compute the output vector of length `N` as `output[i] = dot(weight_row_i, input) + bias[i]` (or without bias if not provided). The function should take a boolean `useBias` parameter, and if `useBias` is true, the bias vector is non-empty and of size exactly `N`; otherwise, it is empty. Handle edge cases where `K`, `N`, or the input vector may be zero or empty. The function signature should be: `std::vector<float> innerProductSingleRow(const std::vector<float>& weight, const std::vector<float>& input, const std::vector<float>& bias, bool useBias)`. Your implementation must not use any external libraries beyond the standard library and must be self-contained.
#include <cassert>
#include <vector>

// Function declaration (should match the solution above)
std::vector<float> innerProductSingleRow(
    const std::vector<float>& weight,
    const std::vector<float>& input,
    const std::vector<float>& bias,
    bool useBias
);

int main() {
    // Example 1: K=3, N=2, no bias
    std::vector<float> weight1 = {1, 2, 3, 4, 5, 6}; // 2x3 matrix: row0=[1,2,3], row1=[4,5,6]
    std::vector<float> input1 = {2, 1, 0};
    std::vector<float> bias1_empty;
    auto out1 = innerProductSingleRow(weight1, input1, bias1_empty, false);
    assert(out1.size() == 2);
    assert(out1[0] == 1*2 + 2*1 + 3*0); // 4
    assert(out1[1] == 4*2 + 5*1 + 6*0); // 13

    // Example 2: K=3, N=2, with bias
    std::vector<float> bias2 = {1, -2};
    auto out2 = innerProductSingleRow(weight1, input1, bias2, true);
    assert(out2[0] == 4 + 1); // 5
    assert(out2[1] == 13 - 2); // 11

    // Example 3: K=0, N=2 via bias, no weight
    std::vector<float> empty_weight;
    std::vector<float> empty_input;
    std::vector<float> bias3 = {3, 4};
    auto out3 = innerProductSingleRow(empty_weight, empty_input, bias3, true);
    assert(out3.size() == 2);
    assert(out3[0] == 3);
    assert(out3[1] == 4);

    // Example 4: K=0, no bias
    auto out4 = innerProductSingleRow(empty_weight, empty_input, bias1_empty, false);
    assert(out4.empty());

    // Example 5: N=0 (empty weight with K=1)
    std::vector<float> weight5; // no rows
    std::vector<float> input5 = {1};
    auto out5 = innerProductSingleRow(weight5, input5, bias1_empty, false);
    assert(out5.empty());

    // Example 6: Single output, K=4
    std::vector<float> weight6 = {1, 2, 3, 4}; // N=1, K=4
    std::vector<float> input6 = {1, 1, 1, 1};
    auto out6 = innerProductSingleRow(weight6, input6, bias1_empty, false);
    assert(out6.size() == 1);
    assert(out6[0] == 10);

    // Example 7: Values with floating precision (close enough with == because exact operations)
    std::vector<float> weight7 = {0.5, 1.5, -2.0, 3.0}; // K=2, N=2
    std::vector<float> input7 = {2.0, -1.0};
    auto out7 = innerProductSingleRow(weight7, input7, bias1_empty, false);
    assert(out7[0] == 0.5*2 + 1.5*(-1)); // -0.5
    assert(out7[1] == -2.0*2 + 3.0*(-1)); // -7.0

    // Example 8: Bias with negative values
    std::vector<float> bias8 = {0.5, -0.5};
    auto out8 = innerProductSingleRow(weight7, input7, bias8, true);
    assert(out8[0] == -0.5 + 0.5); // 0
    assert(out8[1] == -7.0 - 0.5); // -7.5

    // Example 9: Large K=5, N=1
    std::vector<float> weight9 = {1, 2, 3, 4, 5};
    std::vector<float> input9 = {1, 1, 1, 1, 1};
    auto out9 = innerProductSingleRow(weight9, input9, bias1_empty, false);
    assert(out9[0] == 15);

    // Example 10: Input all zeros, bias only
    std::vector<float> weight10 = {1, 2, 3, 4, 5, 6}; // 2x3
    std::vector<float> input10 = {0, 0, 0};
    std::vector<float> bias10 = {100, 200};
    auto out10 = innerProductSingleRow(weight10, input10, bias10, true);
    assert(out10[0] == 100);
    assert(out10[1] == 200);

    return 0;
}
#include <vector>
#include <cassert>

/**
 * @brief Compute the forward pass of an inner-product layer for a single input row.
 * 
 * Produces an output vector of length N, where each element is the dot product
 * of a row of the weight matrix (size K x N, row-major) with the input vector
 * (length K), plus an optional bias.
 * 
 * @param weight  Flat weight matrix in row-major order, size K * N.
 * @param input   Input vector of length K.
 * @param bias    Bias vector of length N; ignored if useBias is false.
 * @param useBias Whether to add bias to the output.
 * @return std::vector<float> Output vector of length N.
 */
std::vector<float> innerProductSingleRow(
    const std::vector<float>& weight,
    const std::vector<float>& input,
    const std::vector<float>& bias,
    bool useBias
) {
    const size_t K = input.size();
    const size_t N = (K == 0) ? 0 : weight.size() / K; // If K>0, N is derived; if K==0, handle below.

    // If K is zero, we cannot reliably determine N from weight, so output empty (or bias-only?).
    // To make the function robust, if K==0 but bias is provided, we could output bias.
    // But the specification implies K>0 for meaningful weight. We'll handle K==0 by outputting bias if provided.
    if (K == 0) {
        if (useBias) {
            return bias;  // Return a copy of bias as output
        } else {
            return std::vector<float>();
        }
    }

    // Validate sizes
    assert(weight.size() % K == 0 && "Weight size must be a multiple of K");
    const size_t N_valid = weight.size() / K;
    if (useBias) {
        assert(bias.size() == N_valid && "Bias size must equal N");
    }

    std::vector<float> output(N_valid, 0.0f);
    for (size_t j = 0; j < N_valid; ++j) {
        float sum = 0.0f;
        const size_t rowOffset = j * K;
        for (size_t i = 0; i < K; ++i) {
            sum += weight[rowOffset + i] * input[i];
        }
        if (useBias) {
            sum += bias[j];
        }
        output[j] = sum;
    }
    return output;
}
// The core operation is a matrix-vector multiplication where the weight matrix of size `K x N` is multiplied by an input vector of length `K` to produce an output of length `N`. Because the weight is stored row-major, each row of length `K` corresponds to one output neuron’s weights. For each output index `j` from `0` to `N-1`, we compute the dot product of `weight[j*K + i]` for `i=0..K-1` with `input[i]`, then add the bias if `useBias` is true (bias element `bias[j]`). Important edge cases: if `K == 0` and `N > 0`, each dot product is zero, so the output is just the bias (or zeros if no bias). If `N == 0`, the output is an empty vector regardless of `K` or bias. If the input vector is empty but `K > 0`, the behavior is undefined per the specification—we can choose to treat it as if `K == 0` (i.e., produce bias-only outputs) for robustness, which is a reasonable interpretation. The bias vector must be exactly size `N` when `useBias` is true; if not, we can either throw or, for simplicity, ignore mismatched sizes, but the correct approach is to assert that the sizes match in a debug build and fall back to ignoring bias if mismatch occurs. Time complexity: O(K * N) because each of N outputs requires K multiplications and additions. Space complexity: O(N) for the output vector. No extra auxiliary space beyond the output is needed.
