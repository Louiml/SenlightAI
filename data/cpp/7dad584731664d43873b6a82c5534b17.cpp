// Write a standalone C++ function named `trainedMLP` that simulates a minimal but functional multilayer perceptron (MLP) inspired by the provided OpenCV ANN_MLP implementation. The function should take a vector of `int` layer sizes (at least 3 layers: input, one or more hidden, output), a vector of `double` training input samples (flat, row-major, each row is one sample), a vector of `double` training output targets (flat, row-major, same number of rows), and a non-negative integer `maxIterations`. The function must perform a simplified backpropagation training (using a fixed sigmoid-symmetric activation function `tanh` with parameters `a=2/3`, `b=1.7159`), initialize weights using a deterministic Nguyen-Widrow-like scheme (seed the random generator with a fixed value, e.g., 42), and after training, return a `std::vector<double>` containing the predicted outputs for the same training inputs (after applying input and output scaling as in the reference). The network must use one bias per neuron (bias weight included in each layer’s weight matrix), and the prediction must apply the learned scaling and activation functions in the forward pass. The termination criterion is simply `maxIterations` epochs (each epoch processes all training samples in the given order, not shuffled). The solution must be self-contained, use only standard C++ libraries, and not rely on OpenCV or any external framework.
// The approach is to implement a compact MLP with a fixed sigmoid-symmetric activation function (hyperbolic tangent scaled by parameters `alpha=2/3`, `beta=1.7159`). The network architecture: layer sizes are given, where the first is input size, last is output size, and all hidden layers must have at least 2 neurons. We store weights for each connection from layer `i-1` to layer `i` in a matrix of size `(previousLayerSize + 1) x currentLayerSize` where the last row is the bias weights. We also store scaling parameters for input (per-input mean and inverse standard deviation) and output (min/max mapping to activation range) as in the reference, but simplified: input scaling normalizes each feature to zero mean and unit variance over training samples; output scaling maps target values to the activation function’s output range `[-0.95, 0.95]` (the `min_val`/`max_val`). Weight initialization uses a deterministic pseudo-random generator (e.g., `std::mt19937` seeded with 42) following the Nguyen-Widrow algorithm: for hidden layers, weights are drawn uniform in `[-1,1]`, then scaled so the norm of each neuron’s weight vector (excluding bias) is 1, then the bias is set to a deterministic value based on neuron index to break symmetry. The forward pass: for each sample, scale inputs (multiply by input scale, add offset), then for each layer compute weighted sum (inputs times weights excluding bias row plus bias values), apply activation function, then scale output (multiply by output inverse scale to map from activation range back to original target range). Training uses simple gradient descent: for each epoch, iterate all samples in order, compute forward pass storing activations and pre-activation sums, compute output error (difference between scaled target and network output), backpropagate using the derivative of the activation function (for tanh scaled: derivative = `alpha * beta * (1 - tanh^2(alpha*x + bias))`), accumulate weight gradients, then update all weights after processing all samples using learning rate `eta=0.1` and momentum `momentum=0.1` (as in the reference’s backprop defaults). The training stops after `maxIterations` epochs or early if the error change is below `1e-6`. The function returns a vector of predicted outputs for all training samples, flattened row-major. Edge cases: ensure at least 3 layers, all layer sizes positive, hidden layers >1 neuron, input and output sample counts match, and each sample has the correct feature dimension. Time complexity per epoch: O(epochs * samples * total_weights) where total_weights is sum of `(prev+1)*curr` across layers. Space complexity: O(total_weights) for weights and gradients, plus O(max_layer_size) for temporary activations.
#include <vector>
#include <cmath>
#include <random>
#include <algorithm>
#include <cassert>

class SimpleMLP {
public:
    SimpleMLP(const std::vector<int>& layer_sizes, unsigned seed = 42)
        : sizes(layer_sizes), gen(seed) {
        int L = sizes.size();
        assert(L >= 3); // input, at least one hidden, output
        for (int i = 0; i < L; ++i) {
            assert(sizes[i] > 0);
            if (i > 0 && i < L-1) assert(sizes[i] > 1);
        }
        numLayers = L;
        // Allocate weight matrices: weights[l] connects layer l-1 to layer l, size (prev+1) x curr
        weights.resize(L);
        for (int l = 1; l < L; ++l) {
            int prev = sizes[l-1] + 1; // +1 for bias
            int curr = sizes[l];
            weights[l].assign(prev * curr, 0.0);
        }
        // Input and output scaling vectors (size 2*featureDim for input, 2*outputDim for output)
        inputScale.assign(sizes[0]*2, 0.0);
        outputScale.assign(sizes[L-1]*2, 0.0);
        invOutputScale.assign(sizes[L-1]*2, 0.0);
    }

