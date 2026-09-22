/*
Write a standalone C++ function named `reduceMean` that computes the mean of a contiguous block of `int32_t` values along a specified set of reduction axes. The input is a 1D array representing a tensor with known dimensions, plus an array of axis indices (each a valid dimension index from 0 to rank-1, possibly duplicated). The function should support two modes controlled by a boolean `keepDims`: if true, the output shape must retain the reduced dimensions as size 1; if false, those dimensions are removed. The output must be written into a caller-provided `int32_t` array, and the function should return a `bool` indicating success. The reduction must sum the elements along the specified axes and divide by the number of elements reduced (floor division toward zero for negative values using C++ integer division semantics). Assume the input tensor has at least 1 dimension, at most 5 dimensions, and at most 2 unique reduction axes; duplicates in the axis list should be counted only once when determining which dimensions to reduce. The function must not allocate dynamic memory; use fixed-size local arrays (max rank 5, max unique axes 2) and handle edge cases such as reducing all dimensions (output is a scalar), reducing along an axis of size 1, and keeping dimensions versus not. The solution must be self-contained with only `<cstdint>` and `<cstddef>` included.
*/

#include <cstdint>
#include <cstddef>

constexpr int kMaxRank = 5;
constexpr int kMaxReducedAxes = 2;

// Computes the mean of a tensor along specified axes.
// input: pointer to input data, shape: input dimensions array of length rank.
// axis: array of axis indices, axisCount: number of entries (may include duplicates).
// keepDims: if true, output retains reduced dimensions as size 1.
// output: pointer to output data (must be preallocated to correct size).
// outputShape: caller-provided array of length rank to receive output dimensions.
// Returns true on success, false on invalid input (invalid axis or too many unique axes).
bool reduceMean(const int32_t* input,
                const int32_t* shape,
                int rank,
                const int32_t* axis,
                int axisCount,
                bool keepDims,
                int32_t* output,
                int32_t* outputShape) {
    if (rank < 1 || rank > kMaxRank || axisCount < 0) return false;

    // Deduplicate axes, ignore invalid ones; up to 2 unique allowed.
    int32_t reducedAxes[kMaxReducedAxes] = {0};
    int reducedCount = 0;
    for (int i = 0; i < axisCount; ++i) {
        int32_t a = axis[i];
        if (a < 0 || a >= rank) return false;
        bool duplicate = false;
        for (int j = 0; j < reducedCount; ++j) {
            if (reducedAxes[j] == a) { duplicate = true; break; }
        }
        if (!duplicate) {
            if (reducedCount >= kMaxReducedAxes) return false;
            reducedAxes[reducedCount++] = a;
        }
    }

    // Compute total input size and output shape.
    int32_t inputSize = 1;
    int32_t outputSize = 1;
    for (int i = 0; i < rank; ++i) {
        inputSize *= shape[i];
        bool isReduced = false;
        for (int j = 0; j < reducedCount; ++j) {
            if (reducedAxes[j] == i) { isReduced = true; break; }
        }
        outputShape[i] = isReduced ? (keepDims ? 1 : 0) : shape[i];
        if (outputShape[i] > 0) outputSize *= outputShape[i];
    }
    if (outputSize == 0 || inputSize == 0) return false;

    // Precompute input strides for fast linear index computation.
    int32_t inputStrides[kMaxRank];
    int32_t stride = 1;
    for (int i = rank - 1; i >= 0; --i) {
        inputStrides[i] = stride;
        stride *= shape[i];
    }

    // Compute output strides (only for non-reduced axes; reduced axes have stride 0 if keepDims).
    int32_t outputStrides[kMaxRank] = {0};
    stride = 1;
    for (int i = rank - 1; i >= 0; --i) {
        bool isReduced = false;
        for (int j = 0; j < reducedCount; ++j) {
            if (reducedAxes[j] == i) { isReduced = true; break; }
        }
        if (!isReduced || (isReduced && keepDims)) {
            outputStrides[i] = stride;
            stride *= outputShape[i];
        }
    }

    // Iterate over all output elements.
    for (int32_t outLinear = 0; outLinear < outputSize; ++outLinear) {
        // Compute multi-dimensional output coordinates and corresponding input coordinates
        // (set reduced dims to 0 for now).
        int32_t outCoord[kMaxRank] = {0};
        int32_t inCoordBase[kMaxRank] = {0};
        int remaining = outLinear;
        for (int i = 0; i < rank; ++i) {
            bool isReduced = false;
            for (int j = 0; j < reducedCount; ++j) {
                if (reducedAxes[j] == i) { isReduced = true; break; }
            }
            if (!isReduced || (isReduced && keepDims)) {
                outCoord[i] = remaining / outputStrides[i];
                remaining %= outputStrides[i];
                inCoordBase[i] = outCoord[i];
            } else {
                inCoordBase[i] = 0;
            }
        }

        // Determine sizes of reduced axes and number of combinations.
        int32_t reducedSizes[kMaxReducedAxes] = {0};
        int32_t combinations = 1;
        for (int j = 0; j < reducedCount; ++j) {
            int32_t a = reducedAxes[j];
            reducedSizes[j] = shape[a];
            combinations *= reducedSizes[j];
        }

        // Accumulate sum over all combinations of reduced indices.
        int32_t sum = 0;
        if (reducedCount == 0) {
            // No reduction: output is a copy.
            sum = input[outLinear];
        } else if (reducedCount == 1) {
            for (int32_t r0 = 0; r0 < reducedSizes[0]; ++r0) {
                int32_t coord[kMaxRank] = {0};
                for (int i = 0; i < rank; ++i) coord[i] = inCoordBase[i];
                coord[reducedAxes[0]] = r0;
                int32_t lin = 0;
                for (int i = 0; i < rank; ++i) lin += coord[i] * inputStrides[i];
                sum += input[lin];
            }
        } else { // reducedCount == 2
            for (int32_t r0 = 0; r0 < reducedSizes[0]; ++r0) {
                for (int32_t r1 = 0; r1 < reducedSizes[1]; ++r1) {
                    int32_t coord[kMaxRank] = {0};
                    for (int i = 0; i < rank; ++i) coord[i] = inCoordBase[i];
                    coord[reducedAxes[0]] = r0;
                    coord[reducedAxes[1]] = r1;
                    int32_t lin = 0;
                    for (int i = 0; i < rank; ++i) lin += coord[i] * inputStrides[i];
                    sum += input[lin];
                }
            }
        }

        // Compute mean (integer division truncates toward zero).
        output[outLinear] = combinations > 0 ? sum / combinations : 0;
    }

    return true;
}

