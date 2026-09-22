Write a standalone C++ function that simulates a simplified version of the softmax output layer backpropagation from the given neural network snippet. Your function should take a 3D vector of floats representing the target answers, a 3D vector of floats representing the current output logits (raw pre-activation values), and a boolean flag indicating whether to apply softmax. It should compute and return a 3D vector of error gradients (the derivative of the regression loss with respect to each output before activation). If `useSoftmax` is true, apply the softmax transform to the logits before computing gradients; otherwise, use the raw logits directly. Use mean squared error (MSE) as the regression loss: `error = 0.5 * (output - answer)^2`, so its gradient is `(output - answer)`. The function must properly handle cases where the sum of exponentials is zero (avoid division by zero) and must preserve the original 3D structure. The input dimensions are guaranteed to be non-zero and consistent across all three dimensions.

The core algorithm mirrors the backpropagation logic in the provided snippet:  
1. If `useSoftmax` is true, first compute the softmax denominator by summing `exp(logit(x))` over all elements in the 3D structure, where `logit(x) = log(x) - log(1-x)` (the logit function as given in the snippet). For numeric stability, we can apply the logit transform to each logit value, exponentiate it, and accumulate.  
2. For each element, compute the transformed output: if softmax, `output = exp(logit(logitValue)) / sumExp`; otherwise `output = logitValue`. In the snippet, `logit(output)` is applied before exponentiating, so the expression is `exp(logit(x))`, which simplifies to `x / (1 - x)` for `x` in (0,1). However, to match the original code, we will follow the literal transformations: `exp(log(a) - log(1-a)) = a/(1-a)`.  
3. Compute the gradient as `toAdd = - (output - answer)` from the snippet, but note that the snippet adds `toAdd` to the error. Since the task is to return the error gradient with respect to the output, the function returns `(output - answer)` (the negative of what the snippet adds).  
4. Edge cases: If `useSoftmax` is true and `sumExp` is zero, avoid division by zero by outputting a gradient of zero for all entries. Also, the logit function requires `x` strictly between 0 and 1; for values outside this range, we can clamp them to avoid `log(0)` or `log(negative)`. For simplicity, we clamp logits to the range `[1e-7, 1-1e-7]` when applying the logit transform.  
5. Complexity: For a 3D grid of size `H*W*D` elements, the function processes each element a constant number of times (one pass for summation, one for gradient), so time complexity is `O(H*W*D)`. Space complexity is `O(H*W*D)` for the result vector, which is required to return the 3D structure.

#include <vector>
#include <cmath>
#include <algorithm>

// Logit transform with clamping to avoid log(0) or log(negative)
double logit_clamped(double x) {
    double clamped = std::max(1e-7, std::min(1.0 - 1e-7, x));
    return std::log(clamped) - std::log(1.0 - clamped);
}

// Compute error gradients for a simplified output layer.
// logits[i][j][k] are raw pre-activation values.
// answers[i][j][k] are target values.
// If useSoftmax is true, apply softmax on the logits before computing gradients.
// Returns a 3D vector of the same dimensions as inputs.
std::vector<std::vector<std::vector<double>>> computeOutputGradients(
    const std::vector<std::vector<std::vector<double>>>& logits,
    const std::vector<std::vector<std::vector<double>>>& answers,
    bool useSoftmax) {
    
    const int height = logits.size();
    const int width = height > 0 ? logits[0].size() : 0;
    const int depth = width > 0 ? logits[0][0].size() : 0;
    
    std::vector<std::vector<std::vector<double>>> gradients(
        height, std::vector<std::vector<double>>(width, std::vector<double>(depth, 0.0)));
    
    double sumExp = 0.0;
    if (useSoftmax) {
        for (int h = 0; h < height; ++h) {
            for (int w = 0; w < width; ++w) {
                for (int d = 0; d < depth; ++d) {
                    double logitVal = logit_clamped(logits[h][w][d]);
                    sumExp += std::exp(logitVal);
                }
            }
        }
    }
    
    for (int h = 0; h < height; ++h) {
        for (int w = 0; w < width; ++w) {
            for (int d = 0; d < depth; ++d) {
                double output;
                if (useSoftmax) {
                    if (sumExp > 0) {
                        double logitVal = logit_clamped(logits[h][w][d]);
                        output = std::exp(logitVal) / sumExp;
                    } else {
                        output = 0.0; // Avoid division by zero; output zero gradient contribution
                    }
                } else {
                    output = logits[h][w][d];
                }
                gradients[h][w][d] = output - answers[h][w][d];
            }
        }
    }
    
    return gradients;
}

