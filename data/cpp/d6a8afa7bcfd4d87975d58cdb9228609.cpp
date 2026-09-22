/*
Write a C++ function that simulates one forward pass of a simple feedforward neural network without any external libraries. Given a vector of positive integers representing the number of neurons in each layer (including input and output layers), a vector of training inputs (each as a `std::vector<double>`), and a vector of weight matrices (each represented as a 2D `std::vector<double>` where rows correspond to the previous layer size and columns to the current layer size), the function must compute and return the output of the last layer for each input. For each layer except the input layer, compute the weighted sum of the previous layer’s outputs (multiplying the previous layer’s output vector by the weight matrix), then apply the hyperbolic tangent activation function to every element of that sum except the last neuron if that layer is not the output layer (to simulate a bias neuron whose activation is fixed to 1). For the output layer, apply tanh to all elements. The input layer is passed through unchanged (including a bias neuron appended with value 1.0 if the input layer is not the only layer). The function must handle any number of layers ≥ 2, assume inputs are already sized correctly (input size equals first topology value), and return a `std::vector<std::vector<double>>` where each inner vector is the output of the last layer for the corresponding input. Use double precision throughout.
*/
#include <vector>
#include <cmath>
#include <cstddef>

// Compute the forward pass outputs for all inputs given topology, weights, and inputs.
// topology: vector of layer sizes (including input and output layers)
// weights: for each transition i from layer i-1 to i, a 2D vector of size (topology[i-1]* (hasBias?1:0) + (i==1?1:0)) x (topology[i] + (i==topology.size()-1?0:1))
// However, to simplify the interface, we assume weights are provided with correct dimensions already.
std::vector<std::vector<double>> forwardPass(
    const std::vector<int>& topology,
    const std::vector<std::vector<double>>& inputs,
    const std::vector<std::vector<std::vector<double>>>& weights)
{
    std::vector<std::vector<double>> outputs;
    outputs.reserve(inputs.size());
    
    for (const auto& input : inputs) {
        // Initialize the current layer vector: input layer plus bias if not the only layer
        std::vector<double> current;
        current.reserve(topology[0] + (topology.size() > 1 ? 1 : 0));
        for (double val : input) current.push_back(val);
        if (topology.size() > 1) current.push_back(1.0); // bias neuron
        
        // Propagate through each layer from 1 to the output layer
        for (size_t layer = 1; layer < topology.size(); ++layer) {
            const auto& weightMat = weights[layer - 1];
            size_t prevSize = current.size();
            size_t currSize = topology[layer] + (layer == topology.size() - 1 ? 0 : 1);
            
            std::vector<double> next(currSize, 0.0);
            // Matrix multiplication: next[j] = sum_i current[i] * weight[i][j]
            for (size_t j = 0; j < currSize; ++j) {
                double sum = 0.0;
                for (size_t i = 0; i < prevSize; ++i) {
                    sum += current[i] * weightMat[i][j];
                }
                // Apply tanh activation, but keep bias neuron (last element) as 1.0 for hidden layers
                if (layer != topology.size() - 1 && j == currSize - 1) {
                    next[j] = 1.0; // bias neuron
                } else {
                    next[j] = std::tanh(sum);
                }
            }
            current = std::move(next);
        }
        
        // For a single-layer network (only input layer), the output is the input itself
        if (topology.size() == 1) {
            outputs.push_back(input);
        } else {
            // Remove the bias neuron from the output layer? No, output layer has no bias appended.
            // current already has exactly topology.back() elements.
            outputs.push_back(current);
        }
    }
    
    return outputs;
}
#include <cassert>
#include <cmath>

