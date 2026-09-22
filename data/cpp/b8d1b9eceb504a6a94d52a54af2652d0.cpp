// Write a standalone C++ function `permuteFeatureToLast` that takes a 4D tensor stored in a feature-blocked layout (`b_fs_yx_fsv4`, where features are grouped in blocks of 4 with padding to the nearest multiple of 4) and permutes its dimensions from `(batch, features, Y, X)` to `(batch, Y, X, features)`, producing a plain row-major output tensor. The input is given as a flat `std::vector<int>` using the blocked layout, along with dimensions `B`, `F`, `Y`, `X` (where `F` is the logical feature count, which may not be a multiple of 4, and the input storage pads each feature block to 4). The output must be a flat `std::vector<int>` of size `B * Y * X * F`, laid out in row-major order (`b` outermost, then `y`, then `x`, then `f` innermost). Your function must handle cases where `F` is not a multiple of 4, and it must not assume any alignment beyond the 4-feature block structure. The function signature is: `std::vector<int> permuteFeatureToLast(const std::vector<int>& input, int B, int F, int Y, int X);`
The main challenge is translating between the blocked input layout and the plain output layout. The input layout `b_fs_yx_fsv4` stores data as follows: for each batch `b`, for each feature block index `fb` (from 0 to `ceil(F/4)-1`), for each `(y, x)` position, there are exactly 4 consecutive values corresponding to features `fb*4 + 0,1,2,3` (with padding for the last block if `F` is not a multiple of 4). The logical index into the flat input vector for element `(b, f, y, x)` is: `((b * ceil(F/4) + fb) * Y + y) * X + x) * 4 + (f % 4)` where `fb = f / 4`.

