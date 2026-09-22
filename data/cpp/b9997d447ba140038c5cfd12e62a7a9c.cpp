Write a C++ function that simulates a single time-step of a simplified recurrent neural network with a hidden state update similar to a GRU (Gated Recurrent Unit). The function must take two dense 1-D vectors as inputs (a new input vector `x` and a previous hidden state vector `h_prev`), along with three weight matrices and three bias vectors (for update gate, reset gate, and candidate hidden state), and it must return the new hidden state vector `h_next`. Each computation must follow the exact GRU equations: `z = sigmoid(W_z * concat(h_prev, x) + b_z)`, `r = sigmoid(W_r * concat(h_prev, x) + b_r)`, `n = tanh(W_n * concat(r * h_prev, x) + b_n)`, and finally `h_next = (1 - z) * h_prev + z * n`, where `*` denotes matrix-vector multiplication, and `concat` means appending the two vectors. The inputs are of type `std::vector<double>`, and all activation functions are applied element-wise. The function must be `const`-correct and free of side effects.

// The solution approach mirrors the memory-layout trick in the snippet: concatenating `h_prev` and `x` into a single vector `concat = [h_prev, x]` (length `history_size + input_size`). Then each gate is a matrix-vector product: `linear_part = Weights * concat + bias`, computed by iterating over rows of the weight matrix and taking the dot product with `concat`. The sigmoid function maps any real number to `(0,1)` using `1 / (1 + exp(-v))`, and the tanh function maps to `(-1,1)` using `(exp(2v)-1)/(exp(2v)+1)`. After computing `z` and `r`, the candidate `n` uses a modified concatenation `[r * h_prev, x]` (element-wise multiplication of `r` with `h_prev`). Edge cases include zero-sized vectors (but the problem expects non-empty inputs), very large/small values causing `exp` overflow/underflow (use `std::tanh` and a stable sigmoid implementation that clamps arguments), and mismatched dimensions (assert or assume valid). The time complexity is `O(3 * history_size * (history_size + input_size))` for the three matrix-vector products, with `O(history_size + input_size)` auxiliary space for the concatenated vector and gate outputs.

#include <vector>
#include <cmath>
#include <cassert>

// Element-wise sigmoid activation.
double sigmoid(double v) {
    // Clamp to avoid overflow in exp(-v)
    if (v >= 0) {
        double e = std::exp(-v);
        return 1.0 / (1.0 + e);
    } else {
        double e = std::exp(v);
        return e / (1.0 + e);
    }
}

// Element-wise tanh activation (using std::tanh for stability).
double tanh_activation(double v) {
    return std::tanh(v);
}

// Helper to compute matrix-vector product plus bias.
// Weights is a vector of rows, each row has size 'cols'.
std::vector<double> linear_layer(const std::vector<std::vector<double>>& weights,
                                 const std::vector<double>& input,
                                 const std::vector<double>& bias) {
    assert(!weights.empty());
    assert(!input.empty());
    assert(weights[0].size() == input.size());
    assert(weights.size() == bias.size());

    std::vector<double> output(weights.size(), 0.0);
    for (size_t i = 0; i < weights.size(); ++i) {
        double sum = bias[i];
        for (size_t j = 0; j < input.size(); ++j) {
            sum += weights[i][j] * input[j];
        }
        output[i] = sum;
    }
    return output;
}

// Single GRU step.
// Returns h_next given h_prev and x.
std::vector<double> gru_step(const std::vector<double>& h_prev,
                             const std::vector<double>& x,
                             const std::vector<std::vector<double>>& w_update,
                             const std::vector<std::vector<double>>& w_reset,
                             const std::vector<std::vector<double>>& w_hidden,
                             const std::vector<double>& b_update,
                             const std::vector<double>& b_reset,
                             const std::vector<double>& b_hidden) {
    const size_t hist_size = h_prev.size();
    const size_t input_size = x.size();
    const size_t total = hist_size + input_size;

    // Concatenate h_prev and x.
    std::vector<double> concat(total);
    std::copy(h_prev.begin(), h_prev.end(), concat.begin());
    std::copy(x.begin(), x.end(), concat.begin() + hist_size);

    // Update gate z = sigmoid(W_z * concat + b_z)
    std::vector<double> z_linear = linear_layer(w_update, concat, b_update);
    std::vector<double> z(hist_size);
    for (size_t i = 0; i < hist_size; ++i) {
        z[i] = sigmoid(z_linear[i]);
    }

    // Reset gate r = sigmoid(W_r * concat + b_r)
    std::vector<double> r_linear = linear_layer(w_reset, concat, b_reset);
    std::vector<double> r(hist_size);
    for (size_t i = 0; i < hist_size; ++i) {
        r[i] = sigmoid(r_linear[i]);
    }

    // Candidate hidden state: use r * h_prev concatenated with x.
    std::vector<double> gated_hist(hist_size);
    for (size_t i = 0; i < hist_size; ++i) {
        gated_hist[i] = r[i] * h_prev[i];
    }
    std::vector<double> concat_candidate(total);
    std::copy(gated_hist.begin(), gated_hist.end(), concat_candidate.begin());
    std::copy(x.begin(), x.end(), concat_candidate.begin() + hist_size);

    std::vector<double> n_linear = linear_layer(w_hidden, concat_candidate, b_hidden);
    std::vector<double> n(hist_size);
    for (size_t i = 0; i < hist_size; ++i) {
        n[i] = tanh_activation(n_linear[i]);
    }

    // h_next = (1 - z) * h_prev + z * n
    std::vector<double> h_next(hist_size);
    for (size_t i = 0; i < hist_size; ++i) {
        h_next[i] = (1.0 - z[i]) * h_prev[i] + z[i] * n[i];
    }

    return h_next;
}

