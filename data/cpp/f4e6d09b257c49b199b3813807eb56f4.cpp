// Write a C++ function named `evaluateXor3` that simulates a feedforward neural network on a 3-input XOR task and returns a fitness value between 0 and 16. The function takes a constant reference to a fixed structure representing a neural network genome (defined below) and a constant reference to a configuration struct. The network has exactly 3 input nodes, 1 output node, and 5 hidden layers (each layer with 1 node) — total 4 hidden nodes in sequence. The activation function for all nodes is a sigmoid: `1.0 / (1.0 + exp(-x))`. The network processes 4 test cases sequentially: (1,0,1) → 1, (0,1,1) → 1, (0,0,1) → 0, (1,1,1) → 0. For each test, after setting the inputs, propagate through all hidden layers in order (each hidden node takes input from all previous hidden nodes and all input nodes, applies the sigmoid, and its output feeds forward; the output node takes input from all hidden nodes and all input nodes, applies the sigmoid, yielding a value in (0,1)). The network state is flushed (all node outputs reset to 0) before each test. Compute the total error as the sum of absolute differences between each output and the target. The fitness is `(4.0 - error) * (4.0 - error)`. The genome struct provides a list of weights and biases; the configuration provides constants. The function must be `const`-correct, take no side effects, and use only standard C++ libraries. The genome and configuration structs are defined in the same scope; you must implement the network propagation manually without any external neural network library. Edge case: the network may have weights that produce overflow in `exp`; clamp the pre-activation to [-50, 50] to avoid `inf`/`nan`. Time complexity is O(1) with fixed small sizes. Provide a self-contained implementation with the required structs as part of your solution (do not rely on external definitions). The function returns a `double`.
The solution models a simple layered neural network with 3 input nodes, 4 hidden nodes arranged in a single sequential chain (each hidden node connects to all inputs and all previous hidden nodes), and one output node that connects to all inputs and all hidden nodes. The genome holds a flat vector of weights plus bias for each node; the bias is added to the weighted sum before activation. We define a `Genome` struct containing `std::vector<double> weights` (ordered as: for each hidden node in order, first weights from all inputs then from all previous hidden nodes, then a single bias; then for the output node, weights from all inputs then from all hidden nodes, then a single bias) and a `Config` struct containing the number of inputs (3), hidden (4), outputs (1), and a depth (we ignore depth since we process exactly once). The main function `evaluateXor3` creates a vector of node outputs: `std::vector<double> hiddenOutputs(4, 0.0)` and `double output = 0.0`. For each test case, we flush by resetting hidden outputs and output to 0. We set inputs to the three values. Then for each hidden node `i` from 0 to 3, compute weighted sum: start with bias (weight index position after the input weights and previous hidden weights). We need to know the index offset: we maintain a running integer `weightIndex`. For hidden node `i`, we read `3 + i` weights (3 inputs + i previous hidden nodes) and one bias. The weighted sum = sum over inputs of `inputs[j] * weights[weightIndex++]` + sum over previous hidden nodes `hiddenOutputs[j] * weights[weightIndex++]` + bias `weights[weightIndex++]`. Apply sigmoid with clamping: `x = max(-50.0, min(50.0, sum)); hiddenOutputs[i] = 1.0 / (1.0 + exp(-x))`. After all hidden nodes, compute output node weighted sum: read `3 + 4 = 7` weights (3 inputs + 4 hidden) plus bias, sum inputs and hidden outputs, clamp, apply sigmoid to get `output`. The error accumulates absolute difference between output and target. After four tests, compute `error` and return `(4.0 - error)*(4.0 - error)`. This matches the fitness formula. Important edge cases: ensure zero initialization on flush; clamp to avoid overflow in `exp`; the weights vector must have exactly `(3+0 + 1) + (3+1 + 1) + (3+2 + 1) + (3+3 + 1) + (3+4 + 1) = 4 + 5 + 6 + 7 + 8 = 30` entries. If the weights vector is shorter, we can throw a `std::invalid_argument`. Time complexity is O(1) (fixed number of operations: 4 tests × (4 hidden nodes × up to 7 multiplications) + output multiplications ≈ constant), space complexity O(1) besides the input structures.
#include <vector>
#include <cmath>
#include <stdexcept>
#include <algorithm>

// Configuration constants for the XOR-3 network.
struct Config {
    size_t numInputs = 3;
    size_t numHidden = 4;
    size_t numOutputs = 1;
    double clampMin = -50.0;
    double clampMax = 50.0;
};

// A simple genome: a flat vector of weights and biases for all nodes.
struct Genome {
    std::vector<double> weights;
    // The order of weights:
    // For hidden node i (i=0..3):
    //   weights for all numInputs inputs, then weights for i previous hidden nodes, then 1 bias.
    // For the output node:
    //   weights for all numInputs inputs, then weights for all numHidden hidden nodes, then 1 bias.
    // Total size = (numInputs+0+1) + (numInputs+1+1) + ... + (numInputs+numHidden-1+1) + (numInputs+numHidden+1)
    // For default Config: (3+0+1)+(3+1+1)+(3+2+1)+(3+3+1)+(3+4+1)=4+5+6+7+8=30.
};

