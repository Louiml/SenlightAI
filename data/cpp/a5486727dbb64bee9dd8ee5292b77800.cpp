/*
Write a standalone C++ function `computeDiagramContributions` that takes as input a vector of `std::complex<double>` values and a vector of 4-dimensional tensors (represented as `Eigen::Tensor<std::complex<double>, 4>`) for both "propagator" tensors and "sink" tensors, where both vectors are guaranteed to have at least one element. The function must compute a fixed set of ten different tensor contraction results, each corresponding to a different pairing of the four indices of the first propagator tensor with the four indices of the first sink tensor. The contraction patterns are specified by the following list of pairing pairs (each pair `{a,b}` means index `a` of the left tensor contracts with index `b` of the right tensor):  
1) {{3,3},{2,2},{1,1},{0,0}}  
2) {{3,3},{2,2},{1,0},{0,1}}  
3) {{3,0},{2,2},{1,3},{0,1}}  
4) {{3,1},{2,2},{1,3},{0,0}}  
5) {{3,1},{2,2},{1,0},{0,3}}  
6) {{3,0},{2,2},{1,1},{0,3}}  
7) {{3,3},{2,2},{1,1},{0,0}}  (duplicate of pattern 1)  
8) {{3,3},{2,0},{1,1},{0,2}}  
9) {{3,1},{2,2},{1,3},{0,0}}  (duplicate of pattern 4)  
10) {{3,1},{2,0},{1,3},{0,2}}  
For each pattern, the function must perform a full contraction of the two tensors—i.e., sum over all repeated indices according to the specified pairing, producing a single complex number per pattern. The ten results must be stored in the given output vector `res` at indices 0 through 9, respectively. You may assume both tensors have dimension size 4 in each mode, so all indices range 0–3, and that the output vector is pre-sized to exactly 10. The function must be efficient by using Eigen’s tensor contraction capabilities (e.g., using Eigen’s `contract` method with an index pair array), and must not modify the input tensors or the input vectors. The function signature must be: `void computeDiagramContributions(std::vector<std::complex<double>>& res, const std::vector<Eigen::Tensor<std::complex<double>, 4>>& bprops, const std::vector<Eigen::Tensor<std::complex<double>, 4>>& bsinks)`, and it must only use the first element of each input vector (i.e., `bprops[0]` and `bsinks[0]`). The function should be self-contained, include all necessary headers, and handle the case where the tensors contain arbitrary complex values (including zeros and negatives). No error checking is required for input validity; assume inputs are correct.
*/
#include <vector>
#include <complex>
#include <Eigen/CXX11/Tensor>

using cd = std::complex<double>;
using Tensor4 = Eigen::Tensor<cd, 4>;

// Compute ten fixed contraction results between the first propagator tensor
// and the first sink tensor. The output vector must be pre-sized to 10.
void computeDiagramContributions(std::vector<cd>& res,
                                 const std::vector<Tensor4>& bprops,
                                 const std::vector<Tensor4>& bsinks) {
    // Build the ten contraction patterns as index pairs.
    // Each pattern is a vector of four pairs {left_index, right_index}.
    const std::vector<std::vector<std::pair<int, int>>> patterns = {
        {{3,3}, {2,2}, {1,1}, {0,0}},
        {{3,3}, {2,2}, {1,0}, {0,1}},
        {{3,0}, {2,2}, {1,3}, {0,1}},
        {{3,1}, {2,2}, {1,3}, {0,0}},
        {{3,1}, {2,2}, {1,0}, {0,3}},
        {{3,0}, {2,2}, {1,1}, {0,3}},
        {{3,3}, {2,2}, {1,1}, {0,0}}, // duplicate of pattern 1
        {{3,3}, {2,0}, {1,1}, {0,2}},
        {{3,1}, {2,2}, {1,3}, {0,0}}, // duplicate of pattern 4
        {{3,1}, {2,0}, {1,3}, {0,2}}
    };

    const Tensor4& left = bprops[0];
    const Tensor4& right = bsinks[0];

    for (std::size_t i = 0; i < patterns.size(); ++i) {
        // Convert pair list into Eigen IndexPair array.
        Eigen::array<Eigen::IndexPair<long>, 4> pairs;
        for (int j = 0; j < 4; ++j) {
            pairs[j] = Eigen::IndexPair<long>(patterns[i][j].first,
                                              patterns[i][j].second);
        }
        // Contract down to scalar tensor.
        auto contracted = left.contract(right, pairs);
        res[i] = contracted(0);
    }
}
#include <cassert>
#include <complex>
#include <vector>
#include <Eigen/CXX11/Tensor>

using cd = std::complex<double>;
using Tensor4 = Eigen::Tensor<cd, 4>;

// The solution function is assumed to be declared above.
void computeDiagramContributions(std::vector<cd>&,
                                 const std::vector<Tensor4>&,
                                 const std::vector<Tensor4>&);

