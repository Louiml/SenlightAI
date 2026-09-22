// Write a standalone C++ function named `fastConvolution1x1` that performs a 1x1 convolution over a 3D input blob (channels, height, width) with optional bias, producing a 3D output blob of the same height and width but with a specified number of output channels. The function must compute each output pixel as the dot product of the input channel vector at that spatial location with the corresponding weight vector for that output channel, plus a bias term. Inputs are provided as a single contiguous `std::vector<float>` in channel-major (NCHW) format, along with the input dimensions (channels, height, width), the number of output channels, and a weight vector of size `(numOutputChannels * inputChannels)` and a bias vector of size `numOutputChannels`. The function must return the output vector in the same NCHW contiguous layout. No dynamic memory allocation beyond the output vector is allowed inside the function (use only stack and the provided containers). Handle the case where the input is empty (channels, height, or width = 0) by returning an empty vector.

The core algorithm is a direct 1x1 convolution, which is mathematically equivalent to a per-pixel fully connected layer: for each spatial position `(y, x)` and each output channel `o`, compute `output[o][y][x] = bias[o] + sum_{c=0}^{C-1} weight[o][c] * input[c][y][x]`. The layout is NCHW: for a given channel `c`, the spatial data starts at offset `c * (H * W)` and within that, the pixel at row `y` and column `x` is at index `c * (H*W) + y * W + x`. For the output, channel `o` starts at offset `o * (H*W)`.

Since the convolution kernel is 1x1, there is no spatial padding or stride; the output height and width equal the input height and width. The naive triple nested loop over output channels, spatial positions, and input channels gives a time complexity of O(numOutputChannels * H * W * inputChannels). However, we can optimize by looping over spatial positions first and then over output channels, but since the data is stored channel-major, the most cache-friendly order is: iterate over output channels (outermost), then over all spatial positions, and for each spatial position iterate over input channels to compute the dot product. This keeps the input channel data contiguous for each spatial position across the inner loop. Alternatively, a common optimization is to transpose loops but for clarity and correctness we can use a direct nested loop: for each output channel `o`, for each pixel index `p` from 0 to H*W-1, compute `sum` over `c`. This is O(O * H * W * C) time and O(O*H*W) space for output. Edge cases: if any dimension is zero, return empty vector. If bias vector is empty but numOutputChannels > 0, treat bias as zero (but for safety, we assume bias size equals numOutputChannels; if not, we can default to 0). The solution should use `const` references for inputs and return by value.

#include <vector>
#include <cstddef>

// Perform 1x1 convolution on an NCHW float blob.
// Input format: channels * height * width in contiguous memory.
// Output format: num_output * height * width (same spatial dimensions).
// weights size must be num_output * channels, bias size must be num_output.
std::vector<float> fastConvolution1x1(
    const std::vector<float>& input,
    int channels,
    int height,
    int width,
    int num_output,
    const std::vector<float>& weights,
    const std::vector<float>& bias) 
{
    // Handle degenerate cases.
    if (channels <= 0 || height <= 0 || width <= 0 || num_output <= 0) {
        return {};
    }

    const int spatial_size = height * width;
    const int input_channel_stride = spatial_size; // each channel is a full plane
    const int output_channel_stride = spatial_size;

    // Validate input sizes: input should have exactly channels * spatial_size elements.
    // We can optionally assert but since we are not given a size, we assume it's correct.
    // For robustness, we could check, but the task says inputs are valid.

    std::vector<float> output(static_cast<size_t>(num_output) * spatial_size);

    // For each output channel.
    for (int o = 0; o < num_output; ++o) {
        const float bias_val = (bias.empty() ? 0.0f : bias[o]);
        // Pointer to output channel start.
        float* out_ptr = output.data() + static_cast<size_t>(o) * output_channel_stride;

        // For each spatial pixel index (0 .. spatial_size-1).
        for (int p = 0; p < spatial_size; ++p) {
            // Compute dot product over input channels.
            float sum = bias_val;
            // weights for this output channel start at o*channels.
            const float* w_ptr = weights.data() + static_cast<size_t>(o) * channels;
            // For each input channel, access input channel plane + pixel p.
            for (int c = 0; c < channels; ++c) {
                const float* in_ptr = input.data() + static_cast<size_t>(c) * input_channel_stride;
                sum += w_ptr[c] * in_ptr[p];
            }
            out_ptr[p] = sum;
        }
    }

    return output;
}