#include <cassert>
#include <cmath>

int main() {
    // Test 1: Basic no-softmax case with simple values
    std::vector<std::vector<std::vector<double>>> logits = {{{1.0, 2.0}, {3.0, 4.0}}};
    std::vector<std::vector<std::vector<double>>> answers = {{{0.5, 1.0}, {2.0, 5.0}}};
    auto grad = computeOutputGradients(logits, answers, false);
    assert(std::fabs(grad[0][0][0] - 0.5) < 1e-6);
    assert(std::fabs(grad[0][0][1] - 1.0) < 1e-6);
    assert(std::fabs(grad[1][0][0] - 1.0) < 1e-6);
    assert(std::fabs(grad[1][0][1] - (-1.0)) < 1e-6);

    // Test 2: Softmax case with single element (output becomes 1.0)
    std::vector<std::vector<std::vector<double>>> logits2 = {{{0.5}}};
    std::vector<std::vector<std::vector<double>>> answers2 = {{{0.2}}};
    auto grad2 = computeOutputGradients(logits2, answers2, true);
    // For logit 0.5, exp(logit(0.5)) = exp(log(0.5)-log(0.5)) = exp(0) = 1, sum=1, output=1
    assert(std::fabs(grad2[0][0][0] - 0.8) < 1e-6);

    // Test 3: Softmax with multiple elements, verify outputs sum to 1
    std::vector<std::vector<std::vector<double>>> logits3 = {{{0.2, 0.8}, {0.5, 0.1}}};
    std::vector<std::vector<std::vector<double>>> answers3 = {{{0.1, 0.2}, {0.3, 0.4}}};
    auto grad3 = computeOutputGradients(logits3, answers3, true);
    double sumGrad = 0.0;
    for (int h = 0; h < 2; ++h)
        for (int w = 0; w < 2; ++w)
            for (int d = 0; d < 2; ++d)
                sumGrad += grad3[h][w][d];
    // Since outputs sum to 1 and answers sum to 1.0 (0.1+0.2+0.3+0.4=1.0), sum of gradients should be 0
    assert(std::fabs(sumGrad) < 1e-6);

    // Test 4: Edge case - all logits = 0.5, logit transform gives 1 for each, sumExp = 4, each output = 0.25
    std::vector<std::vector<std::vector<double>>> logits4 = {{{0.5, 0.5}, {0.5, 0.5}}};
    std::vector<std::vector<std::vector<double>>> answers4 = {{{0.25, 0.25}, {0.25, 0.25}}};
    auto grad4 = computeOutputGradients(logits4, answers4, true);
    for (int h = 0; h < 2; ++h)
        for (int w = 0; w < 2; ++w)
            for (int d = 0; d < 2; ++d)
                assert(std::fabs(grad4[h][w][d]) < 1e-6);

    // Test 5: Edge case - logits outside [0,1] with softmax (clamping occurs)
    std::vector<std::vector<std::vector<double>>> logits5 = {{{-1.0, 2.0}}};
    std::vector<std::vector<std::vector<double>>> answers5 = {{{0.0, 0.0}}};
    auto grad5 = computeOutputGradients(logits5, answers5, true);
    // After clamping: -1.0 becomes ~1e-7, 2.0 becomes ~1-1e-7
    assert(grad5[0][0][0] > -1e-6);
    assert(grad5[0][0][1] > -1e-6);

    // Test 6: Empty depth dimension (but non-zero overall) - ensure no crash
    std::vector<std::vector<std::vector<double>>> logits6 = {{{}, {}}};
    std::vector<std::vector<std::vector<double>>> answers6 = {{{}, {}}};
    auto grad6 = computeOutputGradients(logits6, answers6, false);
    assert(grad6.size() == 2 && grad6[0].size() == 0);

    return 0;
}