int main() {
    // Create a simple 4x4x4x4 tensor where element [i][j][k][l] = i + 2*j + 3*k + 4*l + 1 (as complex).
    Tensor4 left(4,4,4,4);
    Tensor4 right(4,4,4,4);
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            for (int k = 0; k < 4; ++k)
                for (int l = 0; l < 4; ++l) {
                    left(i,j,k,l) = cd(i + 2*j + 3*k + 4*l + 1, 0.0);
                    right(i,j,k,l) = cd(i - j + k - l + 2, 0.0);
                }

    std::vector<Tensor4> bprops(1, left);
    std::vector<Tensor4> bsinks(1, right);
    std::vector<cd> res(10);

    computeDiagramContributions(res, bprops, bsinks);

    // Manually compute a few expected results for verification.
    // Pattern 1: {{3,3},{2,2},{1,1},{0,0}} -> sum_{i0,i1,i2,i3} left[i0,i1,i2,i3] * right[i0,i1,i2,i3]
    cd expected1 = cd(0,0);
    for (int i0 = 0; i0 < 4; ++i0)
        for (int i1 = 0; i1 < 4; ++i1)
            for (int i2 = 0; i2 < 4; ++i2)
                for (int i3 = 0; i3 < 4; ++i3)
                    expected1 += left(i0,i1,i2,i3) * right(i0,i1,i2,i3);
    assert(res[0] == expected1);

    // Pattern 2: {{3,3},{2,2},{1,0},{0,1}} 
    // For left indices: a0=3 pairs with b3, a1=2 pairs with b2, a2=1 pairs with b0, a3=0 pairs with b1
    // So sum over i0,i1,i2,i3 of left(i0,i1,i2,i3)*right(i2,i3,i0,i1)? Wait: pair {1,0} means left index 1 = right index 0, so b0 = left1; pair {0,1} means b1 = left0. Thus for right, b0 = left1, b1 = left0, b2 = left2, b3 = left3. So right(i1, i0, i2, i3).
    cd expected2 = cd(0,0);
    for (int i0 = 0; i0 < 4; ++i0)
        for (int i1 = 0; i1 < 4; ++i1)
            for (int i2 = 0; i2 < 4; ++i2)
                for (int i3 = 0; i3 < 4; ++i3)
                    expected2 += left(i0,i1,i2,i3) * right(i1, i0, i2, i3);
    assert(res[1] == expected2);

    // Pattern 3: {{3,0},{2,2},{1,3},{0,1}}
    // left index 3 -> right index 0, left 2 -> right 2, left 1 -> right 3, left 0 -> right 1
    // So right(i3, i0, i2, i1)
    cd expected3 = cd(0,0);
    for (int i0 = 0; i0 < 4; ++i0)
        for (int i1 = 0; i1 < 4; ++i1)
            for (int i2 = 0; i2 < 4; ++i2)
                for (int i3 = 0; i3 < 4; ++i3)
                    expected3 += left(i0,i1,i2,i3) * right(i3, i0, i2, i1);
    assert(res[2] == expected3);

    // Pattern 7 is duplicate of pattern 1; check that results match.
    assert(res[6] == res[0]);

    // Pattern 9 is duplicate of pattern 4; check that results match.
    assert(res[8] == res[3]);

    // Rest of patterns can be assumed correct if these pass; but we can also spot-check pattern 10.
    // Pattern 10: {{3,1},{2,0},{1,3},{0,2}}
    // left 3 -> right 1, left 2 -> right 0, left 1 -> right 3, left 0 -> right 2
    // So right(i1, i3, i0, i2)
    cd expected10 = cd(0,0);
    for (int i0 = 0; i0 < 4; ++i0)
        for (int i1 = 0; i1 < 4; ++i1)
            for (int i2 = 0; i2 < 4; ++i2)
                for (int i3 = 0; i3 < 4; ++i3)
                    expected10 += left(i0,i1,i2,i3) * right(i1, i3, i0, i2);
    assert(res[9] == expected10);

    return 0;
}
// The solution involves implementing a function that performs ten distinct tensor contractions between a 4th-order tensor `A` (from `bprops[0]`) and another 4th-order tensor `B` (from `bsinks[0]`). Each contraction pattern is a list of four index pairs specifying how the four indices of `A` are contracted with the four indices of `B`. The contraction is a sum over all four contracted indices simultaneously: for each pattern, we compute `sum_{i0,i1,i2,i3} A[i0][i1][i2][i3] * B[j0][j1][j2][j3]` where for each pair `{a,b}` we set `ja = ib` (i.e., the index of `B` at position `a` equals the index of `A` at position `b`). This is equivalent to an inner product of the two tensors after permuting the modes of one tensor so that the contracted axes align. The main algorithmic approach is to use Eigen’s `tensor.contract(other, index_pairs)` method, which applies the pairing directly without manual loops. For each pattern, we construct an `Eigen::array<Eigen::IndexPair<long>, 4>` containing the four pairs, then call `A.contract(B, pairs)` which returns a 0-dimensional tensor (Eigen::Tensor<cd, 0>) whose scalar value we extract via `(expr)(0)`. Important edge cases: the function must correctly handle the duplicated patterns (1 and 7, 4 and 9) by computing them twice; because the input tensors are passed as `const`, we must take care not to copy them unnecessarily; and we must use `long` for the IndexPair type to match Eigen’s expectations. The complexity is dominated by the contraction operation: each contraction sums over 4 indices each ranging from 0 to dimension-1, so for dimension size `D` (here D=4), the cost per pattern is O(D^4) = 256 operations. With 10 patterns, total time is O(10 * D^4) = O(D^4) for fixed D, and space is O(1) extra beyond the output vector. The function uses only the first element of each input vector; the remaining elements are ignored. The implementation must include headers `<vector>`, `<complex>`, `<Eigen/Dense>` (or `<unsupported/Eigen/CXX11/Tensor>` for tensor support), and `<cstddef>` for `std::size_t`.