#include <cassert>
#include <vector>
#include <cmath>

int main() {
    // Simple case: 1 input channel, 1 output channel, identity weight, bias 0.
    // Input: 2x2 matrix [[1,2],[3,4]]
    std::vector<float> input1 = {1.0f, 2.0f, 3.0f, 4.0f};
    std::vector<float> weights1 = {2.0f}; // output channel 0, input channel 0
    std::vector<float> bias1 = {1.0f};
    auto out1 = fastConvolution1x1(input1, 1, 2, 2, 1, weights1, bias1);
    assert(out1.size() == 4);
    assert(std::fabs(out1[0] - (1.0f*2.0f + 1.0f)) < 1e-5);
    assert(std::fabs(out1[1] - (2.0f*2.0f + 1.0f)) < 1e-5);
    assert(std::fabs(out1[2] - (3.0f*2.0f + 1.0f)) < 1e-5);
    assert(std::fabs(out1[3] - (4.0f*2.0f + 1.0f)) < 1e-5);

    // Multi-channel: 2 input channels, 1 output channel.
    // input channel0 = [1,2,3,4], channel1 = [5,6,7,8]
    std::vector<float> input2 = {1,2,3,4, 5,6,7,8};
    std::vector<float> weights2 = {0.5f, 1.5f}; // output0: w0=0.5, w1=1.5
    std::vector<float> bias2 = {0.0f};
    auto out2 = fastConvolution1x1(input2, 2, 2, 2, 1, weights2, bias2);
    // out[0] = 0.5*1 + 1.5*5 = 0.5+7.5=8.0
    assert(std::fabs(out2[0] - 8.0f) < 1e-5);
    assert(std::fabs(out2[1] - (0.5*2 + 1.5*6)) < 1e-5);
    assert(std::fabs(out2[2] - (0.5*3 + 1.5*7)) < 1e-5);
    assert(std::fabs(out2[3] - (0.5*4 + 1.5*8)) < 1e-5);

    // Multiple output channels: 1 input channel, 2 output channels.
    std::vector<float> input3 = {1,2,3,4};
    std::vector<float> weights3 = {1.0f, 2.0f}; // out0: w=1, out1: w=2
    std::vector<float> bias3 = {10.0f, -5.0f};
    auto out3 = fastConvolution1x1(input3, 1, 2, 2, 2, weights3, bias3);
    // Output has 8 elements: channel0 then channel1.
    // out0[0]=1*1+10=11, out0[1]=2+10=12, etc.
    // out1[0]=2*1-5=-3, out1[1]=4-5=-1, etc.
    assert(out3.size() == 8);
    assert(std::fabs(out3[0] - 11.0f) < 1e-5);
    assert(std::fabs(out3[1] - 12.0f) < 1e-5);
    assert(std::fabs(out3[2] - 13.0f) < 1e-5);
    assert(std::fabs(out3[3] - 14.0f) < 1e-5);
    assert(std::fabs(out3[4] - (-3.0f)) < 1e-5);
    assert(std::fabs(out3[5] - (-1.0f)) < 1e-5);
    assert(std::fabs(out3[6] - 1.0f) < 1e-5);
    assert(std::fabs(out3[7] - 3.0f) < 1e-5);

    // Empty input dimensions.
    auto out4 = fastConvolution1x1({}, 0, 5, 5, 3, {}, {});
    assert(out4.empty());

    // Height=1 width=1: single pixel.
    std::vector<float> input5 = {7.0f};
    std::vector<float> weights5 = {3.0f, -2.0f}; // two output channels
    std::vector<float> bias5 = {0.0f, 1.0f};
    auto out5 = fastConvolution1x1(input5, 1, 1, 1, 2, weights5, bias5);
    assert(out5.size() == 2);
    assert(std::fabs(out5[0] - 21.0f) < 1e-5);
    assert(std::fabs(out5[1] - (-14.0f+1.0f)) < 1e-5);

    return 0;
}