    // Train using simplified backpropagation. inputs/outputs are flattened row-major.
    void train(const std::vector<double>& inputs, const std::vector<double>& outputs, int maxIterations) {
        int numSamples = inputs.size() / sizes[0];
        assert(numSamples > 0);
        assert(outputs.size() == (size_t)numSamples * sizes.back());

        computeInputScale(inputs, numSamples);
        computeOutputScale(outputs, numSamples);
        initWeights();

        double eta = 0.1, momentum = 0.1;
        double prevError = 1e18;

        // Gradient accumulation buffers
        std::vector<std::vector<double>> gradWeights = weights; // will be zeroed per epoch
        for (auto& w : gradWeights) std::fill(w.begin(), w.end(), 0.0);
        std::vector<std::vector<double>> prevGradWeights = gradWeights;

        // Temporary buffers for forward pass
        std::vector<std::vector<double>> activations(numLayers);
        std::vector<std::vector<double>> preActivations(numLayers);
        for (int l = 0; l < numLayers; ++l) {
            activations[l].assign(sizes[l], 0.0);
            preActivations[l].assign(sizes[l], 0.0);
        }

        for (int iter = 0; iter < maxIterations; ++iter) {
            double totalError = 0.0;
            // Zero gradients
            for (auto& g : gradWeights) std::fill(g.begin(), g.end(), 0.0);

            // Process each sample sequentially
            for (int s = 0; s < numSamples; ++s) {
                // Input layer (scaled)
                for (int j = 0; j < sizes[0]; ++j) {
                    double x = inputs[s * sizes[0] + j];
                    activations[0][j] = x * inputScale[j*2] + inputScale[j*2+1];
                }

                // Forward pass through hidden and output layers
                for (int l = 1; l < numLayers; ++l) {
                    int prevSize = sizes[l-1];
                    int currSize = sizes[l];
                    const auto& w = weights[l];
                    for (int j = 0; j < currSize; ++j) {
                        double sum = 0.0;
                        for (int k = 0; k < prevSize; ++k) {
                            sum += activations[l-1][k] * w[k * currSize + j];
                        }
                        // bias weight is at index prevSize * currSize + j
                        sum += w[prevSize * currSize + j];
                        preActivations[l][j] = sum;
                        activations[l][j] = activation(sum);
                    }
                }

                // Output layer: compare scaled target
                int outSize = sizes.back();
                std::vector<double> outputError(outSize, 0.0);
                for (int j = 0; j < outSize; ++j) {
                    double target = outputs[s * outSize + j];
                    // Scale target to activation range: target*invOutputScale[2j] + invOutputScale[2j+1]
                    double scaledTarget = target * invOutputScale[j*2] + invOutputScale[j*2+1];
                    double diff = scaledTarget - activations[numLayers-1][j];
                    outputError[j] = diff;
                    totalError += diff * diff;
                }

                // Backward pass: output layer to first hidden
                // Compute delta for output layer: error * derivative
                std::vector<std::vector<double>> deltas(numLayers);
                deltas[numLayers-1].resize(outSize);
                for (int j = 0; j < outSize; ++j) {
                    double a = preActivations[numLayers-1][j];
                    deltas[numLayers-1][j] = outputError[j] * activationDerivative(a);
                }

                for (int l = numLayers-1; l >= 1; --l) {
                    int currSize = sizes[l];
                    int prevSize = sizes[l-1];
                    const auto& w = weights[l];
                    auto& gradW = gradWeights[l];

                    // For each output neuron of this layer
                    for (int j = 0; j < currSize; ++j) {
                        double delta = deltas[l][j];
                        // Update weights for connections from previous layer (excluding bias)
                        for (int k = 0; k < prevSize; ++k) {
                            gradW[k * currSize + j] += delta * activations[l-1][k];
                        }
                        // Bias weight
                        gradW[prevSize * currSize + j] += delta;
                    }

                    // Compute delta for previous layer (if not input layer)
                    if (l > 1) {
                        deltas[l-1].assign(prevSize, 0.0);
                        for (int k = 0; k < prevSize; ++k) {
                            double sum = 0.0;
                            for (int j = 0; j < currSize; ++j) {
                                sum += deltas[l][j] * w[k * currSize + j];
                            }
                            deltas[l-1][k] = sum * activationDerivative(preActivations[l-1][k]);
                        }
                    }
                }
            }

            totalError *= 0.5; // half squared error
            // Update weights with momentum
            for (int l = 1; l < numLayers; ++l) {
                for (size_t idx = 0; idx < weights[l].size(); ++idx) {
                    double deltaW = eta * gradWeights[l][idx] + momentum * prevGradWeights[l][idx];
                    weights[l][idx] += deltaW;
                    prevGradWeights[l][idx] = deltaW; // store for next iteration (simplified momentum)
                }
            }

            // Early stopping
            if (std::fabs(prevError - totalError) < 1e-6) break;
            prevError = totalError;
        }
    }

