Implement a C++ function that simulates the forward pass of an Elman recurrent neural network with one hidden layer. Given a vector of input values, a vector of weights (ordered as: input-to-hidden weights, hidden bias weights, hidden-to-output weights, output bias weights, and recurrent hidden-to-hidden weights), and a specification of the network architecture (number of inputs, hidden neurons, outputs), compute the output vector after applying the tanh activation function to both hidden and output layers. The recurrent connections use the hidden layer's activations from the previous time step, which are initially zero. The function must handle both the case where biases are active or inactive, and return the output vector as a `std::vector<double>`. Ensure the weight vector length exactly matches the required number of weights: `(numInputs * numHidden) + (numHidden * numOutputs) + (numHidden * numHidden) + (activeBias ? numHidden + numOutputs : 0)`.

#include <cassert>
#include <cmath>
#include <vector>

int main() {
    // Helper to compare vectors with tolerance
    auto close = [](const std::vector<double>& a, const std::vector<double>& b) {
        if (a.size() != b.size()) return false;
        for (size_t i = 0; i < a.size(); ++i)
            if (std::fabs(a[i] - b[i]) > 1e-9) return false;
        return true;
    };

    // Test 1: No bias, simple weights, initial hidden zeros
    // Architecture: 1 input, 1 hidden, 1 output
    // Weights: input-hidden = 1.0, recurrent = 0.5, hidden-output = 2.0
    std::vector<double> inputs1 = {0.5};
    std::vector<double> weights1 = {1.0, 0.5, 2.0};
    std::vector<double> prevHidden1 = {0.0};
    std::vector<double> newHidden1;
    auto out1 = elmanForward(inputs1, weights1, 1, 1, 1, false, prevHidden1, newHidden1);
    double expected_H1 = std::tanh(0.5 * 1.0 + 0.0 * 0.5);
    double expected_O1 = std::tanh(expected_H1 * 2.0);
    assert(close(out1, {expected_O1}));
    assert(close(newHidden1, {expected_H1}));

    // Test 2: With bias, 2 inputs, 2 hidden, 1 output
    // Weights: input-hidden (2*2=4), hidden bias (2), hidden-output (2*1=2), output bias (1), recurrent (2*2=4)
    std::vector<double> inputs2 = {1.0, -1.0};
    // Set all weights simply: input-hidden all 0.1, hidden biases all 0.2, hidden-output all 0.3, output bias 0.4, recurrent all 0.5
    std::vector<double> weights2 = {0.1,0.1,0.1,0.1, 0.2,0.2, 0.3,0.3, 0.4, 0.5,0.5,0.5,0.5};
    std::vector<double> prevHidden2 = {0.0, 0.0};
    std::vector<double> newHidden2;
    auto out2 = elmanForward(inputs2, weights2, 2, 2, 1, true, prevHidden2, newHidden2);
    // Manual computation
    double h1 = std::tanh((1.0*0.1 + -1.0*0.1) + (0.0*0.5 + 0.0*0.5) + std::tanh(0.2));
    double h2 = std::tanh((1.0*0.1 + -1.0*0.1) + (0.0*0.5 + 0.0*0.5) + std::tanh(0.2));
    double o1 = std::tanh(h1*0.3 + h2*0.3 + std::tanh(0.4));
    assert(close(out2, {o1}));
    assert(close(newHidden2, {h1, h2}));

    // Test 3: Recurrent effect on second step
    // Use previous hidden from Test 2 output, same inputs and weights
    auto out3 = elmanForward(inputs2, weights2, 2, 2, 1, true, newHidden2, newHidden2);
    // Expected should differ from first step
    assert(!close(out3, out2));

    // Test 4: No bias, multiple hidden with all zero weights → all outputs tanh(0)=0
    std::vector<double> inputs4 = {1.0, 2.0, 3.0};
    std::vector<double> weights4(3*2 + 2*2 + 2*2, 0.0); // 3 inputs, 2 hidden, 2 outputs, recurrent 2x2
    std::vector<double> prevHidden4 = {0.0, 0.0};
    std::vector<double> newHidden4;
    auto out4 = elmanForward(inputs4, weights4, 3, 2, 2, false, prevHidden4, newHidden4);
    assert(close(out4, {0.0, 0.0}));
    assert(close(newHidden4, {0.0, 0.0}));

    // Test 5: Wrong weight count throws
    bool threw = false;
    try {
        std::vector<double> badWeights = {1.0};
        std::vector<double> prevH = {0.0};
        std::vector<double> newH;
        elmanForward({0.0}, badWeights, 1, 1, 1, false, prevH, newH);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    return 0;
}

#include <vector>
#include <cmath>
#include <stdexcept>

/**
 * Performs one forward pass of an Elman recurrent neural network with one hidden layer.
 * @param inputs Input vector of size numInputs.
 * @param weights Flat weight vector. Order: input-to-hidden (numInputs*numHidden),
 *        hidden biases (numHidden if active), hidden-to-output (numHidden*numOutputs),
 *        output biases (numOutputs if active), recurrent hidden-to-hidden (numHidden*numHidden).
 * @param numInputs Number of input neurons.
 * @param numHidden Number of hidden neurons.
 * @param numOutputs Number of output neurons.
 * @param activeBias Whether bias weights are present for hidden and output layers.
 * @param previousHidden Hidden activations from previous time step (size numHidden), initially zeros.
 * @param[out] newHidden Updated hidden activations after this step.
 * @return Output vector of size numOutputs.
 */
std::vector<double> elmanForward(const std::vector<double>& inputs,
                                 const std::vector<double>& weights,
                                 unsigned int numInputs,
                                 unsigned int numHidden,
                                 unsigned int numOutputs,
                                 bool activeBias,
                                 const std::vector<double>& previousHidden,
                                 std::vector<double>& newHidden) {
    // Validate input sizes
    if (inputs.size() != numInputs)
        throw std::invalid_argument("Input size mismatch");
    if (previousHidden.size() != numHidden)
        throw std::invalid_argument("Previous hidden size mismatch");

    // Expected weight count
    size_t expectedWeights = numInputs * numHidden + numHidden * numOutputs + numHidden * numHidden;
    if (activeBias)
        expectedWeights += numHidden + numOutputs;
    if (weights.size() != expectedWeights)
        throw std::invalid_argument("Weight vector size mismatch");

    size_t idx = 0;

    // Hidden layer computation
    std::vector<double> hidden(numHidden, 0.0);
    for (unsigned int j = 0; j < numHidden; ++j) {
        double sum = 0.0;
        // Input connections
        for (unsigned int i = 0; i < numInputs; ++i)
            sum += inputs[i] * weights[idx++];
        // Recurrent connections from previous hidden
        for (unsigned int h = 0; h < numHidden; ++h)
            sum += previousHidden[h] * weights[idx++];
        // Bias
        if (activeBias)
            sum += std::tanh(weights[idx++]); // bias weight passed through tanh, bias value assumed 1.0
        hidden[j] = std::tanh(sum);
    }
    newHidden = hidden;

    // Output layer computation
    std::vector<double> outputs(numOutputs, 0.0);
    for (unsigned int j = 0; j < numOutputs; ++j) {
        double sum = 0.0;
        // Hidden connections
        for (unsigned int h = 0; h < numHidden; ++h)
            sum += hidden[h] * weights[idx++];
        // Bias
        if (activeBias)
            sum += std::tanh(weights[idx++]);
        outputs[j] = std::tanh(sum);
    }

    return outputs;
}

// The solution models a single-hidden-layer Elman network. The algorithm processes the network in two main stages: hidden layer computation and output layer computation. For the hidden layer, each hidden neuron receives weighted sums from all input neurons plus the recurrent connections from all hidden neurons (from the previous time step, initially zero) and optionally a bias weight. The bias weight is first passed through tanh before being multiplied by the bias value (which is set to 1.0 in the default behavior, but here we assume it's 1 for simplicity). The hidden activations are then passed through tanh. For the output layer, each output neuron receives weighted sums from all hidden activations plus optionally a bias weight. The output is also passed through tanh. The recurrent state is updated to the current hidden activations after the computation, allowing the next call to use them. Key edge cases: zero weights must still work correctly, biases can be omitted entirely, and the weight vector must be validated for the exact required length. Time complexity is O(numInputs*numHidden + numHidden*numOutputs + numHidden*numHidden) = O(N*H + H*O + H^2), space complexity is O(H) for the recurrent state plus the output and hidden vectors.