int main() {
    // Test 1: Simple 2-2-1 network (input 2, hidden 2 (with bias), output 1)
    // Topology: [2, 2, 1] -> input layer size 2, hidden layer size 2 (includes bias), output size 1
    std::vector<int> topo1 = {2, 2, 1};
    // Weight matrix from input (size 3: 2 inputs + 1 bias) to hidden (size 3: 2 neurons + 1 bias)
    std::vector<std::vector<double>> W1 = {
        {0.5, -1.0, 0.0}, // from input1
        {0.2, 0.3, 0.0},  // from input2
        {0.1, -0.4, 0.0}  // from bias
    };
    // Weight matrix from hidden (size 3) to output (size 1)
    std::vector<std::vector<double>> W2 = {
        {0.8},
        {-0.2},
        {0.5}
    };
    std::vector<std::vector<std::vector<double>>> weights1 = {W1, W2};
    std::vector<std::vector<double>> inputs1 = {{1.0, 2.0}};
    
    auto out1 = forwardPass(topo1, inputs1, weights1);
    // Manual computation:
    // hidden sums: h1 = 1*0.5 + 2*0.2 + 1*0.1 = 0.5+0.4+0.1=1.0; tanh(1)=0.761594
    // h2 = 1*(-1.0) + 2*0.3 + 1*(-0.4) = -1+0.6-0.4=-0.8; tanh(-0.8)=-0.664037
    // bias neuron = 1.0
    // output sum = 0.761594*0.8 + (-0.664037)*(-0.2) + 1.0*0.5 = 0.609275 + 0.132807 + 0.5 = 1.242082
    // tanh(1.242082) = 0.845535
    assert(std::abs(out1[0][0] - 0.845535) < 1e-5);
    
    // Test 2: Single layer (input only) - should return input unchanged
    std::vector<int> topo2 = {3};
    std::vector<std::vector<double>> inputs2 = {{1.0, -2.0, 0.5}};
    std::vector<std::vector<std::vector<double>>> weights2; // no weights
    auto out2 = forwardPass(topo2, inputs2, weights2);
    assert(out2.size() == 1);
    assert(out2[0].size() == 3);
    assert(std::abs(out2[0][0] - 1.0) < 1e-9);
    assert(std::abs(out2[0][1] + 2.0) < 1e-9);
    assert(std::abs(out2[0][2] - 0.5) < 1e-9);
    
    // Test 3: Two-layer network (2 -> 1) with bias in input layer
    std::vector<int> topo3 = {2, 1};
    std::vector<std::vector<double>> W3 = {
        {0.2},
        {-0.5},
        {0.1}
    }; // from input1, input2, bias
    std::vector<std::vector<std::vector<double>>> weights3 = {W3};
    std::vector<std::vector<double>> inputs3 = {{0.0, 0.0}};
    auto out3 = forwardPass(topo3, inputs3, weights3);
    // sum = 0*0.2 + 0*(-0.5) + 1*0.1 = 0.1
    double expected3 = std::tanh(0.1); // ≈0.099668
    assert(std::abs(out3[0][0] - expected3) < 1e-5);
    
    // Test 4: Multiple inputs, check each output
    std::vector<int> topo4 = {2, 2, 1};
    std::vector<std::vector<double>> W4 = {
        {1.0, 0.0, 0.0},
        {0.0, 1.0, 0.0},
        {0.0, 0.0, 0.0}
    };
    std::vector<std::vector<double>> W5 = {
        {1.0},
        {1.0},
        {0.0}
    };
    std::vector<std::vector<std::vector<double>>> weights4 = {W4, W5};
    std::vector<std::vector<double>> inputs4 = {{1.0, 0.0}, {0.0, 1.0}, {2.0, 3.0}};
    auto out4 = forwardPass(topo4, inputs4, weights4);
    // For first input: hidden sums: h1 = 1*1 + 0*0 + 1*0 = 1 => tanh(1)=0.761594
    // h2 = 1*0 + 0*1 + 1*0 = 0 => tanh(0)=0
    // bias = 1.0
    // output sum = 0.761594*1 + 0*1 + 1*0 = 0.761594 => tanh = 0.64109
    assert(std::abs(out4[0][0] - 0.64109) < 1e-4);
    // For second input: h1 = 0*1 + 1*0 + 1*0 = 0 => 0
    // h2 = 0*0 + 1*1 + 1*0 = 1 => tanh(1)=0.761594
    // output sum = 0*1 + 0.761594*1 = 0.761594 => same
    assert(std::abs(out4[1][0] - 0.64109) < 1e-4);
    // For third input: h1 = 2*1 + 3*0 + 1*0 = 2 => tanh(2)=0.964028
    // h2 = 2*0 + 3*1 + 1*0 = 3 => tanh(3)=0.995055
    // output sum = 0.964028*1 + 0.995055*1 = 1.959083 => tanh(1.959083)=0.960214
    assert(std::abs(out4[2][0] - 0.960214) < 1e-4);
    
    // Test 5: Deeper network with 4 layers
    std::vector<int> topo5 = {1, 2, 2, 1};
    // First weight: from 1+1 bias to 2+1 bias => 2x3
    std::vector<std::vector<double>> W6 = {
        {0.1, 0.2, 0.0},
        {0.3, 0.4, 0.0}
    };
    // Second weight: from 2+1 bias to 2+1 bias => 3x3
    std::vector<std::vector<double>> W7 = {
        {0.5, -0.1, 0.0},
        {0.2, 0.7, 0.0},
        {-0.3, 0.4, 0.0}
    };
    // Third weight: from 2+1 bias to 1 => 3x1
    std::vector<std::vector<double>> W8 = {
        {0.9},
        {-0.2},
        {0.1}
    };
    std::vector<std::vector<std::vector<double>>> weights5 = {W6, W7, W8};
    std::vector<std::vector<double>> inputs5 = {{0.5}};
    auto out5 = forwardPass(topo5, inputs5, weights5);
    // Manual calculation:
    // Hidden layer1: input vector = [0.5, 1.0]
    // sum1[0] = 0.5*0.1 + 1.0*0.3 = 0.05+0.3=0.35 => tanh(0.35)=0.336376
    // sum1[1] = 0.5*0.2 + 1.0*0.4 = 0.1+0.4=0.5 => tanh(0.5)=0.462117
    // bias = 1.0
    // Hidden layer2: vector = [0.336376, 0.462117, 1.0]
    // sum2[0] = 0.336376*0.5 + 0.462117*0.2 + 1.0*(-0.3) = 0.168188+0.092423-0.3 = -0.039389 => tanh(-0.039389)=-0.039372
    // sum2[1] = 0.336376*(-0.1) + 0.462117*0.7 + 1.0*0.4 = -0.033638+0.323482+0.4=0.689844 => tanh(0.689844)=0.600666
    // bias = 1.0
    // Output sum = 0.039372*0.9 + 0.600666*(-0.2) + 1.0*0.1 = 0.035435 - 0.120133 + 0.1 = 0.015302
    // tanh(0.015302)=0.015299
    assert(out5[0].size() == 1);
    assert(std::abs(out5[0][0] - 0.015299) < 1e-4);
    
    return 0;
}
// The solution must implement matrix-vector multiplication for each layer transition. For a given input vector, we first create the input layer vector by appending a bias neuron with value 1.0 to the given input (if the topology has more than one layer). Then for each subsequent layer `i` (from 1 to topology.size()-1), we compute the new vector as the product of the previous layer vector and the weight matrix for that layer. The weight matrix has dimensions `(prev_layer_size) x (curr_layer_size)` where `prev_layer_size` includes the bias neuron if the previous layer was not the output layer, and `curr_layer_size` is the number of neurons in the current layer (which includes a bias neuron for all layers except the output). After computing the weighted sum, we apply the hyperbolic tangent to each element except for the bias neuron in hidden layers (the last element if the current layer is not the output layer). For the output layer, we apply tanh to all elements. Important edge cases: when the topology has exactly two layers (input and output), there is no bias neuron in the input layer (since we’re passing through unchanged? Actually the input layer always gets bias appended if there are more than one layer, but for output layer there is no bias). Also, the weight matrix sizes must match exactly; the function should assume valid input and not perform bounds checking. Time complexity: for each input, the forward pass is O(L * n^2) where L is number of layers and n is the maximum neuron count, because each matrix multiplication is O(prev_size * curr_size). Total time for I inputs is O(I * L * n^2). Space complexity is O(n) per input for the layer vectors, plus the output storage O(I * output_size).
