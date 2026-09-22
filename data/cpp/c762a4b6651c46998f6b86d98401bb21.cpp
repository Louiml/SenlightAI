Write a C++ function named `computeDeconvOutputShape` that takes the spatial dimensions of a single input feature map (`input_height`, `input_width`), the kernel size (`kernel_height`, `kernel_width`), the stride (`stride_height`, `stride_width`), and the padding on each side (`pad_height`, `pad_width`), and returns a `std::pair<int, int>` containing the output height and width of a transposed convolution (deconvolution) layer. The formula for transposed convolution output size is `output = (input - 1) * stride - 2 * padding + kernel_size`. The function must handle two special cases: when the result would be non-positive (return `{0,0}` to indicate invalid dimensions) and when the kernel size is even, where the standard formula may produce ambiguous sizes (in such cases, use the standard formula as-is since no additional output padding is specified). The function should be `const`-correct, use only standard library headers, and not rely on any external framework or global state.
#include <cassert>
#include <utility>

// Declaration of the solution function (normally would be in a header)
std::pair<int, int> computeDeconvOutputShape(
    int input_height, int input_width,
    int kernel_height, int kernel_width,
    int stride_height, int stride_width,
    int pad_height, int pad_width);

int main() {
    // Basic case: stride 1, no padding, kernel 3x3 -> output equals input + 2
    auto result = computeDeconvOutputShape(5, 5, 3, 3, 1, 1, 0, 0);
    assert(result.first == 7 && result.second == 7);

    // Stride 2, padding 1, kernel 4x4 -> output = (in-1)*2 -2 + 4 = 2*in
    result = computeDeconvOutputShape(4, 4, 4, 4, 2, 2, 1, 1);
    assert(result.first == 8 && result.second == 8);

    // Asymmetric dimensions and padding
    result = computeDeconvOutputShape(6, 8, 5, 3, 2, 3, 1, 2);
    // height: (6-1)*2 - 2*1 + 5 = 10 - 2 + 5 = 13
    // width:  (8-1)*3 - 2*2 + 3 = 21 - 4 + 3 = 20
    assert(result.first == 13 && result.second == 20);

    // Zero input dimension -> invalid -> returns {0,0}
    result = computeDeconvOutputShape(0, 5, 3, 3, 1, 1, 0, 0);
    assert(result.first == 0 && result.second == 0);

    // Non-positive output due to excessive padding
    result = computeDeconvOutputShape(2, 2, 1, 1, 1, 1, 2, 2);
    // height: (2-1)*1 - 2*2 + 1 = 1 - 4 + 1 = -2 -> invalid
    assert(result.first == 0 && result.second == 0);

    // Even kernel with stride 1 and zero padding
    result = computeDeconvOutputShape(7, 3, 2, 2, 1, 1, 0, 0);
    assert(result.first == 8 && result.second == 4);

    // Large dimensions and stride
    result = computeDeconvOutputShape(100, 200, 5, 7, 3, 4, 1, 1);
    // height: (99)*3 - 2 + 5 = 297 + 3 = 300
    // width: (199)*4 - 2 + 7 = 796 + 5 = 801
    assert(result.first == 300 && result.second == 801);

    // Single pixel input, stride 2, no padding, kernel 1
    result = computeDeconvOutputShape(1, 1, 1, 1, 2, 2, 0, 0);
    // height: (0)*2 - 0 + 1 = 1
    assert(result.first == 1 && result.second == 1);

    // Single pixel input, stride 2, kernel 3, no padding
    result = computeDeconvOutputShape(1, 1, 3, 3, 2, 2, 0, 0);
    // height: 0*2 - 0 + 3 = 3
    assert(result.first == 3 && result.second == 3);
}
#include <utility> // for std::pair

// Compute output spatial dimensions for a transposed convolution layer.
// Returns {out_height, out_width}. If any computed dimension is <= 0, returns {0, 0}.
std::pair<int, int> computeDeconvOutputShape(
    int input_height, int input_width,
    int kernel_height, int kernel_width,
    int stride_height, int stride_width,
    int pad_height, int pad_width) {
    
    // Transposed convolution output size formula:
    // out = (in - 1) * stride - 2 * pad + kernel
    int out_height = (input_height - 1) * stride_height - 2 * pad_height + kernel_height;
    int out_width  = (input_width - 1) * stride_width  - 2 * pad_width  + kernel_width;
    
    // Validate: invalid if any dimension is non-positive
    if (out_height <= 0 || out_width <= 0) {
        return {0, 0};
    }
    return {out_height, out_width};
}
// The core algorithm is a direct application of the transposed convolution output size formula: `out_dim = (in_dim - 1) * stride - 2 * padding + kernel`. This derives from the inverse of the standard convolution output formula `out = (in + 2*pad - kernel)/stride + 1` when solved for the input size of a convolution that would produce the given output. Important edge cases: (1) If the computed dimension is less than or equal to zero, the configuration is invalid, so return `{0,0}` for both values to signal an error. (2) Even kernel sizes are allowed; the formula still applies, but without explicit output_padding the actual output may vary by implementation, so we use the standard formula. (3) All inputs are assumed to be non-negative integers; no validation is performed. Time complexity is O(1) as we only perform a few arithmetic operations. Space complexity is O(1) since we return a fixed-size pair.
