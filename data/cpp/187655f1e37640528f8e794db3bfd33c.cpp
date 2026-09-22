/*
Given an integer `input_size` and an integer `output_size`, write a C++ function named `make_linear_layer` that creates and returns a `struct LinearLayer` containing a dynamically allocated 2D weight matrix (size `input_size × output_size`) and a bias vector (size `output_size`), all initialized using a normal distribution with mean 0 and standard deviation 2. The function must respect that `input_size` and `output_size` are positive integers, and must use proper memory management (allocate with `new[]`, and provide a helper lambda/free function to deallocate). The returned struct should expose the weights in row-major order (first index = input neuron, second index = output neuron), and the bias as a flat array. The function must not rely on any external random-device-specific code; instead, use a fixed seed (e.g., `std::mt19937` seeded with 42) to make results deterministic. Also ensure that the function is `const`-correct where appropriate: the parameters are passed by value and not modified, and the returned struct holds raw pointers that the caller can free later. The function must be self-contained, including all necessary headers.
*/
#include <cstddef>
#include <random>
#include <stdexcept>

// A simple linear layer structure holding weights and bias.
struct LinearLayer {
    int input_size;
    int output_size;
    float* weights;  // row-major: input_size * output_size
    float* bias;     // output_size
};

// Free the memory allocated inside a LinearLayer.
void destroy_linear_layer(LinearLayer& layer) {
    delete[] layer.weights;
    delete[] layer.bias;
    layer.weights = nullptr;
    layer.bias = nullptr;
    layer.input_size = 0;
    layer.output_size = 0;
}

// Create a linear layer with weights and bias sampled from N(0, 2).
// Uses a fixed seed (42) for deterministic results.
LinearLayer make_linear_layer(int input_size, int output_size) {
    if (input_size <= 0 || output_size <= 0) {
        throw std::invalid_argument("Input and output sizes must be positive.");
    }

    LinearLayer layer;
    layer.input_size = input_size;
    layer.output_size = output_size;
    layer.weights = new float[input_size * output_size];
    layer.bias = new float[output_size];

    // Fixed seed for reproducibility
    std::mt19937 gen(42);
    std::normal_distribution<float> dist(0.0f, 2.0f);

    // Fill weights in row-major order
    for (int i = 0; i < input_size; ++i) {
        for (int j = 0; j < output_size; ++j) {
            layer.weights[i * output_size + j] = dist(gen);
        }
    }

    // Fill bias
    for (int j = 0; j < output_size; ++j) {
        layer.bias[j] = dist(gen);
    }

    return layer;
}
#include <cassert>
#include <cmath>

int main() {
    // Test 1: basic creation with positive sizes
    LinearLayer layer1 = make_linear_layer(2, 3);
    assert(layer1.input_size == 2);
    assert(layer1.output_size == 3);
    assert(layer1.weights != nullptr);
    assert(layer1.bias != nullptr);
    
    // Deterministic values: check first few elements against expected from the fixed seed.
    // These values are precomputed by running the reference code once.
    // (We'll just check that values are not all zero and are within a plausible range.)
    float sum = 0.0f;
    for (int i = 0; i < 2*3; ++i) sum += layer1.weights[i];
    assert(std::fabs(sum) > 0.0f); // Likely nonzero
    for (int i = 0; i < 3; ++i) {
        assert(std::fabs(layer1.bias[i]) <= 10.0f); // With std=2, ~99.7% within ±6, but use generous bound
    }
    
    // Test 2: ensure the same seed produces identical values (determinism)
    LinearLayer layer2 = make_linear_layer(2, 3);
    for (int i = 0; i < 6; ++i) {
        assert(layer1.weights[i] == layer2.weights[i]);
    }
    for (int i = 0; i < 3; ++i) {
        assert(layer1.bias[i] == layer2.bias[i]);
    }
    
    // Test 3: larger size, still deterministic and allocated correctly
    LinearLayer layer3 = make_linear_layer(4, 5);
    assert(layer3.input_size == 4);
    assert(layer3.output_size == 5);
    // Just verify that all weights are finite
    for (int i = 0; i < 20; ++i) {
        assert(std::isfinite(layer3.weights[i]));
    }
    
    // Test 4: shape with 1×1
    LinearLayer layer4 = make_linear_layer(1, 1);
    assert(layer4.input_size == 1);
    assert(layer4.output_size == 1);
    assert(std::isfinite(layer4.weights[0]));
    assert(std::isfinite(layer4.bias[0]));
    
    // Test 5: invalid sizes should throw
    bool threw = false;
    try {
        make_linear_layer(0, 3);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);
    
    threw = false;
    try {
        make_linear_layer(-2, 3);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);
    
    // Clean up
    destroy_linear_layer(layer1);
    destroy_linear_layer(layer2);
    destroy_linear_layer(layer3);
    destroy_linear_layer(layer4);
    
    return 0;
}
// The solution involves allocating two contiguous arrays: `weights` of size `input_size * output_size` and `bias` of size `output_size`. Use a `std::mt19937` generator seeded with a constant (e.g., 42) and a `std::normal_distribution<float>` with mean 0 and standard deviation 2 to generate values. Loop over all weight indices in row-major order and fill each element with `distribution(generator)`. Similarly fill the bias array. Since the seed is fixed, the output is deterministic across runs. Edge cases: if `input_size` or `output_size` is zero, the problem statement says positive, so we can assume they are >0; but to be safe, we could throw an exception or return empty arrays (but the specification says positive). Time complexity is `O(input_size * output_size)` because we fill each element once. Space complexity is `O(input_size * output_size + output_size)`. The returned struct should have fields `int input_size, output_size; float* weights; float* bias;` and possibly a destructor to free memory, but the task asks for a free function that returns the struct; we can provide a helper `free_linear_layer` that deletes the arrays. The solution must be const-correct: the function takes `int` by value (no modification), and const is not needed on the parameters since they are copies. The function itself is not const (it returns a non-const struct). We'll define `struct LinearLayer` with a default constructor that sets pointers to `nullptr`, and a destructor that frees them, to ensure RAII so the caller doesn't forget to free. However the task says "helper lambda/free function" so we can provide `void free_linear_layer(LinearLayer& layer)` or make the struct manage its own memory. To keep it simple, we'll provide a free function `void destroy_linear_layer(LinearLayer&)` that deletes the arrays and sets pointers to nullptr. The main algorithm is straightforward.
