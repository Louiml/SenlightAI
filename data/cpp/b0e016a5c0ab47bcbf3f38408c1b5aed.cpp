// Write a C++ function `spaceToDepth` that reinterprets a 4D tensor stored as a flat `std::vector<float>` in row-major order with dimensions `[batch, height, width, depth]`. Given a positive integer `block_size` that evenly divides both `height` and `width`, the function returns a new flat vector representing a tensor with dimensions `[batch, height / block_size, width / block_size, depth * block_size * block_size]`. The transformation moves spatial blocks of size `block_size × block_size` from the height and width dimensions into the depth dimension: for each output pixel, the depth channel is ordered by iterating over rows of the input block (0 to block_size-1), then columns (0 to block_size-1), and then the original depth channels, producing the output value at `output[b, oh, ow, (ih_offset * block_size + iw_offset) * depth + d]` from `input[b, oh*block_size + ih_offset, ow*block_size + iw_offset, d]`. The input vector size must equal `batch * height * width * depth`, and the function should validate that `block_size` divides both dimensions exactly. The function signature is `std::vector<float> spaceToDepth(const std::vector<float>& input, int batch, int height, int width, int depth, int block_size)`. The implementation must avoid excessive copying and use only standard library headers.

#include <cassert>
#include <vector>

// Declare the function (normally would be in a header; here we prototype).
std::vector<float> spaceToDepth(const std::vector<float>& input,
                                int batch, int height, int width, int depth,
                                int block_size);

int main() {
    // Example 1: block_size=2, depth=1, single batch, 2x2 spatial -> 1x1x4
    {
        std::vector<float> in = {1,2,3,4}; // [1,2,2,1]
        std::vector<float> out = spaceToDepth(in, 1, 2, 2, 1, 2);
        std::vector<float> expected = {1,2,3,4}; // order: (0,0), (0,1), (1,0), (1,1)
        assert(out == expected);
    }

    // Example 2: block_size=2, depth=2, 4x4 spatial, batch=1
    {
        // input [1,4,4,2], fill with index-based values
        std::vector<float> in(1*4*4*2);
        for (int i = 0; i < (int)in.size(); ++i) in[i] = static_cast<float>(i);
        std::vector<float> out = spaceToDepth(in, 1, 4, 4, 2, 2);
        assert(out.size() == 1 * 2 * 2 * (2*4)); // 1*2*2*8 = 32
        // Manual check for first output pixel (0,0), depth 0..7
        // Input block rows 0-1, cols 0-1, depth 0-1:
        // (0,0,0)=from in[0]? Actually base offsets:
        // in index: ((b*4 + ih)*4 + iw)*2 + d
        // Expected out[0] = in[0*?] Let's compute:
        // out index 0: oh=0,ow=0, ih=0,iw=0,d=0 -> in[((0*4+0)*4+0)*2+0] = in[0]=0
        // out index 1: d=1 -> in[1]=1
        // out index 2: ih=0,iw=1,d=0 -> in[((0*4+0)*4+1)*2+0] = in[2]=2
        // out index 3: d=1 -> in[3]=3
        // out index 4: ih=1,iw=0,d=0 -> in[((0*4+1)*4+0)*2+0] = in[8]=8
        // out index 5: d=1 -> in[9]=9
        // out index 6: ih=1,iw=1,d=0 -> in[((0*4+1)*4+1)*2+0] = in[10]=10
        // out index 7: d=1 -> in[11]=11
        std::vector<float> expected_prefix = {0,1,2,3,8,9,10,11};
        for (int i = 0; i < 8; ++i) assert(out[i] == expected_prefix[i]);
    }

    // Example 3: block_size=1, same dimensions
    {
        std::vector<float> in = {5, -1, 3, 7};
        std::vector<float> out = spaceToDepth(in, 1, 2, 2, 1, 1);
        assert(out == in);
    }

    // Example 4: multiple batches
    {
        // batch=2, height=2, width=2, depth=1, block=2 -> output [2,1,1,4]
        std::vector<float> in = {1,2,3,4, 10,20,30,40};
        std::vector<float> out = spaceToDepth(in, 2, 2, 2, 1, 2);
        std::vector<float> expected = {1,2,3,4, 10,20,30,40};
        assert(out == expected);
    }

    // Example 5: validation failure (block_size doesn't divide height)
    {
        std::vector<float> in(2*3*4*1);
        std::vector<float> out = spaceToDepth(in, 2, 3, 4, 1, 2);
        assert(out.empty());
    }

    // Example 6: validation failure (wrong input size)
    {
        std::vector<float> in(10);
        std::vector<float> out = spaceToDepth(in, 1, 2, 2, 1, 2);
        assert(out.empty());
    }

    return 0;
}

