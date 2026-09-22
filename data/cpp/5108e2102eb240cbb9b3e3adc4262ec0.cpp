// Write a standalone C++ function that implements the Space-to-Depth transformation for a 4D tensor (batch, height, width, depth) without using any TensorFlow Lite dependencies. The function should take as input a flat array of data (either `float` or `int8_t`), the input tensor dimensions (batch size, input height, input width, input depth), and a positive integer `block_size` such that both input height and width are divisible by `block_size`. It should output a flat array of the same data type with dimensions (batch, output_height, output_width, output_depth), where `output_height = input_height / block_size`, `output_width = input_width / block_size`, and `output_depth = input_depth * block_size * block_size`. The transformation rearranges data by moving spatial blocks of size `block_size × block_size` from the height and width dimensions into the depth dimension, preserving the order within each block (row-major within the block, and blocks scanned row-major over the spatial grid). The function must be templated on the element type to support both `float` and `int8_t`, and must handle edge cases like `block_size = 1` (identity) and single-element blocks. Provide the function signature: `template<typename T> void SpaceToDepth(const T* input, const int batch, const int input_height, const int input_width, const int input_depth, const int block_size, T* output)`. The function should perform validation using `assert` for dimensions and block_size, but no dynamic memory allocation is needed.
#include <cassert>
#include <cstdint>

// Include the solution function here (or link it).
// For brevity, the function definition is assumed to be available.

int main() {
    // Test case 1: block_size = 1 (identity).
    {
        float input[4] = {1.0f, 2.0f, 3.0f, 4.0f};
        float output[4] = {0.0f};
        SpaceToDepth<float>(input, 1, 2, 2, 1, 1, output);
        assert(output[0] == 1.0f);
        assert(output[1] == 2.0f);
        assert(output[2] == 3.0f);
        assert(output[3] == 4.0f);
    }

    // Test case 2: simple 2x2 spatial to depth, block_size=2, depth=1.
    {
        float input[4] = {1.0f, 2.0f, 3.0f, 4.0f}; // row-major: [[1,2],[3,4]]
        float output[4] = {0.0f};
        // Output should have shape [1,1,1,4] = [1,2,3,4] in order.
        SpaceToDepth<float>(input, 1, 2, 2, 1, 2, output);
        assert(output[0] == 1.0f);
        assert(output[1] == 2.0f);
        assert(output[2] == 3.0f);
        assert(output[3] == 4.0f);
    }

    // Test case 3: 4x4 spatial, block_size=2, depth=2.
    // Input with depth=2 means each pixel has 2 values.
    // We'll construct a pattern where each spatial position (r,c) has values r*100 + c*10 + d (d=0,1).
    {
        const int H = 4, W = 4, D = 2, BS = 2;
        float input[H * W * D];
        for (int r = 0; r < H; ++r)
            for (int c = 0; c < W; ++c)
                for (int d = 0; d < D; ++d)
                    input[(r * W + c) * D + d] = r * 100 + c * 10 + d;

        int outH = H / BS, outW = W / BS, outD = D * BS * BS; // 2,2,8
        float output[outH * outW * outD];
        SpaceToDepth<float>(input, 1, H, W, D, BS, output);

        // Check a few positions manually.
        // Output (0,0) should contain block covering input rows 0-1, cols 0-1.
        // Ordered: for each block cell (by,bx) in row-major, then depth.
        // by=0,bx=0 -> (0,0) depth0=0, depth1=1
        // by=0,bx=1 -> (0,1) depth0=10, depth1=11
        // by=1,bx=0 -> (1,0) depth0=100, depth1=101
        // by=1,bx=1 -> (1,1) depth0=110, depth1=111
        float expected[8] = {0,1,10,11,100,101,110,111};
        for (int i = 0; i < 8; ++i)
            assert(output[i] == expected[i]);

        // Check output (1,1) block: covers input rows 2-3, cols 2-3.
        // by=0,bx=0 -> (2,2) depth0=220, depth1=221
        // by=0,bx=1 -> (2,3) depth0=230, depth1=231
        // by=1,bx=0 -> (3,2) depth0=320, depth1=321
        // by=1,bx=1 -> (3,3) depth0=330, depth1=331
        float expected2[8] = {220,221,230,231,320,321,330,331};
        int base = (1 * outH + 1) * outW * outD; // batch=0, oh=1, ow=1, then depth
        for (int i = 0; i < 8; ++i)
            assert(output[base + i] == expected2[i]);
    }

    // Test case 4: int8_t type, batch=2, block_size=2, depth=1, input 4x4 each.
    {
        const int B = 2, H = 4, W = 4, D = 1, BS = 2;
        int8_t input[B * H * W * D];
        for (int b = 0; b < B; ++b)
            for (int r = 0; r < H; ++r)
                for (int c = 0; c < W; ++c)
                    input[((b * H + r) * W + c) * D + 0] = static_cast<int8_t>(b * 100 + r * 10 + c);

        int outH = H / BS, outW = W / BS, outD = D * BS * BS; // 2,2,4
        int8_t output[B * outH * outW * outD];
        SpaceToDepth<int8_t>(input, B, H, W, D, BS, output);

        // For batch 0, spatial (0,0) block should be [0,1,10,11] (r,c order).
        assert(output[0] == 0);
        assert(output[1] == 1);
        assert(output[2] == 10);
        assert(output[3] == 11);

        // For batch 1, spatial (1,1) block yields input rows 2-3, cols 2-3.
        // Values: r=2,c=2 -> 122; r=2,c=3 -> 123; r=3,c=2 -> 132; r=3,c=3 -> 133.
        // Output index: batch=1, oh=1, ow=1, depth=0..3.
        int idx = ((1 * outH + 1) * outW + 1) * outD; // offset for batch 1, oh=1,ow=1
        assert(output[idx + 0] == 122);
        assert(output[idx + 1] == 123);
        assert(output[idx + 2] == 132);
        assert(output[idx + 3] == 133);
    }

    // Test case 5: block_size larger than 1, depth > 1, single element batch.
    {
        const int H = 2, W = 2, D = 3, BS = 2;
        float input[H * W * D];
        // Fill with sequential numbers: 0,1,2,3,... (12 elements total).
        for (int i = 0; i < H * W * D; ++i) input[i] = static_cast<float>(i);
        float output[D * BS * BS]; // = 12
        SpaceToDepth<float>(input, 1, H, W, D, BS, output);
        // Since block covers entire spatial grid, output depth order is:
        // For each (by,bx) in row-major, then depth.
        // (0,0): depth 0,1,2 from input[0], [1], [2] -> values 0,1,2
        // (0,1): depth from input[3],[4],[5] -> 3,4,5
        // (1,0): from input[6],[7],[8] -> 6,7,8
        // (1,1): from input[9],[10],[11] -> 9,10,11
        for (int i = 0; i < 12; ++i)
            assert(output[i] == static_cast<float>(i));
    }

    return 0;
}
#include <cassert>