#include <cassert>
#include <cstdint>

// Declare the function under test (declaration only; definition in solution).
bool reduceMean(const int32_t* input, const int32_t* shape, int rank,
                const int32_t* axis, int axisCount, bool keepDims,
                int32_t* output, int32_t* outputShape);

int main() {
    // Test 1: Simple 2D matrix, reduce axis 1 (columns) without keepDims.
    {
        int32_t input[] = {1, 2, 3, 4, 5, 6}; // shape [2,3]
        int32_t shape[] = {2, 3};
        int32_t axis[] = {1};
        int32_t output[2];
        int32_t outShape[2];
        assert(reduceMean(input, shape, 2, axis, 1, false, output, outShape));
        assert(outShape[0] == 2 && outShape[1] == 0); // reduced shape is [2]
        assert(output[0] == (1+2+3)/3); // 2
        assert(output[1] == (4+5+6)/3); // 5
    }

    // Test 2: Reduce axis 0 (rows) with keepDims true.
    {
        int32_t input[] = {1, 2, 3, 4, 5, 6};
        int32_t shape[] = {2, 3};
        int32_t axis[] = {0};
        int32_t output[3];
        int32_t outShape[2];
        assert(reduceMean(input, shape, 2, axis, 1, true, output, outShape));
        assert(outShape[0] == 1 && outShape[1] == 3);
        assert(output[0] == (1+4)/2); // 2
        assert(output[1] == (2+5)/2); // 3
        assert(output[2] == (3+6)/2); // 4
    }

    // Test 3: Reduce both axes (scalar output).
    {
        int32_t input[] = {1, 2, 3, 4};
        int32_t shape[] = {2, 2};
        int32_t axis[] = {0, 1};
        int32_t output[1];
        int32_t outShape[2];
        assert(reduceMean(input, shape, 2, axis, 2, false, output, outShape));
        assert(outShape[0] == 0 && outShape[1] == 0);
        assert(output[0] == (1+2+3+4)/4); // 2
    }

    // Test 4: Duplicate axes should be deduplicated.
    {
        int32_t input[] = {1, 2, 3, 4};
        int32_t shape[] = {2, 2};
        int32_t axis[] = {1, 1}; // duplicate
        int32_t output[2];
        int32_t outShape[2];
        assert(reduceMean(input, shape, 2, axis, 2, false, output, outShape));
        assert(outShape[0] == 2 && outShape[1] == 0);
        assert(output[0] == (1+2)/2); // 1
        assert(output[1] == (3+4)/2); // 3
    }

    // Test 5: Reduce axis of size 1.
    {
        int32_t input[] = {5, 7, 9}; // shape [3,1]
        int32_t shape[] = {3, 1};
        int32_t axis[] = {1};
        int32_t output[3];
        int32_t outShape[2];
        assert(reduceMean(input, shape, 2, axis, 1, false, output, outShape));
        assert(outShape[0] == 3 && outShape[1] == 0);
        assert(output[0] == 5);
        assert(output[1] == 7);
        assert(output[2] == 9);
    }

    // Test 6: Reduce all dimensions with keepDims true gives shape all 1s.
    {
        int32_t input[] = {2, 4, 6, 8}; // shape [2,2]
        int32_t shape[] = {2, 2};
        int32_t axis[] = {0, 1};
        int32_t output[1];
        int32_t outShape[2];
        assert(reduceMean(input, shape, 2, axis, 2, true, output, outShape));
        assert(outShape[0] == 1 && outShape[1] == 1);
        assert(output[0] == (2+4+6+8)/4); // 5
    }

    // Test 7: Negative values; integer division truncates toward zero.
    {
        int32_t input[] = {-3, -1, -4, -2}; // shape [2,2]
        int32_t shape[] = {2, 2};
        int32_t axis[] = {1};
        int32_t output[2];
        int32_t outShape[2];
        assert(reduceMean(input, shape, 2, axis, 1, false, output, outShape));
        assert(output[0] == (-3 + -1)/2); // -2
        assert(output[1] == (-4 + -2)/2); // -3
    }

    // Test 8: Invalid axis returns false.
    {
        int32_t input[] = {1, 2, 3, 4};
        int32_t shape[] = {2, 2};
        int32_t axis[] = {2}; // invalid
        int32_t output[4];
        int32_t outShape[2];
        assert(!reduceMean(input, shape, 2, axis, 1, false, output, outShape));
    }

    // Test 9: More than 2 unique axes returns false.
    {
        int32_t input[] = {1, 2, 3, 4, 5, 6, 7, 8}; // shape [2,2,2]
        int32_t shape[] = {2, 2, 2};
        int32_t axis[] = {0, 1, 2};
        int32_t output[1];
        int32_t outShape[3];
        assert(!reduceMean(input, shape, 3, axis, 3, false, output, outShape));
    }

    // Test 10: No reduction axes (axisCount=0) returns copy.
    {
        int32_t input[] = {10, 20, 30};
        int32_t shape[] = {3};
        int32_t axis[] = {};
        int32_t output[3];
        int32_t outShape[1];
        assert(reduceMean(input, shape, 1, axis, 0, false, output, outShape));
        assert(outShape[0] == 3);
        assert(output[0] == 10 && output[1] == 20 && output[2] == 30);
    }

    return 0;
}

// The core algorithm iterates over every element of the output tensor. For each output linear index, we map it back to multi-dimensional coordinates for the output shape (either the reduced shape or the shape with size-1 axes if `keepDims` is true). Then we expand those coordinates back to full input coordinates by inserting placeholder values (0) for positions corresponding to reduced axes. For each possible combination of indices along the reduced axes (the product of their sizes), we compute the linear input index and accumulate the value into a running sum. After summing all contributions, we compute the mean as `sum / count` (using integer division; note that for negative sums C++ truncates toward zero, which is acceptable per spec). The number of unique reduced axes is at most 2, so nested loops cover all combinations. Edge cases: if an axis is listed multiple times, we deduplicate it before proceeding. If the reduced axes set is empty, the output equals the input. If the output is a scalar (all axes reduced), the mapping is trivial. For `keepDims`, the output shape is identical to input shape except reduced axes become 1, so the output linear index mapping must skip those axes when computing strides. Time complexity is \(O(I \cdot \prod_{a \in \text{reduced}} \text{size}_a)\), where \(I\) is the number of output elements; worst-case this is \(O(\text{total input elements})\). Space complexity is \(O(1)\) auxiliary beyond fixed arrays.
