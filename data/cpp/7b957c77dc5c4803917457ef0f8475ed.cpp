Write a C++ function that accepts a vector of integer vectors (representing 1D arrays of varying sizes) and returns a vector of vectors where each output vector has the same rank \( R \) (equal to the number of input vectors), and the shape of each output vector is determined by the lengths of the inputs. Specifically, for inputs of lengths \( L_1, L_2, \dots, L_R \), the output for input \( i \) is an \( R \)-dimensional array where dimension \( j \) has size \( L_j \) (for \( j = 1..R \)), and the element at index \( (a_1, \dots, a_R) \) equals the \( a_i \)-th element of the original \( i \)-th input. If a boolean flag `swapFirstTwo` is true and \( R \ge 2 \), the sizes of the first two dimensions are exchanged in the output shape (i.e., the dimension sizes are permuted), but the value assignment still follows the original input indices (i.e., the mapping uses the original dimension order). The function must handle the case \( R = 1 \) by simply returning a copy of the single input. Implement the function using flattening of multi-dimensional indices into a contiguous vector of size equal to the product of all dimension lengths. Assume all inputs are non-empty. Provide a standalone function `std::vector<std::vector<int>> meshgrid(const std::vector<std::vector<int>>& inputs, bool swapFirstTwo)`. Use 0-based indexing.
The main algorithm mimics the behavior of the provided `meshgrid` operation. We first determine the rank \( R \) as the number of input vectors. For \( R = 1 \), the output is a vector containing a single 1-dimensional vector that is a copy of the input (since the shape is just \( L_1 \), and the mapping is the identity). For \( R \ge 2 \), we compute the total number of elements as the product of all input lengths. We then precompute the "strides" for each dimension: stride[0] = 1, stride[1] = L_1, stride[2] = L_1 * L_2, etc., but we must account for the swap of the first two dimension sizes. If `swapFirstTwo` is true, the size of dimension 0 becomes `L_2`, dimension 1 becomes `L_1`, and the other dimensions keep their sizes. We recompute strides based on the actual output dimensions after the swap. For each output vector (i.e., for each input index `k` from 0 to R-1), we iterate over the flattened index `flat` from 0 to total-1, compute a multi-dimensional index \( (a_0, \dots, a_{R-1}) \) using the output strides and sizes, then compute the corresponding flattened index in the original k-th input. The key is that for output dimension `k`, the index `a_k` (where `k` is the original input index) is used as the position into the k-th input. For dimensions where the sizes were swapped, we still use the original index order (i.e., when extracting the index, we use the order `a_0, a_1, ...` as they were computed from the output shape, but we map the k-th input's value from `a_k`). So we need to compute the multi-index in the original dimension order, not the swapped output order. To avoid confusion, we compute the multi-index using the *original* dimension sizes (before swap) for the purpose of extracting input values, but we output the flattened data in the order defined by the output shape (after swap). A clean way: for each output vector (k), for each flat index in the output data (size product of output sizes), we decompose `flat` into coordinates using the output strides and output sizes. Those coordinates are in the output order (after swap). Then we map them to original coordinates: if swap and R>=1, we swap indices 0 and 1, obtaining original_coords. Then the value for the k-th output vector is `inputs[k][original_coords[k]]`. For the output flattened index, we write into `output[k][flat]`. This ensures the output shape matches the output sizes, but the values are taken from the correct input positions according to the original order. Edge case: for `R = 1`, we ignore the swap flag and return a copy. Time complexity: For each of the \( R \) outputs, we iterate over \( \prod_{j=1}^R L_j \) elements, each with O(R) work for index decomposition, leading to \( O(R \cdot N) \) where \( N \) is the total number of output elements (same for all outputs). This is optimal since we must fill all outputs. Space complexity: O(N * R) for the output storage plus O(R) for auxiliary arrays (strides, sizes, coords).
#include <vector>
#include <cstdint>
#include <numeric>

// Perform meshgrid on a list of 1D vectors.
// inputs: vector of R vectors (R = inputs.size()), each of arbitrary length.
// swapFirstTwo: if true and R>=2, the sizes of the first two output dimensions are exchanged,
//               but the element indices still refer to the original input order.
// Returns: vector of R vectors, each of size product of all output dimensions.
std::vector<std::vector<int>> meshgrid(const std::vector<std::vector<int>>& inputs, bool swapFirstTwo) {
    const int R = static_cast<int>(inputs.size());
    
    // Base case: rank 1, return a copy of the single input
    if (R == 1) {
        return {inputs[0]};
    }
    
    // Original dimension lengths (pre-swap), but later we compute actual output dims
    std::vector<size_t> origDims(R);
    for (int i = 0; i < R; ++i) {
        origDims[i] = inputs[i].size();
    }
    
    // Determine output dimension sizes (with optional swap of first two)
    std::vector<size_t> outDims = origDims;
    if (swapFirstTwo && R >= 2) {
        std::swap(outDims[0], outDims[1]);
    }
    
    // Total number of elements per output vector
    size_t total = 1;
    for (size_t d : outDims) {
        total *= d;
    }
    
    // Strides for decomposing flat index into output coordinates (based on outDims)
    std::vector<size_t> outStrides(R, 1);
    for (int i = R - 2; i >= 0; --i) {
        outStrides[i] = outStrides[i + 1] * outDims[i + 1];
    }
    
    // Strides for original dims (unswapped) for extracting from inputs (not strictly needed, but useful)
    // We'll compute original coordinates after swapping output coordinates back.
    
    std::vector<std::vector<int>> outputs(R);
    for (int k = 0; k < R; ++k) {
        outputs[k].resize(total);
    }
    
    // Temporary storage for output coordinates
    std::vector<size_t> outCoords(R);
    
    for (size_t flat = 0; flat < total; ++flat) {
        // Decompose flat into output coordinates (order after swap)
        size_t remainder = flat;
        for (int i = 0; i < R; ++i) {
            outCoords[i] = remainder / outStrides[i];
            remainder %= outStrides[i];
        }
        
        // Map output coordinates to original coordinates (swap first two if needed)
        size_t origCoords[3]; // R could be more, but use a vector to be safe
        std::vector<size_t> origC(R);
        for (int i = 0; i < R; ++i) {
            origC[i] = outCoords[i];
        }
        if (swapFirstTwo) {
            std::swap(origC[0], origC[1]);
        }
        
        // For each output vector k, pick the value from inputs[k] at position origC[k]
        for (int k = 0; k < R; ++k) {
            outputs[k][flat] = inputs[k][origC[k]];
        }
    }
    
    return outputs;
}
#include <cassert>
#include <vector>
#include <iostream>

// Include the solution function declaration or code here

int main() {
    // Test 1: Basic 2D meshgrid, no swap
    {
        std::vector<std::vector<int>> inputs = {{1, 2, 3}, {4, 5}};
        auto out = meshgrid(inputs, false);
        // out[0] shape (3,2): row-major, each row repeats first input
        assert(out[0] == std::vector<int>({1,1, 2,2, 3,3}));
        // out[1] shape (3,2): each row repeats second input
        assert(out[1] == std::vector<int>({4,5, 4,5, 4,5}));
    }
    
    // Test 2: Swap first two dims for 2D
    {
        std::vector<std::vector<int>> inputs = {{1, 2, 3}, {4, 5}};
        auto out = meshgrid(inputs, true);
        // Output dims: (2,3)
        // out[0] (first original input) shape (2,3): each row uses first input's elements
        assert(out[0] == std::vector<int>({1,2,3, 1,2,3}));
        // out[1] (second original input) shape (2,3): each column uses second input's elements
        assert(out[1] == std::vector<int>({4,4,4, 5,5,5}));
    }
    
    // Test 3: 3D meshgrid, no swap
    {
        std::vector<std::vector<int>> inputs = {{1, 2}, {3, 4, 5}, {6}};
        auto out = meshgrid(inputs, false);
        // Out dims: (2,3,1)
        // First output: all elements are 1 then all 2 (since only 2 elements)
        assert(out[0] == std::vector<int>({1,1,1, 2,2,2}));
        // Second output: within each block of size 1, pattern 3,4,5 repeated appropriately
        assert(out[1] == std::vector<int>({3,4,5, 3,4,5}));
        // Third output: all are 6
        assert(out[2] == std::vector<int>({6,6,6, 6,6,6}));
    }
    
    // Test 4: 3D with swap first two
    {
        std::vector<std::vector<int>> inputs = {{1, 2}, {3, 4, 5}, {6}};
        auto out = meshgrid(inputs, true);
        // Out dims: (3,2,1)
        // First output (orig input 0) shape (3,2): each row repeats [1,2]
        assert(out[0] == std::vector<int>({1,2, 1,2, 1,2}));
        // Second output (orig input 1) shape (3,2): each column is constant per row: [3,3],[4,4],[5,5]
        assert(out[1] == std::vector<int>({3,3, 4,4, 5,5}));
        // Third output: all 6
        assert(out[2] == std::vector<int>({6,6,6, 6,6,6}));
    }
    
    // Test 5: Rank 1
    {
        std::vector<std::vector<int>> inputs = {{7, 8, 9}};
        auto out = meshgrid(inputs, true); // swap flag ignored
        assert(out.size() == 1);
        assert(out[0] == std::vector<int>({7, 8, 9}));
    }
    
    // Test 6: Single element each
    {
        std::vector<std::vector<int>> inputs = {{5}, {6}, {7}};
        auto out = meshgrid(inputs, false);
        assert(out[0] == std::vector<int>({5}));
        assert(out[1] == std::vector<int>({6}));
        assert(out[2] == std::vector<int>({7}));
    }
    
    // Test 7: 4D to check general case
    {
        std::vector<std::vector<int>> inputs = {{0, 1}, {2}, {3, 4}, {5}};
        auto out = meshgrid(inputs, false);
        // Total elements = 2*1*2*1 = 4
        // First output: pattern [0,0, 1,1]? Actually dims (2,1,2,1), outer to inner.
        // Flat index ordering: i0 (2), i1 (1), i2 (2), i3 (1)
        // For each i0, i1, i2, i3: out[0][flat] = inputs[0][i0]
        // So: i0=0: i1=0,i2=0,i3=0 -> 0; i2=1 ->0; then i0=1: ->1,1
        assert(out[0] == std::vector<int>({0,0, 1,1}));
        // Second output: all 2
        assert(out[1] == std::vector<int>({2,2, 2,2}));
        // Third output: i2 varies: for i0=0: 3,4; for i0=1: 3,4
        assert(out[2] == std::vector<int>({3,4, 3,4}));
        // Fourth output: all 5
        assert(out[3] == std::vector<int>({5,5, 5,5}));
    }
    
    std::cout << "All tests passed!" << std::endl;
    return 0;
}