/**
 * Performs the Space-to-Depth transformation on a 4D tensor stored in row-major
 * order.
 *
 * @tparam T Element type (e.g., float, int8_t).
 * @param input Pointer to the input tensor data.
 * @param batch Number of batches.
 * @param input_height Height of input (must be divisible by block_size).
 * @param input_width Width of input (must be divisible by block_size).
 * @param input_depth Number of depth channels.
 * @param block_size Size of spatial block (must be positive).
 * @param output Pointer to the output tensor data (pre-allocated by caller).
 */
template<typename T>
void SpaceToDepth(const T* input,
                  const int batch,
                  const int input_height,
                  const int input_width,
                  const int input_depth,
                  const int block_size,
                  T* output) {
    // Validate dimensions.
    assert(batch > 0);
    assert(input_height > 0);
    assert(input_width > 0);
    assert(input_depth > 0);
    assert(block_size > 0);
    assert(input_height % block_size == 0);
    assert(input_width % block_size == 0);

    const int output_height = input_height / block_size;
    const int output_width = input_width / block_size;
    const int output_depth = input_depth * block_size * block_size;

    // Iterate over all output elements.
    for (int b = 0; b < batch; ++b) {
        for (int oh = 0; oh < output_height; ++oh) {
            for (int ow = 0; ow < output_width; ++ow) {
                for (int od = 0; od < output_depth; ++od) {
                    // Decompose output depth into block offset and original depth.
                    const int d = od % input_depth;
                    const int block_offset = od / input_depth;
                    const int by = block_offset / block_size; // row inside block
                    const int bx = block_offset % block_size; // col inside block

                    // Map to input spatial coordinates.
                    const int ih = oh * block_size + by;
                    const int iw = ow * block_size + bx;

                    // Compute source index in input.
                    const int input_index =
                        ((b * input_height + ih) * input_width + iw) * input_depth + d;

                    // Compute destination index in output.
                    const int output_index =
                        ((b * output_height + oh) * output_width + ow) * output_depth + od;

                    output[output_index] = input[input_index];
                }
            }
        }
    }
}
// The core algorithm iterates over every output element and computes its source index in the input. Given output coordinates `(b, oh, ow, od)`, we need to determine which spatial block and which position within that block it corresponds to. Let `block_size = bs`. The output depth `od` can be decomposed as `od = (by * bs + bx) * input_depth + d`, where `by` and `bx` are the row and column indices within the `bs × bs` block (both in `[0, bs-1]`), and `d` is the original depth index. Then the input spatial coordinates are `ih = oh * bs + by` and `iw = ow * bs + bx`. The input index is therefore `((b * input_height + ih) * input_width + iw) * input_depth + d`. The output index is `((b * output_height + oh) * output_width + ow) * output_depth + od`. We can precompute the total number of output elements and loop over them linearly using div/mod operations, or use nested loops for clarity. Edge cases include `block_size = 1` (output height/width equal input, output depth equal input depth — identity) and when `input_depth` is 1 (pure spatial-to-depth). Time complexity is `O(N)` where `N = batch * output_height * output_width * output_depth`, which equals the input size `batch * input_height * input_width * input_depth`. Space complexity is `O(1)` auxiliary (only loop variables), as the output array is provided by the caller.