// Compute fitness for the 3-input XOR task.
double evaluateXor3(const Genome& genome, const Config& config) {
    const size_t nIn = config.numInputs;
    const size_t nHid = config.numHidden;
    const size_t nOut = config.numOutputs;

    // Calculate expected number of weights.
    size_t expectedWeights = 0;
    for (size_t i = 0; i < nHid; ++i) {
        expectedWeights += nIn + i + 1; // inputs + previous hidden + bias
    }
    expectedWeights += nIn + nHid + 1; // output node

    if (genome.weights.size() != expectedWeights) {
        throw std::invalid_argument("Genome weight vector has incorrect size");
    }

    // Sigmoid activation with clamping.
    auto sigmoid = [&](double x) -> double {
        x = std::max(config.clampMin, std::min(config.clampMax, x));
        return 1.0 / (1.0 + std::exp(-x));
    };

    // Test cases: {input0, input1, input2, target}
    const double tests[4][4] = {
        {1.0, 0.0, 1.0, 1.0},
        {0.0, 1.0, 1.0, 1.0},
        {0.0, 0.0, 1.0, 0.0},
        {1.0, 1.0, 1.0, 0.0}
    };

    double totalError = 0.0;

    for (int t = 0; t < 4; ++t) {
        // Flush the network: reset all hidden and output activations.
        std::vector<double> hiddenOutputs(nHid, 0.0);
        double output = 0.0;

        // Set inputs.
        std::vector<double> inputs(nIn);
        for (size_t i = 0; i < nIn; ++i) {
            inputs[i] = tests[t][i];
        }
        double target = tests[t][3];

        // Process hidden nodes in order.
        size_t weightIndex = 0;
        for (size_t hid = 0; hid < nHid; ++hid) {
            // Weighted sum from inputs.
            double sum = 0.0;
            for (size_t i = 0; i < nIn; ++i) {
                sum += inputs[i] * genome.weights[weightIndex++];
            }
            // Weighted sum from previous hidden nodes.
            for (size_t prev = 0; prev < hid; ++prev) {
                sum += hiddenOutputs[prev] * genome.weights[weightIndex++];
            }
            // Bias.
            sum += genome.weights[weightIndex++];

            hiddenOutputs[hid] = sigmoid(sum);
        }

        // Process output node.
        double outputSum = 0.0;
        for (size_t i = 0; i < nIn; ++i) {
            outputSum += inputs[i] * genome.weights[weightIndex++];
        }
        for (size_t h = 0; h < nHid; ++h) {
            outputSum += hiddenOutputs[h] * genome.weights[weightIndex++];
        }
        outputSum += genome.weights[weightIndex++];
        output = sigmoid(outputSum);

        totalError += std::fabs(output - target);
    }

    double fitness = (4.0 - totalError) * (4.0 - totalError);
    return fitness;
}
#include <cassert>
#include <cmath>
#include <vector>

// Include the solution code here (evaluateXor3, Config, Genome).

int main() {
    // Test 1: Perfect network (all weights zero, bias such that output is near target? 
    // With all weights zero and bias 0, output is 0.5 for all inputs. Error = 0.5*4 = 2, fitness = (4-2)^2=4.
    Config cfg;
    Genome g;
    g.weights = std::vector<double>(30, 0.0);
    double f = evaluateXor3(g, cfg);
    assert(std::fabs(f - 4.0) < 1e-9);

    // Test 2: Very large positive weights cause clamping; output saturates to ~1 for all, error= sum |1-target| = 1+1+1+1=4? Wait targets: 1,1,0,0 -> errors:0,0,1,1=2 -> fitness=(4-2)^2=4.
    // But with saturating to 1, errors: 0,0,1,1 =2, fitness=4.
    Genome g2;
    for (size_t i = 0; i < 30; ++i) g2.weights[i] = 100.0;
    double f2 = evaluateXor3(g2, cfg);
    // output saturates to 1, error=0+0+1+1=2, fitness=4
    assert(std::fabs(f2 - 4.0) < 1e-9);

    // Test 3: Very negative weights saturate output to 0, error=1+1+0+0=2, fitness=4
    Genome g3;
    for (size_t i = 0; i < 30; ++i) g3.weights[i] = -100.0;
    double f3 = evaluateXor3(g3, cfg);
    assert(std::fabs(f3 - 4.0) < 1e-9);

    // Test 4: Incorrect weight vector size should throw.
    Genome g4;
    g4.weights = std::vector<double>(29, 0.0);
    bool threw = false;
    try {
        evaluateXor3(g4, cfg);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    // Test 5: A network that outputs exactly target for all cases gives fitness 16.
    // This requires specific weights, but we can simulate by setting output directly? Not possible. 
    // But we can test that max fitness is limited to 16 when error=0. Use a tricky case:
    // If we set biases such that output is 1 for first two and 0 for last two? Not simple. Skip exact 16.
    // Instead check fitness range: for random weights, fitness in [0,16].
    Genome g5;
    g5.weights = {0.1, -0.2, 0.3, 0.4, -0.1, 0.2, 0.5, -0.3, 0.1, 0.2, -0.4, 0.6, 0.7, -0.2, 0.3, 0.0, 0.1, -0.5, 0.2, 0.3, 0.4, -0.1, 0.2, 0.5, -0.3, 0.1, 0.2, -0.4, 0.6, 0.7};
    double f5 = evaluateXor3(g5, cfg);
    assert(f5 >= 0.0 && f5 <= 16.0);

    // Test 6: Ensure flushing works: first test should not affect second. 
    // With a network that has memory? Our network has no recurrent links, but we flush anyway.
    // Just check that calling twice gives same result (deterministic) with same weights.
    double f6_first = evaluateXor3(g5, cfg);
    double f6_second = evaluateXor3(g5, cfg);
    assert(std::fabs(f6_first - f6_second) < 1e-12);

    return 0;
}