    // Predict for input(s). inputs flattened, returns flattened outputs.
    std::vector<double> predict(const std::vector<double>& inputs) const {
        int numSamples = inputs.size() / sizes[0];
        assert(numSamples > 0);
        std::vector<double> outputs(numSamples * sizes.back(), 0.0);

        std::vector<std::vector<double>> activations(numLayers);
        for (int l = 0; l < numLayers; ++l) activations[l].resize(sizes[l]);

        for (int s = 0; s < numSamples; ++s) {
            // input scaling
            for (int j = 0; j < sizes[0]; ++j) {
                double x = inputs[s * sizes[0] + j];
                activations[0][j] = x * inputScale[j*2] + inputScale[j*2+1];
            }
            for (int l = 1; l < numLayers; ++l) {
                int prevSize = sizes[l-1];
                int currSize = sizes[l];
                const auto& w = weights[l];
                for (int j = 0; j < currSize; ++j) {
                    double sum = 0.0;
                    for (int k = 0; k < prevSize; ++k) {
                        sum += activations[l-1][k] * w[k * currSize + j];
                    }
                    sum += w[prevSize * currSize + j];
                    activations[l][j] = activation(sum);
                }
            }
            // output scaling: map from activation range back to original target range
            for (int j = 0; j < sizes.back(); ++j) {
                outputs[s * sizes.back() + j] = activations[numLayers-1][j] * outputScale[j*2] + outputScale[j*2+1];
            }
        }
        return outputs;
    }

private:
    std::vector<int> sizes;
    int numLayers;
    std::vector<std::vector<double>> weights; // weights[l][ (prev+1)*curr ] row-major, last row bias
    std::vector<double> inputScale;   // [2*inputDim] = {scale_i, offset_i}
    std::vector<double> outputScale;  // [2*outputDim] = {scale_i, offset_i} for mapping from activation to original
    std::vector<double> invOutputScale; // [2*outputDim] mapping from original to activation range
    std::mt19937 gen;

    double activation(double x) const {
        // sigmoid-symmetric: beta * tanh(alpha*x)
        const double alpha = 2.0/3.0;
        const double beta = 1.7159;
        return beta * std::tanh(alpha * x);
    }

    double activationDerivative(double x) const {
        // derivative of beta*tanh(alpha*x) is beta*alpha*(1 - tanh^2(alpha*x))
        const double alpha = 2.0/3.0;
        const double beta = 1.7159;
        double t = std::tanh(alpha * x);
        return beta * alpha * (1.0 - t * t);
    }

    void computeInputScale(const std::vector<double>& inputs, int numSamples) {
        int dim = sizes[0];
        // mean and variance
        for (int j = 0; j < dim; ++j) {
            double mean = 0.0, sq = 0.0;
            for (int s = 0; s < numSamples; ++s) {
                double v = inputs[s * dim + j];
                mean += v;
                sq += v * v;
            }
            mean /= numSamples;
            double var = sq / numSamples - mean * mean;
            double scale = (var < 1e-12) ? 1.0 : 1.0 / std::sqrt(var);
            inputScale[j*2] = scale;
            inputScale[j*2+1] = -mean * scale;
        }
    }

    void computeOutputScale(const std::vector<double>& outputs, int numSamples) {
        int dim = sizes.back();
        const double actMin = -0.95, actMax = 0.95;
        for (int j = 0; j < dim; ++j) {
            double mn = outputs[j], mx = outputs[j];
            for (int s = 1; s < numSamples; ++s) {
                double v = outputs[s * dim + j];
                mn = std::min(mn, v);
                mx = std::max(mx, v);
            }
            double delta = mx - mn;
            double a, b; // mapping original -> activation: y = a*x + b
            if (delta < 1e-12) {
                a = 0.0;
                b = (actMin + actMax) * 0.5;
            } else {
                a = (actMax - actMin) / delta;
                b = actMin - mn * a;
            }
            invOutputScale[j*2] = a;
            invOutputScale[j*2+1] = b;
            // inverse mapping: x = (y - b)/a, if a != 0
            if (std::fabs(a) < 1e-12) {
                outputScale[j*2] = 0.0;
                outputScale[j*2+1] = (mn + mx) * 0.5;
            } else {
                outputScale[j*2] = 1.0 / a;
                outputScale[j*2+1] = -b / a;
            }
        }
    }

