// Write a standalone C++ function `feedForward` that simulates the forward pass of a feedforward neural network given a topology (vector of layer sizes), a vector of weight matrices, a vector of bias vectors, and an input vector. The function should take a const reference to the input, a const reference to the topology, a const reference to a vector of `Eigen::MatrixXd` weights (where `weights[l]` connects layer `l-1` to layer `l`, and `weights[0]` is a 1x1 matrix that is ignored), and a const reference to a vector of `Eigen::VectorXd` biases (where `biases[0]` is a 1-element vector that is ignored). It should return the output of the final layer as an `Eigen::VectorXd`, applying the hyperbolic tangent (tanh) activation function element-wise to each hidden and output layer's pre-activation (i.e., `z = weights[l] * a_{l-1} + biases[l]`, then `a = tanh(z)`). The input is directly assigned to `a` of layer 0 without any transformation. Ensure the function handles a topology with at least two layers and validates sizes only where necessary (you may assume inputs are consistent).
// The solution iterates through the layers starting from layer 1 (skipping the dummy layer 0). For each layer `l`, we compute the pre-activation vector `z` as the matrix-vector product of `weights[l]` with the previous layer's activation `a_{l-1}`, plus the bias `biases[l]`. Then we apply the `tanh` function element-wise to obtain the activation `a_l`. The result of the last layer (index `topology.size()-1`) is returned. Edge cases to consider: the topology must have at least two layers (input layer plus at least one hidden/output layer) to produce a meaningful result; if topology size is 1, an empty or invalid output would result, so we can assume it's at least 2. Since the function uses `Eigen`, the operations are vectorized for efficiency. Time complexity is `O(sum_{l=1}^{L-1} n_l * n_{l-1})` for the matrix multiplications (where `n_l` is the size of layer `l`), and space complexity is `O(max(n_l))` for the temporary `z` and `a` vectors, plus the output vector itself. No additional memory is allocated beyond the Eigen temporaries.
#include <Eigen/Dense>
#include <vector>
#include <cmath>

// Feedforward pass: given input, topology, weights, and biases, return the output of the final layer.
// weights[l] is the weight matrix connecting layer l-1 to layer l (weights[0] is a dummy 1x1).
// biases[l] is the bias vector for layer l (biases[0] is a dummy 1-element).
// Activation: tanh for all non-input layers.
Eigen::VectorXd feedForward(
    const Eigen::VectorXd& input,
    const std::vector<unsigned>& topology,
    const std::vector<Eigen::MatrixXd>& weights,
    const std::vector<Eigen::VectorXd>& biases)
{
    const unsigned num_layers = static_cast<unsigned>(topology.size());
    
    // Store activations for all layers; layer 0 is the input.
    std::vector<Eigen::VectorXd> activations(num_layers);
    activations[0] = input; // Input layer is just the input vector.
    
    // Compute layers 1 through num_layers-1.
    for (unsigned l = 1; l < num_layers; ++l) {
        // Pre-activation: z = W * a^{l-1} + b
        Eigen::VectorXd z = weights[l] * activations[l - 1] + biases[l];
        // Apply tanh element-wise.
        activations[l] = z.unaryExpr([](double x) { return std::tanh(x); });
    }
    
    // Output is the activation of the last layer.
    return activations[num_layers - 1];
}
#include <Eigen/Dense>
#include <vector>
#include <cmath>
#include <cassert>

// The solution function is assumed to be declared/defined above.
// For brevity, we repeat the function here (in practice, include it from header).
Eigen::VectorXd feedForward(
    const Eigen::VectorXd& input,
    const std::vector<unsigned>& topology,
    const std::vector<Eigen::MatrixXd>& weights,
    const std::vector<Eigen::VectorXd>& biases);