For the output, we iterate over all `b, y, x, f` in that nested order (row-major) and copy the value from the corresponding input location. We must skip any padding features (where `f >= F`) when writing to the output (they simply don't exist in the output). The output size is exactly `B * Y * X * F`; we can preallocate and fill by direct indexing.

Edge cases: (1) `F` not a multiple of 4 means the last feature block has 1–3 valid features and the rest are padding, which we ignore. (2) `F` could be less than 4, so the only block is partially filled. (3) The input vector length is `B * ceil(F/4) * Y * X * 4`, which may be larger than the output length if `F` is not a multiple of 4.

Time complexity: O(B × Y × X × F) because we iterate over every output element once. Space complexity: O(B × Y × X × F) for the output, plus O(1) extra.
#include <vector>
#include <cassert>

// Permute from b_fs_yx_fsv4 (blocked features, 4 per block) to plain bfyx -> (b, y, x, f) row-major.
// Input layout: for each b, for each feature block fb (size 4), for each (y,x), 4 consecutive values.
// Output layout: row-major order: b -> y -> x -> f.
std::vector<int> permuteFeatureToLast(const std::vector<int>& input, int B, int F, int Y, int X) {
    if (B <= 0 || F <= 0 || Y <= 0 || X <= 0) {
        return {};
    }

    const int featureBlocks = (F + 3) / 4;   // ceil(F/4)
    const int inputPlaneSize = Y * X;        // number of (y,x) positions per batch per block
    const int inputBatchSize = featureBlocks * inputPlaneSize * 4;

    // Verify input size matches expected blocked layout.
    assert(input.size() == static_cast<size_t>(B) * featureBlocks * inputPlaneSize * 4);

    std::vector<int> output;
    output.reserve(static_cast<size_t>(B) * Y * X * F);

    for (int b = 0; b < B; ++b) {
        const int batchOffset = b * inputBatchSize;
        for (int y = 0; y < Y; ++y) {
            for (int x = 0; x < X; ++x) {
                const int spatialPos = y * X + x;  // position within a feature block
                // For this (b, y, x), gather all features f from 0 to F-1.
                for (int f = 0; f < F; ++f) {
                    const int fb = f / 4;
                    const int inFeatures = f % 4;
                    const int inputIndex = batchOffset + fb * (inputPlaneSize * 4) + spatialPos * 4 + inFeatures;
                    output.push_back(input[inputIndex]);
                }
            }
        }
    }
    return output;
}
#include <cassert>
#include <vector>
#include <iostream>

// The solution function is declared above, but for the test we include it here.
// For completeness, the function is repeated (in real code it would be in a header).
// (Implementation as above.)

int main() {
    // Test 1: Basic 1x4x1x1, F exactly a multiple of 4.
    // Input b_fs_yx_fsv4: one batch, one feature block, one position.
    // Features 0,1,2,3 stored as [0,1,2,3].
    {
        std::vector<int> input = {10, 20, 30, 40};
        std::vector<int> expected = {10, 20, 30, 40};
        auto result = permuteFeatureToLast(input, 1, 4, 1, 1);
        assert(result == expected);
    }

    // Test 2: F not a multiple of 4 (e.g., F=5, needs 2 blocks, padding in second block).
    // B=1, F=5, Y=1, X=1.
    // Block0: features 0-3 stored as [0,1,2,3], Block1: features 4 and padding [4, -1, -1, -1].
    // Output should be [0,1,2,3,4].
    {
        std::vector<int> input = {0, 1, 2, 3, 4, -1, -1, -1};
        std::vector<int> expected = {0, 1, 2, 3, 4};
        auto result = permuteFeatureToLast(input, 1, 5, 1, 1);
        assert(result == expected);
    }

    // Test 3: 2D spatial, B=1, F=4, Y=2, X=2.
    // Input blocked: one batch, one feature block, Y*X=4 positions.
    // For each (y,x) position, 4 features.
    // Let's construct input manually:
    // position (0,0): f0=1,f1=2,f2=3,f3=4
    // position (0,1): f0=5,f1=6,f2=7,f3=8
    // position (1,0): f0=9,f1=10,f2=11,f3=12
    // position (1,1): f0=13,f1=14,f2=15,f3=16
    // Input flat: [1,2,3,4, 5,6,7,8, 9,10,11,12, 13,14,15,16]
    // Output row-major (b,y,x,f): 
    // (0,0,0): f0=1,f1=2,f2=3,f3=4
    // (0,0,1): f0=5,f1=6,f2=7,f3=8
    // (0,1,0): f0=9,f1=10,f2=11,f3=12
    // (0,1,1): f0=13,f1=14,f2=15,f3=16
    // So output = [1,2,3,4, 5,6,7,8, 9,10,11,12, 13,14,15,16]
    {
        std::vector<int> input = {1,2,3,4, 5,6,7,8, 9,10,11,12, 13,14,15,16};
        std::vector<int> expected = input;  // same because F=4 and output order matches per-position block
        auto result = permuteFeatureToLast(input, 1, 4, 2, 2);
        assert(result == expected);
    }

    // Test 4: Multiple batches, F=3 (less than 4), Y=1, X=1.
    // B=2, each batch has one feature block with 3 valid features and 1 padding.
    // Input: batch0: [100,200,300,-1], batch1: [400,500,600,-1]
    // Output: batch0: [100,200,300], batch1: [400,500,600]
    {
        std::vector<int> input = {100,200,300,-1, 400,500,600,-1};
        std::vector<int> expected = {100,200,300, 400,500,600};
        auto result = permuteFeatureToLast(input, 2, 3, 1, 1);
        assert(result == expected);
    }

    // Test 5: F=6 (two blocks), Y=2, X=1, B=1.
    // Input: block0 positions: pos(0,0): f0..f3 = 1,2,3,4 ; pos(1,0): f0..f3=5,6,7,8
    //        block1 positions: pos(0,0): f4=-1, f5=9, pad, pad ; pos(1,0): f4=-1,f5=10,pad,pad
    // Input flat: [1,2,3,4, 5,6,7,8, -1,9,0,0, -1,10,0,0]
    // Output (b,y,x,f): (0,0,0): f0=1,f1=2,f2=3,f3=4,f4=-1,f5=9; (0,1,0): f0=5,f1=6,f2=7,f3=8,f4=-1,f5=10
    // Expected: [1,2,3,4,-1,9, 5,6,7,8,-1,10]
    {
        std::vector<int> input = {1,2,3,4, 5,6,7,8, -1,9,0,0, -1,10,0,0};
        std::vector<int> expected = {1,2,3,4,-1,9, 5,6,7,8,-1,10};
        auto result = permuteFeatureToLast(input, 1, 6, 2, 1);
        assert(result == expected);
    }

    // Test 6: With padding in feature dimension and spatial dimension larger than 1.
    // B=1, F=5, Y=2, X=2. Build manually.
    // For each of 2 spatial rows, 2 cols, there are 2 feature blocks? Wait, F=5 => blocks=2, each block has all positions.
    // Construct input:
    // Block0 (features 0-3): positions (0,0): [1,2,3,4]; (0,1): [5,6,7,8]; (1,0): [9,10,11,12]; (1,1): [13,14,15,16]
    // Block1 (features 4 plus padding): positions (0,0): [17,-1,-1,-1]; (0,1): [18,-1,-1,-1]; (1,0): [19,-1,-1,-1]; (1,1): [20,-1,-1,-1]
    // Input flat: [1,2,3,4, 5,6,7,8, 9,10,11,12, 13,14,15,16, 17,-1,-1,-1, 18,-1,-1,-1, 19,-1,-1,-1, 20,-1,-1,-1]
    // Output (b,y,x,f) where f from 0..4:
    // (0,0,0): f0=1,f1=2,f2=3,f3=4,f4=17
    // (0,0,1): f0=5,f1=6,f2=7,f3=8,f4=18
    // (0,1,0): f0=9,f1=10,f2=11,f3=12,f4=19
    // (0,1,1): f0=13,f1=14,f2=15,f3=16,f4=20
    // Expected: [1,2,3,4,17, 5,6,7,8,18, 9,10,11,12,19, 13,14,15,16,20]
    {
        std::vector<int> input = {
            1,2,3,4, 5,6,7,8, 9,10,11,12, 13,14,15,16,
            17,-1,-1,-1, 18,-1,-1,-1, 19,-1,-1,-1, 20,-1,-1,-1
        };
        std::vector<int> expected = {
            1,2,3,4,17, 5,6,7,8,18, 9,10,11,12,19, 13,14,15,16,20
        };
        auto result = permuteFeatureToLast(input, 1, 5, 2, 2);
        assert(result == expected);
    }

    // Test 7: Empty dimensions
    {
        std::vector<int> input;  // no data
        auto result = permuteFeatureToLast(input, 0, 4, 1, 1);
        assert(result.empty());
    }

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