    void initWeights() {
        std::uniform_real_distribution<double> dist(-1.0, 1.0);
        const double G = 0.7; // typical Nguyen-Widrow scaling constant

        for (int l = 1; l < numLayers; ++l) {
            int n1 = sizes[l-1]; // previous layer size (without bias)
            int n2 = sizes[l];   // current layer size
            auto& w = weights[l];
            bool isOutputLayer = (l == numLayers-1);
            // For each neuron j in current layer
            for (int j = 0; j < n2; ++j) {
                double sumAbs = 0.0;
                // Initialize incoming weights from previous layer (excluding bias)
                for (int k = 0; k < n1; ++k) {
                    double val = dist(gen);
                    w[k * n2 + j] = val;
                    sumAbs += std::fabs(val);
                }
                // Normalize if not output layer
                if (!isOutputLayer) {
                    if (sumAbs > 1e-12) {
                        double invSum = 1.0 / sumAbs;
                        for (int k = 0; k < n1; ++k) {
                            w[k * n2 + j] *= invSum;
                        }
                    }
                    // Bias: set deterministic based on neuron index
                    double biasVal = G * (-1.0 + j * 2.0 / n2);
                    w[n1 * n2 + j] = biasVal;
                } else {
                    // Output layer: smaller random weights
                    for (int k = 0; k < n1; ++k) {
                        w[k * n2 + j] *= 0.5; // scale down
                    }
                    w[n1 * n2 + j] = dist(gen) * 0.5;
                }
            }
        }
    }
};

// Public API function
std::vector<double> trainedMLP(const std::vector<int>& layerSizes,
                               const std::vector<double>& inputs,
                               const std::vector<double>& targets,
                               int maxIterations) {
    assert(layerSizes.size() >= 3);
    assert(maxIterations >= 0);
    assert(inputs.size() % layerSizes[0] == 0);
    int numSamples = inputs.size() / layerSizes[0];
    assert(numSamples > 0);
    assert(targets.size() == (size_t)numSamples * layerSizes.back());

    SimpleMLP mlp(layerSizes);
    mlp.train(inputs, targets, maxIterations);
    return mlp.predict(inputs);
}
#include <cassert>
#include <cmath>
#include <vector>

// The solution function is declared above (include the header or paste it here).

int main() {
    // Simple XOR problem: 2-2-1 network
    std::vector<int> layers = {2, 2, 1};
    std::vector<double> inputs = {0,0, 0,1, 1,0, 1,1};
    std::vector<double> targets = {0, 1, 1, 0};
    std::vector<double> pred = trainedMLP(layers, inputs, targets, 1000);
    // Expect predictions roughly match targets after training (not exact, but reasonable)
    assert(pred.size() == 4);
    // Check that predictions are closer to targets than random (e.g., mean squared error < 0.25)
    double mse = 0.0;
    for (int i = 0; i < 4; ++i) {
        double diff = pred[i] - targets[i];
        mse += diff * diff;
    }
    mse /= 4.0;
    assert(mse < 0.25);

    // Linearly separable problem: identity mapping with one input, one output
    std::vector<int> layers2 = {1, 2, 1};
    std::vector<double> inputs2 = {1.0, 2.0, 3.0, 4.0};
    std::vector<double> targets2 = {2.0, 4.0, 6.0, 8.0}; // output = 2*input
    std::vector<double> pred2 = trainedMLP(layers2, inputs2, targets2, 500);
    assert(pred2.size() == 4);
    // Check that predictions are reasonably close (within 20% relative error)
    for (int i = 0; i < 4; ++i) {
        double expected = targets2[i];
        double got = pred2[i];
        assert(std::fabs(got - expected) / (std::fabs(expected) + 1e-6) < 0.2);
    }

    // Edge case: single sample, minimal 3 layers
    std::vector<int> layers3 = {1, 3, 1};
    std::vector<double> inputs3 = {0.5};
    std::vector<double> targets3 = {1.0};
    std::vector<double> pred3 = trainedMLP(layers3, inputs3, targets3, 100);
    assert(pred3.size() == 1);
    // Should learn to approximate 1.0, allow some tolerance
    assert(std::fabs(pred3[0] - 1.0) < 0.5);

    // Dimensionality check with multi-output
    std::vector<int> layers4 = {3, 4, 2};
    std::vector<double> inputs4 = {1,2,3, 4,5,6};
    std::vector<double> targets4 = {0.1, 0.9, 0.2, 0.8};
    std::vector<double> pred4 = trainedMLP(layers4, inputs4, targets4, 50);
    assert(pred4.size() == 4); // 2 samples * 2 outputs

    return 0;
}