#include <cassert>
#include <cmath>
#include <vector>

// Assume the solution function and helpers are declared above.
// Test with known small sizes and manually computed values.

int main() {
    // Simple case: hist_size = 1, input_size = 1.
    // Let h_prev = [0], x = [1]
    // Weights all 1.0, biases all 0.0.
    std::vector<double> h_prev = {0.0};
    std::vector<double> x = {1.0};
    std::vector<std::vector<double>> w_update = {{1.0, 1.0}};
    std::vector<std::vector<double>> w_reset = {{1.0, 1.0}};
    std::vector<std::vector<double>> w_hidden = {{1.0, 1.0}};
    std::vector<double> b_update = {0.0};
    std::vector<double> b_reset = {0.0};
    std::vector<double> b_hidden = {0.0};

    // concat = [0, 1]
    // z = sigmoid(0*1 + 1*1) = sigmoid(1) ≈ 0.73105857863
    // r = sigmoid(1) ≈ 0.73105857863
    // r*h_prev = 0, concat_candidate = [0, 1]
    // n = tanh(1) ≈ 0.76159415596
    // h_next = (1 - z)*0 + z*n = 0.73105857863 * 0.76159415596 ≈ 0.5568939...
    std::vector<double> result = gru_step(h_prev, x, w_update, w_reset, w_hidden,
                                          b_update, b_reset, b_hidden);
    assert(std::fabs(result[0] - 0.5568939) < 1e-5);

    // Test with zero input and zero history: h_prev = [0], x = [0]
    // concat = [0,0], all linear outputs 0, z = 0.5, r = 0.5, n = 0
    // h_next = (1-0.5)*0 + 0.5*0 = 0
    std::vector<double> x_zero = {0.0};
    std::vector<double> h_prev_zero = {0.0};
    std::vector<double> result_zero = gru_step(h_prev_zero, x_zero, w_update, w_reset, w_hidden,
                                               b_update, b_reset, b_hidden);
    assert(std::fabs(result_zero[0]) < 1e-6);

    // Test with larger dimension: hist_size=2, input_size=1
    std::vector<double> h_prev2 = {1.0, -1.0};
    std::vector<double> x2 = {0.5};
    std::vector<std::vector<double>> w_update2 = {{0.1, 0.2, 0.3}, {0.4, 0.5, 0.6}};
    std::vector<std::vector<double>> w_reset2 = {{0.2, 0.3, 0.4}, {0.5, 0.6, 0.7}};
    std::vector<std::vector<double>> w_hidden2 = {{0.3, 0.4, 0.5}, {0.6, 0.7, 0.8}};
    std::vector<double> b_update2 = {0.1, -0.1};
    std::vector<double> b_reset2 = {0.2, -0.2};
    std::vector<double> b_hidden2 = {0.3, -0.3};

    // Manual computation:
    // concat = [1, -1, 0.5]
    // update linear: row0=0.1*1 + 0.2*(-1) + 0.3*0.5 + 0.1 = 0.1 -0.2 +0.15 +0.1 = 0.15 -> sigmoid≈0.5374
    //                row1=0.4*1 + 0.5*(-1) + 0.6*0.5 -0.1 = 0.4 -0.5 +0.3 -0.1 = 0.1 -> sigmoid≈0.5249
    // reset linear: row0=0.2*1 +0.3*(-1)+0.4*0.5 +0.2 = 0.2 -0.3 +0.2 +0.2 = 0.3 -> sigmoid≈0.5744
    //               row1=0.5*1 +0.6*(-1)+0.7*0.5 -0.2 = 0.5 -0.6 +0.35 -0.2 = 0.05 -> sigmoid≈0.5125
    // gated_hist = [0.5744*1, 0.5125*(-1)] = [0.5744, -0.5125]
    // concat_candidate = [0.5744, -0.5125, 0.5]
    // hidden linear: row0=0.3*0.5744 + 0.4*(-0.5125) + 0.5*0.5 +0.3 = 0.17232 -0.205 +0.25 +0.3 = 0.51732 -> tanh≈0.4762
    //                row1=0.6*0.5744 + 0.7*(-0.5125) + 0.8*0.5 -0.3 = 0.34464 -0.35875 +0.4 -0.3 = 0.08589 -> tanh≈0.08571
    // h_next[0] = (1-0.5374)*1 + 0.5374*0.4762 = 0.4626 + 0.2559 = 0.7185
    // h_next[1] = (1-0.5249)*(-1) + 0.5249*0.08571 = -0.4751 + 0.0450 = -0.4301
    std::vector<double> result2 = gru_step(h_prev2, x2, w_update2, w_reset2, w_hidden2,
                                           b_update2, b_reset2, b_hidden2);
    assert(std::fabs(result2[0] - 0.7185) < 1e-3);
    assert(std::fabs(result2[1] + 0.4301) < 1e-3);

    return 0;
}