int main() {
    // Test 1: Single hidden layer of size 2, output layer size 1.
    std::vector<unsigned> topology = {2, 2, 1};
    std::vector<Eigen::MatrixXd> weights(3);
    std::vector<Eigen::VectorXd> biases(3);
    
    // Dummy layer 0 weights and biases (should be ignored).
    weights[0] = Eigen::MatrixXd::Zero(1, 1);
    biases[0] = Eigen::VectorXd::Zero(1);
    
    // Layer 1: 2x2 weights, 2 biases.
    weights[1] = Eigen::MatrixXd::Zero(2, 2);
    weights[1] << 1.0, 0.0,
                   0.0, 1.0;
    biases[1] = Eigen::VectorXd::Zero(2);
    
    // Layer 2: 1x2 weights, 1 bias.
    weights[2] = Eigen::MatrixXd::Zero(1, 2);
    weights[2] << 1.0, -1.0;
    biases[2] = Eigen::VectorXd::Zero(1);
    
    Eigen::VectorXd input(2);
    input << 0.5, -0.5;
    
    Eigen::VectorXd output = feedForward(input, topology, weights, biases);
    
    // Expected: layer1 a = tanh([0.5, -0.5]) = [tanh(0.5), tanh(-0.5)] ≈ [0.462117, -0.462117]
    // layer2 z = 0.462117 - (-0.462117) = 0.924234, a = tanh(0.924234) ≈ 0.727336
    assert(output.size() == 1);
    assert(std::abs(output(0) - std::tanh(std::tanh(0.5) - std::tanh(-0.5))) < 1e-6);
    
    // Test 2: Two hidden layers, identity-like weights (but tanh distorts).
    topology = {2, 2, 2, 1};
    weights.resize(4);
    biases.resize(4);
    
    weights[0] = Eigen::MatrixXd::Zero(1, 1);
    biases[0] = Eigen::VectorXd::Zero(1);
    
    // Layer 1: identity weights.
    weights[1] = Eigen::MatrixXd::Identity(2, 2);
    biases[1] = Eigen::VectorXd::Zero(2);
    
    // Layer 2: identity weights.
    weights[2] = Eigen::MatrixXd::Identity(2, 2);
    biases[2] = Eigen::VectorXd::Zero(2);
    
    // Layer 3: sum both inputs.
    weights[3] = Eigen::MatrixXd::Zero(1, 2);
    weights[3] << 1.0, 1.0;
    biases[3] = Eigen::VectorXd::Zero(1);
    
    Eigen::VectorXd input2(2);
    input2 << 0.3, -0.7;
    
    Eigen::VectorXd output2 = feedForward(input2, topology, weights, biases);
    
    // layer1: a = tanh([0.3, -0.7]) = [0.2913126, -0.6043678]
    // layer2: a = tanh([0.2913126, -0.6043678]) = [0.283783, -0.539059]
    // layer3: z = 0.283783 - 0.539059 = -0.255276, a = tanh(-0.255276) ≈ -0.24982
    double expected = std::tanh(std::tanh(std::tanh(0.3)) + std::tanh(std::tanh(-0.7)));
    assert(output2.size() == 1);
    assert(std::abs(output2(0) - expected) < 1e-6);
    
    // Test 3: Output layer size 3, check exact values.
    topology = {1, 3};
    weights.resize(2);
    biases.resize(2);
    
    weights[0] = Eigen::MatrixXd::Zero(1, 1);
    biases[0] = Eigen::VectorXd::Zero(1);
    
    weights[1] = Eigen::MatrixXd::Zero(3, 1);
    weights[1] << 1.0, 2.0, -3.0;
    biases[1] = Eigen::VectorXd::Zero(3);
    
    Eigen::VectorXd input3(1);
    input3 << 0.0;
    
    Eigen::VectorXd output3 = feedForward(input3, topology, weights, biases);
    
    // z = [0, 0, 0], a = [tanh(0), tanh(0), tanh(0)] = [0,0,0]
    assert(output3.size() == 3);
    assert((output3.array() == 0.0).all());
    
    // Test 4: Zero input and zero weights/biases, all outputs zero.
    topology = {2, 2, 2};
    weights.assign(3, Eigen::MatrixXd::Zero(1, 1));
    biases.assign(3, Eigen::VectorXd::Zero(1));
    // Set dimensions properly.
    weights[1] = Eigen::MatrixXd::Zero(2, 2);
    biases[1] = Eigen::VectorXd::Zero(2);
    weights[2] = Eigen::MatrixXd::Zero(2, 2);
    biases[2] = Eigen::VectorXd::Zero(2);
    Eigen::VectorXd input4 = Eigen::VectorXd::Zero(2);
    Eigen::VectorXd output4 = feedForward(input4, topology, weights, biases);
    assert(output4.size() == 2);
    assert((output4.array() == 0.0).all());
    
    // Test 5: Large topology, ensure no crash and correct size.
    topology = {5, 10, 20, 15, 3};
    weights.resize(5);
    biases.resize(5);
    weights[0] = Eigen::MatrixXd::Zero(1, 1);
    biases[0] = Eigen::VectorXd::Zero(1);
    for (int l = 1; l < 5; ++l) {
        weights[l] = Eigen::MatrixXd::Random(topology[l], topology[l-1]);
        biases[l] = Eigen::VectorXd::Random(topology[l]);
    }
    Eigen::VectorXd input5 = Eigen::VectorXd::Random(topology[0]);
    Eigen::VectorXd output5 = feedForward(input5, topology, weights, biases);
    assert(output5.size() == 3);
    // Verify each element is in [-1, 1] because tanh.
    assert((output5.array() >= -1.0).all());
    assert((output5.array() <= 1.0).all());
    
    return 0;
}