#include <vector>
#include <cstddef>

// Reinterprets a 4D tensor in row-major order using space-to-depth transform.
// Input dims: [batch, height, width, depth]. block_size must divide height and width.
// Returns output dims: [batch, height/block_size, width/block_size, depth*block_size*block_size].
// Returns an empty vector on invalid dimensions or block_size.
std::vector<float> spaceToDepth(const std::vector<float>& input,
                                int batch, int height, int width, int depth,
                                int block_size) {
    if (block_size <= 0 || height % block_size != 0 || width % block_size != 0) {
        return {};
    }
    const std::size_t expected_size = static_cast<std::size_t>(batch) * height * width * depth;
    if (input.size() != expected_size) {
        return {};
    }

    const int output_height = height / block_size;
    const int output_width = width / block_size;
    const int output_depth = depth * block_size * block_size;
    const std::size_t output_size = static_cast<std::size_t>(batch) * output_height * output_width * output_depth;
    std::vector<float> output(output_size);

    for (int b = 0; b < batch; ++b) {
        for (int oh = 0; oh < output_height; ++oh) {
            for (int ow = 0; ow < output_width; ++ow) {
                for (int ih = 0; ih < block_size; ++ih) {
                    for (int iw = 0; iw < block_size; ++iw) {
                        // Input position: b, oh*block_size+ih, ow*block_size+iw, d
                        const std::size_t input_base = 
                            ((static_cast<std::size_t>(b) * height + (oh * block_size + ih)) * width + 
                             (ow * block_size + iw)) * depth;
                        // Output depth channel index is (ih*block_size+iw)*depth + d
                        const std::size_t output_base = 
                            (((static_cast<std::size_t>(b) * output_height + oh) * output_width + ow) * output_depth) +
                            (ih * block_size + iw) * depth;
                        for (int d = 0; d < depth; ++d) {
                            output[output_base + d] = input[input_base + d];
                        }
                    }
                }
            }
        }
    }
    return output;
}

// The core algorithm performs a direct index mapping. We first compute the output dimensions: `output_height = height / block_size`, `output_width = width / block_size`, and `output_depth = depth * block_size * block_size`. The total output size is `batch * output_height * output_width * output_depth`. We iterate over each batch, output height, output width, and then over each block offset (`ih` from 0 to block_size-1, `iw` from 0 to block_size-1) and each original depth channel `d`. For each such combination, we compute the input index as `((b * height + oh * block_size + ih) * width + ow * block_size + iw) * depth + d`, and the output index as `((b * output_height + oh) * output_width + ow) * output_depth + (ih * block_size + iw) * depth + d`. We assign the value. Validation: check that `block_size > 0`, that `height % block_size == 0` and `width % block_size == 0`, and that the input vector size equals `batch * height * width * depth`. If any check fails, return an empty vector (or throw, but returning empty is simpler for tests). Complexity: The loop runs over `batch * output_height * output_width * block_size * block_size * depth` iterations, which simplifies to `batch * height * width * depth` (the total number of input elements), so time is O(N) where N is the input size. Space complexity is O(N) for the output vector, which is necessary to store the result. Edge cases: if `depth` is 0 or `batch` is 0, the loop is empty and we return an empty vector. If `block_size` is 1, the output is identical to the input (but a new copy is created). Ensure integer arithmetic avoids overflow by using `size_t` for indices.
